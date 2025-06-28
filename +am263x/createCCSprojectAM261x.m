function createCCSprojectAM261x(hCS,buildInfo)
	ccsPath = 'C:\ti\ccs1281';
		projectName = buildInfo.getBuildName;
	BuildDir = RTW.getBuildDir(projectName);
	project_folder = fullfile(BuildDir.BuildDirectory,'CCS_Project');

	targetInfo = codertarget.attributes.getTargetHardwareAttributes(hCS);
	isProfilingEnabled = isequal(get_param(hCS,'CodeExecutionProfiling'), 'on');
    
    this_file_dir = mfilename('fullpath');
    [this_dir, ~] = fileparts(this_file_dir);
	CGTInstallationDirectory = this_dir;
	tokenName = 'MCU_PLUS_SDK_PATH';
	TIDriverPath = getenv('MCU_PLUS_SDK_PATH');
	cgtPath = '';
	bootFromFlash = 0;
	deviceID = 'AM261x';
	configFile = '';
	processingUnit = 'R5';
	%% Get the list of source files
	listOfFiles = buildInfo.getSourceFiles(true,true);
	for index = 1:numel(listOfFiles)
    	listOfFiles{index} = codertarget.utils.replaceTokens(hCS,listOfFiles{index},targetInfo.Tokens);
    	CodeProfilingInstrumentation = get_param(projectName,'CodeProfilingInstrumentation');
    	% When profiling is enabled all source files should be considered from instrumented folder
    	if isProfilingEnabled && contains(listOfFiles{index}, BuildDir.BuildDirectory) && ...
            	~contains(listOfFiles{index}, fullfile(BuildDir.BuildDirectory, 'instrumented')) &&...
            	( strcmp(CodeProfilingInstrumentation, 'coarse') || strcmp(CodeProfilingInstrumentation, 'detailed') ) %% if 'Measure function execution times'  is 'coarse' or 'detailed' then a separate copy of .c will be created inside the instrumented folder and the same code will be deployed on the hardware
        	listOfFiles{index} = strrep(listOfFiles{index}, BuildDir.BuildDirectory, fullfile(BuildDir.BuildDirectory, 'instrumented'));
    	end
	end
	filenames = buildInfo.getSourceFiles(false,false);
	
	%% Get the list of compiler defines
	compiler_defines = buildInfo.getDefines;
	
	%% Get the list of all include files
	includeFiles = buildInfo.getIncludePaths(true);
	for index = 1:numel(includeFiles)
    	includeFiles{index} = codertarget.utils.replaceTokens(hCS,includeFiles{index},targetInfo.Tokens);
    	if (isequal((strfind(includeFiles{index},'..')),1))
        	% If any of the include paths are relative, construct the full
        	% path with the build directory
        	includeFiles{index} = fullfile(BuildDir.BuildDirectory,includeFiles{index});
    	end
    	
	end
	%% Get the list of all the special tokens used
    rootFolders = buildInfo.Settings.getRootFolders();
    sDirs = { rootFolders.PathPattern };
    sToks = { rootFolders.TokenPattern };

    %% Get the list of all linker files
    linkObjs = getLinkObjects(buildInfo);
    Libraries = {};
    for index = 1:numel(linkObjs)
        linkPath = codertarget.utils.replaceTokens(hCS,linkObjs(index).Path,targetInfo.Tokens);
        linkPath = regexprep(linkPath, sToks, sDirs,'ignorecase');
        [~, ~, ext] = fileparts(linkObjs(index).Name);
        if isempty(ext)
            libExtension = '.lib';
        else
            libExtension = '';
        end
        Libraries = [Libraries, fullfile(linkPath, [linkObjs(index).Name, libExtension])]; %#ok<AGROW>
    end
	
	%% Get the Library file for the referenced model
    if(strcmpi(get_param(projectName,'ModelReferenceTargetType'),'NONE'))
        if(~isempty(buildInfo.ModelRefs))
            for ctr = 1 : length(buildInfo.ModelRefs)
                mdlref_libname = buildInfo.ModelRefs(ctr).Name;
                mdlref_libpath = regexprep(buildInfo.ModelRefs(ctr).Path, sToks, sDirs,'ignorecase');
                [~, ~, ext] = fileparts(mdlref_libname);
                if isempty(ext)
                    libExtension = '.lib';
                else
                    libExtension = '';
                end
                Libraries = [Libraries fullfile(mdlref_libpath, [mdlref_libname, libExtension])]; %#ok<AGROW>
            end
        end
	end
	%% Get the list of Linker defines
    linker_defines = {};
    linker_flags = buildInfo.getLinkFlags;
    retainFlags = {};
    for index = 1:numel(linker_flags)
        if(~isempty(strfind(linker_flags{index},'-l')))
            % Split multiple entries for linker with space as delimiter
            libraryEntry = split(linker_flags{index},' ');
            for libIndx = 1 : numel(libraryEntry)
                % Search for the presence of multiple library inclusions
                if(~isempty(strfind(libraryEntry{libIndx},'-l')))
                    % remove -l flag before adding to the existing list of
                    % libraries
                    rtslib = split(libraryEntry{libIndx}, '-l');
                    Libraries = [Libraries,rtslib(2)]; %#ok<AGROW> % Not changing the code here to remove " " and full path : Changing inside the printProjectSpec.m to not break the old workflow
                end
            end
        end
        if(~isempty(strfind(linker_flags{index},'--define')))
            linker_defines = [linker_defines,linker_flags{index}]; %#ok<AGROW>
        end
        if(~isempty(strfind(linker_flags{index},'--retain')))
            retainCell = split(linker_flags{index},' ');
            for retainIndex = 1:numel(retainCell)
                if(~isempty(strfind(retainCell{retainIndex},'--retain')))
                    retainFlags = [retainFlags, retainCell{retainIndex}]; %#ok<AGROW>
                end
            end
        end
	end

	% Get the Build Configuration selected during Code generation
    build_config = get_param(projectName,'BuildConfiguration');

    % Get the value of Stack size
    stacksize = get_param(hCS,'MaxStackSize');

	%% Get the list of Compiler, Assembler and Linker flags based on the Build Configuration
    if(strcmpi(build_config,'Specify'))
        pattern1 = '\s+[-i"$(]+\w*+[)]+[/include]+["]';
        pattern2 = '\s+[-i"$(]+\w*+[)]+["]';
        pattern3 = '[$(]+\w*+[)]*';
        pattern4 = '[-I$(]+\w*+[)]';
        copts = hCS.get_param('CustomToolchainOptions');
        compiler_flags = [copts{2} copts{4}];
        compiler_flags = regexprep(compiler_flags,pattern1,'');
        compiler_flags = regexprep(compiler_flags,pattern2,'');
        compiler_flags = regexprep(compiler_flags,pattern3,'');
        compiler_flags = strrep(compiler_flags,'"','');
        compiler_flags = strrep(compiler_flags,'--compile_only','');
        cflags_skipforsil = buildInfo.getCompileFlags;
        compiler_flags = [compiler_flags ' ' strjoin(cflags_skipforsil(:))];
        linker_flags = copts{6};
        % If the Build Configuration is specify, Replace all Tokens with their corresponding value and remove all the other tokens
        linker_flags = strrep(linker_flags,'$(STACK_SIZE)',stacksize);
        linker_flags = strrep(linker_flags,'$(HEAP_SIZE)',getenv('HEAP_SIZE'));
        linker_flags = strrep(linker_flags,'$(PRODUCT_NAME)',projectName);
        linker_flags = regexprep(linker_flags,pattern4,'');
        linker_flags = regexprep(linker_flags,pattern1,'');
        linker_flags = regexprep(linker_flags,pattern2,'');
        linker_flags = regexprep(linker_flags,pattern3,'');
        linker_flags = [linker_flags ' ' strjoin(retainFlags(:))];
        linker_flags = strrep(linker_flags,'"','');
    else
        aflags = [];
        cflags = [];
        linker_flags = [];
        linkingflags = buildInfo.getLinkFlags;
        k = buildInfo.getBuildToolInfo('ToolchainInfo');
        flags = k.getBuildConfigurationOption(build_config,'Assembler');
        flags{end} = '';
        for index = 1:length(flags)
            if(~isempty(flags{index}))
                aflags = [aflags ' ' flags{index}]; %#ok<AGROW>
            end
        end
        flags = k.getBuildConfigurationOption(build_config,'C Compiler');
        flags{6} = '';
        flags{7} = '';
        flags{8} = '';
        for index = 1:length(flags)
            if(~isempty(flags{index}))
                cflags = [cflags ' ' flags{index}]; %#ok<AGROW>
            end
        end
        cflags_skipforsil = buildInfo.getCompileFlags;
        compiler_flags = [aflags cflags ' ' strjoin(cflags_skipforsil(:))];
        compiler_flags = strrep(compiler_flags,'"','');
        compiler_flags = strrep(compiler_flags,'--compile_only','');
        flags = k.getBuildConfigurationOption(build_config,'Linker');

        % Replace all Tokens with their corresponding value
        flags = strrep(flags, '$(STACK_SIZE)',stacksize);
        flags = strrep(flags, '$(HEAP_SIZE)',getenv('HEAP_SIZE'));
        flags = strrep(flags, '$(PRODUCT_NAME)',projectName);
        flags = strrep(flags, '-I$(TI_LIB)','');
        for index = 1:length(flags)
            if(~isempty(flags{index}))
                linker_flags = [linker_flags ' ' flags{index}]; %#ok<AGROW>
            end
        end
        linker_flags = [linker_flags ' ' strjoin(retainFlags(:))];
        linker_flags = strrep(linker_flags,'"','');
        linker_flags = [linker_flags ' ' linkingflags{1}];
	end

	   %% Check the compiler format being used
    compilerFormat = 'ELF';
    cProjectTemplateFile = 'CProjectTemplate.xml';
    % Remove the abi flags from linker as the abi flags from compiler
    % is added as part of linker automatically by linker
    if (contains(compiler_flags,'--abi')) && (contains(compiler_flags,'eabi'))
        compilerFormat = 'ELF';
        % Use new template for setting ELF specific options
        cProjectTemplateFile = 'CProjectTemplateCCSv9.xml';
        if (contains(linker_flags,'--abi')) && (contains(linker_flags,'eabi'))
            linker_flags = strrep(linker_flags,'--abi=eabi ','');
        end
    elseif (contains(linker_flags,'--abi')) && (contains(linker_flags,'coffabi'))
        linker_flags = strrep(linker_flags,'--abi=coffabi ','');
    end
    % Also remove the -z flag as CCS project automatically adds -z at
    % the beginning of linker flags
    linker_flags = strrep(linker_flags,'-z ','');

    %% Get the tool chain being used
    %[ccsToolVersion, ccsBaseName] = codertarget.tic2000.internal.CCSVersionDetect(hCS);

    %% Get code generation tool version
    verFile = 'README.*';
    verPattern = '(\d*\.(\d*\.)+\w*)';
    fileList = dir(fullfile(cgtPath, verFile));
    if ~isempty(fileList)
        cgtVersion = linkfoundation.util.getVersion(fullfile(cgtPath, fileList(1).name),verPattern);
    else
        cgtVersion = '3.2.2';
    end

    %% Get path to the target configuration file
  %  cmdEnd = codertarget.data.getParameterValue(hCS,'Runtime.LoadCommandArg');
    %configFile = codertarget.utils.replaceTokens(hCS,cmdEnd,targetInfo.Tokens);

	%% XML file creation
    % Extract the component name from buildInfo
    componentName = buildInfo.ComponentName;

    % Construct the file name using the component name
    projectSpecfileName = strcat(componentName, '.projectspec');

    % Determine boot source: 1 for Flash, 0 for RAM
   % bootFromFlash = codertarget.data.getParameterValue(hCS,'Runtime.FlashLoad');
    mainSrcDir = char(getSourcePaths(buildInfo, 1, 'BuildDir'));
    % Extract the Device ID from HardwareInfo
    %deviceID = codertarget.data.getParameterValue(hCS,'Runtime.DeviceID');

    targetHardwareInfo  = codertarget.targethardware.getTargetHardware(hCS);
    toolchain = buildInfo.getBuildToolInfo('ToolchainInfo');
    % Extract the chip family from the target data
    chipFamily = targetHardwareInfo .SubFamily;

	am263x.createProjectSpecFileAM261x(CGTInstallationDirectory, tokenName, TIDriverPath , projectName, cgtVersion, compilerFormat, linker_flags, compiler_flags, listOfFiles, includeFiles, compiler_defines, linker_defines, configFile, Libraries, projectSpecfileName, processingUnit, bootFromFlash, mainSrcDir, cgtPath, deviceID, targetHardwareInfo , toolchain);
     if ~isfolder('CCS_Project')
        mkdir('CCS_Project');
        movefile(projectSpecfileName, fullfile('CCS_Project', '/'), 'f');
    else
        movefile(projectSpecfileName, fullfile('CCS_Project', '/'), 'f');
	 end

	 % Create a folder within code-gen folder to use as the CCS Workspace
    if(~isfolder('CCS_Workspace'))
        mkdir('CCS_Workspace');
	end
	    %% Open the project if user clicks on the hyperlink in diagnostic viewer
    %Simulink.output.info(DAStudio.message('TIC2000:codegen:OpenCCSProject',CCS_InstallDir,project_folder,componentName));
% Command to import the project using CCS command-line tools
%importCommand = sprintf('"%s\\ccs\\eclipsec" -noSplash -data "%s" -application com.ti.ccstudio.apps.projectImport -ccs.projectspec "%s"', ...
%                        ccsPath, ccsPath, projectspecPath);

end