/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Read a MEF3 folder and retrieve the session, channel(s), segment(s) and record(s) metadata
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

#include "meflib/meflib/meflib.c"
#include "meflib/meflib/mefrec.c"


/**
 * Main entry point for 'read_mef_session_metadata'
 *
 * @param sessionPath	Path (absolute or relative) to the MEF3 session folder
 * @param password		Password to the MEF3 data; Pass empty string/variable if not encrypted
 * @param readIndices	Whether to read and map time-series and video indices [0 or 1; default is 0]
 * @param readRecords	Whether to read the records [0 or 1; default is 1]
 * @return				Structure containing session metadata, channels metadata, segments metadata and records
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	//
	// session path
	// 
	
	// check the session path input argument
    if (nrhs < 1)				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:noSessionPathArg", "'sessionPath' input argument not set");
	if (!mxIsChar(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidSessionPathArg", "'sessionPath' input argument invalid, should be a string (array of characters)");
	if (mxIsEmpty(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidSessionPathArg", "'sessionPath' input argument invalid, argument is empty");
	
	// set the session path
	si1 session_path[MEF_FULL_FILE_NAME_BYTES];
	char *mat_session_path = mxArrayToString(prhs[0]);
	MEF_strncpy(session_path, mat_session_path, MEF_FULL_FILE_NAME_BYTES);
	mxFree(mat_session_path);
	
	// 
	// password (optional)
	// 
	
	si1 password[PASSWORD_BYTES] = {0};
	
	// check if a password input argument is given
    if (nrhs > 1) {

		// note: if the password passed to any of the meflib read function is an empty string, than 
		//		 the 'process_password_data' function in 'meflib.c' will crash everything, so make
		// 		 sure it is either NULL or a string with at least one character

		// check if the password input argument is not empty
		if (!mxIsEmpty(prhs[1])) {
			
			// check the password input argument data type
			if (!mxIsChar(prhs[1]))
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidPasswordArg", "'password' input argument invalid, should be a string (array of characters)");

			// convert password (matlab char-array to UTF-8 character string)
			if (!cpyMxStringToUtf8CharString(prhs[1], password, PASSWORD_BYTES))
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidPasswordArg", "'password' input argument invalid, could not convert matlab char-array to UTF-8 bytes");

		}
		
	}
	
	
	// 
	// read time-series and video indices (optional)
	// 
	
	// Read indices flag
    si1 read_indices_flag = 0;	
	
	// check if a read (ts/video) indices input argument is given
    if (nrhs > 2) {
		if (!mxIsEmpty(prhs[2])) {
			
			if ((!mxIsNumeric(prhs[2]) && !mxIsLogical(prhs[2])) || mxGetNumberOfElements(prhs[2]) > 1)
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidReadIndicesArg", "'readIndices' input argument invalid, should be a single value logical or numeric");
			
			// retrieve the map indices flag value
			double mat_read_indices_flag = mxGetScalar(prhs[2]);
			if (mat_read_indices_flag != 0 && mat_read_indices_flag != 1)
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidReadIndicesArg", "'readIndices' input argument invalid, allowed values are 0, false, 1 or true");
			
			// set the flag
			read_indices_flag = mat_read_indices_flag;
			
		}
	}

	
	// 
	// read records (optional)
	// 
	
	// Read indices flag
    si1 read_records_flag = 1;
	
	// check if a read records input argument is given
    if (nrhs > 3) {
		if (!mxIsEmpty(prhs[3])) {
			
			if ((!mxIsNumeric(prhs[3]) && !mxIsLogical(prhs[3])) || mxGetNumberOfElements(prhs[3]) > 1)
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidReadRecordsArg", "'readRecords' input argument invalid, should be a single value logical or numeric");
			
			// retrieve the read records flag value
			double mat_read_records_flag = mxGetScalar(prhs[3]);
			if (mat_read_records_flag != 0 && mat_read_records_flag != 1)
				mexErrMsgIdAndTxt("MATLAB:read_mef_session_metadata:invalidReadRecordsArg", "'readRecords' input argument invalid, allowed values are 0, false, 1 or true");
			
			// set the flag
			read_records_flag = mat_read_records_flag;
			
		}
	}
	
	
	//
	// read session metadata
	//
	
    // initialize MEF library
	initialize_meflib();

	// read the session metadata
	MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
	if (!read_indices_flag) {
		MEF_globals->read_time_series_indices   = 0;
		MEF_globals->read_video_indices         = 0;
	}
	if (!read_records_flag)
		MEF_globals->read_record_indices        = 0;
	SESSION *session = read_MEF_session(	NULL, 															// allocate new session object
											session_path, 													// session filepath
											(password[0] == '\0') ? NULL : password,                        // password
											NULL, 															// empty password
											MEF_FALSE, 														// do not read time series data
											read_records_flag												// read record data
										);
	MEF_globals->behavior_on_fail = EXIT_ON_FAIL;
	
	// check for error
	if (session == NULL)	mexErrMsgTxt("Error while reading session metadata");
	
	// check if the data is encrypted and/or the correctness of password
	if (session->time_series_metadata.section_1 != NULL) {
		if (session->time_series_metadata.section_1->section_2_encryption > 0 || session->time_series_metadata.section_1->section_2_encryption > 0) {
			free_session(session, MEF_TRUE);
			if (password[0] == '\0')
				mexErrMsgTxt("Error: data is encrypted, but no password is given, exiting...");
			else
				mexErrMsgTxt("Error: wrong password for encrypted data, exiting...");
			
		}
	}
	if (session->video_metadata.section_1 != NULL) {
		if (session->video_metadata.section_1->section_2_encryption > 0 || session->video_metadata.section_1->section_2_encryption > 0) {
			free_session(session, MEF_TRUE);
			if (password[0] == '\0')
				mexErrMsgTxt("Error: data is encrypted, but no password is given, exiting...");
			else
				mexErrMsgTxt("Error: wrong password for encrypted data, exiting...");
		}
	}
	
	// check if a session-struct should be returned
	if (nlhs > 0) {
		
		// map session object to matlab output struct and assign to the first return argument
		// Note: if the indices are read then they may also be mapped (2nd argument in the call) and vice versa
		plhs[0] = map_mef3_session(session, read_indices_flag);
		
	}		
	
	// free the session memory
	free_session(session, MEF_TRUE);
	
	// 
	return;
	
}

