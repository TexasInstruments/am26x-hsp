classdef UART_READ_AM263PX < matlab.System & ...
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
        %UART Port
        UARTPort = 'UART0';
        %Output Data Length
        OutputDataLength = 1;
        %Output Data Type
        DataType = 'double';
        
    end
    
    properties (Access = private)
        % Pre-computed constants.
        BaseAddr = 0;
        InterruptNumber = 0;
        BytesReadOutput = 0;
       
    end

    properties(Constant,Hidden)
        UARTPortSet = matlab.system.StringSet({'UART0', 'UART1', 'UART2', 'UART3', 'UART4', 'UART5'}); 
        DataTypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'Unsigned int8', 'int16', 'Unsigned int16', 'int32', 'Unsigned int32', 'boolean'});
    end

    properties (Dependent, Access=protected)
    UARTPortEnum;
    DataTypeEnum;
    end

    properties (Access=private,Hidden,Nontunable)
        dataInputLength;
    end
    
    methods
        % Constructor
        function obj = UART_READ_AM263PX(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.DataTypeEnum(obj)
            if isequal(obj.DataType,'double')
                ret = uint16(0);
            elseif isequal(obj.DataType, 'single')
                ret = uint16(1);
            elseif isequal(obj.DataType, 'int8')
                ret = uint16(2);
            elseif isequal(obj.DataType, 'Unsigned int8')
                ret = uint16(3);
            elseif isequal(obj.DataType, 'int16')
                ret = uint16(4);
            elseif isequal(obj.DataType, 'Unsigned int16')
                ret = uint16(5);
            elseif isequal(obj.DataType, 'int32')
                ret = uint16(6);
            elseif isequal(obj.DataType, 'Unsigned int32')
                ret = uint16(7);
            elseif isequal(obj.DataType, 'boolean')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end


  
        function ret = get.UARTPortEnum(obj)
            if isequal(obj.UARTPort, 'UART0')
                ret = uint16(0);
            elseif isequal(obj.UARTPort, 'UART1')
                ret = uint16(1);
            elseif isequal(obj.UARTPort, 'UART2')
                ret = uint16(2);
            elseif isequal(obj.UARTPort, 'UART3')
                ret = uint16(3);
            elseif isequal(obj.UARTPort, 'UART4')
                ret = uint16(4);
            elseif isequal(obj.UARTPort, 'UART5')
                ret = uint16(5);
            else
                ret = uint16(0);
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

    methods (Static, Access = protected)

      function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','UART Read',...
                'Text', sprintf('%s\n%s','Configure the UART Read block to read data from UART console'),...
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
             'Title', ' General', ...
             'PropertyList',  {'UARTPort', 'OutputDataLength', 'DataType', 'SampleTime',...
             });
       
             groups = General;
            
      end
    end
    
    methods (Access=protected)
        function validateInputsImpl(obj)
            coder.extrinsic('error');
            coder.extrinsic('message');
            if( (obj.OutputDataLength  > 512))
                error(message('UartDataSizeTooHigh'));
            end
        end

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
                % coder.cinclude('sink.h');
                % coder.ceval('sink_init');
                
                coder.cinclude('MW_UART.h');
                switch uint16(obj.UARTPortEnum)
                    case 0
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART0_U_BASE');
                    case 1
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART1_U_BASE');
                    case 2
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART2_U_BASE');
                    case 3
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART3_U_BASE');
                    case 4
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART4_U_BASE');
                    case 5
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART5_U_BASE');
                    otherwise
                        obj.BaseAddr = coder.opaque('uint32_t', 'CSL_UART0_U_BASE');
                end

                switch uint16(obj.UARTPortEnum)
                    case 0
                        obj.InterruptNumber = 38;
                    case 1
                        obj.InterruptNumber = 39;
                    case 2
                        obj.InterruptNumber = 40;
                    case 3
                        obj.InterruptNumber = 41;
                    case 4
                        obj.InterruptNumber = 42;
                    case 5
                        obj.InterruptNumber = 43;
                    otherwise
                        obj.InterruptNumber = 38;
                end

                 coder.ceval('uart_read_setup', uint8(obj.UARTPortEnum));
           
            
            end
        end
        
        function [UARTDataOut] = stepImpl(obj)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
      
                y = uint8(zeros(1,obj.OutputDataLength));
                BytesToRead = uint16(0);

                switch(obj.DataTypeEnum)
                    case 0
                        BytesToRead = obj.OutputDataLength * 8;
                        UARTDataOut = double(zeros(1,obj.OutputDataLength));
                    case 1
                        BytesToRead = obj.OutputDataLength * 4;
                        UARTDataOut = single(zeros(1,obj.OutputDataLength));
                    case 2
                        BytesToRead = obj.OutputDataLength * 1;
                        UARTDataOut = int8(zeros(1,obj.OutputDataLength));
                    case 3
                        BytesToRead = obj.OutputDataLength * 1;
                        UARTDataOut = uint8(zeros(1,obj.OutputDataLength));
                    case 4
                        BytesToRead = obj.OutputDataLength * 2;
                        UARTDataOut = int16(zeros(1,obj.OutputDataLength));
                    case 5
                        BytesToRead = obj.OutputDataLength * 2;
                        UARTDataOut = uint16(zeros(1,obj.OutputDataLength));
                    case 6
                        BytesToRead = obj.OutputDataLength * 4;
                        UARTDataOut = int32(zeros(1,obj.OutputDataLength));
                    case 7
                        BytesToRead = obj.OutputDataLength * 4;
                        UARTDataOut = uint32(zeros(1,obj.OutputDataLength));
                    case 8
                        BytesToRead = obj.OutputDataLength * 1;
                        UARTDataOut = boolean(zeros(1,obj.OutputDataLength));
                    otherwise
                        BytesToRead = obj.OutputDataLength * 1;
                        UARTDataOut = uint8(zeros(1,obj.OutputDataLength));
                end

                obj.BytesReadOutput = BytesToRead;

                coder.ceval('uart_read_step',  coder.wref(UARTDataOut), uint16(obj.BytesReadOutput), uint16(obj.DataTypeEnum), uint16(obj.UARTPortEnum));
               
                % switch(obj.DataTypeEnum)
                %     case 0
                %         UARTDataOut = double(y);
                %     case 1
                %         UARTDataOut = single(y);
                %     case 2
                %         UARTDataOut = int8(y);
                %     case 3
                %         UARTDataOut = uint8(y);
                %     case 4
                %         UARTDataOut = int16(y);
                %     case 5
                %         UARTDataOut = uint16(y);
                %     case 6
                %         UARTDataOut = int32(y);
                %     case 7
                %         UARTDataOut = uint32(y);
                %     case 8
                %         UARTDataOut = boolean(y);
                %     otherwise
                %         UARTDataOut = uint8(y);
                % end

               
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                coder.ceval('uart_release');
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(~)
            num = 0;
        end

        function varargout = getOutputNamesImpl(~)
            varargout{1} = 'UARTData';
           
        end

        function OutputPorts = getNumOutputsImpl(~)
            OutputPorts = 1;
        end
        
        function varargout = getOutputDataTypeImpl(obj)
           if isequal(obj.DataType, 'double')
               varargout{1} = "double";
           elseif isequal(obj.DataType, 'single')
               varargout{1} = "single";
           elseif isequal(obj.DataType, 'int8')
               varargout{1} = "int8";
           elseif isequal(obj.DataType, 'Unsigned int8')
               varargout{1} = "uint8";
           elseif isequal(obj.DataType, 'int16')
               varargout{1} = "int16";
           elseif isequal(obj.DataType, 'Unsigned int16')
               varargout{1} = "uint16";
           elseif isequal(obj.DataType, 'int32')
               varargout{1} = "int32";
           elseif isequal(obj.DataType, 'Unsigned int32')
               varargout{1} = "uint32";
           elseif isequal(obj.DataType, 'boolean')
               varargout{1} = "logical";
           else
               varargout{1} = "uint16";
           end
       end
       
        function varargout = isOutputFixedSizeImpl(~,~)
            varargout{1} = true;
        end
        
        
        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
        end
        
        function varargout = getOutputSizeImpl(obj)
            varargout{1} = [1, obj.OutputDataLength];
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'UART_READ'};
        end    
    end
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'UART_READ';
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
                addIncludeFiles(buildInfo, 'MW_UART.h', includeDir);
                addSourceFiles(buildInfo, 'MW_UART.c', srcDir);
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
