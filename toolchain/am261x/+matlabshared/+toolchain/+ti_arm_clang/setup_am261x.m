function setup
% SETUP Add TI Tools path to an environment variables to a makefile

tmp_Fullpath = mfilename('fullpath');
[setupFileDir, ~] = fileparts(tmp_Fullpath);

load(fullfile(setupFileDir, 'tool_install_paths.mat'), "tool_install_path");


makefileTokens(1).Name  = 'TI_MW_SCRIPTINGTOOLS';
makefileTokens(1).Value = strrep(string(tool_install_path(1)), '\','/');

makefileTokens(2).Name  = 'CCSROOT';
makefileTokens(2).Value = strrep(string(tool_install_path(1)), '\','/');

makefileTokens(3).Name  = 'CCSARMINSTALLDIR';
makefileTokens(3).Value = strrep(string(tool_install_path(2)), '\','/');

makefileTokens(4).Name  = 'MCUSDKINSTALLDIR';
makefileTokens(4).Value = strrep(string(tool_install_path(5)), '\','/');

makefileTokens(5).Name  = 'LINKERCMDDIR';
makefileTokens(5).Value = strrep(setupFileDir, '\','/');

createToolchainPathMakefile(makefileTokens);
end

function createToolchainPathMakefile(makefileTokens)
% Do not change order of the following functions
fileName = 'mw_ti_arm_tools_path.mk';
buildDir = pwd;
[fid,errMsg] = fopen(fullfile(buildDir,fileName), 'w');
if (isequal(fid, -1))
    error(message('codertarget:build:AssemblyFlagsFileError',fileName,errMsg));
end
fidCleanup = onCleanup(@() fclose(fid));
for i=1:numel(makefileTokens)
    if ispc
        rootDir = strrep(makefileTokens(i).Value, '\', '/');
    end
    fprintf(fid,'%s = %s\n',makefileTokens(i).Name,rootDir);
end
end