
classdef IPC_RX_AM263PX < matlab.System & ...
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

        %Sample Time
        SampleTime = 0.1;
        %IPC Self Core
        IPC_TXCore = 'R5FSS0 Core0';
        %IPC Remote Core
        IPC_RXCore = 'R5FSS0 Core0';
        %Input Length
        Input_Len = 0;
        %Source End Point
        End_Point = 0;
        %Data type
        DataType = 'UINT8';

    end

    properties (Constant, Hidden)
        IPC_TXCoreSet = matlab.system.StringSet({'R5FSS0 Core0', 'R5FSS0 Core1', 'R5FSS1 Core0', 'R5FSS1 Core1'});
        IPC_RXCoreSet = matlab.system.StringSet({'R5FSS0 Core0', 'R5FSS0 Core1', 'R5FSS1 Core0', 'R5FSS1 Core1'});
        DataTypeSet = matlab.system.StringSet({'UINT8', 'INT8', 'UINT16', 'INT16', 'UINT32', 'INT32'});
    end
    
    properties (Dependent, Access=protected)
        IPC_TXCoreEnum;
        IPC_RXCoreEnum;
        DataTypeEnum;
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
    end
    
    methods
        % Constructor
        function obj = IPC_RX_AM263PX(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.IPC_TXCoreEnum(obj)
            if(isequal(obj.IPC_TXCore, 'R5FSS0 Core0'))
                ret = uint8(0);
            elseif(isequal(obj.IPC_TXCore, 'R5FSS0 Core1'))
                ret = uint8(1);
            elseif(isequal(obj.IPC_TXCore, 'R5FSS1 Core0'))
                ret = uint8(2);
            elseif(isequal(obj.IPC_TXCore, 'R5FSS1 Core1'))
                ret = uint8(3);
            else
                ret = uint8(0);
            end
        end

        function ret = get.IPC_RXCoreEnum(obj)
            if(isequal(obj.IPC_RXCore, 'R5FSS0 Core0'))
                ret = uint8(0);
            elseif(isequal(obj.IPC_RXCore, 'R5FSS0 Core1'))
                ret = uint8(1);
            elseif(isequal(obj.IPC_RXCore, 'R5FSS1 Core0'))
                ret = uint8(2);
            elseif(isequal(obj.IPC_RXCore, 'R5FSS1 Core1'))
                ret = uint8(3);
            else
                ret = uint8(0);
            end
        end

        function ret = get.DataTypeEnum(obj)
            if(isequal(obj.DataType, 'UINT8'))
                ret = uint16(0);
            elseif isequal(obj.DataType, 'INT8')
                ret = uint16(1);
            elseif isequal(obj.DataType, 'UINT16')
                ret = uint16(2);
            elseif isequal(obj.DataType, 'INT16')
                ret = uint16(3);
            elseif isequal(obj.DataType, 'UINT32')
                ret = uint16(4);
            elseif isequal(obj.DataType, 'INT32')
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

    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','IPC_Receive',...
                'Text', sprintf('%s\n%s','Configure the IPC Receive block'),...
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
             "PropertyList", {'IPC_TXCore', 'IPC_RXCore', 'Input_Len', 'End_Point', 'DataType', 'SampleTime'});

            
            groups = General;

            
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
                % coder.cinclude('sink.h');
                % coder.ceval('sink_init');
                coder.cinclude('MW_IPCRX.h');
                ipc_rx_setup_struct = struct('self_core', uint16(obj.IPC_TXCoreEnum), 'remote_core', uint16(obj.IPC_RXCoreEnum), 'end_point', uint16(obj.End_Point));
                coder.cstructname(ipc_rx_setup_struct, 'IPCRXSetup', 'extern', 'HeaderFile', 'MW_IPCRX.h');
                coder.ceval('IPCRX_setup', coder.ref(ipc_rx_setup_struct));


            end
        end
        function msg_val = stepImpl(obj)   %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);

                if isequal(obj.DataType,'UINT8')
                    msg = uint8(zeros(obj.Input_Len,1));
                elseif isequal(obj.DataType,'INT8')
                    msg = int8(zeros(obj.Input_Len,1));
                elseif isequal(obj.DataType,'UINT16')
                    msg = uint16(zeros(obj.Input_Len,1));
                elseif isequal(obj.DataType,'INT16')
                    msg = int16(zeros(obj.Input_Len,1)); 
                elseif isequal(obj.DataType,'UINT32')
                    msg = uint32(zeros(obj.Input_Len,1));
                elseif isequal(obj.DataType,'INT32')
                    msg = int32(zeros(obj.Input_Len,1)); 
                else
                    msg = uint8(zeros(obj.Input_Len,1));
                end


                ipc_rx_step_struct = struct('self_core', uint16(obj.IPC_TXCoreEnum), 'remote_core', uint16(obj.IPC_RXCoreEnum), 'end_point', uint16(obj.End_Point), 'dataType', uint8(obj.DataTypeEnum));
                coder.cstructname(ipc_rx_step_struct, 'IPCRXStep', 'extern', 'HeaderFile', 'MW_IPCRX.h');
                coder.ceval('IPCRX_step', coder.ref(ipc_rx_step_struct), coder.wref(msg), uint8(obj.Input_Len));

                % msg_val = uint16(msg);

                %msg_val = uint32(msg(1:uint8(obj.Input_Len)));

                if isequal(obj.DataType,'UINT8')
                    msg_val = uint8(msg);
                elseif isequal(obj.DataType,'INT8')
                    msg_val = int8(msg);
                elseif isequal(obj.DataType,'UINT16')
                    msg_val = uint16(msg);
                elseif isequal(obj.DataType,'INT16')
                    msg_val = int16(msg); 
                elseif isequal(obj.DataType,'UINT32')
                    msg_val = uint32(msg);
                elseif isequal(obj.DataType,'INT32')
                    msg_val = int32(msg); 
                else
                    msg_val = uint8(msg);
                end
                

            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                coder.ceval('IPCRX_release', obj.baseaddr);
            end
        end

        
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 0;
        end

        function num = getNumOutputsImpl(obj)

                num = 1;
        end
        
        function varargout = isOutputFixedSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                varargout{indx} = false;
            end

        end


        function varargout = isOutputComplexImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                varargout{indx} = false;
            end

        end


        function varargout = getOutputSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                varargout{indx} = [obj.Input_Len,1];
            end

        end



        function varargout = getOutputDataTypeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                   if isequal(obj.DataType, 'INT8')
                       varargout {1} ="int8";
                   elseif isequal(obj.DataType, 'UINT8')
                       varargout{1}="uint8";
                   elseif isequal(obj.DataType, 'INT16')
                       varargout{1}="int16";
                   elseif isequal(obj.DataType, 'UINT16')
                       varargout{1}="uint16";
                   elseif isequal(obj.DataType, 'INT32')
                       varargout{1}="int32";
                   elseif isequal(obj.DataType, 'UINT32')
                       varargout{1}="uint32";
                   else
                       varargout{1}="uint8";
                  end
            end
        end

        function varargout = getOutputNamesImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if(num > 0)
                varargout{indx} = cell(1,num);
                varargout{indx} = 'Msg';
            end
        end
        
        
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'IPC_RX'};
        end
    end
    
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'IPC_RX';
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
                addIncludeFiles(buildInfo, 'MW_IPCRX.h', includeDir);
                addSourceFiles(buildInfo, 'MW_IPCRX.c', srcDir);
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

