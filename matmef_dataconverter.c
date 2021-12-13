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
#include "mex_utils.h"
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



//
// Functions that copy data from a matlab-array to C data-types
// 

/**
 * Check whether the input matrix is numeric with a single value
 *
 * @param mat   	The matlab array to check
 * @return			True if the matrix has a single numeric value, false elsewise
 */
bool checkSingleNumericValue(const mxArray *mat) {
	if (mxIsEmpty(mat) || !mxIsNumeric(mat) || mxGetNumberOfElements(mat) > 1) {
		mexPrintf("Error: invalid input matrix, cannot transfer the matlab-array value to a C-variable (input is empty, not numeric or has more than 1 element), exiting...\n");
		return false;
	}
	return true;
}

/**
 * Copy a matlab (1x1 real) Uint8 matrix value to an existing MEF ui1 (unsigned 1 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF ui1 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxUint8ToVar(const mxArray *mat, ui1 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxUINT8_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'uint8' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxUint8 *mat_data = (mxUint8 *)mxGetData(mat);
	*var = *mat_data;
	return true;
	
}

/**
 * Copy a matlab (1x1 real) Int8 matrix value to an existing MEF si1 (signed 1 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF si1 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxInt8ToVar(const mxArray *mat, si1 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxINT8_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'int8' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxInt8 *mat_data = (mxInt8 *)mxGetData(mat);
	*var = *mat_data;
	return true;

}

/**
 * Copy the values from a matlab (1x1 real) Uint8 vector/matrix into an existing MEF ui1 (unsigned 1 byte int) array variable
 *
 * @param mat   		The source matlab array
 * @param var	    	A pointer to the destination MEF ui1 array variable
 * @param varBytes		The number of values that the destination MEF ui1 array variable can hold
 * @return				True if successfully tranferred, false on error
 */
bool cpyMxUint8ArrayToVar(const mxArray *mat, ui1 *var, int varBytes) {
	
	// check the input
	if (mxIsEmpty(mat) || !mxIsNumeric(mat) || mxGetNumberOfElements(mat) < 1) {
		mexPrintf("Error: invalid input matrix, cannot transfer the matlab-array values into a C-array (input is empty, not numeric or does not have more any elements), exiting...\n");
		return false;
	}

	if (mxGetClassID(mat) != mxUINT8_CLASS) {
		mexPrintf("Error: could not copy the matlab-array values into a C-array (input is not of the 'uint8' data-type), exiting...\n");
		return false;
	}
	
	// check if the number of elements matches
	size_t numElements = mxGetNumberOfElements(mat);
	if (numElements != varBytes) {
		mexPrintf("Error: could not copy the matlab-array values into a C-array (the input array should be the same size as the C-array; the input has %i values, while the C-array is %i bytes), exiting...\n", numElements, varBytes);
		return false;
	}
	
	// copy and return success
	mxUint8 *mat_data = (mxUint8 *)mxGetData(mat);
	for (int i = 0; i < numElements; i++)
		var[i] = mat_data[i];	
	
	return true;
	
}

/**
 * Copy a matlab (1x1 real) Uint32 matrix value to an existing MEF ui4 (unsigned 4 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF ui4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxUint32ToVar(const mxArray *mat, ui4 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxUINT32_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'uint32' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxUint32 *mat_data = (mxUint32 *)mxGetData(mat);
	*var = *mat_data;
	return true;
	
}

/**
 * Copy a matlab (1x1 real) Int32 matrix value to an existing MEF si4 (signed 4 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF si4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxInt32ToVar(const mxArray *mat, si4 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxINT32_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'int32' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxInt32 *mat_data = (mxInt32 *)mxGetData(mat);
	*var = *mat_data;
	return true;

}

/**
 * Copy a matlab (1x1 real) Uint64 matrix value to an existing MEF ui8 (unsigned 8 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF ui8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxUint64ToVar(const mxArray *mat, ui8 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxUINT64_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'uint64' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxUint64 *mat_data = (mxUint64 *)mxGetData(mat);
	*var = *mat_data;
	return true;
	
}

/**
 * Copy a matlab (1x1 real) Int64 matrix value to an existing MEF si8 (signed 8 byte int) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF si8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxInt64ToVar(const mxArray *mat, si8 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxINT64_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'int64' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxInt64 *mat_data = (mxInt64 *)mxGetData(mat);
	*var = *mat_data;
	return true;

}

/**
 * Copy a matlab (1x1 real) single float matrix value to an existing MEF sf4 (signed 4 byte float) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF sf4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxSingleToVar(const mxArray *mat, sf4 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxSINGLE_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'single' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxSingle *mat_data = (mxSingle *)mxGetData(mat);
	*var = *mat_data;
	return true;

}

/**
 * Copy a matlab (1x1 real) double float matrix value to an existing MEF sf8 (signed 8 byte float) variable
 *
 * @param mat   	The source matlab array
 * @param var	    A pointer to the destination MEF sf8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxDoubleToVar(const mxArray *mat, sf8 *var) {
	
	// check the input
	if (!checkSingleNumericValue(mat))
		return false;

	if (mxGetClassID(mat) != mxDOUBLE_CLASS) {
		mexPrintf("Error: could not copy the matlab-array value to a C-variable (input is not of the 'double' data-type), exiting...\n");
		return false;
	}
	
	// copy and return success
	mxDouble *mat_data = (mxDouble *)mxGetData(mat);
	*var = *mat_data;
	return true;

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
// Functions that copy data from a matlab-struct field to C data-types
// 

/**
 * Retrieve a matlab field from a struct-matrix
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to retrieve
 * @return			The matlab matrix in the field if successful, NULL on error
 */
mxArray *retrieveMxField(const mxArray *pm, const char *fieldname) {
	mxArray *field = mxGetField(pm, 0, fieldname);
	if (field == NULL)
		mexPrintf("Error: could not map field '%s', field does not exist in the structure\n", fieldname);
	return field;
}
 
/**
 * Copy the value from a matlab field (being a 1x1 real, Uint8 matrix) in a struct-matrix to an existing MEF ui1 (unsigned 1 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF ui1 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldUint8ToVar(const mxArray *pm, const char *fieldname, ui1 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxUint8ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, Int8 matrix) in a struct-matrix to an existing MEF si1 (signed 1 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF si1 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldInt8ToVar(const mxArray *pm, const char *fieldname, si1 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxInt8ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the values from a matlab field (being a 1x1 real, Uint8 vector/matrix) in a struct-matrix into an existing MEF ui1 (unsigned 1 byte int) array variable
 *
 * @param pm   			Pointer to the matlab struct-matrix that holds the field
 * @param fieldname 	The name of the field in the matlab struct-matrix to copy the values from
 * @param var	    	A pointer to the destination MEF ui1 array variable
 * @param varBytes		The number of values that the destination MEF ui1 array variable can hold
 * @return				True if successfully tranferred, false on error
 */
bool cpyMxFieldUint8ArrayToVar(const mxArray *pm, const char *fieldname, ui1 *var, int varBytes) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the values
	if (!cpyMxUint8ArrayToVar(field, var, varBytes)) {
		mexPrintf("Error: could not copy the values from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, Uint32 matrix) in a struct-matrix to an existing MEF ui4 (unsigned 4 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF ui4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldUint32ToVar(const mxArray *pm, const char *fieldname, ui4 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxUint32ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, Int32 matrix) in a struct-matrix to an existing MEF si4 (signed 4 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF si4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldInt32ToVar(const mxArray *pm, const char *fieldname, si4 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxInt32ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, UInt64 matrix) in a struct-matrix to an existing MEF ui8 (unsigned 8 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF ui8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldUint64ToVar(const mxArray *pm, const char *fieldname, ui8 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxUint64ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, Int64 matrix) in a struct-matrix to an existing MEF si8 (signed 8 byte int) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF si8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldInt64ToVar(const mxArray *pm, const char *fieldname, si8 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxInt64ToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, single float matrix) in a struct-matrix to an existing MEF sf4 (signed 4 byte float) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF sf4 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldSingleToVar(const mxArray *pm, const char *fieldname, sf4 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxSingleToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a 1x1 real, double float matrix) in a struct-matrix to an existing MEF sf8 (signed 8 byte float) variable
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param var	    A pointer to the destination MEF sf8 variable
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldDoubleToVar(const mxArray *pm, const char *fieldname, sf8 *var) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the value
	if (!cpyMxDoubleToVar(field, var)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
	// return success
	return true;
	
}

/**
 * Copy the value from a matlab field (being a char array) in a struct-matrix to an existing MEF UTF-8 character string with a maximum size
 *
 * @param pm   		Pointer to the matlab struct-matrix that holds the field
 * @param fieldname The name of the field in the matlab struct-matrix to copy the value from
 * @param str	    A pointer to the destination MEF UTF-8 character string
 * @param strSize   The (maximum) size of the destination MEF UTF-8 character string in bytes
 * @return			True if successfully tranferred, false on error
 */
bool cpyMxFieldStringToUtf8CharString(const mxArray *pm, const char *fieldname, char *str, int strSize) {
	
	// retrieve the field
	const mxArray *field = retrieveMxField(pm, fieldname);
	if (field == NULL)	return false;
	
	// copy the UTF-8 string
	if (!cpyMxStringToUtf8CharString(field, str, strSize)) {
		mexPrintf("Error: could not copy the value from struct-field '%s', exiting...\n", fieldname);
		return false;
	}
	
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


//
// Miscellous functions
// 

/**
 * Transfer fields from a source struct-matrix into an existing destination-matrix
 * The destination matrix is expected to already contain the fields to be transfer, so that the data-type
 * of the fields can be checked (should be the same, elsewise the transfer will fail). Upon match by field-name
 * and data-type of the field, the value of the field in the destination matrix will be overwritten
 *
 * @param src   	Pointer to the matlab struct-matrix that holds the fields to be transferred
 * @param dst	    A pointer to the destination matlab struct-matrix in which the field values should be updated
 * @return			True if successfully tranferred, false on error
 */
bool transferMxFields(const mxArray *src, mxArray *dst) {
	
	// transfer the fields from the user
	int iSrc, iDst;
	const char  *fieldName;
	int srcNumFields = mxGetNumberOfFields(src);
	int dstNumFields = mxGetNumberOfFields(dst);
	
	for (iSrc = 0; iSrc < srcNumFields; iSrc++) {
		
		// retrieve the field name in the source struct
		fieldName = mxGetFieldNameByNumber(src, iSrc);
		
		// find the field in the target struct
		for (iDst = 0; iDst < dstNumFields; iDst++) {
			if (strcmp(fieldName, mxGetFieldNameByNumber(dst, iDst)) == 0) {
				break;
			}
		}
		if (dstNumFields == 0 || iDst == dstNumFields) {
			mxForceWarning("matmef:dataconverter", "unknown field '%s' in input struct, ignoring field", fieldName);
		} else {
			
			// retrieve the fields
			mxArray *srcField = mxGetField(src, 0, fieldName);
			mxArray *dstField = mxGetField(dst, 0, fieldName);
			if (srcField == NULL) {
				mexPrintf("Error: the field '%s' in the input struct is empty. Either remove field or make sure it has a valid value, exiting...\n", fieldName);
				return false;
			}
			if (dstField == NULL) {
				mexPrintf("Error: the field '%s' in the destination struct is empty, exiting...\n", fieldName);
				return false;
			}
			
			// retrieve the source field data-type			
			mxClassID srcClassID = mxGetClassID(srcField);
		
			// retrieve and check the number of elements in the field (only when numeric)
			size_t srcNumElements = mxGetNumberOfElements(srcField);
			size_t dstNumElements = mxGetNumberOfElements(dstField);
			if (srcClassID != mxCHAR_CLASS) {
				if (srcNumElements == 0) {
					mexPrintf("Error: empty numeric array in field '%s' of the input struct (either remove the field or provide a valid value), exiting...\n", fieldName);
					return false;
				} else if (srcNumElements != dstNumElements) {
					mexPrintf("Error: invalid number of elements in field '%s' of the input struct, the number of elements of the field is '%i' but should be '%i', exiting...\n", fieldName, srcNumElements, dstNumElements);
					return false;
				}
			}
			
			// check if the field data-types match
			if (srcClassID == mxGetClassID(dstField)) {
				// matching data-types
				
				// transfer the value
				switch(srcClassID) {
					
					case mxUINT8_CLASS:
						
						if (srcNumElements == 1) {
							*(mxUint8 *)mxGetData(dstField) = *(mxUint8 *)mxGetData(srcField);
							
						} else {
							mxUint8 *srcArray = (mxUint8 *)mxGetData(srcField);
							mxUint8 *dstArray = (mxUint8 *)mxGetData(dstField);
							for (int i = 0; i < srcNumElements; i++)
								dstArray[i] = srcArray[i];
							
						}
						break;
					
					case mxINT8_CLASS:
						*(mxInt8 *)mxGetData(dstField) = *(mxInt8 *)mxGetData(srcField);
						break;
					
					case mxUINT32_CLASS:
						*(mxUint32 *)mxGetData(dstField) = *(mxUint32 *)mxGetData(srcField);
						break;
					
					case mxINT32_CLASS:
						*(mxInt32 *)mxGetData(dstField) = *(mxInt32 *)mxGetData(srcField);
						break;
					
					case mxUINT64_CLASS:
						*(mxUint64 *)mxGetData(dstField) = *(mxUint64 *)mxGetData(srcField);
						break;
					
					case mxINT64_CLASS:
						*(mxInt64 *)mxGetData(dstField) = *(mxInt64 *)mxGetData(srcField);
						break;
					
					case mxSINGLE_CLASS:
						*(mxSingle *)mxGetData(dstField) = *(mxSingle *)mxGetData(srcField);
						break;
					
					case mxDOUBLE_CLASS:
						*(mxDouble *)mxGetData(dstField) = *(mxDouble *)mxGetData(srcField);
						break;
					
					case mxCHAR_CLASS:
					
						// Note: The field is updated with a newly created mxArray (based on the source character array), this
						//       is done because the lengths of the source and destination character arrays might differ
						//       so we cannot simply move the values from one mxArray to another mxArray
						mxSetField(dst, 0, fieldName, mxStringByUtf8CharString(mxArrayToUTF8String(srcField)));
						
						break;

                    default:
                        break;
					
				}
				
			} else {
				// mismatch in data-types 
				
				// message and return failure
				mexPrintf("Error: wrong data-type for field '%s' in input struct, the data-type of the field is '%s' but should be '%s', exiting...\n", fieldName, mxGetClassName(srcField), mxGetClassName(dstField));
				return false;
			}
			
		}

	}
	
	// return success
	return true;
	
}
