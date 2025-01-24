function hsp_am26x_setup

% Function to install AM26x Target, peripheral device driver blocks and
% toolchain

%  Copyright (C) 2023-2024 Texas Instruments Incorporated
%
%  Redistribution and use in source and binary forms, with or without
%  modification, are permitted provided that the following conditions
%  are met:
%
%    Redistributions of source code must retain the above copyright
%    notice, this list of conditions and the following disclaimer.
%
%    Redistributions in binary form must reproduce the above copyright
%    notice, this list of conditions and the following disclaimer in the
%    documentation and/or other materials provided with the
%    distribution.
%
%    Neither the name of Texas Instruments Incorporated nor the names of
%    its contributors may be used to endorse or promote products derived
%    from this software without specific prior written permission.
%
%  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
%  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
%  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
%  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
%  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
%  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
%  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
%  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
%  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
%  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
%  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

disp('HSP AM26x setup...');

% Get path of this file (same as target path)
tmp_Fullpath = mfilename('fullpath');
[tgtDir, ~] = fileparts(tmp_Fullpath);

% Validate path for spaces (which causes errors when compiling source code inside target folder)
if ~contains(tgtDir,' ')
    disp('Path validation: Success!');
else
    disp('Path validation: Failed.');
    disp('Please install the package in a valid path (without spaces) and try again');
end

% Add paths to MATLAB environment !

addpath(tgtDir);
addpath(fullfile(tgtDir, 'registry'));
addpath(fullfile(tgtDir, 'blocks'));
addpath(fullfile(tgtDir, 'src'));
addpath(fullfile(tgtDir, 'lib'));
addpath(fullfile(tgtDir, 'blocks', 'block_ADC'));
addpath(fullfile(tgtDir, 'blocks', 'block_CAN'));
addpath(fullfile(tgtDir, 'blocks', 'block_CHAR'));
addpath(fullfile(tgtDir, 'blocks', 'block_CMPSS'));
addpath(fullfile(tgtDir, 'blocks', 'block_DAC'));
addpath(fullfile(tgtDir, 'blocks', 'block_ECAP'));
addpath(fullfile(tgtDir, 'blocks', 'block_EPWM'));
addpath(fullfile(tgtDir, 'blocks', 'block_EQEP'));
addpath(fullfile(tgtDir, 'blocks', 'block_GPIO'));
addpath(fullfile(tgtDir, 'blocks', 'block_MCSPI'));
addpath(fullfile(tgtDir, 'blocks', 'block_HWI'));
addpath(fullfile(tgtDir, 'blocks', 'block_SDFM'));
addpath(fullfile(tgtDir, 'blocks', 'block_UART'));
addpath(fullfile(tgtDir, 'blocks', 'library'));
addpath(fullfile(tgtDir, 'blocks', 'library', 'am261x'));
addpath(fullfile(tgtDir, 'blocks', 'library', 'am263x'));
addpath(fullfile(tgtDir, 'blocks', 'library', 'am263px'));
disp('Installed AM26x HSP and device driver blocks!');
addpath(fullfile(tgtDir, 'toolchain'));
addpath(fullfile(tgtDir, 'toolchain', 'am263x'));
addpath(fullfile(tgtDir, 'toolchain', 'am263x', 'nortos'));
addpath(fullfile(tgtDir, 'toolchain', 'am263x', 'freertos'));
addpath(fullfile(tgtDir, 'toolchain', 'am263px'));
addpath(fullfile(tgtDir, 'toolchain', 'am263px', 'nortos'));
addpath(fullfile(tgtDir, 'toolchain', 'am263px', 'freertos'));
addpath(fullfile(tgtDir, 'toolchain', 'am261x'));
addpath(fullfile(tgtDir, 'toolchain', 'am261x', 'nortos'));
addpath(fullfile(tgtDir, 'toolchain', 'am261x', 'freertos'));
addpath(fullfile(tgtDir, 'code_replacement'));
addpath(fullfile(tgtDir, 'sl_cus'));
disp('Installed TI ARM CLANG toolchain!');

setpref('AM26xPILpref','COMport', 'COM1');
setpref('AM26xPILpref','Baud', '115200');
setpref('AM26xPILpref', 'PILHardware', 'AM263X');



% Get tool paths and save as mat file inside target folder
prompt =   {'Enter CCS installated path:',...
            'Enter TI ARM CLANG compiler installated path:',...
            'Enter AM263x MCUSDK installated path:',...
            'Enter AM263Px MCUSDK installated path:',...
            'Enter AM261x MCUSDK installed path:' ,...
            ' '};
dlgtitle = 'Setup AM26x HSP';
fieldsize = [1 100; 1 100; 1 100; 1 100; 1 100; 5 100];
definput = {'C:\ti\ccs1281', ...
            'C:\ti\ccs1281\ccs\tools\compiler\ti-cgt-armllvm_3.2.2.LTS', ...
            'C:\ti\mcu_plus_sdk_am263x_10_01_00_31',...
            'C:\ti\mcu_plus_sdk_am263px_10_01_00_31',...
            'C:\ti\mcu_plus_sdk_am261x_10_00_01_10',...
            'Please note: It is not mandatory to install both AM263x MCUSDK, AM263Px MCUSDK and AM261x MCUSDK. You may install the MCUSDK for atleast one device (AM263x or AM263Px or AM261x) and enter the corresponding path.'};
tool_install_path = inputdlg(prompt,dlgtitle,fieldsize,definput);

save(fullfile(tgtDir, 'toolchain', 'am263x', '+matlabshared', '+toolchain', '+ti_arm_clang', 'tool_install_paths.mat'), "tool_install_path");
save(fullfile(tgtDir, 'toolchain', 'am263px', '+matlabshared', '+toolchain', '+ti_arm_clang', 'tool_install_paths.mat'), "tool_install_path");
save(fullfile(tgtDir, 'toolchain', 'am261x', '+matlabshared', '+toolchain', '+ti_arm_clang', 'tool_install_paths.mat'), "tool_install_path");

if isempty(tool_install_path)
    disp('Failed!');
else
    disp('Complete!');
end

%Save the paths added to MATLAB environment
savepath;

%Refresh to let target discovered (take effect) in same MATLAB session
sl_refresh_customizations;

end