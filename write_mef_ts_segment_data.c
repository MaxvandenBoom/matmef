/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Write new time-series data in the specified segment
 *	
 *  Copyright 2022, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
 *	Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)
 *
 *  
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "mex.h"
#include "matmef_dataconverter.h"
#include "matmef_write.h"
#include "matmef_utils.h"
#include "mex_utils.h"
#include <ctype.h>


/**
 * Main entry point for 'write_mef_ts_segment_data'
 *
 * @param channelPath			Path (absolute or relative) to a MEF3 channel folder (to be created or existing)
 * @param segmentNum			The segment number Should be 0 or a positive integer (1, 2, ...)
 * @param passwordL1			Level 1 password on the segment data; Pass empty string/variable for no encryption
 * @param passwordL2			Level 2 password on the segment data; Pass empty string/variable for no encryption
 * @param samplesPerMefBlock	Number of samples per MEF3 block
 * @param data					The data to write as a 1-D array of data-type int32
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	//
	// channel and segment paths
	// 

	// check the number of input arguments
    if (nrhs < 1)				mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:noChannelPathArg", "'channelPath' input argument not set");
	if (nrhs < 2)				mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:noSegmentNumPathArg", "'segmentNum' input argument not set");

	si1 channel_path[MEF_FULL_FILE_NAME_BYTES];
	si1 channel_name[MEF_BASE_FILE_NAME_BYTES];
	si1 segment_path[MEF_FULL_FILE_NAME_BYTES];
	int segment_num = 0;
	
	// prepare the channel path input argument, check input and transfer the channel path and name to c-variables
	prep_channel_segment(prhs[0], prhs[1], channel_path, channel_name, &segment_num, segment_path, TIME_SERIES_CHANNEL_TYPE);
	
	// build the time-series metadata filepath
	si1 tmet_path[MEF_FULL_FILE_NAME_BYTES];
	si1 tdat_path[MEF_FULL_FILE_NAME_BYTES];
	sprintf(tmet_path, "%s%c%s-%06d.tmet", segment_path, pathSeparator, channel_name, segment_num);
	sprintf(tdat_path, "%s%c%s-%06d.tdat", segment_path, pathSeparator, channel_name, segment_num);

	// check if a data file already exists
	if (fileExists(tdat_path))
		mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:dataFileExists", "Data file '%s' already exists", tdat_path);
	
	// check if a metadata file exists (should be written before), this also ensures that the directory exists
	if (!fileExists(tmet_path))
		mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:metadataFileDoesNotExists", "Metadata file '%s' does not exists, write the metadata file before writing data", tmet_path);
	
	
	// 
	// passwords
	// 
	
	si1 password_l1[PASSWORD_BYTES] = {0};
	si1 password_l2[PASSWORD_BYTES] = {0};
	
	// check the level 1 password
	if (nrhs < 3)					mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:noPasswordL1Arg", "'passwordL1' input argument not set, pass empty string for no encryption");
	if (!mxIsEmpty(prhs[2])) {
		if (!mxIsChar(prhs[2]))		mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:invalidPasswordL1Arg", "'passwordL1' input argument invalid, should be a string (array of characters)");
		
		// convert matlab char-array to UTF-8 character string
		if (!cpyMxStringToUtf8CharString(prhs[2], password_l1, PASSWORD_BYTES))
			mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:invalidPasswordL1Arg", "'passwordL1' input argument invalid, could not convert matlab char array to UTF-8 bytes");
		
	}
	
	// check the level 2 password
	if (nrhs < 4)					mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:noPasswordL2Arg", "'passwordL2' input argument not set, pass empty string for no encryption");
	if (!mxIsEmpty(prhs[3])) {
		if (!mxIsChar(prhs[3]))		mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:invalidPasswordL2Arg", "'passwordL2' input argument invalid, should be a string (array of characters)");
		
		// convert matlab char-array to UTF-8 character string
		if (!cpyMxStringToUtf8CharString(prhs[3], password_l2, PASSWORD_BYTES))
			mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:invalidPasswordL2Arg", "'passwordL2' input argument invalid, could not convert matlab char array to UTF-8 bytes");
		
	}	
	
	// make sure that level 1 is set when level 2 is set
    if (password_l1[0] == '\0' && password_l2[0] != '\0')
		mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_metadata:level2passWithoutLevel1passArg", "level 2 password cannot be set without level 1 password.");
	
	
	// 
	// Number of samples per block (limit to a smaller number because the samples per block variable is actually of the ui4 data-type)
	// 
	
	si8 samples_per_block = -1;
	if (nrhs < 5)				mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:noSamplesPerBlockArg", "'samplesPerBlock' input argument not set");
	if (!getInputArgAsInt64(prhs[4], "samplesPerBlock", 1, 4294967295, &samples_per_block))	return;
	
	
	//
	// Data
	//
	
	if (nrhs < 6)								mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:noDataArg", "'data' input argument not set");
	if (mxIsEmpty(prhs[5]))						mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:invalidDataArg", "'data' input argument is empty");
	if (!mxIsNumeric(prhs[5])) 					mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:invalidDataArg", "'data' input argument is not numeric, should be an vector of int32 values");
	if (mxGetClassID(prhs[5]) != mxINT32_CLASS) mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:invalidDataArg", "'data' input argument has data as '%s', should be an vector of int32 values", mxGetClassName(prhs[5]));
	if (mxGetNumberOfDimensions(prhs[5]) > 2) 	mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:invalidDataArg", "'data' input argument has too many dimensions, should be an vector of N-x-1 int32 values");
	
	// check the size of the dimensions
	const mwSize *dims = mxGetDimensions(prhs[5]);
	if (dims[1] != 1) 							mexErrMsgIdAndTxt("MATLAB:write_mef_ts_segment_data:invalidDataArg", "'data' input argument does not have the right dimensions, should be a vector of N-x-1 int32 values");
	// TODO: check other dimension, if there are enough samples
	
	// 
	// write the data
	// (# lossy compression flag - not used)
	// 
	bool lossy_flag = false;
	if (!write_mef_ts_data_and_indices(segment_path, password_l1, password_l2, (ui4)samples_per_block, prhs[5], lossy_flag))
		mexErrMsgTxt("Error while writing time-series data");
	
	return;
	
}
