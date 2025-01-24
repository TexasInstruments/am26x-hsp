 classdef GPIO_IN_AM263PX < matlab.System & coder.ExternalDependency
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
        %GPIO Pin
        GPIO_Pin = 'GPIO0';
        %Output Data Type
        Output_DataType = 'Double';
        %Interrupt Trigger
        Trigger_Type = 'None';
        %Enable Interrupt
        GPIOEnableInterrupt (1, 1) logical = false;
        
    end
    
    properties (Access = private)
        % Pre-computed constants.
        GPIOInstanceAddress;
    end 

    properties(Constant,Hidden)
        GPIO_PinSet = matlab.system.StringSet({'GPIO0', 'GPIO1', 'GPIO2', 'GPIO3', 'GPIO4', 'GPIO5', 'GPIO6', 'GPIO7', 'GPIO8', 'GPIO9', 'GPIO10', 'GPIO11', 'GPIO12', 'GPIO13', 'GPIO14', 'GPIO15', 'GPIO16', 'GPIO17', 'GPIO18', 'GPIO19', 'GPIO20', 'GPIO21', 'GPIO22', 'GPIO23', 'GPIO24', 'GPIO25', 'GPIO26', 'GPIO27', 'GPIO28', 'GPIO29' , 'GPIO30', 'GPIO31', 'GPIO32', 'GPIO33', 'GPIO34', 'GPIO35', 'GPIO36', 'GPIO37', 'GPIO38', 'GPIO39', 'GPIO40', 'GPIO41', 'GPIO42', 'GPIO43', 'GPIO44', 'GPIO45', 'GPIO46', 'GPIO47', 'GPIO48' , 'GPIO49', 'GPIO50', 'GPIO51', 'GPIO52', 'GPIO53', 'GPIO54', 'GPIO55', 'GPIO56', 'GPIO57', 'GPIO58', 'GPIO59', 'GPIO60', 'GPIO61', 'GPIO62', 'GPIO63', 'GPIO64', 'GPIO65', 'GPIO66', 'GPIO67', 'GPIO68' ,'GPIO69', 'GPIO70', 'GPIO71', 'GPIO72', 'GPIO73', 'GPIO74', 'GPIO75', 'GPIO76', 'GPIO77', 'GPIO78', 'GPIO79', 'GPIO80', 'GPIO81', 'GPIO82', 'GPIO83', 'GPIO84', 'GPIO85', 'GPIO86', 'GPIO87', 'GPIO88', 'GPIO89', 'GPIO90', 'GPIO91', 'GPIO92', 'GPIO93', 'GPIO94', 'GPIO95', 'GPIO96', 'GPIO97', 'GPIO98', 'GPIO99', 'GPIO100', 'GPIO101', 'GPIO102', 'GPIO103', 'GPIO104', 'GPIO105', 'GPIO106', 'GPIO107', 'GPIO108', 'GPIO109', 'GPIO110', 'GPIO111', 'GPIO112', 'GPIO113', 'GPIO114', 'GPIO115', 'GPIO116', 'GPIO117', 'GPIO118', 'GPIO119', 'GPIO120', 'GPIO121', 'GPIO122', 'GPIO123', 'GPIO124', 'GPIO125', 'GPIO126', 'GPIO127', 'GPIO128', 'GPIO129', 'GPIO130', 'GPIO131', 'GPIO132', 'GPIO133', 'GPIO134', 'GPIO135', 'GPIO136', 'GPIO137', 'GPIO138', 'GPIO139', 'GPIO140', 'GPIO141', 'GPIO142', 'GPIO143', 'GPIO144' });
        Output_DataTypeSet = matlab.system.StringSet({'Double', 'Single', 'INT8', 'Unsigned INT8', 'INT16', 'Unsigned INT16', 'INT32', 'Unsigned INT32'});
        Trigger_TypeSet = matlab.system.StringSet({'None', 'Rising Edge', 'Falling Edge', 'Rising and Falling Edge'});
    end

    properties (Dependent, Access=protected)
        GPIO_PinEnum;
        Output_DataTypeEnum;
        Trigger_TypeEnum;
    end

    methods
        % Constructor
        function obj = GPIO_IN_AM263PX(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.GPIO_PinEnum(obj)
            if isequal(obj.GPIO_Pin,'GPIO0')
                ret = uint16(0);
            elseif isequal(obj.GPIO_Pin, 'GPIO1')
                ret = uint16(1);
            elseif isequal(obj.GPIO_Pin, 'GPIO2')
                ret = uint16(2);
            elseif isequal(obj.GPIO_Pin, 'GPIO3')
                ret = uint16(3);
            elseif isequal(obj.GPIO_Pin, 'GPIO4')
                ret = uint16(4);
            elseif isequal(obj.GPIO_Pin, 'GPIO5')
                ret = uint16(5);
            elseif isequal(obj.GPIO_Pin, 'GPIO6')
                ret = uint16(6);
            elseif isequal(obj.GPIO_Pin, 'GPIO7')
                ret = uint16(7);
            elseif isequal(obj.GPIO_Pin, 'GPIO8')
                ret = uint16(8);
            elseif isequal(obj.GPIO_Pin, 'GPIO9')
                ret = uint16(9);
            elseif isequal(obj.GPIO_Pin, 'GPIO10')
                ret = uint16(10);
            elseif isequal(obj.GPIO_Pin, 'GPIO11')
                ret = uint16(11);
            elseif isequal(obj.GPIO_Pin, 'GPIO12')
                ret = uint16(12);
            elseif isequal(obj.GPIO_Pin, 'GPIO13')
                ret = uint16(13);
            elseif isequal(obj.GPIO_Pin, 'GPIO14')
                ret = uint16(14);
            elseif isequal(obj.GPIO_Pin, 'GPIO15')
                ret = uint16(15);
            elseif isequal(obj.GPIO_Pin, 'GPIO16')
                ret = uint16(16);
            elseif isequal(obj.GPIO_Pin, 'GPIO17')
                ret = uint16(17);
            elseif isequal(obj.GPIO_Pin, 'GPIO18')
                ret = uint16(18);
            elseif isequal(obj.GPIO_Pin, 'GPIO19')
                ret = uint16(19);
            elseif isequal(obj.GPIO_Pin, 'GPIO20')
                ret = uint16(20);
            elseif isequal(obj.GPIO_Pin, 'GPIO21')
                ret = uint16(21);
            elseif isequal(obj.GPIO_Pin, 'GPIO22')
                ret = uint16(22);
            elseif isequal(obj.GPIO_Pin, 'GPIO23')
                ret = uint16(23);
            elseif isequal(obj.GPIO_Pin, 'GPIO24')
                ret = uint16(24);
            elseif isequal(obj.GPIO_Pin, 'GPIO25')
                ret = uint16(25);
            elseif isequal(obj.GPIO_Pin, 'GPIO26')
                ret = uint16(26);
            elseif isequal(obj.GPIO_Pin, 'GPIO27')
                ret = uint16(27);
            elseif isequal(obj.GPIO_Pin, 'GPIO28')
                ret = uint16(28);
            elseif isequal(obj.GPIO_Pin, 'GPIO29')
                ret = uint16(29);
            elseif isequal(obj.GPIO_Pin, 'GPIO30')
                ret = uint16(30);
            elseif isequal(obj.GPIO_Pin, 'GPIO31')
                ret = uint16(31);
            elseif isequal(obj.GPIO_Pin, 'GPIO32')
                ret = uint16(32);
            elseif isequal(obj.GPIO_Pin, 'GPIO33')
                ret = uint16(33);
            elseif isequal(obj.GPIO_Pin, 'GPIO34')
                ret = uint16(34);
            elseif isequal(obj.GPIO_Pin, 'GPIO35')
                ret = uint16(35);
            elseif isequal(obj.GPIO_Pin, 'GPIO36')
                ret = uint16(36);
            elseif isequal(obj.GPIO_Pin, 'GPIO37')
                ret = uint16(37);
            elseif isequal(obj.GPIO_Pin, 'GPIO38')
                ret = uint16(38);
            elseif isequal(obj.GPIO_Pin, 'GPIO39')
                ret = uint16(39);
            elseif isequal(obj.GPIO_Pin, 'GPIO40')
                ret = uint16(40);
            elseif isequal(obj.GPIO_Pin, 'GPIO41')
                ret = uint16(41);
            elseif isequal(obj.GPIO_Pin, 'GPIO42')
                ret = uint16(42);
            elseif isequal(obj.GPIO_Pin, 'GPIO43')
                ret = uint16(43);
            elseif isequal(obj.GPIO_Pin, 'GPIO44')
                ret = uint16(44);
            elseif isequal(obj.GPIO_Pin, 'GPIO45')
                ret = uint16(45);
            elseif isequal(obj.GPIO_Pin, 'GPIO46')
                ret = uint16(46);
            elseif isequal(obj.GPIO_Pin, 'GPIO47')
                ret = uint16(47);
            elseif isequal(obj.GPIO_Pin, 'GPIO48')
                ret = uint16(48);
            elseif isequal(obj.GPIO_Pin, 'GPIO49')
                ret = uint16(49);
            elseif isequal(obj.GPIO_Pin, 'GPIO50')
                ret = uint16(50);
            elseif isequal(obj.GPIO_Pin, 'GPIO51')
                ret = uint16(51);
            elseif isequal(obj.GPIO_Pin, 'GPIO52')
                ret = uint16(52);
            elseif isequal(obj.GPIO_Pin, 'GPIO53')
                ret = uint16(53);
            elseif isequal(obj.GPIO_Pin, 'GPIO54')
                ret = uint16(54);
            elseif isequal(obj.GPIO_Pin, 'GPIO55')
                ret = uint16(55);
            elseif isequal(obj.GPIO_Pin, 'GPIO56')
                ret = uint16(56);
            elseif isequal(obj.GPIO_Pin, 'GPIO57')
                ret = uint16(57);
            elseif isequal(obj.GPIO_Pin, 'GPIO58')
                ret = uint16(58);
            elseif isequal(obj.GPIO_Pin, 'GPIO59')
                ret = uint16(59);
            elseif isequal(obj.GPIO_Pin, 'GPIO60')
                ret = uint16(60);
            elseif isequal(obj.GPIO_Pin, 'GPIO61')
                ret = uint16(61);
            elseif isequal(obj.GPIO_Pin, 'GPIO62')
                ret = uint16(62);
            elseif isequal(obj.GPIO_Pin, 'GPIO63')
                ret = uint16(63);
            elseif isequal(obj.GPIO_Pin, 'GPIO64')
                ret = uint16(64);
            elseif isequal(obj.GPIO_Pin, 'GPIO65')
                ret = uint16(65);
            elseif isequal(obj.GPIO_Pin, 'GPIO66')
                ret = uint16(66);
            elseif isequal(obj.GPIO_Pin, 'GPIO67')
                ret = uint16(67);
            elseif isequal(obj.GPIO_Pin, 'GPIO68')
                ret = uint16(68);
            elseif isequal(obj.GPIO_Pin, 'GPIO69')
                ret = uint16(69);
            elseif isequal(obj.GPIO_Pin, 'GPIO70')
                ret = uint16(70);
            elseif isequal(obj.GPIO_Pin, 'GPIO71')
                ret = uint16(71);
            elseif isequal(obj.GPIO_Pin, 'GPIO72')
                ret = uint16(72);
            elseif isequal(obj.GPIO_Pin, 'GPIO73')
                ret = uint16(73);
            elseif isequal(obj.GPIO_Pin, 'GPIO74')
                ret = uint16(74);
            elseif isequal(obj.GPIO_Pin, 'GPIO75')
                ret = uint16(75);
            elseif isequal(obj.GPIO_Pin, 'GPIO76')
                ret = uint16(76);
            elseif isequal(obj.GPIO_Pin, 'GPIO77')
                ret = uint16(77);
            elseif isequal(obj.GPIO_Pin, 'GPIO78')
                ret = uint16(78);
            elseif isequal(obj.GPIO_Pin, 'GPIO79')
                ret = uint16(79);
            elseif isequal(obj.GPIO_Pin, 'GPIO80')
                ret = uint16(80);
            elseif isequal(obj.GPIO_Pin, 'GPIO81')
                ret = uint16(81);
            elseif isequal(obj.GPIO_Pin, 'GPIO82')
                ret = uint16(82);
            elseif isequal(obj.GPIO_Pin, 'GPIO83')
                ret = uint16(83);
            elseif isequal(obj.GPIO_Pin, 'GPIO84')
                ret = uint16(84);
            elseif isequal(obj.GPIO_Pin, 'GPIO85')
                ret = uint16(85);
            elseif isequal(obj.GPIO_Pin, 'GPIO86')
                ret = uint16(86);
            elseif isequal(obj.GPIO_Pin, 'GPIO87')
                ret = uint16(87);
            elseif isequal(obj.GPIO_Pin, 'GPIO88')
                ret = uint16(88);
            elseif isequal(obj.GPIO_Pin, 'GPIO89')
                ret = uint16(89);
            elseif isequal(obj.GPIO_Pin, 'GPIO90')
                ret = uint16(90);
            elseif isequal(obj.GPIO_Pin, 'GPIO91')
                ret = uint16(91);
            elseif isequal(obj.GPIO_Pin, 'GPIO92')
                ret = uint16(92);
            elseif isequal(obj.GPIO_Pin, 'GPIO93')
                ret = uint16(93);
            elseif isequal(obj.GPIO_Pin, 'GPIO94')
                ret = uint16(94);
            elseif isequal(obj.GPIO_Pin, 'GPIO95')
                ret = uint16(95);
            elseif isequal(obj.GPIO_Pin, 'GPIO96')
                ret = uint16(96);
            elseif isequal(obj.GPIO_Pin, 'GPIO97')
                ret = uint16(97);
            elseif isequal(obj.GPIO_Pin, 'GPIO98')
                ret = uint16(98);
            elseif isequal(obj.GPIO_Pin, 'GPIO99')
                ret = uint16(99);
            elseif isequal(obj.GPIO_Pin, 'GPIO100')
                ret = uint16(100);
            elseif isequal(obj.GPIO_Pin, 'GPIO101')
                ret = uint16(101);
            elseif isequal(obj.GPIO_Pin, 'GPIO102')
                ret = uint16(102);
            elseif isequal(obj.GPIO_Pin, 'GPIO103')
                ret = uint16(103);
            elseif isequal(obj.GPIO_Pin, 'GPIO104')
                ret = uint16(104);
            elseif isequal(obj.GPIO_Pin, 'GPIO105')
                ret = uint16(105);
            elseif isequal(obj.GPIO_Pin, 'GPIO106')
                ret = uint16(106);
            elseif isequal(obj.GPIO_Pin, 'GPIO107')
                ret = uint16(107);
            elseif isequal(obj.GPIO_Pin, 'GPIO108')
                ret = uint16(108);
            elseif isequal(obj.GPIO_Pin, 'GPIO109')
                ret = uint16(109);
            elseif isequal(obj.GPIO_Pin, 'GPIO110')
                ret = uint16(110);
            elseif isequal(obj.GPIO_Pin, 'GPIO111')
                ret = uint16(111);
            elseif isequal(obj.GPIO_Pin, 'GPIO112')
                ret = uint16(112);
            elseif isequal(obj.GPIO_Pin, 'GPIO113')
                ret = uint16(113);
            elseif isequal(obj.GPIO_Pin, 'GPIO114')
                ret = uint16(114);
            elseif isequal(obj.GPIO_Pin, 'GPIO115')
                ret = uint16(115);
            elseif isequal(obj.GPIO_Pin, 'GPIO116')
                ret = uint16(116);
            elseif isequal(obj.GPIO_Pin, 'GPIO117')
                ret = uint16(117);
            elseif isequal(obj.GPIO_Pin, 'GPIO118')
                ret = uint16(118);
            elseif isequal(obj.GPIO_Pin, 'GPIO119')
                ret = uint16(119);
            elseif isequal(obj.GPIO_Pin, 'GPIO120')
                ret = uint16(120);
           elseif isequal(obj.GPIO_Pin, 'GPIO121')
                ret = uint16(121);
            elseif isequal(obj.GPIO_Pin, 'GPIO122')
                ret = uint16(122);
            elseif isequal(obj.GPIO_Pin, 'GPIO123')
                ret = uint16(123);
            elseif isequal(obj.GPIO_Pin, 'GPIO124')
                ret = uint16(124);
            elseif isequal(obj.GPIO_Pin, 'GPIO125')
                ret = uint16(125);
            elseif isequal(obj.GPIO_Pin, 'GPIO126')
                ret = uint16(126);
            elseif isequal(obj.GPIO_Pin, 'GPIO127')
                ret = uint16(127);
            elseif isequal(obj.GPIO_Pin, 'GPIO128')
                ret = uint16(128);
            elseif isequal(obj.GPIO_Pin, 'GPIO129')
                ret = uint16(129);
            elseif isequal(obj.GPIO_Pin, 'GPIO130')
                ret = uint16(130);
            elseif isequal(obj.GPIO_Pin, 'GPIO131')
                ret = uint16(131);
            elseif isequal(obj.GPIO_Pin, 'GPIO132')
                ret = uint16(132);
            elseif isequal(obj.GPIO_Pin, 'GPIO133')
                ret = uint16(133);
            elseif isequal(obj.GPIO_Pin, 'GPIO134')
                ret = uint16(134);
            elseif isequal(obj.GPIO_Pin, 'GPIO135')
                ret = uint16(135);
            elseif isequal(obj.GPIO_Pin, 'GPIO136')
                ret = uint16(136);
            elseif isequal(obj.GPIO_Pin, 'GPIO137')
                ret = uint16(137);
            elseif isequal(obj.GPIO_Pin, 'GPIO138')
                ret = uint16(138);
            elseif isequal(obj.GPIO_Pin, 'GPIO139')
                ret = uint16(139);
            elseif isequal(obj.GPIO_Pin, 'GPIO140')
                ret = uint16(140);
            elseif isequal(obj.GPIO_Pin, 'GPIO141')
                ret = uint16(141);
            elseif isequal(obj.GPIO_Pin, 'GPIO142')
                ret = uint16(142);
            elseif isequal(obj.GPIO_Pin, 'GPIO143')
                ret = uint16(143);
            elseif isequal(obj.GPIO_Pin, 'GPIO144')
                ret = uint16(144);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Output_DataTypeEnum(obj)
            if isequal(obj.Output_DataType, 'Double')
                ret = uint16(0);
            elseif isequal(obj.Output_DataType, 'Single')
                ret = uint16(1);
            elseif isequal(obj.Output_DataType, 'INT8')
                ret = uint16(2);
            elseif isequal(obj.Output_DataType, 'Unsigned INT8')
                ret = uint16(3);
            elseif isequal(obj.Output_DataType, 'INT16')
                ret = uint16(4);
            elseif isequal(obj.Output_DataType, 'Unsigned INT16')
                ret = uint16(5);
            elseif isequal(obj.Output_DataType, 'INT32')
                ret = uint16(6);
            elseif isequal(obj.Output_DataType, 'Unsigned INT32')
                ret = uint16(7);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Trigger_TypeEnum(obj)
            if isequal(obj.Trigger_Type, 'None') 
                ret = uint16(0);
            elseif isequal(obj.Trigger_Type, 'Rising Edge')
                ret = uint16(1);
            elseif isequal(obj.Trigger_Type, 'Falling Edge')
                ret = uint16(2);
            elseif isequal(obj. Trigger_Type, 'Rising and Falling Edge')
                ret = uint16(3);
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
            header = matlab.system.display.Header(...
                'Title','GPIO IN',...
                'Text', sprintf('%s\n%s','Configure the GPIO input block to read the pin state of the configured GPIO pin'),...
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
             'PropertyList',  {'GPIO_Pin', 'Trigger_Type', 'GPIOEnableInterrupt', 'Output_DataType', 'SampleTime'});   
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
                coder.cinclude('MW_GPIOIN.h');
                obj.GPIOInstanceAddress = coder.opaque('uint32_t', 'CSL_GPIO0_U_BASE');
                gpio_init_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum), 'trigType', uint16(obj.Trigger_TypeEnum), 'EnableInterrupt', obj.GPIOEnableInterrupt);
                coder.cstructname(gpio_init_struct, 'GPIOInputSetupStruct', 'extern', 'HeaderFile', 'MW_GPIOIN.h');
                coder.ceval('GPIOInput_setup', coder.ref(gpio_init_struct));
                % Call C-function implementing device initialization
                % coder.cinclude('source.h');
                % coder.ceval('source_init');
            end
        end
        
        function y = stepImpl(obj)   %#ok<MANU>
            x = uint16(0);
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                gpio_step_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum));
                coder.cstructname(gpio_step_struct, 'GPIOInputStepStruct', 'extern', 'HeaderFile', 'MW_GPIOIN.h');
                x = coder.ceval('GPIOInput_step', coder.ref(gpio_step_struct));
                if isequal(obj.Output_DataType, 'Double')
                    y = double(x);
                elseif isequal(obj.Output_DataType,'Single')
                    y=single(x);
                elseif isequal(obj.Output_DataType, 'INT8')
                    y=int8(x);
                elseif isequal(obj.Output_DataType, 'Unsigned INT8')
                    y=uint8(x);
                elseif isequal(obj.Output_DataType, 'INT16')
                    y=int16(x);
                elseif isequal(obj.Output_DataType, 'Unsigned INT16')
                    y=uint16(x);
                elseif isequal(obj.Output_DataType, 'INT32')
                    y=int32(x);
                elseif isequal(obj.Output_DataType, 'Unsigned INT32')
                    y=uint32(x);
                else
                    y=uint16(x);
                end
               

            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                gpio_release_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum), 'EnableInterrupt', obj.GPIOEnableInterrupt);
                coder.cstructname(gpio_release_struct, 'GPIOInputReleaseStruct', 'extern', 'HeaderFile', 'MW_GPIOIN.h');
                coder.ceval('GPIOInput_release', coder.ref(gpio_release_struct));
            end
        end

        %% Define output properties
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
        
        function varargout = getOutputSizeImpl(~)
            varargout{1} = [1,1];
        end
        
        function varargout = getOutputDataTypeImpl(obj)
            if isequal(obj.Output_DataType, 'Double')
               varargout{1} = "double";
           elseif isequal(obj.Output_DataType, 'Single')
               varargout{1}= "single";
           elseif isequal(obj.Output_DataType, 'INT8')
               varargout {1} ="int8";
           elseif isequal(obj.Output_DataType, 'Unsigned INT8')
                 varargout{1}="uint8";
           elseif isequal(obj.Output_DataType, 'INT16')
                 varargout{1}="int16";
           elseif isequal(obj.Output_DataType, 'Unsigned INT16')
                 varargout{1}="uint16";
           elseif isequal(obj.Output_DataType, 'INT32')
                 varargout{1}="int32";
           elseif isequal(obj.Output_DataType, 'Unsigned INT32')
                 varargout{1}="uint32";
             else
                 varargout{1}="uint16";
           end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'GPIO_IN'};
        end    
    end
    
   
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'GPIO_IN';
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
                addIncludeFiles(buildInfo, 'MW_GPIOIN.h', includeDir);
                addSourceFiles(buildInfo, 'MW_GPIOIN.c', srcDir);
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
