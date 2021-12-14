%    
%   Writes time-series data for a specified segment
%
%   write_mef_ts_segment_data(channelPath, segmentNum, passwordL1, passwordL2, samplesPerBlock, data)
%
%       channelPath         = Absolute path to the MEF3 channel directory (to be created or existing)
%       segmentNum          = The segment number. Should be 0 or a positive integer (1, 2, ...)
%       passwordL1          = Segment data level 1 password; Pass empty string/variable if not encrypted
%       passwordL2          = Segment data level 2 password; Pass empty string/variable if not encrypted
%       samplesPerBlock     = Number of samples per MEF 3 block
%       data                = The data to write as a 1-D array of data-type int32
%
%
%   Copyright 2021, Max van den Boom (Multimodal Neuroimaging Lab, Mayo Clinic, Rochester MN)
%   Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)

%   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
%   as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
%   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
%   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%   You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
%
function write_mef_ts_segment_data(channelPath, segmentNum, passwordL1, passwordL2, samplesPerBlock, data)
