function [outputArg] = hsp_am26x_uninstall

% Function to uninstall AM26x Target, peripheral device driver blocks and
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


% Get path of this file (same as target path)
tmp_Fullpath = mfilename('fullpath');
[tgtDir, ~] = fileparts(tmp_Fullpath);

% Remove from MATLAB path !

rmpath(tgtDir);
rmpath(fullfile(tgtDir, 'registry'));
rmpath(fullfile(tgtDir, 'blocks'));
rmpath(fullfile(tgtDir, 'src'));
rmpath(fullfile(tgtDir, 'blocks', 'block_ADC'));
rmpath(fullfile(tgtDir, 'blocks', 'block_CAN'));
rmpath(fullfile(tgtDir, 'blocks', 'block_CHAR'));
rmpath(fullfile(tgtDir, 'blocks', 'block_CMPSS'));
rmpath(fullfile(tgtDir, 'blocks', 'block_DAC'));
rmpath(fullfile(tgtDir, 'blocks', 'block_ECAP'));
rmpath(fullfile(tgtDir, 'blocks', 'block_EPWM'));
rmpath(fullfile(tgtDir, 'blocks', 'block_EQEP'));
rmpath(fullfile(tgtDir, 'blocks', 'block_GPIO'));
rmpath(fullfile(tgtDir, 'blocks', 'block_HWI'));
rmpath(fullfile(tgtDir, 'blocks', 'block_IPC'));
rmpath(fullfile(tgtDir, 'blocks', 'block_IPC_HWI'));
rmpath(fullfile(tgtDir, 'blocks', 'block_MCSPI'));
rmpath(fullfile(tgtDir, 'blocks', 'block_SDFM'));
rmpath(fullfile(tgtDir, 'blocks', 'block_UART'));
disp('Uninstalled AM26x HSP and device driver blocks!');

rmpath(fullfile(tgtDir, 'toolchain'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263x'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263x', 'nortos'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263x', 'freertos'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263px'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263px', 'nortos'));
rmpath(fullfile(tgtDir, 'toolchain', 'am263px', 'freertos'));
rmpath(fullfile(tgtDir, 'toolchain', 'am261x'));
rmpath(fullfile(tgtDir, 'toolchain', 'am261x', 'nortos'));
rmpath(fullfile(tgtDir, 'toolchain', 'am261x', 'freertos'));
rmpath(fullfile(tgtDir, 'code_replacement'));
rmpath(fullfile(tgtDir, 'sl_cus'));

rmpath(fullfile(tgtDir, 'blocks', 'library'));
rmpath(fullfile(tgtDir, 'blocks', 'library', 'am261x'));
rmpath(fullfile(tgtDir, 'blocks', 'library', 'am263x'));
rmpath(fullfile(tgtDir, 'blocks', 'library', 'am263px'));
disp('Uninstalled TI ARM CLANG toolchain!');

rmpref('AM26xPILpref');
mkdir('./temp');
cd ('./temp');
sl_refresh_customizations;
cd ('..');
rmdir('./temp');
RTW.TargetRegistry.reset;
savepath;


outputArg = 0;

end


