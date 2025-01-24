classdef UART_WRITE < matlab.System & coder.ExternalDependency
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
        %Header
        Header = '';

    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

        %UART Port
        UARTPort = 'UART0';
    end
    
    properties (Access = private)
        % Pre-computed constants.
        BaseAddr = 0;
        InterruptNumber = 0;
    end

    properties(Constant,Hidden)
        UARTPortSet = matlab.system.StringSet({'UART0', 'UART1', 'UART2', 'UART3', 'UART4', 'UART5'});
        
    end

    properties (Dependent, Access=protected)
    UARTPortEnum;
    end

    properties (Access=private,Hidden,Nontunable)
        dataInputLength;
        dataType;
    end
    
    methods
        % Constructor
        function obj = UART_WRITE(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
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


    end

    methods (Static, Access = protected)

      function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','UART Write',...
                'Text', sprintf('%s\n%s','Configure the UART Write block to write into UART console'),...
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
             'PropertyList',  {'UARTPort', ...
             });
       
             groups = General;
            
      end
    end

    methods(Static)

        function dataSizeinBytes = parseDataType(dataInput)
            switch (class(dataInput))
                case 'double'
                    dataSizeinBytes = 8;
                case {'single', 'uint32', 'int32'}
                    dataSizeinBytes = 4;
                case {'int16', 'uint16'}
                    dataSizeinBytes = 2;
                case {'int8', 'uint8', 'logical', 'char', 'string'}
                    dataSizeinBytes = 1;
                otherwise
                    dataSizeinBytes = 0;
            end
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

                coder.ceval('uart_write_setup', uint8(obj.UARTPortEnum));

                
            end
        end

            
        
        function stepImpl(obj,u)  %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);

                InputVectorLength = numel(u);
            
                switch(class(u))
                    case 'double'
                        obj.dataType = 0;
                    case 'single'
                        obj.dataType = 1;
                    case 'int8'
                        obj.dataType = 2;
                    case 'uint8'
                        obj.dataType = 3;
                    case 'int16'
                        obj.dataType = 4;
                    case 'unt16'
                        obj.dataType = 5;
                    case 'int32'
                        obj.dataType = 6;
                    case 'uint32'
                        obj.dataType = 7;
                    case 'boolean'
                        obj.dataType = 8;
                    otherwise
                        obj.dataType = 3;
                end

                coder.ceval('uart_write_step', coder.rref(u) , uint32(InputVectorLength), uint8(obj.dataType), uint8(obj.UARTPortEnum));
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');
                coder.ceval('uart_release');
            end
        end

        
    end
       
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(obj)
            num = 1;
        end

        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
            varargout = cell(1,num);
            varargout{index} = 'DataInput';
        end
        
        function num = getNumOutputsImpl(~)
            num = 0;
        end
        
        function flag = isInputSizeMutableImpl(~,~)
            flag = true;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = true;
        end
        
        
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'UART_WRITE'};
        end
    end
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'UART_WRITE';
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
