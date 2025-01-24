function loadAndRun(eefOption,executableFile,~,varargin)

fprintf('Launcher command /n');


this_file_dir = mfilename('fullpath');
[this_dir, ~] = fileparts(this_file_dir);

[path, name, ext] = fileparts(executableFile);
copyfile(executableFile,fullfile(path,[name '.elf']));

setupFile_Dir = strrep(this_dir, '+matlabshared\+target\+am26x\+am261x', 'toolchain\am261x\+matlabshared\+toolchain\+ti_arm_clang');

load(fullfile(setupFile_Dir, 'tool_install_paths.mat'), "tool_install_path");

ccsPath = strrep(string(tool_install_path(1)), '\','/');

ccsPath = convertStringsToChars(ccsPath);

%ccsPath = 'C:/ti/ccs1270/'

thisFilePath = fileparts(mfilename('fullpath') )
thisFilePath = replace (thisFilePath , '\', '/')

ccxmlPath = [thisFilePath '/AM261x.ccxml']
executableFile = replace (executableFile , '\', '/');
binaryPath = executableFile;
disp(executableFile);
dsspath = [ccsPath '/ccs/ccs_base/scripting/bin/']
LoadJSpath = [thisFilePath '/openSession.js']

dssBatFile = [dsspath 'dss.bat']
commandStr = [dssBatFile ' ' LoadJSpath ' ' ccxmlPath ' ' binaryPath ] 
system(commandStr); 


end
