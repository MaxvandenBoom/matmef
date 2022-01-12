/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Create and initialize a new universal-header or metadata structure-array with default values
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
#include "matmef_mapping.h"


/**
 * Main entry point for 'init_mef_struct'
 *
 * @param type         	The type of struct to generate [either 'uh', 'ts_section2', 'tmd2', 'v_section2', 'vmd2', 'section3' or 'md3']
 * @return				A new and initialized matlab structure-array
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {
	
	// check the type input argument
    if (nrhs < 1)				mexErrMsgIdAndTxt("MATLAB:init_mef_struct:noTypeArg", "'type' input argument not set");
	if (mxIsEmpty(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:init_mef_struct:invalidTypeArg", "'type' input argument invalid, argument is empty");
	if (!mxIsChar(prhs[0]))		mexErrMsgIdAndTxt("MATLAB:init_mef_struct:invalidTypeArg", "'type' input argument invalid, should be a string (array of characters)");

	// retrieve the type input string and make lower case
	char *mat_type = mxArrayToString(prhs[0]);
	for(int i = 0; mat_type[i]; i++)	mat_type[i] = tolower(mat_type[i]);
	
	// check if the input type string matches any of the options
	if (strcmp(mat_type, "uh") == 0) {
		// create and initialize a universal-header matlab-struct
		
		plhs[0] = create_init_matlab_uh();		
		
		
	} else if (strcmp(mat_type, "ts_section2") == 0 || strcmp(mat_type, "tmd2") == 0) {
		// create and initialize a time-series section 2 metadata matlab-struct
		
		plhs[0] = create_init_matlab_tmd2();		
		
		
	} else if (strcmp(mat_type, "v_section2") == 0 || strcmp(mat_type, "vmd2") == 0) {
		// create and initialize a video section 2 metadata matlab-struct
		
		plhs[0] = create_init_matlab_vmd2();


	} else if (strcmp(mat_type, "section3") == 0 || strcmp(mat_type, "md3") == 0) {
		// create and initialize a section 3 metadata matlab-struct
		
		plhs[0] = create_init_matlab_md3();
		
		
	} else {
		// invalid type
		
		mexErrMsgIdAndTxt("MATLAB:init_mef_struct:invalidTypeArg", "'type' input argument invalid. Valid arguments are: 'uh', 'ts_section2', 'tmd2', 'v_section2', 'vmd2', 'section3' or 'md3'");
	
	}
	
	return;
	
}
