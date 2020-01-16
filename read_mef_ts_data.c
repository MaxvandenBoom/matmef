/**
 * 	@file 
 * 	MEF 3.0 Library Matlab Wrapper
 * 	...
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



/**
 * Main Entry point for 'read_mef_ts_data'
 *
 * @param channel		T
 * @param ostart		T
 * @param oend			T
 * @return				T
 */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[]) {

	
    // initialize MEF library
	initialize_meflib();

	
	// 
	return;
	
}

