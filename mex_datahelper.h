#ifndef MEX_DATAHELPER_
#define MEX_DATAHELPER_
/**
 *
 * 	@file - header
 *
 *
 */
#include "mex.h"
#include "meflib/meflib/meflib.h"

mxArray *mxUint8ArrayByValue(ui1 *array, int num_bytes);

mxArray *mxUint8ByValue(ui1 value);
mxArray *mxInt8ByValue(si1 value);
mxArray *mxUint32ByValue(ui4 value);
mxArray *mxInt32ByValue(si4 value);
mxArray *mxUint64ByValue(ui8 value);
mxArray *mxInt64ByValue(si8 value);

mxArray *mxDoubleByValue(sf8 value);
mxArray *mxStringByUTF8Value(char *str);


#endif   // MEX_DATAHELPER_