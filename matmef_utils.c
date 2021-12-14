/**
 * 	@file
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
#include <ctype.h>
#include "matmef_utils.h"
#include "mex_utils.h"

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