/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Read the MEF3 data from a time-series channel
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
#include <ctype.h>
#include "mex.h"
#include "matmef_dataconverter.h"
#include "matmef_read.h"


/**
 * Main entry point for 'read_mef_ts_data'
 *
 * @param channelPath       Path (absolute or relative) to the MEF3 channel folder
 * @param password          Password to the MEF3 data; Pass empty string/variable if not encrypted
 * @param rangeType         Modality that is used to define the data-range to read [either 'time' or 'samples' (default)]
 * @param rangeStart        Start-point for the reading of data (0-based; either as an epoch/unix timestamp or samplenumber; -1 for beginning/first)
 * @param rangeEnd          End-point to stop the of reading data (0-based; either as an epoch/unix timestamp or samplenumber; -1 for end/last)
 * @param applyConvFactor   Whether to apply the unit conversion factor to the raw data. [0 = not apply (default), 1 = apply]
 * @return                  A vector of doubles holding the channel data
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	
	//
	// channel path
	// 
	
	// check the channel path input argument
    if (nrhs < 1)				mexErrMsgIdAndTxt("MATLAB:read_mef_ts_data:noChannelPathArg", "'channelPath' input argument not set");
	if(!mxIsChar(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:read_mef_ts_data:invalidChannelPathArg", "'channelPath' input argument invalid, should be a string (array of characters)");
	if(mxIsEmpty(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:read_mef_ts_data:invalidChannelPathArg", "'channelPath' input argument invalid, argument is empty");
	
	// set the channel path
	si1 channel_path[MEF_FULL_FILE_NAME_BYTES];
	char *mat_channel_path = mxArrayToString(prhs[0]);
	MEF_strncpy(channel_path, mat_channel_path, MEF_FULL_FILE_NAME_BYTES);
	mxFree(mat_channel_path);


	// 
	// password (optional)
	// 
	
	si1 password[PASSWORD_BYTES] = {0};
	
	// check if a password input argument is given and is not empty
    if (nrhs > 1 && !mxIsEmpty(prhs[1])) {
	
		// check the password input argument data type
		if (!mxIsChar(prhs[1]))
			mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidPasswordArg", "'password' input argument invalid, should be a string (array of characters)");

		// convert password (matlab char-array to UTF-8 character string)
		if (!cpyMxStringToUtf8CharString(prhs[1], password, PASSWORD_BYTES))
			mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidPasswordArg", "'password' input argument invalid, could not convert matlab char-array to UTF-8 bytes");

	}
	
	
	//
	// range
	//
	
	bool range_type = RANGE_BY_SAMPLES;
	si8 range_start = -1;
	si8 range_end = -1;
	
	// check if a range-type input argument is given
    if (nrhs > 2) {
		
		// check valid range type
		if (!mxIsChar(prhs[2]))
			mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidRangeTypeArg", "'rangeType' input argument invalid, should be a string (array of characters)");
		char *mat_range_type = mxArrayToString(prhs[2]);
		for(int i = 0; mat_range_type[i]; i++)	mat_range_type[i] = tolower(mat_range_type[i]);
		if (strcmp(mat_range_type, "time") != 0 && strcmp(mat_range_type, "samples") != 0)
			mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidRangeTypeArg", "'rangeType' input argument invalid, allowed values are 'time' or 'samples'");
		
		// set the range type
		if (strcmp(mat_range_type, "time") == 0)
			range_type = RANGE_BY_TIME;
		mxFree(mat_range_type);
		
		// check and retrieve a range-start input
		if (nrhs > 3)
			if (!getInputArgAsInt64(prhs[3], "rangeStart", -1, LLONG_MAX, &range_start))	return;
		
		// check and retrieve a range-end input
		if (nrhs > 4)
			if (!getInputArgAsInt64(prhs[4], "rangeEnd", -1, LLONG_MAX, &range_end))	return;
		
	}
	
    
    //
    // Conversion factor
    //
    
    bool apply_conv_factor = false;
	if (nrhs > 5) {
		if (!getInputArgAsBool(prhs[5], "applyConvFactor", &apply_conv_factor))	return;
    }
    
	
	// 
	// read the data
	// 
	mxArray *data = read_channel_data_from_path(channel_path, password, range_type, range_start, range_end, apply_conv_factor);
	if (data == NULL)	
		mexErrMsgTxt("Error while reading channel data");
    
	// set the data as output, if output is expected
	if (nlhs > 0)
		plhs[0] = data;
	
	// succesfull return from call
	return;
	
}
