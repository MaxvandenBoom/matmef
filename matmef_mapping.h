#ifndef MATMEF_MAPPING_
#define MATMEF_MAPPING_
/**
 * 	@file - headers
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Functions to map MEF3 objects and structures to Matlab structures
 *	
 *  Copyright 2020, Max van den Boom and Mayo Clinic (Rochester MN)
 *	Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)
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


mxArray* createFileProcessingStruct();
mxArray* createSegmentStruct();
mxArray* createSegmentStructSized(mwSize rows, mwSize columns);
mxArray* createChannelStruct();
mxArray* createChannelStructSized(mwSize rows, mwSize columns);
mxArray* createSessionStruct();
mxArray* createMetadataStruct();
mxArray* createMetadataSection1Struct();
mxArray* createTsMetadataSection2Struct();
mxArray* createVMetadataSection2Struct();
mxArray* createMetadataSection3Struct();

void map_mef3_segment_tostruct(SEGMENT *segment, si1 mapIndicesFlag, mxArray* matSegment, int matIndex);
mxArray* map_mef3_segment(SEGMENT *segment, si1 mapIndicesFlag);
void map_mef3_channel_tostruct(CHANNEL *channel, si1 mapIndicesFlag, mxArray* matChannel, int matIndex);
mxArray* map_mef3_channel(CHANNEL *channel, si1 mapIndicesFlag);
mxArray* map_mef3_session(SESSION *session, si1 map_indices_flag);
mxArray* map_mef3_md1(METADATA_SECTION_1 *md1);
mxArray* map_mef3_tmd2(TIME_SERIES_METADATA_SECTION_2 *tmd2);
mxArray* map_mef3_vmd2(VIDEO_METADATA_SECTION_2 *vmd2);
mxArray* map_mef3_md3(METADATA_SECTION_3 *md3);


#endif   // MATMEF_MAPPING_