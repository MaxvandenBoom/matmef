#ifndef MEX_UTILS_
#define MEX_UTILS_
/**
 * 	@file - headers
 * 	General utility functions
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
#include "meflib/meflib/meflib.h"

extern const char pathSeparator;

bool dirExists(const char* path);
bool fileExists(const char* path);
char *strchr_sep(const char *str);
char *strrchr_sep(const char *str);
bool createDir(const char *path);

void mxForceWarning(const char* warningId, const char *message, ...);
bool mxIsWarningEnabled(const char* warningId);

#endif   // MEX_UTILS_