function loadAndRun(eefOption,executableFile,~,varargin)

fprintf('Launcher command /n');


this_file_dir = mfilename('fullpath');
[this_dir, ~] = fileparts(this_file_dir);

[path, name, ext] = fileparts(executableFile);
copyfile(executableFile,fullfile(path,[name '.elf']));

setupFile_Dir = strrep(this_dir, '+matlabshared\+target\+am26x\+am263px', 'toolchain\am263px\+matlabshared\+toolchain\+ti_arm_clang');

load(fullfile(setupFile_Dir, 'tool_install_paths.mat'), "tool_install_path");

ccsPath = strrep(string(tool_install_path(1)), '\','/');

ccsPath = convertStringsToChars(ccsPath);

%ccsPath = 'C:/ti/ccs1270/'

thisFilePath = fileparts(mfilename('fullpath') )
thisFilePath = replace (thisFilePath , '\', '/')

ccxmlPath = [thisFilePath '/AM263Px.ccxml']
executableFile = replace (executableFile , '\', '/');
binaryPath = executableFile;
disp(executableFile);
dsspath = [ccsPath '/ccs/ccs_base/scripting/bin/']
hCS=getActiveConfigSet(bdroot) 
coreSelected = codertarget.data.getParameterValue(hCS, 'CpuInfo.CoreSelection') 

LoadJSpath = [thisFilePath '/openSession.js']

dssBatFile = [dsspath 'dss.bat']

if(strcmp(coreSelected,'Core0'))
    commandStr = [dssBatFile ' ' LoadJSpath ' ' ccxmlPath ' ' binaryPath ' ' '".*Cortex_R5_0*"'] 
elseif(strcmp(coreSelected,'Core1'))
    commandStr = [dssBatFile ' ' LoadJSpath ' ' ccxmlPath ' ' binaryPath ' ' '".*Cortex_R5_1*"'] 
elseif(strcmp(coreSelected,'Core2'))
    commandStr = [dssBatFile ' ' LoadJSpath ' ' ccxmlPath ' ' binaryPath ' ' '".*Cortex_R5_2*"']
elseif(strcmp(coreSelected,'Core3'))
    commandStr = [dssBatFile ' ' LoadJSpath ' ' ccxmlPath ' ' binaryPath ' ' '".*Cortex_R5_3*"'] 
end

system(commandStr); 


end
