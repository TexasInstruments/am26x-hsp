
classdef MCSPI_READ_AM263PX < matlab.System & ...
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

        %MCSPI Instance
        MCSPI_Instance = 'MCSPI0';
        %Enable Channel 0
        Enable_Channel0 (1,1) logical = false;
        %Enable Channel 1
        Enable_Channel1 (1,1) logical = false;
        %Channel 0 Data Size(bits)
        Channel0_Data_Size = 8;
        %Channel 0 Enable automatic deassert of CS
        Channel0_Auto_CSDeassert (1,1) logical = true;
        %Channel 1 Data Size(bits)
        Channel1_Data_Size = 8;
        %Channel 1 Enable automatic deassert of CS
        Channel1_Auto_CSDeassert (1,1) logical = true;
        %Output data length(uint32)
        Output_Data_Length = 8;
    end

    properties (Constant, Hidden)
        MCSPI_InstanceSet = matlab.system.StringSet({'MCSPI0', 'MCSPI1', 'MCSPI2', 'MCSPI3', 'MCSPI4', 'MCSPI5', 'MCSPI6', 'MCSPI7'});
    end
    
    properties (Dependent, Access=protected)
        MCSPI_InstanceEnum;
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
    end
    
    methods
        % Constructor
        function obj = MCSPI_READ_AM263PX(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.MCSPI_InstanceEnum(obj)
            if(isequal(obj.MCSPI_Instance, 'MCSPI0'))
                ret = uint32(0);
            elseif(isequal(obj.MCSPI_Instance, 'MCSPI1'))
                ret = uint32(1);
            elseif(isequal(obj.MCSPI_Instance, 'MCSPI2'))
                ret = uint32(2);
            elseif(isequal(obj.MCSPI_Instance, 'MCSPI3'))
                ret = uint32(3);
            elseif(isequall(obj.MCSPI_Instance, 'MCSPI4'))
                ret = uint32(4);
            else
                ret = uint32(0);
            end
        end

        
    end

    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','MCSPI_READ',...
                'Text', sprintf('%s\n%s','Configure the MCSPI READ block'),...
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
             "PropertyList", {'MCSPI_Instance', 'Enable_Channel0', 'Enable_Channel1', 'Output_Data_Length'});

            Channel0_Config = matlab.system.display.SectionGroup(...
                'Title', 'Channel 0 Configurations', ...
                "PropertyList", {'Channel0_Data_Size', 'Channel0_Auto_CSDeassert'});
           
            Channel1_Config = matlab.system.display.SectionGroup(...
                'Title', 'Channel 1 Configurations', ...
                "PropertyList", {'Channel1_Data_Size', 'Channel1_Auto_CSDeassert'});

            groups = [General, Channel0_Config, Channel1_Config];

            
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
                coder.cinclude('MW_MCSPIREAD.h');
                switch uint32(obj.MCSPI_InstanceEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI0_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI1_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI2_U_BASE');
                    case 3
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI3_U_BASE');
                    case 4
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI4_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_MCSPI0_U_BASE');
                end

            end
        end
        
        function msg = stepImpl(obj)  %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
               
                x = uint16(zeros(1024,1));
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);
                mcspi_read_step_struct = struct('base', obj.baseaddr, 'mcspi_instance', obj.MCSPI_InstanceEnum, 'Enable_CH0', obj.Enable_Channel0, 'Enable_CH1', obj.Enable_Channel1, 'CH0_DataSize', obj.Channel0_Data_Size, 'CH1_DataSize', obj.Channel1_Data_Size, 'CH0_CSDisable', obj.Channel0_Auto_CSDeassert, 'CH1_CSDisable', obj.Channel1_Auto_CSDeassert);
                coder.cstructname(mcspi_read_step_struct, 'MCSPIReadStepStruct', 'extern', 'HeaderFile', 'MW_MCSPIREAD.h');
                coder.ceval('MCSPIREAD_step', coder.ref(mcspi_read_step_struct), coder.wref(x), uint32(obj.Output_Data_Length));
                
                msg = uint16(x(1:obj.Output_Data_Length));
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                coder.ceval('MCSPIREAD_release');
            end
        end

        function flag = isInactivePropertyImpl(obj,propertyName)
        
            if ((strcmp(propertyName, 'Channel0_Auto_CSDeassert') | strcmp(propertyName, 'Channel0_Data_Size')) && ~(obj.Enable_Channel0))
                flag = true;
            elseif ((strcmp(propertyName, 'Channel1_Auto_CSDeassert') | strcmp(propertyName, 'Channel1_Data_Size')) && ~(obj.Enable_Channel1))
                flag = true;
            else
                flag = false;
            end

         end

        
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 0;
        end

      
        
        function num = getNumOutputsImpl(~)
            num = 1;
        end
        
        function varargout = isOutputFixedSizeImpl(~,~)
            varargout{1} = true;
        end
        
        
        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
        end
        
        function varargout = getOutputSizeImpl(obj)
            varargout{1} = [obj.Output_Data_Length,1];
        end

        function varargout = getOutputNamesImpl(obj)
            varargout{1} = 'Msg';
        end

        function varargout = getOutputDataTypeImpl(obj)
            varargout{1} = "uint16";
        end

        

        function validateInputsImpl(~, u)
            if isempty(coder.target)
                % Run input validation only in Simulation
                
            end
        end
        
        
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'MCSPI_READ'};
        end
    end
    
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'MCSPI_READ';
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
                addIncludeFiles(buildInfo, 'MW_MCSPIREAD.h', includeDir);
                addSourceFiles(buildInfo, 'MW_MCSPIREAD.c', srcDir);
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

