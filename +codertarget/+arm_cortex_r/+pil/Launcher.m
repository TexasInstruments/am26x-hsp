classdef Launcher < rtw.connectivity.Launcher
%LAUNCHER is an example target connectivity configuration class


    properties
        % For the host-based example, additional arguments may be provided when the
        % executable is launched as a separate process on the host. For example it may
        % be required to specify a TCP/IP port number.
        ArgString= '';

        % For the host-based example, it is necessary to
        % keep track of the process ID of the executable
        % so that this process can be killed when no longer
        % required
        ExePid = '';

        % For the host-based example, it is necessary to keep track of a temporary file
        % created by the process launcher so that it can be deleted when the
        % process is terminated
        TempFile = '';
    end

    methods
        % constructor
        function this = Launcher(componentArgs, builder)
            narginchk(2, 2);
            % call super class constructor
            this@rtw.connectivity.Launcher(componentArgs, builder);
        end
        
        % destructor
        function delete(this) %#ok
            
            % This method is called when an instance of this class is cleared from memory,
            % e.g. when the associated Simulink model is closed. You can use
            % this destructor method to close down any processes, e.g. an IDE or
            % debugger that was originally started by this class. If the
            % stopApplication method already performs this housekeeping at the
            % end of each on-target simulation run then it is not necessary to
            % insert any code in this destructor method. However, if the IDE or
            % debugger may be left open between successive on-target simulation
            % runs then it is recommended to insert code here to terminate that
            % application.

        end

        function setArgString(this, argString)                 
            % Specify command line arguments; for example, you may need to provide a TCP/IP
            % port number to override the default port number. If your Launcher
            % does not require any dynamic parameter configuration then this
            % method may not be required.
            disp('EXECUTING METHOD SETARGSTRING')              
            stack = dbstack;                                   
            disp(['SETARGSTRING called from line '...          
                  int2str(stack(2).line) ' of ' ...            
                  stack(2).file ])                             
                                                               
            this.ArgString = argString;                        
        end                                                    
        
        % Start the application
        function startApplication(this)
            % get name of the executable file
            out = this.getBuilder.getApplicationExecutable;

            % launch               
            TargetHardware = getpref('AM26xPILpref', 'PILHardware');                                  
            disp('Starting PIL simulation')    
            manualLoad = false;
            if ~manualLoad   
                if(isequal(TargetHardware, 'AM263X'))            
                    matlabshared.target.am26x.am263x.loadAndRun('',out);
                elseif(isequal(TargetHardware, 'AM263PX'))
                    matlabshared.target.am26x.am263px.loadAndRun('',out);
                elseif(isequal(TargetHardware, 'AM261X'))
                    matlabshared.target.am26x.am261x.loadAndRun('',out);
                end
            else
                manualLoad = false;
            end

            % Pause to ensure that server-side of TCP/IP connection  
            % is established and ready to accept a client connection 
            pause(0.4);   
        end
                            
        % Stop the application
        function stopApplication(this)
            
            disp('Stopping PIL simulation')                                  
                                              
        end
    end
end
