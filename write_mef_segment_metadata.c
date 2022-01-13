/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Write a new time-series or video metadata file for a specific segment
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
#include "matmef_mapping.h"
#include "matmef_write.h"
#include "matmef_utils.h"
#include "mex_utils.h"
#include <ctype.h>


/**
 * Main entry point for 'write_mef_segment_metadata'
 *
 * @param channelType	The type of channel ['timeseries' or 'ts' for time-series, 'v' or 'video' for video]
 * @param channelPath	Path (absolute or relative) a MEF3 channel folder (to be created or existing)
 * @param segmentNum	The segment number, should be 0 or a positive integer (1, 2, ...)
 * @param passwordL1	Level 1 password on the segment metadata; Pass empty string/variable for no encryption
 * @param passwordL2	Level 2 password on the segment metadata; Pass empty string/variable for no encryption
 * @param startTime		The start epoch time in microseconds (μUTC format) to be stored in the universal-header
 * @param endTime		The end epoch time in microseconds (μUTC format) to be stored in the universal-header
 * @param anonName		The anonymized subject name to be stored in the universal-header
 * @param section2		Structure containing section 2 metadata. Make sure the section 2 metadata matches the channel-type
 * @param section3		Structure containing section 3 metadata
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {


	//
	// channel type
	// 
	
	if (nrhs < 1)					mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noChannelType", "'channelType' input argument not set");
	if (!mxIsChar(prhs[0]))			mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidChannelTypeArg", "'channelType' input argument invalid, should be a string (array of characters)");
	char *mat_channel_type = mxArrayToString(prhs[0]);
	for(int i = 0; mat_channel_type[i]; i++)	mat_channel_type[i] = tolower(mat_channel_type[i]);
	if (strcmp(mat_channel_type, "timeseries") != 0 && strcmp(mat_channel_type, "ts") != 0 && strcmp(mat_channel_type, "video") != 0 && strcmp(mat_channel_type, "v") != 0)
		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidChannelTypeArg", "'channelType' input argument invalid, valid values are: 'timeseries', 'ts', 'video', 'v'");
	
	// set the range type
	si4 channel_type = TIME_SERIES_CHANNEL_TYPE;
	if (!strcmp(mat_channel_type, "video") || !strcmp(mat_channel_type, "v"))
		channel_type = VIDEO_CHANNEL_TYPE;
	mxFree(mat_channel_type);


	//
	// channel and segment paths
	// 

	// check the number of input arguments
    if (nrhs < 2)				mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noChannelPathArg", "'channelPath' input argument not set");
	if (nrhs < 3)				mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noSegmentNumPathArg", "'segmentNum' input argument not set");

	si1 channel_path[MEF_FULL_FILE_NAME_BYTES];
	si1 channel_name[MEF_BASE_FILE_NAME_BYTES];
	si1 segment_path[MEF_FULL_FILE_NAME_BYTES];
	int segment_num = 0;
	
	// prepare the channel path input argument, check input and transfer the channel path and name to c-variables
	prep_channel_segment(prhs[1], prhs[2], channel_path, channel_name, &segment_num, segment_path, channel_type);
	
	// build the metadata filepath
	si1 md_filepath[MEF_FULL_FILE_NAME_BYTES];
	if (channel_type == TIME_SERIES_CHANNEL_TYPE)
		sprintf(md_filepath, "%s%c%s-%06d.tmet", segment_path, pathSeparator, channel_name, segment_num);
	else
		sprintf(md_filepath, "%s%c%s-%06d.vmet", segment_path, pathSeparator, channel_name, segment_num);
		
	// check if a metadata file already exists
	if (fileExists(md_filepath))
		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:metadataFileExists", "metadata file '%s' already exists", md_filepath);
	
	// make sure the segment directory exists (creates recursively if it does not)
	if (!createDir(segment_path))
		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidSegmentPath", "segment path '%s' does not exists, or could not be created", segment_path);
	
	
	// 
	// passwords
	// 
	
	si1 password_l1[PASSWORD_BYTES] = {0};
	si1 password_l2[PASSWORD_BYTES] = {0};
	
	// check the level 1 password
	if (nrhs < 4)					mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noPasswordL1Arg", "'passwordL1' input argument not set, pass empty string for no encryption");
	if (!mxIsEmpty(prhs[3])) {
		if (!mxIsChar(prhs[3]))		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidPasswordL1Arg", "'passwordL1' input argument invalid, should be a string (array of characters)");
		
		// convert matlab char-array to UTF-8 character string
		if (!cpyMxStringToUtf8CharString(prhs[3], password_l1, PASSWORD_BYTES))
			mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidPasswordL1Arg", "'passwordL1' input argument invalid, could not convert matlab char array to UTF-8 bytes");
		
	}
	
	// check the level 2 password
	if (nrhs < 5)					mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noPasswordL2Arg", "'passwordL2' input argument not set, pass empty string for no encryption");
	if (!mxIsEmpty(prhs[4])) {
		if (!mxIsChar(prhs[4]))		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidPasswordL2Arg", "'passwordL2' input argument invalid, should be a string (array of characters)");
		
		// convert matlab char-array to UTF-8 character string
		if (!cpyMxStringToUtf8CharString(prhs[4], password_l2, PASSWORD_BYTES))
			mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidPasswordL2Arg", "'passwordL2' input argument invalid, could not convert matlab char array to UTF-8 bytes");
		
	}
	
	// make sure that level 1 is set when level 2 is set
    if (password_l1[0] == '\0' && password_l2[0] != '\0')
		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:level2passWithoutLevel1passArg", "'passwordL2' cannot be set without level 1 password.");
	
	
	// 
	// universal header start-time, end-time and anonymized subject name
	// 
	
	si8 start_time = 0;
	si8 end_time = 0;
	si1 anon_name[UNIVERSAL_HEADER_ANONYMIZED_NAME_BYTES] = {0};
	
	// check start-time and end-time
	if (nrhs < 6)				mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noStartTimeArg", "'startTime' input argument not set");
	if (!getInputArgAsInt64(prhs[5], "startTime", LLONG_MIN, LLONG_MAX, &start_time))	return;
	
	if (nrhs < 7)				mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noEndTimeArg", "'endTime' input argument not set");
	if (!getInputArgAsInt64(prhs[6], "endTime", LLONG_MIN, LLONG_MAX, &end_time))	return;
	
	// check and convert the anonymized name (to a C UTF-8 character string)
	if (nrhs < 8)					mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noAnonNameArg", "'anonName' input argument not set");
	if (!mxIsEmpty(prhs[7])) {
		if (!mxIsChar(prhs[7]))		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidAnonNameArg", "'anonName' input argument invalid, should be a string (array of characters)");
		if (!cpyMxStringToUtf8CharString(prhs[7], anon_name, UNIVERSAL_HEADER_ANONYMIZED_NAME_BYTES))
			mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidAnonNameArg", "'anonName' input argument invalid, could not convert matlab char array to UTF-8 bytes");
	}
	
	
	//
	// sections metadata structs
	//

	// check section 2
	if (nrhs < 9)										mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noSection2Arg", "'section2' input argument not set");
	if (mxIsEmpty(prhs[8]) || !mxIsStruct(prhs[8]))		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidSection2Arg", "'section2' input argument invalid, should be a structure with section 2 metadata fields");
	
	// create and initialize a new section 2 metadata matlab-struct, and transfer
	// the fields from the input-argument section 2 metadata to overwrite the field in the newly created matlab-struct
	mxArray *md2_struct = NULL;
	if (channel_type == TIME_SERIES_CHANNEL_TYPE) {
		
		md2_struct = create_init_matlab_tmd2();
		if (!transferMxFields(prhs[8], md2_struct))
			mexErrMsgTxt("Error while transferring the input time-series section 2 metadata");
		
	} else {
		
		md2_struct = create_init_matlab_vmd2();
		if (!transferMxFields(prhs[8], md2_struct))
			mexErrMsgTxt("Error while transferring the input video section 2 metadata");
		
	}

	// check section 3
	if (nrhs < 10)										mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:noSection3Arg", "'section3' input argument not set");
	if (mxIsEmpty(prhs[9]) || !mxIsStruct(prhs[9]))		mexErrMsgIdAndTxt("MATLAB:write_mef_segment_metadata:invalidSection3Arg", "'section3' input argument invalid, should be a structure with section 3 metadata fields");

	// create and initialize a new section 3 metadata matlab-struct, and transfer
	// the fields from the input-argument section 3 metadata to overwrite the field in the newly created matlab-struct
	mxArray *md3_struct = create_init_matlab_md3();
	if (!transferMxFields(prhs[9], md3_struct)) {
		mexErrMsgTxt("Error while transferring the input section 3 metadata");
	}
	
	
	// 
	// write the metadata
	// 
	if (!write_metadata(segment_path, password_l1, password_l2, start_time, end_time, anon_name, channel_type, md2_struct, md3_struct))
		mexErrMsgTxt("Error while writing metadata to the file");
	
	// succesfull return from call
	return;
	
}
