/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Read the MEF3 data from a time-series channel
 *	
 *  Copyright 2020, Max van den Boom and Mayo Clinic (Rochester MN)
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
#include "meflib/meflib/meflib.h"
#include "matmef_data.h"



/**
 * Main entry point for 'read_mef_ts_data'
 *
 * @param channelPath	path (absolute or relative) to the MEF3 channel folder
 * @param password		Password to the MEF3 data; pass an empty string if no password should be used
 * @param rangeType		Modality that is used to define the data-range to read [either 'time' or 'samples']
 * @param rangeStart	Start-point for the reading of data (either as a timepoint or samplenumber)
 * @param rangeEnd		End-point to stop the of reading data (either as a timepoint or samplenumber)
 * @return				A vector of doubles holding the channel data
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {


	// check the channel path input argument
    if(nrhs < 1) {
        mexErrMsgIdAndTxt( "MATLAB:read_mef_ts_data:noChannelPathArg", "channelPath input argument not set");
	} else {
		if(!mxIsChar(prhs[0])) {
			mexErrMsgIdAndTxt( "MATLAB:read_mef_ts_data:invalidChannelPathArg", "channelPath input argument invalid, should string (array of characters)");
		}
		if(mxIsEmpty(prhs[0])) {
			mexErrMsgIdAndTxt( "MATLAB:read_mef_ts_data:invalidChannelPathArg", "channelPath input argument invalid, argument is empty");
		}
	}
	
	// set the channel path
	si1 channel_path[MEF_FULL_FILE_NAME_BYTES];
	char *mat_channel_path = mxArrayToString(prhs[0]);
	MEF_strncpy(channel_path, mat_channel_path, MEF_FULL_FILE_NAME_BYTES);



	



	si1 *password = NULL;
	
	
	//
	// by sample
	//
	
	sf8 sampling_frequency = 250.0;
    si8 range_start = 0;
    si8 range_end = range_start + (10 * sampling_frequency);
	//si8 range_end = -1;
	
	
	// 
	// by time
	// 
    //sf8 sampling_frequency = 250.0;
	//si8 range_start = 1533749749914432;        // GMT: 8 August 2018 17:35:49  (beginning of file)
    //si8 range_end = range_start + (10 * 1e6);   // 10 seconds later  (times are specified in microseconds)
    

	// 
	// read the data
	// 
	mxArray* data = read_channel_data_from_path(channel_path, password, RANGE_BY_SAMPLES, range_start, range_end);
	
	// check for error
	if (data == NULL)	mexErrMsgTxt("Error while reading channel data");

	// check if the data should be returned
	if (nlhs > 0) {
		plhs[0] = data;
		
		
	}
	
	// 
	return;
	
}
