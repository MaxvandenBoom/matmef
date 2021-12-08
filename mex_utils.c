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
