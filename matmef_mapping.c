/**
 * 	@file
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
#include "matmef_mapping.h"
#include "mex.h"
#include "meflib/meflib/meflib.h"
#include "mex_datahelper.h"



///
// Fields definitions for MEF3 matlab structs
///

// File Processing Structures
const int FILE_PROCESSING_NUMFIELDS			= 16;
const char *FILE_PROCESSING_FIELDNAMES[]	= {
	"full_file_name",			// full path including extension
	"fp",
	"fd",
	"file_length",
	"file_type_code",
	"universal_header",
	"directives",
	"password_data",
	"metadata",
	"time_series_indices",
	"video_indices",
	"records",
	"record_indices",
	"RED_blocks",
	"raw_data_bytes",
	"raw_data"
};	

// Session, Channel, Segment Processing Structures
const int SEGMENT_NUMFIELDS			= 12;
const char *SEGMENT_FIELDNAMES[] 	= {	
	"channel_type",
	"metadata_fps",
	"time_series_data_fps",
	"time_series_indices_fps",
	"video_indices_fps",
	"record_data_fps",
	"record_indices_fps",
	"name",					// just base name, no extension
	"path",					// full path to enclosing directory (channel directory)
	"channel_name",			// just base name, no extension
	"session_name",			// just base name, no extension
	"level_UUID"
};

const int CHANNEL_NUMFIELDS			= 16;
const char *CHANNEL_FIELDNAMES[] 	= {	
	"channel_type",
	"metadata",
	"record_data_fps",
	"record_indices_fps",
	"number_of_segments",
	"segments",
	"path",			// full path to enclosing directory (session directory)
	"name",			// just base name, no extension
	"extension",	// channel directory extension
	"session_name",	// just base name, no extension
	"level_UUID",
	"anonymized_name",
	// variables below refer to segments
	"maximum_number_of_records",
	"maximum_record_bytes",
	"earliest_start_time",
	"latest_end_time"
};

const int SESSION_NUMFIELDS			= 16;
const char *SESSION_FIELDNAMES[] 	= {	
	"time_series_metadata",
	"number_of_time_series_channels",
	"time_series_channels",
	"video_metadata",
	"number_of_video_channels",
	"video_channels",
	"record_data_fps",			// FILE_PROCESSING_STRUCT
	"record_indices_fps",		// FILE_PROCESSING_STRUCT
	"name",
	"path",
	"anonymized_name",
	"level_UUID",
	// below variables refer to channels
	"maximum_number_of_records",
	"maximum_record_bytes",
	"earliest_start_time",
	"latest_end_time"
};


// Metadata Structures
const int METADATA_NUMFIELDS 		= 4;
const char *METADATA_FIELDNAMES[] 	= {
	"section_1",
	"time_series_section_2",
	"video_section_2",
	"section_3"
};

const int METADATA_SECTION_1_NUMFIELDS		= 4;
const char *METADATA_SECTION_1_FIELDNAMES[] = {
	"section_2_encryption",
	"section_3_encryption",
	"protected_region",
	"discretionary_region"
};

const int TS_METADATA_SECTION_2_NUMFIELDS		= 27;
const char *TS_METADATA_SECTION_2_FIELDNAMES[] = {
	// type-independent fields
	"channel_description",			// utf8[511];
	"session_description",			// utf8[511];
	"recording_duration",
	// type-specific fields
	"reference_description",		// utf8[511];
	"acquisition_channel_number",
	"sampling_frequency",
	"low_frequency_filter_setting",
	"high_frequency_filter_setting",
	"notch_filter_frequency_setting",
	"AC_line_frequency",
	"units_conversion_factor",
	"units_description",			// utf8[31]
	"maximum_native_sample_value",
	"minimum_native_sample_value",
	"start_sample",
	"number_of_samples",
	"number_of_blocks",
	"maximum_block_bytes",
	"maximum_block_samples",
	"maximum_difference_bytes",
	"block_interval",
	"number_of_discontinuities",
	"maximum_contiguous_blocks",
	"maximum_contiguous_block_bytes",
	"maximum_contiguous_samples",
	"protected_region",
	"discretionary_region"
};

const int V_METADATA_SECTION_2_NUMFIELDS		= 12;
const char *V_METADATA_SECTION_2_FIELDNAMES[] = {
	// type-independent fields
	"channel_description",			// utf8[511]
	"session_description",
	"recording_duration",
	// type-specific fields
	"horizontal_resolution",
	"vertical_resolution",
	"frame_rate",
	"number_of_clips",
	"maximum_clip_bytes",
	"video_format",
	"video_file_CRC",
	"protected_region",
	"discretionary_region"
};

const int METADATA_SECTION_3_NUMFIELDS		= 10;
const char *METADATA_SECTION_3_FIELDNAMES[] = {
	"recording_time_offset",
	"DST_start_time",
	"DST_end_time",
	"GMT_offset",
	"subject_name_1",				// utf8[31]
	"subject_name_2",				// utf8[31]
	"subject_ID",					// utf8[31]
	"recording_location",			// utf8[127]
	"protected_region",
	"discretionary_region"
};



///
// Functions to map c-objects to matlab-structs
///



/**
 * 	Map a MEF segment c-struct to an exising matlab-struct
 * 	
 *	The given matlab-struct have multiple entries, the MEF
 *	struct will be mapped on the given index
 *
 * 	@param segment			Pointer to the MEF segment c-struct
 * 	@param mapIndicesFlag	
 * 	@param matSegment		Pointer to the existing matlab-struct
 * 	@param matIndex			The index in the existing matlab-struct at which to map the data	
 * 	@return					Pointer to the new matlab-struct
 */
void map_mef3_segment_tostruct(SEGMENT *segment, si1 mapIndicesFlag, mxArray *matSegment, int matIndex) {

	// set segment specific properties
	mxSetField(matSegment, matIndex, "channel_type", 			mxInt32ByValue(segment->channel_type));
	//mxSetField(matSegment, matIndex, "time_series_data_fps",	mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matSegment, matIndex, "time_series_indices_fps",mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matSegment, matIndex, "video_indices_fps",		mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matSegment, matIndex, "record_data_fps",		mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matSegment, matIndex, "record_indices_fps",	mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	mxSetField(matSegment, matIndex, "name", 					mxCreateString(segment->name));
	mxSetField(matSegment, matIndex, "path", 					mxCreateString(segment->path));
	mxSetField(matSegment, matIndex, "channel_name", 			mxCreateString(segment->channel_name));
	mxSetField(matSegment, matIndex, "session_name", 			mxCreateString(segment->session_name));
	//mxSetField(matSegment, matIndex, "level_UUID", 				mxCreateString(segment->level_UUID));	// TODO: check with valid value
	
	// TODO: 
    // Read segment records if present and add it to metadata
    //if ((segment->record_indices_fps != NULL) & (segment->record_data_fps != NULL)){
    //    records_dict = map_mef3_records(segment->record_indices_fps, segment->record_data_fps);
    //    PyDict_SetItemString(metadata_dict, "records_info", records_dict);
    //}
	

	// create a metadata struct and assign it to the 'metadata_fps' field
	mxArray *segmentMetadataStruct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
	mxSetField(matSegment, matIndex, "metadata_fps", segmentMetadataStruct);
	
	// map segment metadata sections
	mxSetField(segmentMetadataStruct, 0, "section_1", map_mef3_md1(segment->metadata_fps->metadata.section_1));
	switch (segment->channel_type){
		case TIME_SERIES_CHANNEL_TYPE:
			mxSetField(segmentMetadataStruct, 0, "time_series_section_2", map_mef3_tmd2(segment->metadata_fps->metadata.time_series_section_2));
			break;
		case VIDEO_CHANNEL_TYPE:
			mxSetField(segmentMetadataStruct, 0, "video_section_2", map_mef3_vmd2(segment->metadata_fps->metadata.video_section_2));
			break;
		default:
			mexErrMsgTxt("Unrecognized channel type, exiting...");
	}  
	mxSetField(segmentMetadataStruct, 0, "section_3", map_mef3_md3(segment->metadata_fps->metadata.section_3));
	

	// TODO: more here
/*	                       

	

    // TODO - this should be a list - there is more indices in indices file!!!!

    // Set the TOC to NULL so that the logic works
    TOC = NULL;

    // Create indices dictionary
    
    switch (segment->channel_type){
        case TIME_SERIES_CHANNEL_TYPE:
            number_of_entries = segment->time_series_indices_fps->universal_header->number_of_entries;
            tsi = segment->time_series_indices_fps->time_series_indices;

            PyDict_SetItemString(metadata_dict, "indices", map_mef3_ti(tsi, number_of_entries));

            // Create TOC
            TOC = create_mef3_TOC(segment);
            break;
        case VIDEO_CHANNEL_TYPE:
            number_of_entries = segment->video_indices_fps->universal_header->number_of_entries;
            vi = segment->video_indices_fps->video_indices;

            PyDict_SetItemString(metadata_dict, "indices", map_mef3_vi(vi, number_of_entries));

            break;
        default:
            PyErr_SetString(PyExc_RuntimeError, "Unrecognized channel type, exiting...");
            PyErr_Occurred();
            return NULL;
    }

    
    if (TOC != NULL)
        PyDict_SetItemString(metadata_dict, "TOC", TOC);

    // Get universal headers
    uhs_dict = PyDict_New();

    // Metadata
    uh_dict = map_mef3_uh(segment->metadata_fps->universal_header);
    PyDict_SetItemString(uhs_dict, "metadata", uh_dict);

    // Data an indices universal headers
    switch (segment->channel_type){
        case TIME_SERIES_CHANNEL_TYPE:
            PyDict_SetItemString(uhs_dict, "time_series_data", map_mef3_uh(segment->time_series_data_fps->universal_header));
            PyDict_SetItemString(uhs_dict, "time_series_indices", map_mef3_uh(segment->time_series_indices_fps->universal_header));
            break;
        case VIDEO_CHANNEL_TYPE:
            PyDict_SetItemString(uhs_dict, "time_series_indices", map_mef3_uh(segment->video_indices_fps->universal_header));
            break;
        default:
            PyErr_SetString(PyExc_RuntimeError, "Unrecognized channel type, exiting...");
            PyErr_Occurred();
            return NULL;
    }

    PyDict_SetItemString(metadata_dict, "universal_headers", uhs_dict);

*/

	
}
 
/**
 * 	Map a MEF segment c-struct to a newly created matlab-struct
 *
 * 	@param segment			Pointer to the MEF segment c-struct
 * 	@param mapIndicesFlag	
 * 	@return					Pointer to the new matlab-struct
 */
mxArray *map_mef3_segment(SEGMENT *segment, si1 mapIndicesFlag) {
	mxArray *matSegment = mxCreateStructMatrix(1, 1, SEGMENT_NUMFIELDS, SEGMENT_FIELDNAMES);
	map_mef3_segment_tostruct(segment, mapIndicesFlag, matSegment, 0);
	return matSegment;
}


/**
 * 	Map a MEF channel c-struct to an exising matlab-struct
 * 	
 *	The given matlab-struct have multiple entries, the MEF
 *	struct will be mapped on the given index
 *  
 * 	
 *	note: this funtion also loops through segments
 *
 * 	@param channel			A pointer to the MEF channel c-struct
 * 	@param mapIndicesFlag	
 * 	@param matChannel		A pointer to the existing matlab-struct
 * 	@param matIndex			The index in the existing matlab-struct at which to map the data	
 * 	@return					A pointer to the new matlab-struct
 */
void map_mef3_channel_tostruct(CHANNEL *channel, si1 mapIndicesFlag, mxArray *matChannel, int matIndex) {
	si4   	i;
	SEGMENT *segment;

	// set channel specific properties
	mxSetField(matChannel, matIndex, "channel_type", 			mxInt32ByValue(channel->channel_type));
	//mxSetField(matChannel, matIndex, "record_data_fps", 		mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matChannel, matIndex, "record_indices_fps",	mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	mxSetField(matChannel, matIndex, "number_of_segments", 		mxInt64ByValue(channel->number_of_segments));
	mxSetField(matChannel, matIndex, "path", 					mxCreateString(channel->path));
	mxSetField(matChannel, matIndex, "name", 					mxCreateString(channel->name));
	mxSetField(matChannel, matIndex, "extension", 				mxCreateString(channel->extension));
	mxSetField(matChannel, matIndex, "session_name", 			mxCreateString(channel->session_name));
	//mxSetField(matChannel, matIndex, "level_UUID", 				mxCreateString(channel->level_UUID));	// TODO: check with valid value
	mxSetField(matChannel, matIndex, "anonymized_name", 		mxCreateString(channel->anonymized_name));
	mxSetField(matChannel, matIndex, "maximum_number_of_records", mxInt64ByValue(channel->maximum_number_of_records));
	mxSetField(matChannel, matIndex, "maximum_record_bytes", 	mxInt64ByValue(channel->maximum_record_bytes));
	mxSetField(matChannel, matIndex, "earliest_start_time", 	mxInt64ByValue(channel->earliest_start_time));
	mxSetField(matChannel, matIndex, "latest_end_time", 		mxInt64ByValue(channel->latest_end_time));
	
	// TODO: 
	// Read channel records if present and add it to metadata
	//if ((channel->record_indices_fps != NULL) & (channel->record_data_fps != NULL)){
	//	records_dict = map_mef3_records(channel->record_indices_fps, channel->record_data_fps);
	//	PyDict_SetItemString(metadata_dict, "records_info", records_dict);
	//}
	
	// create a metadata struct and assign it to the 'metadata' field
	mxArray *channelMetadataStruct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
	mxSetField(matChannel, matIndex, "metadata", channelMetadataStruct);
	
	// map channel metadata sections
	mxSetField(channelMetadataStruct, 0, "section_1", map_mef3_md1(channel->metadata.section_1));
	switch (channel->channel_type){
		case TIME_SERIES_CHANNEL_TYPE:
			mxSetField(channelMetadataStruct, 0, "time_series_section_2", map_mef3_tmd2(channel->metadata.time_series_section_2));
			break;
		case VIDEO_CHANNEL_TYPE:
			mxSetField(channelMetadataStruct, 0, "video_section_2", map_mef3_vmd2(channel->metadata.video_section_2));
			break;
		default:
			mexErrMsgTxt("Unrecognized channel type, exiting...");
	}  
	mxSetField(channelMetadataStruct, 0, "section_3", map_mef3_md3(channel->metadata.section_3));
	
	// check if there are segments for this channel
	if (channel->number_of_segments > 0) {
		
		// create a segments struct
		mxArray *segmentsStruct = mxCreateStructMatrix(1, channel->number_of_segments, SEGMENT_NUMFIELDS, SEGMENT_FIELDNAMES);
		
		// map the segments      
		for (i = 0; i < channel->number_of_segments; ++i) {
			segment = channel->segments + i;
			map_mef3_segment_tostruct(segment, mapIndicesFlag, segmentsStruct, i);
		}
		
		// assign the channels matlab-struct to the 'segments' field
		mxSetField(matChannel, matIndex, "segments", segmentsStruct);
		
	}
	
}
 
/**
 * 	Map a MEF channel c-struct to a newly created matlab-struct
 * 	
 *	note: this funtion also loops through segments
 *
 * 	@param channel			A pointer to the MEF channel c-struct
 * 	@param mapIndicesFlag	
 * 	@return					A pointer to the new matlab-struct
 */
mxArray *map_mef3_channel(CHANNEL *channel, si1 mapIndicesFlag) {
	mxArray *matChannel = mxCreateStructMatrix(1, 1, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);
	map_mef3_channel_tostruct(channel, mapIndicesFlag, matChannel, 0);
	return matChannel;
}

/**
 * 	Map a MEF session c-struct to a newly created matlab-struct
 *
 * 	@param session			A pointer to the MEF session c-struct
 * 	@param mapIndicesFlag	
 * 	@return					A pointer to the new matlab-struct
 */
mxArray *map_mef3_session(SESSION *session, si1 mapIndicesFlag) {
    si4   	i;
	CHANNEL *channel;
	
	// create the a matlab 'session' struct
	mxArray *matSession = mxCreateStructMatrix(1, 1, SESSION_NUMFIELDS, SESSION_FIELDNAMES);
	
	// set session-specific properties
	mxSetField(matSession, 0, "number_of_time_series_channels", mxInt32ByValue(session->number_of_time_series_channels));
	mxSetField(matSession, 0, "number_of_video_channels",		mxInt32ByValue(session->number_of_video_channels));
	
	mxSetField(matSession, 0, "name", 							mxCreateString(session->name));
	mxSetField(matSession, 0, "path", 							mxCreateString(session->path));
	mxSetField(matSession, 0, "anonymized_name", 				mxCreateString(session->anonymized_name));
	//mxSetField(matSession, 0, "level_UUID", 					mxCreateString(session->level_UUID));	// TODO: check with valid value

	mxSetField(matSession, 0, "maximum_number_of_records", 		mxInt64ByValue(session->maximum_number_of_records));
	mxSetField(matSession, 0, "maximum_record_bytes", 			mxInt64ByValue(session->maximum_record_bytes));
	mxSetField(matSession, 0, "earliest_start_time", 			mxInt64ByValue(session->earliest_start_time));
	mxSetField(matSession, 0, "latest_end_time", 				mxInt64ByValue(session->latest_end_time));
	
	
	// TODO: need file with session records to test
	//mxSetField(matSession, 0, "record_data_fps", 				mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
	//mxSetField(matSession, 0, "record_indices_fps", 			mxCreateStructMatrix(1, 1, FILE_PROCESSING_NUMFIELDS, FILE_PROCESSING_FIELDNAMES));	// TODO: fill later
    // Read session records if present and add it to metadata
    //if ((session->record_indices_fps != NULL) & (session->record_data_fps != NULL)){
    //    records_dict = map_mef3_records(session->record_indices_fps, session->record_data_fps);
    //    PyDict_SetItemString(metadata_dict, "records_info", records_dict);
    //}
	
    // check if there are time series channels
    if (session->number_of_time_series_channels > 0) {
		
		//
		// time_series_metadata
		//
		
		// create a metadata struct and assign it to the 'time_series_metadata' field
		mxArray *timeSeriesMetadataStruct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
		mxSetField(matSession, 0, "time_series_metadata", timeSeriesMetadataStruct);
		
		// map metadata sections
		mxSetField(timeSeriesMetadataStruct, 0, "section_1", map_mef3_md1(session->time_series_metadata.section_1));
		mxSetField(timeSeriesMetadataStruct, 0, "time_series_section_2", map_mef3_tmd2(session->time_series_metadata.time_series_section_2));
		mxSetField(timeSeriesMetadataStruct, 0, "section_3", map_mef3_md3(session->time_series_metadata.section_3));
		
		
		//
		// time_series_channels
		// 
		
		
		// create a channels struct
		mxArray *channelsStruct = mxCreateStructMatrix(1, session->number_of_time_series_channels, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);

		// add extra fields
		mxAddField(channelsStruct, "records_info");		// will be left empty if there are no records
		// note: segments field already exists in struct
		
		// map the time-serie channels      
		for (i = 0; i < session->number_of_time_series_channels; ++i) {
			channel = session->time_series_channels + i;
			map_mef3_channel_tostruct(channel, mapIndicesFlag, channelsStruct, i);
		}
		
		// assign the channels matlab-struct to the 'time_series_channels' field
		mxSetField(matSession, 0, "time_series_channels", channelsStruct);

	}
	
    // check if there are video channels
    if (session->number_of_video_channels > 0) {

		//
		// video_metadata
		//
	
		// create a metadata struct and assign it to the 'video_metadata' field
		mxArray *videoMetadataStruct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
		mxSetField(matSession, 0, "video_metadata", videoMetadataStruct);
		
		// map metadata sections
		mxSetField(videoMetadataStruct, 0, "section_1", map_mef3_md1(session->video_metadata.section_1));
		mxSetField(videoMetadataStruct, 0, "video_section_2", map_mef3_vmd2(session->video_metadata.video_section_2));
		mxSetField(videoMetadataStruct, 0, "section_3", map_mef3_md3(session->video_metadata.section_3));
		
		
		//
		// video_channels
		// 
		
		// create a channels struct
		mxArray *channelsStruct = mxCreateStructMatrix(1, session->number_of_video_channels, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);

		// add extra fields
		mxAddField(channelsStruct, "records_info");		// will be left empty if there are no records
		// note: segments field already exists in struct
		
		// map the video channels      
		for (i = 0; i < session->number_of_video_channels; ++i) {
			channel = session->video_channels + i;
			map_mef3_channel_tostruct(channel, mapIndicesFlag, channelsStruct, i);
		}
		
		// assign the channels matlab-struct to the 'video_channels' field
		mxSetField(matSession, 0, "video_channels", channelsStruct);
		
    }
	
	// return the output-struct
	return matSession;
	
}

mxArray *map_mef3_md1(METADATA_SECTION_1 *md1) {

    mxArray *matMd = mxCreateStructMatrix(1, 1, METADATA_SECTION_1_NUMFIELDS, METADATA_SECTION_1_FIELDNAMES);
	//mxSetField(matMd, 0, "section_2_encryption", 			mxCreateString(md1->section_2_encryption));	// TODO: check with valid value
	//mxSetField(matMd, 0, "section_3_encryption", 			mxCreateString(md1->section_3_encryption));	// TODO: check with valid value

	// return the struct
	return matMd;
}

mxArray *map_mef3_tmd2(TIME_SERIES_METADATA_SECTION_2 *tmd2) {

    mxArray *matMd = mxCreateStructMatrix(1, 1, TS_METADATA_SECTION_2_NUMFIELDS, TS_METADATA_SECTION_2_FIELDNAMES);
	
	
	mxSetField(matMd, 0, "channel_description", 			mxCreateString(tmd2->channel_description));
	mxSetField(matMd, 0, "session_description", 			mxCreateString(tmd2->session_description));
	
	mxSetField(matMd, 0, "recording_duration", 				mxInt64ByValue(tmd2->recording_duration));
	mxSetField(matMd, 0, "reference_description", 			mxCreateString(tmd2->reference_description));
	mxSetField(matMd, 0, "acquisition_channel_number", 		mxInt64ByValue(tmd2->acquisition_channel_number));
	mxSetField(matMd, 0, "sampling_frequency", 				mxDoubleByValue(tmd2->sampling_frequency));
	mxSetField(matMd, 0, "low_frequency_filter_setting", 	mxDoubleByValue(tmd2->low_frequency_filter_setting));
	mxSetField(matMd, 0, "high_frequency_filter_setting", 	mxDoubleByValue(tmd2->high_frequency_filter_setting));
	mxSetField(matMd, 0, "notch_filter_frequency_setting", 	mxDoubleByValue(tmd2->notch_filter_frequency_setting));
	mxSetField(matMd, 0, "AC_line_frequency", 				mxDoubleByValue(tmd2->AC_line_frequency));
	mxSetField(matMd, 0, "units_conversion_factor", 		mxDoubleByValue(tmd2->units_conversion_factor));
	mxSetField(matMd, 0, "units_description", 				mxCreateString(tmd2->units_description));
	mxSetField(matMd, 0, "maximum_native_sample_value", 	mxDoubleByValue(tmd2->maximum_native_sample_value));
	mxSetField(matMd, 0, "minimum_native_sample_value", 	mxDoubleByValue(tmd2->minimum_native_sample_value));
	mxSetField(matMd, 0, "start_sample", 					mxInt64ByValue(tmd2->start_sample));
	mxSetField(matMd, 0, "number_of_samples", 				mxInt64ByValue(tmd2->number_of_samples));
	mxSetField(matMd, 0, "number_of_blocks", 				mxInt64ByValue(tmd2->number_of_blocks));
	mxSetField(matMd, 0, "maximum_block_bytes", 			mxInt64ByValue(tmd2->maximum_block_bytes));
	mxSetField(matMd, 0, "maximum_block_samples", 			mxUInt32ByValue(tmd2->maximum_block_samples));
	mxSetField(matMd, 0, "maximum_difference_bytes", 		mxUInt32ByValue(tmd2->maximum_difference_bytes));	
	mxSetField(matMd, 0, "block_interval", 					mxInt64ByValue(tmd2->block_interval));
	mxSetField(matMd, 0, "number_of_discontinuities", 		mxInt64ByValue(tmd2->number_of_discontinuities));
	mxSetField(matMd, 0, "maximum_contiguous_blocks", 		mxInt64ByValue(tmd2->maximum_contiguous_blocks));
	mxSetField(matMd, 0, "maximum_contiguous_block_bytes", 	mxInt64ByValue(tmd2->maximum_contiguous_block_bytes));
	mxSetField(matMd, 0, "maximum_contiguous_samples", 		mxInt64ByValue(tmd2->maximum_contiguous_samples));
	
	// return the struct
	return matMd;
}

mxArray *map_mef3_vmd2(VIDEO_METADATA_SECTION_2 *vmd2) {

    mxArray *matMd = mxCreateStructMatrix(1, 1, V_METADATA_SECTION_2_NUMFIELDS, V_METADATA_SECTION_2_FIELDNAMES);
	
	mxSetField(matMd, 0, "channel_description", 			mxCreateString(vmd2->channel_description));
	mxSetField(matMd, 0, "session_description", 			mxCreateString(vmd2->session_description));
	
	mxSetField(matMd, 0, "recording_duration", 				mxInt64ByValue(vmd2->recording_duration));
	mxSetField(matMd, 0, "horizontal_resolution", 			mxInt64ByValue(vmd2->horizontal_resolution));
	mxSetField(matMd, 0, "vertical_resolution", 			mxInt64ByValue(vmd2->vertical_resolution));
	mxSetField(matMd, 0, "frame_rate", 						mxDoubleByValue(vmd2->frame_rate));
	mxSetField(matMd, 0, "number_of_clips", 				mxInt64ByValue(vmd2->number_of_clips));
	mxSetField(matMd, 0, "maximum_clip_bytes", 				mxInt64ByValue(vmd2->maximum_clip_bytes));
	mxSetField(matMd, 0, "video_format", 					mxCreateString(vmd2->video_format));
	//mxSetField(matMd, 0, "video_file_CRC", 				mxUInt32ByValue(vmd2->video_file_CRC));			// TODO: check with valid value, leave empty for now
	
	// return the struct
	return matMd;
}

mxArray *map_mef3_md3(METADATA_SECTION_3 *md3) {

    mxArray *matMd = mxCreateStructMatrix(1, 1, METADATA_SECTION_3_NUMFIELDS, METADATA_SECTION_3_FIELDNAMES);
	
	mxSetField(matMd, 0, "recording_time_offset", 			mxInt64ByValue(md3->recording_time_offset));
	mxSetField(matMd, 0, "DST_start_time", 					mxInt64ByValue(md3->DST_start_time));
	mxSetField(matMd, 0, "DST_end_time", 					mxInt64ByValue(md3->DST_end_time));
	mxSetField(matMd, 0, "GMT_offset", 						mxInt32ByValue(md3->GMT_offset));
	mxSetField(matMd, 0, "subject_name_1", 					mxCreateString(md3->subject_name_1));
	mxSetField(matMd, 0, "subject_name_2", 					mxCreateString(md3->subject_name_2));
	mxSetField(matMd, 0, "subject_ID", 						mxCreateString(md3->subject_ID));
	mxSetField(matMd, 0, "recording_location", 				mxCreateString(md3->recording_location));
	
	// return the struct
	return matMd;
}

