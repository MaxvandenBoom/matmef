% pop_matmef - import MEF file to EEGLAB structure.
%
% Usage:
%   EEG = pop_matmef; % pop up menu to select file
%   EEG = pop_matmef(folder); % import folder
%
% Input:
%  folder - foldername for the MEF data
%
% Output:
%  EEG     - EEGLAB structure

function [EEG, com] = pop_matmef(fileName)

com = '';

if nargin < 1
    
    % pop up window
    % -------------
    fileName = uigetdir('*', 'Select a MEF3 folder');
    if isempty(fileName), return; end
    
end

[metadata, signaldata] = readMef3(fileName);  
EEG = eeg_emptyset;
EEG.data   = signaldata;
EEG.nbchan = size(EEG.data,1);
EEG.pnts   = size(EEG.data,2);

if isfield(metadata, 'time_series_channels')
    for iChan = 1:length(metadata.time_series_channels)
        if isfield(metadata.time_series_channels(iChan), 'name')
            EEG.chanlocs(iChan).labels = metadata.time_series_channels(iChan).name;
        end
    end
end
if isfield(metadata, 'time_series_metadata')
    sections = fieldnames(metadata.time_series_metadata);
    for iSection = 1:length(sections)
        if isfield(metadata.time_series_metadata.(sections{iSection}), 'sampling_frequency')
            EEG.srate = metadata.time_series_metadata.(sections{iSection}).sampling_frequency;
        end
    end
end

com = sprintf('EEG = pop_matmef(''%s'');', fileName);
