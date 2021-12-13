% eegplugin_matmef() - plugin for importing read MEF 3.0 data
%
% Usage:
%   >> eegplugin_mffmatlabio(fig, trystrs, catchstrs);
%
% Inputs:
%   fig        - [integer] eeglab figure.
%   trystrs    - [struct] "try" strings for menu callbacks.
%   catchstrs  - [struct] "catch" strings for menu callbacks.
%

function versionstr = eegplugin_matmef(fig, trystrs, catchstrs)

    % retrieve the version
    try 
        versionstr = fileread('VERSION');
    catch 
        disp('eegplugin_matmef cannot find the ''VERSION'' file');
        return
    end
    
    % check input arguments
    if nargin < 3
        disp('eegplugin_matmef requires 3 arguments');
        return
    end
    
    % add amica folder to path
    % -----------------------
    if ~exist('pop_matmef')
        p = which('eegplugin_matmef');
        p = p(1:findstr(p,'eegplugin_matmef.m')-1);
        addpath(p);
    end

    % find tools menu
    % ---------------
    menui = findobj(fig, 'tag', 'import data'); 
    
    % menu callback commands
    % ----------------------
    comload    = [  trystrs.no_check '[EEG, LASTCOM] = pop_matmef;' catchstrs.new_non_empty ];
    
    % create menus (CHANGING THESE MENUS AFFECTS THE MAIN eeglab.m FUNCTION)
    % ------------
    submenu = uimenu( menui, 'Label', 'Import MEF3 folder', 'separator', 'on', 'CallBack', comload);
