/**
 * 	@file
 * 	General utility functions
 *	
 *  Copyright 2021, Max van den Boom
 *	mxIsWarningEnabled function by Pavel Holoborodko (https://undocumentedmatlab.com/articles/checking-status-of-warning-messages-in-mex)
 *
 *  
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "mex_utils.h"
#include <stdarg.h>
#include <string.h>


const char pathSeparator =
#ifdef _WIN32
	'\\';
#else
	'/';
#endif

/**
 * 	Check the given path exists and is a directory
 *
 * 	@param path            		The path to the directory that need to be checked
 * 	@return						True if the path exists and is a directory, false if not
 */
bool dirExists(const char* path) {
	struct stat	sb;
	
#ifndef _WIN32
	return stat(path, &sb) == 0 && sb.st_mode & S_IFDIR;
#else
	return _stat(path, &sb) == 0 && sb.st_mode & _S_IFDIR;
#endif

}

/**
 * 	Check the given path exists and is a file
 *
 * 	@param path            		The path to the file that need to be checked
 * 	@return						True if the path exists and is a file, false if not
 */
bool fileExists(const char* path) {
	
#ifndef _WIN32
	struct stat	sb;
	return stat(path, &sb) == 0 && !(sb.st_mode & S_IFDIR);
#else
	struct _stat64 sb64;
	return _stat64(path, &sb64) == 0 && !(sb64.st_mode & _S_IFDIR);
#endif
	
}

/**
 * 	Search for a directory seperator character ('/' or '\\')
 *
 * 	@param str            		The string to be searched
 * 	@return						Pointer to the first occurrence of character, null pointer if not found
 */
char *strchr_sep(const char *str) {
	char *pch1 = strchr(str, '/');
	char *pch2 = strchr(str, '\\');
	if (pch1 != NULL && pch2 == NULL)	return pch1;
	if (pch1 == NULL && pch2 != NULL)	return pch2;
	if (pch1 != NULL && pch2 != NULL) {
		if (pch1 < pch2)	return pch1;
		else				return pch2;
	}
	return NULL;
}

/**
 * 	Search for a directory seperator character ('/' or '\\'). Starting at the back of the string, searching forward
 *
 * 	@param str            		The string to be searched
 * 	@return						Pointer to the last occurrence of character, null pointer if not found
 */
char *strrchr_sep(const char *str) {
	char *pch1 = strrchr(str, '/');
	char *pch2 = strrchr(str, '\\');
	if (pch1 != NULL && pch2 == NULL)	return pch1;
	if (pch1 == NULL && pch2 != NULL)	return pch2;
	if (pch1 != NULL && pch2 != NULL) {
		if (pch1 < pch2)	return pch2;
		else				return pch1;
	}
	return NULL;
}

bool createDir(const char *path) {
	
	// check if there is a parent path
	char *p = strrchr_sep(path);
	if (p == NULL)	return true;
	
	// skip empty (but not if we are on the root)
	if (p != path && (*(p - 1) == '\\' || *(p - 1) == '/'))
		p--;
	
	// extract the parent directory
	char *temp = calloc(1, strlen(path) + 1);
	memcpy(temp, path, p - path);
	temp[p - path] = '\0';
	
	// check if there is a parent path to check
	if (strlen(temp) > 0 && *(p - 1) != ':') {
	
		// recursively check directory
		if (!createDir(temp)) {
			
			// something went wrong, propegate failure upward
			free(temp);
			return false;
			
		}
	}
	
	// free memory
	free(temp);
	
	
	// 
	// safe to handle the current directory
	// (the parent directory is either at root, directory already existed or has been created succesfully)
	// 
	
	// check if the current directory exists
	if (dirExists(path))
		return true;
	
	// create current directory
	#ifdef _WIN32
		if (_mkdir(path, 0774) != 0)
	#else
		if (mkdir(path, 0774) != 0)
	#endif
			return false;
	
	// return succes
	return true;
	
	
}

/**
 * 	Output a warning, regardless of whether warnings
 *  are enabled or disabled in matlab
 *
 * 	@param warningId            The warningId
 * 	@param message              The warning message
 */
void mxForceWarning(const char* warningId, const char *message, ...) {
    
    // retrieve the warning text buffer (incl parsing)
    char buffer[1024];
    va_list args;
    va_start(args, message);
    vsnprintf(buffer, sizeof(buffer), message, args);
    va_end (args);
    
    // print message in matlab
    if (mxIsWarningEnabled(warningId))
        mexWarnMsgIdAndTxt(warningId, buffer);
    else
        mexPrintf("%s%s%s", "Warning: ", buffer, "\n");
    
}

/**
 * 	Check whether warnings are turned off
 *  (written by Pavel Holoborodko)
 *
 * 	@param warningId            The warningId to check for
 * 	@return						True if warnings are enabled, false if disabled
 */
bool mxIsWarningEnabled(const char* warningId) {
    bool enabled = true;
    if (NULL != warningId) {
        mxArray *mxCommandResponse = NULL, *mxException = NULL;
        mxArray *args[2];
        args[0] = mxCreateString("query");
        args[1] = mxCreateString(warningId);
        mxException = mexCallMATLABWithTrap(1,&mxCommandResponse,2,args,"warning");
        if (NULL == mxException && NULL != mxCommandResponse) {
            if (mxIsStruct(mxCommandResponse)) {
                const mxArray* state_field = mxGetField(mxCommandResponse, 0, "state");
                if (mxIsChar(state_field)) {
                    char state_value[8] = {0};
                    enabled = (mxGetString(state_field, state_value, 8) == 0) &&
                              (strcmp(state_value, "on") == 0);
                }
            }
            mxDestroyArray(mxCommandResponse);
        } else {
            mxDestroyArray(mxException);
        }
        mxDestroyArray(args[0]);
        mxDestroyArray(args[1]);
    }
    return enabled;
}
