%    
%   Writes time-series data (.tdat & tidx) for a specified segment
%
%   write_mef_ts_segment_data(channelPath, segmentNum, passwordL1, passwordL2, samplesPerBlock, data)
%
%       channelPath         = Absolute path to the MEF3 channel directory (to be created or existing)
%       segmentNum          = The segment number. Should be 0 or a positive integer (1, 2, ...)
%       passwordL1          = Segment data level 1 password; Pass empty string/variable for no encryption
%       passwordL2          = Segment data level 2 password; Pass empty string/variable for no encryption
%       samplesPerBlock     = Number of samples per MEF 3 block
%       data                = The data to write as a 1-D array of data-type int32
%
%   Note:  This function requires that a time-series metadata file (.tmet) is already written for the 
%          specified segment. The universal-header data of the metadata file (.tmet) will be the base for
%          universal-headers of the data files (.tdat & tidx). In addition, universal header fields in the
%          metadata file (.tmet) will be updated according to the data that is passed to this function
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
function write_mef_ts_segment_data(channelPath, segmentNum, passwordL1, passwordL2, samplesPerBlock, data)
