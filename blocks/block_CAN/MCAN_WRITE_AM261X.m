
classdef MCAN_WRITE_AM261X < matlab.System & ...
                     coder.ExternalDependency
    %
    % System object template for a sink block.
    % 
    % This template includes most, but not all, possible properties,
    % attributes, and methods that you can implement for a System object in
    % Simulink.
    %
    % NOTE: When renaming the class name Sink, the file name and
    % constructor name must be updated to use the class name.
    %
    
    
    %#codegen
    %#ok<*EMCA>
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

        %MCAN Instance
        MCAN_Instance = 'MCAN0';
        %Transmit message ID
        TX_MsgID = 0x0;
        %Length(bytes)
        Input_length = 0;
        %Enable blocking mode
        Blocking_mode_tx (1,1) logical = false;

    end

    properties (Constant, Hidden)
        MCAN_InstanceSet = matlab.system.StringSet({'MCAN0', 'MCAN1'});
    end
    
    properties (Dependent, Access=protected)
        MCAN_InstanceEnum;
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
    end
    
    methods
        % Constructor
        function obj = MCAN_WRITE_AM261X(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.MCAN_InstanceEnum(obj)
            if(isequal(obj.MCAN_Instance, 'MCAN0'))
                ret = uint32(0);
            elseif(isequal(obj.MCAN_Instance, 'MCAN1'))
                ret = uint32(1);
            elseif(isequal(obj.MCAN_Instance, 'MCAN2'))
                ret = uint32(2);
            elseif(isequal(obj.MCAN_Instance, 'MCAN3'))
                ret = uint32(3);
            elseif(isequal(obj.MCAN_Instance, 'MCAN4'))
                ret = uint32(4);
            elseif(isequal(obj.MCAN_Instance, 'MCAN5'))
                ret = uint32(5);
            elseif(isequal(obj.MCAN_Instance, 'MCAN6'))
                ret = uint32(6);
            elseif(isequal(obj.MCAN_Instance, 'MCAN7'))
                ret = uint32(7);
            else
                ret = uint32(0);
            end
        end

        
    end

    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','MCAN_WRITE',...
                'Text', sprintf('%s\n%s','Configure the CAN Write block'),...
                'ShowSourceLink', false);
        end
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end

        function groups = getPropertyGroupsImpl

          
            General = matlab.system.display.SectionGroup(...
             'Title', 'General', ...
             "PropertyList", {'MCAN_Instance','TX_MsgID', 'Blocking_mode_tx'});

            
            groups = General;

            
        end  
    end
    
    methods (Access=protected)
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                % coder.cinclude('sink.h');
                % coder.ceval('sink_init');
                coder.cinclude('MW_CAN.h');
                switch uint16(obj.MCAN_InstanceEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN0_MSG_RAM_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN1_MSG_RAM_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN2_MSG_RAM_U_BASE');
                    case 3
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN3_MSG_RAM_U_BASE');
                    case 4
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN4_MSG_RAM_U_BASE');
                    case 5
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN5_MSG_RAM_U_BASE');
                    case 6
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN6_MSG_RAM_U_BASE');
                    case 7
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN7_MSG_RAM_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCAN0_MSG_RAM_U_BASE');
                end

            end
        end
        
        function stepImpl(obj, u)  %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);
                InputLen = numel(u);
                coder.ceval('CANWRITE_step', obj.baseaddr, uint32(obj.MCAN_InstanceEnum), uint32(obj.TX_MsgID), uint32(InputLen), coder.rref(u), obj.Blocking_mode_tx);
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                coder.ceval('CANWRITE_release', obj.baseaddr);
            end
        end

        
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 1;
        end

        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
            varargout = cell(1,num);
            varargout{index} = 'DIN';
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = false;
        end

        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end

        function validateInputsImpl(~, u)
            if isempty(coder.target)
                % Run input validation only in Simulation
                
            end
        end
        
        
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'MCAN_WRITE'};
        end
    end
    
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'MCAN_WRITE';
        end
        
        function b = isSupportedContext(context)
            b = context.isCodeGenTarget('rtw');
        end
        
        function updateBuildInfo(buildInfo, context)
            if context.isCodeGenTarget('rtw')
                % Update buildInfo
                srcDir = fullfile(fileparts(mfilename('fullpath')),'src'); %#ok<NASGU>
                includeDir = fullfile(fileparts(mfilename('fullpath')),'include');
                addIncludePaths(buildInfo,includeDir);
                addIncludeFiles(buildInfo, 'MW_CAN.h', includeDir);
                addSourceFiles(buildInfo, 'MW_CAN.c', srcDir);
                % Use the following API's to add include files, sources and
                % linker flags
                %addIncludeFiles(buildInfo,'source.h',includeDir);
                %addSourceFiles(buildInfo,'source.c',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end

