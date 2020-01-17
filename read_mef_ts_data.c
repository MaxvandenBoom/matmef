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
#include "meflib\meflib.h"
#include "matmef_data.h"



/**
 * Main Entry point for 'read_mef_ts_data'
 *
 * @param channel		T
 * @param rangeType		Modality that is used to define the data-range to read [either 'time' or 'samples']
 * @param rangeStart	Start-point for the reading of data (either as a timepoint or samplenumber)
 * @param rangeEnd		End-point to stop the of reading data (either as a timepoint or samplenumber)
 * @return				T
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	si1 *channel_path = "D:\\MatMEF3\\sample_dora\\mef3.mefd\\EKG.timd";
	si1 *password = NULL;
	
	
	//
	// by sample
	//
	
	sf8 sampling_frequency = 250.0;
    si8 range_start = 0;
    si8 range_end = range_start + (10 * sampling_frequency);
    si4 num_samples = range_end - range_start;
	
	
	// 
	// by time
	// 
    //sf8 sampling_frequency = 250.0;
	//si8 range_start = 1533749749914432;        // GMT: 8 August 2018 17:35:49  (beginning of file)
    //si8 range_end = range_start + (10 * 1e6);   // 10 seconds later  (times are specified in microseconds)
    //si4 num_samples = (((double)(range_end - range_start) / 1e6) * sampling_frequency) + 1;  // add one to round up
    

	// 
    // allocate output buffer
	// 
    si4 *data = (si4*)calloc(num_samples, sizeof(si4));
    
	// 
	// read the data
	// 
	mxArray* samples_read = read_channel_data_from_path(channel_path, password, RANGE_BY_SAMPLES, range_start, range_end, data);
	
	// check for error
	if (samples_read == NULL)	mexErrMsgTxt("Error while reading channel data");

	// check if the data should be returned
	if (nlhs > 0) {
		plhs[0] = samples_read;
		
		
	}
	
	// 
	return;
	
}
