
classdef EQEP_AM263PX < matlab.System & ...
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
        
        %Maximum Position
        Maximum_Position=0;
        %Position Counter (QPOSCNT)
        Position_Counter=0;
        %Position Counter Init Value
        Position_counter_Init_Value=0;
        %Compare Value
        Compare_Value=0;
        %Compare Cycles
        Compare_Cycles=0;
        %Unit Timer Period
        Unit_Timer_Period=0;
        %EQEP Watchdog Timer Period
        EQEP_Watchdog_Timer_Period=0;
        %EQEP Watchdog Timer Value
        EQEP_Watchdog_Timer_Value=0;
        
        
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

       %Sample Time
       SampleTime = 0.0001;
       %EQEP Module
       EQEP_Module = 'EQEP0';
       %Invert QEPA
       Invert_QEPA_Polarity (1,1) logical = false;
       %Invert QEPB
       Invert_QEPB_Polarity (1,1) logical = false;
       %Invert Index
       Invert_Index (1,1) logical = false;
       %Invert Strobe
       Invert_Strobe (1,1) logical = false;
       %Position Counter Source
       Position_Counter_Source = 'Quadrature clock mode';
       %Resolution
       Resolution = '2x resolution, count rising and falling edge';
       %Swap QEPA and QEPB
       Swap_QEPA_QEPB = 'Do not swap QEPA and QEPB';
       %Enable direction change during index
       Enable_direction_change_index (1,1) logical = false;
       %Emulation Mode
       Emulation_mode = 'Counters stop immediately';
       %QMA Module Mode A
       QMA_Module_ModeA = 'QMA module is bypassed';
       %QEPA Source
       QEPA_Source = 'Signal is 0';
       %QEPB Source
       QEPB_Source = 'Signal is 0';
       %QEPI/Index Source
       QEPI_INDX_Source = 'Signal is 0';
       %Strobe Source
       Strobe_Source = 'Strobe signal comes from GPIO';
       %Output Direction Flag
       Direction_Flag_OP (1,1) logical = false;
       
       %Position Counter and Control Unit

       %Position Counter Mode
       Position_counter_mode = 'Reset position on index pulse';
       %QEP Capture Latch Mode
       QEP_Capture_Latch = 'On position counter read';
       %Strobe Event Latch
       Strobe_Event_Latch = 'On rising edge of strobe';
       %Index Event Latch
       Index_Event_Latch = 'On rising edge of index';
       %Position Strobe Events
       Position_strobe_events = 'Action is disabled';
       %Position Index Events
       Position_index_events = 'Action is disabled';
       %Set software init position counter
       Set_software_init_position_counter (1,1) logical = false;
       %Enable position counter
       Enable_position_counter (1,1) logical = false;
       %Configure position compare unit
       Configure_position_compare_unit (1,1) logical = false;
       %Sync output pin selection
       Sync_output_pin_selection = 'Disable sync output';
       %Position compare of shadow load
       Position_compare_shadow_load = 'Disable shadow of QPOSCMP';
       %Enable position compare unit
       Enable_position_compare_unit (1,1) logical = false;
       %Output Position Counter
       Position_Cntr_OP (1,1) logical = false;
       %Output position counter latch value
       Position_cntr_latch_OP (1,1) logical = false;
       %Output index position latch
       Indx_position_latch_OP (1,1) logical = false;
       %Output strobe position latch
       Strobe_position_latch_OP (1,1) logical = false;

       %Edge Capture Unit


       %Configure edge capture unit
       Config_edge_capture_unit (1,1) logical = false;
       %Capture Prescaler
       Capture_Prescaler = 'CAPCLK = SYSCLKOUT/1';
       %Unit position event prescaler
       Unit_position_event_prescaler = 'UPEVNT = QCLK/1';
       %Enable edge capture unit
       Enable_edge_capture_unit (1,1) logical = false;
       %Enable unit timer
       Enable_unit_timer (1,1) logical = false;
       %Output Capture timer value
       Capture_timer_OP (1,1) logical = false;
       %Output Capture period value
       Capture_period_OP (1,1) logical = false;
       %Output Capture timer latch value
       Capture_timer_latch_OP (1,1) logical = false;
       %Output Capture period latch value
       Capture_period_latch_OP (1,1) logical = false;
      
       %Watchdog Timer

       %Enable EQEP Watchdog
       Enable_EQEP_watchdog (1,1) logical = false;
       %Output watchdog timer value
       Watchdog_timer_OP (1,1) logical = false;

       %Interrupts

       %Use Interrupt
       Use_Interrupt (1,1) logical = false;
       %Enable Interrupt
       Enable_Interrupt (1,1) logical = false;
       %Enable global interrupt flag
       Global_interrupt_flag (1,1) logical = false;
       %Enable position counter error interrupt
       Position_counter_error (1,1) logical = false;
       %Enable quadrature phase error interrupt
       Quadrature_phase_error (1,1) logical = false;
       %Enable quadrature direction change interrupt
       Quadrature_direction_change (1,1) logical = false;
       %Enable watchdog timeout interrupt
       Watchdog_timeout_interrupt (1,1) logical = false;
       %Enable position counter underflow interrupt
       Position_counter_underflow (1,1) logical = false;
       %Enable position counter overflow interrupt
       Position_counter_overflow (1,1) logical = false;
       %Enable position compare ready interrupt
       Position_compare_ready (1,1) logical = false;
       %Enable position compare match interrupt
       Position_compare_match (1,1) logical = false;
       %Enable strobe event latch interrupt
       Strobe_event_latch_intr (1,1) logical = false;
       %Enable index event latch interrupt
       Index_event_latch_interrupt (1,1) logical = false;
       %Enable unit timeout interrupt
       Unit_timeout_interrupt (1,1) logical = false;
       %Enable QMA error interrupt
       QMA_error_interrupt (1,1) logical = false;

       %Signal data types
       
       %Direction flag value data type 
       Dir_flag_datatype = 'double';
       %Position counter value data type
       Pos_cntr_datatype = 'double';
       %Position counter latch value data type
       Pos_cntr_latch_datatype = 'double';
       %Index position latch value data type
       Indx_position_latch_datatype = 'double';
       %Strobe position latch value data type
       Strobe_position_latch_datatype = 'double';
       %Capture timer value data type
       Capture_timer_datatype = 'double';
       %Capture period value data type
       Capture_period_datatype = 'double';
       %Capture timer latch value data type 
       Capture_timer_latch_datatype = 'double';
       %Capture period latch value data type
       Capture_period_latch_datatype = 'double';
       %Watchdog timer value data type
       Watchdog_timer_value_datatype = 'double';

    end

    properties (Access = private)
        % Pre-computed constants.
        baseaddr = 0;
        
    end
    
    properties (Constant, Hidden)
     EQEP_ModuleSet = matlab.system.StringSet({'EQEP0', 'EQEP1', 'EQEP2'});
     Position_Counter_SourceSet = matlab.system.StringSet({'Quadrature clock mode', 'Direction-count mode', 'Up-count mode', 'Down-count mode'});
     ResolutionSet = matlab.system.StringSet({'2x resolution, count rising and falling edge', '1x resolution, count rising edge only'});
     Swap_QEPA_QEPBSet = matlab.system.StringSet({'Do not swap QEPA and QEPB', 'Swap QEPA and QEPB'});
     Emulation_modeSet = matlab.system.StringSet({'Counters stop immediately', 'Counters stop at period rollover', 'Counter unaffected by suspend'});
     QMA_Module_ModeASet = matlab.system.StringSet({'QMA module is bypassed', 'QMA mode-1 operation is selected', 'QMA mode-2 operation is selcted'});
     QEPA_SourceSet = matlab.system.StringSet({'Signal is 0', 'Signal comes from device pin', 'Signal comes from PWM XBAR Out 0', 'Signal comes from PWM XBAR Out 1', 'Signal comes from PWM XBAR Out 2', 'Signal comes from PWM XBAR Out 3', 'Signal comes from PWM XBAR Out 4', 'Signal comes from PWM XBAR Out 5', 'Signal comes from PWM XBAR Out 6', 'Signal comes from PWM XBAR Out 7', 'Signal comes from PWM XBAR Out 8', 'Signal comes from PWM XBAR Out 9', 'Signal comes from PWM XBAR Out 10', 'Signal comes from PWM XBAR Out 11', 'Signal comes from PWM XBAR Out 12', 'Signal comes from PWM XBAR Out 13', 'Signal comes from PWM XBAR Out 14', 'Signal comes from PWM XBAR Out 15', 'Signal comes from PWM XBAR Out 16', 'Signal comes from PWM XBAR Out 17', 'Signal comes from PWM XBAR Out 18', 'Signal comes from PWM XBAR Out 19', 'Signal comes from PWM XBAR Out 20', 'Signal comes from PWM XBAR Out 21', 'Signal comes from PWM XBAR Out 22', 'Signal comes from PWM XBAR Out 23', 'Signal comes from PWM XBAR Out 24', 'Signal comes from PWM XBAR Out 25', 'Signal comes from PWM XBAR Out 26', 'Signal comes from PWM XBAR Out 27', 'Signal comes from PWM XBAR Out 28', 'Signal comes from PWM XBAR Out 29'});
     QEPB_SourceSet = matlab.system.StringSet({'Signal is 0', 'Signal comes from device pin', 'Signal comes from PWM XBAR Out 0', 'Signal comes from PWM XBAR Out 1', 'Signal comes from PWM XBAR Out 2', 'Signal comes from PWM XBAR Out 3', 'Signal comes from PWM XBAR Out 4', 'Signal comes from PWM XBAR Out 5', 'Signal comes from PWM XBAR Out 6', 'Signal comes from PWM XBAR Out 7', 'Signal comes from PWM XBAR Out 8', 'Signal comes from PWM XBAR Out 9', 'Signal comes from PWM XBAR Out 10', 'Signal comes from PWM XBAR Out 11', 'Signal comes from PWM XBAR Out 12', 'Signal comes from PWM XBAR Out 13', 'Signal comes from PWM XBAR Out 14', 'Signal comes from PWM XBAR Out 15', 'Signal comes from PWM XBAR Out 16', 'Signal comes from PWM XBAR Out 17', 'Signal comes from PWM XBAR Out 18', 'Signal comes from PWM XBAR Out 19', 'Signal comes from PWM XBAR Out 20', 'Signal comes from PWM XBAR Out 21', 'Signal comes from PWM XBAR Out 22', 'Signal comes from PWM XBAR Out 23', 'Signal comes from PWM XBAR Out 24', 'Signal comes from PWM XBAR Out 25', 'Signal comes from PWM XBAR Out 26', 'Signal comes from PWM XBAR Out 27', 'Signal comes from PWM XBAR Out 28', 'Signal comes from PWM XBAR Out 29'});
     QEPI_INDX_SourceSet = matlab.system.StringSet({'Signal is 0', 'Signal comes from device pin', 'Signal comes from PWM XBAR Out 0', 'Signal comes from PWM XBAR Out 1', 'Signal comes from PWM XBAR Out 2', 'Signal comes from PWM XBAR Out 3', 'Signal comes from PWM XBAR Out 4', 'Signal comes from PWM XBAR Out 5', 'Signal comes from PWM XBAR Out 6', 'Signal comes from PWM XBAR Out 7', 'Signal comes from PWM XBAR Out 8', 'Signal comes from PWM XBAR Out 9', 'Signal comes from PWM XBAR Out 10', 'Signal comes from PWM XBAR Out 11', 'Signal comes from PWM XBAR Out 12', 'Signal comes from PWM XBAR Out 13', 'Signal comes from PWM XBAR Out 14', 'Signal comes from PWM XBAR Out 15', 'Signal comes from PWM XBAR Out 16', 'Signal comes from PWM XBAR Out 17', 'Signal comes from PWM XBAR Out 18', 'Signal comes from PWM XBAR Out 19', 'Signal comes from PWM XBAR Out 20', 'Signal comes from PWM XBAR Out 21', 'Signal comes from PWM XBAR Out 22', 'Signal comes from PWM XBAR Out 23', 'Signal comes from PWM XBAR Out 24', 'Signal comes from PWM XBAR Out 25', 'Signal comes from PWM XBAR Out 26', 'Signal comes from PWM XBAR Out 27', 'Signal comes from PWM XBAR Out 28', 'Signal comes from PWM XBAR Out 29'});
     Strobe_SourceSet = matlab.system.StringSet({'Strobe signal comes from GPIO'});
     Position_counter_modeSet = matlab.system.StringSet({'Reset position on index pulse', 'Reset position on maximum position', 'Reset position on first index pulse', 'Reset position on unit time event'});
     QEP_Capture_LatchSet = matlab.system.StringSet({'On position counter read', 'On unit time-out event'});
     Strobe_Event_LatchSet = matlab.system.StringSet({'On rising edge of strobe', 'On rising edge when clockwise, on falling edge when counter clockwise'});
     Index_Event_LatchSet = matlab.system.StringSet({'On rising edge of index', 'On falling edge of index', 'On software index marker'});
     Position_strobe_eventsSet = matlab.system.StringSet({'Action is disabled', 'On rising edge of strobe', 'On rising edge when clockwise, on falling edge when counter clockwise'});
     Position_index_eventsSet = matlab.system.StringSet({'Action is disabled', 'On rising edge of index', 'On falling edge of index'});
     Sync_output_pin_selectionSet = matlab.system.StringSet({'Disable sync output', 'Sync output on index pin', 'Sync output on strobe pin'});
     Position_compare_shadow_loadSet = matlab.system.StringSet({'Disable shadow of QPOSCMP', 'Load on QPOSCNT = 0', 'Load on QPOSCNT = QPOSCMP'});
     Capture_PrescalerSet = matlab.system.StringSet({'CAPCLK = SYSCLKOUT/1', 'CAPCLK = SYSCLKOUT/2', 'CAPCLK = SYSCLKOUT/4', 'CAPCLK = SYSCLKOUT/8', 'CAPCLK = SYSCLKOUT/16', 'CAPCLK = SYSCLKOUT/32', 'CAPCLK = SYSCLKOUT/64', 'CAPCLK = SYSCLKOUT/128'});
     Unit_position_event_prescalerSet = matlab.system.StringSet({'UPEVNT = QCLK/1', 'UPEVNT = QCLK/2', 'UPEVNT = QCLK/4', 'UPEVNT = QCLK/8', 'UPEVNT = QCLK/16', 'UPEVNT = QCLK/32', 'UPEVNT = QCLK/64', 'UPEVNT = QCLK/128','UPEVNT = QCLK/256','UPEVNT = QCLK/512','UPEVNT = QCLK/1024','UPEVNT = QCLK/2048'});
     Dir_flag_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Pos_cntr_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Pos_cntr_latch_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Indx_position_latch_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Strobe_position_latch_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Capture_timer_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Capture_period_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Capture_timer_latch_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Capture_period_latch_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
     Watchdog_timer_value_datatypeSet = matlab.system.StringSet({'double', 'single', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'boolean'});
    end
    
    properties (Dependent, Access=protected)
     EQEP_ModuleEnum;
     Position_Counter_SourceEnum;
     ResolutionEnum;
     Swap_QEPA_QEPBEnum;
     Emulation_modeEnum;
     QMA_Module_ModeAEnum;
     QEPA_SourceEnum;
     QEPB_SourceEnum;
     QEPI_INDX_SourceEnum;
     Strobe_SourceEnum;
     Position_counter_modeEnum;
     QEP_Capture_LatchEnum;
     Strobe_Event_LatchEnum;
     Index_Event_LatchEnum;
     Position_strobe_eventsEnum;
     Position_index_eventsEnum;
     Sync_output_pin_selectionEnum;
     Position_compare_shadow_loadEnum;
     Capture_PrescalerEnum;
     Unit_position_event_prescalerEnum;
    end

    
    methods
        % Constructor
        function obj = EQEP_AM263PX(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.EQEP_ModuleEnum(obj)
            if(isequal(obj.EQEP_Module, 'EQEP0'))
                ret = uint16(0);
            elseif(isequal(obj.EQEP_Module, 'EQEP1'))
                ret = uint16(1);
            elseif(isequal(obj.EQEP_Module, 'EQEP2'))
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Position_Counter_SourceEnum(obj)
            if(isequal(obj.Position_Counter_Source, 'Quadrature clock mode'))
                ret = uint16(0);
            elseif(isequal(obj.Position_Counter_Source, 'Direction-count mode'))
                ret = uint16(1);
            elseif(isequal(obj.Position_Counter_Source, 'Up-count mode'))
                ret = uint16(2);
            elseif(isequal(obj.Position_Counter_Source, 'Down-count mode'))
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ResolutionEnum(obj)
            if(isequal(obj.Resolution, '2x resolution, count rising and falling edge'))
                ret = uint16(0);
            elseif(isequal(obj.Resolution, '1x resolution, count rising edge only'))
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Swap_QEPA_QEPBEnum(obj)
            if(isequal(obj.Swap_QEPA_QEPB, 'Do not swap QEPA and QEPB'))
                ret = uint16(0);
            elseif(isequal(obj.Swap_QEPA_QEPB, 'Swap QEPA and QEPB'))
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Emulation_modeEnum(obj)
            if(isequal(obj.Emulation_mode, 'Counters stop immediately'))
                ret = uint16(0);
            elseif(isequal(obj.Emulation_mode, 'Counters stop at period rollover'))
                ret = uint16(1);
            elseif(isequal(obj.Emulation_mode, 'Counter unaffected by suspend'))
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.QMA_Module_ModeAEnum(obj)
            if(isequal(obj.QMA_Module_ModeA, 'QMA module is bypassed'))
                ret = uint16(0);
            elseif(isequal(obj.QMA_Module_ModeA, 'QMA mode-1 operation is selected'))
                ret = uint16(1);
            elseif(isequal(obj.QMA_Module_ModeA, 'QMA mode-2 operation is selcted'))
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.QEPA_SourceEnum(obj)
            if(isequal(obj.QEPA_Source, 'Signal is 0'))
                ret = 0x0000;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from device pin'))
                ret = 0x0001;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 0'))
                ret = 0x0002;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 1'))
                ret = 0x0003;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 2'))
                ret = 0x0004;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 3'))
                ret = 0x0005;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 4'))
                ret = 0x0006;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 5'))
                ret = 0x0007;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 6'))
                ret = 0x0008;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 7'))
                ret = 0x0009;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 8'))
                ret = 0x000A;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 9'))
                ret = 0x000B;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 10'))
                ret = 0x000C;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 11'))
                ret = 0x000D;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 12'))
                ret = 0x000E;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 13'))
                ret = 0x000F;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 14'))
                ret = 0x0010;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 15'))
                ret = 0x0011;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 16'))
                ret = 0x0012;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 17'))
                ret = 0x0013;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 18'))
                ret = 0x0014;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 19'))
                ret = 0x0015;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 20'))
                ret = 0x0016;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 21'))
                ret = 0x0017;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 22'))
                ret = 0x0018;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 23'))
                ret = 0x0019;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 24'))
                ret = 0x001A;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 25'))
                ret = 0x001B;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 26'))
                ret = 0x001C;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 27'))
                ret = 0x001D;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 28'))
                ret = 0x001E;
            elseif(isequal(obj.QEPA_Source, 'Signal comes from PWM XBAR Out 29'))
                ret = 0x001F;
            else
                ret = 0x0000;
            end
        end

        function ret = get.QEPB_SourceEnum(obj)
            if(isequal(obj.QEPB_Source, 'Signal is 0'))
                ret = 0x0000;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from device pin'))
                ret = 0x0001;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 0'))
                ret = 0x0002;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 1'))
                ret = 0x0003;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 2'))
                ret = 0x0004;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 3'))
                ret = 0x0005;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 4'))
                ret = 0x0006;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 5'))
                ret = 0x0007;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 6'))
                ret = 0x0008;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 7'))
                ret = 0x0009;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 8'))
                ret = 0x000A;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 9'))
                ret = 0x000B;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 10'))
                ret = 0x000C;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 11'))
                ret = 0x000D;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 12'))
                ret = 0x000E;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 13'))
                ret = 0x000F;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 14'))
                ret = 0x0010;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 15'))
                ret = 0x0011;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 16'))
                ret = 0x0012;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 17'))
                ret = 0x0013;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 18'))
                ret = 0x0014;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 19'))
                ret = 0x0015;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 20'))
                ret = 0x0016;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 21'))
                ret = 0x0017;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 22'))
                ret = 0x0018;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 23'))
                ret = 0x0019;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 24'))
                ret = 0x001A;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 25'))
                ret = 0x001B;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 26'))
                ret = 0x001C;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 27'))
                ret = 0x001D;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 28'))
                ret = 0x001E;
            elseif(isequal(obj.QEPB_Source, 'Signal comes from PWM XBAR Out 29'))
                ret = 0x001F;
            else
                ret = 0x0000;
            end
        end

        function ret = get.QEPI_INDX_SourceEnum(obj)
            if(isequal(obj.QEPI_INDX_Source, 'Signal is 0'))
                ret = 0x0000;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from device pin'))
                ret = 0x0001;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 0'))
                ret = 0x0002;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 1'))
                ret = 0x0003;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 2'))
                ret = 0x0004;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 3'))
                ret = 0x0005;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 4'))
                ret = 0x0006;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 5'))
                ret = 0x0007;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 6'))
                ret = 0x0008;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 7'))
                ret = 0x0009;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 8'))
                ret = 0x000A;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 9'))
                ret = 0x000B;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 10'))
                ret = 0x000C;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 11'))
                ret = 0x000D;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 12'))
                ret = 0x000E;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 13'))
                ret = 0x000F;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 14'))
                ret = 0x0010;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 15'))
                ret = 0x0011;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 16'))
                ret = 0x0012;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 17'))
                ret = 0x0013;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 18'))
                ret = 0x0014;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 19'))
                ret = 0x0015;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 20'))
                ret = 0x0016;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 21'))
                ret = 0x0017;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 22'))
                ret = 0x0018;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 23'))
                ret = 0x0019;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 24'))
                ret = 0x001A;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 25'))
                ret = 0x001B;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 26'))
                ret = 0x001C;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 27'))
                ret = 0x001D;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 28'))
                ret = 0x001E;
            elseif(isequal(obj.QEPI_INDX_Source, 'Signal comes from PWM XBAR Out 29'))
                ret = 0x001F;
            else
                ret = 0x0000;
            end
        end 

        function ret = get.Strobe_SourceEnum(obj)
            if(isequal(obj.Strobe_Source, 'Strobe signal comes from GPIO'))
                ret = uint16(0);
            else
                ret = uint16(1);
            end
        end

        function ret = get.Position_counter_modeEnum(obj)
            if(isequal(obj.Position_counter_mode, 'Reset position on index pulse'))
                ret = 0x0000;
            elseif(isequal(obj.Position_counter_mode, 'Reset position on maximum position'))
                ret = 0x1000;
            elseif(isequal(obj.Position_counter_mode, 'Reset position on first index pulse'))
                ret = 0x2000;
            elseif(isequal(obj.Position_counter_mode, 'Reset position on unit time event'))
                ret = 0x3000;
            else
                ret = 0x0000;
            end
        end

        function ret = get.QEP_Capture_LatchEnum(obj)
            if(isequal(obj.QEP_Capture_Latch, 'On position counter read'))
                ret = 0x0000;
            elseif(isequal(obj.QEP_Capture_Latch, 'On unit time-out event'))
                ret = 0x0004;
            end
        end

        function ret = get.Strobe_Event_LatchEnum(obj)
            if(isequal(obj.Strobe_Event_Latch, 'On rising edge of strobe'))
                ret = 0x0000;
            elseif(isequal(obj.Strobe_Event_Latch, 'On rising edge when clockwise, on falling edge when counter clockwise'))
                ret = 0x0004;
            end
        end

        function ret = get.Index_Event_LatchEnum(obj)
            if(isequal(obj.Index_Event_Latch, 'On rising edge of index'))
                ret = 0x0010;
            elseif(isequal(obj.Index_Event_Latch, 'On falling edge of index'))
                ret = 0x0020;
            elseif(isequal(obj.Index_Event_Latch, 'On software index marker'))
                ret = 0x0030;
            end
        end

        function ret = get.Position_strobe_eventsEnum(obj)
            if(isequal(obj.Position_strobe_events, 'Action is disabled'))
                ret = 0x0000;
            elseif(isequal(obj.Position_strobe_events, 'On rising edge of strobe'))
                ret = 0x0800;
            elseif(isequal(obj.Position_strobe_events, 'On rising edge when clockwise, on falling edge when counter clockwise'))
                ret = 0x0C00;
            end
        end

        function ret = get.Position_index_eventsEnum(obj)
            if(isequal(obj.Position_index_events, 'Action is disabled'))
                ret = 0x0000;
            elseif(isequal(obj.Position_index_events, 'On rising edge of index'))
                ret = 0x0200;
            elseif(isequal(obj.Position_index_events, 'On falling edge of index'))
                ret = 0x0300;
            end
        end

        function ret = get.Sync_output_pin_selectionEnum(obj)
            if(isequal(obj.Sync_output_pin_selection, 'Disable sync output'))
                ret = 0x0000;
            elseif(isequal(obj.Sync_output_pin_selection, 'Sync output on index pin'))
                ret = 0x2000;
            elseif(isequal(obj.Sync_output_pin_selection, 'Sync output on strobe pin'))
                ret = 0x3000;
            end
        end

        function ret = get.Position_compare_shadow_loadEnum(obj)
            if(isequal(obj.Position_compare_shadow_load, 'Disable shadow of QPOSCMP'))
                ret = 0x0000;
            elseif(isequal(obj.Position_compare_shadow_load, 'Load on QPOSCNT = 0'))
                ret = 0x8000;
            elseif(isequal(obj.Position_compare_shadow_load, 'Load on QPOSCNT = QPOSCMP'))
                ret = 0xC000;
            end
        end

        function ret = get.Capture_PrescalerEnum(obj)
            if(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/1'))
                ret = 0x00;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/2'))
                ret = 0x10;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/4'))
                ret = 0x20;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/8'))
                ret = 0x30;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/16'))
                ret = 0x40;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/32'))
                ret = 0x50;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/64'))
                ret = 0x60;
            elseif(isequal(obj.Capture_Prescaler, 'CAPCLK = SYSCLKOUT/128'))
                ret = 0x70;
            end
        end

        function ret = get.Unit_position_event_prescalerEnum(obj)
            if(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/1'))
                ret = uint16(0);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/2'))
                ret = uint16(1);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/4'))
                ret = uint16(2);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/8'))
                ret = uint16(3);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/16'))
                ret = uint16(4);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/32'))
                ret = uint16(5);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/64'))
                ret = uint16(6);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/128'))
                ret = uint16(7);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/256'))
                ret = uint16(8);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/512'))
                ret = uint16(9);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/1024'))
                ret = uint16(10);
            elseif(isequal(obj.Unit_position_event_prescaler, 'UPEVNT = QCLK/2048'))
                ret = uint16(11);
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
            header = matlab.system.display.Header(...s
                'Title','Type 2.1 EQEP',...
                'Text', sprintf('%s\n\n%s','The enhanced quadrature encoder pulse (eQEP)'),...
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
                    'PropertyList', {'EQEP_Module', 'Emulation_mode', 'QMA_Module_ModeA', 'QEPA_Source', 'QEPB_Source', 'QEPI_INDX_Source', 'Strobe_Source', 'Direction_Flag_OP', 'SampleTime'});

                Quadrature_Decoder_Unit = matlab.system.display.SectionGroup(...
                    'Title', 'Quadrature Decoder Unit', ...
                    'PropertyList', {'Invert_QEPA_Polarity', 'Invert_QEPB_Polarity', 'Invert_Index', 'Invert_Strobe', 'Position_Counter_Source', 'Resolution', 'Swap_QEPA_QEPB', 'Enable_direction_change_index'});

                Position_Counter_Control_Unit = matlab.system.display.SectionGroup(...
                    'Title', 'Position Counter Control Unit', ...
                    'PropertyList', {'Position_counter_mode', 'Maximum_Position', 'Position_Counter', 'QEP_Capture_Latch', 'Strobe_Event_Latch', 'Index_Event_Latch', 'Position_strobe_events', 'Position_index_events', 'Set_software_init_position_counter', 'Position_counter_Init_Value', 'Enable_position_counter', 'Configure_position_compare_unit', 'Sync_output_pin_selection', 'Position_compare_shadow_load', 'Compare_Value', 'Compare_Cycles', 'Enable_position_compare_unit', 'Position_Cntr_OP', 'Position_cntr_latch_OP', 'Indx_position_latch_OP', 'Strobe_position_latch_OP'});
                
                Edge_Capture_Unit = matlab.system.display.SectionGroup(...
                    'Title', 'Edge Capture Unit', ...
                    'PropertyList', {'Config_edge_capture_unit', 'Capture_Prescaler', 'Unit_position_event_prescaler', 'Enable_edge_capture_unit', 'Enable_unit_timer', 'Unit_Timer_Period', 'Capture_timer_OP', 'Capture_period_OP', 'Capture_timer_latch_OP', 'Capture_period_latch_OP'});
                
                EQEP_Watchdog_Timer = matlab.system.display.SectionGroup(...
                    'Title', 'EQEP Watchdog Timer', ...
                    'PropertyList', {'Enable_EQEP_watchdog', 'EQEP_Watchdog_Timer_Period', 'EQEP_Watchdog_Timer_Value', 'Watchdog_timer_OP'});

                Interrupt = matlab.system.display.SectionGroup(...
                    'Title', 'Interrupt', ...
                    'PropertyList', {'Use_Interrupt', 'Enable_Interrupt', 'Global_interrupt_flag', 'Position_counter_error', 'Quadrature_phase_error', 'Quadrature_direction_change', 'Watchdog_timeout_interrupt', 'Position_counter_underflow', 'Position_counter_overflow', 'Position_compare_ready', 'Position_compare_match', 'Strobe_event_latch_intr', 'Index_event_latch_interrupt', 'Unit_timeout_interrupt', 'QMA_error_interrupt'});

                SignalDataTypes = matlab.system.display.SectionGroup(...
                    'Title', 'Signal Data types', ...
                    'PropertyList', {'Dir_flag_datatype', 'Pos_cntr_datatype', 'Pos_cntr_latch_datatype', 'Indx_position_latch_datatype', 'Strobe_position_latch_datatype', 'Capture_timer_datatype', 'Capture_period_datatype', 'Capture_timer_latch_datatype', 'Capture_period_latch_datatype', 'Watchdog_timer_value_datatype'});
                groups = [General, Quadrature_Decoder_Unit, Position_Counter_Control_Unit, Edge_Capture_Unit, EQEP_Watchdog_Timer, Interrupt, SignalDataTypes];
  
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

                coder.cinclude('MW_EQEP.h');
                switch uint16(obj.EQEP_ModuleEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_EQEP0_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_EQEP1_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_EQEP2_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_EQEP0_U_BASE');
                end

                eqep_setup_struct = struct('base', obj.baseaddr, 'strbsrc', uint16(obj.Strobe_SourceEnum), 'invertQEPA', obj.Invert_QEPA_Polarity, 'invertQEPB', obj.Invert_QEPB_Polarity, 'invertIndex', obj.Invert_Index, 'invertStrobe', obj.Invert_Strobe, 'posCntrSrc', uint16(obj.Position_Counter_SourceEnum), 'resolution', uint16(obj.ResolutionEnum), 'swap', uint16(obj.Swap_QEPA_QEPBEnum), 'changeDirIndx', uint16(obj.Enable_direction_change_index), 'emulationMode', uint16(obj.Emulation_modeEnum), 'posCntrMode', uint16(obj.Position_counter_modeEnum), 'enableUnitTimer', uint16(obj.Enable_unit_timer), 'unitTimerPeriod', uint32(obj.Unit_Timer_Period), 'enableWatchdog', uint16(obj.Enable_EQEP_watchdog), 'QEPCaptureLatch', uint32(obj.QEP_Capture_LatchEnum), 'StrobeEventLatch', uint32(obj.Strobe_Event_LatchEnum), 'IndexEventLatch', uint32(obj.Index_Event_LatchEnum), 'QMAModuleMode', uint16(obj.QMA_Module_ModeAEnum), 'PositionStrobeEvents', uint16(obj.Position_strobe_eventsEnum), 'PositionIndexEvents', uint16(obj.Position_index_eventsEnum), 'SWInitPositionCntr', uint16(obj.Set_software_init_position_counter), 'initialPosition', uint32(obj.Position_counter_Init_Value), 'Enable_PositionCntr', uint16(obj.Enable_position_counter), 'Config_Pos_CmpUnit', uint16(obj.Configure_position_compare_unit), 'SPSEL', uint16(obj.Sync_output_pin_selectionEnum), 'Pos_Cmp_ShadowLoad', uint16(obj.Position_compare_shadow_loadEnum), 'compareValues', uint32(obj.Compare_Value), 'compareCycles', uint32(obj.Compare_Cycles), 'EnablePosCmpUnit', uint16(obj.Enable_position_compare_unit), 'Config_Edge_CapUnit', uint16(obj.Config_edge_capture_unit), 'Capture_Prescaler', uint16(obj.Capture_PrescalerEnum), 'evntPrescale', uint16(obj.Unit_position_event_prescalerEnum), 'Enable_edgecaptureunit', uint16(obj.Enable_edge_capture_unit), 'maxPosition', uint32(obj.Maximum_Position), 'posCntr', uint32(obj.Position_Counter), 'watchdogTimerPrd', uint16(obj.EQEP_Watchdog_Timer_Period), 'watchdogTimerValue', uint16(obj.EQEP_Watchdog_Timer_Value), 'QEPASource', uint16(obj.QEPA_SourceEnum), 'QEPBSource', uint16(obj.QEPB_SourceEnum), 'IndexSource', uint16(obj.QEPI_INDX_SourceEnum), 'Use_Interrupt', obj.Use_Interrupt, 'Enable_Interrupt', obj.Enable_Interrupt, 'Global_int_flag', obj.Global_interrupt_flag, 'Position_cntr_error', obj.Position_counter_error, 'Quad_phase_error', obj.Quadrature_phase_error, 'Quad_dir_change', obj.Quadrature_direction_change, 'Watchdog_timeout', obj.Watchdog_timeout_interrupt, 'Pos_cntr_underflow', obj.Position_counter_underflow, 'Pos_cntr_overflow', obj.Position_counter_overflow, 'Pos_cmp_ready', obj.Position_compare_ready, 'Pos_cmp_match', obj.Position_compare_match, 'Strobe_event_latch', obj.Strobe_event_latch_intr, 'Index_event_latch', obj.Index_event_latch_interrupt, 'Unit_timeout', obj.Unit_timeout_interrupt, 'QMA_error', obj.QMA_error_interrupt);
                coder.cstructname(eqep_setup_struct, 'EQEPSetupStruct', 'extern', 'HeaderFile', 'MW_EQEP.h');
                coder.ceval('EQEP_setup', coder.ref(eqep_setup_struct));
            end
        end
        
        function [varargout] = stepImpl(obj)   %#ok<MANU>
            
            if isempty(coder.target)
                % Place simulation output code here
            else
                % Call C-function implementing device output
                %y = coder.ceval('source_output');
                index = 1;
                x = zeros(1,10,'uint32');
                y = int16(0);
                index_arr = 1;

                eqep_step_struct = struct('base', obj.baseaddr, 'getPositionCounter', obj.Position_Cntr_OP, 'getDirectionFlagValue', obj.Direction_Flag_OP, 'CaptureTimerValue', obj.Capture_timer_OP, 'CapturePeriodValue', obj.Capture_period_OP, 'CaptureTimerLatchValue', obj.Capture_timer_latch_OP, 'CapturePeriodLatchValue', obj.Capture_period_latch_OP, 'PositionCounterLatchValue', obj.Position_cntr_latch_OP, 'WatchdogTimerValue', obj.Watchdog_timer_OP, 'IndexPositionLatchValue', obj.Indx_position_latch_OP, 'StrobePositionLatchValue', obj.Strobe_position_latch_OP, 'use_interrupt', obj.Use_Interrupt, 'enable_interrupt', obj.Enable_Interrupt, 'strobe_pos_latch_intr', obj.Strobe_event_latch_intr, 'indx_pos_latch_intr', obj.Index_event_latch_interrupt);
                coder.cstructname(eqep_step_struct, 'EQEPStepStruct', 'extern', 'HeaderFile','MW_EQEP.h');
                coder.ceval('EQEP_step', coder.ref(eqep_step_struct), coder.ref(x), coder.ref(y));

                if(obj.Direction_Flag_OP)
                    if isequal(obj.Dir_flag_datatype, 'double')
                        varargout{index} = double(y);
                    elseif isequal(obj.Dir_flag_datatype, 'single')
                        varargout{index} = single(y);
                    elseif isequal(obj.Dir_flag_datatype, 'int8')
                        varargout{index} = int8(y);
                    elseif isequal(obj.Dir_flag_datatype, 'uint8')
                        varargout{index} = uint8(y);
                    elseif isequal(obj.Dir_flag_datatype, 'int16')
                        varargout{index} = int16(y);
                    elseif isequal(obj.Dir_flag_datatype, 'uint16')
                        varargout{index} = uint16(y);
                    elseif isequal(obj.Dir_flag_datatype, 'int32')
                        varargout{index} = int32(y);
                    elseif isequal(obj.Dir_flag_datatype, 'uint32')
                        varargout{index} = uint32(y);
                    elseif isequal(obj.Dir_flag_datatype, 'boolean')
                        varargout{index} = boolean(y);
                    else
                        varargout{index} = int16(y);
                    end
                index = index + 1;
                end

                if(obj.Position_Cntr_OP)
                    if isequal(obj.Pos_cntr_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Pos_cntr_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Position_cntr_latch_OP)
                    if isequal(obj.Pos_cntr_latch_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Pos_cntr_latch_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Indx_position_latch_OP)
                    if isequal(obj.Indx_position_latch_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Indx_position_latch_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Strobe_position_latch_OP)
                    if isequal(obj.Strobe_position_latch_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Strobe_position_latch_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1; 
                 index_arr = index_arr + 1;
                end

                if(obj.Capture_timer_OP)
                    if isequal(obj.Capture_timer_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Capture_timer_datatype, 'boolean')
                        varargout{index} = boolean(x(indexindex_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Capture_period_OP)
                    if isequal(obj.Capture_period_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Capture_period_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Capture_timer_latch_OP)
                    if isequal(obj.Capture_timer_latch_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Capture_timer_latch_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr = index_arr + 1;
                end

                if(obj.Capture_period_latch_OP)
                    if isequal(obj.Capture_period_latch_datatype ,'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Capture_period_latch_datatype ,'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1; 
                 index_arr = index_arr + 1;
                end

                if(obj.Watchdog_timer_OP)
                    if isequal(obj.Watchdog_timer_value_datatype, 'double')
                        varargout{index} = double(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'single')
                        varargout{index} = single(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'int8')
                        varargout{index} = int8(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'uint8')
                        varargout{index} = uint8(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'int16')
                        varargout{index} = int16(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'uint16')
                        varargout{index} = uint16(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'int32')
                        varargout{index} = int32(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'uint32')
                        varargout{index} = uint32(x(index_arr));
                    elseif isequal(obj.Watchdog_timer_value_datatype, 'boolean')
                        varargout{index} = boolean(x(index_arr));
                    else
                        varargout{index} = uint32(x(index_arr));
                    end
                 index = index + 1;
                 index_arr
                end

         end
        end
        
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                if(obj.Enable_edge_capture_unit)
                    coder.ceval('EQEP_disableCapture', obj.baseaddr);
                end
                if(obj.Enable_position_compare_unit)
                    coder.ceval('EQEP_disableCompare', obj.baseaddr);
                end
                if(obj.Enable_unit_timer)
                    coder.ceval('EQEP_disableUnitTimer', obj.baseaddr);
                end
                if(obj.Enable_EQEP_watchdog)
                    coder.ceval('EQEP_disableWatchdog', obj.baseaddr);
                end
                coder.ceval('EQEP_disableModule', obj.baseaddr);
            end
        end

        function flag = isInactivePropertyImpl(obj,propertyName)

            if (strcmp(propertyName,'Dir_flag_datatype') && (obj.Direction_Flag_OP==false))
                flag=true;
            elseif (strcmp(propertyName, 'Pos_cntr_datatype') && (obj.Position_Cntr_OP==false))
                flag = true;
            elseif (strcmp(propertyName, 'Pos_cntr_latch_datatype') && (obj.Position_cntr_latch_OP==false))
                flag = true;
            elseif (strcmp(propertyName, 'Indx_position_latch_datatype') && (obj.Indx_position_latch_OP==false))
                flag = true;
            elseif (strcmp(propertyName, 'Strobe_position_latch_datatype') && (obj.Strobe_position_latch_OP==false))
                flag = true;
            elseif(strcmp(propertyName, 'Capture_timer_datatype') && (obj.Capture_timer_OP==false))
                flag = true;
            elseif(strcmp(propertyName, 'Capture_period_datatype') && (obj.Capture_period_OP==false))
                flag = true;
            elseif(strcmp(propertyName, 'Capture_timer_latch_datatype') && (obj.Capture_timer_latch_OP==false))
                flag = true;
            elseif(strcmp(propertyName, 'Capture_period_latch_datatype') && (obj.Capture_period_latch_OP==false))
                flag = true;
            elseif(strcmp(propertyName, 'Watchdog_timer_value_datatype') && (obj.Watchdog_timer_OP==false))
                flag = true;
            else
                flag = false;
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
            if(obj.Position_Cntr_OP)
                num = num + 1;
            end
            if(obj.Direction_Flag_OP)
                num = num + 1;
            end
            if(obj.Capture_timer_OP)
                num = num + 1;
            end
            if(obj.Capture_period_OP)
                num = num + 1;
            end
            if(obj.Capture_timer_latch_OP)
                num = num + 1;
            end
            if(obj.Capture_period_latch_OP)
                num = num + 1;
            end
            if(obj.Position_cntr_latch_OP)
                num = num + 1;
            end
            if(obj.Watchdog_timer_OP)
                num = num + 1;
            end
            if(obj.Indx_position_latch_OP)
                num = num + 1;
            end
            if(obj.Strobe_position_latch_OP)
                num = num + 1;
            end
        end

        function varargout = getOutputNamesImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num > 0
              varargout = cell(1,num);
              if(obj.Direction_Flag_OP)
                  varargout{indx} = 'DIR';
                  indx = indx + 1;
              end
              if(obj.Position_Cntr_OP)
                  varargout{indx} = 'POS';
                  indx = indx + 1;
              end
              if(obj.Position_cntr_latch_OP)
                  varargout{indx} = 'POSL';
                  indx = indx + 1;
              end
              if(obj.Indx_position_latch_OP)
                  varargout{indx} = 'IPOSL';
                  indx = indx + 1;
              end
              if(obj.Strobe_position_latch_OP)
                  varargout{indx} = 'SPOSL';
                  indx = indx + 1;
              end
              if(obj.Capture_timer_OP)
                  varargout{indx} = 'CAPT';
                  indx = indx + 1;
              end
              if(obj.Capture_period_OP)
                  varargout{indx} = 'CAPP';
                  indx = indx + 1;
              end
              if(obj.Capture_timer_latch_OP)
                  varargout{indx} = 'CAPTL';
                  indx = indx + 1;
              end
              if(obj.Capture_period_latch_OP)
                  varargout{indx} = 'CAPPL';
                  indx = indx + 1;
              end
              if(obj.Watchdog_timer_OP)
                  varargout{indx} = 'WTCHD';
                  indx = indx + 1;
              end
            end

        end
        
        function varargout = isOutputFixedSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num>0
                varargout = cell(1,num);
                if(obj.Direction_Flag_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Position_Cntr_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Position_cntr_latch_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Indx_position_latch_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Strobe_position_latch_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Capture_period_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_latch_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Capture_period_latch_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
              if(obj.Watchdog_timer_OP)
                  varargout{indx} = true;
                  indx = indx + 1;
              end
            end
        end
        % 
        % 
        function varargout = isOutputComplexImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num>0
                varargout = cell(1,num);
                if(obj.Direction_Flag_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Position_Cntr_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Position_cntr_latch_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Indx_position_latch_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Strobe_position_latch_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Capture_period_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_latch_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Capture_period_latch_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
              if(obj.Watchdog_timer_OP)
                  varargout{indx} = false;
                  indx = indx + 1;
              end
            end
        end
        
        function varargout = getOutputSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj)
            if num>0
                varargout = cell(1,num);
                if(obj.Direction_Flag_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Position_Cntr_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Position_cntr_latch_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Indx_position_latch_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Strobe_position_latch_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Capture_timer_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Capture_period_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Capture_timer_latch_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Capture_period_latch_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
              if(obj.Watchdog_timer_OP)
                  varargout{indx} = [1,1];
                  indx = indx + 1;
              end
            end
        end
        
        function varargout = getOutputDataTypeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if num>0
                varargout = cell(1,num);
                if(obj.Direction_Flag_OP)
                  varargout{indx} = obj.Dir_flag_datatype;
                  indx = indx + 1;
              end
              if(obj.Position_Cntr_OP)
                  varargout{indx} = obj.Pos_cntr_datatype;
                  indx = indx + 1;
              end
              if(obj.Position_cntr_latch_OP)
                  varargout{indx} = obj.Pos_cntr_latch_datatype;
                  indx = indx + 1;
              end
              if(obj.Indx_position_latch_OP)
                  varargout{indx} = obj.Indx_position_latch_datatype;
                  indx = indx + 1;
              end
              if(obj.Strobe_position_latch_OP)
                  varargout{indx} = obj.Strobe_position_latch_datatype;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_OP)
                  varargout{indx} = obj.Capture_timer_datatype;
                  indx = indx + 1;
              end
              if(obj.Capture_period_OP)
                  varargout{indx} = obj.Capture_period_datatype;
                  indx = indx + 1;
              end
              if(obj.Capture_timer_latch_OP)
                  varargout{indx} = obj.Capture_timer_latch_datatype;
                  indx = indx + 1;
              end
              if(obj.Capture_period_latch_OP)
                  varargout{indx} = obj.Capture_period_latch_datatype;
                  indx = indx + 1;
              end
              if(obj.Watchdog_timer_OP)
                  varargout{indx} = obj.Watchdog_timer_value_datatype;
                  indx = indx + 1;
              end
            end

        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'EQEP','Type 2P1'};
        end
        
    end
   
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'EQEP';
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
                addIncludeFiles(buildInfo, 'MW_EQEP.h', includeDir);
                addSourceFiles(buildInfo, 'MW_EQEP.c', srcDir);
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

