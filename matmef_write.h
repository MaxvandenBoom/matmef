#ifndef MATMEF_WRITE_
#define MATMEF_WRITE_
/**
 * 	@file - headers
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Functions to write data to MEF3 files
 *	
 *  Copyright 2021, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
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


// 
// Functions
//

bool write_metadata(si1 *segment_path, si1 *password_l1, si1 *password_l2, si8 start_time, si8 end_time, si1 *anonymized_name, si4 channelType, mxArray *mat_tmd2, mxArray *mat_md3);
bool write_mef_ts_data_and_indices(si1 *segment_path, si1 *password_l1, si1 *password_l2, ui4 samples_per_block, const mxArray *data, bool lossy_flag);


#endif   // MATMEF_WRITE_