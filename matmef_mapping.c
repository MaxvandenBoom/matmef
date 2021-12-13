/**
 * 	@file
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Functions to map MEF3 objects and structures to Matlab structures
 *	
 *  Copyright 2020, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
 *	Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)
 *	
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "matmef_mapping.h"
#include "mex.h"
#include "matmef_dataconverter.h"

#include "meflib/meflib/meflib.h"


///
// Fields definitions for MEF3 matlab structs
///


// Universal Header Structures
const int UNIVERSAL_HEADER_NUMFIELDS		= 20;
const char *UNIVERSAL_HEADER_FIELDNAMES[] 	= {	
	"header_CRC",
	"body_CRC",
	"file_type_string",
	"mef_version_major",
	"mef_version_minor",
	"byte_order_code",
	"start_time",
	"end_time",
	"number_of_entries",
	"maximum_entry_size",
	"segment_number",
	"channel_name",					// utf8[63], base name only, no extension
	"session_name",					// utf8[63], base name only, no extension
	"anonymized_name",				// utf8[63]
	"level_UUID",
	"file_UUID",
	"provenance_UUID",
	"level_1_password_validation_field",
	"level_2_password_validation_field",
	//"protected_region",			// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};

// Session, Channel, Segment Processing Structures
const int SEGMENT_NUMFIELDS			= 12;
const char *SEGMENT_FIELDNAMES[] 	= {	
	"channel_type",
	//"metadata_fps",				// instead of mapping the FILE_PROCESSING_STRUCT object, the data held within is mapped to the 'metadata_uh' field
	//"time_series_data_fps",		// instead of mapping the FILE_PROCESSING_STRUCT object, the data held within is mapped to the 'time_series_data_uh' field
	//"time_series_indices_fps",	// instead of mapping the FILE_PROCESSING_STRUCT object, the (number of) time-series indices are mapped to the 'time_series_indices' field
	//"video_indices_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the (number of) video indices are mapped to the 'video_indices' field
	//"record_data_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the (number of) data-records are mapped to the 'records' field
	//"record_indices_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the (number of) records-indices held within are mapped to the 'records' field
	"name",							// just base name, no extension
	"path",							// full path to enclosing directory (channel directory)
	"channel_name",					// just base name, no extension
	"session_name",					// just base name, no extension
	"level_UUID",
	
	// non-meflib fields, added for mapping (contains data normally held within the _fps structs)
	"metadata",
	"time_series_indices",
	"video_indices",
	"records",
	"metadata_uh",
	"time_series_data_uh"
};

const int CHANNEL_NUMFIELDS			= 15;
const char *CHANNEL_FIELDNAMES[] 	= {	
	"channel_type",
	"metadata",
	//"record_data_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the records-data held within is mapped to the 'records' field
	//"record_indices_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the records-indices held within are mapped to the 'records' field
	"number_of_segments",
	"segments",
	"path",							// full path to enclosing directory (session directory)
	"name",							// just base name, no extension
	"extension",					// channel directory extension
	"session_name",					// just base name, no extension
	"level_UUID",
	"anonymized_name",
	// variables below refer to segments
	"maximum_number_of_records",
	"maximum_record_bytes",
	"earliest_start_time",
	"latest_end_time",
	
	// non-meflib fields, added for mapping (contains data normally held within the _fps structs)
	"records"
};

const int SESSION_NUMFIELDS			= 15;
const char *SESSION_FIELDNAMES[] 	= {	
	"time_series_metadata",
	"number_of_time_series_channels",
	"time_series_channels",
	"video_metadata",
	"number_of_video_channels",
	"video_channels",
	//"record_data_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the record-data held within is mapped to the 'records' field
	//"record_indices_fps",			// instead of mapping the FILE_PROCESSING_STRUCT object, the record-indices held within are mapped to the 'records' field
	"name",
	"path",
	"anonymized_name",
	"level_UUID",
	// below variables refer to channels
	"maximum_number_of_records",
	"maximum_record_bytes",
	"earliest_start_time",
	"latest_end_time",

	// non-meflib fields, added for mapping (contains data normally held within the _fps structs)
	"records"
};


// Metadata Structures
const int METADATA_SECTION_1_NUMFIELDS		= 3;
const char *METADATA_SECTION_1_FIELDNAMES[] = {
	"section_2_encryption",
	"section_3_encryption",
	//"protected_region",			// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};

const int TIME_SERIES_METADATA_SECTION_2_NUMFIELDS		= 26;
const char *TIME_SERIES_METADATA_SECTION_2_FIELDNAMES[] = {
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
	//"protected_region",			// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};

const int VIDEO_METADATA_SECTION_2_NUMFIELDS		= 11;
const char *VIDEO_METADATA_SECTION_2_FIELDNAMES[] 	= {
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
	//"protected_region",			// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};

const int METADATA_SECTION_3_NUMFIELDS		= 9;
const char *METADATA_SECTION_3_FIELDNAMES[] = {
	"recording_time_offset",
	"DST_start_time",
	"DST_end_time",
	"GMT_offset",
	"subject_name_1",				// utf8[31]
	"subject_name_2",				// utf8[31]
	"subject_ID",					// utf8[31]
	"recording_location",			// utf8[127]
	//"protected_region",			// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};

const int METADATA_NUMFIELDS 		= 3;
const char *METADATA_FIELDNAMES[] 	= {
	"section_1",
	"section_2",					// non-meflib field, added for mapping 'time_series_section_2' or 'video_section_2' (since it is always one or the other)
	//"time_series_section_2",		// instead of mapping the time_series_section_2 object, the data is mapped to the 'section_2' field for time-series
	//"video_section_2",			// instead of mapping the video_section_2 object, the data is mapped to the 'section_2' field for video
	"section_3"	
};


// Record Structures
const int RECORD_HEADER_NUMFIELDS		= 7;
const char *RECORD_HEADER_FIELDNAMES[] 	= {	
	"record_CRC",
	"type_string",
	"version_major",
	"version_minor",
	"encryption",
	"bytes",
	"time"
};

const int RECORD_INDEX_NUMFIELDS		= 6;
const char *RECORD_INDEX_FIELDNAMES[] 	= {	
	"type_string",
	"version_major",
	"version_minor",
	"encryption",
	"file_offset",
	"time"
};


// Block Indices Structures
const int TIME_SERIES_INDEX_NUMFIELDS		= 9;
const char *TIME_SERIES_INDEX_FIELDNAMES[] 	= {	
	"file_offset",
	"start_time",
	"start_sample",
	"number_of_samples",
	"block_bytes",
	"maximum_sample_value",
	"minimum_sample_value",
	//"protected_region",				// (not mapped, reserved, should not be used by end users)
	"RED_block_flags",
	//"RED_block_protected_region",		// (not mapped, reserved, should not be used by end users)
	"RED_block_discretionary_region"
};


// Frame Indices Structures
const int VIDEO_INDEX_NUMFIELDS			= 9;
const char *VIDEO_INDEX_FIELDNAMES[] 	= {
	"start_time",
	"end_time",
	"start_frame",
	"end_frame",
	"file_offset",
	"clip_bytes",
	//"protected_region",				// (not mapped, reserved, should not be used by end users)
	"discretionary_region"
};


// File Processing Structures
const int FILE_PROCESSINGFILE_PROCESSING_NUMFIELDS		= 16;
const char *FILE_PROCESSING_FIELDNAMES[]				= {
	"full_file_name",					// full path including extension
	"fp",								// runtime file pointer		(not mapped)
	"fd",								// runtime file descriptor	(not mapped)
	"file_length",
	"file_type_code",
	"universal_header",
	"directives",						// runtime struct 	(not mapped)
	"password_data",					// this will often be the same for all files
	"metadata",
	"time_series_indices",
	"video_indices",
	"records",
	"record_indices",
	"RED_blocks",						// only used when type = TIME_SERIES_DATA_FILE_TYPE_CODE; (not mapped)
	"raw_data_bytes",
	"raw_data"							// (not mapped)
};	


// Record Type Structures
const int MEFREC_EDFA_1_0_NUMFIELDS			= 1;
const char *MEFREC_EDFA_1_0_FIELDNAMES[] 	= {	
	"duration"
};

const int MEFREC_LNTP_1_0_NUMFIELDS			= 1;
const char *MEFREC_LNTP_1_0_FIELDNAMES[] 	= {	
	"length"
};

const int MEFREC_SEIZ_1_0_NUMFIELDS			= 8;
const char *MEFREC_SEIZ_1_0_FIELDNAMES[] 	= {	
	"earliest_onset",
	"latest_offset",
	"duration",
	"number_of_channels",
	"onset_code",
	"marker_name_1",
	"marker_name_2",
	"annotation"
};

const int MEFREC_CSTI_1_0_NUMFIELDS			= 4;
const char *MEFREC_CSTI_1_0_FIELDNAMES[] 	= {	
	"task_type",
	"stimulus_duration",
	"stimulus_type",
	"patient_response"
};

const int MEFREC_ESTI_1_0_NUMFIELDS			= 8;
const char *MEFREC_ESTI_1_0_FIELDNAMES[] 	= {	
	"amplitude",
	"frequency",
	"pulse_width",
	"ampunit_code",
	"mode_code",
	"waveform",
	"anode",
	"catode"
};

const int MEFREC_CURS_1_0_NUMFIELDS			= 5;
const char *MEFREC_CURS_1_0_FIELDNAMES[] 	= {	
	"id_number",
	"trace_timestamp",
	"latency",
	"value",
	"name"
};

const int MEFREC_EPOC_1_0_NUMFIELDS			= 6;
const char *MEFREC_EPOC_1_0_FIELDNAMES[] 	= {	
	"id_number",
	"timestamp",
	"end_timestamp",
	"duration",
	"type",
	"text"
};



///
// Functions to create new and initialized matlab-structs
///

/**
 * 	Create and initialize a new MEF universal-header matlab-struct
 *
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *create_init_matlab_uh() {
	
    mxArray *mat_uh = mxCreateStructMatrix(1, 1, UNIVERSAL_HEADER_NUMFIELDS, UNIVERSAL_HEADER_FIELDNAMES);
	
	mxSetField(mat_uh, 0, "header_CRC", 					mxUint32ByValue(UNIVERSAL_HEADER_HEADER_CRC_NO_ENTRY));
	mxSetField(mat_uh, 0, "body_CRC", 						mxUint32ByValue(UNIVERSAL_HEADER_BODY_CRC_NO_ENTRY));
	mxSetField(mat_uh, 0, "file_type_string", 				mxCreateString(""));
	mxSetField(mat_uh, 0, "mef_version_major", 				mxUint8ByValue(UNIVERSAL_HEADER_MEF_VERSION_MAJOR_NO_ENTRY));
	mxSetField(mat_uh, 0, "mef_version_minor", 				mxUint8ByValue(UNIVERSAL_HEADER_MEF_VERSION_MINOR_NO_ENTRY));
	mxSetField(mat_uh, 0, "byte_order_code", 				mxUint8ByValue(UNIVERSAL_HEADER_BYTE_ORDER_CODE_NO_ENTRY));
	mxSetField(mat_uh, 0, "start_time", 					mxInt64ByValue(UNIVERSAL_HEADER_START_TIME_NO_ENTRY));
	mxSetField(mat_uh, 0, "end_time", 						mxInt64ByValue(UNIVERSAL_HEADER_END_TIME_NO_ENTRY));
	mxSetField(mat_uh, 0, "number_of_entries", 				mxInt64ByValue(UNIVERSAL_HEADER_NUMBER_OF_ENTRIES_NO_ENTRY));
	mxSetField(mat_uh, 0, "maximum_entry_size", 			mxInt64ByValue(UNIVERSAL_HEADER_MAXIMUM_ENTRY_SIZE_NO_ENTRY));
	mxSetField(mat_uh, 0, "segment_number", 				mxInt32ByValue(UNIVERSAL_HEADER_SEGMENT_NUMBER_NO_ENTRY));
	mxSetField(mat_uh, 0, "channel_name", 					mxCreateString(""));
	mxSetField(mat_uh, 0, "session_name", 					mxCreateString(""));
	mxSetField(mat_uh, 0, "anonymized_name", 				mxCreateString(""));
	mxSetField(mat_uh, 0, "level_UUID", 					mxCreateNumericMatrix(1, UUID_BYTES, mxUINT8_CLASS, mxREAL));
	mxSetField(mat_uh, 0, "file_UUID", 						mxCreateNumericMatrix(1, UUID_BYTES, mxUINT8_CLASS, mxREAL));
	mxSetField(mat_uh, 0, "provenance_UUID", 				mxCreateNumericMatrix(1, UUID_BYTES, mxUINT8_CLASS, mxREAL));
	mxSetField(mat_uh, 0, "level_1_password_validation_field", 	mxCreateNumericMatrix(1, PASSWORD_VALIDATION_FIELD_BYTES, mxUINT8_CLASS, mxREAL));
	mxSetField(mat_uh, 0, "level_2_password_validation_field", 	mxCreateNumericMatrix(1, PASSWORD_VALIDATION_FIELD_BYTES, mxUINT8_CLASS, mxREAL));
	mxSetField(mat_uh, 0, "discretionary_region", 			mxCreateNumericMatrix(1, UNIVERSAL_HEADER_DISCRETIONARY_REGION_BYTES, mxUINT8_CLASS, mxREAL));
	
	// return the struct
	return mat_uh;
}

/**
 * 	Create and initialize a new MEF time-series section 2 metadata matlab-struct
 *
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *create_init_matlab_tmd2() {
	
    mxArray *mat_md = mxCreateStructMatrix(1, 1, TIME_SERIES_METADATA_SECTION_2_NUMFIELDS, TIME_SERIES_METADATA_SECTION_2_FIELDNAMES);
	
	// type-independent fields
	mxSetField(mat_md, 0, "channel_description", 			mxCreateString(""));
	mxSetField(mat_md, 0, "session_description", 			mxCreateString(""));
	mxSetField(mat_md, 0, "recording_duration", 			mxInt64ByValue(METADATA_RECORDING_DURATION_NO_ENTRY));
	
	// type-specific fields
	mxSetField(mat_md, 0, "reference_description", 			mxCreateString(""));
	mxSetField(mat_md, 0, "acquisition_channel_number", 	mxInt64ByValue(TIME_SERIES_METADATA_ACQUISITION_CHANNEL_NUMBER_NO_ENTRY));
	mxSetField(mat_md, 0, "sampling_frequency", 			mxDoubleByValue(TIME_SERIES_METADATA_SAMPLING_FREQUENCY_NO_ENTRY));
	mxSetField(mat_md, 0, "low_frequency_filter_setting", 	mxDoubleByValue(TIME_SERIES_METADATA_LOW_FREQUENCY_FILTER_SETTING_NO_ENTRY));
	mxSetField(mat_md, 0, "high_frequency_filter_setting", 	mxDoubleByValue(TIME_SERIES_METADATA_HIGH_FREQUENCY_FILTER_SETTING_NO_ENTRY));
	mxSetField(mat_md, 0, "notch_filter_frequency_setting", mxDoubleByValue(TIME_SERIES_METADATA_NOTCH_FILTER_FREQUENCY_SETTING_NO_ENTRY));
	mxSetField(mat_md, 0, "AC_line_frequency", 				mxDoubleByValue(TIME_SERIES_METADATA_AC_LINE_FREQUENCY_NO_ENTRY));
	mxSetField(mat_md, 0, "units_conversion_factor", 		mxDoubleByValue(TIME_SERIES_METADATA_UNITS_CONVERSION_FACTOR_NO_ENTRY));
	mxSetField(mat_md, 0, "units_description", 				mxCreateString(""));
	mxSetField(mat_md, 0, "maximum_native_sample_value", 	mxDoubleByValue(TIME_SERIES_METADATA_MAXIMUM_NATIVE_SAMPLE_VALUE_NO_ENTRY));
	mxSetField(mat_md, 0, "minimum_native_sample_value", 	mxDoubleByValue(TIME_SERIES_METADATA_MINIMUM_NATIVE_SAMPLE_VALUE_NO_ENTRY));
	mxSetField(mat_md, 0, "start_sample", 					mxInt64ByValue(TIME_SERIES_METADATA_START_SAMPLE_NO_ENTRY));
	mxSetField(mat_md, 0, "number_of_samples", 				mxInt64ByValue(TIME_SERIES_METADATA_NUMBER_OF_SAMPLES_NO_ENTRY));
	mxSetField(mat_md, 0, "number_of_blocks", 				mxInt64ByValue(TIME_SERIES_METADATA_NUMBER_OF_BLOCKS_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_block_bytes", 			mxInt64ByValue(TIME_SERIES_METADATA_MAXIMUM_BLOCK_BYTES_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_block_samples", 			mxUint32ByValue(TIME_SERIES_METADATA_MAXIMUM_BLOCK_SAMPLES_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_difference_bytes", 		mxUint32ByValue(TIME_SERIES_METADATA_MAXIMUM_DIFFERENCE_BYTES_NO_ENTRY));	
	mxSetField(mat_md, 0, "block_interval", 				mxInt64ByValue(TIME_SERIES_METADATA_BLOCK_INTERVAL_NO_ENTRY));
	mxSetField(mat_md, 0, "number_of_discontinuities", 		mxInt64ByValue(TIME_SERIES_METADATA_NUMBER_OF_DISCONTINUITIES_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_contiguous_blocks", 		mxInt64ByValue(TIME_SERIES_METADATA_MAXIMUM_CONTIGUOUS_BLOCKS_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_contiguous_block_bytes", mxInt64ByValue(TIME_SERIES_METADATA_MAXIMUM_CONTIGUOUS_BLOCK_BYTES_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_contiguous_samples", 	mxInt64ByValue(TIME_SERIES_METADATA_MAXIMUM_CONTIGUOUS_SAMPLES_NO_ENTRY));
    mxSetField(mat_md, 0, "discretionary_region", 			mxCreateNumericMatrix(1, TIME_SERIES_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES, mxUINT8_CLASS, mxREAL));
	
	// return the struct
	return mat_md;
}

/**
 * 	Create and initialize a new MEF video section 2 metadata matlab-struct
 *
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *create_init_matlab_vmd2() {
	
    mxArray *mat_md = mxCreateStructMatrix(1, 1, VIDEO_METADATA_SECTION_2_NUMFIELDS, VIDEO_METADATA_SECTION_2_FIELDNAMES);
	
	// type-independent fields
	mxSetField(mat_md, 0, "channel_description", 			mxCreateString(""));
	mxSetField(mat_md, 0, "session_description", 			mxCreateString(""));
	mxSetField(mat_md, 0, "recording_duration", 			mxInt64ByValue(METADATA_RECORDING_DURATION_NO_ENTRY));
	
	// type-specific fields
	mxSetField(mat_md, 0, "horizontal_resolution", 			mxInt64ByValue(VIDEO_METADATA_HORIZONTAL_RESOLUTION_NO_ENTRY));
	mxSetField(mat_md, 0, "vertical_resolution", 			mxInt64ByValue(VIDEO_METADATA_VERTICAL_RESOLUTION_NO_ENTRY));
	mxSetField(mat_md, 0, "frame_rate", 					mxDoubleByValue(VIDEO_METADATA_FRAME_RATE_NO_ENTRY));
	mxSetField(mat_md, 0, "number_of_clips", 				mxInt64ByValue(VIDEO_METADATA_NUMBER_OF_CLIPS_NO_ENTRY));
	mxSetField(mat_md, 0, "maximum_clip_bytes", 			mxInt64ByValue(VIDEO_METADATA_MAXIMUM_CLIP_BYTES_NO_ENTRY));
	mxSetField(mat_md, 0, "video_format", 					mxCreateString(""));
	mxSetField(mat_md, 0, "video_file_CRC", 				mxUint32ByValue(VIDEO_METADATA_VIDEO_FILE_CRC_NO_ENTRY));
    mxSetField(mat_md, 0, "discretionary_region", 			mxCreateNumericMatrix(1, VIDEO_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES, mxUINT8_CLASS, mxREAL));
	
	// return the struct
	return mat_md;
	
}

/**
 * 	Create and initialize a new MEF section 3 metadata matlab-struct
 *
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *create_init_matlab_md3() {
	
    mxArray *mat_md = mxCreateStructMatrix(1, 1, METADATA_SECTION_3_NUMFIELDS, METADATA_SECTION_3_FIELDNAMES);
	
	mxSetField(mat_md, 0, "recording_time_offset", 			mxInt64ByValue(METADATA_RECORDING_TIME_OFFSET_NO_ENTRY));
	mxSetField(mat_md, 0, "DST_start_time", 				mxInt64ByValue(METADATA_DST_START_TIME_NO_ENTRY));
	mxSetField(mat_md, 0, "DST_end_time", 					mxInt64ByValue(METADATA_DST_END_TIME_NO_ENTRY));
	mxSetField(mat_md, 0, "GMT_offset", 					mxInt32ByValue(GMT_OFFSET_NO_ENTRY));
	mxSetField(mat_md, 0, "subject_name_1", 				mxCreateString(""));
	mxSetField(mat_md, 0, "subject_name_2", 				mxCreateString(""));
	mxSetField(mat_md, 0, "subject_ID", 					mxCreateString(""));
	mxSetField(mat_md, 0, "recording_location", 			mxCreateString(""));
    mxSetField(mat_md, 0, "discretionary_region", 			mxCreateNumericMatrix(1, METADATA_SECTION_3_DISCRETIONARY_REGION_BYTES, mxUINT8_CLASS, mxREAL));
	
	// return the struct
	return mat_md;
	
}



///
// Functions to map c-objects to matlab-structs
///

/**
 * 	Map a MEF segment c-struct to an exising matlab-struct
 * 	
 *	The given matlab-struct have multiple entries, the MEF
 *	struct will be mapped on the given index
 *
 * 	@param segment				Pointer to the MEF segment c-struct
 * 	@param map_indices_flag		Whether to map the time-series and video indices
 * 	@param mat_segment			Pointer to the existing matlab-struct
 * 	@param mat_index			The index in the existing matlab-struct at which to map the data	
 */
void map_mef3_segment_tostruct(SEGMENT *segment, si1 map_indices_flag, mxArray *mat_segment, int mat_index) {

	// set segment properties
	mxSetField(mat_segment, mat_index, "channel_type", 				mxInt32ByValue(segment->channel_type));
	mxSetField(mat_segment, mat_index, "name", 						mxCreateString(segment->name));
	mxSetField(mat_segment, mat_index, "path", 						mxCreateString(segment->path));
	mxSetField(mat_segment, mat_index, "channel_name", 				mxCreateString(segment->channel_name));
	mxSetField(mat_segment, mat_index, "session_name", 				mxCreateString(segment->session_name));
	mxSetField(mat_segment, mat_index, "level_UUID", 				mxUint8ArrayByValue(segment->level_UUID, UUID_BYTES));
	
	
	//
	// universal headers (from file processing struct)
	//
	
	mxSetField(mat_segment, mat_index, "metadata_uh", 					map_mef3_uh(segment->metadata_fps->universal_header));
	if (segment->channel_type == TIME_SERIES_CHANNEL_TYPE)
		mxSetField(mat_segment, mat_index, "time_series_data_uh", 		map_mef3_uh(segment->time_series_data_fps->universal_header));

	
	//
	// records
	//
	
	// check segment records and (if existent) add
    if ((segment->record_indices_fps != NULL) & (segment->record_data_fps != NULL))
		mxSetField(mat_segment, mat_index, "records", map_mef3_records(segment->record_indices_fps, segment->record_data_fps));
	
	
	//
	// metadata
	//
	
	// create a metadata struct (for the segment) and assign it to the 'metadata' field
	mxArray *segment_metadata_struct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
	mxSetField(mat_segment, mat_index, "metadata", segment_metadata_struct);
	
	// map segment metadata sections
	mxSetField(segment_metadata_struct, 0, "section_1", map_mef3_md1(segment->metadata_fps->metadata.section_1));
	switch (segment->channel_type){
		case TIME_SERIES_CHANNEL_TYPE:
			mxSetField(segment_metadata_struct, 0, "section_2", map_mef3_tmd2(segment->metadata_fps->metadata.time_series_section_2));
			break;
		case VIDEO_CHANNEL_TYPE:
			mxSetField(segment_metadata_struct, 0, "section_2", map_mef3_vmd2(segment->metadata_fps->metadata.video_section_2));
			break;
		default:
			mexErrMsgTxt("Unrecognized channel type, exiting...");
	}  
	mxSetField(segment_metadata_struct, 0, "section_3", map_mef3_md3(segment->metadata_fps->metadata.section_3));
	
	
	//
	// map indices
	//
		
	// check if indices should be mapped
	if (map_indices_flag) {
		
		switch (segment->channel_type){
			case TIME_SERIES_CHANNEL_TYPE:
	
				// check if the time-series indices are available
				if (segment->time_series_indices_fps != NULL) {
					
					// create a time-series indices struct (for the segment) and assign it to the 'time_series_indices' field
					mxSetField(	mat_segment, 
								mat_index, 
								"time_series_indices", 
								map_mef3_ti(	segment->time_series_indices_fps->time_series_indices,
												segment->time_series_indices_fps->universal_header->number_of_entries));
				}
					
				break;
			case VIDEO_CHANNEL_TYPE:
				
				// check if the video indices are available
				if (segment->video_indices_fps != NULL) {
					
					// create a video indices struct (for the segment) and assign it to the 'video_indices' field
					mxSetField(	mat_segment, 
								mat_index, 
								"video_indices", 
								map_mef3_vi(	segment->video_indices_fps->video_indices,
												segment->video_indices_fps->universal_header->number_of_entries));
				}
				
				break;
			default:
				mexErrMsgTxt("Unrecognized channel type, exiting...");
				
		}
	}
	
}
 
/**
 * 	Map a MEF segment c-struct to a newly created matlab-struct
 *
 * 	@param segment				Pointer to the MEF segment c-struct
 * 	@param map_indices_flag		Whether to map the time-series and video indices
 * 	@return						Pointer to the new matlab-struct
 */
mxArray *map_mef3_segment(SEGMENT *segment, si1 map_indices_flag) {
	mxArray *mat_segment = mxCreateStructMatrix(1, 1, SEGMENT_NUMFIELDS, SEGMENT_FIELDNAMES);
	map_mef3_segment_tostruct(segment, map_indices_flag, mat_segment, 0);
	return mat_segment;
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
 * 	@param channel				A pointer to the MEF channel c-struct
 * 	@param map_indices_flag		Whether to map the time-series and video indices
 * 	@param mat_channel			A pointer to the existing matlab-struct
 * 	@param mat_index			The index in the existing matlab-struct at which to map the data	
 */
void map_mef3_channel_tostruct(CHANNEL *channel, si1 map_indices_flag, mxArray *mat_channel, int mat_index) {
	si4   	i;
	SEGMENT *segment;

	// set channel specific properties
	mxSetField(mat_channel, mat_index, "channel_type", 				mxInt32ByValue(channel->channel_type));
	mxSetField(mat_channel, mat_index, "number_of_segments", 		mxInt64ByValue(channel->number_of_segments));
	mxSetField(mat_channel, mat_index, "path", 						mxCreateString(channel->path));
	mxSetField(mat_channel, mat_index, "name", 						mxCreateString(channel->name));
	mxSetField(mat_channel, mat_index, "extension", 				mxCreateString(channel->extension));
	mxSetField(mat_channel, mat_index, "session_name", 				mxCreateString(channel->session_name));
	mxSetField(mat_channel, mat_index, "level_UUID", 				mxUint8ArrayByValue(channel->level_UUID, UUID_BYTES));
	mxSetField(mat_channel, mat_index, "anonymized_name", 			mxCreateString(channel->anonymized_name));
	mxSetField(mat_channel, mat_index, "maximum_number_of_records", mxInt64ByValue(channel->maximum_number_of_records));
	mxSetField(mat_channel, mat_index, "maximum_record_bytes", 		mxInt64ByValue(channel->maximum_record_bytes));
	mxSetField(mat_channel, mat_index, "earliest_start_time", 		mxInt64ByValue(channel->earliest_start_time));
	mxSetField(mat_channel, mat_index, "latest_end_time", 			mxInt64ByValue(channel->latest_end_time));
	
	
	//
	// records
	//
	
	// check channel records and (if existent) add
    if ((channel->record_indices_fps != NULL) & (channel->record_data_fps != NULL)) {
		mxSetField(mat_channel, mat_index, "records", map_mef3_records(channel->record_indices_fps, channel->record_data_fps));
    }
	
	
	//
	// metadata
	//
	
	// create a metadata struct and assign it to the 'metadata' field
	mxArray *channel_metadata_struct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
	mxSetField(mat_channel, mat_index, "metadata", channel_metadata_struct);
	
	// map channel metadata sections
	mxSetField(channel_metadata_struct, 0, "section_1", map_mef3_md1(channel->metadata.section_1));
	switch (channel->channel_type){
		case TIME_SERIES_CHANNEL_TYPE:
			mxSetField(channel_metadata_struct, 0, "section_2", map_mef3_tmd2(channel->metadata.time_series_section_2));
			break;
		case VIDEO_CHANNEL_TYPE:
			mxSetField(channel_metadata_struct, 0, "section_2", map_mef3_vmd2(channel->metadata.video_section_2));
			break;
		default:
			mexErrMsgTxt("Unrecognized channel type, exiting...");
	}  
	mxSetField(channel_metadata_struct, 0, "section_3", map_mef3_md3(channel->metadata.section_3));
	
	// check if there are segments for this channel
	if (channel->number_of_segments > 0) {
		
		// create a segments struct
		mxArray *segmentsStruct = mxCreateStructMatrix(1, channel->number_of_segments, SEGMENT_NUMFIELDS, SEGMENT_FIELDNAMES);
		
		// map the segments      
		for (i = 0; i < channel->number_of_segments; ++i) {
			segment = channel->segments + i;
			map_mef3_segment_tostruct(segment, map_indices_flag, segmentsStruct, i);
		}
		
		// assign the channels matlab-struct to the 'segments' field
		mxSetField(mat_channel, mat_index, "segments", segmentsStruct);
		
	}
	
}
 
/**
 * 	Map a MEF channel c-struct to a newly created matlab-struct
 * 	
 *	note: this funtion also loops through segments
 *
 * 	@param channel				A pointer to the MEF channel c-struct
 * 	@param map_indices_flag		Whether to map the time-series and video indices
 * 	@return						A pointer to the new matlab-struct
 */
mxArray *map_mef3_channel(CHANNEL *channel, si1 map_indices_flag) {
	mxArray *mat_channel = mxCreateStructMatrix(1, 1, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);
	map_mef3_channel_tostruct(channel, map_indices_flag, mat_channel, 0);
	return mat_channel;
}

/**
 * 	Map a MEF session c-struct to a newly created matlab-struct
 *
 * 	@param session				A pointer to the MEF session c-struct
 * 	@param map_indices_flag		Whether to map the time-series and video indices
 * 	@return						A pointer to the new matlab-struct
 */
mxArray *map_mef3_session(SESSION *session, si1 map_indices_flag) {
    si4   	i;
	CHANNEL *channel;
	
	// create the a matlab 'session' struct
	mxArray *mat_session = mxCreateStructMatrix(1, 1, SESSION_NUMFIELDS, SESSION_FIELDNAMES);
	
	// set session-specific properties
	mxSetField(mat_session, 0, "number_of_time_series_channels", 	mxInt32ByValue(session->number_of_time_series_channels));
	mxSetField(mat_session, 0, "number_of_video_channels",			mxInt32ByValue(session->number_of_video_channels));
	
	mxSetField(mat_session, 0, "name", 								mxCreateString(session->name));
	mxSetField(mat_session, 0, "path", 								mxCreateString(session->path));
	mxSetField(mat_session, 0, "anonymized_name", 					mxCreateString(session->anonymized_name));
	mxSetField(mat_session, 0, "level_UUID", 						mxUint8ArrayByValue(session->level_UUID, UUID_BYTES));
	mxSetField(mat_session, 0, "maximum_number_of_records", 		mxInt64ByValue(session->maximum_number_of_records));
	mxSetField(mat_session, 0, "maximum_record_bytes", 				mxInt64ByValue(session->maximum_record_bytes));
	mxSetField(mat_session, 0, "earliest_start_time", 				mxInt64ByValue(session->earliest_start_time));
	mxSetField(mat_session, 0, "latest_end_time", 					mxInt64ByValue(session->latest_end_time));
	
	
	//
	// records
	// 
	
	// check session records and (if existent) add
    if ((session->record_indices_fps != NULL) & (session->record_data_fps != NULL)) {
		mxSetField(mat_session, 0, "records", map_mef3_records(session->record_indices_fps, session->record_data_fps));
    }
	
	
	//
	// time_series
	// 
	
    // check if there are time series channels
    if (session->number_of_time_series_channels > 0) {
		
		//
		// time_series_metadata
		//
		
		// create a metadata struct and assign it to the 'time_series_metadata' field
		mxArray *time_series_metadata_struct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
		mxSetField(mat_session, 0, "time_series_metadata", time_series_metadata_struct);
		
		// map metadata sections
		mxSetField(time_series_metadata_struct, 0, "section_1", 	map_mef3_md1(session->time_series_metadata.section_1));
		mxSetField(time_series_metadata_struct, 0, "section_2", 	map_mef3_tmd2(session->time_series_metadata.time_series_section_2));
		mxSetField(time_series_metadata_struct, 0, "section_3", 	map_mef3_md3(session->time_series_metadata.section_3));
		
		
		//
		// time_series_channels
		// 
		
		
		// create a channels struct
		mxArray *channels_struct = mxCreateStructMatrix(1, session->number_of_time_series_channels, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);
		
		// map the time-serie channels      
		for (i = 0; i < session->number_of_time_series_channels; ++i) {
			channel = session->time_series_channels + i;
			map_mef3_channel_tostruct(channel, map_indices_flag, channels_struct, i);
		}
		
		// assign the channels matlab-struct to the 'time_series_channels' field
		mxSetField(mat_session, 0, "time_series_channels", channels_struct);

	}
	
	
	//
	// video
	// 
	
    // check if there are video channels
    if (session->number_of_video_channels > 0) {

		//
		// video_metadata
		//
	
		// create a metadata struct and assign it to the 'video_metadata' field
		mxArray *video_metadata_struct = mxCreateStructMatrix(1, 1, METADATA_NUMFIELDS, METADATA_FIELDNAMES);
		mxSetField(mat_session, 0, "video_metadata", video_metadata_struct);
		
		// map metadata sections
		mxSetField(video_metadata_struct, 0, "section_1", 		map_mef3_md1(session->video_metadata.section_1));
		mxSetField(video_metadata_struct, 0, "section_2",		map_mef3_vmd2(session->video_metadata.video_section_2));
		mxSetField(video_metadata_struct, 0, "section_3", 		map_mef3_md3(session->video_metadata.section_3));
		
		
		//
		// video_channels
		// 
		
		// create a channels struct
		mxArray *channels_struct = mxCreateStructMatrix(1, session->number_of_video_channels, CHANNEL_NUMFIELDS, CHANNEL_FIELDNAMES);
		
		// map the video channels      
		for (i = 0; i < session->number_of_video_channels; ++i) {
			channel = session->video_channels + i;
			map_mef3_channel_tostruct(channel, map_indices_flag, channels_struct, i);
		}
		
		// assign the channels matlab-struct to the 'video_channels' field
		mxSetField(mat_session, 0, "video_channels", channels_struct);
		
    }
	
	// return the output-struct
	return mat_session;
	
}

/**
 * 	Map a MEF section 1 metadata c-struct to a newly created matlab-struct
 *
 * 	@param md1			A pointer to the MEF section 1 metadata c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_md1(METADATA_SECTION_1 *md1) {

    mxArray *mat_md = mxCreateStructMatrix(1, 1, METADATA_SECTION_1_NUMFIELDS, METADATA_SECTION_1_FIELDNAMES);
	mxSetField(mat_md, 0, "section_2_encryption", 			mxInt8ByValue(md1->section_2_encryption));
	mxSetField(mat_md, 0, "section_3_encryption", 			mxInt8ByValue(md1->section_3_encryption));
	mxSetField(mat_md, 0, "discretionary_region", 			mxUint8ArrayByValue(md1->discretionary_region, METADATA_SECTION_1_DISCRETIONARY_REGION_BYTES));

	// return the struct
	return mat_md;
}

/**
 * 	Map a MEF section 2 time-series metadata c-struct to a newly created matlab-struct
 *
 * 	@param tmd2			A pointer to the MEF section 2 time-series metadata c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_tmd2(TIME_SERIES_METADATA_SECTION_2 *tmd2) {

    mxArray *mat_md = mxCreateStructMatrix(1, 1, TIME_SERIES_METADATA_SECTION_2_NUMFIELDS, TIME_SERIES_METADATA_SECTION_2_FIELDNAMES);
	
	mxSetField(mat_md, 0, "channel_description", 			mxCreateString(tmd2->channel_description));
	mxSetField(mat_md, 0, "session_description", 			mxCreateString(tmd2->session_description));
	
	mxSetField(mat_md, 0, "recording_duration", 			mxInt64ByValue(tmd2->recording_duration));
	mxSetField(mat_md, 0, "reference_description", 			mxCreateString(tmd2->reference_description));
	mxSetField(mat_md, 0, "acquisition_channel_number", 	mxInt64ByValue(tmd2->acquisition_channel_number));
	mxSetField(mat_md, 0, "sampling_frequency", 			mxDoubleByValue(tmd2->sampling_frequency));
	mxSetField(mat_md, 0, "low_frequency_filter_setting", 	mxDoubleByValue(tmd2->low_frequency_filter_setting));
	mxSetField(mat_md, 0, "high_frequency_filter_setting", 	mxDoubleByValue(tmd2->high_frequency_filter_setting));
	mxSetField(mat_md, 0, "notch_filter_frequency_setting", mxDoubleByValue(tmd2->notch_filter_frequency_setting));
	mxSetField(mat_md, 0, "AC_line_frequency", 				mxDoubleByValue(tmd2->AC_line_frequency));
	mxSetField(mat_md, 0, "units_conversion_factor", 		mxDoubleByValue(tmd2->units_conversion_factor));
	mxSetField(mat_md, 0, "units_description", 				mxCreateString(tmd2->units_description));
	mxSetField(mat_md, 0, "maximum_native_sample_value", 	mxDoubleByValue(tmd2->maximum_native_sample_value));
	mxSetField(mat_md, 0, "minimum_native_sample_value", 	mxDoubleByValue(tmd2->minimum_native_sample_value));
	mxSetField(mat_md, 0, "start_sample", 					mxInt64ByValue(tmd2->start_sample));
	mxSetField(mat_md, 0, "number_of_samples", 				mxInt64ByValue(tmd2->number_of_samples));
	mxSetField(mat_md, 0, "number_of_blocks", 				mxInt64ByValue(tmd2->number_of_blocks));
	mxSetField(mat_md, 0, "maximum_block_bytes", 			mxInt64ByValue(tmd2->maximum_block_bytes));
	mxSetField(mat_md, 0, "maximum_block_samples", 			mxUint32ByValue(tmd2->maximum_block_samples));
	mxSetField(mat_md, 0, "maximum_difference_bytes", 		mxUint32ByValue(tmd2->maximum_difference_bytes));	
	mxSetField(mat_md, 0, "block_interval", 				mxInt64ByValue(tmd2->block_interval));
	mxSetField(mat_md, 0, "number_of_discontinuities", 		mxInt64ByValue(tmd2->number_of_discontinuities));
	mxSetField(mat_md, 0, "maximum_contiguous_blocks", 		mxInt64ByValue(tmd2->maximum_contiguous_blocks));
	mxSetField(mat_md, 0, "maximum_contiguous_block_bytes", mxInt64ByValue(tmd2->maximum_contiguous_block_bytes));
	mxSetField(mat_md, 0, "maximum_contiguous_samples", 	mxInt64ByValue(tmd2->maximum_contiguous_samples));
	mxSetField(mat_md, 0, "discretionary_region", 			mxUint8ArrayByValue(tmd2->discretionary_region, TIME_SERIES_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES));
	
	// return the struct
	return mat_md;
}

/**
 * 	Map a MEF section 2 video metadata c-struct to a newly created matlab-struct
 *
 * 	@param vmd2			A pointer to the MEF section 2 video metadata c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_vmd2(VIDEO_METADATA_SECTION_2 *vmd2) {

    mxArray *mat_md = mxCreateStructMatrix(1, 1, VIDEO_METADATA_SECTION_2_NUMFIELDS, VIDEO_METADATA_SECTION_2_FIELDNAMES);
	
	mxSetField(mat_md, 0, "channel_description", 			mxCreateString(vmd2->channel_description));
	mxSetField(mat_md, 0, "session_description", 			mxCreateString(vmd2->session_description));
	
	mxSetField(mat_md, 0, "recording_duration", 			mxInt64ByValue(vmd2->recording_duration));
	mxSetField(mat_md, 0, "horizontal_resolution", 			mxInt64ByValue(vmd2->horizontal_resolution));
	mxSetField(mat_md, 0, "vertical_resolution", 			mxInt64ByValue(vmd2->vertical_resolution));
	mxSetField(mat_md, 0, "frame_rate", 					mxDoubleByValue(vmd2->frame_rate));
	mxSetField(mat_md, 0, "number_of_clips", 				mxInt64ByValue(vmd2->number_of_clips));
	mxSetField(mat_md, 0, "maximum_clip_bytes", 			mxInt64ByValue(vmd2->maximum_clip_bytes));
	mxSetField(mat_md, 0, "video_format", 					mxCreateString(vmd2->video_format));
	mxSetField(mat_md, 0, "video_file_CRC", 				mxUint32ByValue(vmd2->video_file_CRC));
	mxSetField(mat_md, 0, "discretionary_region", 			mxUint8ArrayByValue(vmd2->discretionary_region, VIDEO_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES));
	
	// return the struct
	return mat_md;
}

/**
 * 	Map a MEF section 3 metadata c-struct to a newly created matlab-struct
 *
 * 	@param md3			A pointer to the MEF section 3 metadata c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_md3(METADATA_SECTION_3 *md3) {

    mxArray *mat_md = mxCreateStructMatrix(1, 1, METADATA_SECTION_3_NUMFIELDS, METADATA_SECTION_3_FIELDNAMES);
	
	mxSetField(mat_md, 0, "recording_time_offset", 			mxInt64ByValue(md3->recording_time_offset));
	mxSetField(mat_md, 0, "DST_start_time", 				mxInt64ByValue(md3->DST_start_time));
	mxSetField(mat_md, 0, "DST_end_time", 					mxInt64ByValue(md3->DST_end_time));
	mxSetField(mat_md, 0, "GMT_offset", 					mxInt32ByValue(md3->GMT_offset));
	mxSetField(mat_md, 0, "subject_name_1", 				mxCreateString(md3->subject_name_1));
	mxSetField(mat_md, 0, "subject_name_2", 				mxCreateString(md3->subject_name_2));
	mxSetField(mat_md, 0, "subject_ID", 					mxCreateString(md3->subject_ID));
	mxSetField(mat_md, 0, "recording_location", 			mxCreateString(md3->recording_location));
	mxSetField(mat_md, 0, "discretionary_region", 			mxUint8ArrayByValue(md3->discretionary_region, METADATA_SECTION_3_DISCRETIONARY_REGION_BYTES));
	
	// return the struct
	return mat_md;
}


mxArray *map_mef3_ti(TIME_SERIES_INDEX *ti, si8 number_of_entries) {

	// create the a matlab 'time_series_index' struct
    mxArray *mat_ti = mxCreateStructMatrix(1, number_of_entries, TIME_SERIES_INDEX_NUMFIELDS, TIME_SERIES_INDEX_FIELDNAMES);
	
	// loop through the time-series indices
	si8 i;
	for (i = 0; i < number_of_entries; ++i) {
		TIME_SERIES_INDEX *cur_ti = ti + i;
		
		mxSetField(mat_ti, i, "file_offset", 					mxInt64ByValue(cur_ti->file_offset));
		mxSetField(mat_ti, i, "start_time", 					mxInt64ByValue(cur_ti->start_time));
		mxSetField(mat_ti, i, "start_sample", 					mxInt64ByValue(cur_ti->start_sample));
		mxSetField(mat_ti, i, "number_of_samples", 				mxUint32ByValue(cur_ti->number_of_samples));
		mxSetField(mat_ti, i, "block_bytes", 					mxUint32ByValue(cur_ti->block_bytes));
		mxSetField(mat_ti, i, "maximum_sample_value", 			mxInt32ByValue(cur_ti->maximum_sample_value));
		mxSetField(mat_ti, i, "minimum_sample_value", 			mxInt32ByValue(cur_ti->minimum_sample_value));
        mxSetField(mat_ti, i, "RED_block_flags", 				mxUint8ByValue(cur_ti->RED_block_flags));
		mxSetField(mat_ti, i, "RED_block_discretionary_region", mxUint8ArrayByValue(cur_ti->RED_block_discretionary_region, RED_BLOCK_DISCRETIONARY_REGION_BYTES));
	}	
	
	// return the struct
	return mat_ti;
}


mxArray *map_mef3_vi(VIDEO_INDEX *vi, si8 number_of_entries) {

	// create the a matlab 'video_index' struct
    mxArray *mat_vi = mxCreateStructMatrix(1, number_of_entries, VIDEO_INDEX_NUMFIELDS, VIDEO_INDEX_FIELDNAMES);
	
	// loop through the video indices
	si8 i;
	for (i = 0; i < number_of_entries; ++i) {
		VIDEO_INDEX *cur_vi = vi + i;
		
		mxSetField(mat_vi, i, "start_time", 				mxInt64ByValue(cur_vi->start_time));
		mxSetField(mat_vi, i, "end_time", 					mxInt64ByValue(cur_vi->end_time));
		mxSetField(mat_vi, i, "start_frame", 				mxUint32ByValue(cur_vi->start_frame));
		mxSetField(mat_vi, i, "end_frame", 					mxUint32ByValue(cur_vi->end_frame));
		mxSetField(mat_vi, i, "file_offset", 				mxInt64ByValue(cur_vi->file_offset));
		mxSetField(mat_vi, i, "clip_bytes", 				mxInt64ByValue(cur_vi->clip_bytes));
		mxSetField(mat_vi, i, "discretionary_region", 		mxUint8ArrayByValue(cur_vi->discretionary_region, VIDEO_INDEX_DISCRETIONARY_REGION_BYTES));
		
	}	
	
	// return the struct
	return mat_vi;
}


/**
 * 	Map the MEF records to a matlab-struct
 *
 * 	@param ri_fps			
 * 	@param rd_fps	
 */
mxArray *map_mef3_records(FILE_PROCESSING_STRUCT *ri_fps, FILE_PROCESSING_STRUCT *rd_fps) {
	si4 i;
	RECORD_HEADER *rh;
	ui4     *type_str_int;
	ui4 	type_code;
	
    // retrieve the number of records
    si8 number_of_records = ri_fps->universal_header->number_of_entries;

	// create custom record struct list
	const int RECORD_NUMFIELDS		= 5;
	const char *RECORD_FIELDNAMES[]	= {	
		"time",
		"type",
		"version_major",
		"version_minor",
		"body"
	};
	mxArray *mat_records = mxCreateStructMatrix(1, number_of_records, RECORD_NUMFIELDS, RECORD_FIELDNAMES);
	
    // point to first record entry
    ui1 *rd = rd_fps->raw_data + UNIVERSAL_HEADER_BYTES;
	
	// loop through the records
	for (i = 0; i < number_of_records; ++i) {
		
		// cast the record header
        rh = (RECORD_HEADER *) rd;


		//
		// header
		//
		mxSetField(mat_records, i, "time", 					mxInt64ByValue(rh->time));
		mxSetField(mat_records, i, "type", 					mxCreateString(rh->type_string));
		mxSetField(mat_records, i, "version_major", 		mxUint8ByValue(rh->version_major));
		mxSetField(mat_records, i, "version_minor", 		mxUint8ByValue(rh->version_minor));
		
		
		//
		// body
		//
		type_str_int = (ui4 *) rh->type_string;
		type_code = *type_str_int;
		switch (type_code) {
			case MEFREC_Note_TYPE_CODE:
				{			
					mxArray *mat_note = map_mef3_note(rh);
					if (mat_note != NULL)
						mxSetField(mat_records, i, "body", 	mat_note);
				}
				
				break;
			case MEFREC_EDFA_TYPE_CODE:
				{
					mxArray *mat_edfa = map_mef3_edfa(rh);
					if (mat_edfa != NULL)
						mxSetField(mat_records, i, "body", 	mat_edfa);
				}
				
				break;
			case MEFREC_LNTP_TYPE_CODE:
				{
					mxArray *mat_lntp = map_mef3_lntp(rh);
					if (mat_lntp != NULL)
						mxSetField(mat_records, i, "body", 	mat_lntp);
				}
				
				break;
			case MEFREC_Seiz_TYPE_CODE:
				{
					mxArray *mat_seiz = map_mef3_seiz(rh);
					if (mat_seiz != NULL)
						mxSetField(mat_records, i, "body", 	mat_seiz);
				}
				
				break;
			case MEFREC_CSti_TYPE_CODE:
				{
					mxArray *mat_csti = map_mef3_csti(rh);
					if (mat_csti != NULL)
						mxSetField(mat_records, i, "body", 	mat_csti);
				}
				
				break;
			case MEFREC_ESti_TYPE_CODE:
				{
					mxArray *mat_esti = map_mef3_esti(rh);
					if (mat_esti != NULL)
						mxSetField(mat_records, i, "body", 	mat_esti);
				}
				
				break;
			case MEFREC_Curs_TYPE_CODE:
				{
					mxArray *mat_curs = map_mef3_curs(rh);
					if (mat_curs != NULL)
						mxSetField(mat_records, i, "body", 	mat_curs);
				}
				
				break;
			case MEFREC_Epoc_TYPE_CODE:
				{
					mxArray *mat_epoc = map_mef3_epoc(rh);
					if (mat_epoc != NULL)
						mxSetField(mat_records, i, "body", 	mat_epoc);
				}
				
				break;
			case MEFREC_SyLg_TYPE_CODE:

				if (rh->version_major == 1 && rh->version_minor == 0) {
					si1 *log_entry = (si1 *) rh + MEFREC_SyLg_1_0_TEXT_OFFSET;
					mxSetField(mat_records, i, "body", 		mxCreateString(log_entry));
					
				} else
					mexPrintf("Warning: unrecognized SyLg version, skipping SyLg body\n");
				
				break;
			case MEFREC_UnRc_TYPE_CODE:
				
				mexPrintf("Warning: \"%s\" (0x%x) is an unrecognized record type, skipping body\n", rh->type_string, type_code);

				break;
			default:
				
				mexPrintf("Warning: \"%s\" (0x%x) is an unrecognized record type, skipping body\n", rh->type_string, type_code);
		}
		
		// forward the record pointer to the next record 
		rd += (RECORD_HEADER_BYTES + rh->bytes);
        
	}
	
	// return the struct
	return mat_records;
}

/**
 * 	Map a MEF record Note c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a Note c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_note(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {

		// return the text
		si1 *body_p = (si1 *) rh + MEFREC_Note_1_0_TEXT_OFFSET;
		return mxCreateString(body_p);
		
	}
	
	// message
	mexPrintf("Warning: unrecognized Note version, skipping Note body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record EDFA c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a EDFA c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_edfa(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_EDFA_1_0 *edfa = (MEFREC_EDFA_1_0 *) ((ui1 *) rh + MEFREC_EDFA_1_0_OFFSET);
		
		mxArray *mat_edfa = mxCreateStructMatrix(1, 1, MEFREC_EDFA_1_0_NUMFIELDS, MEFREC_EDFA_1_0_FIELDNAMES);
		mxSetField(mat_edfa, 0, "duration", 		mxInt64ByValue(edfa->duration));
		
		// return the struct
		return mat_edfa;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized EDFA version, skipping EDFA body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record LNTP c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a LNTP c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_lntp(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_LNTP_1_0 *lntp = (MEFREC_LNTP_1_0 *) ((ui1 *) rh + MEFREC_LNTP_1_0_OFFSET);
		
		mxArray *mat_lntp = mxCreateStructMatrix(1, 1, MEFREC_LNTP_1_0_NUMFIELDS, MEFREC_LNTP_1_0_FIELDNAMES);
		mxSetField(mat_lntp, 0, "length", 			mxInt64ByValue(lntp->length));
		
		// return the struct
		return mat_lntp;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized LNTP version, skipping LNTP body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record Seiz c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a Seiz c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_seiz(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_Seiz_1_0 *seiz = (MEFREC_Seiz_1_0 *) ((ui1 *) rh + MEFREC_Seiz_1_0_OFFSET);
		
		mxArray *mat_seiz = mxCreateStructMatrix(1, 1, MEFREC_SEIZ_1_0_NUMFIELDS, MEFREC_SEIZ_1_0_FIELDNAMES);
		mxSetField(mat_seiz, 0, "earliest_onset", 		mxInt64ByValue(seiz->earliest_onset));
		mxSetField(mat_seiz, 0, "latest_offset", 		mxInt64ByValue(seiz->latest_offset));
		mxSetField(mat_seiz, 0, "duration", 			mxInt64ByValue(seiz->duration));
		mxSetField(mat_seiz, 0, "number_of_channels", 	mxInt32ByValue(seiz->number_of_channels));
		mxSetField(mat_seiz, 0, "onset_code", 			mxInt32ByValue(seiz->onset_code));
		mxSetField(mat_seiz, 0, "marker_name_1", 		mxCreateString(seiz->marker_name_1));
		mxSetField(mat_seiz, 0, "marker_name_2", 		mxCreateString(seiz->marker_name_2));
		mxSetField(mat_seiz, 0, "annotation", 			mxCreateString(seiz->annotation));
		
		// return the struct
		return mat_seiz;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized Seiz version, skipping Seiz body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record CSti c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a CSti c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_csti(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_CSti_1_0 *csti = (MEFREC_CSti_1_0 *) ((ui1 *) rh + MEFREC_CSti_1_0_OFFSET);
		mxArray *mat_csti = mxCreateStructMatrix(1, 1, MEFREC_CSTI_1_0_NUMFIELDS, MEFREC_CSTI_1_0_FIELDNAMES);
		mxSetField(mat_csti, 0, "task_type", 				mxCreateString(csti->task_type));
		mxSetField(mat_csti, 0, "stimulus_duration", 		mxInt64ByValue(csti->stimulus_duration));
		mxSetField(mat_csti, 0, "stimulus_type", 			mxCreateString(csti->stimulus_type));
		mxSetField(mat_csti, 0, "patient_response", 		mxCreateString(csti->patient_response));
		
		// return the struct
		return mat_csti;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized CSti version, skipping CSti body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record ESti c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a ESti c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_esti(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_ESti_1_0 *esti = (MEFREC_ESti_1_0 *) ((ui1 *) rh + MEFREC_ESti_1_0_OFFSET);
		mxArray *mat_esti = mxCreateStructMatrix(1, 1, MEFREC_ESTI_1_0_NUMFIELDS, MEFREC_ESTI_1_0_FIELDNAMES);
		mxSetField(mat_esti, 0, "amplitude", 				mxDoubleByValue(esti->amplitude));
		mxSetField(mat_esti, 0, "frequency", 				mxDoubleByValue(esti->frequency));
		mxSetField(mat_esti, 0, "pulse_width", 				mxInt64ByValue(esti->pulse_width));
		mxSetField(mat_esti, 0, "ampunit_code", 			mxInt32ByValue(esti->ampunit_code));
		mxSetField(mat_esti, 0, "mode_code", 				mxInt32ByValue(esti->mode_code));
		mxSetField(mat_esti, 0, "waveform", 				mxCreateString(esti->waveform));
		mxSetField(mat_esti, 0, "anode", 					mxCreateString(esti->anode));
		mxSetField(mat_esti, 0, "catode", 					mxCreateString(esti->catode));
		
		// return the struct
		return mat_esti;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized ESti version, skipping ESti body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record Curs c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a Curs c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_curs(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_Curs_1_0 *cursor = (MEFREC_Curs_1_0 *) ((ui1 *) rh + MEFREC_Curs_1_0_OFFSET);
		
		mxArray *mat_curs = mxCreateStructMatrix(1, 1, MEFREC_CURS_1_0_NUMFIELDS, MEFREC_CURS_1_0_FIELDNAMES);
		mxSetField(mat_curs, 0, "id_number", 				mxInt64ByValue(cursor->id_number));
		mxSetField(mat_curs, 0, "trace_timestamp", 			mxInt64ByValue(cursor->trace_timestamp));
		mxSetField(mat_curs, 0, "latency", 					mxInt64ByValue(cursor->latency));
		mxSetField(mat_curs, 0, "value", 					mxDoubleByValue(cursor->value));
		mxSetField(mat_curs, 0, "name", 					mxCreateString(cursor->name));
		
		// return the struct
		return mat_curs;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized Curs version, skipping Curs body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF record Epoc c-struct to a newly created matlab-struct
 *
 * 	@param rh			A pointer to the MEF record header of a Epoc c-struct
 * 	@return				A pointer to the new matlab-struct
 */
mxArray *map_mef3_epoc(RECORD_HEADER *rh) {

	if (rh->version_major == 1 && rh->version_minor == 0) {
		
		MEFREC_Epoc_1_0 *epoc = (MEFREC_Epoc_1_0 *) ((ui1 *) rh + MEFREC_Epoc_1_0_OFFSET);
		
		mxArray *mat_epoc = mxCreateStructMatrix(1, 1, MEFREC_EPOC_1_0_NUMFIELDS, MEFREC_EPOC_1_0_FIELDNAMES);
		mxSetField(mat_epoc, 0, "id_number", 				mxInt64ByValue(epoc->id_number));
		mxSetField(mat_epoc, 0, "timestamp", 				mxInt64ByValue(epoc->timestamp));
		mxSetField(mat_epoc, 0, "end_timestamp", 			mxInt64ByValue(epoc->end_timestamp));
		mxSetField(mat_epoc, 0, "duration", 				mxInt64ByValue(epoc->duration));
		mxSetField(mat_epoc, 0, "type", 					mxCreateString(epoc->epoch_type));
		mxSetField(mat_epoc, 0, "text", 					mxCreateString(epoc->text));
		
		// return the struct
		return mat_epoc;
		
	}
	
	// message
	mexPrintf("Warning: unrecognized Epoc version, skipping Epoc body\n");
	
	// return failure
	return NULL;
}

/**
 * 	Map a MEF universal_header c-struct to a newly created matlab-struct
 *  (contribution by Richard J. Cui, 4 apr 2020)
 *
 * 	@param universal_header		Pointer to the MEF universal_header c-struct
 * 	@return						Pointer to the new matlab-struct
 */
mxArray *map_mef3_uh(UNIVERSAL_HEADER *universal_header) {
	
    mxArray *mat_uh = mxCreateStructMatrix(1, 1, UNIVERSAL_HEADER_NUMFIELDS, UNIVERSAL_HEADER_FIELDNAMES);
	
    mxSetField(mat_uh, 0, "header_CRC", 					mxUint32ByValue(universal_header->header_CRC));
    mxSetField(mat_uh, 0, "body_CRC", 						mxUint32ByValue(universal_header->body_CRC));
    mxSetField(mat_uh, 0, "file_type_string", 				mxCreateString(universal_header->file_type_string));
    mxSetField(mat_uh, 0, "mef_version_major", 				mxUint8ByValue(universal_header->mef_version_major));
    mxSetField(mat_uh, 0, "mef_version_minor", 				mxUint8ByValue(universal_header->mef_version_minor));
    mxSetField(mat_uh, 0, "byte_order_code", 				mxUint8ByValue(universal_header->byte_order_code));
    mxSetField(mat_uh, 0, "start_time", 					mxInt64ByValue(universal_header->start_time));
    mxSetField(mat_uh, 0, "end_time", 						mxInt64ByValue(universal_header->end_time));  
    mxSetField(mat_uh, 0, "number_of_entries", 				mxInt64ByValue(universal_header->number_of_entries));  
    mxSetField(mat_uh, 0, "maximum_entry_size", 			mxInt64ByValue(universal_header->maximum_entry_size));  
    mxSetField(mat_uh, 0, "segment_number", 				mxInt32ByValue(universal_header->segment_number));  
    mxSetField(mat_uh, 0, "channel_name", 					mxCreateString(universal_header->channel_name));  
    mxSetField(mat_uh, 0, "session_name", 					mxCreateString(universal_header->session_name));  
    mxSetField(mat_uh, 0, "anonymized_name", 				mxCreateString(universal_header->anonymized_name));  
    mxSetField(mat_uh, 0, "level_UUID", 					mxUint8ArrayByValue(universal_header->level_UUID, UUID_BYTES));
    mxSetField(mat_uh, 0, "file_UUID", 						mxUint8ArrayByValue(universal_header->file_UUID, UUID_BYTES));
    mxSetField(mat_uh, 0, "provenance_UUID", 				mxUint8ArrayByValue(universal_header->provenance_UUID, UUID_BYTES));
    mxSetField(mat_uh, 0, "level_1_password_validation_field", mxUint8ArrayByValue(universal_header->level_1_password_validation_field, PASSWORD_VALIDATION_FIELD_BYTES));
    mxSetField(mat_uh, 0, "level_2_password_validation_field", mxUint8ArrayByValue(universal_header->level_2_password_validation_field, PASSWORD_VALIDATION_FIELD_BYTES));
    mxSetField(mat_uh, 0, "discretionary_region", 			mxUint8ArrayByValue(universal_header->discretionary_region, UNIVERSAL_HEADER_DISCRETIONARY_REGION_BYTES));
    
	// return the struct
	return mat_uh;
	
}



///
// Functions to map matlab-structs to existing c-objects
///


/**
 * 	Map MEF time-series section 2 metadata from a matlab-struct to an existing c-struct
 *
 * 	@param mat_tmd2		A pointer to the source matlab-struct with the MEF time-series section 2 metadata 
 * 	@param tmd2			A pointer to the destination c-struct
 */
bool map_matlab_tmd2(mxArray *mat_tmd2, TIME_SERIES_METADATA_SECTION_2 *tmd2) {
	
	if (!cpyMxFieldStringToUtf8CharString(mat_tmd2, "channel_description", tmd2->channel_description, METADATA_CHANNEL_DESCRIPTION_BYTES))		return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_tmd2, "session_description", tmd2->session_description, METADATA_SESSION_DESCRIPTION_BYTES))		return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "recording_duration", &tmd2->recording_duration))														return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_tmd2, "reference_description", tmd2->reference_description, TIME_SERIES_METADATA_REFERENCE_DESCRIPTION_BYTES))	return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "acquisition_channel_number", &tmd2->acquisition_channel_number))										return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "sampling_frequency", &tmd2->sampling_frequency))														return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "low_frequency_filter_setting", &tmd2->low_frequency_filter_setting))									return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "high_frequency_filter_setting", &tmd2->high_frequency_filter_setting))								return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "notch_filter_frequency_setting", &tmd2->notch_filter_frequency_setting))								return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "AC_line_frequency", &tmd2->AC_line_frequency))														return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "units_conversion_factor", &tmd2->units_conversion_factor))											return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_tmd2, "units_description", tmd2->units_description, TIME_SERIES_METADATA_UNITS_DESCRIPTION_BYTES))	return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "maximum_native_sample_value", &tmd2->maximum_native_sample_value))									return false;
	if (!cpyMxFieldDoubleToVar(mat_tmd2, "minimum_native_sample_value", &tmd2->minimum_native_sample_value))									return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "start_sample", &tmd2->start_sample))																	return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "number_of_samples", &tmd2->number_of_samples))															return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "number_of_blocks", &tmd2->number_of_blocks))															return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "maximum_block_bytes", &tmd2->maximum_block_bytes))														return false;
	if (!cpyMxFieldUint32ToVar(mat_tmd2, "maximum_block_samples", &tmd2->maximum_block_samples))												return false;
	if (!cpyMxFieldUint32ToVar(mat_tmd2, "maximum_difference_bytes", &tmd2->maximum_difference_bytes))											return false;	
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "block_interval", &tmd2->block_interval))																return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "number_of_discontinuities", &tmd2->number_of_discontinuities))											return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "maximum_contiguous_blocks", &tmd2->maximum_contiguous_blocks))											return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "maximum_contiguous_block_bytes", &tmd2->maximum_contiguous_block_bytes))								return false;
	if (!cpyMxFieldInt64ToVar(mat_tmd2, "maximum_contiguous_samples", &tmd2->maximum_contiguous_samples))										return false;
	if (!cpyMxFieldUint8ArrayToVar(mat_tmd2, "discretionary_region", tmd2->discretionary_region, TIME_SERIES_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES))	return false;
	
	// return success
	return true;
}

/**
 * 	Map MEF video section 2 metadata from a matlab-struct to an existing c-struct
 *
 * 	@param mat_vmd2		A pointer to the source matlab-struct with the MEF video section 2 metadata 
 * 	@param vmd2			A pointer to the destination c-struct
 */
bool map_matlab_vmd2(mxArray *mat_vmd2, VIDEO_METADATA_SECTION_2 *vmd2) {
	
	if (!cpyMxFieldStringToUtf8CharString(mat_vmd2, "channel_description", vmd2->channel_description, METADATA_CHANNEL_DESCRIPTION_BYTES))		return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_vmd2, "session_description", vmd2->session_description, METADATA_SESSION_DESCRIPTION_BYTES))		return false;
	if (!cpyMxFieldInt64ToVar(mat_vmd2, "recording_duration", &vmd2->recording_duration))														return false;
	if (!cpyMxFieldInt64ToVar(mat_vmd2, "horizontal_resolution", &vmd2->horizontal_resolution))													return false;
	if (!cpyMxFieldInt64ToVar(mat_vmd2, "vertical_resolution", &vmd2->vertical_resolution))														return false;
	if (!cpyMxFieldDoubleToVar(mat_vmd2, "frame_rate", &vmd2->frame_rate))																		return false;
	if (!cpyMxFieldInt64ToVar(mat_vmd2, "number_of_clips", &vmd2->number_of_clips))																return false;
	if (!cpyMxFieldInt64ToVar(mat_vmd2, "maximum_clip_bytes", &vmd2->maximum_clip_bytes))														return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_vmd2, "video_format", vmd2->video_format, VIDEO_METADATA_VIDEO_FORMAT_BYTES))						return false;
	if (!cpyMxFieldUint32ToVar(mat_vmd2, "video_file_CRC", &vmd2->video_file_CRC))																return false;
	if (!cpyMxFieldUint8ArrayToVar(mat_vmd2, "discretionary_region", vmd2->discretionary_region, VIDEO_METADATA_SECTION_2_DISCRETIONARY_REGION_BYTES))	return false;
	
	// return success
	return true;
}

/**
 * 	Map MEF section 3 metadata from a matlab-struct to an existing c-struct
 *
 * 	@param mat_md3		A pointer to the source matlab-struct with the MEF section 3 metadata 
 * 	@param md3			A pointer to the destination c-struct
 */
bool map_matlab_md3(mxArray *mat_md3, METADATA_SECTION_3 *md3) {
	
	if (!cpyMxFieldInt64ToVar(mat_md3, "recording_time_offset", &md3->recording_time_offset))											return false;
	if (!cpyMxFieldInt64ToVar(mat_md3, "DST_start_time", &md3->DST_start_time))															return false;
	if (!cpyMxFieldInt64ToVar(mat_md3, "DST_end_time", &md3->DST_end_time))																return false;
	if (!cpyMxFieldInt32ToVar(mat_md3, "GMT_offset", &md3->GMT_offset))																	return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_md3, "subject_name_1", md3->subject_name_1, METADATA_SUBJECT_NAME_BYTES))					return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_md3, "subject_name_2", md3->subject_name_2, METADATA_SUBJECT_NAME_BYTES))					return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_md3, "subject_ID", md3->subject_ID, METADATA_SUBJECT_ID_BYTES))							return false;
	if (!cpyMxFieldStringToUtf8CharString(mat_md3, "recording_location", md3->recording_location, METADATA_RECORDING_LOCATION_BYTES))	return false;
	if (!cpyMxFieldUint8ArrayToVar(mat_md3, "discretionary_region", md3->discretionary_region, METADATA_SECTION_3_DISCRETIONARY_REGION_BYTES))	return false;
	
	// return success
	return true;
}
