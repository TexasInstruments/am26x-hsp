
classdef ECAP_AM261X < matlab.System & ...
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
        
        %Event Prescaler
        Event_Prescaler = 0;
        %APWM Period
        APWM_Period = 0;
        %APWM Compare
        APWM_Compare = 0;
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.
        
        %Sample Time
        SampleTime=0.001;
        %ECAP Instance
        ECAP_Instance = 'ECAP0';
        %Emulation Mode
        Emulation_Mode = 'TSCTR is stopped on emulation suspension';
        %ECAP Mode
        ECAP_Mode = 'Capture';
        %Capture Mode
        Capture_Mode = 'ECAP operates in continuous capture mode';
        %Capture stops at event
        Capture_stop_event = 'eCAP event 1';
        %Event 1 Polarity
        Event1_Polarity = 'Rising edge polarity';
        %Event 2 Polarity
        Event2_Polarity = 'Rising edge polarity';
        %Event 3 Polarity
        Event3_Polarity = 'Rising edge polarity';
        %Event 4 Polarity
        Event4_Polarity = 'Rising edge polarity';
        %Use Interrupt
        Use_Interrupt (1,1) logical = false;
        %Enable Interrupt
        Enable_Interrupt (1,1) logical = false;
        %Enable ECAP Event 1 Interrupt
        Event1_Interrupt (1,1) logical = false;
        %Enable ECAP Event 2 Interrupt
        Event2_Interrupt (1,1) logical = false;
        %Enable ECAP Event 3 Interrupt
        Event3_Interrupt (1,1) logical = false;
        %Enable ECAP Event 4 Interrupt
        Event4_Interrupt (1,1) logical = false;
        %Enable Interrupt due to counter overflow
        Counter_overflow_Interrupt (1,1) logical = false;
        %Enable Counter equals to period Interrupt
        Counter_period_Interrupt (1,1) logical = false;
        %Enable Counter equals to compare Interrupt
        Counter_compare_Interrupt (1,1) logical = false;
        %ECAP Event 1 counter reset
        Enable_ECAP_Event1_Counter_Reset (1,1) logical = false;
        %ECAP Event 2 counter reset
        Enable_ECAP_Event2_Counter_Reset (1,1) logical = false;
        %ECAP Event 3 counter reset
        Enable_ECAP_Event3_Counter_Reset (1,1) logical = false;
        %ECAP Event 4 counter reset
        Enable_ECAP_Event4_Counter_Reset (1,1) logical = false;
        %Phase Shift Count
        Phase_Shift_Count_Value = 0;
        %Enable Load Counter
        Enable_Load_Counter (1,1) logical = false;
        %Load Counter
        Load_Counter (1,1) logical = false;
        %Sync Out Mode
        Sync_Out_Mode = 'Sync out on sync in signal and software force';
        %ECAP Input
        ECAP_Input = 'Capture input is FSI_RX0 Trigger 0';
        %Filter Out Pulses
        Filter_Out_Pulses = 'ECAP Pulse width filter bypass';
        %Reset Counters
        Reset_Counter_Enable (1,1) logical = false;
        %Sync-in pulse source
        Sync_in_pulse_src = 'Disable sync-in';
        %Re-arm ECAP
        Re_ARM_ECAP (1,1) logical = false;
        %Data type
        DataType = 'Double';
        %APWM Polarity
        APWM_Polarity = 'APWM is active high';
        %Specify APWM Period
        APWM_Period_Specify = 'Specify via Dialog Box';
        %Specify APWM Compare
        APWM_Compare_Specify = 'Specify via Dialog Box';
        %APWM Period Initial
        APWM_Period_Initial = 0;
        %APWM Compare Initial
        APWM_Compare_Initial = 0;
        %Enable capture event status flag
        Capture_event_status (1,1) logical = false;
        %Enable overflow status flag
        Overflow_status (1,1) logical = false;


        %Signal moitoring unit

        %Trip Selection
        Trip_Selection = 'Disabled';
        %Global Load Strobe
        Global_Load_Strobe = 'Disabled';

        %ECAP Monitoring Unit 1

        %Enable Monitoring Unit 1
        Enable_Monitoring_Unit1 (1,1) logical = false;
        %Select monitoring type of MUNIT 1
        Type_MUNIT_Monitoring1 = 'High pulse width';
        %Minimum value for monitoring unit 1
        Min_value_monitoring1 = 0;
        %Maximum value for monitoring unit 1
        Max_value_monitoring1 = 0;
        %Enable Sync-in for monitoring unit 1
        Enable_syncin_monitoring1 (1,1) logical = false;
        %Force copy from shadow monitoring unit 1
        Force_copy_shadow_monitoring1 (1,1) logical = false;
        %Select shadow lode mode monitoring unit 1
        Shadow_load_monitoring1 = 'Load on next sync event';
        %Shadow minimum value for monitoring unit 1
        Shadow_min_value_monitoring1 = 0;
        %Shadow maximum value for monitoring unit 1
        Shadow_max_value_monitoring1 = 0;
        %Enable Debug Mode for monitoring unit 1
        Enable_debug_monitoring1 (1,1) logical = false;

        %ECAP Monitoring Unit 2

        %Enable Monitoring Unit 2
        Enable_Monitoring_Unit2 (1,1) logical = false;
        %Select monitoring type of MUNIT 2
        Type_MUNIT_Monitoring2 = 'High pulse width';
        %Minimum value for monitoring unit 2
        Min_value_monitoring2 = 0;
        %Maximum value for monitoring unit 2
        Max_value_monitoring2 = 0;
        %Enable Sync-in for monitoring unit 2
        Enable_syncin_monitoring2 (1,1) logical = false;
        %Force copy from shadow monitoring unit 2
        Force_copy_shadow_monitoring2 (1,1) logical = false;
        %Select shadow lode mode monitoring unit 2
        Shadow_load_monitoring2 = 'Load on next sync event';
        %Shadow minimum value for monitoring unit 2
        Shadow_min_value_monitoring2 = 0;
        %Shadow maximum value for monitoring unit 2
        Shadow_max_value_monitoring2 = 0;
        %Enable Debug Mode for monitoring unit 2
        Enable_debug_monitoring2 (1,1) logical = false;


    end
    
    properties (Constant, Hidden)
        ECAP_InstanceSet = matlab.system.StringSet({'ECAP0', 'ECAP1', 'ECAP2', 'ECAP3', 'ECAP4', 'ECAP5', 'ECAP6', 'ECAP7'});
        Emulation_ModeSet = matlab.system.StringSet({'TSCTR is stopped on emulation suspension', 'TSCTR runs until 0 before stopping on emulation suspension', 'TSCTR is not affected by emulation suspension'});
        ECAP_ModeSet = matlab.system.StringSet({'Capture', 'APWM'});
        Capture_ModeSet = matlab.system.StringSet({'ECAP operates in continuous capture mode', 'ECAP operates in one shot capture mode'});
        Capture_stop_eventSet = matlab.system.StringSet({'eCAP event 1', 'eCAP event 2', 'eCAP event 3', 'eCAP event 4'});
        DataTypeSet = matlab.system.StringSet({'Double', 'Single', 'INT8', 'Unsigned INT8', 'INT16', 'Unsigned INT16', 'INT32', 'Unsigned INT32'});
        Event1_PolaritySet = matlab.system.StringSet({'Rising edge polarity', 'Falling edge polarity'});
        Event2_PolaritySet = matlab.system.StringSet({'Rising edge polarity', 'Falling edge polarity'});
        Event3_PolaritySet = matlab.system.StringSet({'Rising edge polarity', 'Falling edge polarity'});
        Event4_PolaritySet = matlab.system.StringSet({'Rising edge polarity', 'Falling edge polarity'});
        Sync_Out_ModeSet = matlab.system.StringSet({'Sync out on sync in signal and software force', 'Sync out on counter equals period', 'Disable sync out signal'});
        ECAP_InputSet = matlab.system.StringSet({'Capture input is FSI_RX0 Trigger 0', 'Capture input is FSI_RX0 Trigger 1', 'Capture input is FSI_RX0 Trigger 2', 'Capture input is FSI_RX0 Trigger 3', 'Capture input is EQEP0 QI signal', 'Capture input is EQEP0 QS signal', 'Capture input is EQEP1 QI signal', 'Capture input is EQEP1 QS signal', 'Capture input is EPWM0 Del Active signal', 'Capture input is EPWM1 Del Active signal', 'Capture input is EPWM2 Del Active signal', 'Capture input is EPWM3 Del Active signal', 'Capture input is EPWM4 Del Active signal', 'Capture input is EPWM5 Del Active signal', 'Capture input is EPWM6 Del Active signal', 'Capture input is EPWM7 Del Active signal', 'Capture input is EPWM8 Del Active signal', 'Capture input is EPWM9 Del Active signal', 'Capture input is EPWM0 SOCA signal', 'Capture input is EPWM1 SOCA signal', 'Capture input is EPWM2 SOCA signal', 'Capture input is EPWM3 SOCA signal', 'Capture input is EPWM4 SOCA signal', 'Capture input is EPWM5 SOCA signal', 'Capture input is EPWM6 SOCA signal', 'Capture input is EPWM7 SOCA signal', 'Capture input is EPWM8 SOCA signal', 'Capture input is EPWM9 SOCA signal', 'Capture input is EPWM0 SOCB signal', 'Capture input is EPWM1 SOCB signal', 'Capture input is EPWM2 SOCB signal', 'Capture input is EPWM3 SOCB signal', 'Capture input is EPWM4 SOCB signal', 'Capture input is EPWM5 SOCB signal', 'Capture input is EPWM6 SOCB signal', 'Capture input is EPWM7 SOCB signal', 'Capture input is EPWM8 SOCB signal', 'Capture input is EPWM9 SOCB signal', 'Capture input is SDFM0 Compare1 High', 'Capture input is SDFM0 Compare1 Low', 'Capture input is SDFM0 Compare Z1', 'Capture input is SDFM0 Compare2 High', 'Capture input is SDFM0 Compare2 Low', 'Capture input is SDFM0 Compare Z2', 'Capture input is SDFM0 Compare3 High', 'Capture input is SDFM0 Compare3 Low', 'Capture input is SDFM0 Compare Z3', 'Capture input is SDFM0 Compare4 High', 'Capture input is SDFM0 Compare4 Low', 'Capture input is SDFM0 Compare Z4', 'Capture input is SDFM1 Compare1 High', 'Capture input is SDFM1 Compare1 Low', 'Capture input is SDFM1 Compare Z1', 'Capture input is SDFM1 Compare2 High', 'Capture input is SDFM1 Compare2 Low', 'Capture input is SDFM1 Compare Z2', 'Capture input is SDFM1 Compare3 High', 'Capture input is SDFM1 Compare3 Low', 'Capture input is SDFM1 Compare Z3', 'Capture input is SDFM1 Compare4 High', 'Capture input is SDFM1 Compare4 Low', 'Capture input is SDFM1 Compare Z4',  'Capture input is CMPSSA0 CTRIP low', 'Capture input is CMPSSA0 CTRIP high', 'Capture input is CMPSSA1 CTRIP low', 'Capture input is CMPSSA1 CTRIP high', 'Capture input is CMPSSA2 CTRIP low', 'Capture input is CMPSSA2 CTRIP high', 'Capture input is CMPSSA3 CTRIP low', 'Capture input is CMPSSA3 CTRIP high', 'Capture input is CMPSSA4 CTRIP low', 'Capture input is CMPSSA4 CTRIP high', 'Capture input is CMPSSA5 CTRIP low', 'Capture input is CMPSSA5 CTRIP high', 'Capture input is CMPSSA6 CTRIP low', 'Capture input is CMPSSA6 CTRIP high', 'Capture input is CMPSSA7 CTRIP low', 'Capture input is CMPSSA7 CTRIP high', 'Capture input is CMPSSA8 CTRIP low', 'Capture input is CMPSSA8 CTRIP high', 'Capture input is ADC0 Event 0', 'Capture input is ADC0 Event 1', 'Capture input is ADC0 Event 2', 'Capture input is ADC0 Event 3', 'Capture input is ADC1 Event 0', 'Capture input is ADC1 Event 1', 'Capture input is ADC1 Event 2', 'Capture input is ADC1 Event 3', 'Capture input is ADC2 Event 0', 'Capture input is ADC2 Event 1', 'Capture input is ADC2 Event 2', 'Capture input is ADC2 Event 3', 'Capture input is ADC4 Event 0', 'Capture input is InputXBAR Output 0 signal', 'Capture input is InputXBAR Output 1 signal', 'Capture input is InputXBAR Output 2 signal', 'Capture input is InputXBAR Output 3 signal', 'Capture input is InputXBAR Output 4 signal', 'Capture input is InputXBAR Output 5 signal', 'Capture input is InputXBAR Output 6 signal', 'Capture input is InputXBAR Output 7 signal', 'Capture input is InputXBAR Output 8 signal', 'Capture input is InputXBAR Output 9 signal', 'Capture input is InputXBAR Output 10 signal', 'Capture input is InputXBAR Output 11 signal', 'Capture input is InputXBAR Output 12 signal', 'Capture input is InputXBAR Output 13 signal', 'Capture input is InputXBAR Output 14 signal', 'Capture input is InputXBAR Output 15 signal', 'Capture input is InputXBAR Output 16 signal', 'Capture input is InputXBAR Output 17 signal', 'Capture input is InputXBAR Output 18 signal', 'Capture input is InputXBAR Output 19 signal', 'Capture input is InputXBAR Output 20 signal', 'Capture input is InputXBAR Output 21 signal', 'Capture input is InputXBAR Output 22 signal', 'Capture input is InputXBAR Output 23 signal', 'Capture input is InputXBAR Output 24 signal', 'Capture input is InputXBAR Output 25 signal', 'Capture input is InputXBAR Output 26 signal', 'Capture input is InputXBAR Output 27 signal', 'Capture input is InputXBAR Output 28 signal', 'Capture input is InputXBAR Output 29 signal', 'Capture input is InputXBAR Output 30 signal', 'Capture input is InputXBAR Output 31 signal'});
        Filter_Out_PulsesSet = matlab.system.StringSet({'ECAP Pulse width filter bypass', 'ECAP Pulse width filter cycle 1', 'ECAP Pulse width filter cycle 2', 'ECAP Pulse width filter cycle 3', 'ECAP Pulse width filter cycle 4', 'ECAP Pulse width filter cycle 5', 'ECAP Pulse width filter cycle 6', 'ECAP Pulse width filter cycle 7', 'ECAP Pulse width filter cycle 8', 'ECAP Pulse width filter cycle 9', 'ECAP Pulse width filter cycle 10', 'ECAP Pulse width filter cycle 11', 'ECAP Pulse width filter cycle 12', 'ECAP Pulse width filter cycle 13', 'ECAP Pulse width filter cycle 14', 'ECAP Pulse width filter cycle 15'});
        Sync_in_pulse_srcSet = matlab.system.StringSet({'Disable sync-in', 'Sync-in source is EPWM 0 syncout signal', 'Sync-in source is EPWM 1 syncout signal', 'Sync-in source is EPWM 2 syncout signal', 'Sync-in source is EPWM 3 syncout signal', 'Sync-in source is EPWM 4 syncout signal', 'Sync-in source is EPWM 5 syncout signal', 'Sync-in source is EPWM 6 syncout signal', 'Sync-in source is EPWM 7 syncout signal', 'Sync-in source is EPWM 8 syncout signal', 'Sync-in source is EPWM 9 syncout signal'});
        APWM_PolaritySet = matlab.system.StringSet({'APWM is active high', 'APWM is active low'});
        APWM_Period_SpecifySet = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        APWM_Compare_SpecifySet = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        Trip_SelectionSet = matlab.system.StringSet({'Disabled', 'PWM XBAR OUT0', 'PWM XBAR OUT1', 'PWM XBAR OUT2', 'PWM XBAR OUT3', 'PWM XBAR OUT4', 'PWM XBAR OUT5', 'PWM XBAR OUT6', 'PWM XBAR OUT7', 'PWM XBAR OUT8', 'PWM XBAR OUT9', 'PWM XBAR OUT10', 'PWM XBAR OUT11', 'PWM XBAR OUT12', 'PWM XBAR OUT13', 'PWM XBAR OUT14', 'PWM XBAR OUT15', 'PWM XBAR OUT16', 'PWM XBAR OUT17', 'PWM XBAR OUT18', 'PWM XBAR OUT19', 'PWM XBAR OUT20', 'PWM XBAR OUT21', 'PWM XBAR OUT22', 'PWM XBAR OUT23', 'PWM XBAR OUT24', 'PWM XBAR OUT25', 'PWM XBAR OUT26', 'PWM XBAR OUT27', 'PWM XBAR OUT28', 'PWM XBAR OUT29', 'EPWM0 TRIPOUT', 'EPWM1 TRIPOUT', 'EPWM2 TRIPOUT', 'EPWM3 TRIPOUT', 'EPWM4 TRIPOUT', 'EPWM5 TRIPOUT', 'EPWM6 TRIPOUT', 'EPWM7 TRIPOUT', 'EPWM8 TRIPOUT', 'EPWM9 TRIPOUT'});
        Global_Load_StrobeSet = matlab.system.StringSet({'Disabled','EPWM0 Global Load Strobe', 'EPWM1 Global Load Strobe', 'EPWM2 Global Load Strobe', 'EPWM3 Global Load Strobe', 'EPWM4 Global Load Strobe', 'EPWM5 Global Load Strobe', 'EPWM6 Global Load Strobe', 'EPWM7 Global Load Strobe', 'EPWM8 Global Load Strobe', 'EPWM9 Global Load Strobe', 'EPWM10 Global Load Strobe'});
        Type_MUNIT_Monitoring1Set = matlab.system.StringSet({'High pulse width', 'Low pulse width', 'Period width from rise to rise', 'Period width from fall to fall', 'Monitor rise edge', 'Monitor fall edge'});
        Shadow_load_monitoring1Set = matlab.system.StringSet({'Load on next sync event', 'Load on EPWM GLDLCSTRB Event'});
        Type_MUNIT_Monitoring2Set = matlab.system.StringSet({'High pulse width', 'Low pulse width', 'Period width from rise to rise', 'Period width from fall to fall', 'Monitor rise edge', 'Monitor fall edge'});
        Shadow_load_monitoring2Set = matlab.system.StringSet({'Load on next sync event', 'Load on EPWM GLDLCSTRB Event'});
    end

    properties (Dependent, Access=protected)
        ECAP_InstanceEnum;
        Emulation_ModeEnum;
        ECAP_ModeEnum;
        Capture_ModeEnum;
        Capture_stop_eventEnum;
        Event1_PolarityEnum;
        Event2_PolarityEnum;
        Event3_PolarityEnum;
        Event4_PolarityEnum;
        Sync_Out_ModeEnum;
        ECAP_InputEnum;
        Filter_Out_PulsesEnum;
        Sync_in_pulse_srcEnum;
        APWM_PolarityEnum;
        APWM_Period_SpecifyEnum;
        APWM_Compare_SpecifyEnum;
        Trip_SelectionEnum;
        Global_Load_StrobeEnum;
        Type_MUNIT_Monitoring1Enum;
        Shadow_load_monitoring1Enum;
        Type_MUNIT_Monitoring2Enum;
        Shadow_load_monitoring2Enum;
        DataTypeEnum;
    end
    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
        module_instance = 0;
    end
    
    methods
        % Constructor
        function obj = ECAP_AM261X(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.ECAP_InstanceEnum(obj)
            if(isequal(obj.ECAP_Instance, 'ECAP0'))
                ret = uint16(0);
            elseif(isequal(obj.ECAP_Instance, 'ECAP1'))
                ret = uint16(1);
            elseif(isequal(obj.ECAP_Instance, 'ECAP2'))
                ret = uint16(2);
            elseif(isequal(obj.ECAP_Instance, 'ECAP3'))
                ret = uint16(3);
            elseif(isequal(obj.ECAP_Instance, 'ECAP4'))
                ret = uint16(4);
            elseif(isequal(obj.ECAP_Instance, 'ECAP5'))
                ret = uint16(5);
            elseif(isequal(obj.ECAP_Instance, 'ECAP6'))
                ret = uint16(6);
            elseif(isequal(obj.ECAP_Instance, 'ECAP7'))
                ret = uint16(7);
            elseif(isequal(obj.ECAP_Instance, 'ECAP8'))
                ret = uint16(8);
            elseif(isequal(obj.ECAP_Instance, 'ECAP9'))
                ret = uint16(9);
            elseif(isequal(obj.ECAP_Instance, 'ECAP10'))
                ret = uint16(10);
            elseif(isequal(obj.ECAP_Instance, 'ECAP11'))
                ret = uint16(11);
            elseif(isequal(obj.ECAP_Instance, 'ECAP12'))
                ret = uint16(12);
            elseif(isequal(obj.ECAP_Instance, 'ECAP13'))
                ret = uint16(13);
            elseif(isequal(obj.ECAP_Instance, 'ECAP14'))
                ret = uint16(14);
            elseif(isequal(obj.ECAP_Instance, 'ECAP15'))
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Emulation_ModeEnum(obj)
            if(isequal(obj.Emulation_Mode, 'TSCTR is stopped on emulation suspension'))
                ret = 0x0;
            elseif(isequal(obj.Emulation_Mode, 'TSCTR runs until 0 before stopping on emulation suspension'))
                ret = 0x1;
            elseif(isequal(obj.Emulation_Mode, 'TSCTR is not affected by emulation suspension'))
                ret = 0x2;
            else
                ret = 0x0;
            end
        end

        function ret = get.ECAP_ModeEnum(obj)
            if(isequal(obj.ECAP_Mode, 'Capture'))
                ret = uint16(0);
            elseif(isequal(obj.ECAP_Mode, 'APWM'))
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Capture_ModeEnum(obj)
            if(isequal(obj.Capture_Mode, 'ECAP operates in continuous capture mode'))
                ret = uint16(0);
            elseif(isequal(obj.Capture_Mode, 'ECAP operates in one shot capture mode'))
                ret = uint16(1);
            end
        end

        function ret = get.Capture_stop_eventEnum(obj)
            if(isequal(obj.Capture_stop_event, 'eCAP event 1'))
                ret = uint16(0);
            elseif(isequal(obj.Capture_stop_event, 'eCAP event 2'))
                ret = uint16(1);
            elseif(isequal(obj.Capture_stop_event, 'eCAP event 3'))
                ret = uint16(2);
            elseif(isequal(obj.Capture_stop_event, 'eCAP event 4'))
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Event1_PolarityEnum(obj)
            if(isequal(obj.Event1_Polarity, 'Rising edge polarity'))
                ret = uint16(0);
            elseif(isequal(obj.Event1_Polarity, 'Falling edge polarity'))
                ret = uint16(1);
            end
        end

        function ret = get.Event2_PolarityEnum(obj)
            if(isequal(obj.Event2_Polarity, 'Rising edge polarity'))
                ret = uint16(0);
            elseif(isequal(obj.Event2_Polarity, 'Falling edge polarity'))
                ret = uint16(1);
            end
        end

        function ret = get.Event3_PolarityEnum(obj)
            if(isequal(obj.Event3_Polarity, 'Rising edge polarity'))
                ret = uint16(0);
            elseif(isequal(obj.Event3_Polarity, 'Falling edge polarity'))
                ret = uint16(1);
            end
        end

        function ret = get.Event4_PolarityEnum(obj)
            if(isequal(obj.Event4_Polarity, 'Rising edge polarity'))
                ret = uint16(0);
            elseif(isequal(obj.Event4_Polarity, 'Falling edge polarity'))
                ret = uint16(1);
            end
        end

        function ret = get.Sync_Out_ModeEnum(obj)
            if(isequal(obj.Sync_Out_Mode, 'Sync out on sync in signal and software force'))
                ret = 0x00;
            elseif(isequal(obj.Sync_Out_Mode, 'Sync out on counter equals period'))
                ret = 0x40;
            elseif(isequal(obj.Sync_Out_Mode, 'Disable sync out signal'))
                ret = 0x80;
            end
        end

        function ret = get.ECAP_InputEnum(obj)
            if(isequal(obj.ECAP_Input, 'Capture input is FSI_RX0 Trigger 0'))
                ret = uint16(0);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX0 Trigger 1'))
                ret = uint16(1);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX0 Trigger 2'))
                ret = uint16(2);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX0 Trigger 3'))
                ret = uint16(3);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX1 Trigger 0'))
                ret = uint16(4);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX1 Trigger 1'))
                ret = uint16(5);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX1 Trigger 2'))
                ret = uint16(6);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX1 Trigger 3'))
                ret = uint16(7);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX2 Trigger 0'))
                ret = uint16(8);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX2 Trigger 1'))
                ret = uint16(9);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX2 Trigger 2'))
                ret = uint16(10);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX2 Trigger 3'))
                ret = uint16(11);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX3 Trigger 0'))
                ret = uint16(12);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX3 Trigger 1'))
                ret = uint16(13);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX3 Trigger 2'))
                ret = uint16(14);
            elseif(isequal(obj.ECAP_Input, 'Capture input is FSI_RX3 Trigger 3'))
                ret = uint16(15);
            elseif(isequal(obj.ECAP_Input,  'Capture input is EQEP0 QI signal'))
                ret = uint16(16);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EQEP0 QS signal'))
                ret = uint16(17);
            elseif(isequal(obj.ECAP_Input,  'Capture input is EQEP1 QI signal'))
                ret = uint16(18);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EQEP1 QS signal'))
                ret = uint16(19);
            elseif(isequal(obj.ECAP_Input,  'Capture input is EQEP2 QI signal'))
                ret = uint16(20);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EQEP2 QS signal'))
                ret = uint16(21);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM0 Del Active signal'))
                ret = uint16(22);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM1 Del Active signal'))
                ret = uint16(23);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM2 Del Active signal'))
                ret = uint16(24);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM3 Del Active signal'))
                ret = uint16(25);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM4 Del Active signal'))
                ret = uint16(26);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM5 Del Active signal'))
                ret = uint16(27);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM6 Del Active signal'))
                ret = uint16(28);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM7 Del Active signal'))
                ret = uint16(29);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM8 Del Active signal'))
                ret = uint16(30);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM9 Del Active signal'))
                ret = uint16(31);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM10 Del Active signal'))
                ret = uint16(32);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM11 Del Active signal'))
                ret = uint16(33);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM12 Del Active signal'))
                ret = uint16(34);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM13 Del Active signal'))
                ret = uint16(35);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM14 Del Active signal'))
                ret = uint16(36);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM15 Del Active signal'))
                ret = uint16(37);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM16 Del Active signal'))
                ret = uint16(38);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM17 Del Active signal'))
                ret = uint16(39);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM18 Del Active signal'))
                ret = uint16(40);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM19 Del Active signal'))
                ret = uint16(41);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM20 Del Active signal'))
                ret = uint16(42);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM21 Del Active signal'))
                ret = uint16(43);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM22 Del Active signal'))
                ret = uint16(44);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM23 Del Active signal'))
                ret = uint16(45);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM24 Del Active signal'))
                ret = uint16(46);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM25 Del Active signal'))
                ret = uint16(47);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM26 Del Active signal'))
                ret = uint16(48);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM27 Del Active signal'))
                ret = uint16(49);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM28 Del Active signal'))
                ret = uint16(50);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM29 Del Active signal'))
                ret = uint16(51);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM30 Del Active signal'))
                ret = uint16(52);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM31 Del Active signal'))
                ret = uint16(53);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM0 SOCA signal'))
                ret = uint16(54);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM1 SOCA signal'))
                ret = uint16(55);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM2 SOCA signal'))
                ret = uint16(56);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM3 SOCA signal'))
                ret = uint16(57);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM4 SOCA signal'))
                ret = uint16(58);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM5 SOCA signal'))
                ret = uint16(59);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM6 SOCA signal'))
                ret = uint16(60);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM7 SOCA signal'))
                ret = uint16(61);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM8 SOCA signal'))
                ret = uint16(62);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM9 SOCA signal'))
                ret = uint16(63);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM10 SOCA signal'))
                ret = uint16(64);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM11 SOCA signal'))
                ret = uint16(65);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM12 SOCA signal'))
                ret = uint16(66);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM13 SOCA signal'))
                ret = uint16(67);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM14 SOCA signal'))
                ret = uint16(68);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM15 SOCA signal'))
                ret = uint16(69);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM16 SOCA signal'))
                ret = uint16(70);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM17 SOCA signal'))
                ret = uint16(71);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM18 SOCA signal'))
                ret = uint16(72);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM19 SOCA signal'))
                ret = uint16(73);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM20 SOCA signal'))
                ret = uint16(74);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM21 SOCA signal'))
                ret = uint16(75);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM22 SOCA signal'))
                ret = uint16(76);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM23 SOCA signal'))
                ret = uint16(77);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM24 SOCA signal'))
                ret = uint16(78);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM25 SOCA signal'))
                ret = uint16(79);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM26 SOCA signal'))
                ret = uint16(80);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM27 SOCA signal'))
                ret = uint16(81);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM28 SOCA signal'))
                ret = uint16(82);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM29 SOCA signal'))
                ret = uint16(83);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM30 SOCA signal'))
                ret = uint16(84);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM31 SOCA signal'))
                ret = uint16(85);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM0 SOCB signal'))
                ret = uint16(86);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM1 SOCB signal'))
                ret = uint16(87);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM2 SOCB signal'))
                ret = uint16(88);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM3 SOCB signal'))
                ret = uint16(89);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM4 SOCB signal'))
                ret = uint16(90);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM5 SOCB signal'))
                ret = uint16(91);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM6 SOCB signal'))
                ret = uint16(92);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM7 SOCB signal'))
                ret = uint16(93);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM8 SOCB signal'))
                ret = uint16(94);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM9 SOCB signal'))
                ret = uint16(95);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM10 SOCB signal'))
                ret = uint16(96);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM11 SOCB signal'))
                ret = uint16(97);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM12 SOCB signal'))
                ret = uint16(98);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM13 SOCB signal'))
                ret = uint16(99);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM14 SOCB signal'))
                ret = uint16(100);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM15 SOCB signal'))
                ret = uint16(101);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM16 SOCB signal'))
                ret = uint16(102);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM17 SOCB signal'))
                ret = uint16(103);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM18 SOCB signal'))
                ret = uint16(104);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM19 SOCB signal'))
                ret = uint16(105);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM20 SOCB signal'))
                ret = uint16(106);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM21 SOCB signal'))
                ret = uint16(107);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM22 SOCB signal'))
                ret = uint16(108);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM23 SOCB signal'))
                ret = uint16(109);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM24 SOCB signal'))
                ret = uint16(110);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM25 SOCB signal'))
                ret = uint16(111);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM26 SOCB signal'))
                ret = uint16(112);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM27 SOCB signal'))
                ret = uint16(113);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM28 SOCB signal'))
                ret = uint16(114);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM29 SOCB signal'))
                ret = uint16(115);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM30 SOCB signal'))
                ret = uint16(116);
            elseif(isequal(obj.ECAP_Input, 'Capture input is EPWM31 SOCB signal'))
                ret = uint16(117);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare1 High'))
                ret = uint16(118);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare1 Low'))
                ret = uint16(119);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare Z1'))
                ret = uint16(120);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare2 High'))
                ret = uint16(121);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare2 Low'))
                ret = uint16(122);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare Z2'))
                ret = uint16(123);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare3 High'))
                ret = uint16(124);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare3 Low'))
                ret = uint16(125);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare Z3'))
                ret = uint16(126);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare4 High'))
                ret = uint16(127);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare4 Low'))
                ret = uint16(128);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM0 Compare Z4'))
                ret = uint16(129);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare1 High'))
                ret = uint16(130);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare1 Low'))
                ret = uint16(131);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare Z1'))
                ret = uint16(132);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare2 High'))
                ret = uint16(133);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare2 Low'))
                ret = uint16(134);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare Z2'))
                ret = uint16(135);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare3 High'))
                ret = uint16(136);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare3 Low'))
                ret = uint16(137);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare Z3'))
                ret = uint16(138);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare4 High'))
                ret = uint16(139);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare4 Low'))
                ret = uint16(140);
            elseif(isequal(obj.ECAP_Input, 'Capture input is SDFM1 Compare Z4'))
                ret = uint16(141);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA0 CTRIP low'))
                ret = uint16(142);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA0 CTRIP high'))
                ret = uint16(143);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA1 CTRIP low'))
                ret = uint16(144);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA1 CTRIP high'))
                ret = uint16(145);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA2 CTRIP low'))
                ret = uint16(146);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA2 CTRIP high'))
                ret = uint16(147);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA3 CTRIP low'))
                ret = uint16(148);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA3 CTRIP high'))
                ret = uint16(149);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA4 CTRIP low'))
                ret = uint16(150);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA4 CTRIP high'))
                ret = uint16(151);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA5 CTRIP low'))
                ret = uint16(152);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA5 CTRIP high'))
                ret = uint16(153);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA6 CTRIP low'))
                ret = uint16(154);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA6 CTRIP high'))
                ret = uint16(155);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA7 CTRIP low'))
                ret = uint16(156);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA7 CTRIP high'))
                ret = uint16(157);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA8 CTRIP low'))
                ret = uint16(158);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA8 CTRIP high'))
                ret = uint16(159);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA9 CTRIP low'))
                ret = uint16(160);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSA9 CTRIP high'))
                ret = uint16(161);
             elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB0 CTRIP low'))
                ret = uint16(162);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB0 CTRIP high'))
                ret = uint16(163);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB1 CTRIP low'))
                ret = uint16(164);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB1 CTRIP high'))
                ret = uint16(165);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB2 CTRIP low'))
                ret = uint16(166);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB2 CTRIP high'))
                ret = uint16(167);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB3 CTRIP low'))
                ret = uint16(168);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB3 CTRIP high'))
                ret = uint16(169);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB4 CTRIP low'))
                ret = uint16(170);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB4 CTRIP high'))
                ret = uint16(171);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB5 CTRIP low'))
                ret = uint16(172);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB5 CTRIP high'))
                ret = uint16(173);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB6 CTRIP low'))
                ret = uint16(174);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB6 CTRIP high'))
                ret = uint16(175);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB7 CTRIP low'))
                ret = uint16(176);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB7 CTRIP high'))
                ret = uint16(177);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB8 CTRIP low'))
                ret = uint16(178);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB8 CTRIP high'))
                ret = uint16(179);   
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB9 CTRIP low'))
                ret = uint16(180);
            elseif(isequal(obj.ECAP_Input, 'Capture input is CMPSSB9 CTRIP high'))
                ret = uint16(181); 
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC0 Event 0'))
                ret = uint16(182);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC0 Event 1'))
                ret = uint16(183);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC0 Event 2'))
                ret = uint16(184);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC0 Event 3'))
                ret = uint16(185);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC1 Event 0'))
                ret = uint16(186);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC1 Event 1'))
                ret = uint16(187);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC1 Event 2'))
                ret = uint16(188);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC1 Event 3'))
                ret = uint16(189);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC2 Event 0'))
                ret = uint16(190);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC2 Event 1'))
                ret = uint16(191);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC2 Event 2'))
                ret = uint16(192);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC2 Event 3'))
                ret = uint16(193);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC3 Event 0'))
                ret = uint16(194);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC3 Event 1'))
                ret = uint16(195);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC3 Event 2'))
                ret = uint16(196);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC3 Event 3'))
                ret = uint16(197);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC4 Event 0'))
                ret = uint16(198);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC4 Event 1'))
                ret = uint16(199);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC4 Event 2'))
                ret = uint16(200);
            elseif(isequal(obj.ECAP_Input, 'Capture input is ADC4 Event 3'))
                ret = uint16(201);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 0 signal'))
                ret = uint16(202);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 1 signal'))
                ret = uint16(203);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 2 signal'))
                ret = uint16(204);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 3 signal'))
                ret = uint16(205);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 4 signal'))
                ret = uint16(206);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 5 signal'))
                ret = uint16(207);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 6 signal'))
                ret = uint16(208);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 7 signal'))
                ret = uint16(209);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 8 signal'))
                ret = uint16(210);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 9 signal'))
                ret = uint16(211);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 10 signal'))
                ret = uint16(212);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 11 signal'))
                ret = uint16(213);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 12 signal'))
                ret = uint16(214);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 13 signal'))
                ret = uint16(215);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 14 signal'))
                ret = uint16(216);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 15 signal'))
                ret = uint16(217);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 16 signal'))
                ret = uint16(218);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 17 signal'))
                ret = uint16(219);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 18 signal'))
                ret = uint16(220);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 19 signal'))
                ret = uint16(221);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 20 signal'))
                ret = uint16(222);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 21 signal'))
                ret = uint16(223);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 22 signal'))
                ret = uint16(224);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 23 signal'))
                ret = uint16(225);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 24 signal'))
                ret = uint16(226);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 25 signal'))
                ret = uint16(227);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 26 signal'))
                ret = uint16(228);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 27 signal'))
                ret = uint16(229);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 28 signal'))
                ret = uint16(230);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 29 signal'))
                ret = uint16(231);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 30 signal'))
                ret = uint16(232);
            elseif(isequal(obj.ECAP_Input, 'Capture input is InputXBAR Output 31 signal'))
                ret = uint16(233);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter_Out_PulsesEnum(obj)
            if(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter bypass'))
                ret = 0x0;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 1'))
                ret = 0x1;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 2'))
                ret = 0x2;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 3'))
                ret = 0x3;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 4'))
                ret = 0x4;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 5'))
                ret = 0x5;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 6'))
                ret = 0x6;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 7'))
                ret = 0x7;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 8'))
                ret = 0x8;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 9'))
                ret = 0x9;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 10'))
                ret = 0x10;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 11'))
                ret = 0x11;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 12'))
                ret = 0x12;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 13'))
                ret = 0x13;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 14'))
                ret = 0x14;
            elseif(isequal(obj.Filter_Out_Pulses, 'ECAP Pulse width filter cycle 15'))
                ret = 0x15;
            else
                ret = 0x0;
            end   
        end

        function ret = get.Sync_in_pulse_srcEnum(obj)
            if(isequal(obj.Sync_in_pulse_src, 'Disable sync-in'))
                ret = 0x0;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 0 syncout signal'))
                ret = 0x1;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 1 syncout signal'))
                ret = 0x2;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 2 syncout signal'))
                ret = 0x3;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 3 syncout signal'))
                ret = 0x4;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 4 syncout signal'))
                ret = 0x5;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 5 syncout signal'))
                ret = 0x6;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 6 syncout signal'))
                ret = 0x7;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 7 syncout signal'))
                ret = 0x8;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 8 syncout signal'))
                ret = 0x9;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 9 syncout signal'))
                ret = 0xA;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 10 syncout signal'))
                ret = 0XB;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 11 syncout signal'))
                ret = 0XC;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 12 syncout signal'))
                ret = 0XD;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 13 syncout signal'))
                ret = 0XE;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 14 syncout signal'))
                ret = 0XF;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 15 syncout signal'))
                ret = 0X10;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 16 syncout signal'))
                ret = 0X11;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 17 syncout signal'))
                ret = 0X12;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 18 syncout signal'))
                ret = 0X13;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 19 syncout signal'))
                ret = 0X14;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 20 syncout signal'))
                ret = 0X15;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 21 syncout signal'))
                ret = 0X16;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 22 syncout signal'))
                ret = 0X17;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 23 syncout signal'))
                ret = 0X18;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 24 syncout signal'))
                ret = 0X19;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 25 syncout signal'))
                ret = 0X1A;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 26 syncout signal'))
                ret = 0X1B;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 27 syncout signal'))
                ret = 0X1C;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 28 syncout signal'))
                ret = 0X1D;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 29 syncout signal'))
                ret = 0X1E;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 30 syncout signal'))
                ret = 0X1F;
            elseif(isequal(obj.Sync_in_pulse_src, 'Sync-in source is EPWM 31 syncout signal'))
                ret = 0X20;
            else
                ret = 0x0;
            end
        end

        function ret = get.APWM_PolarityEnum(obj)
            if(isequal(obj.APWM_Polarity, 'APWM is active high'))
                ret = 0x000;
            elseif(isequal(obj.APWM_Polarity, 'APWM is active low'))
                ret = 0x400;
            end
        end

        function ret = get.Trip_SelectionEnum(obj)
            if(isequal(obj.Trip_Selection, 'Disabled'))
                ret = uint16(0);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT0'))
                ret = uint16(1);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT1'))
                ret = uint16(2);
            elseif(isequal(ob3.Trip_Selection, 'PWM XBAR OUT2'))
                ret = uint16(3);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT3'))
                ret = uint16(4);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT4'))
                ret = uint16(5);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT5'))
                ret = uint16(6);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT6'))
                ret = uint16(7);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT7'))
                ret = uint16(8);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT8'))
                ret = uint16(9);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT9'))
                ret = uint16(10);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT10'))
                ret = uint16(11);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT11'))
                ret = uint16(12);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT12'))
                ret = uint16(13);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT13'))
                ret = uint16(14);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT14'))
                ret = uint16(15);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT15'))
                ret = uint16(16);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT16'))
                ret = uint16(17);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT17'))
                ret = uint16(18);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT18'))
                ret = uint16(19);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT19'))
                ret = uint16(20);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT20'))
                ret = uint16(21);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT21'))
                ret = uint16(22);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT22'))
                ret = uint16(23);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT23'))
                ret = uint16(24);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT24'))
                ret = uint16(25);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT25'))
                ret = uint16(26);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT26'))
                ret = uint16(27);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT27'))
                ret = uint16(28);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT28'))
                ret = uint16(29);
            elseif(isequal(obj.Trip_Selection, 'PWM XBAR OUT29'))
                ret = uint16(30);
            elseif(isequal(obj.Trip_Selection, 'EPWM0 TRIPOUT'))
                ret = uint16(32);
            elseif(isequal(obj.Trip_Selection, 'EPWM1 TRIPOUT'))
                ret = uint16(33);
            elseif(isequal(obj.Trip_Selection, 'EPWM2 TRIPOUT'))
                ret = uint16(34);
            elseif(isequal(obj.Trip_Selection, 'EPWM3 TRIPOUT'))
                ret = uint16(35);
            elseif(isequal(obj.Trip_Selection, 'EPWM4 TRIPOUT'))
                ret = uint16(36);
            elseif(isequal(obj.Trip_Selection, 'EPWM5 TRIPOUT'))
                ret = uint16(37);
            elseif(isequal(obj.Trip_Selection, 'EPWM6 TRIPOUT'))
                ret = uint16(38);
            elseif(isequal(obj.Trip_Selection, 'EPWM7 TRIPOUT'))
                ret = uint16(39);
            elseif(isequal(obj.Trip_Selection, 'EPWM8 TRIPOUT'))
                ret = uint16(40);
            elseif(isequal(obj.Trip_Selection, 'EPWM9 TRIPOUT'))
                ret = uint16(41);
            elseif(isequal(obj.Trip_Selection, 'EPWM10 TRIPOUT'))
                ret = uint16(42);
            elseif(isequal(obj.Trip_Selection, 'EPWM11 TRIPOUT'))
                ret = uint16(43);
            elseif(isequal(obj.Trip_Selection, 'EPWM12 TRIPOUT'))
                ret = uint16(44);
            elseif(isequal(obj.Trip_Selection, 'EPWM13 TRIPOUT'))
                ret = uint16(45);
            elseif(isequal(obj.Trip_Selection, 'EPWM14 TRIPOUT'))
                ret = uint16(46);
            elseif(isequal(obj.Trip_Selection, 'EPWM15 TRIPOUT'))
                ret = uint16(47);
            elseif(isequal(obj.Trip_Selection, 'EPWM16 TRIPOUT'))
                ret = uint16(48);
            elseif(isequal(obj.Trip_Selection, 'EPWM17 TRIPOUT'))
                ret = uint16(49);
            elseif(isequal(obj.Trip_Selection, 'EPWM18 TRIPOUT'))
                ret = uint16(50);
            elseif(isequal(obj.Trip_Selection, 'EPWM19 TRIPOUT'))
                ret = uint16(51);
            elseif(isequal(obj.Trip_Selection, 'EPWM20 TRIPOUT'))
                ret = uint16(52);
            elseif(isequal(obj.Trip_Selection, 'EPWM21 TRIPOUT'))
                ret = uint16(53);
            elseif(isequal(obj.Trip_Selection, 'EPWM22 TRIPOUT'))
                ret = uint16(54);
            elseif(isequal(obj.Trip_Selection, 'EPWM23 TRIPOUT'))
                ret = uint16(55);
            elseif(isequal(obj.Trip_Selection, 'EPWM24 TRIPOUT'))
                ret = uint16(56);
            elseif(isequal(obj.Trip_Selection, 'EPWM25 TRIPOUT'))
                ret = uint16(57);
            elseif(isequal(obj.Trip_Selection, 'EPWM26 TRIPOUT'))
                ret = uint16(58);
            elseif(isequal(obj.Trip_Selection, 'EPWM27 TRIPOUT'))
                ret = uint16(59);
            elseif(isequal(obj.Trip_Selection, 'EPWM28 TRIPOUT'))
                ret = uint16(60);
            elseif(isequal(obj.Trip_Selection, 'EPWM29 TRIPOUT'))
                ret = uint16(61);
            elseif(isequal(obj.Trip_Selection, 'EPWM30 TRIPOUT'))
                ret = uint16(62);
            elseif(isequal(obj.Trip_Selection, 'EPWM31 TRIPOUT'))
                ret = uint16(63);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Global_Load_StrobeEnum(obj)
            if(isequal(obj.Global_Load_Strobe, 'Disabled'))
                ret = uint16(0);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM0 Global Load Strobe'))
                ret = uint16(1);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM1 Global Load Strobe'))
                ret = uint16(2);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM2 Global Load Strobe'))
                ret = uint16(3);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM3 Global Load Strobe'))
                ret = uint16(4);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM4 Global Load Strobe'))
                ret = uint16(5);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM5 Global Load Strobe'))
                ret = uint16(6);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM6 Global Load Strobe'))
                ret = uint16(7);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM7 Global Load Strobe'))
                ret = uint16(8);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM8 Global Load Strobe'))
                ret = uint16(9);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM9 Global Load Strobe'))
                ret = uint16(10);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM10 Global Load Strobe'))
                ret = uint16(11);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM11 Global Load Strobe'))
                ret = uint16(12);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM12 Global Load Strobe'))
                ret = uint16(13);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM13 Global Load Strobe'))
                ret = uint16(14);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM14 Global Load Strobe'))
                ret = uint16(15);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM15 Global Load Strobe'))
                ret = uint16(16);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM16 Global Load Strobe'))
                ret = uint16(17);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM17 Global Load Strobe'))
                ret = uint16(18);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM18 Global Load Strobe'))
                ret = uint16(19);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM19 Global Load Strobe'))
                ret = uint16(20);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM20 Global Load Strobe'))
                ret = uint16(21);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM21 Global Load Strobe'))
                ret = uint16(22);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM22 Global Load Strobe'))
                ret = uint16(23);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM23 Global Load Strobe'))
                ret = uint16(24);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM24 Global Load Strobe'))
                ret = uint16(25);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM25 Global Load Strobe'))
                ret = uint16(26);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM26 Global Load Strobe'))
                ret = uint16(27);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM27 Global Load Strobe'))
                ret = uint16(28);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM28 Global Load Strobe'))
                ret = uint16(29);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM29 Global Load Strobe'))
                ret = uint16(30);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM30 Global Load Strobe'))
                ret = uint16(31);
            elseif(isequal(obj.Global_Load_Strobe, 'EPWM31 Global Load Strobe'))
                ret = uint16(32);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Type_MUNIT_Monitoring1Enum(obj)
            if(isequal(obj.Type_MUNIT_Monitoring1, 'High pulse width'))
                ret = uint16(0);
            elseif(isequal(obj.Type_MUNIT_Monitoring1, 'Low pulse width'))
                ret = uint16(1);
            elseif(isequal(obj.Type_MUNIT_Monitoring1,  'Period width from rise to rise'))
                ret = uint16(2);
            elseif(isequal(obj.Type_MUNIT_Monitoring1, 'Period width from fall to fall'))
                ret = uint16(3);
            elseif(isequal(obj.Type_MUNIT_Monitoring1, 'Monitor rise edge'))
                ret = uint16(4);
            elseif(isequal(obj.Type_MUNIT_MOnitoring1, 'Monitor fall edge'))
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Shadow_load_monitoring1Enum(obj)
            if(isequal(obj.Shadow_load_monitoring1, 'Load on next sync event'))
                ret = uint16(0);
            elseif(isequal(obj.Shadow_load_monitoring1, 'Load on EPWM GLDLCSTRB Event'))
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end
        
        function ret = get.Type_MUNIT_Monitoring2Enum(obj)
            if(isequal(obj.Type_MUNIT_Monitoring2, 'High pulse width'))
                ret = uint16(0);
            elseif(isequal(obj.Type_MUNIT_Monitoring2, 'Low pulse width'))
                ret = uint16(1);
            elseif(isequal(obj.Type_MUNIT_Monitoring2,  'Period width from rise to rise'))
                ret = uint16(2);
            elseif(isequal(obj.Type_MUNIT_Monitoring2, 'Period width from fall to fall'))
                ret = uint16(3);
            elseif(isequal(obj.Type_MUNIT_Monitoring2, 'Monitor rise edge'))
                ret = uint16(4);
            elseif(isequal(obj.Type_MUNIT_MOnitoring2, 'Monitor fall edge'))
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Shadow_load_monitoring2Enum(obj)
            if(isequal(obj.Shadow_load_monitoring2, 'Load on next sync event'))
                ret = uint16(0);
            elseif(isequal(obj.Shadow_load_monitoring2, 'Load on EPWM GLDLCSTRB Event'))
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DataTypeEnum(obj)
            if isequal(obj.DataType,'Double')
                ret = uint16(0);
            elseif isequal(obj.DataType, 'Single')
                ret = uint16(1);
            elseif isequal(obj.DataType, 'INT8')
                ret = uint16(2);
            elseif isequal(obj.DataType, 'Unsigned INT8')
                ret = uint16(3);
            elseif isequal(obj.DataType, 'INT16')
                ret = uint16(4);
            elseif isequal(obj.DataType, 'Unsigned INT16')
                ret = uint16(5);
            elseif isequal(obj.DataType, 'INT32')
                ret = uint16(4);
            elseif isequal(obj.DataType, 'Unsigned INT32')
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
                'Title','ECAP',...
                'Text', sprintf('%s\n\n%s','ECAP Type 3 Block'),...
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
             "PropertyList", {'ECAP_Instance', 'Emulation_Mode', 'ECAP_Mode', 'Use_Interrupt', 'Enable_Interrupt', 'Phase_Shift_Count_Value', 'Enable_Load_Counter', 'Load_Counter', 'Sync_Out_Mode', 'Sync_in_pulse_src', 'SampleTime', 'DataType'});
            
             CaptureMode = matlab.system.display.SectionGroup(...
                 'Title', 'CaptureMode',...
                 "PropertyList", {'Capture_Mode', 'Capture_stop_event', 'Event_Prescaler', 'Event1_Polarity', 'Enable_ECAP_Event1_Counter_Reset' ,'Event2_Polarity', 'Enable_ECAP_Event2_Counter_Reset', 'Event3_Polarity', 'Enable_ECAP_Event3_Counter_Reset' , 'Event4_Polarity', 'Enable_ECAP_Event4_Counter_Reset', ...
                 'ECAP_Input', 'Filter_Out_Pulses', 'Reset_Counter_Enable', 'Re_ARM_ECAP', 'Capture_event_status', 'Overflow_status', 'Trip_Selection', 'Global_Load_Strobe', 'Enable_Monitoring_Unit1', 'Type_MUNIT_Monitoring1', 'Min_value_monitoring1', 'Max_value_monitoring1', 'Enable_syncin_monitoring1', 'Force_copy_shadow_monitoring1', 'Shadow_load_monitoring1', ...
                 'Shadow_min_value_monitoring1', 'Shadow_max_value_monitoring1', 'Enable_debug_monitoring1', 'Enable_Monitoring_Unit2', 'Type_MUNIT_Monitoring2', 'Min_value_monitoring2', 'Max_value_monitoring2', 'Enable_syncin_monitoring2', 'Force_copy_shadow_monitoring2', 'Shadow_load_monitoring2', 'Shadow_min_value_monitoring2', 'Shadow_max_value_monitoring2', 'Enable_debug_monitoring2'});

             APWMMode = matlab.system.display.SectionGroup(...
                 'Title', 'APWMMode',...
                 "PropertyList", {'APWM_Polarity', 'APWM_Period_Specify', 'APWM_Period', 'APWM_Period_Initial', 'APWM_Compare_Specify', 'APWM_Compare', 'APWM_Compare_Initial'});
             Interrupt = matlab.system.display.SectionGroup(...
                 'Title', 'Interrupt',...
                 "PropertyList", {'Event1_Interrupt', 'Event2_Interrupt', 'Event3_Interrupt', 'Event4_Interrupt', 'Counter_overflow_Interrupt', 'Counter_period_Interrupt', 'Counter_compare_Interrupt'});
             groups = [General, CaptureMode, APWMMode, Interrupt];

            
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
                coder.cinclude('MW_ECAP.h');
                switch uint16(obj.ECAP_InstanceEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP0_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP1_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP2_U_BASE');
                    case 3
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP3_U_BASE');
                    case 4
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP4_U_BASE');
                    case 5
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP5_U_BASE');
                    case 6
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP6_U_BASE');
                    case 7
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP7_U_BASE');
                    case 8
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP8_U_BASE');
                    case 9
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP9_U_BASE');
                    case 10
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP10_U_BASE');
                    case 11
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP11_U_BASE');
                    case 12
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP12_U_BASE');
                    case 13
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP13_U_BASE');
                    case 14
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP14_U_BASE');
                    case 15
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP15_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ECAP0_U_BASE');
                end
                APWM_Period_Setup = uint32(0);
                APWM_Compare_Setup = uint32(0);
                if(isequal(obj.ECAP_Mode, 'APWM'))
                    if(isequal(obj.APWM_Period_Specify, 'Specify via Input Port'))
                        APWM_Period_Setup = uint32(obj.APWM_Period_Initial);
                    elseif(isequal(obj.APWM_Period_Specify, 'Specify via Dialog Box'))
                        APWM_Period_Setup = uint32(obj.APWM_Period);
                    end

                    if(isequal(obj.APWM_Compare_Specify, 'Specify via Input Port'))
                        APWM_Compare_Setup = uint32(obj.APWM_Compare_Initial);
                    elseif(isequal(obj.APWM_Compare_Specify, 'Specify via Dialog Box'))
                        APWM_Compare_Setup = uint32(obj.APWM_Compare);
                    end
                end

                ecap_setup_struct = struct('base', obj.baseaddr, 'capture_mode', uint16(obj.Capture_ModeEnum), 'ecap_events', uint16(obj.Capture_stop_eventEnum), 'ECAP_Mode', uint16(obj.ECAP_ModeEnum), 'preScalerValue', uint16(obj.Event_Prescaler), 'Event1_Polarity', uint16(obj.Event1_PolarityEnum), 'Event2_Polarity', uint16(obj.Event2_PolarityEnum), 'Event3_Polarity', uint16(obj.Event3_PolarityEnum), 'Event4_Polarity', uint16(obj.Event4_PolarityEnum), 'Enable_CounterReset_Event1', obj.Enable_ECAP_Event1_Counter_Reset, 'Enable_CounterReset_Event2', obj.Enable_ECAP_Event2_Counter_Reset, 'Enable_CounterReset_Event3', obj.Enable_ECAP_Event3_Counter_Reset, 'Enable_CounterReset_Event4', obj.Enable_ECAP_Event4_Counter_Reset, 'phaseValue', uint16(obj.Phase_Shift_Count_Value), 'enable_LoadCounter', uint16(obj.Enable_Load_Counter), 'LoadCounter', uint16(obj.Load_Counter), 'sync_outmode', uint16(obj.Sync_Out_ModeEnum), 'inputCaptureSignals', uint16(obj.ECAP_InputEnum), 'pulse_width_filter_select', uint16(obj.Filter_Out_PulsesEnum), 'reset_counters', uint16(obj.Reset_Counter_Enable), 'rearm_ecap', obj.Re_ARM_ECAP, 'apwm_polarity', uint16(obj.APWM_PolarityEnum), 'apwmPeriod', uint32(APWM_Period_Setup), 'apwmCompare', uint32(APWM_Compare_Setup), 'emulation_mode', uint16(obj.Emulation_ModeEnum), 'syncInPulseSrc', uint16(obj.Sync_in_pulse_srcEnum), 'UseInterrupt', obj.Use_Interrupt, 'EnableInterrupt', obj.Enable_Interrupt, 'Enable_Event1_ISR', obj.Event1_Interrupt, 'Enable_Event2_ISR', obj.Event2_Interrupt, 'Enable_Event3_ISR', obj.Event3_Interrupt, 'Enable_Event4_ISR', obj.Event4_Interrupt, 'Counter_Overflow_ISR', obj.Counter_overflow_Interrupt, 'Counter_Period_ISR', obj.Counter_period_Interrupt, 'Counter_Compare_ISR', obj.Counter_compare_Interrupt, 'trip_signal', uint16(obj.Trip_SelectionEnum), 'global_strobe', uint16(obj.Global_Load_StrobeEnum), 'Enable_signal_MUNIT1', uint16(obj.Enable_Monitoring_Unit1), 'Monitoring_type_MUNIT1', uint16(obj.Type_MUNIT_Monitoring1Enum), 'Monitoring_minval_MUNIT1', uint32(obj.Min_value_monitoring1), 'Monitoring_maxval_MUNIT1', uint32(obj.Max_value_monitoring1), 'Enable_syncin_MUNIT1', uint16(obj.Enable_syncin_monitoring1), 'Force_copy_from_shadow_MUNIT1', uint16(obj.Force_copy_shadow_monitoring1), 'SHDW_Load_Mode_MUNIT1', uint32(obj.Shadow_load_monitoring1Enum), 'SHDW_min_val_MUNIT1', uint32(obj.Shadow_min_value_monitoring1), 'SHDW_max_val_MUNIT1', uint32(obj.Shadow_max_value_monitoring1), 'Enable_Debug_MUNIT1', uint16(obj.Enable_debug_monitoring1), 'Enable_signal_MUNIT2', uint16(obj.Enable_Monitoring_Unit2), 'Monitoring_type_MUNIT2', uint16(obj.Type_MUNIT_Monitoring2Enum), 'Monitoring_minval_MUNIT2', uint32(obj.Min_value_monitoring2), 'Monitoring_maxval_MUNIT2', uint32(obj.Max_value_monitoring2), 'Enable_syncin_MUNIT2', uint16(obj.Enable_syncin_monitoring2), 'Force_copy_from_shadow_MUNIT2', uint16(obj.Force_copy_shadow_monitoring2), 'SHDW_Load_Mode_MUNIT2', uint32(obj.Shadow_load_monitoring2Enum), 'SHDW_min_val_MUNIT2', uint32(obj.Shadow_min_value_monitoring2), 'SHDW_max_val_MUNIT2', uint32(obj.Shadow_max_value_monitoring2), 'Enable_Debug_MUNIT2', uint16(obj.Enable_debug_monitoring2));
                coder.cstructname(ecap_setup_struct, 'ECAPSetupStruct', 'extern', 'HeaderFile', 'MW_ECAP.h');
                coder.ceval('ECAP_setup', coder.ref(ecap_setup_struct));
             
            end
        end
        
        function [varargout] = stepImpl(obj, varargin)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                value = 1;
                if(isequal(obj.ECAP_Mode, 'APWM'))
                    if(isequal(obj.APWM_Period_Specify, 'Specify via Input Port'))
                        coder.ceval('ECAP_setAPWMPeriod', obj.baseaddr, varargin{value});
                        value = value + 1;
                    elseif(isequal(obj.APWM_Period_Specify, 'Specify via Dialog Box'))
                        coder.ceval('ECAP_setAPWMPeriod', obj.baseaddr, uint32(obj.APWM_Period));
                    end

                    if(isequal(obj.APWM_Compare_Specify, 'Specify via Input Port'))
                        coder.ceval('ECAP_setAPWMCompare', obj.baseaddr, varargin{value});
                        value = value + 1;
                    elseif(isequal(obj.APWM_Compare_Specify, 'Specify via Dialog Box'))
                        coder.ceval('ECAP_setAPWMCompare', obj.baseaddr, uint32(obj.APWM_Compare));
                    end

                end
                if(isequal(obj.ECAP_Mode, 'Capture'))
                    coder.ceval('ECAP_setEventPrescaler', obj.baseaddr, uint16(obj.Event_Prescaler))
                    if(isequal(obj.Capture_Mode, 'ECAP operates in one shot capture mode') && obj.Re_ARM_ECAP)
                       if(varargin{value} == 1)
                            ECAP_reArm(obj.baseaddr);
                       end
                    end
                end
                x = uint32(zeros(4,1));
                a = uint16(zeros(4,1));
                b = uint16(zeros(1,1));
                index = 1;
                ecap_step_struct = struct('base', obj.baseaddr, 'ECAP_Mode', uint16(obj.ECAP_ModeEnum), 'capture_mode', uint16(obj.Capture_ModeEnum), 'UseInterrupt', obj.Use_Interrupt, 'EnableInterrupt', obj.Enable_Interrupt, 'Enable_Event1_ISR', obj.Event1_Interrupt, 'Enable_Event2_ISR', obj.Event2_Interrupt, 'Enable_Event3_ISR', obj.Event3_Interrupt, 'Enable_Event4_ISR', obj.Event4_Interrupt, 'Counter_Overflow_ISR', obj.Counter_overflow_Interrupt, 'Counter_Period_ISR', obj.Counter_period_Interrupt, 'Counter_Compare_ISR', obj.Counter_compare_Interrupt, 'rearm_ecap', obj.Re_ARM_ECAP, 'ecap_events', uint16(obj.Capture_stop_eventEnum));
                coder.cstructname(ecap_step_struct, 'ECAPStepStruct', 'extern', 'HeaderFile', 'MW_ECAP.h');
                coder.ceval('ECAP_step', coder.ref(ecap_step_struct), coder.wref(x), coder.wref(a), coder.wref(b));

                if(isequal(obj.Capture_stop_event, 'eCAP event 1'))
                    if isequal(obj.DataType, 'Double')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = double(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = double(a(1:1));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType,'Single')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = single(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = single(a(1:1));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType, 'INT8')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int8(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int8(a(1:1));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT8')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint8(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint8(a(1:1));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT16')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int16(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int16(a(1:1));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT16')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint16(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint16(a(1:1));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT32')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int32(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int32(a(1:1));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT32')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint32(x(1:1));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint32(a(1:1));
                            index = index + 1;
                        end
                    else
                        varargout{index} = uint32(x(1:1));
                        index = index + 1;
                        varargout{index} = uint32(a(1:1));
                         index = index + 1;
                    end
                elseif(isequal(obj.Capture_stop_event, 'eCAP event 2'))
                    if isequal(obj.DataType, 'Double')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = double(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = double(a(1:2));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType,'Single')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = single(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = single(a(1:2));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType, 'INT8')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int8(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int8(a(1:2));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT8')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint8(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint8(a(1:2));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT16')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int16(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int16(a(1:2));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT16')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint16(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint16(a(1:2));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT32')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int32(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int32(a(1:2));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT32')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint32(x(1:2));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint32(a(1:2));
                            index = index + 1;
                        end
                    else
                        varargout{index} = uint32(x(1:2));
                        index = index + 1;
                        varargout{index} = uint32(a(1:2));
                         index = index + 1;
                    end
                elseif(isequal(obj.Capture_stop_event, 'eCAP event 3'))
                    if isequal(obj.DataType, 'Double')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = double(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = double(a(1:3));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType,'Single')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = single(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = single(a(1:3));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType, 'INT8')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int8(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int8(a(1:3));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT8')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint8(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint8(a(1:3));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT16')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int16(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int16(a(1:3));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT16')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint16(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint16(a(1:3));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT32')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int32(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int32(a(1:3));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT32')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint32(x(1:3));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint32(a(1:3));
                            index = index + 1;
                        end
                    else
                        varargout{index} = uint32(x(1:3));
                        index = index + 1;
                        varargout{index} = uint32(a(1:3));
                         index = index + 1;
                    end
                elseif(isequal(obj.Capture_stop_event, 'eCAP event 4'))
                    if isequal(obj.DataType, 'Double')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = double(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = double(a(1:4));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType,'Single')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = single(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = single(a(1:4));
                            index = index + 1;
                        end
                    elseif isequal(obj.DataType, 'INT8')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int8(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int8(a(1:4));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT8')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint8(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint8(a(1:4));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT16')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int16(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int16(a(1:4));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT16')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint16(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint16(a(1:4));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'INT32')
                        if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = int32(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = int32(a(1:4));
                            index = index + 1;
                        end
                     elseif isequal(obj.DataType, 'Unsigned INT32')
                         if(obj.ECAP_ModeEnum == 0)
                            varargout{index} = uint32(x(1:4));
                            index = index + 1;
                        end
                        if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                            varargout{index} = uint32(a(1:4));
                            index = index + 1;
                        end
                    else
                        varargout{index} = uint32(x(1:4));
                        index = index + 1;
                        varargout{index} = uint32(a(1:4));
                         index = index + 1;
                    end
                else
                    varargout{index} = uint32(x);
                        index = index + 1;
                    varargout{index} = uint32(a);
                        index = index + 1;
                end

                if((obj.ECAP_ModeEnum == 0) && obj.Overflow_status)
                   varargout{index} = b;
                end
            end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                ecap_release_struct = struct('base', obj.baseaddr);
                coder.cstructname(ecap_release_struct, 'ECAPReleaseStruct', 'extern', 'HeaderFile', 'MW_ECAP.h');
                coder.ceval('ECAP_release', coder.ref(ecap_release_struct));
            end
        end

        function flag = isInactivePropertyImpl(obj,propertyName)
            if ((strcmp(propertyName, 'APWM_Polarity') | strcmp(propertyName, 'APWM_Period_Specify') | strcmp(propertyName, 'APWM_Compare_Specify')) && (isequal(obj.ECAP_Mode, 'Capture')))
               flag = true;
            elseif((strcmp(propertyName, 'APWM_Period')) && (isequal(obj.ECAP_Mode, 'Capture') | isequal(obj.APWM_Period_Specify, 'Specify via Input Port')))
                flag = true;
            elseif((strcmp(propertyName, 'APWM_Period_Initial')) && (isequal(obj.ECAP_Mode, 'Capture') | isequal(obj.APWM_Period_Specify, 'Specify via Dialog Box')))
                flag = true;
            elseif((strcmp(propertyName, 'APWM_Compare')) && (isequal(obj.ECAP_Mode, 'Capture') | isequal(obj.APWM_Compare_Specify, 'Specify via Input Port')))
                flag = true;
            elseif((strcmp(propertyName, 'APWM_Compare_Initial')) && (isequal(obj.ECAP_Mode, 'Capture') | isequal(obj.APWM_Compare_Specify, 'Specify via Dialog Box')))
                flag = true;
            elseif((strcmp(propertyName, 'Capture_Mode') | strcmp(propertyName, 'DataType') | strcmp(propertyName, 'Capture_stop_event') | strcmp(propertyName, 'Event_Prescaler') | strcmp(propertyName, 'ECAP_Input') | strcmp(propertyName, 'Filter_Out_Pulses') | strcmp(propertyName, 'Reset_Counter_Enable') | strcmp(propertyName, 'Capture_event_status') | strcmp(propertyName, 'Overflow_status') | strcmp(propertyName, 'Trip_Selection') | strcmp(propertyName, 'Global_Load_Strobe') | strcmp(propertyName, 'Enable_Monitoring_Unit1') | strcmp(propertyName, 'Enable_Monitoring_Unit2')) && (isequal(obj.ECAP_Mode, 'APWM')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event1_Polarity') | strcmp(propertyName, 'Enable_ECAP_Event1_Counter_Reset')) && (isequal(obj.ECAP_Mode, 'APWM')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event2_Polarity') | strcmp(propertyName, 'Enable_ECAP_Event2_Counter_Reset')) && (isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event3_Polarity') | strcmp(propertyName, 'Enable_ECAP_Event3_Counter_Reset')) && (isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1') | isequal(obj.Capture_stop_event, 'eCAP event 2')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event4_Polarity') | strcmp(propertyName, 'Enable_ECAP_Event4_Counter_Reset')) && (isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1') | isequal(obj.Capture_stop_event, 'eCAP event 2') | isequal(obj.Capture_stop_event, 'eCAP event 3')))
                    flag = true;
            elseif((strcmp(propertyName, 'Re_ARM_ECAP')) && (isequal(obj.ECAP_Mode, 'APWM')))
                flag = true;
            elseif((strcmp(propertyName, 'Enable_Interrupt')) && (~(obj.Use_Interrupt)))
                    flag = true;
            elseif((strcmp(propertyName, 'Event1_Interrupt') | strcmp(propertyName, 'Counter_overflow_Interrupt')) && ((~obj.Use_Interrupt) | isequal(obj.ECAP_Mode, 'APWM')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event2_Interrupt')) && ((~obj.Use_Interrupt) | isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event3_Interrupt')) && ((~obj.Use_Interrupt) | isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1') | isequal(obj.Capture_stop_event, 'eCAP event 2')))
                    flag = true;
            elseif((strcmp(propertyName, 'Event4_Interrupt')) && ((~obj.Use_Interrupt) | isequal(obj.ECAP_Mode, 'APWM') | isequal(obj.Capture_stop_event, 'eCAP event 1') | isequal(obj.Capture_stop_event, 'eCAP event 2') | isequal(obj.Capture_stop_event, 'eCAP event 3')))
                    flag = true;
           elseif((strcmp(propertyName, 'Counter_period_Interrupt')  | strcmp(propertyName, 'Counter_compare_Interrupt')) && ((~obj.Use_Interrupt) | isequal(obj.ECAP_Mode, 'Capture')))
                    flag = true;
            elseif((strcmp(propertyName, 'Type_MUNIT_Monitoring1') | strcmp(propertyName, 'Min_value_monitoring1') | strcmp(propertyName, 'Max_value_monitoring1') | strcmp(propertyName, 'Enable_syncin_monitoring1') | strcmp(propertyName, 'Enable_debug_monitoring1')) && (isequal(obj.ECAP_Mode, 'APWM') | (~obj.Enable_Monitoring_Unit1)))
                    flag = true;
            elseif((strcmp(propertyName, 'Force_copy_shadow_monitoring1') | strcmp(propertyName, 'Shadow_load_monitoring1') | strcmp(propertyName, 'Shadow_min_value_monitoring1') | strcmp(propertyName, 'Shadow_max_value_monitoring1')) && (isequal(obj.ECAP_Mode, 'APWM') | (~obj.Enable_Monitoring_Unit1) | (~obj.Enable_syncin_monitoring1)))
                    flag = true;
            elseif((strcmp(propertyName, 'Type_MUNIT_Monitoring2') | strcmp(propertyName, 'Min_value_monitoring2') | strcmp(propertyName, 'Max_value_monitoring2') | strcmp(propertyName, 'Enable_syncin_monitoring2') | strcmp(propertyName, 'Enable_debug_monitoring2')) && (isequal(obj.ECAP_Mode, 'APWM') | (~obj.Enable_Monitoring_Unit2)))
                    flag = true;
            elseif((strcmp(propertyName, 'Force_copy_shadow_monitoring2') | strcmp(propertyName, 'Shadow_load_monitoring2') | strcmp(propertyName, 'Shadow_min_value_monitoring2') | strcmp(propertyName, 'Shadow_max_value_monitoring2')) && (isequal(obj.ECAP_Mode, 'APWM') | (~obj.Enable_Monitoring_Unit2) | (~obj.Enable_syncin_monitoring2)))
                    flag = true;
            else
                    flag = false;
            end
        end
    end
    
    methods (Access=protected)
        %% Define output properties
        function num = getNumInputsImpl(obj)
            num = 0;
            if(strcmp(obj.ECAP_Mode, 'APWM') && strcmp(obj.APWM_Period_Specify, 'Specify via Input Port'))
                num = num+1;
            end
            if(strcmp(obj.ECAP_Mode, 'APWM') && strcmp(obj.APWM_Compare_Specify, 'Specify via Input Port'))
                num = num+1;
            end
            if(strcmp(obj.ECAP_Mode, 'Capture') && strcmp(obj.Capture_Mode, 'ECAP operates in one shot capture mode') && obj.Re_ARM_ECAP)
                num = num+1;
            end
        end

        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
        if num > 0
            varargout = cell(1,num);
            
                if(strcmp(obj.ECAP_Mode, 'APWM') && strcmp(obj.APWM_Period_Specify, 'Specify via Input Port'))
                    varargout{index} = 'T';
                    index = index + 1;
                end

                if(strcmp(obj.ECAP_Mode, 'APWM') && strcmp(obj.APWM_Compare_Specify, 'Specify via Input Port'))
                    varargout{index} = 'D';
                    index = index + 1;
                end

                if(strcmp(obj.ECAP_Mode, 'Capture') && strcmp(obj.Capture_Mode, 'ECAP operates in one shot capture mode') && obj.Re_ARM_ECAP)
                    varargout{index} = 'R';
                    index = index + 1;
                end
            end
        end
    
        
        function num = getNumOutputsImpl(obj)
            num = 0;
            if(obj.ECAP_ModeEnum == 0)
                num = num + 1;
            end
            if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                num = num + 1;
            end
            if((obj.ECAP_ModeEnum == 0) && obj.Overflow_status)
                num = num + 1;
            end    
        end

        function varargout = getOutputNamesImpl(obj)
            index = 1;
            n = getNumOutputsImpl(obj);
            if n > 0
                varargout = cell(1,n);
                if(obj.ECAP_ModeEnum == 0)
                    varargout{index} = 'TS';
                    index = index + 1;
                end
                if((obj.ECAP_ModeEnum == 0) && obj.Capture_event_status)
                    varargout{index} = 'CF';
                    index = index + 1;
                end
                if((obj.ECAP_ModeEnum == 0) && obj.Overflow_status)
                    varargout{index} = 'OF';
                    index = index + 1;
                end
            end
        end
        
        function varargout = isOutputFixedSizeImpl(obj)
            num = getNumOutputsImpl(obj);
            varargout = cell(1,num);
            indx = 1;
            if(obj.ECAP_ModeEnum == 0)
                varargout{indx} = true;
                indx = indx + 1;
            end
            if(obj.Capture_event_status)
                varargout{indx} = true;
                indx = indx + 1;
            end
            if(obj.Overflow_status)
                varargout{indx} = true;
            end
        end
        
        
        function varargout = isOutputComplexImpl(obj)
            num = getNumOutputsImpl(obj);
            varargout = cell(1,num);
            indx = 1;
            if(obj.ECAP_ModeEnum == 0)
                varargout{indx} = false;
                indx = indx + 1;
            end
            if(obj.Capture_event_status)
                varargout{indx} = false;
                indx = indx + 1;
            end
            if(obj.Overflow_status)
                varargout{indx} = false;
            end
        end
        
        function varargout = getOutputSizeImpl(obj)
            num = getNumOutputsImpl(obj);
            varargout = cell(1,num);
            indx = 1;
            if(obj.ECAP_ModeEnum == 0)
                if(obj.Capture_stop_eventEnum == 0)
                    varargout{indx} = [1,1];
                elseif(obj.Capture_stop_eventEnum == 1)
                    varargout{indx} = [2,1];
                elseif(obj.Capture_stop_eventEnum == 2)
                    varargout{indx} = [3,1];
                elseif(obj.Capture_stop_eventEnum == 3)
                    varargout{indx} = [4,1];
                else
                    varargout{indx} = [1,1];
                end
                indx = indx + 1;
            end
            if(obj.Capture_event_status)
                if(obj.Capture_stop_eventEnum == 0)
                    varargout{indx} = [1,1];
                elseif(obj.Capture_stop_eventEnum == 1)
                    varargout{indx} = [2,1];
                elseif(obj.Capture_stop_eventEnum == 2)
                    varargout{indx} = [3,1];
                elseif(obj.Capture_stop_eventEnum == 3)
                    varargout{indx} = [4,1];
                else
                    varargout{indx} = [1,1];
                end
                indx = indx + 1;
            end
            if(obj.Overflow_status)
                varargout{indx} = [1,1];
            end
        end
        
        function varargout = getOutputDataTypeImpl(obj)
            num = getNumOutputsImpl(obj);
            varargout = cell(1,num);
            indx = 1;
            if(obj.ECAP_ModeEnum == 0)
               if isequal(obj.DataType, 'Double')
                   varargout{indx} = "double";
               elseif isequal(obj.DataType, 'Single')
                   varargout{indx}= "single";
               elseif isequal(obj.DataType, 'INT8')
                   varargout{indx} ="int8";
               elseif isequal(obj.DataType, 'Unsigned INT8')
                   varargout{indx} ="uint8";
               elseif isequal(obj.DataType, 'INT16')
                   varargout{indx} ="int16";
               elseif isequal(obj.DataType, 'INT32')
                   varargout{indx} ="int32";
               elseif isequal(obj.DataType, 'Unsigned INT32')
                   varargout{indx} ="uint32";
               else
                   varargout{indx} ="uint16";
               end
               indx = indx + 1;
            end
            if(obj.Capture_event_status)
                if isequal(obj.DataType, 'Double')
                   varargout{indx} = "double";
               elseif isequal(obj.DataType, 'Single')
                   varargout{indx}= "single";
               elseif isequal(obj.DataType, 'INT8')
                   varargout{indx} ="int8";
               elseif isequal(obj.DataType, 'Unsigned INT8')
                   varargout{indx} ="uint8";
               elseif isequal(obj.DataType, 'INT16')
                   varargout{indx} ="int16";
               elseif isequal(obj.DataType, 'INT32')
                   varargout{indx} ="int32";
               elseif isequal(obj.DataType, 'Unsigned INT32')
                   varargout{indx} ="uint32";
               else
                   varargout{indx} ="uint16";
                end
                indx = indx + 1;
            end
            if(obj.Overflow_status)
                if isequal(obj.DataType, 'Double')
                   varargout{indx} = "double";
               elseif isequal(obj.DataType, 'Single')
                   varargout{indx}= "single";
               elseif isequal(obj.DataType, 'INT8')
                   varargout{indx} ="int8";
               elseif isequal(obj.DataType, 'Unsigned INT8')
                   varargout{indx} ="uint8";
               elseif isequal(obj.DataType, 'INT16')
                   varargout{indx} ="int16";
               elseif isequal(obj.DataType, 'INT32')
                   varargout{indx} ="int32";
               elseif isequal(obj.DataType, 'Unsigned INT32')
                   varargout{indx} ="uint32";
               else
                   varargout{indx} ="uint16";
                end
            end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'ECAP', 'Type 3'};
        end    
    end
    
   
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'ECAP';
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
                addIncludeFiles(buildInfo, 'MW_ECAP.h', includeDir);
                addSourceFiles(buildInfo, 'MW_ECAP.c', srcDir);
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

