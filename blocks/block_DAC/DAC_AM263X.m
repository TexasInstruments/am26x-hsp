
classdef DAC_AM263X < matlab.System & coder.ExternalDependency
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
        
        % DAC Instance
        Module = 'DAC0';     
        %Reference Voltage
        Reference_Voltage = 'VREF reference voltage (External)';
        %Load Mode
        Load_Mode = 'Load on next SYSCLK';
        % EPWMSYNCPER Signal
        EPWMSYNCPER_Signal = 'EPWM0SYNCPER';
        % Enable Output
        Enable_Output(1, 1) logical = false;
        % Lock Control Register
        Control_Register (1,1) logical= false;
        %Lock Shadow Register
        Shadow_Register (1,1) logical =false;
        % Lock Output Register
        Output_Register (1,1) logical= false;
        % Initial Shadow Value
        Shadow_Value=0;
        
    end
    
    properties (Access = private)
        % Pre-computed constants.
        
        baseAddr=0;
        
    end

    properties (Constant, Hidden)
       ModuleSet = matlab.system.StringSet({'DAC0'});
       Reference_VoltageSet = matlab.system.StringSet({'VREF reference voltage (External)', 'VDDA reference voltage (Internal)'});
       Load_ModeSet = matlab.system.StringSet({'Load on next SYSCLK', 'Load on next PWMSYNC specified by SYNCSEL'});
       EPWMSYNCPER_SignalSet = matlab.system.StringSet({'EPWM0SYNCPER', 'EPWM1SYNCPER', 'EPWM2SYNCPER', 'EPWM3SYNCPER', 'EPWM4SYNCPER', 'EPWM5SYNCPER', 'EPWM6SYNCPER', 'EPWM7SYNCPER', 'EPWM8SYNCPER', 'EPWM9SYNCPER', 'EPWM10SYNCPER', 'EPWM11SYNCPER', 'EPWM12SYNCPER', 'EPWM13SYNCPER', 'EPWM14SYNCPER', 'EPWM15SYNCPER', 'EPWM16SYNCPER', 'EPWM17SYNCPER', 'EPWM18SYNCPER', 'EPWM19SYNCPER', 'EPWM20SYNCPER', 'EPWM21SYNCPER', 'EPWM22SYNCPER', 'EPWM23SYNCPER', 'EPWM24SYNCPER', 'EPWM25SYNCPER', 'EPWM26SYNCPER', 'EPWM27SYNCPER', 'EPWM28SYNCPER', 'EPWM29SYNCPER', 'EPWM30SYNCPER', 'EPWM31SYNCPER'});

    end

    properties (Dependent, Access=protected)
        ModuleEnum;
        Reference_VoltageEnum;
        Load_ModeEnum;
        EPWMSYNCPER_SignalEnum;
    end
    
    methods
        % Constructor
        function obj = DAC_AM263X(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.ModuleEnum(obj)
            if isequal(obj.Module, 'DAC0')
                ret = 0;
            else
                ret = 1;
            end
        end

        function ret = get.Reference_VoltageEnum(obj)
            if isequal(obj.Reference_Voltage, 'VREF reference voltage (External)')
                ret = 0;
            elseif isequal(obj.Reference_Voltage, 'VDDA reference voltage (Internal)')
                ret = 1;
            else
                ret = 0;
            end
        end

        function ret = get.Load_ModeEnum(obj)
            if isequal(obj.Load_Mode, 'Load on next sysclk')
                ret = 0;
            elseif isequal(obj.Load_Mode, 'Load on next PWMSYNC specified by SYNCSEL')
                ret = 1;
            else
                ret = 0;
            end
        end

        function ret = get.EPWMSYNCPER_SignalEnum(obj)
            if isequal(obj.EPWMSYNCPER_Signal, 'EPWM0SYNCPER')
                ret = 1;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM1SYNCPER')
                ret = 2;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM2SYNCPER')
                ret = 3;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM3SYNCPER')
                ret = 4;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM4SYNCPER')
                ret = 5;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM5SYNCPER')
                ret = 6;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM6SYNCPER')
                ret = 7;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM7SYNCPER')
                ret = 8;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM8SYNCPER')
                ret = 9;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM9SYNCPER')
                ret = 10;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM10SYNCPER')
                ret = 11;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM11SYNCPER')
                ret = 12;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM12SYNCPER')
                ret = 13;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM13SYNCPER')
                ret = 14;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM14SYNCPER')
                ret = 15;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM15SYNCPER')
                ret = 16;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM16SYNCPER')
                ret = 17;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM17SYNCPER')
                ret = 18;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM18SYNCPER')
                ret = 19;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM19SYNCPER')
                ret = 20;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM20SYNCPER')
                ret = 21;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM21SYNCPER')
                ret = 22;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM22SYNCPER')
                ret = 23;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM23SYNCPER')
                ret = 24;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM24SYNCPER')
                ret = 25;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM25SYNCPER')
                ret = 26;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM26SYNCPER')
                ret = 27;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM27SYNCPER')
                ret = 28;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM28SYNCPER')
                ret = 29;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM29SYNCPER')
                ret = 30;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM30SYNCPER')
                ret = 31;
            elseif isequal(obj.EPWMSYNCPER_Signal, 'EPWM31SYNCPER')
                ret = 32;
            else
                ret = 0;
            end
        end



    end

    methods (Static, Access=protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
       
        function groups = getPropertyGroupsImpl
            General = matlab.system.display.SectionGroup(...
             'Title', ' General', ...
             'PropertyList',  {'Reference_Voltage', 'Load_Mode', 'EPWMSYNCPER_Signal', 'Shadow_Value', 'Enable_Output', 'Control_Register', 'Shadow_Register', 'Output_Register'});

            groups = [General];

        end
        
        function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','Type 1.2 DAC',...
                'Text', sprintf('%s\n\n%s','DAC Block for converting Digital value to Analog'),...
                'ShowSourceLink', false);
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
                coder.cinclude('MW_DAC.h');
                obj.baseAddr= coder.opaque('uint32_t', 'CSL_CONTROLSS_DAC0_U_BASE ');
                dac_setup_struct = struct('base', obj.baseAddr, 'DACRef', uint16(obj.Reference_VoltageEnum), 'LoadMode', uint16(obj.Load_ModeEnum), 'PWMSyncSignal', uint16(obj.EPWMSYNCPER_SignalEnum), 'Enable_output', obj.Enable_Output, 'ShdwValue', uint16(obj.Shadow_Value), 'DACLockControl', obj.Control_Register, 'DACLockOutput', obj.Output_Register, 'DACLockShadow', obj.Shadow_Register);
                coder.cstructname(dac_setup_struct, 'DACSetupStruct', 'extern', 'HeaderFile', 'MW_DAC.h');
                coder.ceval('DAC_setup', coder.ref(dac_setup_struct));
            end
        end
        
        function stepImpl(obj,u)  %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);
               if ((u >= 0) && (u <= 4095))
                    coder.ceval('DAC_setShadowValue',obj.baseAddr, u)
                elseif u >=4096
                    coder.ceval('DAC_setShadowValue',obj.baseAddr, 0) 
                end
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');
                coder.ceval('DAC_release', obj.baseAddr);
            end
        end
        
        function flag = isInactivePropertyImpl(obj,propertyName)
            if (strcmp(propertyName, 'EPWMSYNCPER_Signal') && isequal(obj.Load_Mode, 'Load on next SYSCLK'))
                flag = true;
            else
                flag = false;
            end
        
        end
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 1;
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end

        function varargout = getInputNamesImpl(obj)
            varargout = cell(1,1);
            index = 1;
            varargout{index} = 'DIN';
        end
            
        
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = false;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end
        
        % function validateInputsImpl(~, u)
        %     if isempty(coder.target)
        %         % Run input validation only in Simulation
        %         validateattributes(u,{'double'},{'scalar'},'','u');
        %     end
        % end

        function validatePropertiesImpl(obj)
            if (obj.Shadow_Value>4095)|| (obj.Shadow_Value<0) 
              error("Value should lie in the range of 0 to 4095");
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'DAC', 'Type 1P2'};
        end
    end
    
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'DAC';
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
                addIncludeFiles(buildInfo, 'MW_DAC.h', includeDir);
                addSourceFiles(buildInfo, 'MW_DAC.c', srcDir);
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

