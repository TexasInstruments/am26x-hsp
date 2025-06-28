rootDir = codertarget.arm_cortex_r.internal.getRootDir;
toolchainDir = fullfile(rootDir,'toolchain','am263x','+matlabshared','+toolchain','+ti_arm_clang')
load(fullfile(toolchainDir, 'tool_install_paths.mat'), "tool_install_path");

ccsPath = strrep(string(tool_install_path(1)), '\','/')
ccsARMPath = strrep(string(tool_install_path(2)), '\','/')
sdkPath = strrep(string(tool_install_path(3)), '\','/')
linkerPath = toolchainDir;
ccsPath = convertStringsToChars(ccsPath);
ccsARMPath = convertStringsToChars(ccsARMPath);
sdkPath = convertStringsToChars(sdkPath);


setenv('MCUSDKINSTALLDIR',sdkPath); 
setenv('CCSARMINSTALLDIR',ccsARMPath); 
setenv('CCSROOT',ccsPath);
setenv('LINKERCMDDIR',linkerPath);

MCUSDKINSTALLDIR = getenv('MCUSDKINSTALLDIR'); 
CCSARMINSTALLDIR = getenv('CCSARMINSTALLDIR');
CCSROOT = getenv('CCSROOT');
LINKERCMDDIR = getenv('LINKERCMDDIR'); 

setenvFile = fullfile(pwd, 'mwsetenv.bat');
fd = fopen(setenvFile,'w+');
fprintf(fd, 'setx CCSARMINSTALLDIR "%s"\n', CCSARMINSTALLDIR);
fprintf(fd, 'setx CCSROOT "%s"\n', CCSROOT);
fprintf(fd, 'setx MCUSDKINSTALLDIR "%s"\n', MCUSDKINSTALLDIR);
fprintf(fd, 'setx LINKERCMDDIR "%s"\n', LINKERCMDDIR);
fprintf(fd, 'setx TI_TOOLS "%s"\n', fullfile(CCSARMINSTALLDIR,'bin'));
fprintf(fd, 'setx TI_INCLUDE "%s"\n', fullfile(CCSARMINSTALLDIR,'include'));
fprintf(fd, 'setx TI_LIB "%s"\n', fullfile(CCSARMINSTALLDIR,'lib'));
fprintf(fd, 'setx TI_NORTOS "%s"\n', fullfile(MCUSDKINSTALLDIR,'source','kernel','nortos','lib'));
fprintf(fd, 'setx TI_DRIVERS "%s"\n', fullfile(MCUSDKINSTALLDIR,'source','drivers','lib'));
fprintf(fd, 'setx TI_BOARD "%s"\n', fullfile(MCUSDKINSTALLDIR,'source','board','lib'));
fprintf(fd, 'setx SFO_LIB "%s"\n', fullfile(MCUSDKINSTALLDIR,'source','calibration','lib'));
fprintf(fd, 'setx MCUSDK_INCLUDE "%s"\n', fullfile(MCUSDKINSTALLDIR,'source'));
fprintf(fd, 'setx LINKER_CMD_DIR "%s"\n', LINKERCMDDIR);
fclose(fd);   
system(setenvFile);