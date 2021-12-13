#ifndef MATMEF_DATACONVERTER_
#define MATMEF_DATACONVERTER_
/**
 * 	@file - headers
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
#include "mex.h"
#include "meflib/meflib/meflib.h"


mxArray *mxUint8ByValue(ui1 value);
mxArray *mxInt8ByValue(si1 value);
mxArray *mxUint8ArrayByValue(ui1 *values, int numBytes);
mxArray *mxUint32ByValue(ui4 value);
mxArray *mxInt32ByValue(si4 value);
mxArray *mxUint64ByValue(ui8 value);
mxArray *mxInt64ByValue(si8 value);
mxArray *mxSingleByValue(sf4 value);
mxArray *mxDoubleByValue(sf8 value);
mxArray *mxStringByUtf8CharString(char *str);

bool checkSingleNumericValue(const mxArray *mat);
bool cpyMxUint8ToVar(const mxArray *mat, ui1 *var);
bool cpyMxInt8ToVar(const mxArray *mat, si1 *var);
bool cpyMxUint8ArrayToVar(const mxArray *mat, ui1 *var, int varBytes);
bool cpyMxUint32ToVar(const mxArray *mat, ui4 *var);
bool cpyMxInt32ToVar(const mxArray *mat, si4 *var);
bool cpyMxUint64ToVar(const mxArray *mat, ui8 *var);
bool cpyMxInt64ToVar(const mxArray *mat, si8 *var);
bool cpyMxSingleToVar(const mxArray *mat, sf4 *var);
bool cpyMxDoubleToVar(const mxArray *mat, sf8 *var);
bool cpyMxStringToUtf8CharString(const mxArray *mat, char *str, int strSize);

mxArray *retrieveMxField(const mxArray *pm, const char *fieldname);
bool cpyMxFieldUint8ToVar(const mxArray *pm, const char *fieldname, ui1 *var);
bool cpyMxFieldInt8ToVar(const mxArray *pm, const char *fieldname, si1 *var);
bool cpyMxFieldUint8ArrayToVar(const mxArray *pm, const char *fieldname, ui1 *var, int varBytes);
bool cpyMxFieldUint32ToVar(const mxArray *pm, const char *fieldname, ui4 *var);
bool cpyMxFieldInt32ToVar(const mxArray *pm, const char *fieldname, si4 *var);
bool cpyMxFieldUint64ToVar(const mxArray *pm, const char *fieldname, ui8 *var);
bool cpyMxFieldInt64ToVar(const mxArray *pm, const char *fieldname, si8 *var);
bool cpyMxFieldSingleToVar(const mxArray *pm, const char *fieldname, sf4 *var);
bool cpyMxFieldDoubleToVar(const mxArray *pm, const char *fieldname, sf8 *var);
bool cpyMxFieldStringToUtf8CharString(const mxArray *pm, const char *fieldname, char *str, int strSize);

bool getInputArgAsBool(const mxArray *mat, const char *argName, bool *pVar);
bool getInputArgAsInt64(const mxArray *mat, const char *argName, si8 minValue, si8 maxValue, si8 *pVar);
bool getInputArgAsUint64(const mxArray *mat, const char *argName, ui8 maxValue, ui8 *pVar);

bool transferMxFields(const mxArray *src, mxArray *dst);

#endif   // MATMEF_DATACONVERTER_