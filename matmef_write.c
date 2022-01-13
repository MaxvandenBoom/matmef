/**
 * 	@file
 * 	MEF 3.0 Library Matlab Wrapper
 * 	Functions to write data to MEF3 files
 *	
 *  Copyright 2022, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
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
 *	@param anonymized_name      The anonymized subject name to be stored in the universal-header of the file
 *	@param channel_type         The type of channel [either TIME_SERIES_CHANNEL_TYPE or VIDEO_CHANNEL_TYPE]
 *	@param mat_md2              Pointer to a matlab-struct (mxArray) with either time-series or video section 2 metadata
 *	@param mat_md3              Pointer to a matlab-struct (mxArray) with the section 3 metadata
 * 	@return                     True if succesfully written, or False on failure
 */
bool write_metadata(si1 *segment_path, si1 *password_l1, si1 *password_l2, si8 start_time, si8 end_time, si1 *anonymized_name, si4 channel_type, mxArray *mat_md2, mxArray *mat_md3) {
	
    FILE_PROCESSING_STRUCT *gen_fps, *metadata_fps;
    UNIVERSAL_HEADER        *uh;
	
	// if the password is just the null character, then correct to a null pointer
	if (password_l1 != NULL && password_l1[0] == '\0')	password_l1 = NULL;
	if (password_l2 != NULL && password_l2[0] == '\0')	password_l2 = NULL;
	
	// initialize MEF library
	(void) initialize_meflib();
	MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
	
    // set up a generic mef3 fps (is used later to base the time-series metadata fps on)
    gen_fps = allocate_file_processing_struct(UNIVERSAL_HEADER_BYTES, NO_FILE_TYPE_CODE, NULL, NULL, 0);
    initialize_universal_header(gen_fps, MEF_TRUE, MEF_FALSE, MEF_TRUE);
    uh = gen_fps->universal_header;
	
	// transfer the start-time, end-time and anonymized name from the arguments to the universal header
    uh->start_time = start_time;
    uh->end_time = end_time;
	MEF_strncpy(uh->anonymized_name, anonymized_name, UNIVERSAL_HEADER_ANONYMIZED_NAME_BYTES);

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

/**
 * 	Write time-series data (.tdat & .tidx files) to a segment directory. 
 * 
 *  Note:  This function requires that a time-series metadata file (.tmet) is already written for the 
 *         specified segment. The universal-header data of the metadata file (.tmet) will be the base for
 *         universal-headers of the data files (.tdat & tidx). In addition, universal header fields in the
 *         metadata file (.tmet) will be updated according to the data that is passed to this function
 * 	
 * 	@param segment_path         The path to the segment directory
 * 	@param password_l1          Level 1 password for the data (no password = NULL)
 * 	@param password_l2          Level 2 password for the data (no password = NULL)
 *	@param samples_per_block    Number of samples per MEF3 block
 *	@param data             	The data to write as a 1-D array of data-type int32
 * 	@return                     True if succesfully written, or False on failure
 */
bool write_mef_ts_data_and_indices(si1 *segment_path, si1 *password_l1, si1 *password_l2, ui4 samples_per_block, const mxArray *data, bool lossy_flag) {
    
    PASSWORD_DATA           *pwd;
    UNIVERSAL_HEADER    	*ts_data_uh;
    FILE_PROCESSING_STRUCT  *gen_fps, *metadata_fps;
	TIME_SERIES_INDEX   	*tsi;
    RED_PROCESSING_STRUCT   *rps;
	RED_BLOCK_HEADER    	*block_header;
	
    si1     path_in[MEF_FULL_FILE_NAME_BYTES], path_out[MEF_FULL_FILE_NAME_BYTES], name[MEF_BASE_FILE_NAME_BYTES], type[TYPE_BYTES];
    si1     full_file_name[MEF_FULL_FILE_NAME_BYTES], file_path[MEF_FULL_FILE_NAME_BYTES], segment_name[MEF_BASE_FILE_NAME_BYTES];
	si4     max_samp, min_samp;
    ui4     block_samps;
    si8     start_sample, samps_remaining, file_offset;
	si8     curr_time, time_inc;

	//
	// 
	//

	// if the password is just the null character, then correct to a null pointer
	if (password_l1 != NULL && password_l1[0] == '\0')	password_l1 = NULL;
	if (password_l2 != NULL && password_l2[0] == '\0')	password_l2 = NULL;
	
	//// check the data type
	if (mxGetClassID(data) != mxINT32_CLASS) {
		mexPrintf("Error: Incorrect data-type, should be int32, exiting...\n");
		return false;
	}
	
	// create a pointer to the data
	si4 *pData = (si4 *)mxGetData(data);
	
	// initialize MEF library
	(void) initialize_meflib();
	MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
	
    // set up a generic mef3 fps and process the password data with it
    gen_fps = allocate_file_processing_struct(UNIVERSAL_HEADER_BYTES, NO_FILE_TYPE_CODE, NULL, NULL, 0);
	initialize_universal_header(gen_fps, MEF_TRUE, MEF_FALSE, MEF_TRUE);
    MEF_globals->behavior_on_fail = SUPPRESS_ERROR_OUTPUT;
    pwd = process_password_data(NULL, password_l1, password_l2, gen_fps->universal_header);
    MEF_globals->behavior_on_fail = EXIT_ON_FAIL;
	
	// extract the segment name and check the directory-type (if indeed segment)
	extract_path_parts(segment_path, path_out, name, type);
    MEF_strncpy(file_path, segment_path, MEF_FULL_FILE_NAME_BYTES);
    if (!strcmp(type, SEGMENT_DIRECTORY_TYPE_STRING)) {
		// segment type/directory

        // copy the segment name for file name construction later
        MEF_strncpy(segment_name, name, MEF_BASE_FILE_NAME_BYTES);
		
		// extract the channel name and check the type (if indeed time-series)
        MEF_strncpy(path_in, path_out, MEF_FULL_FILE_NAME_BYTES);
        extract_path_parts(path_in, path_out, name, type);
        if (!strcmp(type, TIME_SERIES_CHANNEL_DIRECTORY_TYPE_STRING)) {
			// correct/corresponding directory-type
			
            // extract the session name
            MEF_strncpy(path_in, path_out, MEF_FULL_FILE_NAME_BYTES);
            extract_path_parts(path_in, path_out, name, type);
			
        } else {
			// incorrect directory-type
			
			mexPrintf("Error: Not a time-series channel, exiting...\n");
            return false;
        }
		
    } else {
		// not segment type/directory
		
		mexPrintf("Error: Not a segment, exiting...\n");
        return false;
    }
	
	
	
	// 
    // Read the existing time-series metadata file
	//
	// Note: the start_time in the universal header struct does not match the binary start_time because
	//	     the meflib read_MEF_file function does operations that offset the universal-header
	// 
    MEF_snprintf(full_file_name, MEF_FULL_FILE_NAME_BYTES, "%s/%s.%s", file_path, segment_name, TIME_SERIES_METADATA_FILE_TYPE_STRING);
    metadata_fps = read_MEF_file(NULL, full_file_name, password_l1, pwd, NULL, USE_GLOBAL_BEHAVIOR);

	// 
    MEF_globals->recording_time_offset = metadata_fps->metadata.section_3->recording_time_offset;
	
	
	
	//
	// Point to and update the time-series section 2 of the metadata struct (from the .tmet file)
	// 
	// The fields in this section 2 struct will be updated her and later to reflect the
	// data (that we will be writing), in the end the updated metadata will be written (to the .tmet file)
	// 
	//
	
	// create a pointer to the existing time-series section 2 metadata (from the .tmet file)
    TIME_SERIES_METADATA_SECTION_2 *tmd2 = metadata_fps->metadata.time_series_section_2;
	
	// update fields in the time-series section 2 metadata based on the data (to be written)
	const mwSize *dims = mxGetDimensions(data);
	tmd2->number_of_samples = (si8) dims[0];
    tmd2->recording_duration = (si8) (((sf8)tmd2->number_of_samples / (sf8) tmd2->sampling_frequency) * 1e6);
    tmd2->number_of_blocks = (si8) ceil((sf8) tmd2->number_of_samples / (sf8)samples_per_block);
    tmd2->maximum_block_samples = samples_per_block;
	
	

	// 
    // Set up a file-processing-struct and universal-header for the time-series indices (file)
	//
	
	// allocate a fps and univeral header for the ts-indices (file), based on the ts-metadata (copying the directives, password data, and raw data)
    si8 ts_indices_file_bytes 			= (tmd2->number_of_blocks * TIME_SERIES_INDEX_BYTES) + UNIVERSAL_HEADER_BYTES;
    FILE_PROCESSING_STRUCT *ts_idx_fps 	= allocate_file_processing_struct(ts_indices_file_bytes, TIME_SERIES_INDICES_FILE_TYPE_CODE, NULL, metadata_fps, UNIVERSAL_HEADER_BYTES);
    MEF_snprintf(ts_idx_fps->full_file_name, MEF_FULL_FILE_NAME_BYTES, "%s/%s.%s", file_path, segment_name, TIME_SERIES_INDICES_FILE_TYPE_STRING);
	
	// generate a uuid and set some of the index entries fields
    generate_UUID(ts_idx_fps->universal_header->file_UUID);
    ts_idx_fps->universal_header->number_of_entries = tmd2->number_of_blocks;
    ts_idx_fps->universal_header->maximum_entry_size = TIME_SERIES_INDEX_BYTES;



	// 
	// Set up a file-processing-struct and universal-header for the time-series data and write to a file
	//
	
	// allocate a fps and univeral header for the ts-data, based on the ts-metadata (copying the directives, password data, and raw data, including start_)
	FILE_PROCESSING_STRUCT *ts_data_fps = allocate_file_processing_struct(UNIVERSAL_HEADER_BYTES + RED_MAX_COMPRESSED_BYTES(samples_per_block, 1), TIME_SERIES_DATA_FILE_TYPE_CODE, NULL, metadata_fps, UNIVERSAL_HEADER_BYTES);
    MEF_snprintf(ts_data_fps->full_file_name, MEF_FULL_FILE_NAME_BYTES, "%s/%s.%s", file_path, segment_name, TIME_SERIES_DATA_FILE_TYPE_STRING);
    
	// pointer to the universal-header of the time-series data (file)
	ts_data_uh = ts_data_fps->universal_header;
	
	// generate/update the ts-data file uuid and set some of the index entries fields
    generate_UUID(ts_data_uh->file_UUID);
    ts_data_uh->number_of_entries = tmd2->number_of_blocks;
    ts_data_uh->maximum_entry_size = samples_per_block;
	
	// write the universal header of the ts-data file
    ts_data_fps->directives.io_bytes = UNIVERSAL_HEADER_BYTES;
    ts_data_fps->directives.close_file = MEF_FALSE;
    write_MEF_file(ts_data_fps);


	//
	//
	//
	
    // TODO optional filtration
    // use allocation below if lossy
    if (lossy_flag == 1) {
        
		rps = RED_allocate_processing_struct(samples_per_block, 0, samples_per_block, RED_MAX_DIFFERENCE_BYTES(samples_per_block), samples_per_block, samples_per_block, pwd);
        // ASK RED lossy compression user specified???
        rps->compression.mode = RED_MEAN_RESIDUAL_RATIO;
        rps->directives.detrend_data = MEF_TRUE;
        rps->directives.require_normality = MEF_TRUE;
        rps->compression.goal_mean_residual_ratio = 0.10;
        rps->compression.goal_tolerance = 0.01;
		
    } else {
        
		rps = RED_allocate_processing_struct(samples_per_block, 0, 0, RED_MAX_DIFFERENCE_BYTES(samples_per_block), 0, 0, pwd);
		
    }
    rps->block_header = (RED_BLOCK_HEADER *) (rps->compressed_data = ts_data_fps->RED_blocks);

    // create new RED blocks
    curr_time = metadata_fps->universal_header->start_time;
    time_inc = (si8) (((sf8) samples_per_block / tmd2->sampling_frequency) * (sf8) 1e6);
    samps_remaining = tmd2->number_of_samples;
    block_header = rps->block_header;
    tsi = ts_idx_fps->time_series_indices;
    min_samp = RED_POSITIVE_INFINITY;
    max_samp = RED_NEGATIVE_INFINITY;
    block_samps = samples_per_block;
    file_offset = UNIVERSAL_HEADER_BYTES;

    start_sample = 0;

    // Write the data and update the metadata
    while (samps_remaining) {

        // check
        if (samps_remaining < block_samps)
            block_samps = (ui4) samps_remaining;
        block_header->number_of_samples = block_samps;
        block_header->start_time = (si8) (curr_time + 0.5); // ASK Why 0.5 here?
        curr_time += time_inc;

        rps->original_data = rps->original_ptr = (si4 *)pData + (tmd2->number_of_samples - samps_remaining);

        // filter - comment out if don't want
        // filtps->data_length = block_samps;
        // RED_filter(filtps);

        samps_remaining -= (si8) block_samps;

        // compress
        (void) RED_encode(rps);
        ts_data_fps->universal_header->body_CRC = CRC_update((ui1 *) block_header, block_header->block_bytes, ts_data_fps->universal_header->body_CRC);
        e_fwrite((void *) block_header, sizeof(ui1), block_header->block_bytes, ts_data_fps->fp, ts_data_fps->full_file_name, __FUNCTION__, __LINE__, EXIT_ON_FAIL);

        // time series indices
        tsi->file_offset = file_offset;
        file_offset += (tsi->block_bytes = block_header->block_bytes);
        tsi->start_time = block_header->start_time;
        tsi->start_sample = start_sample;
        start_sample += (tsi->number_of_samples = (si8) block_samps);
        RED_find_extrema(rps->original_ptr, block_samps, tsi);
        if (max_samp < tsi->maximum_sample_value)
            max_samp = tsi->maximum_sample_value;
        if (min_samp > tsi->minimum_sample_value)
            min_samp = tsi->minimum_sample_value;
        tsi->RED_block_flags = block_header->flags;
        ++tsi;

        // update metadata
        if (tmd2->maximum_block_bytes < block_header->block_bytes)
            tmd2->maximum_block_bytes = block_header->block_bytes;
        if (tmd2->maximum_difference_bytes < block_header->difference_bytes)
            tmd2->maximum_difference_bytes = block_header->difference_bytes;
    }

    // update metadata
    tmd2->maximum_contiguous_block_bytes = file_offset - UNIVERSAL_HEADER_BYTES;
    if (tmd2->units_conversion_factor >= 0.0) {
        tmd2->maximum_native_sample_value = (sf8) max_samp * tmd2->units_conversion_factor;
        tmd2->minimum_native_sample_value = (sf8) min_samp * tmd2->units_conversion_factor;
    } else {
        tmd2->maximum_native_sample_value = (sf8) min_samp * tmd2->units_conversion_factor;
        tmd2->minimum_native_sample_value = (sf8) max_samp * tmd2->units_conversion_factor;
    }
    tmd2->maximum_contiguous_blocks = tmd2->number_of_blocks;

    // calculate the CRC for the time-series data-file and set in the universal header
    ts_data_fps->universal_header->header_CRC = CRC_calculate(ts_data_fps->raw_data + CRC_BYTES, UNIVERSAL_HEADER_BYTES - CRC_BYTES);
	
	// re-write the universal header of the ts-data file (which now includes the CRC) and manually close (since directives.close_file was set to off for this file)
    e_fseek(ts_data_fps->fp, 0, SEEK_SET, ts_data_fps->full_file_name, __FUNCTION__, __LINE__, MEF_globals->behavior_on_fail);
    e_fwrite(ts_data_uh, sizeof(ui1), UNIVERSAL_HEADER_BYTES, ts_data_fps->fp, ts_data_fps->full_file_name, __FUNCTION__, __LINE__, MEF_globals->behavior_on_fail);
    fclose(ts_data_fps->fp);
	
    // write/update the time-series metadata file
    write_MEF_file(metadata_fps);
	
	// write time-series indices (file)
    write_MEF_file(ts_idx_fps);

    // clean up
    free_file_processing_struct(metadata_fps);
    free_file_processing_struct(ts_data_fps);
    free_file_processing_struct(ts_idx_fps);
    free_file_processing_struct(gen_fps);
    rps->block_header = NULL;
    rps->compressed_data = NULL;
    rps->original_data = NULL;
    rps->original_ptr = NULL;
    RED_free_processing_struct(rps);

	// return succes
	return true;
	
}