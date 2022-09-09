% eegplugin_mef3() - plugin for importing read MEF 3.0 data
%
% Usage:
%   >> eegplugin_mffmatlabio(fig, trystrs, catchstrs);
%
% Inputs:
%   fig        - [integer] eeglab figure.
%   trystrs    - [struct] "try" strings for menu callbacks.
%   catchstrs  - [struct] "catch" strings for menu callbacks.
%

function versionstr = eegplugin_mef3(fig, trystrs, catchstrs)

    % retrieve the version
    try 
        versionstr = fileread('VERSION');
    catch 
        disp('eegplugin_mef3 cannot find the ''VERSION'' file');
        return
    end
    
    % check input arguments
    if nargin < 3
        disp('eegplugin_mef3 requires 3 arguments');
        return
    end
    
    % add amica folder to path
    % -----------------------
    if ~exist('pop_mef3')
        p = which('eegplugin_mef3');
        p = p(1:findstr(p,'eegplugin_mef3.m')-1);
        addpath(p);
    end

    % find tools menu
    % ---------------
    menui = findobj(fig, 'tag', 'import data'); 
    
    % menu callback commands
    % ----------------------
    comload    = [  trystrs.no_check '[EEG, LASTCOM] = pop_mef3;' catchstrs.new_non_empty ];
    
    % create menus (CHANGING THESE MENUS AFFECTS THE MAIN eeglab.m FUNCTION)
    % ------------
    submenu = uimenu( menui, 'Label', 'Import MEF3 folder', 'separator', 'on', 'CallBack', comload);
