/**
 * 	@file
 * 	Functions to convert primitive c-datatypes to matlab primitive (1x1) arrays/matrices and vice versa
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
#include "matmef_dataconverter.h"
#include "mex.h"
#include "meflib/meflib/meflib.h"



//
// Functions that create matlab-array(s) based on C data values/types
// 

/**
 * Create a (1x1 real) Uint8 matrix based on a MEF ui1 (unsigned 1 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxUint8ByValue(ui1 value) {
	
	// create the matlab variable (1x1 real uint8 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT8_CLASS, mxREAL);
	mxUint8 *data = (mxUint8 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxUint8)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Int8 matrix based on a MEF si1 (signed 1 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxInt8ByValue(si1 value) {
	
	// create the matlab variable (1x1 real int8 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT8_CLASS, mxREAL);
	mxInt8 *data = (mxInt8 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxInt8)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1xN real) Uint8 vector/matrix based on a MEF array of ui1 (unsigned 1 byte int) values
 * (contributed by Richard J. Cui, 4 apr 2020; updates by Max, 2020-2021)
 *
 * @param values		The array of values to store in the matlab variable
 * @param numBytes		The number of values in the array to transfer
 * @return				The mxArray containing the array
 */
mxArray *mxUint8ArrayByValue(ui1 *values, int numBytes) {
    int i;
	
	// create the matlab variable (1x1 real double matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, numBytes, mxUINT8_CLASS, mxREAL);
	
	// transfer the values to the matlab (allocated memory)
	mxUint8 *data = (mxUint8 *)mxGetData(retArr);
	for (i = 0; i < numBytes; i++)		data[i] = values[i];
    
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Uint32 matrix based on a MEF ui4 (unsigned 4 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxUint32ByValue(ui4 value) {
	
	// create the matlab variable (1x1 real uint32 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
	mxUint32 *data = (mxUint32 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxUint32)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Int32 matrix based on a MEF si4 (signed 4 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxInt32ByValue(si4 value) {
	
	// create the matlab variable (1x1 real int32 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	mxInt32 *data = (mxInt32 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxInt32)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Uint64 matrix based on a MEF ui8 (unsigned 8 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxUint64ByValue(ui8 value) {
	
	// create the matlab variable (1x1 real uint64 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	mxUint64 *data = (mxUint64 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxUint64)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Int64 matrix based on a MEF si8 (signed 8 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxInt64ByValue(si8 value) {
	
	// create the matlab variable (1x1 real int64 matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	mxInt64 *data = (mxInt64 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxInt64)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) double matrix based on a MEF sf4 (signed 4 byte float) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxSingleByValue(sf4 value) {
	
	// create the matlab variable (1x1 real single matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxSINGLE_CLASS, mxREAL);
	mxSingle *data = (mxSingle *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxSingle)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a (1x1 real) double matrix based on a MEF sf8 (signed 8 byte float) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxDoubleByValue(sf8 value) {
	
	// create the matlab variable (1x1 real double matrix) and retrieve the pointer to the allocated memory
	mxArray *retArr = mxCreateDoubleMatrix(1, 1, mxREAL);
	mxDouble *data = (mxDouble *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	*data = (mxDouble)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a matlab char array based on a MEF UTF-8 character string
 * (force/indicate UTF-8 encoding; depending on the OS locale matlab might be stuck on windows-1252) 
 *
 * Note: Make sure the given char array pointer has a NULL-character at the end, else 'strlen' might crash
 *
 * @param str		The char array to store in the matlab variable
 * @return			The mxArray containing the char array
 */
mxArray *mxStringByUtf8CharString(char *str) {
	if (str == NULL)	return mxCreateString("");
	
	// retrieve the number of bytes in the (UTF-8) input string
	// note: this might be differt then the actual number of characters
    //	     (depending on the occurance of non-ASCII characters)
	mwSize lengthInBytes = strlen(str);
	
	// copy the byte values from the input string into an matlab uint8 array
	// note: mxCreateString would do this, but also would destroy the non-ASCII
	//		 code points (any byte with a value above 127 is converted to 65535)
	mxArray *mat_uint8 = mxCreateNumericMatrix(1, lengthInBytes, mxUINT8_CLASS, mxREAL);
	mxUint8 *p_mat_uint8 = (mxUint8 *)mxGetData(mat_uint8);
	for (int i = 0; i < lengthInBytes; i++) {
		p_mat_uint8[i] = str[i];
	}
	
	// use the native2unicode call from matlab to convert the
	// UTF-8 bytes to a UTF-8 encoded char array in matlab
    mxArray *lhs[1]; 
    mxArray *rhs[] = {mat_uint8, mxCreateString("UTF-8")};
	mexCallMATLAB(1, lhs, 2, rhs, "native2unicode");
	
	// free the memory
	mxDestroyArray(rhs[1]);
	mxDestroyArray(mat_uint8);
	
	// return matlab array (should now be a matlab UTF-8 string)
	return lhs[0];
	
}

/**
 * Copy a matlab char array to an existing MEF UTF-8 character string with a maximum size
 *
 * @param mat		The source matlab char array
 * @param str	    A pointer to the destination MEF UTF-8 character string
 * @param strSize   The (maximum) size of the destination MEF UTF-8 character string in bytes
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxStringToUtf8CharString(const mxArray *mat, char *str, int strSize) {

	// check the input
	if (mxGetClassID(mat) != mxCHAR_CLASS) {
		mexPrintf("Error: could not copy the matlab-array character string to C data-type (input is not of the 'char' data-type), exiting...\n");
		return false;
	}
	
	// convert the char array in matlab to UTF-8 bytes
	char *p_mat_uint8 = mxArrayToUTF8String(mat);
	if (p_mat_uint8 == NULL) {
		mexPrintf("Error: could not convert matlab char-array to UTF-8 bytes (input is most likely not a char-array), exiting...\n");
		return false;
	}

	// retrieve the number of bytes in the UTF-8 input string
	// note: this might be differt then the actual number of characters
	//	     (depending on the occurance of non-ASCII characters)
	mwSize lengthInBytes = strlen(p_mat_uint8);
	
	// check maximum number of bytes
	if (lengthInBytes + 1 > strSize) {
		mexPrintf("Error: char array too large (length in UTF-8 bytes plus a NULL character: %i), variable can only hold %i bytes\n", lengthInBytes + 1, strSize);
		mxFree(p_mat_uint8);
		return false;
	}
	
	// transfer the string
	for (mwSize i = 0; i < lengthInBytes; i++)
		str[i] = p_mat_uint8[i];
	
	// add a null character
	str[lengthInBytes] = '\0';
	
	// free the memory
	mxFree(p_mat_uint8);
	
	// return success
	return true;
	
}



//
// Functions that check and convert a matlab input argument matrix with a single value
// 

/**
 * Check and extract a boolean value from a matlab input argument matrix
 *
 * @param mat   	The matlab array with a single value to check and extract
 * @param argName   The name of the input argument (used in error messages)
 * @param pVar  	Pointer to a boolean variable to store the value in
 * @return			True if a valid single numeric boolean value was extracted, false elsewise
 */
bool getInputArgAsBool(const mxArray *mat, const char *argName, bool *pVar) {
	
	// check the input matrix
	if (mxIsEmpty(mat)) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is empty, should be 0 (false) or 1 (true)", argName);
		return false;
	}
	if (!mxIsLogicalScalar(mat) && !mxIsNumeric(mat)) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is not numeric or logical, should be 0 (false) or 1 (true)", argName);
		return false;
	}
	if (mxGetNumberOfElements(mat) != 1) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument invalid, should be a single numeric or logical value", argName);
		return false;
	}
	
	// assign the value and return succes
	*pVar = (mxIsLogicalScalarTrue(mat) || mxGetScalar(mat) == 1);
	return true;
	
}

/**
 * Check and extract a single MEF si8 (signed 8 byte int) value from a matlab input argument matrix
 *
 * @param mat   	The matlab array with a single value to check and extract
 * @param argName   The name of the input argument (used in error messages)
 * @param minValue  The minimum integer value (also used to determine whether the double data-type can be used)
 * @param maxValue  The maximum integer value (also used to determine whether the double data-type can be used)
 * @param pVar  	Pointer to a si8 variable to store the value in
 * @return			True if a valid single numeric si8 value was extracted, false elsewise
 */
bool getInputArgAsInt64(const mxArray *mat, const char *argName, si8 minValue, si8 maxValue, si8 *pVar) {
	
	// check the input matrix
	if (mxIsEmpty(mat)) {
		if (minValue == -1)
			mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is empty, should be -1, 0 or a positive integer (1, 2, ...)", argName);
		else
			mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is empty, should be a numeric value", argName);
		return false;
	}
	if (!mxIsNumeric(mat)) {
		if (minValue == -1)
			mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is not numeric, should be -1, 0 or a positive integer (1, 2, ...)", argName);
		else
			mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is not numeric", argName);
		return false;
	}
	if (mxGetNumberOfElements(mat) != 1) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument invalid, should be a single numeric value", argName);
		return false;
	}
	
	// try to retrieve the value as an si8
	si8 mat_si8 = 0;
	mxClassID classID = mxGetClassID(mat);
    if (classID == mxINT64_CLASS) {
        // same type, just use the value
        
        mat_si8 = (si8)*(mxInt64 *)mxGetData(mat);
        
        
    } else if (classID == mxDOUBLE_CLASS) {

        // retrieve the value as a scalar first
        double dbl_mat = mxGetScalar(mat);

        // check if we only allow a negative value of -1 and the value is indeed -1
        // (if the value that is passed is -1 then there is no risk of imprecision)
        if (minValue == -1 && dbl_mat == -1) {
            // allow only -1 and value is -1

            mat_si8 = -1;

        } else {
            // elsewise

            // check if the value that we are trying to retrieve from the double can is smaller or
            // larger than what a double variable can contain without losing precision (<= 2^53; so <= 9007199254740992)
            if (classID == mxDOUBLE_CLASS && (dbl_mat < -9007199254740992 || dbl_mat > 9007199254740992)) {
                mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type is invalid. The argument is a double; because the argument might require a value smaller than -2^53 or larger than 2^53, using a double for input could result in a loss of precision. Instead pass the value as an signed 64-bit integer (e.g. 'int64(1024)')", argName);
                return false;				
            }

            // check the scalar
            if (mxIsNaN(dbl_mat) || mxIsInf(dbl_mat)) {
                mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is NaN or Inf, should be an integer", argName);
                return false;
            }
            if (floor(dbl_mat) != dbl_mat) {
                mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is a fraction, should be an integer", argName);
                return false;
            }

            // transfer to si8 variable
            mat_si8 = (si8)dbl_mat;
            
        }
        
    } else {

        if (minValue < -9007199254740992 || maxValue > 9007199254740992)
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type (%s) is invalid, should be int64", argName, mxGetClassName(mat));
        else
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type (%s) is invalid, should be an int64 or double", argName, mxGetClassName(mat));
        return false;

	}
	
	// check the value
	if (mat_si8 < minValue) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is invalid, the numeric value cannot be smaller than %lld", argName, minValue);
		return false;
	}
	if (mat_si8 > maxValue) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is invalid, the numeric value cannot be greater than %lld", argName, maxValue);
		return false;
	}
	
	// assign the value and return succes
	*pVar = mat_si8;
	return true;
	
}

/**
 * Check and extract a single MEF ui8 (unsigned 8 byte int) value from a matlab input argument matrix
 *
 * @param mat   	The matlab array with a single value to check and extract
 * @param argName   The name of the input argument (used in error messages)
 * @param maxValue  The maximum integer value (also used to determine whether the double data-type can be used)
 * @param pVar  	Pointer to a ui8 variable to store the value in
 * @return			True if a valid single numeric si8 value was extracted, false elsewise
 */
bool getInputArgAsUint64(const mxArray *mat, const char *argName, ui8 maxValue, ui8 *pVar) {
	
	// check the input matrix
	if (mxIsEmpty(mat)) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is empty", argName);
		return false;
	}
	if (!mxIsNumeric(mat)) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is not numeric, should be 0 or a positive integer (1, 2, ...)", argName);
		return false;
	}
	if (mxGetNumberOfElements(mat) != 1) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument invalid, should be a single numeric value", argName);
		return false;
	}
	
	// try to retrieve the value as an ui8
	ui8 mat_ui8 = 0;
	mxClassID classID = mxGetClassID(mat);
	if (classID == mxUINT64_CLASS) {
        // same type, just use the value
        
        mat_ui8 = (ui8)*(mxUint64 *)mxGetData(mat);
        
    } else if (classID == mxDOUBLE_CLASS) {

        // 
        // check if the value that we are trying to retrieve from the double can is larger than
        // what a double variable can contain without losing precision (<= 2^53; so <= 9007199254740992)
        if (classID == mxDOUBLE_CLASS && mat_ui8 > 9007199254740992) {
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type is invalid. The argument is a double; because the argument might require a value larger than 2^53, using a double for input could result in a loss of precision. Instead pass the value as an unsigned 64-bit integer (e.g. 'uint64(1024)')", argName);
            return false;				
        }

        // retrieve the value as a scalar first
        double dbl_mat = mxGetScalar(mat);

        // check the scalar
        if (mxIsNaN(dbl_mat) || mxIsInf(dbl_mat)) {
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is NaN or Inf, should be 0 or a positive integer (1, 2, ...)", argName);
            return false;
        }
        if (floor(dbl_mat) != dbl_mat) {
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is a fraction, should be 0 or a positive integer (1, 2, ...)", argName);
            return false;
        }

        // transfer to ui8 variable
        mat_ui8 = (ui8)dbl_mat;
        
    } else {

        if (maxValue > 9007199254740992)
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type (%s) is invalid, should be uint64", argName, mxGetClassName(mat));
        else
            mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument data-type (%s) is invalid, should be an uint64 or double", argName, mxGetClassName(mat));

        return false;

	}
	
	// check the value
	if (mat_ui8 < 0) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is negative, should be 0 or a positive integer (1, 2, ...)", argName);
		return false;
	}
	if (mat_ui8 > maxValue) {
		mexErrMsgIdAndTxt("MATLAB:matmef_utils:invalidArg", "'%s' input argument is invalid, the numeric value cannot be greater than %llu", argName, maxValue);
		return false;
	}
	
	// assign the value and return succes
	*pVar = mat_ui8;
	return true;
	
}
