%
%   Read the MEF3 data from a time-series channel
%
%   [data] = read_mef_ts_data(channelPath, password, rangeType, rangeStart, rangeEnd, applyConvFactor)
%
%       channelPath     = path (absolute or relative) to the MEF3 channel folder
%       password        = password to the MEF3 data; Pass empty string/variable if not encrypted. Default is ''.
%       rangeType       = Modality that is used to define the data-range to read [either 'time' or 'samples'].
%                         Default is 'samples'.
%       rangeStart      = Start-point for the reading of data (either as an epoch/unix timestamp or samplenumber)
%                         Pass -1 to start at the first sample of the timeseries. Default is -1, first sample
%       rangeEnd        = End-point to stop the of reading data (either as an epoch/unix timestamp or samplenumber)
%                         Pass -1 as value to end at the last sample of the timeseries. Default is -1, last sample
%       applyConvFactor = Apply the unit conversion factor to the raw data [0 = not apply, 1 = apply]
%                         Default = 0 - Do not apply conversion factor
%
%   Returns:
%       data            = A vector of doubles holding the channel data
%
%   Note:  When the rangeType is set to 'samples', the function simply returns the samples as they are
%          found (consecutively) in the datafile, without any regard for time or data gaps; Meaning
%          that, if there is a time-gap between samples, then these will not appear in the result returned.
%          In contrast, the 'time' rangeType will return the data with NaN values in place for the missing samples.
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
function data = read_mef_ts_data(channelPath, password, rangeType, rangeStart, rangeEnd)
