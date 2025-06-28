function printProjectSpec(endianness, CGTInstallationDirectory, ccsProjectFolderPath, tokenName, TIDriverPath, processingUnit, mainSrcDir, projectName, cgtVersion, outputFormat, linker_flags, configFile, LibrariesWithC2000WareMacro, projectSpecfileName, cgtPath, deviceID, includeFilesString, configurationName, compiler_defines_string, linker_defines_string, compiler_flags, listOfFilesWithRelativePath)

% PRINTPROJECTSPEC Generates a project specification (.PROJECTSPEC) XML file for Code Composer Studio PROJECT.
%
% This function creates a .projectspec file, which is used by Texas Instruments
% Code Composer Studio (CCS) to define project settings, paths, and configurations.
%
% INPUTS:
%   endianness               - contains the little endian or big endian as per the compiler_flags
%   CGTInstallationDirectory - The directory where CCS is installed.
%   ccsProjectFolderPath     - The folder path for the CCS project.
%   tokenName                - A string representing the token name for the installation directory
%   TIDriverPath             - The value of the path variable.
%   processingUnit           - The processing unit type (e.g., 'CortexM4').
%   mainSrcDir               - The main source directory for the project.
%   projectName              - The name of the project.
%   cgtVersion               - The version of the Code Generation Tools.
%   outputFormat             - The output format for the project.
%   linker_flags             - Flags for the linker.
%   configFile               - The configuration file path.
%   LibrariesWithC2000WareMacro - A list of libraries with C2000Ware macros.
%   projectSpecfileName      - The name of the .projectspec file to be created.
%   cgtPath                  - The path to the Code Generation Tools.
%   deviceID                 - The device ID for the target hardware.
%   includeFilesString       - A string of include file paths.
%   configurationName        - The name of the configuration.
%   compiler_defines_string  - Compiler defines as a string.
%   linker_defines_string    - Linker defines as a string.
%   compiler_flags           - Compiler flags.
%   listOfFilesWithRelativePath - A list of files with their paths relative to the CCS_Project path.
%
% OUTPUTS:
%   The function does not return any outputs but writes the .projectspec file to the specified location.
%
%
%   Copyright 2024 The MathWorks, Inc.

finalDeviceName = strcat('Cortex R.', deviceID);
deviceFamily = 'ARM';
toolChain = 'TICLANG';
file = fopen(projectSpecfileName, 'wt');
% Write the XML header
fprintf(file, '<?xml version="1.0" encoding="utf-8"?>\n');
fprintf(file, '<projectSpec>\n');

%write the applicability 
fprintf(file, '        <applicability>\n');
fprintf(file, '           <when>\n');
fprintf(file, '              <context\n');
fprintf(file, '                 deviceFamily = "%s"\n',deviceFamily);
fprintf(file, '                 deviceId = "%s"\n',finalDeviceName);
fprintf(file, '              />\n');
fprintf(file, '           </when>\n');
fprintf(file, '        </applicability>\n');

fprintf(file, '\n'); %new line

% Write the project element with attributes
fprintf(file, '        <project\n');
fprintf(file, '        name="%s"\n', projectName);


% Concatenate based on the core type
finalDeviceName = strcat('Cortex R.', deviceID);  %% Able to use the direct callback in ARM : device ID is not there in configSet for ARM core
%output format for c28x core
fprintf(file, '        outputFormat="%s"\n', outputFormat);

fprintf(file, '        device="%s"\n', finalDeviceName);
fprintf(file, '        toolChain="%s"\n', toolChain);
fprintf(file, '        cgtVersion="%s"\n', cgtVersion);
fprintf(file, '        deviceCore="%s"\n', 'Cortex_R5_0');
fprintf(file, '        ignoreDefaultDeviceSettings="%s"\n', 'true');
fprintf(file, '        ignoreDefaultCCSSettings="%s"\n', 'true');
fprintf(file, '        endianness="%s"\n', endianness);

fprintf(file, '        launchWizard="False"\n'); 
fprintf(file, '        linkerCommandFile=""\n');
fprintf(file, '        >\n');

% Printing the MACRO definition for C2000WARE_DIR
% replacing the path 'C:\ti\c2000\C2000Ware_5_02_00_00\' with our
% custom MACRO : C2000WARE_DIR
% Define the path variable XML snippet


scope = 'project'; 
% Write the path variable XML snippet to the .projectspec file
fprintf(file, '<pathVariable name="%s" path="%s" scope="%s" />\n', tokenName, TIDriverPath , scope);

% Printing the MACRO definition for CCSProjectFolder to make use of relative paths,
% 'IncludeOptions' in the CCS Project properties works with MACROS but not
% with the absolute Path 
% Define the path variable XML snippet for the CCS project folder
projectFolderPathValue = fullfile(mainSrcDir, 'CCS_Project');
scope = 'project';
% Write the path variable XML snippet to the .projectspec file
fprintf(file, '<pathVariable name="%s" path="%s" scope="%s" />\n', ccsProjectFolderPath, projectFolderPathValue, scope);


% define the TI_INCLUDE manually
projectFolderPathValue = fullfile(CGTInstallationDirectory, 'include');
%modify the compiler_flags here : replace : $(TI_INCLUDE) with the  projectFolderPathValue
%path : 
compiler_flags = strrep(compiler_flags, '$(TI_INCLUDE)', projectFolderPathValue);
compiler_flags = strrep(compiler_flags, ' ', newline);
includeFilesString = regexprep(includeFilesString, '(?<!Program)\s(?!Files)', newline);
compiler_defines_string = strrep(compiler_defines_string, ' ', newline);
linker_flags = strrep(linker_flags, ' ', newline);
linker_defines_string = regexprep(linker_defines_string, '(?<!Program)\s(?!Files)', newline);
%printing the configurations to the projectspec
fprintf(file, '    <configuration name="%s" \n compilerBuildOptions="%s %s %s" \n linkerBuildOptions="%s %s" />\n', ...
    configurationName, compiler_flags, includeFilesString, compiler_defines_string, linker_flags,linker_defines_string );


%Printing the ListOfFiles in the projectspec, one by one
% Loop through each file in listOfFiles
for i = 1:length(listOfFilesWithRelativePath)
    % Extract the string from the cell array
    filePath = listOfFilesWithRelativePath{i};

    % Write the string to the file
    fprintf(file, '    <file action="link" path="%s" targetDirectory=""/>\n', filePath);
end


% Print the Libraries and .cmds
% Loop through each library in Libraries
for i = 1:length(LibrariesWithC2000WareMacro)
    % Extract the string from the cell array
    libraryPath = LibrariesWithC2000WareMacro{i};

    % Remove double quotes from the libraryPath
    % This is required because the 'rtslib' from the createCCSProject
    % contains the "", which is incorrect for the .projectspec format
    libraryPath = strrep(libraryPath, '"', '');

 
    % Check if the library contains the keyword 'rts' and has a '.lib' extension
    if contains(libraryPath, 'rts') && endsWith(libraryPath, '.lib')
        % Extract the library name from the path
        [~, libName, ext] = fileparts(libraryPath);

        % Append the cgtPath with the specific path up to 'lib'
        % as full path is required in the projectspec
        libraryPath = fullfile(cgtPath, 'lib', [libName, ext]);
    end

    % Write the string to the file with the specified format
    fprintf(file, '    <file action="link" path="%s" targetDirectory="" applicableConfigurations="%s"/>\n', libraryPath, configurationName); % pick from config set
end


% print the configFile details to the .projectSpec
%fprintf(file, '<file action="link" path="%s" targetDirectory="" />\n', configFile);


% Close the project and projectSpec elements
fprintf(file, '  </project>\n');
fprintf(file, '</projectSpec>\n');

% Close the file
fclose(file);
end
