
classdef IPC_TX_AM261X < matlab.System & ...
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
        %IPC Self Core
        IPC_TXCore = 'R5FSS0 Core0';
        %IPC Remote Core
        IPC_RXCore = 'R5FSS0 Core0';
        %Source End Point
        SrcEnd_Point = 0;
        %Destination End Point
        DstEnd_Point = 0;
        

    end

    properties (Constant, Hidden)
        IPC_TXCoreSet = matlab.system.StringSet({'R5FSS0 Core0', 'R5FSS0 Core1', 'R5FSS1 Core0', 'R5FSS1 Core1'});
        IPC_RXCoreSet = matlab.system.StringSet({'R5FSS0 Core0', 'R5FSS0 Core1', 'R5FSS1 Core0', 'R5FSS1 Core1'});
       
    end
    
    properties (Dependent, Access=protected)
        IPC_TXCoreEnum;
        IPC_RXCoreEnum;
        
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
        %DataType
        DataType;
    end
    
    methods
        % Constructor
        function obj = IPC_TX_AM261X(varargin)
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
                coder.cinclude('MW_IPCTX.h');
                ipc_tx_setup_struct = struct('self_core', uint16(obj.IPC_TXCoreEnum), 'remote_core', uint16(obj.IPC_RXCoreEnum), 'end_point', uint16(obj.SrcEnd_Point), 'dest_endpoint', uint16(obj.DstEnd_Point));
                coder.cstructname(ipc_tx_setup_struct, 'IPCTXSetup', 'extern', 'HeaderFile', 'MW_IPCTX.h');
                coder.ceval('IPCTX_setup', coder.ref(ipc_tx_setup_struct));

                
            end
        end
        
        function stepImpl(obj, u)
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                InputLen = numel(u)

                switch (class(u))                      
                    case 'uint32'
                        obj.DataType = 2;
                    case 'int32'
                        obj.DataType = 3;
                    case 'int16'
                        obj.DataType = 4;
                    case 'uint16'
                        obj.DataType = 5;
                    case 'int8'
                        obj.DataType = 6;
                    case 'uint8'
                        obj.DataType = 7;
                    otherwise
                        obj.DataType = 0;
                end
                ipc_tx_step_struct = struct('self_core', uint16(obj.IPC_TXCoreEnum), 'remote_core', uint16(obj.IPC_RXCoreEnum), 'end_point', uint16(obj.SrcEnd_Point), 'dest_endpoint', uint16(obj.DstEnd_Point), 'dataType', uint8(obj.DataType));
                coder.cstructname(ipc_tx_step_struct, 'IPCTXStep', 'extern', 'HeaderFile', 'MW_IPCTX.h');
                coder.ceval('IPCTX_step', coder.ref(ipc_tx_step_struct), coder.rref(u), uint16(InputLen));

            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                coder.ceval('IPCTX_release');
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(obj)
            num = 1;
        end

        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
            varargout = cell(1,num);
            varargout{index} = 'Msg';
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
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'IPC_TX'};
        end    
    end
    
    methods (Static, Access=protected)

        function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','IPC_TX',...
                'Text', sprintf('%s\n%s','Configure the IPC TX block'),...
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
             "PropertyList", {'IPC_TXCore', 'IPC_RXCore', 'SrcEnd_Point', 'DstEnd_Point', 'SampleTime'});

           
            groups = [General];
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'IPC_TX';
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
                addIncludeFiles(buildInfo, 'MW_IPCTX.h', includeDir);
                % addIncludeFiles(buildInfo, 'MW_IPCRX.h', includeDir);
                addSourceFiles(buildInfo, 'MW_IPCTX.c', srcDir);
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

