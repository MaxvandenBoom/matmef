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
#include "mex_datahelper.h"
#include "mex.h"
#include "meflib/meflib.h"



/**
 * Create a (1x1 real) double matrix based on a MEF sf8 (signed 8 byte float) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray* mxDoubleByValue(sf8 value) {
	
	// create the matlab variable (1x1 real double matrix)
	mxArray *retArr = mxCreateDoubleMatrix(1, 1, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxDouble *data = mxGetPr(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxDouble)value;
	
	// return the double variable
	return retArr;
	
}

/**
 * Create a (1x1 real) UInt32 matrix based on a MEF ui4 (unsigned 4 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray* mxUInt32ByValue(ui4 value) {
	
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
mxArray* mxInt32ByValue(si4 value) {
	
	// create the matlab variable (1x1 real int32 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxInt32 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxInt32)value;
	
	// return the double variable
	return retArr;
	
}

/**
 * Create a (1x1 real) Int64 matrix based on a MEF si8 (signed 8 byte int) variable
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
mxArray* mxInt64ByValue(si8 value) {
	
	// create the matlab variable (1x1 real int64 matrix)
	mxArray *retArr = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxInt64 *data = mxGetData(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxInt64)value;
	
	// return the double variable
	return retArr;
	
}



///
// standard c helpers
///


/**
 * Create a (1x1 real) double matrix based on the given double
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
/*
mxArray* mxDoubleByValue(double value) {
	
	// create the matlab variable (1x1 real double matrix)
	mxArray *retArr = mxCreateDoubleMatrix(1, 1, mxREAL);
	
	// retrieve the pointer to the memory allocated by matlab
	mxDouble *data = mxGetPr(retArr);
	
	// transfer the value to the matlab (allocated memory)
	data[0] = (mxDouble)value;
	
	// return the double variable
	return retArr;
	
}
*/


/**
 * Create a (1x1 real) numeric matrix based on the given integer
 *
 * @param value		The value to store in the matlab variable
 * @return			The mxArray containing the value
 */
/*
mxArray* mxIntByValue(int value, bool sign, int bits) {
	mxArray *retArr;
	
	// create the matlab variable (1x1 real double matrix)
	// retrieve the pointer to the memory allocated by matlab
	// transfer the value to the matlab (allocated memory)
	if (sign) {
		if (bits == 8) {
			retArr = mxCreateNumericMatrix(1, 1, mxINT8_CLASS, mxREAL);
			mxInt8 *data = mxGetPr(retArr);
			data[0] = (mxInt8)value;
			
		} else if (bits == 16) {
			retArr = mxCreateNumericMatrix(1, 1, mxINT16_CLASS, mxREAL);
			mxInt16 *data = mxGetPr(retArr);
			data[0] = (mxInt16)value;
			
		} else if (bits == 32) {
			retArr = mxCreateNumericMatrix(1, 1, mxINT32_CLASS, mxREAL);
			mxInt32 *data = mxGetPr(retArr);
			data[0] = (mxInt32)value;
			
		} else if (bits == 64) {
			retArr = mxCreateNumericMatrix(1, 1, mxINT64_CLASS, mxREAL);
			mxInt64 *data = mxGetPr(retArr);
			data[0] = (mxInt64)value;
		}
	} else {
		if (bits == 8) {
			retArr = mxCreateNumericMatrix(1, 1, mxUINT8_CLASS, mxREAL);
			mxUint8 *data = mxGetPr(retArr);
			data[0] = (mxUint8)value;
			
		} else if (bits == 16) {
			retArr = mxCreateNumericMatrix(1, 1, mxUINT16_CLASS, mxREAL);
			mxUint16 *data = mxGetPr(retArr);
			data[0] = (mxUint16)value;
			
		} else if (bits == 32) {
			retArr = mxCreateNumericMatrix(1, 1, mxUINT32_CLASS, mxREAL);
			mxUint32 *data = mxGetPr(retArr);
			data[0] = (mxUint32)value;
			
		} else if (bits == 64) {
			retArr = mxCreateNumericMatrix(1, 1, mxUINT64_CLASS, mxREAL);
			mxUint64 *data = mxGetPr(retArr);
			data[0] = (mxUint64)value;
		}
	}
	
	// return the double variable
	return retArr;
	
}
*/
