function createProjectSpecFileAM263Px(CGTInstallationDirectory, tokenName, TIDriverPath , projectName,  cgtVersion, outputFormat, linker_flags, compiler_flags, listofFiles, includeFiles, compiler_defines, linker_defines, configFile, Libraries, projectSpecfileName, processingUnit, bootFromFlash, mainSrcDir, cgtPath, deviceID, targetHardwareInfo, toolchain)

% CREATEPROJECTSPECFILE transforms the variables required for the .projectspec file for TI CCS project creation.
%
% PARAMETERS:
% tokenName - Macro for path replacement (e.g., 'C2000WAREINSTALLDIR').
% TIDriverPath  - Actual path value corresponding to the macro.
% projectName - Name of the CCS project.
% cgtVersion - Version of code generation tools.
% outputFormat - Desired output format for the project.
% linker_flags - Flags for the linking process.
% compiler_flags - Flags for the compilation process.
% listofFiles - List of source files to include.
% includeFiles - List of include directories.
% compiler_defines - Compiler-specific defines.
% linker_defines - Linker-specific defines.
% configFile - Configuration file for the project.
% Libraries - Libraries to link with the project.
% projectSpecfileName - Name of the output .projectspec file.
% processingUnit - Processing unit used (e.g., 'CortexM4').
% bootFromFlash - Flag indicating boot from flash (1 for true).
% mainSrcDir - Main source directory for the project.
% cgtPath - Path to code generation tools.
% deviceID - Identifier for the target device.
% targetHardwareInfo  - Data target specification.
% toolchain - Toolchain for code generation.

%   Copyright 2024 The MathWorks, Inc.

% Replacing the c2000ware path with the MACRO

% Initialize the updated file lists
listOfFilesWithC2000WareDirMacro = listofFiles;
endianness = 'little'; % little by default
LibrariesWithC2000WareMacro = '';

% Modifying the full path to the relative path with respect to the .projectSpec directory
% to incorporate the portability of the code across different users.
% This section converts absolute include paths to relative paths using the CCSProjectFolder macro.
% For example, if the include path is 'C:\Users\Username\Projects\MyProject\Include',
% and the CCS workspace is            'C:\Users\Username\Projects\MyProject\CCS_Project',
% the relative path becomes '..\Include', and the macro will be appended as '${CCSProjectFolder}\..\Include'.

replaceStringCCS = '${CCSProjectFolder}\';

% Construct the full CCS workspace path
ccsWorkspace = fullfile(mainSrcDir, 'CCS_Project');

% Preallocate updated include files, replacing the full path with the relative paths
includeFilesWithCCSProjectMacro= includeFiles;

% Loop through each path in the INCLUDEFILES
for i = 1:length(includeFiles)

    % Use the getRelativeFilePath function to calculate the relative path
    relativePath = am263x.getRelativeFilePath(ccsWorkspace, includeFiles{i});

    % Check if the relative path is different from the original include path
    if ~strcmp(includeFiles{i}, relativePath)
        % If different, append the CCSProjectFolder macro
        includeFilesWithCCSProjectMacro{i} = fullfile(replaceStringCCS, relativePath);
    else
        % If the same, keep the original include path
        includeFilesWithCCSProjectMacro{i} = includeFiles{i};
    end
end


% 'I' is needed before the includefiles as per the .projectspec syntax
% Add the '-I ' prefix to each file path (note the space after -I)
includeFilesWithPrefix = strcat('-I ', includeFilesWithCCSProjectMacro);
% Join all the file paths into a single string separated by spaces
% (projectspec format requirement for include files )
includeFilesString = strjoin(includeFilesWithPrefix, ' ');


%   'arm_cortex_m_multitasking.c' is not having the full path in the
%   listOfFiles , so need to search if this is present then  need  to
%   append the full path with it , ( projectspec requires the fullpath of
%   the files )
% File to search for
fileToSearch = 'arm_cortex_m_multitasking.c';
% Check if the file is present in the list
fileFound  = false;
for i = 1:length(listOfFilesWithC2000WareDirMacro)
    if strcmp(listOfFilesWithC2000WareDirMacro{i}, fileToSearch)
        fileFound  = true;
        break;
    end
end
% Append the path if the file is found
if fileFound 
    new_file = fullfile(pwd, fileToSearch);
    listOfFilesWithFullPathAppend = [listOfFilesWithC2000WareDirMacro(1:i-1), {new_file}, listOfFilesWithC2000WareDirMacro(i+1:end)];
else
    listOfFilesWithFullPathAppend = listOfFilesWithC2000WareDirMacro;
end


% Initialize a cell array to hold the updated file paths
listOfFilesWithRelativePath = cell(size(listOfFilesWithFullPathAppend));

% Define the CCS project folder path
ccsProjectFolderPath = 'CCSProjectFolder';

% Loop through each file path
for i = 1:length(listOfFilesWithFullPathAppend)
    filePath = listOfFilesWithFullPathAppend{i};

    % Construct the full path to the CCS_Project directory
    ccsProjectDir = fullfile(mainSrcDir, 'CCS_Project');

    % Check if the file path starts with the main source directory
    if startsWith(filePath, mainSrcDir)
        % Use the getRelativeFilePath function to calculate the relative path
        relativePath = am263x.getRelativeFilePath(ccsProjectDir, filePath);

        % use the relative path 
        listOfFilesWithRelativePath{i} = relativePath;
    else
        % Keep the original path for files outside the main source directory
        listOfFilesWithRelativePath{i} = filePath;
    end
end


% constructing the configuration name
% Determine configuration based on processing unit and boot source
if strcmp(processingUnit, 'R5')
    % For CortexM4, use 'CM'
    coreName = 'Cortex R.AM263Px';

    if contains(compiler_flags, '-mlittle-endian')
        endianness = 'little';
		compiler_flags = strrep(compiler_flags, '-mlittle-endian', '');
    elseif contains(compiler_flags, '-mbig-endian')
        endianness = 'big';  
    end
     % Remove '-ml  -c' from the compiler flags :for arm core
    compiler_flags = strrep(compiler_flags, '-c', '');
	compiler_flags = strrep(compiler_flags, '-ml', '');
else
    % For other processing units, determine CPU1 or CPU2 ( including CPU1CLA, CPU2CLA )
    %% endianness isn't defined for the c28x core in the compiler flags as it is little by default and uneditable in CCS project properties , it's only coming up for the ARM cortex in the compiler flags 
    if contains(processingUnit, 'CPU1')
        coreName = 'CPU1';
    elseif contains(processingUnit, 'CPU2')
        coreName = 'CPU2';
    end
end
% Determine RAM or Flash based on boot source
if bootFromFlash == 1
    memoryType = 'FLASH';
else
    memoryType = 'RAM';
end
% Construct the final configuration name
configurationName = 'debug';


% projectSPec requires compiler flags and linker flags  as a string
% Convert cell array to string array
compiler_defines = string(compiler_defines);
% Join all the entries into a single string separated by spaces
compiler_defines_string = strjoin(compiler_defines, ' ');
% Convert cell array to string array
linker_defines = string(linker_defines);
% Join all the entries into a single string separated by spaces
linker_defines_string = strjoin(linker_defines, ' ');


% Compiler_flags modification as required by .projectspec
%replace the 'i' in compiler flags with 'I' : required by .projectSPec
compiler_flags = strrep(compiler_flags, '-i$', '-I$');

flagsArray = strsplit(compiler_flags, ' ');
compiler_flags = strjoin(flagsArray, ' ');


% Create the .projectspec file
am263x.printProjectSpec(endianness, CGTInstallationDirectory, ccsProjectFolderPath, tokenName, TIDriverPath ,processingUnit, mainSrcDir, projectName, cgtVersion, outputFormat, linker_flags, configFile, LibrariesWithC2000WareMacro, projectSpecfileName, cgtPath, deviceID, includeFilesString, configurationName, compiler_defines_string, linker_defines_string, compiler_flags, listOfFilesWithRelativePath);

end