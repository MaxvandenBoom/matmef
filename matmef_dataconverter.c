/**
 * 	@file
 * 	Functions to convert primitive c-datatypes to matlab primitive (1x1) arrays/matrices
 *	
 *  Copyright 2020, Max van den Boom
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



/**
 * Create a (1xN real) Uint8 vector/matrix based on a MEF array of ui1 (unsigned 1 byte int) values
 * (contributed by Richard J. Cui, 4 apr 2020)
 *
 * @param array			The array to store in the matlab variable
 * @param num_bytes		The number of values in the array to transfer
 * @return				The mxArray containing the array
 */
mxArray *mxUint8ArrayByValue(ui1 *array, int num_bytes) {
    int i;
	
	// create the matlab variable (1x1 real double matrix)
    mxArray *retArr = mxCreateNumericMatrix(1, num_bytes, mxUINT8_CLASS, mxREAL);
	
	// transfer the values to the matlab (allocated memory)
    unsigned char *ucp = (unsigned char *)mxGetData(retArr);
    for (i = 0; i < num_bytes; i++)		ucp[i] = array[i];
    
	// return the matlab variable
    return retArr;
	
}



/**
 * Create a (1x1 real) Uint8 matrix based on a MEF ui1 (unsigned 1 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxUint8ByValue(ui1 value) {
	
	// create the matlab variable (1x1 real uint8 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT8_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxUint8 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxUint8)value;
	
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
	
	// create the matlab variable (1x1 real int8 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT8_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxInt8 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxInt8)value;
	
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
	
	// create the matlab variable (1x1 real uint32 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxUint32 *data = (mxUint32 *)mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxUint32)value;
	
	// return the double variable
	return retArr;
	
}


/**
 * Create a (1x1 real) Int32 matrix based on a MEF si4 (signed 4 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray *mxInt32ByValue(si4 value) {
	
	// create the matlab variable (1x1 real int32 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxInt32 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxInt32)value;
	
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
	
	// create the matlab variable (1x1 real uint64 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxUint64 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxUint64)value;
	
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
	
	// create the matlab variable (1x1 real int64 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxInt64 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxInt64)value;
	
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
	
	// create the matlab variable (1x1 real double matrix)
	mxArray *retArr = mxCreateDoubleMatrix(1, 1, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxDouble *data = mxGetPr(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxDouble)value;
	
	// return the matlab variable
	return retArr;
	
}

/**
 * Create a matlab char array based on a MEF UTF-8 character string
 * (force/indicate UTF-8 encoding; depending on the OS locale matlab might be stuck on windows-1252) 
 *
 * @param str		The char array to store in the matlab variable
 * @return			The mxArray containing the char array
 */
mxArray *mxStringByUtf8CharString(char *str) {
	
	// TODO: try UTF-8 strings, function exists but need set to test
	
	// retrieve the number of bytes in the (UTF-8) input string
	// note: this might be differt then the actual number of characters
    //	     (depending on the occurance of non-ASCII characters)
	mwSize lengthInBytes = strlen(str);
	
	// copy the byte values from the input string into an matlab uint8 array
	// note: mxCreateString would do this, but also would destroy the non-ASCII
	//		 code points (any byte with a value above 127 is converted to 65535)
	mxArray *mat_uint8 = mxCreateNumericMatrix(1, lengthInBytes, mxUINT8_CLASS, mxREAL);
	mxUint8 *p_mat_uint8 = mxGetData(mat_uint8);
	//mxUint8 *p_mat_uint8 = mxGetUint8s(mat_uint8);
	for (int i = 0; i < lengthInBytes; i++) {
		p_mat_uint8[i] = str[i];
	}
	
	// use the native2unicode call from matlab to convert the
	// UTF-8 bytes to a UTF-8 encoded char array in matlab
    mxArray *lhs[1]; 
    mxArray *rhs[] = {mat_uint8, mxCreateString("UTF-8")};
	mexCallMATLAB(1, lhs, 2, rhs, "native2unicode");
	
	// free the memory
	mxFree(mat_uint8);
	
	// return matlab array (should now be a matlab UTF-8 string)
	return lhs[0];
	
}

/**
 * Copy a matlab char array to an existing MEF UTF-8 character string with a maximum size
 *
 * @param mat_str   The source matlab char array
 * @param str	    The destination MEF UTF-8 character string
 * @param strSize   The (maximum) size of the destination MEF UTF-8 character string in bytes
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxStringToUtf8CharString(mxArray *mat_str, char *str, int strSize) {
	
	// convert the char array in matlab to UTF-8 bytes
	char *p_mat_uint8 = mxArrayToUTF8String(mat_str);
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