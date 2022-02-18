%	
%   Write MEF3 meta- and signaldata
%	
%   writeMef3(outputPath, data, sampleFreq)
%   writeMef3(outputPath, data, sampleFreq, channelNames)
%   writeMef3(outputPath, data, sampleFreq, channelNames, password)
%   writeMef3(outputPath, data, sampleFreq, channelNames, password, overwrite, channelAcqNums, unitConvFactor, section2, section3)
%	
%       outputPath     = the output path in which the MEF3 directories and files should be written
%       data           = matrix that contains the signal data to be written. The matrix should be formatted as 
%                        <channels> x <samples>, so the first dimension (rows) represents the channels and the second
%                        dimension (columns) represents the samples.
%       sampleFreq     = the sampling frequency of the data. This argument takes either a single value that will be duplicated
%                        for each of the channels, or a vector/array of values to specify a sampling frequency for each of
%                        the channels. When specifying sampling frequencies for each channel, the values should correspond
%                        to the channels (rows) in the 'data' argument, with the vector/array being equal in size to the 
%                        number of  channels in the 'data' argument.
%       channelNames   = (optional) the names of the channels. This argument takes either a cell-array or string-array to 
%                        manually specify a name for each channel, or can be left empty for automatic naming. When manually
%                        specifying names per channel, the cells should correspond to the channels (rows) in the data argument,
%                        with the cell- or string-array being equal in size to the number of channels in the data argument.
%                        If left empty, channels will be named automatically according to the channel index of the data 
%                        argument (channel/row 1 in the data argument will be 'Ch01', channel/row 2 will be 'Ch02', etc...).
%       password       = (optional) password to encrypt the MEF3 data with. Leave empty ([] or '') for no encryption
%       overwrite      = (optional) whether to overwrite existing directories and files in the session folder. A check will
%                        be performed before anything is written, set to 1 to overwrite, or 0 (default) to abort if any 
%                        file/directory already exists
%       channelAcqNums = (optional) the channel acquisition numbers to be stored in the section 2 metadata (field 
%                        'acquisition_channel_number'). This argument either takes a vector/array of values to manually 
%                        specify a channel number for each of the channels, or can be left empty for automatic numbering.
%                        When manually specifying numbers for each channel, the values should correspond to the channels (rows)
%                        in the data argument, with the vector/array being equal in size to the number of channels in the 
%                        data argument. If left empty, channels will be numbered automatically according to the channel index of
%                        the data argument (i.e. channel/row 1 in the data argument will be 1, channel/row 2 will be 2, etc...).
%       unitConvFactor = (optional) the unit conversion factor(s) to be stored in the section 2 metadata (field
%                        'units_conversion_factor'). The input data is divided by the unit conversion factor(s) before input
%                        checks and writing. This argument takes either a single value that will be duplicated for each of the
%                        channels, or a vector/array of values to specify a conversion factor for each of the channels. When
%                        specifying a conversion factor for each channel, the values should correspond to the channels (rows)
%                        in the 'data' argument, with the vector/array being equal in size to the number of channels in the
%                        'data' argument. If left empty, a value of 1 will be set for all channels.
%       section2       = (optional) additional section 2 metadata that should be stored. This argument takes either a single 
%                        struct where the values in the fields of the struct will be applied to each of the channels, or an 
%                        array of structs to provide metadata for each of the channels seperately. Only the following 
%                        struct-fields are accepted and applied: 'channel_description', 'session_description', 
%                        'reference_description', 'low_frequency_filter_setting', 'high_frequency_filter_setting', 
%                        'notch_filter_frequency_setting', 'AC_line_frequency', 'units_description' and 'discretionary_region'.
%                        The fields 'sampling_frequency', 'units_conversion_factor' and 'acquisition_channel_number' will be
%                        ignored since they are managed by the input arguments 'sampleFreq', 'unitConvFactor' and 'channelNumbers'
%                        respectively. When specifying metadata for each channel, the struct-array should correspond to the
%                        channels (rows) in the 'data' argument, with the struct-array being equal in size to the number
%                        of channels in the 'data' argument. Leave empty to write standard section 2 values.
%       section3       = (optional) additional section 3 metadata that should be stored. This argument takes either a single 
%                        struct where the values in the fields of the struct will be applied to each of the channels, or an 
%                        array of structs to provide metadata for each of the channels seperately. The following 
%                        struct-fields are accepted and applied: 'recording_time_offset', 'DST_start_time', 'DST_end_time',
%                        'GMT_offset', 'subject_name_1', 'subject_name_2', 'subject_ID', 'recording_location'. When specifying
%                        metadata for each channel, the struct-array should correspond to the channels (rows) in the 'data' 
%                        argument, with the struct array being equal in size to the number of channels in the 'data' argument. 
%
%
%   Notes:
%       - If a unit conversion factor is set, the input data is divided by the factor(s) before input checks and writing. 
%       - Some section 2 metadata fields will be set by the writing routines, these are: 'recording_duration', 
%         'maximum_native_sample_value', 'minimum_native_sample_value', 'number_of_samples', 'number_of_blocks', 
%         'maximum_block_bytes', 'maximum_block_samples', 'maximum_difference_bytes', 'maximum_contiguous_blocks',
%         and 'maximum_contiguous_block_bytes'
%
%
%   Examples:
%
%       % write generated data
%       data = round(rand(3, 40000) * 100);
%       writeMef3('./mefSessWriteDir.mefd/', data, 1024);
%
%       % write generated data with channel names
%       data = round(rand(3, 40000) * 100);
%       writeMef3('./mefSessWriteDir.mefd/', data, 1024, {'LCh01', 'LCh02', 'RCh01'});
%
%       % read and re-write (with same conversion factor over all channels)
%       [meta, data] = readMef3('./mefSessDir.mefd/');
%       convFact = meta.time_series_metadata.section_2.units_conversion_factor;
%       writeMef3('./mefSessWriteDir.mefd/', data, 2048, [], [], 1, [], convFact);
%
%
%   Copyright 2022, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
%   

%   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
%   as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
%   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
%   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%   You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
%
function writeMef3(outputPath, data, sampleFreq, channelNames, password, overwrite, channelAcqNums, unitConvFactor, section2, section3)

    % set defaults
    if ~exist('password', 'var')        || isempty(password),       password = [];          end
    if ~exist('channelNames', 'var')    || isempty(channelNames),   channelNames = [];      end
    if ~exist('overwrite', 'var')       || isempty(overwrite),      overwrite = 0;          end
    if ~exist('channelAcqNums', 'var')  || isempty(channelAcqNums), channelAcqNums = [];    end
    if ~exist('unitConvFactor', 'var')  || isempty(unitConvFactor), unitConvFactor = 1;     end
    if ~exist('section2', 'var')        || isempty(section2),       section2 = [];          end
    if ~exist('section3', 'var')        || isempty(section3),       section3 = [];          end
    
    
    % 
    % check the input arguments
    % 

    % check session path
    if ~exist('outputPath', 'var') || isempty(outputPath) || ~ischar(outputPath)
        error('Error: missing or invalid output directory');
    end
    
    % check data
    if ~exist('data', 'var') || isempty(data)
        error('Error: missing ''data'' input argument ');
    elseif ~ismatrix(data) || ~isnumeric(data) || size(data, 1) < 1
        error('Error: invalid ''data'' input argument, should be a NxM matrix containing numeric values with the N dimension representing the channels and M the samples');
    end
    
    % determine the number of channels to write
    numChannels = size(data, 1);

    % check sampling frequency
    if ~exist('sampleFreq', 'var') || isempty(sampleFreq)
        error('Error: missing ''sampleFreq'' input argument ');
    end
    if ischar(sampleFreq) || numel(sampleFreq) == 1
        if ~isnumeric(sampleFreq) || sampleFreq <= 0
            error('Error: invalid ''sampleFreq'' input argument, the sampling frequency should be a numeric value above 0');
        end
    else
        if ~isvector(sampleFreq)
            error('Error: invalid ''sampleFreq'' input argument, multiple sampling frequencies should be passed as a vector (one-dimensional array)');
        elseif ~isnumeric(sampleFreq) || any(sampleFreq <= 0)
            error('Error: invalid ''sampleFreq'' input argument, multiple sampling frequencies should be passed as a numeric vector/array with all values above 0');
        elseif numel(sampleFreq) ~= numChannels
            error(['Error: invalid ''sampleFreq'' input argument, the number of sampling frequencies (', num2str(numel(sampleFreq)), ') in the vector/array should match the number of channels (', num2str(numChannels), ') in the ''data''']);
        end
    end
    
    % check channel names
    if isempty(channelNames)
        
        % generate channel names (zero padded on the left with 'Ch' prefix)
        numChannelDigits = floor(log10(numChannels)) + 1;
        if numChannelDigits < 2,    numChannelDigits = 2;   end
        channelNames = pad(string(1:numChannels), numChannelDigits, 'left', '0');
        channelNames = strcat('Ch', channelNames);
        
    else
        
        if ~iscell(channelNames) && ischar(channelNames)
            if numChannels == 1
                channelNames = {channelNames};
            else
                error(['Error: invalid ''channelNames'' input argument, should be a cell array containing channel names (e.g. {''Ch1'', ''Ch2'', ''Ch3''})']);
            end
        end
        
        if ~isvector(channelNames)
            error('Error: invalid ''channelNames'' input argument, the channel names should be passed as a one-dimensional cell-array or string-array (e.g.{''Ch1'', ''Ch2'', ''Ch3''})');
        elseif numel(channelNames) ~= numChannels
            error(['Error: invalid ''channelNames'' input argument, the number of names (', num2str(numel(channelNames)), ') in the array should match the number of channels (', num2str(numChannels), ') in the ''data''']);
        elseif iscell(channelNames)
            if ~iscellstr(channelNames)
                error('Error: invalid ''channelNames'' input argument, one of the cells is empty or not a character-array');
            end
            channelNames = string(channelNames);
        end
        
        channelNames = strip(channelNames);
        if any(ismember(channelNames, ''))
            error('Error: invalid ''channelNames'' input argument, one of the values in the array is empty');
        end
        
        % check for duplicate names
        [~, uFirstIdx, uIdxs] = unique(lower(channelNames));
        uEntries = channelNames(uFirstIdx);
        nEntries = histc(uIdxs, 1:numel(uEntries));
        if any(nEntries > 1)
            error(['Error: invalid ''channelNames'' input argument, array contains the following duplicate names: ', num2str(strjoin(strcat('''', uEntries(nEntries > 1), ''''), ', '))]);
        end
        
    end
    
    % check password
    if ~isempty(password)
        if isstring(password)
            password = num2str(password);
        elseif ~ischar(password)
            error('Error: invalid ''password'' input argument, should either be empty or a character-array');
        end
    end
    
    % check overwrite argument
    if islogical(overwrite),    overwrite = double(overwrite);  end
    if ischar(overwrite) && (overwrite == '0' || overwrite == '1'),  overwrite = str2double(overwrite);  end
    if numel(overwrite) ~= 1 || ~isnumeric(overwrite) || ~(overwrite == 0 || overwrite == 1)
        error('Error: invalid ''overwrite'' input argument, can be either 0 (no overwrite) or 1 (overwrite)');
    end
    
    % check channel acquisition numbers
    if isempty(channelAcqNums)
        channelAcqNums = 1:numChannels;
    else
        if ~isvector(channelAcqNums)
            error('Error: invalid ''channelAcqNums'' input argument, the channel acquisition numbers should be passed as a vector (one-dimensional array)');
        elseif ~isnumeric(channelAcqNums) || any(channelAcqNums < 1)
            error('Error: invalid ''channelAcqNums'' input argument, the channel acquisition numbers should all positive numeric values (e.g. [1, 2, 3])');
        elseif numel(channelAcqNums) ~= numChannels
            error(['Error: invalid ''channelAcqNums'' input argument, the acquisition number vector (length: ', num2str(numel(channelNames)), ') should match the number of channels (', num2str(numChannels), ') in the ''data''']);
        end
        
        % check for duplicates
        [uEntries, ~, uIdxs] = unique(channelAcqNums);
        nEntries = histc(uIdxs, 1:numel(uEntries));
        if any(nEntries > 1)
            error(['Error: invalid ''channelAcqNums'' input argument, array contains the following duplicate numbers: ', num2str(strjoin(string(uEntries(nEntries > 1)), ', '))]);
        end
        
        % warn if not starting at 1
        if ~any(channelAcqNums == 1)
            warning('on'); warning('backtrace', 'off');
            warning('The ''channelAcqNums'' argument does not contain 1');
        end
        
        % warn if there are gaps in the sequence
        sortedNums = sort(channelAcqNums, 'Asc');
        seqNums = sortedNums(1):sortedNums(end);
        if numel(sortedNums) ~= numel(seqNums) || any(sortedNums ~= sortedNums(1):sortedNums(end))
            warning('on'); warning('backtrace', 'off');
            warning('The ''channelAcqNums'' argument has gaps in the ordered numeric sequence');
        end
    end
    
    % check unit conversion factor
    if ~isempty(unitConvFactor) 
        if ischar(unitConvFactor) || numel(unitConvFactor) == 1
            if ~isnumeric(unitConvFactor) || unitConvFactor == 0
                error('Error: invalid ''unitConvFactor'' input argument, the conversion factor should be a numeric value and not 0');
            end
        else
            if ~isvector(unitConvFactor)
                error('Error: invalid ''unitConvFactor'' input argument, multiple conversion factors should be passed as a vector (one-dimensional array)');
            elseif ~isnumeric(unitConvFactor) || any(unitConvFactor == 0)
                error('Error: invalid ''unitConvFactor'' input argument, multiple converion factors should be passed as a numeric vector/array without any value being 0');
            elseif numel(unitConvFactor) ~= numChannels
                error(['Error: invalid ''unitConvFactor'' input argument, the number of converion factors (', num2str(numel(unitConvFactor)), ') in the vector/array should match the number of channels (', num2str(numChannels), ') in the ''data''']);
            end
        end
    end

    % check the section 2 input
    if ~isempty(section2) 
        if ~isstruct(section2)
            error('Error: invalid ''section2'' input argument, should be a single struct, or an array of structs');
        elseif numel(section2) > 1
            if ~isvector(section2)
                error('Error: invalid ''section2'' input argument, multiple section-2 structs should be passed as an one-dimensional array');
            elseif numel(section2) ~= numChannels
                error(['Error: invalid ''section2'' input argument, the number of section-2 struct in the array (', num2str(numel(section2)), ') in the vector/array should match the number of channels (', num2str(numChannels), ') in the ''data''']);
            end
        end
    end
    
    % check the section 3 input
    if ~isempty(section3) 
        if ~isstruct(section3)
            error('Error: invalid ''section3'' input argument, should be a single struct ,or an array of structs');
        elseif numel(section3) > 1
            if ~isvector(section3)
                error('Error: invalid ''section3'' input argument, multiple section-3 structs should be passed as an one-dimensional array');
            elseif numel(section3) ~= numChannels
                error(['Error: invalid ''section3'' input argument, the number of section-3 struct in the array (', num2str(numel(section3)), ') in the vector/array should match the number of channels (', num2str(numChannels), ') in the ''data''']);
            end
        end
    end
    
    % check if there are remainers in the data
    if all(unitConvFactor == 1)
        % no conversion factor need to be applied
        % (check the data directly on the input argument, no copy and faster)
    
        % check if input is floating point
        if isa(data, 'half') || isa(data, 'single') || isa(data, 'double')
            
            % determine if there are remainders (which are not nan, else rem ~= 0 is also true)
            if any(~isnan(data(:)) & rem(data(:), 1) ~= 0)
                warning('on'); warning('backtrace', 'off');
                warning('The ''data'' matrix has one or more fractional values. MEF3 data is stored as int32, as a result fractional precision will be lost');
            end
            
        end
        
        % TODO: check > +2,147,483,646 || < -2,147,483,646
        
    else
        % conversion factor needs to be applied
        
        % check per channel
        % 
        % Note: deliberately per channel. The input data matrix can be quite big and vectorized
        %       manipulation will create a copy of that data. So performing this per channel will
        %       limit the memory usage
        % Note2: Additionally, the loop can stop executing if any values in the channel are found to be fractional
        for iCh = 1:numChannels
            
            % retrieve the channel's conversion factor
            if numel(unitConvFactor) == 1
                chFactor = unitConvFactor;
            else
                chFactor = unitConvFactor(iCh);
            end

            % check remainders only if the data has fractional numbers or
            % if the data are integers and multiplied by a fractional conversion factor
            if  (isa(data, 'half') || isa(data, 'single') || isa(data, 'double')) || ... 
                (~isa(data, 'half') && ~isa(data, 'single') && ~isa(data, 'double') && rem(chFactor, 1) ~= 0)

                % retrieve and convert the data
                chData = double(data(iCh, :)) / chFactor;

                % determine if there are remainders (which are not nan, else rem ~= 0 is also true)
                if any(~isnan(chData) & rem(chData, 1) ~= 0)
                    warning('on'); warning('backtrace', 'off');
                    warning('After applying the conversion factor(s), the ''data'' matrix has one or more fractional values. MEF3 data is stored as int32, as a result fractional precision will be lost');
                    break;
                end

                % TODO: check > +2,147,483,646 || < -2,147,483,646

                clear chData;
            end
        end
        
    end
    
    % check the format of the output directory, should end with .mefd
    if length(outputPath) < 6 || (~strcmpi(outputPath(end - 4:end), '.mefd') && ...
            (length(outputPath) > 6 && ~strcmpi(outputPath(end - 5:end), '.mefd\') && ~strcmpi(outputPath(end - 5:end), '.mefd/')))
        error('Error: invalid output directory, the MEF3 output path should end with ''.mefd'' (e.g. ''/path/session.mefd'')');
    end
    if strcmp(outputPath(1), '~')
        error('Error: invalid ouput directory, the tilde (''~'') in the output path is only known to the shell, specify the full output path instead');
    end
    
    % check for the existence of the output directory and files
    if ~exist(outputPath, 'dir')
        
        % create the output directory
        [status, msg, ~] = mkdir(outputPath);
        if status == 0
           error('Error: could not create output directory ''%s''. %s', outputPath, msg); 
        end
        
    elseif overwrite == 0
        % output directory already exists, and we cannot overwrite, check files
        
        existingFiles = {};
        for iCh = 1:numChannels
            
            % check channel files
            chPath = fullfile(outputPath, [channelNames{iCh}, '.timd']);
            if exist(chPath, 'dir')                
                chFilePath = fullfile(chPath, [channelNames{iCh}, '-000000.segd'], [channelNames{iCh}, '-000000']);
                if exist([chFilePath, '.tmet'], 'file') == 2,   existingFiles{end + 1} = [chFilePath, '.tmet'];   end
                if exist([chFilePath, '.tdat'], 'file') == 2,   existingFiles{end + 1} = [chFilePath, '.tdat'];   end
                if exist([chFilePath, '.tidx'], 'file') == 2,   existingFiles{end + 1} = [chFilePath, '.tidx'];   end
            end
        
        end
        if ~isempty(existingFiles)            
            error(sprintf(['Error: the following files already exist\n', strrep(num2str(strjoin(strcat({'     '}, existingFiles), newline)), '\', '\\'), '\nSet the overwrite argument to ''1'' to overwrite existing files']));
        end
        
    end
    
    
    % check and warn if there are other channel or segment folders in the folder that
    % are not part of the data that will be written (this could confuse the read function)
    % TODO:
    for iCh = 1:numChannels
        chPath = fullfile(outputPath, [channelNames{iCh}, '.timd']);
        if exist(chPath, 'dir')

            % check if there are other segment folders (.segd) in the channel folder
            %files = dir(topLevelFolder);
            %dirFlags = [files.isdir];
            
        end
    end
    
    %
    % write the meta- and signal-data
    %
    
    % loop over the channels
    for iCh = 1:numChannels
        
        % create section 2 struct (with our own default values, just to be sure)
        wrSection2 = struct;
        wrSection2.session_description             = '';
        wrSection2.reference_description           = '';
        wrSection2.notch_filter_frequency_setting  = double(-1);
        wrSection2.low_frequency_filter_setting    = double(-1);
        wrSection2.high_frequency_filter_setting   = double(-1);
        wrSection2.AC_line_frequency               = double(-1);
        wrSection2.units_description               = '';
        
        % 
        wrSection2.acquisition_channel_number   = int64(channelAcqNums(iCh));
        if numel(sampleFreq) == 1
            wrSection2.sampling_frequency       = double(sampleFreq);
        else
            wrSection2.sampling_frequency = double(sampleFreq(iCh));
        end
        if numel(unitConvFactor) == 1
            wrSection2.units_conversion_factor  = double(unitConvFactor);
        else
            wrSection2.units_conversion_factor  = double(unitConvFactor(iCh));
        end
        
        % set the section 2 metadata start_sample to a value of 0 (we will only be writing one single segment)
        wrSection2.start_sample = int64(0);
        
        % set the samples_per_block and block_interval
        %
        % The samples_per_block, block_interval are related. samples_per_block is used for reading and writing, while
        % the use of block_interval is optional (and currently not used in meflib). The two are related in the sense that:
        %    block interval should be the samples_per_block / sampling_frequency * 1000000
        %
        % For optimal compression, the samples_per_block is ideally >= ~10 000 (article Brinkman et al, 2010). But if we
        % would aim for 10 000 samples per block and had a low sampling rate - for example 256Hz - then that would mean that
        % each block holds 39.0625 seconds of data. This is not only a messy number, but when only 3s of data should be loaded,
        % it would need to load 39.0625 seconds of block data first. Therefore, splitting it up in shorter (5s instead of
        % 39.0625s) blocks at the expense of compression might be beneficial.
        %
        % Here we assume that current technology has a sampling_rate of at least 1024Hz and we assume preferable epoching
        % of ~10s, higher sampling rates will result in more samples_per_block and therefore better compression.
        % So the block interval will be 10s and with 10s of samples per block
        wrSection2.block_interval = int64(10000000);
        samples_per_block = 10 * wrSection2.sampling_frequency;

        %
        % we have no information on these, so set the section 2 metadata number_of_discontinuities 
        % to a default no-entry value of -1
        wrSection2.number_of_discontinuities = int64(-1);
        wrSection2.maximum_contiguous_samples = int64(-1);

        % transfer section 2 fields (if available)
        if ~isempty(section2)
            if numel(section2) == 1
                chSection2 = section2;
            else
                chSection2 = section2(iCh);
            end
            
            % TODO: check if it has field that will be ignored
            % TODO: move checks of fields to before writing
            
            %
            if isfield(chSection2, 'channel_description')
                
                % TODO: create function below for these
                if isempty(chSection2.channel_description) || ~ischar(chSection2.channel_description)
                    warning('on'); warning('backtrace', 'off');
                    warning('The ''field'' channel_description in the section 2 input argument has no or an invalid value, it will not be stored');
                else
                    wrSection2.channel_description = chSection2.channel_description;
                end
                
            end
            
            if isfield(chSection2, 'session_description'), wrSection2.session_description = chSection2.session_description; end
            if isfield(chSection2, 'reference_description'), wrSection2.reference_description = chSection2.reference_description; end
            if isfield(chSection2, 'low_frequency_filter_setting'), wrSection2.low_frequency_filter_setting = chSection2.low_frequency_filter_setting; end
            if isfield(chSection2, 'high_frequency_filter_setting'), wrSection2.high_frequency_filter_setting = chSection2.high_frequency_filter_setting; end
            if isfield(chSection2, 'notch_filter_frequency_setting'), wrSection2.notch_filter_frequency_setting = chSection2.notch_filter_frequency_setting; end
            if isfield(chSection2, 'AC_line_frequency'), wrSection2.AC_line_frequency = chSection2.AC_line_frequency; end
            if isfield(chSection2, 'units_description'), wrSection2.units_description = chSection2.units_description; end
            if isfield(chSection2, 'discretionary_region'), wrSection2.discretionary_region = chSection2.discretionary_region; end
            
        end

        % determine the universal header end-time
        uhEndTime = floor((size(data, 2) / wrSection2.sampling_frequency) * 10^6);
        
        % build the channel and segment paths and make sure the directories exists
        chPath = fullfile(outputPath, [channelNames{iCh}, '.timd']);
        if ~exist(chPath, 'dir')
            [status, msg, ~] = mkdir(chPath);
            if status == 0
               error('Error: could not create channel directory ''%s''. %s', chPath, msg); 
            end
        end
        segPath = fullfile(chPath, [channelNames{iCh}, '-000000.segd']);
        if ~exist(segPath, 'dir')
            [status, msg, ~] = mkdir(segPath);
            if status == 0
               error('Error: could not create segment directory ''%s''. %s', segPath, msg); 
            end
        end
        
        % create section 3 struct
        wrSection3 = struct;
        wrSection3.DST_start_time                     = int64(0);
        wrSection3.DST_end_time                       = int64(0);
        wrSection3.GMT_offset                         = int32(0);
        wrSection3.subject_name_1                     = '';
        wrSection3.subject_name_2                     = '';
        wrSection3.subject_ID                         = '';
        wrSection3.recording_location                 = '';
        
        % set the recording time offset to zero, this is important because elsewise it
        % could get shifted by the no_entry value (possible bug in meflib)
        wrSection3.recording_time_offset              = int64(0);
        

        % transfer section 3 fields (if available)
        if ~isempty(section3)
            if numel(section3) == 1
                chSection3 = section3;
            else
                chSection3 = section3(iCh);
            end
            
            % TODO: check if it has field that will be ignored
            % TODO: move checks of fields to before writing
            
            if isfield(chSection3, 'recording_time_offset'), wrSection3.recording_time_offset = chSection3.recording_time_offset; end
            if isfield(chSection3, 'DST_start_time'), wrSection3.DST_start_time = chSection3.DST_start_time; end
            if isfield(chSection3, 'DST_end_time'), wrSection3.DST_end_time = chSection3.DST_end_time; end
            if isfield(chSection3, 'GMT_offset'), wrSection3.GMT_offset = chSection3.GMT_offset; end
            if isfield(chSection3, 'subject_name_1'), wrSection3.subject_name_1 = chSection3.subject_name_1; end
            if isfield(chSection3, 'subject_name_2'), wrSection3.subject_name_2 = chSection3.subject_name_2; end
            if isfield(chSection3, 'subject_ID'), wrSection3.subject_ID = chSection3.subject_ID; end
            if isfield(chSection3, 'recording_location'), wrSection3.recording_location = chSection3.recording_location; end
            
        end
        
        
        %
        % write meta- and signal-data
        %

        % remove existing files channel files
        if overwrite == 1
            chFilePath = fullfile(chPath, [channelNames{iCh}, '-000000.segd'], [channelNames{iCh}, '-000000']);
            if exist([chFilePath, '.tmet'], 'file') == 2,   delete([chFilePath, '.tmet']);   end
            if exist([chFilePath, '.tdat'], 'file') == 2,   delete([chFilePath, '.tdat']);   end
            if exist([chFilePath, '.tidx'], 'file') == 2,   delete([chFilePath, '.tidx']);   end
        end

        
        % write metadata
        write_mef_segment_metadata('ts', chPath, 0, ...
                                    password, password, ...
                                    int64(0), int64(uhEndTime), '', wrSection2, wrSection3);
        
        % retrieve the channel data and apply conversion factor
        chData = data(iCh, :);
        if wrSection2.units_conversion_factor ~= 1
            chData = double(chData) / wrSection2.units_conversion_factor;
        end
        
        % determine the nan/inf values
        chDataNans = isnan(chData);
        chDataNegInf = isinf(chData) & chData < 0;
        chDataPosInf = isinf(chData) & chData > 0;
        
        % convert to int32
        % Note: Normally, casting would just drop the fractional part, but matlab will round the numbers instead
        %       (in this context, rounding will result in a more precise output)
        chData = int32(chData);
        
        % set the nan/inf values to the pre-defined int32 nan/inf values
        chData(chDataNans) = 0x80000000;
        chData(chDataNegInf) = 0x80000001;
        chData(chDataPosInf) = 0x7FFFFFFF;
        
        % write the signal data
        write_mef_ts_segment_data(chPath, 0, [], [], samples_per_block, chData'); 
        
    end
    
end
