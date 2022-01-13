%    
%   Write a new time-series or video metadata file for a specific segment
%
%   write_mef_segment_metadata(channelType, channelPath, segmentNum, password1, password2, startTime, endTime, anonName, section2, section3)
%
%       channelType  = The type of channel ['timeseries' or 'ts' for time-series, 'v' or 'video' for video]
%       channelPath  = Absolute path to the MEF3 channel directory (to be created or existing)
%       segmentNum   = The segment number. Should be 0 or a positive integer (1, 2, ...)
%       passwordL1   = Segment metadata level 1 password; Pass empty string/variable for no encryption
%       passwordL2   = Segment metadata level 2 password; Pass empty string/variable for no encryption
%       startTime    = The start epoch time in microseconds (μUTC format) to be stored in the universal-header
%       endTime      = The end epoch time in microseconds (μUTC format) to be stored in the universal-header
%       anonName     = The anonymized subject name to be stored in the universal-header
%       section2     = Structure containing section 2 metadata. Be sure to pass time-series section 2 metadata when
%                      writing to a time-series channel-type, and pass video metadata section 2 for a video channel type
%       section3     = Structure containing section 3 metadata
%
%
%   Copyright 2022, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
%   Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)

%   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
%   as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
%   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
%   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%   You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
%
function write_mef_segment_metadata(channelType, channelPath, segmentNum, passwordL1, passwordL2, startTime, endTime, anonName, section2, section3)
