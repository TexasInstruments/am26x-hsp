
classdef CMPSS_AM263X < matlab.System & ...
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

        % High Comparator DAC Value
        COMPH_DAC_Value = 0;
        %Low Comparator DAC Value
        COMPL_DAC_Value = 0;
        %High Comparator DAC Value 2
        COMPH_DAC_Value2 = 0;
        %Low Comparator DAC Value 2
        COMPL_DAC_Value2 = 0;
        %Max Ramp Generator reference value
        Max_ramp_generator_ref_value = 0;
        %Ramp generator decrement value
        Ramp_gen_decrement_value = 0;
        %Ramp generator delay value
        Ramp_gen_delay_value = 0;
        

        
    end
    
    properties (Nontunable)

        %Sample Time
        SampleTime=0.1;
        %CMPSS Instance
        CMPSS_Instance = 'CMPSSA0';
        %Enable Module
        Enable_Module (1,1) logical = false;
        %Enable High Comparator
        Enable_High_Comparator (1,1) logical = false;
        %Enable Low Comparator
        Enable_Low_Comparator (1,1) logical = false;

        %High Comparator Configuration

        %Negative Input Source
        Negative_Input_Src = 'Internal DAC';
        %DAC Source Select
        DAC_Src = 'DAC module';
        %Invert High Comparator Output
        Invert_COMPH_Output (1,1) logical = false;
        %Async or Latch COMPH
        Async_Latch_COMPH (1,1) logical = false;
        %Signal driving CTRIPOUTH
        CTRIPOUTH = 'Asynchronous comparator output';
        %Signal driving CTRIPH
        CTRIPH = 'Asynchronous comparator output';
        %Specify DAC/RAMP parameters
        COMPH_Specify_DAC_params = 'Dialog';
        %DACH Initital Value
        COMPH_DAC1_Initial_Value = 0;
        
        %Digital Filter Configuration
        
        %Initialize digital filter
        Initialize_digital_filter_COMPH (1,1) logical = false;
        %Clear Filter Latch
        Clear_Filter_Latch_COMPH (1,1) logical = false;
        %Reset latch on EPWMSYNCPER
        Reset_Latch_EPWMSYNCPER_COMPH (1,1) logical = false;
        %Reset Digital Filter Latch
        Reset_Digital_Filter_Latch_COMPH (1,1) logical = false;
        %Digital Filter Sample Prescale
        Digital_Filter_Sample_Prescale_COMPH = 0;
        %Digital Filter Sample Window
        Digital_Filter_Sample_Window_COMPH = 1;
        %Digital Filter Threshold 
        Digital_Filter_Threshold_COMPH = 1;
       

        %Ramp Generator Configuration

        %Max Ramp Generator initial reference value
        Max_ramp_generator_ref_value_init = 0;
        %Ramp generator initial decrement value
        Ramp_gen_decrement_value_init = 0;
        %Ramp generator initial delay value
        Ramp_gen_delay_value_init = 0;
        %EPWMSYNCPER Source Number
        EPWMSYNCPER = 'EPWM1SYNCPER';
        %Ramp generator reset
        Ramp_gen_reset = 'Load from shadow register';

        %Low Comparator Configuration

        %Positive Input Source
        Positive_Input_Source = 'External pin INL';
        %Output is inverted
        Output_Inverted_COMPL (1,1) logical = false;
        %Asynch or Latch
        Asynch_Latch_COMPL (1,1) logical = false;
        %Signal driving CTRIPOUTL
        Signal_CTRIPOUTL = 'Asynchronous comparator output';
        %Signal driving CTRIPL
        Signal_CTRIPL = 'Asynchronous comparator output';
        %Specify DAC parameters
        COMPL_Specify_DAC_params = 'Dialog';
        %DACL Initital Value
        COMPL_DAC1_Initial_Value = 0;

        %Digital Filter Configuration

        %Initialize digital filter
        Initialize_digital_filter_COMPL (1,1) logical = false;
        %Clear Filter Latch
        Clear_Filter_Latch_COMPL (1,1) logical = false;
        %Reset latch on EPWMSYNCPER
        Reset_Latch_EPWMSYNCPER_COMPL (1,1) logical = false;
        %Reset Digital Filter Latch
        Reset_Digital_Filter_Latch_COMPL (1,1) logical = false;
        %Digital Filter Sample Prescale
        Digital_Filter_Sample_Prescale_COMPL = 0;
        %Digital Filter Sample Window
        Digital_Filter_Sample_Window_COMPL = 1;
        %Digital Filter Threshold 
        Digital_Filter_Threshold_COMPL = 1;

        %DAC Configuration

        %DAC Value Load
        DAC_Value_Load = 'DAC Value updated from SYSCLK';
        %DAC reference voltage
        DAC_Reference_Voltage = 'VDDA is reference voltage';
        %DAC value source
        DAC_Value_Source = 'DAC value updated from shadow register';
        %Enable Diode Emulation Mode
        Diode_Emulation_Enable (1,1) logical = false;
        %Diode emulation DEACTIVE Source
        Diode_emulation_deactive_source = 'EPWM0.DEACTIVE';

        %Hysterisis for High Comparator
        COMPH_Hysterisis = '0 LSB when comparator output is 1';
        %Hysterisis for Low Comparator
        COMPL_Hysterisis = '0 LSB when comparator output is 1';
        %Blanking signal
        Blanking_signal = 'EPWM1BLANK';
        %Enable blanking signal
        Enable_blanking_signal (1,1) logical = false;

        %Enable Latch CMPSS Output
        Enable_Latch_Output (1,1) logical = false;

    end
    
    properties (Access = private)
        % Pre-computed constants.
        baseAddr = 0;
    end

    properties (Constant, Hidden)
        CMPSS_InstanceSet = matlab.system.StringSet({'CMPSSA0', 'CMPSSA1', 'CMPSSA2', 'CMPSSA3', 'CMPSSA4', 'CMPSSA5', 'CMPSSA6', 'CMPSSA7', 'CMPSSA8', 'CMPSSA9', 'CMPSSB0', 'CMPSSB1', 'CMPSSB2', 'CMPSSB3', 'CMPSSB4', 'CMPSSB5', 'CMPSSB6', 'CMPSSB7', 'CMPSSB8', 'CMPSSB9'});
        Negative_Input_SrcSet = matlab.system.StringSet({'Internal DAC', 'External pin INL'});
        CTRIPOUTHSet = matlab.system.StringSet({'Asynchronous comparator output', 'Synchronous comparator output', 'Filter output', 'Latched filter output'});
        CTRIPHSet = matlab.system.StringSet({'Asynchronous comparator output', 'Synchronous comparator output', 'Filter output', 'Latched filter output'});
        EPWMSYNCPERSet = matlab.system.StringSet({'EPWM1SYNCPER', 'EPWM2SYNCPER', 'EPWM3SYNCPER', 'EPWM4SYNCPER', 'EPWM5SYNCPER', 'EPWM6SYNCPER', 'EPWM7SYNCPER', 'EPWM8SYNCPER', 'EPWM9SYNCPER', 'EPWM10SYNCPER', 'EPWM11SYNCPER', 'EPWM12SYNCPER', 'EPWM13SYNCPER', 'EPWM14SYNCPER', 'EPWM15SYNCPER', 'EPWM16SYNCPER', 'EPWM17SYNCPER', 'EPWM18SYNCPER', 'EPWM19SYNCPER', 'EPWM20SYNCPER', 'EPWM21SYNCPER', 'EPWM22SYNCPER', 'EPWM23SYNCPER', 'EPWM24SYNCPER', 'EPWM25SYNCPER', 'EPWM26SYNCPER', 'EPWM27SYNCPER', 'EPWM28SYNCPER', 'EPWM29SYNCPER', 'EPWM30SYNCPER', 'EPWM31SYNCPER', 'EPWM32SYNCPER'});
        Ramp_gen_resetSet = matlab.system.StringSet({'Load from shadow register', 'Load from latched value'});
        Positive_Input_SourceSet = matlab.system.StringSet({'External pin INL', 'External pin INH'});
        Signal_CTRIPOUTLSet = matlab.system.StringSet({'Asynchronous comparator output', 'Synchronous comparator outpt', 'Filter output', 'Latched filter output'});
        Signal_CTRIPLSet = matlab.system.StringSet({'Asynchronous comparator output', 'Synchronous comparator outpt', 'Filter output', 'Latched filter output'});
        DAC_Value_LoadSet = matlab.system.StringSet({'DAC Value updated from SYSCLK', 'DAC Value updated from EPWMSYNCPER'});
        DAC_Reference_VoltageSet = matlab.system.StringSet({'VDDA is reference voltage', 'VDAC is reference voltage'});
        DAC_Value_SourceSet = matlab.system.StringSet({'DAC value updated from shadow register', 'DAC value updated from ramp register'});
        Diode_emulation_deactive_sourceSet = matlab.system.StringSet({'EPWM0.DEACTIVE', 'EPWM1.DEACTIVE', 'EPWM2.DEACTIVE', 'EPWM3.DEACTIVE', 'EPWM4.DEACTIVE', 'EPWM5.DEACTIVE', 'EPWM6.DEACTIVE', 'EPWM7.DEACTIVE', 'EPWM8.DEACTIVE', 'EPWM9.DEACTIVE', 'EPWM10.DEACTIVE', 'EPWM11.DEACTIVE', 'EPWM12.DEACTIVE', 'EPWM13.DEACTIVE', 'EPWM14.DEACTIVE', 'EPWM15.DEACTIVE', 'EPWM16.DEACTIVE', 'EPWM17.DEACTIVE', 'EPWM18.DEACTIVE', 'EPWM19.DEACTIVE', 'EPWM20.DEACTIVE', 'EPWM21.DEACTIVE', 'EPWM22.DEACTIVE', 'EPWM23.DEACTIVE', 'EPWM24.DEACTIVE', 'EPWM25.DEACTIVE', 'EPWM26.DEACTIVE', 'EPWM27.DEACTIVE', 'EPWM28.DEACTIVE', 'EPWM29.DEACTIVE', 'EPWM30.DEACTIVE', 'EPWM31.DEACTIVE'});
        COMPH_HysterisisSet = matlab.system.StringSet({'0 LSB when comparator output is 1', '17.5 LSB when comparator output is 1', '35 LSB when comparator output is 1', '52.2 LSB when comparator output is 1', '0 LSB when comparator output is 0', '17.5 LSB when comparator output is 0', '35 LSB when comparator output is 0', '52.2 LSB when comparator output is 0'});
        COMPL_HysterisisSet = matlab.system.StringSet({'0 LSB when comparator output is 1', '17.5 LSB when comparator output is 1', '35 LSB when comparator output is 1', '52.2 LSB when comparator output is 1', '0 LSB when comparator output is 0', '17.5 LSB when comparator output is 0', '35 LSB when comparator output is 0', '52.2 LSB when comparator output is 0'});
        Blanking_signalSet = matlab.system.StringSet({'EPWM1BLANK', 'EPWM2BLANK', 'EPWM3BLANK', 'EPWM4BLANK', 'EPWM5BLANK', 'EPWM6BLANK', 'EPWM7BLANK', 'EPWM8BLANK', 'EPWM9BLANK', 'EPWM10BLANK', 'EPWM11BLANK', 'EPWM12BLANK', 'EPWM13BLANK', 'EPWM14BLANK', 'EPWM15BLANK', 'EPWM16BLANK', 'EPWM17BLANK', 'EPWM18BLANK', 'EPWM19BLANK', 'EPWM20BLANK', 'EPWM21BLANK', 'EPWM22BLANK', 'EPWM23BLANK', 'EPWM24BLANK', 'EPWM25BLANK', 'EPWM26BLANK', 'EPWM27BLANK', 'EPWM28BLANK', 'EPWM29BLANK', 'EPWM30BLANK', 'EPWM31BLANK', 'EPWM32BLANK'});
        DAC_SrcSet = matlab.system.StringSet({'DAC module', 'Ramp module'});
        COMPH_Specify_DAC_paramsSet = matlab.system.StringSet({'Dialog', 'Input port'});
        COMPL_Specify_DAC_paramsSet = matlab.system.StringSet({'Dialog', 'Input port'});
    end

    properties (Dependent, Access=protected)
        CMPSS_InstanceEnum;
        Negative_Input_SrcEnum;
        CTRIPOUTHEnum;
        CTRIPHEnum;
        EPWMSYNCPEREnum;
        Ramp_gen_resetEnum;
        Positive_Input_SourceEnum;
        Signal_CTRIPOUTLEnum;
        Signal_CTRIPLEnum;
        DAC_Value_LoadEnum;
        DAC_Reference_VoltageEnum;
        DAC_Value_SourceEnum;
        Diode_emulation_deactive_sourceEnum;
        COMPH_HysterisisEnum;
        COMPL_HysterisisEnum;
        Blanking_signalEnum;
        DAC_SrcEnum;
        COMPH_Specify_DAC_paramsEnum;
        COMPL_Specify_DAC_paramsEnum;
    end
    
    methods
        % Constructor
        function obj = CMPSS_AM263X(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.CMPSS_InstanceEnum(obj)
            if isequal(obj.CMPSS_Instance, 'CMPSSA0')
                ret = uint16(0);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA1')
                ret = uint16(1);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA2')
                ret = uint16(2);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA3')
                ret = uint16(3);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA4')
                ret = uint16(4);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA5')
                ret = uint16(5);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA6')
                ret = uint16(6);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA7')
                ret = uint16(7);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA8')
                ret = uint16(8);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSA9')
                ret = uint16(9);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB0')
                ret = uint16(10);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB1')
                ret = uint16(11);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB2')
                ret = uint16(12);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB3')
                ret = uint16(13);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB4')
                ret = uint16(14);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB5')
                ret = uint16(15);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB6')
                ret = uint16(16);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB7')
                ret = uint16(17);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB8')
                ret = uint16(18);
            elseif isequal(obj.CMPSS_Instance, 'CMPSSB9')
                ret = uint16(19);
            else
                ret = uint16(0);
            end 
        end

            function ret = get.Negative_Input_SrcEnum(obj)
                if isequal(obj.Negative_Input_Src, 'Internal DAC')
                    ret = uint16(0);
                elseif isequal(obj.Negative_Input_Src, 'External pin INL')
                    ret = uint16(1);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.CTRIPOUTHEnum(obj)
                if isequal(obj.CTRIPOUTH, 'Asynchronous comparator output')
                    ret = uint16(0);
                elseif isequal(obj.CTRIPOUTH, 'Synchronous comparator output')
                    ret = uint16(16);
                elseif isequal(obj.CTRIPOUTH, 'Filter output')
                    ret = uint16(32);
                elseif isequal(obj.CTRIPOUTH, 'Latched filter output')
                    ret = uint16(48);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.CTRIPHEnum(obj)
                if isequal(obj.CTRIPH, 'Asynchronous comparator output')
                    ret = uint16(0);
                elseif isequal(obj.CTRIPH, 'Synchronous comparator output')
                    ret = uint16(4);
                elseif isequal(obj.CTRIPH, 'Filter output')
                    ret = uint16(8);
                elseif isequal(obj.CTRIPH, 'Latched filter output')
                    ret = uint16(12);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.EPWMSYNCPEREnum(obj)
                if isequal(obj.EPWMSYNCPER, 'EPWM1SYNCPER')
                    ret = uint16(1);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM2SYNCPER')
                    ret = uint16(2);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM3SYNCPER')
                    ret = uint16(3);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM4SYNCPER')
                    ret = uint16(4);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM5SYNCPER')
                    ret = uint16(5);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM6SYNCPER')
                    ret = uint16(6);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM7SYNCPER')
                    ret = uint16(7);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM8SYNCPER')
                    ret = uint16(8);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM9SYNCPER')
                    ret = uint16(9);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM10SYNCPER')
                    ret = uint16(10);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM11SYNCPER')
                    ret = uint16(11);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM12SYNCPER')
                    ret = uint16(12);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM13SYNCPER')
                    ret = uint16(13);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM14SYNCPER')
                    ret = uint16(14);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM15SYNCPER')
                    ret = uint16(15);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM16SYNCPER')
                    ret = uint16(16);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM17SYNCPER')
                    ret = uint16(17);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM18SYNCPER')
                    ret = uint16(18);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM19SYNCPER')
                    ret = uint16(19);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM20SYNCPER')
                    ret = uint16(20);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM21SYNCPER')
                    ret = uint16(21);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM22SYNCPER')
                    ret = uint16(22);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM23SYNCPER')
                    ret = uint16(23);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM24SYNCPER')
                    ret = uint16(24);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM25SYNCPER')
                    ret = uint16(25);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM26SYNCPER')
                    ret = uint16(26);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM27SYNCPER')
                    ret = uint16(27);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM28SYNCPER')
                    ret = uint16(28);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM29SYNCPER')
                    ret = uint16(29);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM30SYNCPER')
                    ret = uint16(30);
                elseif isequal(obj.EPWMSYNCPER, 'EPWM31SYNCPER')
                    ret = uint16(31);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Ramp_gen_resetEnum(obj)
                if isequal(obj.Ramp_gen_reset, 'Load from shadow register')
                    ret = uint16(0);
                elseif isequal(obj.Ramp_gen_reset, 'Load from latched value')
                    ret = uint16(1);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Positive_Input_SourceEnum(obj)
                if isequal(obj.Positive_Input_Source, 'External pin INL')
                    ret = uint16(0);
                elseif isequal(obj.Positive_Input_Source, 'External pin INH')
                    ret = uint16(1);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Signal_CTRIPOUTLEnum(obj)
                if isequal(obj.Signal_CTRIPOUTL, 'Asynchronous comparator output')
                    ret = uint16(0);
                elseif isequal(obj.Signal_CTRIPOUTL, 'Synchronous comparator output')
                    ret = uint16(16);
                elseif isequal(obj.Signal_CTRIPOUTL, 'Filter output')
                    ret = uint16(32);
                elseif isequal(obj.Signal_CTRIPOUTL, 'Latched filter output')
                    ret = uint16(48);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Signal_CTRIPLEnum(obj)
                if isequal(obj.Signal_CTRIPL, 'Asynchronous comparator output')
                    ret = uint16(0);
                elseif isequal(obj.Signal_CTRIPL, 'Synchronous comparator output')
                    ret = uint16(4);
                elseif isequal(obj.Signal_CTRIPL, 'Filter output')
                    ret = uint16(8);
                elseif isequal(obj.Signal_CTRIPL, 'Latched filter output')
                    ret = uint16(12);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.DAC_Value_LoadEnum(obj)
                if isequal(obj.DAC_Value_Load, 'DAC Value updated from SYSCLK')
                    ret = uint16(0);
                elseif isequal(obj.DAC_Value_Load, 'DAC Value updated from EPWMSYNCPER')
                    ret = uint16(128);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.DAC_Reference_VoltageEnum(obj)
                if isequal(obj.DAC_Reference_Voltage, 'VDDA is reference voltage')
                    ret = uint16(0);
                elseif isequal(obj.DAC_Reference_Voltage, 'VDAC is reference voltage')
                    ret = uint16(32);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.DAC_Value_SourceEnum(obj)
                if isequal(obj.DAC_Value_Source, 'DAC value updated from shadow register')
                    ret = uint16(0);
                elseif isequal(obj.DAC_Value_Source, 'DAC value updated from ramp register')
                    ret = uint16(1);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Diode_emulation_deactive_sourceEnum(obj)
                if isequal(obj.Diode_emulation_deactive_source, 'EPWM0.DEACTIVE')
                    ret = uint16(0);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM1.DEACTIVE')
                    ret = uint16(1);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM2.DEACTIVE')
                    ret = uint16(2);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM3.DEACTIVE')
                    ret = uint16(3);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM4.DEACTIVE')
                    ret = uint16(4);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM5.DEACTIVE')
                    ret = uint16(5);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM6.DEACTIVE')
                    ret = uint16(6);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM7.DEACTIVE')
                    ret = uint16(7);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM8.DEACTIVE')
                    ret = uint16(8);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM9.DEACTIVE')
                    ret = uint16(9);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM10.DEACTIVE')
                    ret = uint16(10);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM11.DEACTIVE')
                    ret = uint16(11);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM12.DEACTIVE')
                    ret = uint16(12);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM13.DEACTIVE')
                    ret = uint16(13);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM14.DEACTIVE')
                    ret = uint16(14);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM15.DEACTIVE')
                    ret = uint16(15);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM16.DEACTIVE')
                    ret = uint16(16);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM17.DEACTIVE')
                    ret = uint16(17);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM18.DEACTIVE')
                    ret = uint16(18);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM19.DEACTIVE')
                    ret = uint16(19);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM20.DEACTIVE')
                    ret = uint16(20);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM21.DEACTIVE')
                    ret = uint16(21);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM22.DEACTIVE')
                    ret = uint16(22);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM23.DEACTIVE')
                    ret = uint16(23);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM24.DEACTIVE')
                    ret = uint16(24);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM25.DEACTIVE')
                    ret = uint16(25);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM26.DEACTIVE')
                    ret = uint16(26);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM27.DEACTIVE')
                    ret = uint16(27);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM28.DEACTIVE')
                    ret = uint16(28);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM29.DEACTIVE')
                    ret = uint16(29);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM30.DEACTIVE')
                    ret = uint16(30);
                elseif isequal(obj.Diode_emulation_deactive_source, 'EPWM31.DEACTIVE')
                    ret = uint16(31);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.COMPH_HysterisisEnum(obj)
                if isequal(obj.COMPH_Hysterisis, '0 LSB when comparator output is 1')
                    ret = uint16(0);
                elseif isequal(obj.COMPH_Hysterisis, '17.5 LSB when comparator output is 1')
                    ret = uint16(1);
                elseif isequal(obj.COMPH_Hysterisis, '35 LSB when comparator output is 1')
                    ret = uint16(2);
                elseif isequal(obj.COMPH_Hysterisis, '52.5 LSB when comparator output is 1')
                    ret = uint16(3);
                elseif isequal(obj.COMPH_Hysterisis, '0 LSB when comparator output is 0')
                    ret = uint16(4);
                elseif isequal(obj.COMPH_Hysterisis, '17.5 LSB when comparator output is 0')
                    ret = uint16(5);
                elseif isequal(obj.COMPH_Hysterisis, '35 LSB when comparator output is 0')
                    ret = uint16(6);
                elseif isequal(obj.COMPH_Hysterisis, '52.5 LSB when comparator output is 0')
                    ret = uint16(7);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.COMPL_HysterisisEnum(obj)
                if isequal(obj.COMPL_Hysterisis, '0 LSB when comparator output is 1')
                    ret = uint16(0);
                elseif isequal(obj.COMPL_Hysterisis, '17.5 LSB when comparator output is 1')
                    ret = uint16(1);
                elseif isequal(obj.COMPL_Hysterisis, '35 LSB when comparator output is 1')
                    ret = uint16(2);
                elseif isequal(obj.COMPL_Hysterisis, '52.5 LSB when comparator output is 1')
                    ret = uint16(3);
                elseif isequal(obj.COMPL_Hysterisis, '0 LSB when comparator output is 0')
                    ret = uint16(4);
                elseif isequal(obj.COMPL_Hysterisis, '17.5 LSB when comparator output is 0')
                    ret = uint16(5);
                elseif isequal(obj.COMPL_Hysterisis, '35 LSB when comparator output is 0')
                    ret = uint16(6);
                elseif isequal(obj.COMPL_Hysterisis, '52.5 LSB when comparator output is 0')
                    ret = uint16(7);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.Blanking_signalEnum(obj)
                if isequal(obj.Blanking_signal, 'EPWM1BLANK')
                    ret = uint16(1);
                elseif isequal(obj.Blanking_signal, 'EPWM2BLANK')
                    ret = uint16(2);
                elseif isequal(obj.Blanking_signal, 'EPWM3BLANK')
                    ret = uint16(3);
                elseif isequal(obj.Blanking_signal, 'EPWM4BLANK')
                    ret = uint16(4);
                elseif isequal(obj.Blanking_signal, 'EPWM5BLANK')
                    ret = uint16(5);
                elseif isequal(obj.Blanking_signal, 'EPWM6BLANK')
                    ret = uint16(6);
                elseif isequal(obj.Blanking_signal, 'EPWM7BLANK')
                    ret = uint16(7);
                elseif isequal(obj.Blanking_signal, 'EPWM8BLANK')
                    ret = uint16(8);
                elseif isequal(obj.Blanking_signal, 'EPWM9BLANK')
                    ret = uint16(9);
                elseif isequal(obj.Blanking_signal, 'EPWM10BLANK')
                    ret = uint16(10);
                elseif isequal(obj.Blanking_signal, 'EPWM11BLANK')
                    ret = uint16(11);
                elseif isequal(obj.Blanking_signal, 'EPWM12BLANK')
                    ret = uint16(12);
                elseif isequal(obj.Blanking_signal, 'EPWM13BLANK')
                    ret = uint16(13);
                elseif isequal(obj.Blanking_signal, 'EPWM14BLANK')
                    ret = uint16(14);
                elseif isequal(obj.Blanking_signal, 'EPWM15BLANK')
                    ret = uint16(15);
                elseif isequal(obj.Blanking_signal, 'EPWM16BLANK')
                    ret = uint16(16);
                elseif isequal(obj.Blanking_signal, 'EPWM17BLANK')
                    ret = uint16(17);
                elseif isequal(obj.Blanking_signal, 'EPWM18BLANK')
                    ret = uint16(18);
                elseif isequal(obj.Blanking_signal, 'EPWM19BLANK')
                    ret = uint16(19);
                elseif isequal(obj.Blanking_signal, 'EPWM20BLANK')
                    ret = uint16(20);
                elseif isequal(obj.Blanking_signal, 'EPWM21BLANK')
                    ret = uint16(21);
                elseif isequal(obj.Blanking_signal, 'EPWM22BLANK')
                    ret = uint16(22);
                elseif isequal(obj.Blanking_signal, 'EPWM23BLANK')
                    ret = uint16(23);
                elseif isequal(obj.Blanking_signal, 'EPWM24BLANK')
                    ret = uint16(24);
                elseif isequal(obj.Blanking_signal, 'EPWM25BLANK')
                    ret = uint16(25);
                elseif isequal(obj.Blanking_signal, 'EPWM26BLANK')
                    ret = uint16(26);
                elseif isequal(obj.Blanking_signal, 'EPWM27BLANK')
                    ret = uint16(27);
                elseif isequal(obj.Blanking_signal, 'EPWM28BLANK')
                    ret = uint16(28);
                elseif isequal(obj.Blanking_signal, 'EPWM29BLANK')
                    ret = uint16(29);
                elseif isequal(obj.Blanking_signal, 'EPWM30BLANK')
                    ret = uint16(30);
                elseif isequal(obj.Blanking_signal, 'EPWM31BLANK')
                    ret = uint16(31);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.DAC_SrcEnum(obj)
                if isequal(obj.DAC_Src, 'DAC module')
                    ret = uint16(0);
                elseif isequal(obj.DAC_Src, 'Ramp module')
                    ret = uint16(1);
                else
                    ret = uint16(0);
                end
            end

            function ret = get.COMPH_Specify_DAC_paramsEnum(obj)
                if isequal(obj.COMPH_Specify_DAC_params, 'Dialog')
                    ret = uint8(0);
                elseif isequal(obj.COMPH_Specify_DAC_params, 'Input port')
                    ret = uint8(1);
                else
                    ret = uint8(0);
                end
            end

            function ret = get.COMPL_Specify_DAC_paramsEnum(obj)
                if isequal(obj.COMPL_Specify_DAC_params, 'Dialog')
                    ret = uint8(0);
                elseif isequal(obj.COMPL_Specify_DAC_params, 'Input port')
                    ret = uint8(1);
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
                coder.cinclude('MW_CMPSS.h');
                switch uint16(obj.CMPSS_InstanceEnum)
                    case 0
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA0_U_BASE');
                    case 1
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA1_U_BASE');
                    case 2
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA2_U_BASE');
                    case 3
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA3_U_BASE');
                    case 4
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA4_U_BASE');
                    case 5
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA5_U_BASE');
                    case 6
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA6_U_BASE');
                    case 7
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA7_U_BASE');
                    case 8
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA8_U_BASE');
                    case 9
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA9_U_BASE');
                    case 10
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB0_U_BASE');
                    case 11
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB1_U_BASE');
                    case 12
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB2_U_BASE');
                    case 13
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB3_U_BASE');
                    case 14
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB4_U_BASE');
                    case 15
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB5_U_BASE');
                    case 16
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB6_U_BASE');
                    case 17
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB7_U_BASE');
                    case 18
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB8_U_BASE');
                    case 19
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSB9_U_BASE');
                    otherwise
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_CMPSSA0_U_BASE');
                end
                   
                
                cmpss_setup_struct = struct('base', obj.baseAddr, 'neg_inp_src_high_comp', uint16(obj.Negative_Input_SrcEnum), 'pos_inp_src_low_comp', uint16(obj.Positive_Input_SourceEnum), 'output_inverted_high_comp', obj.Invert_COMPH_Output, 'asynch_high_comp', obj.Async_Latch_COMPH, 'output_inverted_low_comp', obj.Output_Inverted_COMPL, 'asynch_low_comp', obj.Asynch_Latch_COMPL, 'DAC_Load', uint16(obj.DAC_Value_LoadEnum), 'DAC_Ref_voltage', uint16(obj.DAC_Reference_VoltageEnum), 'DAC_value_src', uint16(obj.DAC_Value_SourceEnum), 'High_comp_digfilt_sample_prescale', uint16(obj.Digital_Filter_Sample_Prescale_COMPH), 'High_comp_digfilt_sample_window', uint16(obj.Digital_Filter_Sample_Window_COMPH), 'High_comp_digfilt_threshold', uint16(obj.Digital_Filter_Threshold_COMPH), 'Low_comp_digfilt_sample_prescale', uint16(obj.Digital_Filter_Sample_Prescale_COMPL), 'Low_comp_digfilt_sample_window', uint16(obj.Digital_Filter_Sample_Window_COMPL), 'Low_comp_digfilt_threshold', uint16(obj.Digital_Filter_Threshold_COMPL), 'signal_ctripouth_high_comp', uint16(obj.CTRIPOUTHEnum), 'signal_ctriph_high_comp', uint16(obj.CTRIPHEnum), 'signal_ctripoutl_low_comp', uint16(obj.Signal_CTRIPOUTLEnum), 'signal_ctripl_low_comp', uint16(obj.Signal_CTRIPLEnum), 'initialize_dig_high_filter', obj.Initialize_digital_filter_COMPH, 'initialize_dig_low_filter', obj.Initialize_digital_filter_COMPL, 'clear_filt_latch_high_filt', obj.Clear_Filter_Latch_COMPH, 'clear_filter_latch_low_filter', obj.Clear_Filter_Latch_COMPL, 'hysterisis_high_comp', uint16(obj.COMPH_HysterisisEnum), 'hysterisis_low_comp', uint16(obj.COMPL_HysterisisEnum), 'max_ramp_gen_ref_value', uint16(obj.Max_ramp_generator_ref_value), 'ramp_gen_dec_value', uint16(obj.Ramp_gen_decrement_value), 'ramp_gen_delay_value', uint16(obj.Ramp_gen_delay_value), 'pwmSync', uint16(obj.EPWMSYNCPEREnum), 'ramp_gen_reset_value', uint16(obj.Ramp_gen_resetEnum), 'pwm_blanking', uint16(obj.Blanking_signalEnum), 'enable_blanking', obj.Enable_blanking_signal, 'enable_diode_emulation', obj.Diode_Emulation_Enable, 'cmpss_deactive_signal', uint8(obj.Diode_emulation_deactive_sourceEnum), 'high_comp_dac_value2', uint16(obj.COMPH_DAC_Value2), 'low_comp_dac_value2', uint16(obj.COMPL_DAC_Value2), 'enable_module', obj.Enable_Module, 'reset_dig_filter_latch_high_comp', obj.Reset_Digital_Filter_Latch_COMPH, 'reset_dig_filter_latch_low_comp', obj.Reset_Digital_Filter_Latch_COMPL, 'specify_comph_dacramp', uint8(obj.COMPH_Specify_DAC_paramsEnum), 'specify_compl_dac', uint8(obj.COMPL_Specify_DAC_paramsEnum), 'High_comp_DAC_value', uint16(obj.COMPH_DAC_Value), 'Low_comp_DAC_value', uint16(obj.COMPL_DAC_Value), 'High_comp_DAC_value_init', uint16(obj.COMPH_DAC1_Initial_Value), 'Low_comp_DAC_value_init', uint16(obj.COMPL_DAC1_Initial_Value), 'max_ramp_gen_ref_value_init', uint16(obj.Max_ramp_generator_ref_value_init), 'ramp_gen_dec_value_init', uint16(obj.Ramp_gen_decrement_value_init), 'ramp_gen_delay_value_init', uint16(obj.Ramp_gen_delay_value_init), 'DAC_Src', uint8(obj.DAC_SrcEnum));
                coder.cstructname(cmpss_setup_struct, 'CMPSSSetupStruct', 'extern', 'HeaderFile', 'MW_CMPSS.h');
                coder.ceval('CMPSS_setup', coder.ref(cmpss_setup_struct));

                
            end
        end
        
        function [varargout] = stepImpl(obj, varargin)   %#ok<MANU>
            STS = uint16(0);
            STSL = uint16(0);
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                index = 1;
                 ramp_reset = true;

                 if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'DAC module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                     coder.ceval('CMPSS_setDACValueHigh', obj.baseAddr, uint16(varargin{index}));
                     index = index + 1;
                 elseif ((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'DAC module') && isequal(obj.COMPH_Specify_DAC_params, 'Dialog'))
                     coder.ceval('CMPSS_setDACValueHigh', obj.baseAddr, uint16(obj.COMPH_DAC_Value));
                 end
                 if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'Ramp module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                     if(obj.Ramp_gen_resetEnum == 0)
                         ramp_reset = true;
                     else
                         ramp_reset = false;
                     end
                     coder.ceval('CMPSS_configRamp', obj.baseAddr, uint16(varargin{index}), uint16(varargin{index+1}), uint16(varargin{index+2}), uint16(obj.EPWMSYNCPEREnum), ramp_reset);
                     index = index + 3;
                 elseif((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'Ramp module') && isequal(obj.COMPH_Specify_DAC_params, 'Dialog'))
                     if(obj.Ramp_gen_resetEnum == 0)
                         ramp_reset = true;
                     else
                         ramp_reset = false;
                     end
                     coder.ceval('CMPSS_configRamp', obj.baseAddr, uint16(obj.Max_ramp_generator_ref_value), uint16(obj.Ramp_gen_decrement_value), uint16(obj.Ramp_gen_delay_value), uint16(obj.EPWMSYNCPEREnum), ramp_reset);
                 end
                 if((obj.Enable_Low_Comparator == true) && isequal(obj.COMPL_Specify_DAC_params, 'Input port'))
                     coder.ceval('CMPSS_setDACValueLow', obj.baseAddr, uint16(varargin{index}))
                 elseif((obj.Enable_Low_Comparator == true) && isequal(obj.COMPL_Specify_DAC_params, 'Dialog'))
                     coder.ceval('CMPSS_setDACValueLow', obj.baseAddr, uint16(obj.COMPL_DAC_Value));
                 end

                 if(obj.Diode_Emulation_Enable)
                     coder.ceval('CMPSS_configHighDACShadowValue2', obj.baseaddr, uint16(obj.COMPH_DAC_Value2));
                     coder.ceval('CMPSS_configLowDACShadowValue2', obj.baseaddr, uint16(obj.COMPL_DAC_Value2));
                 end
            end

            cmpss_step_struct = struct('base', obj.baseAddr, 'Enable_high_comparator', obj.Enable_High_Comparator, 'Enable_low_comparator', obj.Enable_Low_Comparator);
            coder.cstructname(cmpss_step_struct, 'CMPSSStepStruct', 'extern', 'HeaderFile', 'MW_CMPSS.h');
            coder.ceval('CMPSS_step', coder.ref(cmpss_step_struct), coder.wref(STS), coder.wref(STSL));

            varargout{index} = uint16(STS);
            index = index + 1;
            if(obj.Enable_Latch_Output)
                varargout{index} = uint16(STSL);
            end

           
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                coder.ceval('CMPSS_disableModule', obj.baseAddr);
            end
        end
        
        function flag = isInactivePropertyImpl(obj,propertyName)
            if ((strcmp(propertyName,'Diode_emulation_deactive_source') || strcmp(propertyName,'COMPH_DAC_Value2') || strcmp(propertyName,'COMPL_DAC_Value2')) && obj.Diode_Emulation_Enable==false)
                flag=true;
            elseif ((strcmp(propertyName, 'Negative_Input_Src') | strcmp(propertyName, 'DAC_Src') | strcmp(propertyName, 'COMPH_Specify_DAC_params') | strcmp(propertyName, 'COMPH_DAC1_Initial_Value') | strcmp(propertyName, 'COMPH_DAC_Value') | strcmp(propertyName, 'Invert_COMPH_Output') | strcmp(propertyName, 'Async_Latch_COMPH') | strcmp(propertyName, 'CTRIPOUTH') | strcmp(propertyName, 'CTRIPH') | strcmp(propertyName, 'Initialize_digital_filter_COMPH') | strcmp(propertyName, 'Clear_Filter_Latch_COMPH') | strcmp(propertyName, 'Reset_Latch_EPWMSYNCPER_COMPH') | strcmp(propertyName, 'Digital_Filter_Sample_Prescale_COMPH') | strcmp(propertyName, 'Digital_Filter_Sample_Window_COMPH') | strcmp(propertyName, 'Digital_Filter_Threshold_COMPH') | strcmp(propertyName, 'Reset_Digital_Filter_Latch_COMPH') | strcmp(propertyName , 'Max_ramp_generator_ref_value') | strcmp(propertyName, 'Ramp_gen_decrement_value') | strcmp(propertyName, 'Ramp_gen_delay_value') | strcmp(propertyName, 'EPWMSYNCPER') | strcmp(propertyName, 'Ramp_gen_reset')) && (obj.Enable_High_Comparator == false))
                flag = true;
            elseif ((strcmp(propertyName, 'Positive_Input_Source') | strcmp(propertyName, 'COMPL_Specify_DAC_params') | strcmp(propertyName, 'COMPL_DAC1_Initial_Value') | strcmp(propertyName, 'COMPL_DAC_Value') | strcmp(propertyName, 'Output_Inverted_COMPL') | strcmp(propertyName, 'Asynch_Latch_COMPL') | strcmp(propertyName, 'Signal_CTRIPOUTL') | strcmp(propertyName, 'Signal_CTRIPL') | strcmp(propertyName, 'Initialize_digital_filter_COMPL') | strcmp(propertyName, 'Clear_Filter_Latch_COMPL') | strcmp(propertyName, 'Reset_Latch_EPWMSYNCPER_COMPL') | strcmp(propertyName, 'Digital_Filter_Sample_Prescale_COMPL') | strcmp(propertyName, 'Digital_Filter_Sample_Window_COMPL') | strcmp(propertyName, 'Digital_Filter_Threshold_COMPL') | strcmp(propertyName, 'Reset_Digital_Filter_Latch_COMPL') | strcmp(propertyName, 'COMPL_DAC_Value')) && (obj.Enable_Low_Comparator == false))
                flag = true;
            elseif ((strcmp(propertyName, 'DAC_Src') | strcmp(propertyName, 'COMPH_Specify_DAC_params')) && (isequal(obj.Negative_Input_Src, 'External pin INL')))
                flag = true;
            elseif ((strcmp(propertyName, 'COMPH_DAC1_Initial_Value') | strcmp(propertyName, 'COMPH_DAC_Value')) && (isequal(obj.Negative_Input_Src, 'External pin INL') | isequal(obj.DAC_Src, 'Ramp module')))
                flag = true;
            elseif ((strcmp(propertyName, 'Max_ramp_generator_ref_value') | strcmp(propertyName, 'Ramp_gen_decrement_value') | strcmp(propertyName, 'Ramp_gen_delay_value') | strcmp(propertyName, 'EPWMSYNCPER') | strcmp(propertyName, 'Ramp_gen_reset')) && (isequal(obj.Negative_Input_Src, 'External pin INL') | isequal(obj.DAC_Src, 'DAC module')))
                flag = true;
            elseif ((strcmp(propertyName, 'COMPL_DAC1_Initial_Value') && (isequal(obj.COMPL_Specify_DAC_params, 'Dialog'))))
                flag = true;
            elseif ((strcmp(propertyName, 'COMPH_DAC1_Initial_Value') && (isequal(obj.COMPH_Specify_DAC_params, 'Dialog'))))
                flag = true;
            elseif ((strcmp(propertyName, 'COMPL_DAC_Value') && (isequal(obj.COMPL_Specify_DAC_params, 'Input port'))))
                flag = true;
            elseif ((strcmp(propertyName, 'COMPH_DAC_Value') && (isequal(obj.COMPH_Specify_DAC_params, 'Input port'))))
                flag = true;
            else 
                flag= false;
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
                    'Title', 'General', ...
                    'PropertyList',  {'CMPSS_Instance','Enable_Module','COMPH_Hysterisis','COMPL_Hysterisis','Blanking_signal','Enable_blanking_signal', 'Enable_High_Comparator', 'Enable_Low_Comparator', 'Enable_Latch_Output', 'SampleTime'});
            
            High_Comparator_Configuration = matlab.system.display.SectionGroup(...
                'Title','High Comparator Configuration',...
                'PropertyList',{'Negative_Input_Src', 'DAC_Src', 'COMPH_Specify_DAC_params', 'COMPH_DAC1_Initial_Value', 'COMPH_DAC_Value', 'Invert_COMPH_Output', 'Async_Latch_COMPH', 'CTRIPOUTH', 'CTRIPH', 'Initialize_digital_filter_COMPH', 'Clear_Filter_Latch_COMPH', 'Reset_Latch_EPWMSYNCPER_COMPH', 'Digital_Filter_Sample_Prescale_COMPH', 'Digital_Filter_Sample_Window_COMPH', 'Digital_Filter_Threshold_COMPH', 'Reset_Digital_Filter_Latch_COMPH', 'Max_ramp_generator_ref_value', 'Ramp_gen_decrement_value', 'Ramp_gen_delay_value', 'EPWMSYNCPER', 'Ramp_gen_reset'});
            
            Low_Comparator_Configuration=matlab.system.display.SectionGroup(...
               'Title','Low Comparator Configuration',...
               'PropertyList',{'Positive_Input_Source',  'COMPL_Specify_DAC_params', 'COMPL_DAC1_Initial_Value', 'COMPL_DAC_Value', 'Output_Inverted_COMPL', 'Asynch_Latch_COMPL', 'Signal_CTRIPOUTL', 'Signal_CTRIPL', 'Initialize_digital_filter_COMPL', 'Clear_Filter_Latch_COMPL', 'Reset_Latch_EPWMSYNCPER_COMPL', 'Digital_Filter_Sample_Prescale_COMPL', 'Digital_Filter_Sample_Window_COMPL', 'Digital_Filter_Threshold_COMPL', 'Reset_Digital_Filter_Latch_COMPL'});
            
            DAC_Configuration = matlab.system.display.SectionGroup(...
                'Title', 'DAC Configuration',...
                'PropertyList',{'DAC_Value_Load', 'DAC_Reference_Voltage', 'DAC_Value_Source', 'Diode_Emulation_Enable', 'Diode_emulation_deactive_source', 'COMPH_DAC_Value2', 'COMPL_DAC_Value2'});
            
            groups = [General, High_Comparator_Configuration, Low_Comparator_Configuration, DAC_Configuration];
            
        end
        
        function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','Type 4 CMPSS',...
                'Text', sprintf('%s\n\n%s','Configures the CMPSS modules (COMPH or COMPL) to output the comparison result on the comparator pins.'),...
                'ShowSourceLink', false);
        end
    end
    
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(obj)
            num = 0;
            if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'DAC module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                num = num + 1;
            end
            if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'Ramp module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                num = num + 3;
            end
            if((obj.Enable_Low_Comparator == true) && isequal(obj.COMPL_Specify_DAC_params, 'Input port'))
                num = num + 1;
            end

           
        end
        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
            if num > 0
                varargout = cell(1,num);
                if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'DAC module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                    varargout{index} = 'DACH';
                    index = index + 1;
                end
                if((obj.Enable_High_Comparator == true) && isequal(obj.Negative_Input_Src, 'Internal DAC') && isequal(obj.DAC_Src, 'Ramp module') && isequal(obj.COMPH_Specify_DAC_params, 'Input port'))
                    varargout{index} = 'REF';
                    index = index + 1;
                    varargout{index} = 'DEC';
                    index = index + 1;
                    varargout{index} = 'DLY';
                    index = index + 1;
                end
                if((obj.Enable_Low_Comparator == true) && isequal(obj.COMPL_Specify_DAC_params, 'Input port'))
                    varargout{index} = 'DACL';
                end

            end
        end
        function num = getNumOutputsImpl(obj)
            num = 1;
            if(obj.Enable_Latch_Output)
                num = num + 1;
            end
        end
        
        function varargout = isOutputFixedSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              varargout{indx} = true;
              indx = indx + 1;
              if(obj.Enable_Latch_Output)
                  varargout{indx} = true;
              end
            end
        end

        function varargout = getOutputNamesImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              varargout{indx} = 'STS';
              indx = indx + 1;
              if(obj.Enable_Latch_Output)
                  varargout{indx} = 'STSL';
              end
            end
        end

        
        
        function varargout = isOutputComplexImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              varargout{indx} = false;
              indx = indx + 1;
              if(obj.Enable_Latch_Output)
                  varargout{indx} = false;
              end
            end
        end
        
        function varargout = getOutputSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              varargout{indx} = [1,1];
              indx = indx + 1;
              if(obj.Enable_Latch_Output)
                  varargout{indx} = [1,1];
              end
            end
        end
        
        function varargout = getOutputDataTypeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              varargout{indx} = 'uint16';
              indx = indx + 1;
              if(obj.Enable_Latch_Output)
                  varargout{indx} = 'uint16';
              end
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'CMPSS', 'Type 4'};
        end
        
    end
    
   
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'CMPSS';
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
                addIncludeFiles(buildInfo, 'MW_CMPSS.h', includeDir);
                addSourceFiles(buildInfo, 'MW_CMPSS.c', srcDir);
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

