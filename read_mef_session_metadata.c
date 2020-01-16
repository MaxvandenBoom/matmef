/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Read a MEF3 folder and retrieve the session, channel(s), segment(s) and record(s) metadata
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
#include "matmef_mapping.h"



/**
 * Main Entry point for 'read_mef_session_metadata'
 *
 * @param sessionPath	T
 * @param password		T
 * @param mapIndices	T
 * @return				T
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	// 
    SESSION *session;
    //si1     session_path[MEF_FULL_FILE_NAME_BYTES];
	//si1 	*password = NULL;
    
	// check input arguments
	/*
    if(nrhs!=1)
        mexErrMsgIdAndTxt( "MATLAB:read_mef_session_metadata:invalidNumInputs",
                "One input required.");
    else if(nlhs > 1)
        mexErrMsgIdAndTxt( "MATLAB:read_mef_session_metadata:maxlhs",
                "Too many output arguments.");
    else if(!mxIsStruct(prhs[0]))
        mexErrMsgIdAndTxt( "MATLAB:read_mef_session_metadata:inputNotStruct",
                "Input must be a structure.");
		
	*/		


	/*
	// TODO: 
	
	if (argc != 3) {
		fprintf(stderr, "\n\t%cUSAGE: %s session_directory password\n\n", 7, argv[0]);
		return(1);
	}
	session_directory = argv[1];
	password = argv[2];
	*/
    // Read indices flag
    si1 map_indices_flag = 1;
	
	
    // initialize MEF library
	initialize_meflib();

	//si1* dir = "D:\\MatMEF3\\sample_dora\\mef3.mefd"; 
	//MEF_strncpy(session_path, "D:\\MatMEF3\\sample_dora\\mef3.mefd", MEF_FULL_FILE_NAME_BYTES);
	si1 *session_path = "D:\\MatMEF3\\sample_dora\\mef3.mefd";
	si1 *password = NULL;
	
	// 
	session = read_MEF_session(	NULL, 					// allocate new session object
								session_path, 			// session filepath
								password, 				// password
								NULL, 					// empty password
								MEF_FALSE, 				// do not read time series data
								MEF_TRUE				// read record data
								);
	
	// check if a session-struct should be returned
	if (nlhs > 0) {
		
		// map session object to matlab output struct
		// and assign to the first return argument
		plhs[0] = map_mef3_session(session, map_indices_flag);
		
	}		
	
	// 
	return;
	
}

