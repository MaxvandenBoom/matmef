/**
 * 	@file
 * 	Matmef specific utility functions
 *	
 *  Copyright 2022, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
 *
 *  
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <ctype.h>
#include "matmef_utils.h"
#include "mex_utils.h"


/**
 * 	Prepares the channel path and (optionally) segment path
 *  Checks the input, transfers the channel path and name to c-variables, and (optionally) builds the segment path
 * 	
 *  Note: calls mexErrMsgIdAndTxt on error, so no need to return succes or failure
 * 
 * 	@param mxChannelPath        Path (absolute or relative) to a MEF3 channel folder
 *  @param mxSegmentNum			The segment number, should be 0 or a positive integer (1, 2, ...); NULL to skip segment
 * 	@param channel_path         Char-array to transfer the channel path into
 * 	@param channel_name         Char-array to transfer the channel name into
 * 	@param segment_num         	Pointer to an c- variable to store the segment number as an integer into; NULL to skip segment
 * 	@param segment_path         Char-array to build the segment path into; NULL to skip segment
 * 	@param inputChannelType     The type of channel [either TIME_SERIES_CHANNEL_TYPE or VIDEO_CHANNEL_TYPE]
 */
void prep_channel_segment(const mxArray *mxChannelPath, const mxArray *mxSegmentNum, si1 *channel_path, si1 *channel_name, int *segment_num, si1 *segment_path, si4 inputChannelType) {

	//
	// channel path
	//
	
	if (mxIsEmpty(mxChannelPath))		mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidChannelPathArg", "'channelPath' input argument invalid, argument is empty");
	if (!mxIsChar(mxChannelPath))		mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidChannelPathArg", "'channelPath' input argument invalid, should be a string (array of characters)");
	
	// set the channel path
	MEF_strncpy(channel_path, mxArrayToString(mxChannelPath), MEF_FULL_FILE_NAME_BYTES);
	
	// remove trailing slash
	size_t channel_path_len = strlen(channel_path);
	if(channel_path[channel_path_len - 1] == '\\' || channel_path[channel_path_len - 1] == '/') {
		channel_path[channel_path_len - 1] = 0;
		channel_path_len--;
	}	
	
	// check channel path ending
	// Note:   this check is only performed in Matmef writing operations because the channel name
	//	       needs to be extracted from the channel-path for later use (in the segment pathname)
	// Note 2: we are deliberately not using the meflib 'channel_type_from_path' function, because
	//         here we also want to make sure we are pointing to a channel directory and not
	//         further beyond that (e.g. segment)
	if (channel_path_len <= 5 || tolower(channel_path[channel_path_len - 5]) != '.' ||
		(inputChannelType == TIME_SERIES_CHANNEL_TYPE && (	tolower(channel_path[channel_path_len - 4]) != TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING[0] ||
															tolower(channel_path[channel_path_len - 3]) != TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING[1] || 
															tolower(channel_path[channel_path_len - 2]) != TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING[2] || 
															tolower(channel_path[channel_path_len - 1]) != TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING[3])) ||
		(inputChannelType == VIDEO_CHANNEL_TYPE 	  && (	tolower(channel_path[channel_path_len - 4]) != VIDEO_CHANNEL_DIRECTORY_TYPE_STRING[0] ||
															tolower(channel_path[channel_path_len - 3]) != VIDEO_CHANNEL_DIRECTORY_TYPE_STRING[1] || 
															tolower(channel_path[channel_path_len - 2]) != VIDEO_CHANNEL_DIRECTORY_TYPE_STRING[2] || 
															tolower(channel_path[channel_path_len - 1]) != VIDEO_CHANNEL_DIRECTORY_TYPE_STRING[3]))		) {
		
		// error, wrong directory
		if (inputChannelType == TIME_SERIES_CHANNEL_TYPE)
			mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidChannelPathArg", "'channelPath' input argument invalid, the channel path should point to a time-series channel directory and therefore end with .timd");
		else
			mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidChannelPathArg", "'channelPath' input argument invalid, the channel path should point to a video channel directory and therefore end with .vidd");
		
	}
	
	// retrieve the channel name from the path (assume relative path if slash is not found)
	char *				pch = strrchr_sep(channel_path);
	if (pch != NULL)	pch++;	else 	pch = channel_path;
	memcpy(channel_name, pch, channel_path_len - (pch - channel_path) - 5);
	channel_name[channel_path_len - (pch - channel_path) - 5] = '\0';
	
	
	// 
	// segment number and build segment path
	//
	
	if (mxSegmentNum != NULL && segment_num != NULL && segment_path != NULL) {

		// check input and transfer the segment number to an integer variable
		if (!mxIsScalar(mxSegmentNum))	mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidSegmentNumArg", "'segmentNum' input argument invalid, should be 0 or a positive integer (1, 2, ...)");
		
		double dbl_segment_num = mxGetScalar(mxSegmentNum);
		if (mxIsNaN(dbl_segment_num) || mxIsInf(dbl_segment_num) || floor(dbl_segment_num) != dbl_segment_num || dbl_segment_num < 0)
			mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidSegmentNumArg", "'segmentNum' input argument invalid, should be 0 or a positive integer (1, 2, ...)");
		if (dbl_segment_num > 999999)
			mexErrMsgIdAndTxt("MATLAB:prep_channel_segment:invalidSegmentNumArg", "'segmentNum' input argument invalid, the segment number has a limit of 6 digits therefore the highest possible segment number is 999999");
		
		*segment_num = (int)dbl_segment_num;
		
		// build segment path
		sprintf(segment_path, "%s%c%s-%06d.segd", channel_path, pathSeparator, channel_name, *segment_num);
		
	}
	
}

si4 extract_segment_number(si1 *segment_name) {
    si1     *c;
    si4     segment_number;

    // move pointer to the end of the string
    c = segment_name + strlen(segment_name) - 1;

    // Get to the dash
    while(*--c == '-') {
        if (*c == '/') {
			mexPrintf("Error: Segment name not in valid form XXX-000000\n");
            return -1;
        }
    }
    c++;

    segment_number = (si4) strtol(c, NULL, 10);
    return segment_number;
	
}