classdef GPIO_OUT_AM263PX < matlab.System & coder.ExternalDependency
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
        
        %GPIO Pin
        GPIO_Pin = 'GPIO0';
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
        GPIO_PinSet = matlab.system.StringSet({'GPIO0', 'GPIO1', 'GPIO2', 'GPIO3', 'GPIO4', 'GPIO5', 'GPIO6', 'GPIO7', 'GPIO8', 'GPIO9', 'GPIO10', 'GPIO11', 'GPIO12', 'GPIO13', 'GPIO14', 'GPIO15', 'GPIO16', 'GPIO17', 'GPIO18', 'GPIO19', 'GPIO20', 'GPIO21', 'GPIO22', 'GPIO23', 'GPIO24', 'GPIO25', 'GPIO26', 'GPIO27', 'GPIO28', 'GPIO29' , 'GPIO30', 'GPIO31', 'GPIO32', 'GPIO33', 'GPIO34', 'GPIO35', 'GPIO36', 'GPIO37', 'GPIO38', 'GPIO39', 'GPIO40', 'GPIO41', 'GPIO42', 'GPIO43', 'GPIO44', 'GPIO45', 'GPIO46', 'GPIO47', 'GPIO48' , 'GPIO49', 'GPIO50', 'GPIO51', 'GPIO52', 'GPIO53', 'GPIO54', 'GPIO55', 'GPIO56', 'GPIO57', 'GPIO58', 'GPIO59', 'GPIO60', 'GPIO61', 'GPIO62', 'GPIO63', 'GPIO64', 'GPIO65', 'GPIO66', 'GPIO67', 'GPIO68' ,'GPIO69', 'GPIO70', 'GPIO71', 'GPIO72', 'GPIO73', 'GPIO74', 'GPIO75', 'GPIO76', 'GPIO77', 'GPIO78', 'GPIO79', 'GPIO80', 'GPIO81', 'GPIO82', 'GPIO83', 'GPIO84', 'GPIO85', 'GPIO86', 'GPIO87', 'GPIO88', 'GPIO89', 'GPIO90', 'GPIO91', 'GPIO92', 'GPIO93', 'GPIO94', 'GPIO95', 'GPIO96', 'GPIO97', 'GPIO98', 'GPIO99', 'GPIO100'});
        Trigger_TypeSet = matlab.system.StringSet({'None', 'Rising Edge', 'Falling Edge', 'Rising and Falling Edge'});
    end

    properties (Dependent, Access=protected)
        GPIO_PinEnum;
        Trigger_TypeEnum;
    end

  
    
    methods
        % Constructor
        function obj = GPIO_OUT_AM263PX(varargin)
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
    end

    methods (Static, Access = protected)

      function header = getHeaderImpl()
            header = matlab.system.display.Header(...
                'Title','GPIO OUT',...
                'Text', sprintf('%s\n%s','Configure the GPIO output block to be set to a high state or low state depending on input signal'),...
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
             'PropertyList',  {'GPIO_Pin', 'Trigger_Type', 'GPIOEnableInterrupt', 'GPIO_Sample_Time'});   
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
                
                coder.cinclude('MW_GPIOOUT.h');
                obj.GPIOInstanceAddress = coder.opaque('uint32_t', 'CSL_GPIO0_U_BASE');
                gpioout_init_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum), 'trigType', uint16(obj.Trigger_TypeEnum), 'EnableInterrupt', obj.GPIOEnableInterrupt);
                coder.cstructname(gpioout_init_struct, 'GPIOOutputSetupStruct', 'extern', 'HeaderFile', 'MW_GPIOOUT.h');
                coder.ceval('GPIOOutput_setup', coder.ref(gpioout_init_struct));
                                
                %Move to init.c
                %coder.ceval('util_iomux_config_for_gpio', obj.GPIO_PINS);
                
            end
        end
        
        function stepImpl(obj,u)  %#ok<INUSD>
            if isempty(coder.target)
                % Place simulation output code here 
            else
                % Call C-function implementing device output
                %coder.ceval('sink_output',u);
                
                gpioout_step_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum), 'input_signal', u);
                coder.cstructname(gpioout_step_struct, 'GPIOOutputStepStruct', 'extern', 'HeaderFile', 'MW_GPIOOUT.h');
                coder.ceval('GPIOOutput_step', coder.ref(gpioout_step_struct));
                
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');

                gpio_release_struct = struct('GPIOBaseAddress', obj.GPIOInstanceAddress, 'pinInstance', uint16(obj.GPIO_PinEnum));
                coder.cstructname(gpio_release_struct, 'GPIOOutputReleaseStruct', 'extern', 'HeaderFile', 'MW_GPIOOUT.h');
                coder.ceval('GPIOOutput_release', coder.ref(gpio_release_struct));
            end
        end
        
        %function flag = isInactivePropertyImpl(obj,propertyName)
        
         %   if strcmp(propertyName,'GPIO_PINS')
          %      flag = false;
           % else
            %    pin = extract(propertyName, digitsPattern);
             %   pindec = str2double(pin);
                % if pin is in range of GPIO_Group, then active
              %  min = (obj.GPIO_PINS) * 32;
               % max = ((obj.GPIO_PINS) * 32) + 31;
                %if (pindec>=min) && (pindec<=max)
                 %   flag = false;
               % else
                %    flag = true;
               % end
           % end
        
       % end
    end
    
    methods (Access=protected)
        %% Define input properties
        function num = getNumInputsImpl(~)
            num = 1;
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
            icon = {'GPIO_OUT'};
        end
    end
    
  
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'GPIO_OUT';
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
                % Use the following API's to add include files, sources and
                % linker flags
                addIncludeFiles(buildInfo,'MW_GPIOOUT.h',includeDir);
                addSourceFiles(buildInfo,'MW_GPIOOUT.c',srcDir);
                %addLinkFlags(buildInfo,{'-lSource'});
                %addLinkObjects(buildInfo,'sourcelib.a',srcDir);
                %addCompileFlags(buildInfo,{'-D_DEBUG=1'});
                %addDefines(buildInfo,'MY_DEFINE_1')
            end
        end
    end
end
