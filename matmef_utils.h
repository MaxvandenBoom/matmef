#ifndef MATMEF_UTILS_
#define MATMEF_UTILS_
/**
 * 	@file - headers
 * 	Matmef specific utility functions
 *	
 *  Copyright 2021, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
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

void prep_channel_segment(const mxArray *mxChannelPath, const mxArray *mxSegmentNum, si1 *channel_path, si1 *channel_name, int *segment_num, si1 *segment_path, si4 inputChannelType);

si4 extract_segment_number(si1 *segment_name);

#endif   // MATMEF_UTILS_