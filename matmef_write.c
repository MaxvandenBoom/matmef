/**
 * 	@file
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Functions to write data to MEF3 files
 *	
 *  Copyright 2021, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
 *	Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)
 *  
 *	
 *  This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
 *  You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "matmef_write.h"
#include "mex.h"

#include "meflib/meflib/meflib.c"
#include "meflib/meflib/mefrec.c"
#include "matmef_utils.h"
#include "matmef_mapping.h"


/**
 * 	Write time-series or video metadata to a segment directory
 * 	
 * 	@param segment_path         The path to the segment directory
 * 	@param password_l1          Level 1 password for the metadata (no password = NULL)
 * 	@param password_l2          Level 2 password for the metadata (no password = NULL)
 *	@param start_time           The start epoch time in microseconds (μUTC format) to be stored in the universal-header of the file
 *	@param end_time             The end epoch time in microseconds (μUTC format) to be stored in the universal-header of the file
*	@param channel_type         The type of channel [either TIME_SERIES_CHANNEL_TYPE or VIDEO_CHANNEL_TYPE]
 *	@param mat_md2              Pointer to a matlab-struct (mxArray) with either time-series or video section 2 metadata
 *	@param mat_md3              Pointer to a matlab-struct (mxArray) with the section 3 metadata
 * 	@return                     True if succesfully written, or False on failure
 */
bool write_metadata(si1 *segment_path, si1 *password_l1, si1 *password_l2, si8 start_time, si8 end_time, si4 channel_type, mxArray *mat_md2, mxArray *mat_md3) {
	
    FILE_PROCESSING_STRUCT *gen_fps, *metadata_fps;
    UNIVERSAL_HEADER        *uh;
	
	// if the password is just the null character, then correct to a null pointer
	if (password_l1 != NULL && password_l1[0] == '\0')	password_l1 = NULL;
	if (password_l2 != NULL && password_l2[0] == '\0')	password_l2 = NULL;
	
	// initialize MEF library
	(void) initialize_meflib();
	MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
	
    // set up a generic mef3 fps for universal header with the start- and end-time
    gen_fps = allocate_file_processing_struct(UNIVERSAL_HEADER_BYTES, NO_FILE_TYPE_CODE, NULL, NULL, 0);
    initialize_universal_header(gen_fps, MEF_TRUE, MEF_FALSE, MEF_TRUE);
    uh = gen_fps->universal_header;
    uh->start_time = start_time;
    uh->end_time = end_time;

	// set the password data
    MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
    gen_fps->password_data = process_password_data(NULL, password_l1, password_l2, uh);
    MEF_globals->behavior_on_fail = EXIT_ON_FAIL;

	//  
    si1     path_in[MEF_FULL_FILE_NAME_BYTES], path_out[MEF_FULL_FILE_NAME_BYTES], name[MEF_BASE_FILE_NAME_BYTES], type[TYPE_BYTES];
    si1     file_path[MEF_FULL_FILE_NAME_BYTES], segment_name[MEF_BASE_FILE_NAME_BYTES];
	
	// extract the segment name and check the type (if indeed segment)
	extract_path_parts(segment_path, path_out, name, type);
    MEF_strncpy(file_path, segment_path, MEF_FULL_FILE_NAME_BYTES);
    if (!strcmp(type, SEGMENT_DIRECTORY_TYPE_STRING)) {
		// segment type/directory
		
		// extract segment number from the segment name
        uh->segment_number = extract_segment_number(&name[0]);

        // copy the segment name for file name construction
        MEF_strncpy(segment_name, name, MEF_BASE_FILE_NAME_BYTES);
		
		// extract the channel name and check the type (if indeed time-series)
        MEF_strncpy(path_in, path_out, MEF_FULL_FILE_NAME_BYTES);
        extract_path_parts(path_in, path_out, name, type);
		bool valid_type = 	(channel_type == TIME_SERIES_CHANNEL_TYPE && !strcmp(type, TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING)) ||
							(channel_type == VIDEO_CHANNEL_TYPE && !strcmp(type, VIDEO_CHANNEL_DIRECTORY_TYPE_STRING));
        if (valid_type) {
			// Correct/corresponding directory-type
			
			// set the channel name in the universal header
            MEF_strncpy(uh->channel_name, name, MEF_BASE_FILE_NAME_BYTES);
			
            // extract the session name
            MEF_strncpy(path_in, path_out, MEF_FULL_FILE_NAME_BYTES);
            extract_path_parts(path_in, path_out, name, type);
			
			// set the session name in the universal header
            MEF_strncpy(uh->session_name, name, MEF_BASE_FILE_NAME_BYTES);
			
        } else {
			// incorrect directory-type
			
			if (channel_type == TIME_SERIES_CHANNEL_TYPE)	mexPrintf("Error: Not a time-series channel, exiting...\n");
			if (channel_type == VIDEO_CHANNEL_TYPE)			mexPrintf("Error: Not a video channel, exiting...\n");
            return false;
			
        }
		
    } else {
		// not segment type/directory
		
		mexPrintf("Error: Not a segment, exiting...\n");
        return false;
		
    }
	
    // generate level UUID into generic universal_header
    generate_UUID(gen_fps->universal_header->level_UUID);
	
    // set up mef3 metadata file
	if (channel_type == TIME_SERIES_CHANNEL_TYPE) {
		metadata_fps = allocate_file_processing_struct(METADATA_FILE_BYTES, TIME_SERIES_METADATA_FILE_TYPE_CODE, NULL, gen_fps, UNIVERSAL_HEADER_BYTES);
		MEF_snprintf(metadata_fps->full_file_name, MEF_FULL_FILE_NAME_BYTES, "%s/%s.%s", file_path, segment_name, TIME_SERIES_METADATA_FILE_TYPE_STRING);
	} else {
		metadata_fps = allocate_file_processing_struct(METADATA_FILE_BYTES, VIDEO_METADATA_FILE_TYPE_CODE, NULL, gen_fps, UNIVERSAL_HEADER_BYTES);
		MEF_snprintf(metadata_fps->full_file_name, MEF_FULL_FILE_NAME_BYTES, "%s/%s.%s", file_path, segment_name, VIDEO_METADATA_FILE_TYPE_STRING);		
	}
    uh = metadata_fps->universal_header;
    generate_UUID(uh->file_UUID);
    uh->number_of_entries = 1;
    uh->maximum_entry_size = METADATA_FILE_BYTES;
    initialize_metadata(metadata_fps);
    metadata_fps->metadata.section_1->section_2_encryption = LEVEL_1_ENCRYPTION_DECRYPTED;
    metadata_fps->metadata.section_1->section_3_encryption = LEVEL_2_ENCRYPTION_DECRYPTED;
	
	// transfer the section 2 metadata from the matlab struct to the metadata file
	if (channel_type == TIME_SERIES_CHANNEL_TYPE) {
		// time-series type
		
		if (!map_matlab_tmd2(mat_md2, metadata_fps->metadata.time_series_section_2)) {
			mexPrintf("Error: could not map the time-series section 2 metadata from the matlab struct, exiting...\n");
			free_file_processing_struct(metadata_fps);
			free_file_processing_struct(gen_fps);
			return false;
		}
		
	} else {
		// video type

		if (!map_matlab_vmd2(mat_md2, metadata_fps->metadata.video_section_2)) {
			mexPrintf("Error: could not map the video section 2 metadata from the matlab struct, exiting...\n");
			free_file_processing_struct(metadata_fps);
			free_file_processing_struct(gen_fps);
			return false;
		}
		
	}
	
    // transfer the section 3 metadata from the matlab struct to the metadata file
    if (!map_matlab_md3(mat_md3, metadata_fps->metadata.section_3)) {
		// upon error
		
		mexPrintf("Error: could not map the section 3 metadata from the matlab struct, exiting...\n");
		free_file_processing_struct(metadata_fps);
		free_file_processing_struct(gen_fps);
		return false;
		
	}
	
    // Assign recording_time_offset
    MEF_globals->recording_time_offset = metadata_fps->metadata.section_3->recording_time_offset;

	// write the metadata
    write_MEF_file(metadata_fps);
	
    // clean up
    free_file_processing_struct(metadata_fps);
    free_file_processing_struct(gen_fps);

	// return succes
	return true;
	
}
