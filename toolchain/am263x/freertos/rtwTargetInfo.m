function rtwTargetInfo(tr)
%RTWTARGETINFO Target info callback

% Copyright 2016 The MathWorks, Inc.

tr.registerTargetInfo(@loc_createToolchain);
end

% -------------------------------------------------------------------------
function config = loc_createToolchain
config = coder.make.ToolchainInfoRegistry; % initialize
[toolsDir, ~, ~] = fileparts(mfilename('fullpath'));

if strcmpi(computer('arch'), 'win64')
    config(end).Name                = 'TI ARM Clang Compiler AM263x FreeRTOS | gmake';
    config(end).Alias               = 'TI ARM Clang Compiler AM263X FreeRTOS | gmake';
    config(end).TargetHWDeviceType	= {'*'};
    config(end).FileName        = fullfile(toolsDir, 'ti_arm_clang_gmake_win64_AM263XFreeRTOS.mat');
    config(end).Platform        = {'win64'};
end

end
