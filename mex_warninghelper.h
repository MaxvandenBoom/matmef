#ifndef MEX_WARNINGHELPER_
#define MEX_WARNINGHELPER_
/**
 * 	@file - headers
 * 	Functions to ensure warnings are always shown in Matlab
 *	
 *  Copyright 2021, Max van den Boom
 *
 *  
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "mex.h"

void mxForceWarning(const char* warningId, const char *message, ...);
bool mxIsWarningEnabled(const char* warningId);

#endif   // MEX_WARNINGHELPER_