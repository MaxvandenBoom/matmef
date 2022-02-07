%	
%   Read MEF3 meta- and signaldata from a session directory
%	
%   [metadata, data] = readMef3(sessPath)
%   [metadata, data] = readMef3(sessPath, password, channels, rangeType, ranges)
%   [metadata, data] = readMef3(sessPath, password, channels, rangeType, rangeStart, rangeEnd)
%	
%       sessPath    = path (absolute or relative) to the MEF3 session folder
%       password    = (optional) password to the MEF3 data; Leave empty ([] or '') if not encrypted
%       channels    = (optional) a cell array with the names of the channels to return the signal data from. The order of channels
%                     in this input argument will determine the order of channels in the output matrix. If left empty, all
%                     channels will be read and ordered as in the metadata.time_series_channels (ordered according to
%                     the 'acquisition_channel_number' metadata variable of each channel)
%       rangeType   = (optional) modality that is used to define the data-range to read [either 'time' or 'samples' (default)].
%       rangeStart  = (optional) start-point for the reading of data (0-based; depending on the rangeType, defined either as an epoch/unix
%                     timestamp or samplenumber). Pass -1 (default) to start at the first sample or beginning of the timeseries.
%       rangeEnd    = (optional) end-point to stop the of reading data (0-based; depending on the rangeType, defined either as an epoch/unix
%                     timestamp or samplenumber). Pass -1 (default) to end at the last sample or end of the timeseries
%       ranges      = (optional) a Nx2 matrix of multiple ranges (start- and end-points) for the reading of data.
%
%
%   Returns:
%       metadata    = A structure that contains all session/channel/segment metadata
%       data        = A matrix of doubles containing the signal data (of the requested channels). The matrix will be formatted as
%                     <channels> x <samples/time>, with the first dimension (rows) representing the channels (ordered based on
%                     the 'channels' input argument) and the second dimension (columns) the samples/time. If multiple ranges are
%                     requested then the return format will be <channels> x <samples/time> x <ranges>, so that the third
%                     dimension represents the requested ranges/epochs.
% 
%
%   Notes:
%       - When the rangeType is set to 'samples', the function simply returns the samples as they are found (consecutively) in
%         the datafile, without any regard for time or data gaps; Meaning that, if there is a time-gap between samples, then 
%         these will not appear in the returned result. In contrast, the 'time' rangeType will return the data with NaN values
%         in place for the missing samples.
%       - Because the range is 0-based, data are loaded "up-till" the range end-index. So the result does not include the value
%         at the end-index (e.g. a requested sample range of 0-3 will return first 3 values, being the values at [0], [1], [2])
%       - Since this is a high-level read function, the data that is returned has the unit conversion factor applied. Use the
%         low-level read function to read the raw data without applying the unit conversion factor
%
%
%   Examples:
%
%       % single range/epoch
%       [metadata]             = readMef3('./mefSessDir.mefd/');                              % read metadata only  
%       [metadata, signaldata] = readMef3('./mefSessDir.mefd/', [], {'Ch02', 'Ch07'});        % read metadata and two channels of data
%       [metadata, signaldata] = readMef3('./mefSessDir.mefd/', [], [], 'samples', 0, 1000);  % read all channels, samples 0-1000 
%
%       % multiple ranges/epochs
%       ranges = [[0,    1000]; ...
%                 [1000, 2000]; ...
%                 [5000, 6000]];
%       [metadata, signaldata] = readMef3('./mefSessionData/', [], [], 'samples', ranges);
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
function [metadata, data] = readMef3(sessPath, password, channels, rangeType, varargin)
    metadata = [];    
    data = [];
    
    % set defaults
    if ~exist('password', 'var') || isempty(password),  password = [];  end
    if ~exist('channels', 'var') || isempty(channels),  channels = {};  end
    
    % make sure the session directory is valid and exists
    if ~exist('sessPath', 'var') || isempty(sessPath) || ~ischar(sessPath)
        error('Error: missing or invalid session directory');
    end
    if ~exist(sessPath, 'dir')
        error(['Error: session directory ''', sessPath, ''' could not be found']);
    end
    
    % check the password input
    if ~isempty(password)
        if isstring(password)
            password = num2str(password);
        elseif ~ischar(password)
            error('Error: invalid ''password'' input argument, should either be empty or a character-array');
        end
    end

    % read all the metadata in the session (including channels and segments)
    try
        metadata = read_mef_session_metadata(sessPath, password);
    catch e
        error('%s\nUnable to read MEF3 metadata', e.message);
    end
    
    % check whether any (meta)data was found
    % note: kind of weak check, but the underlying meflib doesn't give
    % us much more to work with in terms of error handling
    if isempty(metadata) || ...
        ~isfield(metadata, 'earliest_start_time') || metadata.earliest_start_time < 0 || ...
        ~isfield(metadata, 'latest_end_time') || metadata.latest_end_time < 0
        
        error('Error: no MEF3 (meta)data found in directory ''%s''', sessPath);
    end
    
    % warn if there are no channels
    if metadata.number_of_time_series_channels == 0 && metadata.number_of_video_channels == 0
        warning('on'); warning('backtrace', 'off');
        warning('No channels found in session directory');
    end
    
    % check if too many input arguments are given
    if nargin > 6
        error('Error: too many input arguments');
    end
    
    % check if signal data should be returned
    if nargout > 1
        
        % check the range type input argument
        if ~exist('rangeType', 'var'),  rangeType = 'samples';  end
        if isempty(rangeType) || ~ischar(rangeType)
            error('Error: invalid rangeType input argument, should be either ''time'' or ''samples''');
        end
        rangeType = lower(rangeType);
        if ~strcmp(rangeType, 'time') && ~strcmp(rangeType, 'samples')
            error('Error: invalid rangeType input argument, should be either ''time'' or ''samples''');
        end
        
        % set range defaults
        if ~exist('rangeStart', 'var'), rangeStart = -1;        end
        if ~exist('rangeEnd', 'var'),   rangeEnd = -1;          end
        ranges = [-1, -1];
        
        % check whether a single range or multiple ranges are given
        if nargin == 5
            % 5 input args, matrix with multiple ranges given

            % retrieve the ranges
            ranges = varargin{1};
            
            % check the range start input argument
            if isempty(ranges) || ~isnumeric(ranges) || size(ranges, 2) ~= 2 || size(ranges, 1) < 1 || any(ranges(:) < 0) || any(rem(ranges, 1) ~= 0, 'all')
                error('Error: invalid ''ranges'' input argument, should be a Nx2 matrix with whole numeric values (>= 0)');
            end
            
            % sort the ranges and calculate their lengths
            ranges = sort(ranges, 2, 'asc');
            rangesDiff = ranges(:, 2) - ranges(:, 1);
            
            % check whether any of the ranges results in a length of 0
            if any(rangesDiff <= 0)
                error('Error: invalid ''ranges'' input argument, one or more ranges result in a length of 0');
            end
            
            % warn if input range differs in length, data will be padded with nans at the end
            if any(rangesDiff ~= rangesDiff(1))
                warning('on'); warning('backtrace', 'off');
                warning('The ranges that were requested differ in length, data will be padded with nans at the end');
            end
            
        elseif nargin == 6
            % 6 input args, range start and end given
            
            % check the range start and end input arguments
            if isempty(rangeStart) || ~isnumeric(rangeStart) || length(rangeStart) ~= 1 || (~(rangeStart == -1) && rangeStart < 0) || rem(rangeStart, 1) ~= 0
                error('Error: invalid rangeStart input argument, should be a single whole numeric value (either -1 or >= 0)');
            end
            if isempty(rangeEnd) || ~isnumeric(rangeEnd) || length(rangeEnd) ~= 1 || (~(rangeEnd == -1) && rangeEnd < 0) || rem(rangeEnd, 1) ~= 0
                error('Error: invalid rangeEnd input argument, should be a single whole numeric value (either -1 or >= 0)');
            end

            % transfer the start and end of the range
            ranges = [varargin{1}, varargin{2}];
            
            % if there are no -1's, % order and check length
            if ~any(ranges == -1)
                ranges = sort(ranges, 2, 'asc');
                if ranges(2) - ranges(1) <= 0
                    error('Error: invalid range, the requested range length is 0');
                end
            end
            
        end
    end

    
    % 
    % sort the channels in the metadata (using channel->metadata->section_2->acquisition_channel_number)
    % 

    % list the acquisition channel numbers
    acqChNum = [];
    for iChannel = 1:metadata.number_of_time_series_channels
        acqChNum(iChannel) = metadata.time_series_channels(iChannel).metadata.section_2.acquisition_channel_number;
    end

    % sort the channels
    [ordAcqChNum, prevIndex] = sort(acqChNum);

    % check if it starts at one
    if min(acqChNum) ~= 1
        warning('on'); warning('backtrace', 'off');
        warning('The acquisition channel count does not start at 1, check the (metadata) output to see if ordered correctly');
    end

    % check if not consecutive
    if ~isempty(setdiff(min(acqChNum):max(acqChNum), acqChNum))
        warning('on'); warning('backtrace', 'off');
        warning('The acquisition channel count is not consecutive, check the (metadata) output to see if ordered correctly');
    end

    % re-order the channels in the metadata
    for iChannel = 1:length(ordAcqChNum)
        tmpStruct(iChannel) = metadata.time_series_channels(prevIndex(iChannel));
    end
    metadata.time_series_channels = tmpStruct;

    
    %
    % load signal data
    %
    
    % check if signal data should be returned
    if nargout > 1
        
        % include all channel if no specific channels were given
        if isempty(channels)
            for iChannel = 1:metadata.number_of_time_series_channels
                channels{end + 1} = metadata.time_series_channels(iChannel).name;
            end
        end
        
        % allow the request of a single channel as a string argument
        if ischar(channels), channels = {channels}; end
        if isstring(channels), channels = cellstr(channels);    end
        
        % check the channels input argument
        if ~isvector(channels)
            error('Error: invalid ''channels'' input argument, should be passed as a one-dimensional cell-array');
        elseif ~iscell(channels)
            error('Error: invalid input argument for ''channels'', should be a cell array containing channel names (e.g. {''Ch1'', ''Ch2'', ''Ch3''})');
        end
        for iChannel = 1:length(channels)
            if ~ischar(channels{iChannel})
                error('Error: invalid input argument for ''channels'', should be a cell array containing channel names (e.g. {''Ch1'', ''Ch2'', ''Ch3''})');
            end
        end
        
        % check for duplicate names
        [uEntries, ~, uIdxs] = unique(channels);
        nEntries = histc(uIdxs, 1:numel(uEntries));
        if any(nEntries > 1)
            error(['Error: invalid ''channels'' input argument, array contains duplicate names: ', num2str(strjoin(strcat('''', uEntries(nEntries > 1), ''''), ', '))]);
        end
        
        % make sure all requested channels exist
        % note: if one or more channels are not found will return an error. Channel selection can be sensitive, this
        %       approach prevents unexpected consequences that could arise when - instead - returning less or empty channels
        channelsFound = ismember(lower(channels), lower({metadata.time_series_channels.name}));
        if sum(channelsFound) < length(channels)
            for iChannel = 1:length(channels)
                if channelsFound(iChannel) == 0
                    error('Error: requested channel ''%s'' was not found', channels{iChannel});
                end
            end
            return;
        end
        
        % catch errors
        try
            
            % loop through the channels in the order they are requested
            for iChannel = 1:length(channels)
                channelIndex = find(ismember(lower({metadata.time_series_channels.name}), lower(channels{iChannel})));
                channelPath = [metadata.time_series_channels(channelIndex).path, filesep, metadata.time_series_channels(channelIndex).name, '.', metadata.time_series_channels(channelIndex).extension];
                
                %
                % read the data of each channel
                %

                % loop through the ranges
                for iRange = 1:size(ranges, 1)

                    % read the signal
                    signal = read_mef_ts_data(channelPath, password, rangeType, int64(ranges(iRange, 1)), int64(ranges(iRange, 2)), true)';

                    % on the first read, initialize the array
                    % note: we cannot beforehand determine the size of matrix because of potential
                    %       recording gaps in the data. Therefore, we allocate memory here.
                    if iChannel == 1 && iRange == 1
                        data = nan(length(channels), length(signal), size(ranges, 1));
                    end

                    % if signal too large for the data matrix, pad data matrix with nans
                    if length(signal) > size(data, 2)
                       data = padarray(data, [0, (length(signal) - size(data, 2))], nan, 'post');
                    end

                    % store the data 
                    data(iChannel, 1:length(signal), iRange) = signal;

                end
                
            end

        catch e
            
            % error message
            error('%s\nError while reading MEF3 channel data', e.message);
            
        end
        
    end
    
end
