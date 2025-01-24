function makeInfo = rtwmakecfg
%RTWMAKECFG adds include and source directories to code generation make files.

% Specify include directories
srcDir = fullfile(fileparts(mfilename('fullpath')),'src'); %#ok<NASGU>
includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
makeInfo.includePath = {includeDir};

% Specify source files
makeInfo.sourcePath = {srcDir};
makeInfo.sources = {'MW_hwi.c'};

% Specify library directories and libraries
% makeInfo.libraryPath = {'path/to/libs'};
% makeInfo.libraries = {'libname1', 'libname2'};

% Specify preprocessor definitions
% makeInfo.precompile = struct('precompile', '', 'postcompile', '');
% makeInfo.precompile(1).precompile = {'DEFINE1=1', 'DEFINE2=0'};