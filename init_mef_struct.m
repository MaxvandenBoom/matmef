%    
%   Create and initialize a new universal-header or metadata structure-array with default values
%   
%   mef_struct = init_mef_struct(type)
%
%       type         = The type of structure array, possible values:
%                          'uh'                    for universal-header
%                          'ts_section2' or 'tmd2' for time-series section 2
%                          'v_section2'  or 'vmd2' for video section 2
%                          'section3'    or 'md3'  for section 3
%
%   Returns:
%       mef_struct   = a new structure-array containing default values
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
function mef_struct = init_mef_struct(type)
