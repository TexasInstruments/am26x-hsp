classdef TargetApplicationFramework < rtw.pil.RtIOStreamApplicationFramework
%TARGETAPPLICATIONFRAMEWORK is an example target connectivity configuration class
    
    
    methods
        function this = TargetApplicationFramework(componentArgs)
            narginchk(1, 1);
            % call super class constructor
            this@rtw.pil.RtIOStreamApplicationFramework(componentArgs);
            
            % To build the PIL application you must specify a main.c file.       
            % The following PIL main.c files are provided and can be             
            % added to the application framework via the "addPILMain"            
            % method:                                                            
            %                                                                    
            % 1) A main.c adapted for on-target PIL and suitable                 
            %    for most PIL implementations. Select by specifying              
            %    'target' argument to "addPILMain" method.                       
            %                                                                    
            % 2) A main.c adapted for host-based PIL such as the                 
            %    "mypil" host example. Select by specifying 'host'               
            %    argument to "addPILMain" method.                                
            this.addPILMain('target');                                             
            
            % Additional source and library files to include in the build        
            % must be added to the BuildInfo property                            
            
            % Get the BuildInfo object to update                                 
            buildInfo = this.getBuildInfo;                                       
            
            % Add device driver files to implement the target-side of the        
            % host-target rtIOStream communications channel   

            rootDir = codertarget.arm_cortex_r.internal.getRootDir;
            srcpath = fullfile(rootDir, 'src');
            buildInfo.addSourcePaths(srcpath);

            TargetHardware = getpref('AM26xPILpref', 'PILHardware');

            if(isequal(TargetHardware, 'AM263X'))
                buildInfo.addIncludeFiles({'rtiostream.h', 'am263x_main.h'}, fullfile(rootDir, 'include'));
                buildInfo.addSourceFiles({'rtiostream_serial_am263.c', 'am263x_init.c'}, fullfile(rootDir, 'src'));
            elseif(isequal(TargetHardware, 'AM263PX'))
                buildInfo.addIncludeFiles({'rtiostream.h', 'am263px_main.h'}, fullfile(rootDir, 'include'));
                buildInfo.addSourceFiles({'rtiostream_serial_am263p.c', 'am263px_init.c'}, fullfile(rootDir, 'src'));
            elseif(isequal(TargetHardware, 'AM261X'))
                buildInfo.addIncludeFiles({'rtiostream.h', 'am261x_main.h'}, fullfile(rootDir, 'include'));
                buildInfo.addSourceFiles({'rtiostream_serial_am261.c', 'am261x_init.c'}, fullfile(rootDir, 'src'));
            end
            % toolfileDir = fullfile(rootDir, 'toolchain', '+matlabshared', '+toolchain', '+ti_arm_clang');
            % load(fullfile(toolfileDir, 'tool_install_paths.mat'), "tool_install_path");
            % AM263SDKPath = strrep(string(tool_install_path(3)), '\','/');
            % AM263SrcPath = fullfile(AM263SDKPath, 'source');
            % buildInfo.addIncludePaths(AM263SrcPath);
            % buildInfo.addIncludeFiles('uart.h', fullfile(AM263SDKPath, 'source', 'drivers'));
            % buildInfo.addIncludeFiles('uart_lld.h', fullfile(AM263SDKPath, 'source', 'drivers', 'uart', 'v0', 'lld'));
            % buildInfo.addSourceFiles('uart_v0_lld.c', fullfile(AM263SDKPath, 'source', 'drivers', 'uart', 'v0', 'lld'));
            % buildInfo.addSourceFiles('HwiP_armv7r_vim.c', fullfile(AM263SDKPath, 'source', 'kernel', 'nortos', 'dpl', 'r5'));

        end
    end
end
