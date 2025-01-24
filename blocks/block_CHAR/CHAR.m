classdef CHAR < matlab.System & ...
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
     
        
        
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.
        
       
      
      
    end
    
    properties (Access = private)
        % Pre-computed constants.

        %DataType
        DataType;
        %DigitCount
        DigitCount;
    end
    
    methods
        function obj = CHAR(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end
        
    end
    
    methods (Access=protected)
        function setupImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                % coder.cinclude('source.h');
                % coder.ceval('source_init');
            end
        end
        
        function [y] = stepImpl(obj, u)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementyng device output
                %y = coder.ceval('source_output');
                coder.cinclude('MW_CHAR.h');
             
                InputLength = numel(u);

                switch (class(u))
                    case 'double'
                        obj.DataType = 0;
                    case 'single'
                        obj.DataType = 1;                      
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
                    case 'logical'
                        obj.DataType = 8;
                    otherwise
                        obj.DataType = 0;
                end

              
                InputLength = numel(u);
                a = uint8(0);
                x = uint8(zeros(1024,1));
                coder.ceval('CHAR_step1', coder.rref(u), uint8(obj.DataType), coder.wref(a), InputLength, coder.wref(x));
                
                
                y = uint8(char(x(1:a)));
                 
                
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
            end
        end
        
       
        
    end
    
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(~)
            num = 1;
        end

        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
            varargout = cell(1,num);
            varargout{index} = 'DataInput';
        end

        function flag = isInputSizeMutableImpl(obj,~)
            flag = true;
        end
        
        function flag = isInputComplexityMutableImpl(~,~)
            flag = false;
        end
        
        function validateInputsImpl(~, u)
            if isempty(coder.target)
                % Run input validation only in Simulation
                
            end
        end
        
        function num = getNumOutputsImpl(~)
            num = 1;
        end
        
        function varargout = isOutputFixedSizeImpl(~,~)
            varargout{1} = false;
       
        end
        
        
        function varargout = isOutputComplexImpl(~)
            varargout{1} = false;
       
        end

       
          function varargout = getOutputSizeImpl(obj)
            varargout{1} = [1024, 1];
        end
        
       
        
        function varargout = getOutputDataTypeImpl(obj)
            varargout{1} = 'uint8';
        
        end

        function varargout = getOutputNamesImpl(~)
            varargout{1} = 'UARTData';

        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'CHAR'};
        end
        
    end
    
    methods (Static, Access=protected)
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
        
        
        
        function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','CHAR',...
                'Text', sprintf('%s\n\n%s','Used to change the data type of input to char when the input has to be printed into UART PORT'),...
                'ShowSourceLink', false);
        end
    end
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'CHAR';
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
                 addIncludeFiles(buildInfo, 'MW_CHAR.h', includeDir);
                addSourceFiles(buildInfo, 'MW_CHAR.c', srcDir);
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
