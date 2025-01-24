classdef MCAN_READ < matlab.System & ...
        coder.ExternalDependency
    %
    % System object template for a source block.
    % 
    % This template includes most, but not all, possible properties,
    % attributes, and methods that you can implement for a System object in
    % Simulink.
    %
    % NOTE: When renaming the class name Source, the file name and
    % constructor name must be updated to use the class name.
    %
    
    
    %#codegen
    %#ok<*EMCA>
    
    properties
        % Public, tunable properties.
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

        %Sample Time
        SampleTime = 0.1;
        %MCAN Instance
        MCAN_Instance = 'MCAN0';
        %Read source
        Read_src = 'Buffer';
        %Buffer number
        Buffer_num = 0;
        %Enable blocking mode
        Enable_blockingrx (1,1) logical = false;
        %Enable Output Port
        Enable_OutputPort (1,1) logical = false;
    end

    properties (Constant, Hidden)
        MCAN_InstanceSet = matlab.system.StringSet({'MCAN0', 'MCAN1', 'MCAN2', 'MCAN3', 'MCAN4', 'MCAN5', 'MCAN6', 'MCAN7'});
        Read_srcSet = matlab.system.StringSet({'Buffer', 'FIFO0', 'FIF01'});
       
    end
    
    properties (Dependent, Access=protected)
        MCAN_InstanceEnum;
        Read_srcEnum;
        
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
        datalen = 0;
    end
    
    methods
        % Constructor
        function obj = MCAN_READ(varargin)
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
        
        function ret = get.Read_srcEnum(obj)
            if(isequal(obj.Read_src, 'Buffer'))
                ret = uint32(0);
            elseif(isequal(obj.Read_src, 'FIFO0'))
                ret = uint32(1);
            elseif(isequal(obj.Read_src, 'FIFO1'))
                ret = uint32(2);
            else
                ret = uint32(0);
            end
        end

        function set.SampleTime(obj,newTime)
            % Sample time must be a real scalar value or 2 element array.
            if isreal(newTime) && ...
                    (all(all(isfinite(newTime))) || all(all(isinf(newTime)))) && ... %need to work all dimensions to scalar logical
                    (numel(newTime) == 1 || numel(newTime) == 2)
                sampleTime = real(newTime);
            else
                error('source:build:InvalidSampleTimeNeedScalar',...
                    'Invalid sample time. Sample time must be a real scalar value or an array of two real values.');
            end
            if sampleTime(1) < 0.0 && sampleTime(1) ~= -1.0
                error('source:build:InvalidSampleTimeNeedPositive',...
                    'Invalid sample time. Sample time must be non-negative or -1 (for inherited).');
            end
            if numel(sampleTime) == 2
                if sampleTime(1) > 0.0 && sampleTime(2) >= sampleTime(1)
                    error('source:build:InvalidSampleTimeNeedSmallerOffset',...
                        'Invalid sample time. Offset must be smaller than period.');
                end
                if sampleTime(1) == -1.0 && sampleTime(2) ~= 0.0
                    error('source:build:InvalidSampleTimeNeedZeroOffset',...
                        'Invalid sample time. When period is -1, offset must be 0.');
                end
                if sampleTime(1) == 0.0 && sampleTime(2) ~= 1.0
                    error('source:build:InvalidSampleTimeNeedOffsetOne',...
                        'Invalid sample time. When period is 0, offset must be 1.');
                end
            end
            obj.SampleTime = sampleTime;
        end
        


    end
    
    methods (Access=protected)

        function st = getSampleTimeImpl(obj)
            if isequal(obj.SampleTime, -1) || isequal(obj.SampleTime, [-1, 0])
                st = matlab.system.SampleTimeSpecification('Type', 'Inherited');
            elseif isequal(obj.SampleTime, [0, 1])
                st = matlab.system.SampleTimeSpecification('Type', 'Fixed In Minor Step');
            else
                if numel(obj.SampleTime) == 1
                    sampleTime = obj.SampleTime;
                    offset = 0;
                else
                    sampleTime = obj.SampleTime(1);
                    offset = obj.SampleTime(2);
                end
                st = matlab.system.SampleTimeSpecification('Type', 'Discrete', ...
                    'SampleTime', sampleTime, 'OffsetTime', offset);
            end
        end
        
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                % coder.cinclude('source.h');
                % coder.ceval('source_init');
                coder.cinclude('MW_CANREAD.h');
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
        
        function [varargout] = stepImpl(obj)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                x = uint32(0);
                a = uint8(zeros(64,1));
                can_read_step_struct = struct('base', obj.baseaddr, 'instance', uint32(obj.MCAN_InstanceEnum), 'Enable_blockingrx', uint32(obj.Enable_blockingrx), 'rxSource', uint32(obj.Read_srcEnum), 'buffernum', uint32(obj.Buffer_num));
                coder.cstructname(can_read_step_struct, 'CANREAD_step_struct', 'extern', 'HeaderFile', 'MW_CANREAD.h');
                coder.ceval('CANRead_Step', coder.ref(can_read_step_struct), coder.wref(x), coder.wref(a));
                obj.datalen = x;
                y = uint8(a(1:uint8(obj.datalen)));
                indx = 1;
                if(obj.Enable_OutputPort)
                    varargout{indx} = y;
                end
         
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                coder.ceval('CANREAD_release', obj.baseaddr);
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(~)
            num = 0;
        end

         function num = getNumOutputsImpl(obj)
             num = 0;
             if(obj.Enable_OutputPort)
                num = 1;
             end
        end
        
        function varargout = isOutputFixedSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                if(obj.Enable_OutputPort)
                    varargout{indx} = false;
                end
            end

        end


        function varargout = isOutputComplexImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                if(obj.Enable_OutputPort)
                    varargout{indx} = false;
                end
            end

        end


        function varargout = getOutputSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                if(obj.Enable_OutputPort)
                    varargout{indx} = [64,1];
                end
            end

        end



        function varargout = getOutputDataTypeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                if(obj.Enable_OutputPort)
                    varargout{indx} = 'uint8';
                end
            end
        end

        function varargout = getOutputNamesImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                if(obj.Enable_OutputPort)
                    varargout{indx} = 'Msg';
                end
            end
     end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'MCAN_READ'};
        end    
    end
    
    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','MCAN_READ',...
                'Text', sprintf('%s\n%s','Configure the CAN Read block'),...
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
             "PropertyList", {'MCAN_Instance', 'Enable_blockingrx', 'Read_src', 'Buffer_num', 'Enable_OutputPort', 'SampleTime'});

           
            groups = [General];
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'MCAN_READ';
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
                addIncludeFiles(buildInfo, 'MW_CANREAD.h', includeDir);
                addSourceFiles(buildInfo, 'MW_CANREAD.c', srcDir);
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
