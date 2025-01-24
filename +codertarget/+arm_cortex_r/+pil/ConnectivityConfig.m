classdef ConnectivityConfig < rtw.connectivity.Config
%CONNECTIVITYCONFIG is an example target connectivity configuration class
    
    
    methods
        function this = ConnectivityConfig(componentArgs)
            
            % A target application framework specifies additional source files and libraries
            % required for building the executable
            componentArgs.CoderAssumptionsEnabled = false;
            targetApplicationFramework = ...
                codertarget.arm_cortex_r.pil.TargetApplicationFramework(componentArgs);
            
            % Filename extension for executable on the target system (e.g.
            % '.exe' for Windows or '' for Unix
            if ispc
                exeExtension = '.out';
            else
                exeExtension = '';
            end
            
            % Create an instance of MakefileBuilder; this works in conjunction with your
            % template makefile to build the PIL executable
            builder = rtw.connectivity.MakefileBuilder(componentArgs, ...
                targetApplicationFramework, ...
                exeExtension);
            
            % Launcher
            launcher = codertarget.arm_cortex_r.pil.Launcher(componentArgs, builder);
            
            % File extension for shared libraries (e.g. .dll on Windows)
            [~, ~, sharedLibExt] = coder.BuildConfig.getStdLibInfo;

            % Evaluate name of the rtIOStream shared library
            rtiostreamLib = ['libmwrtiostreamserial' sharedLibExt];
            
            hostCommunicator = rtw.connectivity.RtIOStreamHostCommunicator(...
                componentArgs, ...
                launcher, ...
                rtiostreamLib);
            
            % For some targets it may be necessary to set a timeout value
            % for initial setup of the communications channel. For example,
            % the target processor may take a few seconds before it is
            % ready to open its side of the communications channel. If a
            % non-zero timeout value is set then the communicator will
            % repeatedly try to open the communications channel until the
            % timeout value is reached.
            hostCommunicator.setInitCommsTimeout(20); 
            
            % Configure a timeout period for reading of data by the host 
            % from the target. If no data is received with the specified 
            % period an error will be thrown.
            timeoutReadDataSecs = 60;
            hostCommunicator.setTimeoutRecvSecs(timeoutReadDataSecs);

             
            % Specify additional arguments when starting the           
            % executable (this configures the target-side of the       
            % communications channel)    
            %COMPort = 'COM67';
            COMPort = getpref('AM26xPILpref', 'COMport');
            BaudRate = getpref('AM26xPILpref', 'Baud');
            %launcher.setArgString(['-port ' COMPort ' -blocking 1']);

            % Custom arguments that will be passed to the              
            % rtIOStreamOpen function in the rtIOStream shared        
            % library (this configures the host-side of the           
            % communications channel) 

            %COMPort = 'COM67';
            %BaudRate = 115200;
            rtIOStreamOpenArgs = {...
                '-baud', num2str(BaudRate), ...
                '-port', COMPort, ...
                };                                                  
                      
            hostCommunicator.setOpenRtIOStreamArgList(...          
                rtIOStreamOpenArgs); 

            
            % call super class constructor to register components
            this@rtw.connectivity.Config(componentArgs,...
                                         builder,...
                                         launcher,...
                                         hostCommunicator);
            
            % Optionally, you can register a hardware-specific timer. Registering a timer
            % enables the code execution profiling feature. In this example
            % implementation, we use a timer for the host platform.
            timer = coder.profile.crlHostTimer();
            this.setTimer(timer);
            
        end
    end
end

