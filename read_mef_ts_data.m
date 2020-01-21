%	
%   Read the MEF3 data from a time-series channel
%	
%   [data] = read_mef_ts_data(channel, rangeType, rangeStart, rangeEnd)
%	
%   channelPath     = path (absolute or relative) to the MEF3 channel folder
%   password        = password to the MEF3 data; pass an empty string if no password should be used
%   rangeType       = Modality that is used to define the data-range to read [either 'time' or 'samples']
%   rangeStart      = Start-point for the reading of data (either as a timepoint or samplenumber)
%                     Pass -1 to start at the first sample of the timeseries
%   rangeEnd 		= End-point to stop the of reading data (either as a timepoint or samplenumber)
%                     Pass -1 as value to end at the last sample of the timeseries
%
%   Returns: 
%       data        = A vector of doubles holding the channel data
%
%
%   Copyright 2020, Max van den Boom and Mayo Clinic (Rochester MN)
%   Adapted from PyMef (by Jan Cimbalnik, Matt Stead, Ben Brinkmann, and Dan Crepeau)

%   This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License
%   as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
%   This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied 
%   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
%   You should have received a copy of the GNU General Public License along with this program.  If not, see <https://www.gnu.org/licenses/>.
%
function data = read_mef_ts_data(channelPath, password, rangeType, rangeStart, rangeEnd)
