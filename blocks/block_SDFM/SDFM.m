classdef SDFM < matlab.System & ...
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
        SampleTime=0.1;
        %SDFM Instance
        SDFM_Instance = 'SDFM0';
        %Enable Filter Channel 
        Filter_Channel = 'Filter Channel 1';
        %Channel 1 SD Clock Source
        Filter1_ch1_sdclk_src = 'SD1 channel clock is the source';
        %SD Modulator Mode
        Filter1_SD_modulator_mode = 'Modulator clock is identical to data rate';
        %Enable Data Filter
        Filter1_enable_data_filter (1,1) logical = false;
        %Filter Type
        Filter1_filter_type = 'Digital filter with Sinc3 structure';
        %DOSR
        Filter1_DOSR = 256;
        %Data filter output representation
        Filter1_data_filter_output_rep = 'Filter output is in 16 bits 2 complement format';
        %Shift 32-bit filter output
        Filter1_Shift = 10;
        %Use PWM Synchronization
        Filter1_PWM_Synchronization (1,1) logical = false;
        %Enable External Reset
        Filter1_Enable_External_Reset (1,1) logical = false;
        %Source of SDSYNC Event
        Filter1_Source_SDSYNC_Event = 'SDFM sync source is PWM0 SOCA';
        %Clear wait for SYNC Flag manually
        Filter1_clear_wait_sync_flag (1,1) logical = false;
        %Use FIFO
        Filter1_Use_FIFO (1,1) logical = false;
        %SDFIFO Interrupt Level
        Filter1_SDFIFO_int_level = 'FIFO level empty';
        %Clear FIFO on SDSYNC event
        Filter1_Clear_FIFO_SDSYNC_event (1,1) logical = false;
        %Enable Comparator
        Filter1_Enable_Comparator (1,1) logical = false;
        %Filter Type
        Filter1_CompFilter_Type = 'Digital filter with Sinc3 structure';
        %COSR
        Filter1_comp_COSR = 32;
        %High level threshold 1
        Filter1_comp_highlvl_threshold1 = 32767;
        %High level threshold 2
        Filter1_comp_highlvl_threshold2 = 32767;
        %Use high threshold(Z)
        Filter1_use_high_thresholdz (1,1) logical = false;
        %High level threshold(Z)
        High_level_threshold_z = 32767;
        %Low level threshold 1
        Filter1_low_level_threshold1 = 0;
        %Low level threshold 2
        Filter1_low_level_threshold2 = 0;
        %Comparator Event1 Source Select
        Filter1_Comparator_event1_source_select = 'COMPH1 event is source';
        %Use CEVT1 Digital Filter
        Filter1_use_CEVT1_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter1_Initialize_digital_filter_CEVT1 (1,1) logical = false;
        %CEVT1 Prescale
        Filter1_CEVT1_prescale = 0;
        %CEVT1 Threshold
        Filter1_CEVT1_threshold = 0;
        %CEVT1 Sample Window
        Filter1_CEVT1_sample_window = 0;
        %Comparator Event2 Source Select
        Filter1_Comparator_event2_source_select = 'COMPL1 event is source';
        %Use CEVT2 Digital Filter
        Filter1_use_CEVT2_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter1_Initialize_digital_filter_CEVT2 (1,1) logical = false;
        %CEVT2 Prescale
        Filter1_CEVT2_prescale = 0;
        %CEVT2 Threshold
        Filter1_CEVT2_threshold = 0;
        %CEVT2 Sample Window
        Filter1_CEVT2_sample_window = 0;
        %Channel 2 SD Clock Source
        Filter2_ch2_sdclk_src = 'Source is respective channel clock';
        %SD Modulator Mode
        Filter2_SD_modulator_mode = 'Modulator clock is identical to data rate';
        %Enable Data Filter
        Filter2_enable_data_filter (1,1) logical = false;
        %Filter Type
        Filter2_filter_type = 'Digital filter with Sinc3 structure';
        %DOSR
        Filter2_DOSR = 256;
        %Data filter output representation
        Filter2_data_filter_output_rep = 'Filter output is in 16 bits 2 complement format';
        %Shift 32-bit filter output
        Filter2_Shift = 10;
        %Use PWM Synchronization
        Filter2_PWM_Synchronization (1,1) logical = false;
        %Enable External Reset
        Filter2_Enable_External_Reset (1,1) logical = false;
        %Source of SDSYNC Event
        Filter2_Source_SDSYNC_Event = 'SDFM sync source is PWM0 SOCA';
        %Clear wait for SYNC Flag manually
        Filter2_clear_wait_sync_flag (1,1) logical = false;
        %Use FIFO
        Filter2_Use_FIFO (1,1) logical = false;
        %SDFIFO Interrupt Level
        Filter2_SDFIFO_int_level = 'FIFO level empty';
        %Clear FIFO on SDSYNC event
        Filter2_Clear_FIFO_SDSYNC_event (1,1) logical = false;
        %Enable Comparator
        Filter2_Enable_Comparator (1,1) logical = false;
        %Filter Type
        Filter2_CompFilter_Type = 'Digital filter with Sinc3 structure';
        %COSR
        Filter2_comp_COSR = 32;
        %High level threshold 1
        Filter2_comp_highlvl_threshold1 = 32767;
        %High level threshold 2
        Filter2_comp_highlvl_threshold2 = 32767;
        %Use high threshold(Z)
        Filter2_use_high_thresholdz (1,1) logical = false;
        %High level threshold(Z)
        Filter2_High_level_threshold_z = 32767;
        %Low level threshold 1
        Filter2_low_level_threshold1 = 0;
        %Low level threshold 2
        Filter2_low_level_threshold2 = 0;
        %Comparator Event1 Source Select
        Filter2_Comparator_event1_source_select = 'COMPH1 event is source';
        %Use CEVT1 Digital Filter
        Filter2_use_CEVT1_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter2_Initialize_digital_filter_CEVT1 (1,1) logical = false;
        %CEVT1 Prescale
        Filter2_CEVT1_prescale = 0;
        %CEVT1 Threshold
        Filter2_CEVT1_threshold = 0;
        %CEVT1 Sample Window
        Filter2_CEVT1_sample_window = 0;
        %Comparator Event2 Source Select
        Filter2_Comparator_event2_source_select = 'COMPL1 event is source';
        %Use CEVT2 Digital Filter
        Filter2_use_CEVT2_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter2_Initialize_digital_filter_CEVT2 (1,1) logical = false;
        %CEVT2 Prescale
        Filter2_CEVT2_prescale = 0;
        %CEVT2 Threshold
        Filter2_CEVT2_threshold = 0;
        %CEVT2 Sample Window
        Filter2_CEVT2_sample_window = 0;
        %Channel 3 SD Clock Source
        Filter3_ch3_sdclk_src = 'Source is respective channel clock';
        %SD Modulator Mode
        Filter3_SD_modulator_mode = 'Modulator clock is identical to data rate';
        %Enable Data Filter
        Filter3_enable_data_filter (1,1) logical = false;
        %Filter Type
        Filter3_filter_type = 'Digital filter with Sinc3 structure';
        %DOSR
        Filter3_DOSR = 256;
        %Data filter output representation
        Filter3_data_filter_output_rep = 'Filter output is in 16 bits 2 complement format';
        %Shift 32-bit filter output
        Filter3_Shift = 10;
        %Use PWM Synchronization
        Filter3_PWM_Synchronization (1,1) logical = false;
        %Enable External Reset
        Filter3_Enable_External_Reset (1,1) logical = false;
        %Source of SDSYNC Event
        Filter3_Source_SDSYNC_Event = 'SDFM sync source is PWM0 SOCA';
        %Clear wait for SYNC Flag manually
        Filter3_clear_wait_sync_flag (1,1) logical = false;
        %Use FIFO
        Filter3_Use_FIFO (1,1) logical = false;
        %SDFIFO Interrupt Level
        Filter3_SDFIFO_int_level = 'FIFO level empty';
        %Clear FIFO on SDSYNC event
        Filter3_Clear_FIFO_SDSYNC_event (1,1) logical = false;
        %Enable Comparator
        Filter3_Enable_Comparator (1,1) logical = false;
        %Filter Type
        Filter3_CompFilter_Type = 'Digital filter with Sinc3 structure';
        %COSR
        Filter3_comp_COSR = 32;
        %High level threshold 1
        Filter3_comp_highlvl_threshold1 = 32767;
        %High level threshold 2
        Filter3_comp_highlvl_threshold2 = 32767;
        %Use high threshold(Z)
        Filter3_use_high_thresholdz (1,1) logical = false;
        %High level threshold(Z)
        Filter3_High_level_threshold_z = 32767;
        %Low level threshold 1
        Filter3_low_level_threshold1 = 0;
        %Low level threshold 2
        Filter3_low_level_threshold2 = 0;
        %Comparator Event1 Source Select
        Filter3_Comparator_event1_source_select = 'COMPH1 event is source';
        %Use CEVT1 Digital Filter
        Filter3_use_CEVT1_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter3_Initialize_digital_filter_CEVT1 (1,1) logical = false;
        %CEVT1 Prescale
        Filter3_CEVT1_prescale = 0;
        %CEVT1 Threshold
        Filter3_CEVT1_threshold = 0;
        %CEVT1 Sample Window
        Filter3_CEVT1_sample_window = 0;
        %Comparator Event2 Source Select
        Filter3_Comparator_event2_source_select = 'COMPL1 event is source';
        %Use CEVT2 Digital Filter
        Filter3_use_CEVT2_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter3_Initialize_digital_filter_CEVT2 (1,1) logical = false;
        %CEVT2 Prescale
        Filter3_CEVT2_prescale = 0;
        %CEVT2 Threshold
        Filter3_CEVT2_threshold = 0;
        %CEVT2 Sample Window
        Filter3_CEVT2_sample_window = 0;
        %Channel 4 SD Clock Source
        Filter4_ch4_sdclk_src = 'Source is respective channel clock';
        %SD Modulator Mode
        Filter4_SD_modulator_mode = 'Modulator clock is identical to data rate';
        %Enable Data Filter
        Filter4_enable_data_filter (1,1) logical = false;
        %Filter Type
        Filter4_filter_type = 'Digital filter with Sinc3 structure';
        %DOSR
        Filter4_DOSR = 256;
        %Data filter output representation
        Filter4_data_filter_output_rep = 'Filter output is in 16 bits 2 complement format';
        %Shift 32-bit filter output
        Filter4_Shift = 10;
        %Use PWM Synchronization
        Filter4_PWM_Synchronization (1,1) logical = false;
        %Enable External Reset
        Filter4_Enable_External_Reset (1,1) logical = false;
        %Source of SDSYNC Event
        Filter4_Source_SDSYNC_Event = 'SDFM sync source is PWM0 SOCA';
        %Clear wait for SYNC Flag manually
        Filter4_clear_wait_sync_flag (1,1) logical = false;
        %Use FIFO
        Filter4_Use_FIFO (1,1) logical = false;
        %SDFIFO Interrupt Level
        Filter4_SDFIFO_int_level = 'FIFO level empty';
        %Clear FIFO on SDSYNC event
        Filter4_Clear_FIFO_SDSYNC_event (1,1) logical = false;
        %Enable Comparator
        Filter4_Enable_Comparator (1,1) logical = false;
        %Filter Type
        Filter4_CompFilter_Type = 'Digital filter with Sinc3 structure';
        %COSR
        Filter4_comp_COSR = 32;
        %High level threshold 1
        Filter4_comp_highlvl_threshold1 = 32767;
        %High level threshold 2
        Filter4_comp_highlvl_threshold2 = 32767;
        %Use high threshold(Z)
        Filter4_use_high_thresholdz (1,1) logical = false;
        %High level threshold(Z)
        Filter4_High_level_threshold_z = 32767;
        %Low level threshold 1
        Filter4_low_level_threshold1 = 0;
        %Low level threshold 2
        Filter4_low_level_threshold2 = 0;
        %Comparator Event1 Source Select
        Filter4_Comparator_event1_source_select = 'COMPH1 event is source';
        %Use CEVT1 Digital Filter
        Filter4_use_CEVT1_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter4_Initialize_digital_filter_CEVT1 (1,1) logical = false;
        %CEVT1 Prescale
        Filter4_CEVT1_prescale = 0;
        %CEVT1 Threshold
        Filter4_CEVT1_threshold = 0;
        %CEVT1 Sample Window
        Filter4_CEVT1_sample_window = 0;
        %Comparator Event2 Source Select
        Filter4_Comparator_event2_source_select = 'COMPL1 event is source';
        %Use CEVT2 Digital Filter
        Filter4_use_CEVT2_digital_filter (1,1) logical = false;
        %Initialize digital filter
        Filter4_Initialize_digital_filter_CEVT2 (1,1) logical = false;
        %CEVT2 Prescale
        Filter4_CEVT2_prescale = 0;
        %CEVT2 Threshold
        Filter4_CEVT2_threshold = 0;
        %CEVT2 Sample Window
        Filter4_CEVT2_sample_window = 0;

        %Interrupts

        %Use SDFM Interrupts
        Use_SDFM_Interrupts (1,1) logical = false;
        %Enable Filter 1 Modulator Clock Failure Interrupt
        Filter1_ModClock_Interrupt (1,1) logical = false;
        %Enable Filter 2 Modulator Clock Failure Interrupt
        Filter2_ModClock_Interrupt (1,1) logical = false;
        %Enable Filter 3 Modulator Clock Failure Interrupt
        Filter3_ModClock_Interrupt (1,1) logical = false;
        %Enable Filter 4 Modulator Clock Failure Interrupt
        Filter4_ModClock_Interrupt (1,1) logical = false;
        %Enable Filter 1 SDFM Data Ready Interrupt
        Filter1_SDFM_Data_Ready (1,1) logical = false;
        %Enable Filter 2 SDFM Data Ready Interrupt
        Filter2_SDFM_Data_Ready (1,1) logical = false;
        %Enable Filter 3 SDFM Data Ready Interrupt
        Filter3_SDFM_Data_Ready (1,1) logical = false;
        %Enable Filter 4 SDFM Data Ready Interrupt
        Filter4_SDFM_Data_Ready (1,1) logical = false;
        %Enable Filter 1 Comparator Event 1 Interrupt
        Filter1_Comparator_Event1_Int (1,1) logical = false;
        %Enable Filter 2 Comparator Event 1 Interrupt
        Filter2_Comparator_Event1_Int (1,1) logical = false;
        %Enable Filter 3 Comparator Event 1 Interrupt
        Filter3_Comparator_Event1_Int (1,1) logical = false;
        %Enable Filter 4 Comparator Event 1 Interrupt
        Filter4_Comparator_Event1_Int (1,1) logical = false;
        %Enable Filter 1 Comparator Event 2 Interrupt
        Filter1_Comparator_Event2_Int (1,1) logical = false;
        %Enable Filter 2 Comparator Event 2 Interrupt
        Filter2_Comparator_Event2_Int (1,1) logical = false;
        %Enable Filter 3 Comparator Event 2 Interrupt
        Filter3_Comparator_Event2_Int (1,1) logical = false;
        %Enable Filter 4 Comparator Event 2 Interrupt
        Filter4_Comparator_Event2_Int (1,1) logical = false;
        %Filter 1 FIFO Overflow Interrupt
        Filter1_FIFO_Overflow_Int (1,1) logical = false;
        %Filter 2 FIFO Overflow Interrupt
        Filter2_FIFO_Overflow_Int (1,1) logical = false;
        %Filter 3 FIFO Overflow Interrupt
        Filter3_FIFO_Overflow_Int (1,1) logical = false;
        %Filter 4 FIFO Overflow Interrupt
        Filter4_FIFO_Overflow_Int (1,1) logical = false;
        %Filter 1 SDFM FIFO Interrupt
        Filter1_SDFM_FIFO_Int (1,1) logical = false;
        %Filter 2 SDFM FIFO Interrupt
        Filter2_SDFM_FIFO_Int (1,1) logical = false;
        %Filter 3 SDFM FIFO Interrupt
        Filter3_SDFM_FIFO_Int (1,1) logical = false;
        %Filter 4 SDFM FIFO Interrupt
        Filter4_SDFM_FIFO_Int (1,1) logical = false;

        %Enable Comparator Output
        Comparator_Output (1,1) logical = false;

    end
    
   properties (Access = private)
        % Pre-computed constants.
        baseAddr=0;
        
   end

   properties(Constant,Hidden)
      SDFM_InstanceSet = matlab.system.StringSet({'SDFM0', 'SDFM1'});
      Filter_ChannelSet = matlab.system.StringSet({'Filter Channel 1', 'Filter Channel 2', 'Filter Channel 3', 'Filter Channel 4'});
      Filter1_ch1_sdclk_srcSet = matlab.system.StringSet({'SD1 channel clock is the source'});
      Filter1_SD_modulator_modeSet = matlab.system.StringSet({'Modulator clock is identical to data rate'});
      Filter1_filter_typeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter1_data_filter_output_repSet = matlab.system.StringSet({'Filter output is in 32 bits 2 complement format', 'Filter output is in 16 bits 2 complement format'});
      Filter1_Source_SDSYNC_EventSet = matlab.system.StringSet({'SDFM sync source is PWM0 SOCA', 'SDFM sync source is PWM0 SOCB', 'SDFM sync source is PWM1 SOCA', 'SDFM sync source is PWM1 SOCB', 'SDFM sync source is PWM2 SOCA', 'SDFM sync source is PWM2 SOCB', 'SDFM sync source is PWM3 SOCA', 'SDFM sync source is PWM3 SOCB', 'SDFM sync source is PWM4 SOCA', 'SDFM sync source is PWM4 SOCB', 'SDFM sync source is PWM5 SOCA', 'SDFM sync source is PWM5 SOCB', 'SDFM sync source is PWM6 SOCA', 'SDFM sync source is PWM6 SOCB', 'SDFM sync source is PWM7 SOCA', 'SDFM sync source is PWM7 SOCB', 'SDFM sync source is PWM8 SOCA', 'SDFM sync source is PWM8 SOCB', 'SDFM sync source is PWM9 SOCA', 'SDFM sync source is PWM9 SOCB', 'SDFM sync source is PWM10 SOCA', 'SDFM sync source is PWM10 SOCB', 'SDFM sync source is PWM11 SOCA', 'SDFM sync source is PWM11 SOCB', 'SDFM sync source is PWM12 SOCA', 'SDFM sync source is PWM12 SOCB', 'SDFM sync source is PWM13 SOCA', 'SDFM sync source is PWM13 SOCB', 'SDFM sync source is PWM14 SOCA', 'SDFM sync source is PWM14 SOCB', 'SDFM sync source is PWM15 SOCA', 'SDFM sync source is PWM15 SOCB', 'SDFM sync source is PWM16 SOCA', 'SDFM sync source is PWM16 SOCB', 'SDFM sync source is PWM17 SOCA', 'SDFM sync source is PWM17 SOCB', 'SDFM sync source is PWM18 SOCA', 'SDFM sync source is PWM18 SOCB', 'SDFM sync source is PWM19 SOCA', 'SDFM sync source is PWM19 SOCB', 'SDFM sync source is PWM20 SOCA', 'SDFM sync source is PWM20 SOCB', 'SDFM sync source is PWM21 SOCA', 'SDFM sync source is PWM21 SOCB', 'SDFM sync source is PWM22 SOCA', 'SDFM sync source is PWM22 SOCB', 'SDFM sync source is PWM23 SOCA', 'SDFM sync source is PWM23 SOCB', 'SDFM sync source is PWM24 SOCA', 'SDFM sync source is PWM24 SOCB', 'SDFM sync source is PWM25 SOCA', 'SDFM sync source is PWM25 SOCB', 'SDFM sync source is PWM26 SOCA', 'SDFM sync source is PWM26 SOCB', 'SDFM sync source is PWM27 SOCA', 'SDFM sync source is PWM27 SOCB', 'SDFM sync source is PWM28 SOCA', 'SDFM sync source is PWM28 SOCB', 'SDFM sync source is PWM29 SOCA', 'SDFM sync source is PWM29 SOCB', 'SDFM sync source is PWM30 SOCA', 'SDFM sync source is PWM30 SOCB', 'SDFM sync source is PWM31 SOCA', 'SDFM sync source is PWM31 SOCB'});
      Filter1_SDFIFO_int_levelSet = matlab.system.StringSet({'FIFO level empty', 'Fifo level 1', 'Fifo level 2', 'Fifo level 3', 'Fifo level 4', 'Fifo level 5', 'Fifo level 6', 'Fifo level 7', 'Fifo level 8', 'Fifo level 9', 'Fifo level 10', 'Fifo level 11', 'Fifo level 12', 'Fifo level 13', 'Fifo level 14', 'Fifo level 15', 'Fifo level 16'});
      Filter1_CompFilter_TypeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter1_Comparator_event1_source_selectSet = matlab.system.StringSet({'COMPH1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'Either of COMPH2 or COMPL2 event'});
      Filter1_Comparator_event2_source_selectSet = matlab.system.StringSet({'COMPL1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'COMPL2 event is source'});
      Filter2_ch2_sdclk_srcSet = matlab.system.StringSet({'Source is respective channel clock', 'SD1 channel clock is the source'});
      Filter2_SD_modulator_modeSet = matlab.system.StringSet({'Modulator clock is identical to data rate'});
      Filter2_filter_typeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter2_data_filter_output_repSet = matlab.system.StringSet({'Filter output is in 32 bits 2 complement format', 'Filter output is in 16 bits 2 complement format'});
      Filter2_Source_SDSYNC_EventSet = matlab.system.StringSet({'SDFM sync source is PWM0 SOCA', 'SDFM sync source is PWM0 SOCB', 'SDFM sync source is PWM1 SOCA', 'SDFM sync source is PWM1 SOCB', 'SDFM sync source is PWM2 SOCA', 'SDFM sync source is PWM2 SOCB', 'SDFM sync source is PWM3 SOCA', 'SDFM sync source is PWM3 SOCB', 'SDFM sync source is PWM4 SOCA', 'SDFM sync source is PWM4 SOCB', 'SDFM sync source is PWM5 SOCA', 'SDFM sync source is PWM5 SOCB', 'SDFM sync source is PWM6 SOCA', 'SDFM sync source is PWM6 SOCB', 'SDFM sync source is PWM7 SOCA', 'SDFM sync source is PWM7 SOCB', 'SDFM sync source is PWM8 SOCA', 'SDFM sync source is PWM8 SOCB', 'SDFM sync source is PWM9 SOCA', 'SDFM sync source is PWM9 SOCB', 'SDFM sync source is PWM10 SOCA', 'SDFM sync source is PWM10 SOCB', 'SDFM sync source is PWM11 SOCA', 'SDFM sync source is PWM11 SOCB', 'SDFM sync source is PWM12 SOCA', 'SDFM sync source is PWM12 SOCB', 'SDFM sync source is PWM13 SOCA', 'SDFM sync source is PWM13 SOCB', 'SDFM sync source is PWM14 SOCA', 'SDFM sync source is PWM14 SOCB', 'SDFM sync source is PWM15 SOCA', 'SDFM sync source is PWM15 SOCB', 'SDFM sync source is PWM16 SOCA', 'SDFM sync source is PWM16 SOCB', 'SDFM sync source is PWM17 SOCA', 'SDFM sync source is PWM17 SOCB', 'SDFM sync source is PWM18 SOCA', 'SDFM sync source is PWM18 SOCB', 'SDFM sync source is PWM19 SOCA', 'SDFM sync source is PWM19 SOCB', 'SDFM sync source is PWM20 SOCA', 'SDFM sync source is PWM20 SOCB', 'SDFM sync source is PWM21 SOCA', 'SDFM sync source is PWM21 SOCB', 'SDFM sync source is PWM22 SOCA', 'SDFM sync source is PWM22 SOCB', 'SDFM sync source is PWM23 SOCA', 'SDFM sync source is PWM23 SOCB', 'SDFM sync source is PWM24 SOCA', 'SDFM sync source is PWM24 SOCB', 'SDFM sync source is PWM25 SOCA', 'SDFM sync source is PWM25 SOCB', 'SDFM sync source is PWM26 SOCA', 'SDFM sync source is PWM26 SOCB', 'SDFM sync source is PWM27 SOCA', 'SDFM sync source is PWM27 SOCB', 'SDFM sync source is PWM28 SOCA', 'SDFM sync source is PWM28 SOCB', 'SDFM sync source is PWM29 SOCA', 'SDFM sync source is PWM29 SOCB', 'SDFM sync source is PWM30 SOCA', 'SDFM sync source is PWM30 SOCB', 'SDFM sync source is PWM31 SOCA', 'SDFM sync source is PWM31 SOCB'});
      Filter2_SDFIFO_int_levelSet = matlab.system.StringSet({'FIFO level empty', 'Fifo level 1', 'Fifo level 2', 'Fifo level 3', 'Fifo level 4', 'Fifo level 5', 'Fifo level 6', 'Fifo level 7', 'Fifo level 8', 'Fifo level 9', 'Fifo level 10', 'Fifo level 11', 'Fifo level 12', 'Fifo level 13', 'Fifo level 14', 'Fifo level 15', 'Fifo level 16'});
      Filter2_CompFilter_TypeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter2_Comparator_event1_source_selectSet = matlab.system.StringSet({'COMPH1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'Either of COMPH2 or COMPL2 event'});
      Filter2_Comparator_event2_source_selectSet = matlab.system.StringSet({'COMPL1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'COMPL2 event is source'});
      Filter3_ch3_sdclk_srcSet = matlab.system.StringSet({'Source is respective channel clock', 'SD1 channel clock is the source'});
      Filter3_SD_modulator_modeSet = matlab.system.StringSet({'Modulator clock is identical to data rate'});
      Filter3_filter_typeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter3_data_filter_output_repSet = matlab.system.StringSet({'Filter output is in 32 bits 2 complement format', 'Filter output is in 16 bits 2 complement format'});
      Filter3_Source_SDSYNC_EventSet = matlab.system.StringSet({'SDFM sync source is PWM0 SOCA', 'SDFM sync source is PWM0 SOCB', 'SDFM sync source is PWM1 SOCA', 'SDFM sync source is PWM1 SOCB', 'SDFM sync source is PWM2 SOCA', 'SDFM sync source is PWM2 SOCB', 'SDFM sync source is PWM3 SOCA', 'SDFM sync source is PWM3 SOCB', 'SDFM sync source is PWM4 SOCA', 'SDFM sync source is PWM4 SOCB', 'SDFM sync source is PWM5 SOCA', 'SDFM sync source is PWM5 SOCB', 'SDFM sync source is PWM6 SOCA', 'SDFM sync source is PWM6 SOCB', 'SDFM sync source is PWM7 SOCA', 'SDFM sync source is PWM7 SOCB', 'SDFM sync source is PWM8 SOCA', 'SDFM sync source is PWM8 SOCB', 'SDFM sync source is PWM9 SOCA', 'SDFM sync source is PWM9 SOCB', 'SDFM sync source is PWM10 SOCA', 'SDFM sync source is PWM10 SOCB', 'SDFM sync source is PWM11 SOCA', 'SDFM sync source is PWM11 SOCB', 'SDFM sync source is PWM12 SOCA', 'SDFM sync source is PWM12 SOCB', 'SDFM sync source is PWM13 SOCA', 'SDFM sync source is PWM13 SOCB', 'SDFM sync source is PWM14 SOCA', 'SDFM sync source is PWM14 SOCB', 'SDFM sync source is PWM15 SOCA', 'SDFM sync source is PWM15 SOCB', 'SDFM sync source is PWM16 SOCA', 'SDFM sync source is PWM16 SOCB', 'SDFM sync source is PWM17 SOCA', 'SDFM sync source is PWM17 SOCB', 'SDFM sync source is PWM18 SOCA', 'SDFM sync source is PWM18 SOCB', 'SDFM sync source is PWM19 SOCA', 'SDFM sync source is PWM19 SOCB', 'SDFM sync source is PWM20 SOCA', 'SDFM sync source is PWM20 SOCB', 'SDFM sync source is PWM21 SOCA', 'SDFM sync source is PWM21 SOCB', 'SDFM sync source is PWM22 SOCA', 'SDFM sync source is PWM22 SOCB', 'SDFM sync source is PWM23 SOCA', 'SDFM sync source is PWM23 SOCB', 'SDFM sync source is PWM24 SOCA', 'SDFM sync source is PWM24 SOCB', 'SDFM sync source is PWM25 SOCA', 'SDFM sync source is PWM25 SOCB', 'SDFM sync source is PWM26 SOCA', 'SDFM sync source is PWM26 SOCB', 'SDFM sync source is PWM27 SOCA', 'SDFM sync source is PWM27 SOCB', 'SDFM sync source is PWM28 SOCA', 'SDFM sync source is PWM28 SOCB', 'SDFM sync source is PWM29 SOCA', 'SDFM sync source is PWM29 SOCB', 'SDFM sync source is PWM30 SOCA', 'SDFM sync source is PWM30 SOCB', 'SDFM sync source is PWM31 SOCA', 'SDFM sync source is PWM31 SOCB'});
      Filter3_SDFIFO_int_levelSet = matlab.system.StringSet({'FIFO level empty', 'Fifo level 1', 'Fifo level 2', 'Fifo level 3', 'Fifo level 4', 'Fifo level 5', 'Fifo level 6', 'Fifo level 7', 'Fifo level 8', 'Fifo level 9', 'Fifo level 10', 'Fifo level 11', 'Fifo level 12', 'Fifo level 13', 'Fifo level 14', 'Fifo level 15', 'Fifo level 16'});
      Filter3_CompFilter_TypeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter3_Comparator_event1_source_selectSet = matlab.system.StringSet({'COMPH1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'Either of COMPH2 or COMPL2 event'});
      Filter3_Comparator_event2_source_selectSet = matlab.system.StringSet({'COMPL1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'COMPL2 event is source'});
      Filter4_ch4_sdclk_srcSet = matlab.system.StringSet({'Source is respective channel clock', 'SD1 channel clock is the source'});
      Filter4_SD_modulator_modeSet = matlab.system.StringSet({'Modulator clock is identical to data rate'});
      Filter4_filter_typeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter4_data_filter_output_repSet = matlab.system.StringSet({'Filter output is in 32 bits 2 complement format', 'Filter output is in 16 bits 2 complement format'});
      Filter4_Source_SDSYNC_EventSet = matlab.system.StringSet({'SDFM sync source is PWM0 SOCA', 'SDFM sync source is PWM0 SOCB', 'SDFM sync source is PWM1 SOCA', 'SDFM sync source is PWM1 SOCB', 'SDFM sync source is PWM2 SOCA', 'SDFM sync source is PWM2 SOCB', 'SDFM sync source is PWM3 SOCA', 'SDFM sync source is PWM3 SOCB', 'SDFM sync source is PWM4 SOCA', 'SDFM sync source is PWM4 SOCB', 'SDFM sync source is PWM5 SOCA', 'SDFM sync source is PWM5 SOCB', 'SDFM sync source is PWM6 SOCA', 'SDFM sync source is PWM6 SOCB', 'SDFM sync source is PWM7 SOCA', 'SDFM sync source is PWM7 SOCB', 'SDFM sync source is PWM8 SOCA', 'SDFM sync source is PWM8 SOCB', 'SDFM sync source is PWM9 SOCA', 'SDFM sync source is PWM9 SOCB', 'SDFM sync source is PWM10 SOCA', 'SDFM sync source is PWM10 SOCB', 'SDFM sync source is PWM11 SOCA', 'SDFM sync source is PWM11 SOCB', 'SDFM sync source is PWM12 SOCA', 'SDFM sync source is PWM12 SOCB', 'SDFM sync source is PWM13 SOCA', 'SDFM sync source is PWM13 SOCB', 'SDFM sync source is PWM14 SOCA', 'SDFM sync source is PWM14 SOCB', 'SDFM sync source is PWM15 SOCA', 'SDFM sync source is PWM15 SOCB', 'SDFM sync source is PWM16 SOCA', 'SDFM sync source is PWM16 SOCB', 'SDFM sync source is PWM17 SOCA', 'SDFM sync source is PWM17 SOCB', 'SDFM sync source is PWM18 SOCA', 'SDFM sync source is PWM18 SOCB', 'SDFM sync source is PWM19 SOCA', 'SDFM sync source is PWM19 SOCB', 'SDFM sync source is PWM20 SOCA', 'SDFM sync source is PWM20 SOCB', 'SDFM sync source is PWM21 SOCA', 'SDFM sync source is PWM21 SOCB', 'SDFM sync source is PWM22 SOCA', 'SDFM sync source is PWM22 SOCB', 'SDFM sync source is PWM23 SOCA', 'SDFM sync source is PWM23 SOCB', 'SDFM sync source is PWM24 SOCA', 'SDFM sync source is PWM24 SOCB', 'SDFM sync source is PWM25 SOCA', 'SDFM sync source is PWM25 SOCB', 'SDFM sync source is PWM26 SOCA', 'SDFM sync source is PWM26 SOCB', 'SDFM sync source is PWM27 SOCA', 'SDFM sync source is PWM27 SOCB', 'SDFM sync source is PWM28 SOCA', 'SDFM sync source is PWM28 SOCB', 'SDFM sync source is PWM29 SOCA', 'SDFM sync source is PWM29 SOCB', 'SDFM sync source is PWM30 SOCA', 'SDFM sync source is PWM30 SOCB', 'SDFM sync source is PWM31 SOCA', 'SDFM sync source is PWM31 SOCB'});
      Filter4_SDFIFO_int_levelSet = matlab.system.StringSet({'FIFO level empty', 'Fifo level 1', 'Fifo level 2', 'Fifo level 3', 'Fifo level 4', 'Fifo level 5', 'Fifo level 6', 'Fifo level 7', 'Fifo level 8', 'Fifo level 9', 'Fifo level 10', 'Fifo level 11', 'Fifo level 12', 'Fifo level 13', 'Fifo level 14', 'Fifo level 15', 'Fifo level 16'});
      Filter4_CompFilter_TypeSet = matlab.system.StringSet({'Digital filter with sincFast structure', 'Digital filter with Sinc1 structure', 'Digital filter with Sinc2 structure', 'Digital filter with Sinc3 structure'});
      Filter4_Comparator_event1_source_selectSet = matlab.system.StringSet({'COMPH1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'Either of COMPH2 or COMPL2 event'});
      Filter4_Comparator_event2_source_selectSet = matlab.system.StringSet({'COMPL1 event is source', 'Either of COMPH1 or COMPL1 event', 'COMPH2 event is source', 'COMPL2 event is source'});
      
   end

   properties (Dependent, Access=protected)
      SDFM_InstanceEnum;
      Filter_ChannelEnum;
      Filter1_ch1_sdclk_srcEnum;
      Filter1_SD_modulator_modeEnum;
      Filter1_filter_typeEnum;
      Filter1_data_filter_output_repEnum;
      Filter1_Source_SDSYNC_EventEnum;
      Filter1_SDFIFO_int_levelEnum;
      Filter1_CompFilter_TypeEnum;
      Filter1_Comparator_event1_source_selectEnum;
      Filter1_Comparator_event2_source_selectEnum;
      Filter2_ch2_sdclk_srcEnum;
      Filter2_SD_modulator_modeEnum;
      Filter2_filter_typeEnum;
      Filter2_data_filter_output_repEnum;
      Filter2_Source_SDSYNC_EventEnum;
      Filter2_SDFIFO_int_levelEnum;
      Filter2_CompFilter_TypeEnum;
      Filter2_Comparator_event1_source_selectEnum;
      Filter2_Comparator_event2_source_selectEnum;
      Filter3_ch3_sdclk_srcEnum;
      Filter3_SD_modulator_modeEnum;
      Filter3_filter_typeEnum;
      Filter3_data_filter_output_repEnum;
      Filter3_Source_SDSYNC_EventEnum;
      Filter3_SDFIFO_int_levelEnum;
      Filter3_CompFilter_TypeEnum;
      Filter3_Comparator_event1_source_selectEnum;
      Filter3_Comparator_event2_source_selectEnum;
      Filter4_ch4_sdclk_srcEnum;
      Filter4_SD_modulator_modeEnum;
      Filter4_filter_typeEnum;
      Filter4_data_filter_output_repEnum;
      Filter4_Source_SDSYNC_EventEnum;
      Filter4_SDFIFO_int_levelEnum;
      Filter4_CompFilter_TypeEnum;
      Filter4_Comparator_event1_source_selectEnum;
      Filter4_Comparator_event2_source_selectEnum;
      
   end

    methods
        % Constructor
        function obj = SDFM(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.Filter_ChannelEnum(obj)
            if isequal(obj.Filter_Channel, 'Filter Channel 1')
                ret = uint16(0);
            elseif isequal(obj.Filter_Channel, 'Filter Channel 2')
                ret = uint16(1);
            elseif isequal(obj.Filter_Channel, 'Filter Channel 3')
                ret = uint16(2);
            elseif isequal(obj.Filter_Channel, 'Filter Channel 4')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SDFM_InstanceEnum(obj)
            if isequal(obj.SDFM_Instance, 'SDFM0')
                ret = uint16(0);
            elseif isequal(obj.SDFM_Instance, 'SDFM1')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter1_ch1_sdclk_srcEnum(obj)
            if isequal(obj.Filter1_ch1_sdclk_src, 'SD1 channel clock is the source')
                ret = uint32(1);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter1_SD_modulator_modeEnum(obj)
            if isequal(obj.Filter1_SD_modulator_mode, 'Modulator clock is identical to data rate')
                ret = uint32(0);
            else
                ret = uint32(1);
            end
        end

        function ret = get.Filter1_filter_typeEnum(obj)
            if isequal(obj.Filter1_filter_type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter1_filter_type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter1_filter_type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter1_filter_type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter1_data_filter_output_repEnum(obj)
            if isequal(obj.Filter1_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format')
                ret = uint16(1);
            elseif isequal(obj.Filter1_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format')
                ret = uint16(0);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter1_Source_SDSYNC_EventEnum(obj)
            if isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCA')
                ret = uint32(0);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCB')
                ret = uint32(1);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCA')
                ret = uint32(2);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCB')
                ret = uint32(3);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCA')
                ret = uint32(4);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCB')
                ret = uint32(5);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCA')
                ret = uint32(6);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCB')
                ret = uint32(7);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCA')
                ret = uint32(8);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCB')
                ret = uint32(9);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCA')
                ret = uint32(10);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCB')
                ret = uint32(11);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCA')
                ret = uint32(12);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCB')
                ret = uint32(13);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCA')
                ret = uint32(14);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCB')
                ret = uint32(15);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCA')
                ret = uint32(16);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCB')
                ret = uint32(17);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCA')
                ret = uint32(18);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCB')
                ret = uint32(19);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCA')
                ret = uint32(20);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCB')
                ret = uint32(21);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCA')
                ret = uint32(22);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCB')
                ret = uint32(23);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCA')
                ret = uint32(24);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCB')
                ret = uint32(25);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCA')
                ret = uint32(26);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCB')
                ret = uint32(27);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCA')
                ret = uint32(28);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCB')
                ret = uint32(29);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCA')
                ret = uint32(30);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCB')
                ret = uint32(31);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCA')
                ret = uint32(32);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCB')
                ret = uint32(33);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCA')
                ret = uint32(34);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCB')
                ret = uint32(35);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCA')
                ret = uint32(36);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCB')
                ret = uint32(37);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCA')
                ret = uint32(38);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCB')
                ret = uint32(39);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCA')
                ret = uint32(40);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCB')
                ret = uint32(41);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCA')
                ret = uint32(42);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCB')
                ret = uint32(43);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCA')
                ret = uint32(44);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCB')
                ret = uint32(45);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCA')
                ret = uint32(46);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCB')
                ret = uint32(47);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCA')
                ret = uint32(48);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCB')
                ret = uint32(49);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCA')
                ret = uint32(50);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCB')
                ret = uint32(51);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCA')
                ret = uint32(52);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCB')
                ret = uint32(53);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCA')
                ret = uint32(54);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCB')
                ret = uint32(55);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCA')
                ret = uint32(56);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCB')
                ret = uint32(57);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCA')
                ret = uint32(58);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCB')
                ret = uint32(59);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCA')
                ret = uint32(60);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCB')
                ret = uint32(61);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCA')
                ret = uint32(62);
            elseif isequal(obj.Filter1_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCB')
                ret = uint32(63);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter1_SDFIFO_int_levelEnum(obj)
            if isequal(obj.Filter1_SDFIFO_int_level, 'FIFO level empty')
                ret = uint16(0);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 1')
                ret = uint16(1);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 2')
                ret = uint16(2);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 3')
                ret = uint16(3);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 4')
                ret = uint16(4);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 5')
                ret = uint16(5);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 6')
                ret = uint16(6);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 7')
                ret = uint16(7);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 8')
                ret = uint16(8);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 9')
                ret = uint16(9);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 10')
                ret = uint16(10);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 11')
                ret = uint16(11);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 12')
                ret = uint16(12);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 13')
                ret = uint16(13);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 14')
                ret = uint16(14);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 15')
                ret = uint16(15);
            elseif isequal(obj.Filter1_SDFIFO_int_level, 'Fifo level 16')
                ret = uint16(16);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter1_CompFilter_TypeEnum(obj)
            if isequal(obj.Filter1_CompFilter_Type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter1_CompFilter_Type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter1_CompFilter_Type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter1_CompFilter_Type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter1_Comparator_event1_source_selectEnum(obj)
            if isequal(obj.Filter1_Comparator_event1_source_select, 'COMPH1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter1_Comparator_event1_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter1_Comparator_event1_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter1_Comparator_event1_source_select, 'Either of COMPH2 or COMPL2 event')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter1_Comparator_event2_source_selectEnum(obj)
            if isequal(obj.Filter1_Comparator_event2_source_select, 'COMPL1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter1_Comparator_event2_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter1_Comparator_event2_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter1_Comparator_event2_source_select, 'COMPL2 event is source')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter2_ch2_sdclk_srcEnum(obj)
            if isequal(obj.Filter2_ch2_sdclk_src, 'Source is respective channel clock')
                ret = uint32(0);
            elseif isequal(obj.Filter2_ch2_sdclk_src, 'SD1 channel clock is the source')
                ret = uint32(1);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter2_SD_modulator_modeEnum(obj)
            if isequal(obj.Filter2_SD_modulator_mode, 'Modulator clock is identical to data rate')
                ret = uint32(0);
            else
                ret = uint32(1);
            end
        end

        function ret = get.Filter2_filter_typeEnum(obj)
            if isequal(obj.Filter2_filter_type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter2_filter_type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter2_filter_type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter2_filter_type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter2_data_filter_output_repEnum(obj)
            if isequal(obj.Filter2_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format')
                ret = uint16(1);
            elseif isequal(obj.Filter2_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format')
                ret = uint16(0);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter2_Source_SDSYNC_EventEnum(obj)
            if isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCA')
                ret = uint32(0);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCB')
                ret = uint32(1);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCA')
                ret = uint32(2);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCB')
                ret = uint32(3);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCA')
                ret = uint32(4);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCB')
                ret = uint32(5);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCA')
                ret = uint32(6);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCB')
                ret = uint32(7);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCA')
                ret = uint32(8);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCB')
                ret = uint32(9);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCA')
                ret = uint32(10);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCB')
                ret = uint32(11);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCA')
                ret = uint32(12);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCB')
                ret = uint32(13);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCA')
                ret = uint32(14);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCB')
                ret = uint32(15);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCA')
                ret = uint32(16);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCB')
                ret = uint32(17);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCA')
                ret = uint32(18);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCB')
                ret = uint32(19);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCA')
                ret = uint32(20);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCB')
                ret = uint32(21);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCA')
                ret = uint32(22);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCB')
                ret = uint32(23);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCA')
                ret = uint32(24);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCB')
                ret = uint32(25);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCA')
                ret = uint32(26);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCB')
                ret = uint32(27);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCA')
                ret = uint32(28);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCB')
                ret = uint32(29);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCA')
                ret = uint32(30);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCB')
                ret = uint32(31);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCA')
                ret = uint32(32);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCB')
                ret = uint32(33);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCA')
                ret = uint32(34);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCB')
                ret = uint32(35);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCA')
                ret = uint32(36);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCB')
                ret = uint32(37);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCA')
                ret = uint32(38);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCB')
                ret = uint32(39);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCA')
                ret = uint32(40);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCB')
                ret = uint32(41);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCA')
                ret = uint32(42);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCB')
                ret = uint32(43);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCA')
                ret = uint32(44);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCB')
                ret = uint32(45);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCA')
                ret = uint32(46);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCB')
                ret = uint32(47);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCA')
                ret = uint32(48);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCB')
                ret = uint32(49);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCA')
                ret = uint32(50);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCB')
                ret = uint32(51);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCA')
                ret = uint32(52);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCB')
                ret = uint32(53);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCA')
                ret = uint32(54);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCB')
                ret = uint32(55);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCA')
                ret = uint32(56);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCB')
                ret = uint32(57);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCA')
                ret = uint32(58);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCB')
                ret = uint32(59);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCA')
                ret = uint32(60);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCB')
                ret = uint32(61);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCA')
                ret = uint32(62);
            elseif isequal(obj.Filter2_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCB')
                ret = uint32(63);
            else
                ret = uint32(0);
            end
        end
        function ret = get.Filter2_SDFIFO_int_levelEnum(obj)
            if isequal(obj.Filter2_SDFIFO_int_level, 'FIFO level empty')
                ret = uint16(0);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 1')
                ret = uint16(1);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 2')
                ret = uint16(2);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 3')
                ret = uint16(3);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 4')
                ret = uint16(4);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 5')
                ret = uint16(5);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 6')
                ret = uint16(6);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 7')
                ret = uint16(7);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 8')
                ret = uint16(8);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 9')
                ret = uint16(9);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 10')
                ret = uint16(10);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 11')
                ret = uint16(11);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 12')
                ret = uint16(12);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 13')
                ret = uint16(13);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 14')
                ret = uint16(14);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 15')
                ret = uint16(15);
            elseif isequal(obj.Filter2_SDFIFO_int_level, 'Fifo level 16')
                ret = uint16(16);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter2_CompFilter_TypeEnum(obj)
            if isequal(obj.Filter2_CompFilter_Type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter2_CompFilter_Type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter2_CompFilter_Type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter2_CompFilter_Type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter2_Comparator_event1_source_selectEnum(obj)
            if isequal(obj.Filter2_Comparator_event1_source_select, 'COMPH1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter2_Comparator_event1_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter2_Comparator_event1_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter2_Comparator_event1_source_select, 'Either of COMPH2 or COMPL2 event')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter2_Comparator_event2_source_selectEnum(obj)
            if isequal(obj.Filter2_Comparator_event2_source_select, 'COMPL1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter2_Comparator_event2_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter2_Comparator_event2_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter2_Comparator_event2_source_select, 'COMPL2 event is source')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter3_ch3_sdclk_srcEnum(obj)
            if isequal(obj.Filter3_ch3_sdclk_src, 'Source is respective channel clock')
                ret = uint32(0);
            elseif isequal(obj.Filter3_ch3_sdclk_src, 'SD1 channel clock is the source')
                ret = uint32(1);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter3_SD_modulator_modeEnum(obj)
            if isequal(obj.Filter3_SD_modulator_mode, 'Modulator clock is identical to data rate')
                ret = uint32(0);
            else
                ret = uint32(1);
            end
        end

        function ret = get.Filter3_filter_typeEnum(obj)
            if isequal(obj.Filter3_filter_type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter3_filter_type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter3_filter_type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter3_filter_type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter3_data_filter_output_repEnum(obj)
            if isequal(obj.Filter3_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format')
                ret = uint16(1);
            elseif isequal(obj.Filter3_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format')
                ret = uint16(0);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter3_Source_SDSYNC_EventEnum(obj)
            if isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCA')
                ret = uint32(0);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCB')
                ret = uint32(1);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCA')
                ret = uint32(2);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCB')
                ret = uint32(3);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCA')
                ret = uint32(4);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCB')
                ret = uint32(5);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCA')
                ret = uint32(6);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCB')
                ret = uint32(7);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCA')
                ret = uint32(8);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCB')
                ret = uint32(9);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCA')
                ret = uint32(10);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCB')
                ret = uint32(11);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCA')
                ret = uint32(12);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCB')
                ret = uint32(13);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCA')
                ret = uint32(14);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCB')
                ret = uint32(15);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCA')
                ret = uint32(16);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCB')
                ret = uint32(17);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCA')
                ret = uint32(18);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCB')
                ret = uint32(19);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCA')
                ret = uint32(20);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCB')
                ret = uint32(21);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCA')
                ret = uint32(22);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCB')
                ret = uint32(23);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCA')
                ret = uint32(24);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCB')
                ret = uint32(25);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCA')
                ret = uint32(26);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCB')
                ret = uint32(27);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCA')
                ret = uint32(28);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCB')
                ret = uint32(29);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCA')
                ret = uint32(30);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCB')
                ret = uint32(31);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCA')
                ret = uint32(32);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCB')
                ret = uint32(33);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCA')
                ret = uint32(34);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCB')
                ret = uint32(35);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCA')
                ret = uint32(36);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCB')
                ret = uint32(37);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCA')
                ret = uint32(38);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCB')
                ret = uint32(39);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCA')
                ret = uint32(40);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCB')
                ret = uint32(41);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCA')
                ret = uint32(42);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCB')
                ret = uint32(43);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCA')
                ret = uint32(44);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCB')
                ret = uint32(45);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCA')
                ret = uint32(46);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCB')
                ret = uint32(47);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCA')
                ret = uint32(48);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCB')
                ret = uint32(49);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCA')
                ret = uint32(50);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCB')
                ret = uint32(51);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCA')
                ret = uint32(52);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCB')
                ret = uint32(53);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCA')
                ret = uint32(54);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCB')
                ret = uint32(55);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCA')
                ret = uint32(56);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCB')
                ret = uint32(57);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCA')
                ret = uint32(58);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCB')
                ret = uint32(59);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCA')
                ret = uint32(60);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCB')
                ret = uint32(61);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCA')
                ret = uint32(62);
            elseif isequal(obj.Filter3_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCB')
                ret = uint32(63);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter3_SDFIFO_int_levelEnum(obj)
            if isequal(obj.Filter3_SDFIFO_int_level, 'FIFO level empty')
                ret = uint16(0);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 1')
                ret = uint16(1);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 2')
                ret = uint16(2);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 3')
                ret = uint16(3);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 4')
                ret = uint16(4);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 5')
                ret = uint16(5);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 6')
                ret = uint16(6);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 7')
                ret = uint16(7);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 8')
                ret = uint16(8);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 9')
                ret = uint16(9);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 10')
                ret = uint16(10);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 11')
                ret = uint16(11);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 12')
                ret = uint16(12);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 13')
                ret = uint16(13);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 14')
                ret = uint16(14);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 15')
                ret = uint16(15);
            elseif isequal(obj.Filter3_SDFIFO_int_level, 'Fifo level 16')
                ret = uint16(16);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter3_CompFilter_TypeEnum(obj)
            if isequal(obj.Filter3_CompFilter_Type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter3_CompFilter_Type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter3_CompFilter_Type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter3_CompFilter_Type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter3_Comparator_event1_source_selectEnum(obj)
            if isequal(obj.Filter3_Comparator_event1_source_select, 'COMPH1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter3_Comparator_event1_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter3_Comparator_event1_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter3_Comparator_event1_source_select, 'Either of COMPH2 or COMPL2 event')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter3_Comparator_event2_source_selectEnum(obj)
            if isequal(obj.Filter3_Comparator_event2_source_select, 'COMPL1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter3_Comparator_event2_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter3_Comparator_event2_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter3_Comparator_event2_source_select, 'COMPL2 event is source')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter4_ch4_sdclk_srcEnum(obj)
            if isequal(obj.Filter4_ch4_sdclk_src, 'Source is respective channel clock')
                ret = uint32(0);
            elseif isequal(obj.Filter4_ch4_sdclk_src, 'SD1 channel clock is the source')
                ret = uint32(1);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter4_SD_modulator_modeEnum(obj)
            if isequal(obj.Filter4_SD_modulator_mode, 'Modulator clock is identical to data rate')
                ret = uint32(0);
            else
                ret = uint32(1);
            end
        end

        function ret = get.Filter4_filter_typeEnum(obj)
            if isequal(obj.Filter4_filter_type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter4_filter_type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter4_filter_type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter4_filter_type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter4_data_filter_output_repEnum(obj)
            if isequal(obj.Filter4_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format')
                ret = uint16(1);
            elseif isequal(obj.Filter4_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format')
                ret = uint16(0);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter4_Source_SDSYNC_EventEnum(obj)
            if isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCA')
                ret = uint32(0);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM0 SOCB')
                ret = uint32(1);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCA')
                ret = uint32(2);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM1 SOCB')
                ret = uint32(3);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCA')
                ret = uint32(4);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM2 SOCB')
                ret = uint32(5);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCA')
                ret = uint32(6);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM3 SOCB')
                ret = uint32(7);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCA')
                ret = uint32(8);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM4 SOCB')
                ret = uint32(9);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCA')
                ret = uint32(10);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM5 SOCB')
                ret = uint32(11);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCA')
                ret = uint32(12);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM6 SOCB')
                ret = uint32(13);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCA')
                ret = uint32(14);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM7 SOCB')
                ret = uint32(15);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCA')
                ret = uint32(16);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM8 SOCB')
                ret = uint32(17);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCA')
                ret = uint32(18);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM9 SOCB')
                ret = uint32(19);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCA')
                ret = uint32(20);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM10 SOCB')
                ret = uint32(21);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCA')
                ret = uint32(22);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM11 SOCB')
                ret = uint32(23);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCA')
                ret = uint32(24);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM12 SOCB')
                ret = uint32(25);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCA')
                ret = uint32(26);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM13 SOCB')
                ret = uint32(27);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCA')
                ret = uint32(28);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM14 SOCB')
                ret = uint32(29);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCA')
                ret = uint32(30);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM15 SOCB')
                ret = uint32(31);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCA')
                ret = uint32(32);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM16 SOCB')
                ret = uint32(33);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCA')
                ret = uint32(34);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM17 SOCB')
                ret = uint32(35);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCA')
                ret = uint32(36);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM18 SOCB')
                ret = uint32(37);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCA')
                ret = uint32(38);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM19 SOCB')
                ret = uint32(39);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCA')
                ret = uint32(40);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM20 SOCB')
                ret = uint32(41);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCA')
                ret = uint32(42);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM21 SOCB')
                ret = uint32(43);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCA')
                ret = uint32(44);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM22 SOCB')
                ret = uint32(45);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCA')
                ret = uint32(46);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM23 SOCB')
                ret = uint32(47);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCA')
                ret = uint32(48);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM24 SOCB')
                ret = uint32(49);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCA')
                ret = uint32(50);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM25 SOCB')
                ret = uint32(51);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCA')
                ret = uint32(52);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM26 SOCB')
                ret = uint32(53);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCA')
                ret = uint32(54);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM27 SOCB')
                ret = uint32(55);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCA')
                ret = uint32(56);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM28 SOCB')
                ret = uint32(57);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCA')
                ret = uint32(58);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM29 SOCB')
                ret = uint32(59);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCA')
                ret = uint32(60);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM30 SOCB')
                ret = uint32(61);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCA')
                ret = uint32(62);
            elseif isequal(obj.Filter4_Source_SDSYNC_Event, 'SDFM sync source is PWM31 SOCB')
                ret = uint32(63);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter4_SDFIFO_int_levelEnum(obj)
            if isequal(obj.Filter4_SDFIFO_int_level, 'FIFO level empty')
                ret = uint16(0);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 1')
                ret = uint16(1);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 2')
                ret = uint16(2);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 3')
                ret = uint16(3);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 4')
                ret = uint16(4);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 5')
                ret = uint16(5);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 6')
                ret = uint16(6);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 7')
                ret = uint16(7);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 8')
                ret = uint16(8);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 9')
                ret = uint16(9);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 10')
                ret = uint16(10);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 11')
                ret = uint16(11);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 12')
                ret = uint16(12);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 13')
                ret = uint16(13);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 14')
                ret = uint16(14);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 15')
                ret = uint16(15);
            elseif isequal(obj.Filter4_SDFIFO_int_level, 'Fifo level 16')
                ret = uint16(16);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Filter4_CompFilter_TypeEnum(obj)
            if isequal(obj.Filter4_CompFilter_Type, 'Digital filter with sincFast structure')
                ret = 0x00;
            elseif isequal(obj.Filter4_CompFilter_Type, 'Digital filter with Sinc1 structure')
                ret = 0x10;
            elseif isequal(obj.Filter4_CompFilter_Type, 'Digital filter with Sinc2 structure')
                ret = 0x20;
            elseif isequal(obj.Filter4_CompFilter_Type, 'Digital filter with Sinc3 structure')
                ret = 0x30;
            else
                ret = 0x00;
            end
        end

        function ret = get.Filter4_Comparator_event1_source_selectEnum(obj)
            if isequal(obj.Filter4_Comparator_event1_source_select, 'COMPH1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter4_Comparator_event1_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter4_Comparator_event1_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter4_Comparator_event1_source_select, 'Either of COMPH2 or COMPL2 event')
                ret = uint32(3);
            else
                ret = uint32(0);
            end
        end

        function ret = get.Filter4_Comparator_event2_source_selectEnum(obj)
            if isequal(obj.Filter4_Comparator_event2_source_select, 'COMPL1 event is source')
                ret = uint32(0);
            elseif isequal(obj.Filter4_Comparator_event2_source_select, 'Either of COMPH1 or COMPL1 event')
                ret = uint32(1);
            elseif isequal(obj.Filter4_Comparator_event2_source_select, 'COMPH2 event is source')
                ret = uint32(2);
            elseif isequal(obj.Filter4_Comparator_event2_source_select, 'COMPL2 event is source')
                ret = uint32(3);
            else
                ret = uint32(0);
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
                'Title','Type 2.1 SDFM',...
                'Text', sprintf('%s\n%s','Configure the SDFM Block to output data collected.'),...
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
             'PropertyList',  {'SDFM_Instance', 'Filter_Channel', 'Filter1_ch1_sdclk_src', 'Filter1_SD_modulator_mode', 'Filter2_ch2_sdclk_src', 'Filter2_SD_modulator_mode', 'Filter3_ch3_sdclk_src', 'Filter3_SD_modulator_mode', 'Filter4_ch4_sdclk_src', 'Filter4_SD_modulator_mode', 'Comparator_Output', 'SampleTime'});

             Data_Filter = matlab.system.display.SectionGroup(...
             'Title', 'Data Filter', ...
             'PropertyList',  {'Filter1_enable_data_filter','Filter1_filter_type', 'Filter1_DOSR', 'Filter1_data_filter_output_rep', 'Filter1_Shift', 'Filter1_PWM_Synchronization', 'Filter1_Enable_External_Reset','Filter1_Source_SDSYNC_Event', 'Filter1_clear_wait_sync_flag', 'Filter1_Use_FIFO', 'Filter1_SDFIFO_int_level', 'Filter1_Clear_FIFO_SDSYNC_event', ...
             'Filter2_enable_data_filter', 'Filter2_filter_type', 'Filter2_DOSR', 'Filter2_data_filter_output_rep', 'Filter2_Shift', 'Filter2_PWM_Synchronization', 'Filter2_Enable_External_Reset', 'Filter2_Source_SDSYNC_Event', 'Filter2_clear_wait_sync_flag', 'Filter2_Use_FIFO', 'Filter2_SDFIFO_int_level', 'Filter2_Clear_FIFO_SDSYNC_event', ...
             'Filter3_enable_data_filter', 'Filter3_filter_type', 'Filter3_DOSR', 'Filter3_data_filter_output_rep', 'Filter3_Shift','Filter3_PWM_Synchronization', 'Filter3_Enable_External_Reset', 'Filter3_Source_SDSYNC_Event', 'Filter3_clear_wait_sync_flag', 'Filter3_Use_FIFO', 'Filter3_SDFIFO_int_level', 'Filter3_Clear_FIFO_SDSYNC_event', ...
             'Filter4_enable_data_filter', 'Filter4_filter_type', 'Filter4_DOSR', 'Filter4_data_filter_output_rep', 'Filter4_Shift', 'Filter4_PWM_Synchronization', 'Filter4_Enable_External_Reset', 'Filter4_Source_SDSYNC_Event', 'Filter4_clear_wait_sync_flag', 'Filter4_Use_FIFO', 'Filter4_SDFIFO_int_level', 'Filter4_Clear_FIFO_SDSYNC_event'});
             
             Comparator = matlab.system.display.SectionGroup(...
             'Title', 'Comparator Filter Settings', ...
             'PropertyList',  {'Filter1_Enable_Comparator', 'Filter1_CompFilter_Type', 'Filter1_comp_COSR', 'Filter1_comp_highlvl_threshold1', 'Filter1_comp_highlvl_threshold2', 'Filter1_use_high_thresholdz', 'High_level_threshold_z', 'Filter1_low_level_threshold1', 'Filter1_low_level_threshold2', 'Filter1_Comparator_event1_source_select', 'Filter1_use_CEVT1_digital_filter', 'Filter1_Initialize_digital_filter_CEVT1', 'Filter1_CEVT1_prescale', 'Filter1_CEVT1_threshold', 'Filter1_CEVT1_sample_window', 'Filter1_Comparator_event2_source_select', 'Filter1_use_CEVT2_digital_filter', 'Filter1_Initialize_digital_filter_CEVT2', 'Filter1_CEVT2_prescale', 'Filter1_CEVT2_threshold', 'Filter1_CEVT2_sample_window', ...
             'Filter2_Enable_Comparator', 'Filter2_CompFilter_Type', 'Filter2_comp_COSR', 'Filter2_comp_highlvl_threshold1', 'Filter2_comp_highlvl_threshold2', 'Filter2_use_high_thresholdz', 'Filter2_High_level_threshold_z', 'Filter2_low_level_threshold1', 'Filter2_low_level_threshold2', 'Filter2_Comparator_event1_source_select', 'Filter2_use_CEVT1_digital_filter', 'Filter2_Initialize_digital_filter_CEVT1', 'Filter2_CEVT1_prescale', 'Filter2_CEVT1_threshold', 'Filter2_CEVT1_sample_window', 'Filter2_Comparator_event2_source_select', 'Filter2_use_CEVT2_digital_filter', 'Filter2_Initialize_digital_filter_CEVT2', 'Filter2_CEVT2_prescale', 'Filter2_CEVT2_threshold', 'Filter2_CEVT2_sample_window', ...
             'Filter3_Enable_Comparator', 'Filter3_CompFilter_Type', 'Filter3_comp_COSR', 'Filter3_comp_highlvl_threshold1', 'Filter3_comp_highlvl_threshold2', 'Filter3_use_high_thresholdz', 'Filter3_High_level_threshold_z', 'Filter3_low_level_threshold1', 'Filter3_low_level_threshold2', 'Filter3_Comparator_event1_source_select', 'Filter3_use_CEVT1_digital_filter', 'Filter3_Initialize_digital_filter_CEVT1', 'Filter3_CEVT1_prescale', 'Filter3_CEVT1_threshold', 'Filter3_CEVT1_sample_window', 'Filter3_Comparator_event2_source_select', 'Filter3_use_CEVT2_digital_filter', 'Filter3_Initialize_digital_filter_CEVT2', 'Filter3_CEVT2_prescale', 'Filter3_CEVT2_threshold', 'Filter3_CEVT2_sample_window', ...
             'Filter4_Enable_Comparator', 'Filter4_CompFilter_Type', 'Filter4_comp_COSR', 'Filter4_comp_highlvl_threshold1', 'Filter4_comp_highlvl_threshold2', 'Filter4_use_high_thresholdz', 'Filter4_High_level_threshold_z', 'Filter4_low_level_threshold1', 'Filter4_low_level_threshold2', 'Filter4_Comparator_event1_source_select', 'Filter4_use_CEVT1_digital_filter', 'Filter4_Initialize_digital_filter_CEVT1', 'Filter4_CEVT1_prescale', 'Filter4_CEVT1_threshold', 'Filter4_CEVT1_sample_window', 'Filter4_Comparator_event2_source_select', 'Filter4_use_CEVT2_digital_filter', 'Filter4_Initialize_digital_filter_CEVT2', 'Filter4_CEVT2_prescale', 'Filter4_CEVT2_threshold', 'Filter4_CEVT2_sample_window'});
 
             Interrupt = matlab.system.display.SectionGroup(...
             'Title', ' Interrupt', ...
             'PropertyList',  {'Use_SDFM_Interrupts', 'Filter1_ModClock_Interrupt', 'Filter2_ModClock_Interrupt', 'Filter3_ModClock_Interrupt', 'Filter4_ModClock_Interrupt', 'Filter1_SDFM_Data_Ready', 'Filter2_SDFM_Data_Ready', 'Filter3_SDFM_Data_Ready', 'Filter4_SDFM_Data_Ready', 'Filter1_Comparator_Event1_Int', 'Filter2_Comparator_Event1_Int', 'Filter3_Comparator_Event1_Int', 'Filter4_Comparator_Event1_Int', 'Filter1_Comparator_Event2_Int', 'Filter2_Comparator_Event2_Int', 'Filter3_Comparator_Event2_Int', 'Filter4_Comparator_Event2_Int', 'Filter1_FIFO_Overflow_Int', 'Filter2_FIFO_Overflow_Int', 'Filter3_FIFO_Overflow_Int', 'Filter4_FIFO_Overflow_Int', 'Filter1_SDFM_FIFO_Int', 'Filter2_SDFM_FIFO_Int', 'Filter3_SDFM_FIFO_Int', 'Filter4_SDFM_FIFO_Int'});
             
             groups = [General, Data_Filter, Comparator, Interrupt];
            
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


        function setupImpl(obj)
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
               coder.cinclude('MW_SDFM.h');
                switch(obj.SDFM_InstanceEnum)
                    case 0
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_SDFM0_U_BASE');
                    case 1
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_SDFM1_U_BASE');
                    otherwise
                        obj.baseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_SDFM0_U_BASE');
                end

                sdfm_setup_struct = struct('base', uint32(obj.baseAddr),'filter_channel', obj.Filter_ChannelEnum, 'filter1_ch1_sdclk_src', obj.Filter1_ch1_sdclk_srcEnum , 'filter1_sd_modulator_mode', obj.Filter1_SD_modulator_modeEnum, 'filter1_enable_data_filter', obj.Filter1_enable_data_filter, 'filter1_filtertype', uint16(obj.Filter1_filter_typeEnum), 'filter1_dosr', uint16(obj.Filter1_DOSR), 'filter1_data_filter_output_rep', uint16(obj.Filter1_data_filter_output_repEnum), 'filter1_shift', uint16(obj.Filter1_Shift), 'filter1_pwm_synchronization', obj.Filter1_PWM_Synchronization, 'filter1_src_sdsync_event', obj.Filter1_Source_SDSYNC_EventEnum, 'filter1_clear_wait_sync_flag_manually', obj.Filter1_clear_wait_sync_flag, 'filter1_use_fifo', obj.Filter1_Use_FIFO, 'filter1_fifo_lvl', obj.Filter1_SDFIFO_int_levelEnum, 'filter1_clearfifo_sdsync', obj.Filter1_Clear_FIFO_SDSYNC_event, 'filter1_enable_comparator', obj.Filter1_Enable_Comparator, 'filter1_comparator_filter_type', uint16(obj.Filter1_CompFilter_TypeEnum), 'filter1_comparator_osr', uint16(obj.Filter1_comp_COSR), 'filter1_comparator_highlvl_threshold1', uint32(obj.Filter1_comp_highlvl_threshold1), 'filter1_comparator_highlvl_threshold2', uint32(obj.Filter1_comp_highlvl_threshold2), 'filter1_comparator_lowlvl_threshold1', uint32(obj.Filter1_low_level_threshold1), 'filter1_comparator_lowlvl_threshold2', uint32(obj.Filter1_low_level_threshold2), 'filter1_enable_highthresholdz', obj.Filter1_use_high_thresholdz, 'filter1_highthresholdz', uint16(obj.High_level_threshold_z), 'filter1_comparator_event1_src', obj.Filter1_Comparator_event1_source_selectEnum , 'filter1_use_cevt1', obj.Filter1_use_CEVT1_digital_filter, 'filter1_initialize_cevt1', obj.Filter1_Initialize_digital_filter_CEVT1, 'filter1_cevt1_prescale', uint16(obj.Filter1_CEVT1_prescale), 'filter1_cevt1_threshold', uint16(obj.Filter1_CEVT1_threshold), 'filter1_cevt1_samplewindow', uint16(obj.Filter1_CEVT1_sample_window), 'filter1_comparator_event2_src', obj.Filter1_Comparator_event2_source_selectEnum, 'filter1_use_cevt2', obj.Filter1_use_CEVT2_digital_filter, 'filter1_initialize_cevt2', obj.Filter1_Initialize_digital_filter_CEVT2, 'filter1_cevt2_prescale', uint16(obj.Filter1_CEVT2_prescale), 'filter1_cevt2_threshold', uint16(obj.Filter1_CEVT2_threshold), 'filter1_cevt2_samplewindow', uint16(obj.Filter1_CEVT2_sample_window), 'filter2_ch2_sdclk_src', obj.Filter2_ch2_sdclk_srcEnum , 'filter2_sd_modulator_mode', obj.Filter2_SD_modulator_modeEnum, 'filter2_enable_data_filter', obj.Filter2_enable_data_filter, 'filter2_filtertype', uint16(obj.Filter2_filter_typeEnum), 'filter2_dosr', uint16(obj.Filter2_DOSR), 'filter2_data_filter_output_rep', uint16(obj.Filter2_data_filter_output_repEnum), 'filter2_shift', uint16(obj.Filter2_Shift), 'filter2_pwm_synchronization', obj.Filter2_PWM_Synchronization, 'filter2_src_sdsync_event', obj.Filter2_Source_SDSYNC_EventEnum, 'filter2_clear_wait_sync_flag_manually', obj.Filter2_clear_wait_sync_flag, 'filter2_use_fifo', obj.Filter2_Use_FIFO, 'filter2_fifo_lvl', obj.Filter2_SDFIFO_int_levelEnum, 'filter2_clearfifo_sdsync', obj.Filter2_Clear_FIFO_SDSYNC_event, 'filter2_enable_comparator', obj.Filter2_Enable_Comparator, 'filter2_comparator_filter_type', uint16(obj.Filter2_CompFilter_TypeEnum), 'filter2_comparator_osr', uint16(obj.Filter2_comp_COSR), 'filter2_comparator_highlvl_threshold1', uint32(obj.Filter2_comp_highlvl_threshold1), 'filter2_comparator_highlvl_threshold2', uint32(obj.Filter2_comp_highlvl_threshold2), 'filter2_comparator_lowlvl_threshold1', uint32(obj.Filter2_low_level_threshold1), 'filter2_comparator_lowlvl_threshold2', uint32(obj.Filter2_low_level_threshold2), 'filter2_enable_highthresholdz', obj.Filter2_use_high_thresholdz, 'filter2_highthresholdz', uint16(obj.High_level_threshold_z), 'filter2_comparator_event1_src', obj.Filter2_Comparator_event1_source_selectEnum , 'filter2_use_cevt1', obj.Filter2_use_CEVT1_digital_filter, 'filter2_initialize_cevt1', obj.Filter2_Initialize_digital_filter_CEVT1, 'filter2_cevt1_prescale', uint16(obj.Filter2_CEVT1_prescale), 'filter2_cevt1_threshold', uint16(obj.Filter2_CEVT1_threshold), 'filter2_cevt1_samplewindow', uint16(obj.Filter2_CEVT1_sample_window), 'filter2_comparator_event2_src', obj.Filter2_Comparator_event2_source_selectEnum, 'filter2_use_cevt2', obj.Filter2_use_CEVT2_digital_filter, 'filter2_initialize_cevt2', obj.Filter2_Initialize_digital_filter_CEVT2, 'filter2_cevt2_prescale', uint16(obj.Filter2_CEVT2_prescale), 'filter2_cevt2_threshold', uint16(obj.Filter2_CEVT2_threshold), 'filter2_cevt2_samplewindow', uint16(obj.Filter2_CEVT2_sample_window), 'filter3_ch3_sdclk_src', obj.Filter3_ch3_sdclk_srcEnum , 'filter3_sd_modulator_mode', obj.Filter3_SD_modulator_modeEnum, 'filter3_enable_data_filter', obj.Filter3_enable_data_filter, 'filter3_filtertype', uint16(obj.Filter3_filter_typeEnum), 'filter3_dosr', uint16(obj.Filter3_DOSR), 'filter3_data_filter_output_rep', uint16(obj.Filter3_data_filter_output_repEnum), 'filter3_shift', uint16(obj.Filter3_Shift), 'filter3_pwm_synchronization', obj.Filter3_PWM_Synchronization, 'filter3_src_sdsync_event', obj.Filter3_Source_SDSYNC_EventEnum, 'filter3_clear_wait_sync_flag_manually', obj.Filter3_clear_wait_sync_flag, 'filter3_use_fifo', obj.Filter3_Use_FIFO, 'filter3_fifo_lvl', obj.Filter3_SDFIFO_int_levelEnum, 'filter3_clearfifo_sdsync', obj.Filter3_Clear_FIFO_SDSYNC_event, 'filter3_enable_comparator', obj.Filter3_Enable_Comparator, 'filter3_comparator_filter_type', uint16(obj.Filter3_CompFilter_TypeEnum), 'filter3_comparator_osr', uint16(obj.Filter3_comp_COSR), 'filter3_comparator_highlvl_threshold1', uint32(obj.Filter3_comp_highlvl_threshold1), 'filter3_comparator_highlvl_threshold2', uint32(obj.Filter3_comp_highlvl_threshold2), 'filter3_comparator_lowlvl_threshold1', uint32(obj.Filter3_low_level_threshold1), 'filter3_comparator_lowlvl_threshold2', uint32(obj.Filter3_low_level_threshold2), 'filter3_enable_highthresholdz', obj.Filter3_use_high_thresholdz, 'filter3_highthresholdz', uint16(obj.High_level_threshold_z), 'filter3_comparator_event1_src', obj.Filter3_Comparator_event1_source_selectEnum , 'filter3_use_cevt1', obj.Filter3_use_CEVT1_digital_filter, 'filter3_initialize_cevt1', obj.Filter3_Initialize_digital_filter_CEVT1, 'filter3_cevt1_prescale', uint16(obj.Filter3_CEVT1_prescale), 'filter3_cevt1_threshold', uint16(obj.Filter3_CEVT1_threshold), 'filter3_cevt1_samplewindow', uint16(obj.Filter3_CEVT1_sample_window), 'filter3_comparator_event2_src', obj.Filter3_Comparator_event2_source_selectEnum, 'filter3_use_cevt2', obj.Filter3_use_CEVT2_digital_filter, 'filter3_initialize_cevt2', obj.Filter3_Initialize_digital_filter_CEVT2, 'filter3_cevt2_prescale', uint16(obj.Filter3_CEVT2_prescale), 'filter3_cevt2_threshold', uint16(obj.Filter3_CEVT2_threshold), 'filter3_cevt2_samplewindow', uint16(obj.Filter3_CEVT2_sample_window), 'filter4_ch4_sdclk_src', obj.Filter4_ch4_sdclk_srcEnum , 'filter4_sd_modulator_mode', obj.Filter4_SD_modulator_modeEnum, 'filter4_enable_data_filter', obj.Filter4_enable_data_filter, 'filter4_filtertype', uint16(obj.Filter4_filter_typeEnum), 'filter4_dosr', uint16(obj.Filter4_DOSR), 'filter4_data_filter_output_rep', uint16(obj.Filter4_data_filter_output_repEnum), 'filter4_shift', uint16(obj.Filter4_Shift), 'filter4_pwm_synchronization', obj.Filter4_PWM_Synchronization, 'filter4_src_sdsync_event', obj.Filter4_Source_SDSYNC_EventEnum, 'filter4_clear_wait_sync_flag_manually', obj.Filter4_clear_wait_sync_flag, 'filter4_use_fifo', obj.Filter4_Use_FIFO, 'filter4_fifo_lvl', obj.Filter4_SDFIFO_int_levelEnum, 'filter4_clearfifo_sdsync', obj.Filter4_Clear_FIFO_SDSYNC_event, 'filter4_enable_comparator', obj.Filter4_Enable_Comparator, 'filter4_comparator_filter_type', uint16(obj.Filter4_CompFilter_TypeEnum), 'filter4_comparator_osr', uint16(obj.Filter4_comp_COSR), 'filter4_comparator_highlvl_threshold1', uint32(obj.Filter4_comp_highlvl_threshold1), 'filter4_comparator_highlvl_threshold2', uint32(obj.Filter4_comp_highlvl_threshold2), 'filter4_comparator_lowlvl_threshold1', uint32(obj.Filter4_low_level_threshold1), 'filter4_comparator_lowlvl_threshold2', uint32(obj.Filter4_low_level_threshold2), 'filter4_enable_highthresholdz', obj.Filter4_use_high_thresholdz, 'filter4_highthresholdz', uint16(obj.High_level_threshold_z), 'filter4_comparator_event1_src', obj.Filter4_Comparator_event1_source_selectEnum , 'filter4_use_cevt1', obj.Filter4_use_CEVT1_digital_filter, 'filter4_initialize_cevt1', obj.Filter4_Initialize_digital_filter_CEVT1, 'filter4_cevt1_prescale', uint16(obj.Filter4_CEVT1_prescale), 'filter4_cevt1_threshold', uint16(obj.Filter4_CEVT1_threshold), 'filter4_cevt1_samplewindow', uint16(obj.Filter4_CEVT1_sample_window), 'filter4_comparator_event2_src', obj.Filter4_Comparator_event2_source_selectEnum, 'filter4_use_cevt2', obj.Filter4_use_CEVT2_digital_filter, 'filter4_initialize_cevt2', obj.Filter4_Initialize_digital_filter_CEVT2, 'filter4_cevt2_prescale', uint16(obj.Filter4_CEVT2_prescale), 'filter4_cevt2_threshold', uint16(obj.Filter4_CEVT2_threshold), 'filter4_cevt2_samplewindow', uint16(obj.Filter4_CEVT2_sample_window), 'use_interrupts', obj.Use_SDFM_Interrupts, 'filter1_modulator_clk_failure_int', obj.Filter1_ModClock_Interrupt, 'filter2_modulator_clk_failure_int', obj.Filter2_ModClock_Interrupt, 'filter3_modulator_clk_failure_int', obj.Filter3_ModClock_Interrupt, 'filter4_modulator_clk_failure_int', obj.Filter4_ModClock_Interrupt, 'filter1_comparator_event1_int', obj.Filter1_Comparator_Event1_Int, 'filter2_comparator_event1_int', obj.Filter2_Comparator_Event1_Int, 'filter3_comparator_event1_int', obj.Filter3_Comparator_Event1_Int, 'filter4_comparator_event1_int', obj.Filter4_Comparator_Event1_Int, 'filter1_comparator_event2_int', obj.Filter1_Comparator_Event2_Int, 'filter2_comparator_event2_int', obj.Filter2_Comparator_Event2_Int, 'filter3_comparator_event2_int', obj.Filter3_Comparator_Event2_Int, 'filter4_comparator_event2_int', obj.Filter4_Comparator_Event2_Int, 'filter1_fifo_overflow_int', obj.Filter1_FIFO_Overflow_Int, 'filter2_fifo_overflow_int', obj.Filter2_FIFO_Overflow_Int, 'filter3_fifo_overflow_int', obj.Filter3_FIFO_Overflow_Int, 'filter4_fifo_overflow_int', obj.Filter4_FIFO_Overflow_Int, 'filter1_data_ack_int', obj.Filter1_SDFM_Data_Ready,'filter2_data_ack_int', obj.Filter2_SDFM_Data_Ready, 'filter3_data_ack_int', obj.Filter3_SDFM_Data_Ready, 'filter4_data_ack_int', obj.Filter4_SDFM_Data_Ready, 'filter1_sdfm_fifo_int', obj.Filter1_SDFM_FIFO_Int, 'filter2_sdfm_fifo_int', obj.Filter2_SDFM_FIFO_Int, 'filter3_sdfm_fifo_int', obj.Filter3_SDFM_FIFO_Int, 'filter4_sdfm_fifo_int', obj.Filter4_SDFM_FIFO_Int, 'filt1_enable_externalreset', obj.Filter1_Enable_External_Reset, 'filt2_enable_externalreset', obj.Filter2_Enable_External_Reset, 'filt3_enable_externalreset', obj.Filter3_Enable_External_Reset, 'filt4_enable_externalreset', obj.Filter4_Enable_External_Reset);
                coder.cstructname(sdfm_setup_struct, 'SDFMSetupStruct', 'extern', 'HeaderFile', 'MW_SDFM.h');
                coder.ceval('SDFM_setup', coder.ref(sdfm_setup_struct));
               
               
           end
        end
        function [varargout] = stepImpl(obj)  
            
             
            if isempty(coder.target)
                % Place simulation output code here
            else
                
                if((obj.Filter_ChannelEnum == 0) && (obj.Filter1_data_filter_output_repEnum == 0))
                    y = int16(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 0) && (obj.Filter1_data_filter_output_repEnum == 1))
                    y = int32(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 1) && (obj.Filter2_data_filter_output_repEnum == 0))
                    y = int16(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 1) && (obj.Filter2_data_filter_output_repEnum == 1))
                    y = int32(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 2) && (obj.Filter3_data_filter_output_repEnum == 0))
                    y = int16(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 2) && (obj.Filter3_data_filter_output_repEnum == 1))
                    y = int32(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 3) && (obj.Filter4_data_filter_output_repEnum == 0))
                    y = int16(zeros(16,1));
                elseif((obj.Filter_ChannelEnum == 3) && (obj.Filter4_data_filter_output_repEnum == 1))
                    y = int32(zeros(16,1));
                else
                    y = int16(zeros(16,1));
                end


                x = uint8(0);
                a = uint8(0);
                b = uint32(0);
                c = uint16(0);
                sdfm_step_struct = struct('base', obj.baseAddr, 'filter_channel', obj.Filter_ChannelEnum, 'filter1_data_filter_output_rep', obj.Filter1_data_filter_output_repEnum, 'filter2_data_filter_output_rep', obj.Filter2_data_filter_output_repEnum, 'filter3_data_filter_output_rep', obj.Filter3_data_filter_output_repEnum, 'filter4_data_filter_output_rep', obj.Filter4_data_filter_output_repEnum, 'filter1_use_fifo', obj.Filter1_Use_FIFO, 'filter2_use_fifo', obj.Filter2_Use_FIFO, 'filter3_use_fifo', obj.Filter3_Use_FIFO, 'filter4_use_fifo', obj.Filter4_Use_FIFO, 'filter1_fifo_lvl', obj.Filter1_SDFIFO_int_levelEnum, 'filter2_fifo_lvl', obj.Filter2_SDFIFO_int_levelEnum, 'filter3_fifo_lvl', obj.Filter3_SDFIFO_int_levelEnum, 'filter4_fifo_lvl', obj.Filter4_SDFIFO_int_levelEnum, 'filt1_newdata_int', obj.Filter1_SDFM_Data_Ready, 'filt2_newdata_int', obj.Filter2_SDFM_Data_Ready, 'filt3_newdata_int', obj.Filter3_SDFM_Data_Ready, 'filt4_newdata_int', obj.Filter4_SDFM_Data_Ready, 'filt1_fifo_int', obj.Filter1_SDFM_FIFO_Int , 'filt2_fifo_int', obj.Filter2_SDFM_FIFO_Int , 'filt3_fifo_int', obj.Filter3_SDFM_FIFO_Int , 'filt4_fifo_int', obj.Filter4_SDFM_FIFO_Int);
                coder.cstructname(sdfm_step_struct, 'SDFMStepStruct', 'extern', 'HeaderFile', 'MW_SDFM.h');
                coder.ceval('SDFM_step', coder.ref(sdfm_step_struct), coder.wref(x), coder.wref(y), coder.wref(a), coder.wref(b), coder.wref(c));
                index = 1;
                varargout{index} = uint8(x);
                index = index + 1;
                varargout{index} = y(1:a);
                index = index + 1;
                if (obj.Comparator_Output == true)
                    varargout{index} = uint32(b);
                    index = index + 1;
                    varargout{index} = uint16(c);
                end
               
            end    
        end
        
        function releaseImpl(obj)
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                coder.ceval('SDFM_disableMainInterrupt', obj.baseAddr);
                coder.ceval('SDFM_disableMainFilter', obj.baseAddr);
             end
        end
        
        function flag = isInactivePropertyImpl(obj,propertyName)
                
            
            if ((strcmp(propertyName, 'Filter1_ch1_sdclk_src') | strcmp(propertyName, 'Filter1_SD_modulator_mode') | strcmp(propertyName, 'Filter1_enable_data_filter') | strcmp(propertyName, 'Filter1_Enable_Comparator')) && ~(isequal(obj.Filter_Channel, 'Filter Channel 1')))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_filter_type') | strcmp(propertyName, 'Filter1_data_filter_output_rep') | strcmp(propertyName, 'Filter1_DOSR') | strcmp(propertyName, 'Filter1_PWM_Synchronization') | strcmp(propertyName, 'Filter1_Shift') | isequal(propertyName, 'Filter1_Use_FIFO')) && ((~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_enable_data_filter == false))))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_Enable_External_Reset') | strcmp(propertyName, 'Filter1_Source_SDSYNC_Event') | strcmp(propertyName, 'Filter1_clear_wait_sync_flag')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_enable_data_filter == false) | (obj.Filter1_PWM_Synchronization == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_SDFIFO_int_level') | strcmp(propertyName, 'Filter1_Clear_FIFO_SDSYNC_event')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_enable_data_filter == false) | (obj.Filter1_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_CompFilter_Type') | strcmp(propertyName, 'Filter1_comp_COSR') | strcmp(propertyName, 'Filter1_comp_highlvl_threshold1') | strcmp(propertyName, 'Filter1_comp_highlvl_threshold2') | strcmp(propertyName, 'Filter1_use_high_thresholdz') | strcmp(propertyName, 'High_level_threshold_z') | strcmp(propertyName, 'Filter1_low_level_threshold1') | strcmp(propertyName, 'Filter1_low_level_threshold2') | strcmp(propertyName, 'Filter1_Comparator_event1_source_select') | strcmp(propertyName, 'Filter1_use_CEVT1_digital_filter') | strcmp(propertyName, 'Filter1_Comparator_event2_source_select') | strcmp(propertyName, 'Filter1_use_CEVT2_digital_filter')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_Initialize_digital_filter_CEVT1') | strcmp(propertyName, 'Filter1_CEVT1_prescale') | strcmp(propertyName, 'Filter1_CEVT1_threshold') | strcmp(propertyName, 'Filter1_CEVT1_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Enable_Comparator == false) | (obj.Filter1_use_CEVT1_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_Initialize_digital_filter_CEVT2') | strcmp(propertyName, 'Filter1_CEVT2_prescale') | strcmp(propertyName, 'Filter1_CEVT2_threshold') | strcmp(propertyName, 'Filter1_CEVT2_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Enable_Comparator == false) | (obj.Filter1_use_CEVT2_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_ch2_sdclk_src') | strcmp(propertyName, 'Filter2_SD_modulator_mode') | strcmp(propertyName, 'Filter2_enable_data_filter') | strcmp(propertyName, 'Filter2_Enable_Comparator')) && ~(isequal(obj.Filter_Channel, 'Filter Channel 2')))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_filter_type') | strcmp(propertyName, 'Filter2_data_filter_output_rep') | strcmp(propertyName, 'Filter2_DOSR') | strcmp(propertyName, 'Filter2_PWM_Synchronization') | strcmp(propertyName, 'Filter2_Shift') |isequal(propertyName, 'Filter2_Use_FIFO')) && ((~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_enable_data_filter == false))))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_Enable_External_Reset') | strcmp(propertyName, 'Filter2_Source_SDSYNC_Event') | strcmp(propertyName, 'Filter2_clear_wait_sync_flag')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_enable_data_filter == false) | (obj.Filter2_PWM_Synchronization == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_SDFIFO_int_level') | strcmp(propertyName, 'Filter2_Clear_FIFO_SDSYNC_event')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_enable_data_filter == false) | (obj.Filter2_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_CompFilter_Type') | strcmp(propertyName, 'Filter2_comp_COSR') | strcmp(propertyName, 'Filter2_comp_highlvl_threshold1') | strcmp(propertyName, 'Filter2_comp_highlvl_threshold2') | strcmp(propertyName, 'Filter2_use_high_thresholdz') | strcmp(propertyName, 'Filter2_High_level_threshold_z') | strcmp(propertyName, 'Filter2_low_level_threshold1') | strcmp(propertyName, 'Filter2_low_level_threshold2') | strcmp(propertyName, 'Filter2_Comparator_event1_source_select') | strcmp(propertyName, 'Filter2_use_CEVT1_digital_filter') | strcmp(propertyName, 'Filter2_Comparator_event2_source_select') | strcmp(propertyName, 'Filter2_use_CEVT2_digital_filter')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_Initialize_digital_filter_CEVT1') | strcmp(propertyName, 'Filter2_CEVT1_prescale') | strcmp(propertyName, 'Filter2_CEVT1_threshold') | strcmp(propertyName, 'Filter2_CEVT1_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Enable_Comparator == false) | (obj.Filter2_use_CEVT1_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_Initialize_digital_filter_CEVT2') | strcmp(propertyName, 'Filter2_CEVT2_prescale') | strcmp(propertyName, 'Filter2_CEVT2_threshold') | strcmp(propertyName, 'Filter2_CEVT2_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Enable_Comparator == false) | (obj.Filter2_use_CEVT2_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_ch3_sdclk_src') | strcmp(propertyName, 'Filter3_SD_modulator_mode') | strcmp(propertyName, 'Filter3_enable_data_filter') | strcmp(propertyName, 'Filter3_Enable_Comparator')) && ~(isequal(obj.Filter_Channel, 'Filter Channel 3')))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_filter_type') | strcmp(propertyName, 'Filter3_data_filter_output_rep') | strcmp(propertyName, 'Filter3_DOSR') | strcmp(propertyName, 'Filter3_PWM_Synchronization') | strcmp(propertyName, 'Filter3_Shift') | isequal(propertyName, 'Filter3_Use_FIFO')) && ((~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_enable_data_filter == false))))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_Enable_External_Reset') | strcmp(propertyName, 'Filter3_Source_SDSYNC_Event') | strcmp(propertyName, 'Filter3_clear_wait_sync_flag')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_enable_data_filter == false) | (obj.Filter3_PWM_Synchronization == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_SDFIFO_int_level') | strcmp(propertyName, 'Filter3_Clear_FIFO_SDSYNC_event')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_enable_data_filter == false) | (obj.Filter3_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_CompFilter_Type') | strcmp(propertyName, 'Filter3_comp_COSR') | strcmp(propertyName, 'Filter3_comp_highlvl_threshold1') | strcmp(propertyName, 'Filter3_comp_highlvl_threshold2') | strcmp(propertyName, 'Filter3_use_high_thresholdz') | strcmp(propertyName, 'Filter3_High_level_threshold_z') | strcmp(propertyName, 'Filter3_low_level_threshold1') | strcmp(propertyName, 'Filter3_low_level_threshold2') | strcmp(propertyName, 'Filter3_Comparator_event1_source_select') | strcmp(propertyName, 'Filter3_use_CEVT1_digital_filter') | strcmp(propertyName, 'Filter3_Comparator_event2_source_select') | strcmp(propertyName, 'Filter3_use_CEVT2_digital_filter')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_Initialize_digital_filter_CEVT1') | strcmp(propertyName, 'Filter3_CEVT1_prescale') | strcmp(propertyName, 'Filter3_CEVT1_threshold') | strcmp(propertyName, 'Filter3_CEVT1_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Enable_Comparator == false) | (obj.Filter3_use_CEVT1_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_Initialize_digital_filter_CEVT2') | strcmp(propertyName, 'Filter3_CEVT2_prescale') | strcmp(propertyName, 'Filter3_CEVT2_threshold') | strcmp(propertyName, 'Filter3_CEVT2_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Enable_Comparator == false) | (obj.Filter3_use_CEVT2_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_ch4_sdclk_src') | strcmp(propertyName, 'Filter4_SD_modulator_mode') | strcmp(propertyName, 'Filter4_enable_data_filter') | strcmp(propertyName, 'Filter4_Enable_Comparator')) && ~(isequal(obj.Filter_Channel, 'Filter Channel 4')))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_filter_type') | strcmp(propertyName, 'Filter4_data_filter_output_rep') | strcmp(propertyName, 'Filter4_DOSR') | strcmp(propertyName, 'Filter4_PWM_Synchronization') | strcmp(propertyName, 'Filter4_Shift') | isequal(propertyName, 'Filter4_Use_FIFO')) && ((~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_enable_data_filter == false))))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_Enable_External_Reset') | strcmp(propertyName, 'Filter4_Source_SDSYNC_Event') | strcmp(propertyName, 'Filter4_clear_wait_sync_flag')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_enable_data_filter == false) | (obj.Filter4_PWM_Synchronization == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_SDFIFO_int_level') | strcmp(propertyName, 'Filter4_Clear_FIFO_SDSYNC_event')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_enable_data_filter == false) | (obj.Filter4_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_CompFilter_Type') | strcmp(propertyName, 'Filter4_comp_COSR') | strcmp(propertyName, 'Filter4_comp_highlvl_threshold1') | strcmp(propertyName, 'Filter4_comp_highlvl_threshold2') | strcmp(propertyName, 'Filter4_use_high_thresholdz') | strcmp(propertyName, 'Filter4_High_level_threshold_z') | strcmp(propertyName, 'Filter4_low_level_threshold1') | strcmp(propertyName, 'Filter4_low_level_threshold2') | strcmp(propertyName, 'Filter4_Comparator_event1_source_select') | strcmp(propertyName, 'Filter4_use_CEVT1_digital_filter') | strcmp(propertyName, 'Filter4_Comparator_event2_source_select') | strcmp(propertyName, 'Filter4_use_CEVT2_digital_filter')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_Initialize_digital_filter_CEVT1') | strcmp(propertyName, 'Filter4_CEVT1_prescale') | strcmp(propertyName, 'Filter4_CEVT1_threshold') | strcmp(propertyName, 'Filter4_CEVT1_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Enable_Comparator == false) | (obj.Filter4_use_CEVT1_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_Initialize_digital_filter_CEVT2') | strcmp(propertyName, 'Filter4_CEVT2_prescale') | strcmp(propertyName, 'Filter4_CEVT2_threshold') | strcmp(propertyName, 'Filter4_CEVT2_sample_window')) && (~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Enable_Comparator == false) | (obj.Filter4_use_CEVT2_digital_filter == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_ModClock_Interrupt')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 1'))))
                flag = true;
            elseif (( strcmp(propertyName, 'Filter1_SDFM_Data_Ready')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Use_FIFO == true)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_ModClock_Interrupt')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 2'))))
                flag = true;
            elseif (( strcmp(propertyName, 'Filter2_SDFM_Data_Ready')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Use_FIFO == true)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_ModClock_Interrupt')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 3'))))
                flag = true;
            elseif (( strcmp(propertyName, 'Filter3_SDFM_Data_Ready')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Use_FIFO == true)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_ModClock_Interrupt')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 4'))))
                flag = true;
            elseif (( strcmp(propertyName, 'Filter4_SDFM_Data_Ready')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Use_FIFO == true)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_Comparator_Event1_Int') | strcmp(propertyName, 'Filter1_Comparator_Event2_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_Comparator_Event1_Int') | strcmp(propertyName, 'Filter2_Comparator_Event2_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_Comparator_Event1_Int') | strcmp(propertyName, 'Filter3_Comparator_Event2_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_Comparator_Event1_Int') | strcmp(propertyName, 'Filter4_Comparator_Event2_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Enable_Comparator == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter1_FIFO_Overflow_Int') | strcmp(propertyName, 'Filter1_SDFM_FIFO_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 1')) | (obj.Filter1_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter2_FIFO_Overflow_Int') | strcmp(propertyName, 'Filter2_SDFM_FIFO_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 2')) | (obj.Filter2_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter3_FIFO_Overflow_Int') | strcmp(propertyName, 'Filter3_SDFM_FIFO_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 3')) | (obj.Filter3_Use_FIFO == false)))
                flag = true;
            elseif ((strcmp(propertyName, 'Filter4_FIFO_Overflow_Int') | strcmp(propertyName, 'Filter4_SDFM_FIFO_Int')) && ((obj.Use_SDFM_Interrupts == false) | ~(isequal(obj.Filter_Channel, 'Filter Channel 4')) | (obj.Filter4_Use_FIFO == false)))
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
        end
        
        function num = getNumOutputsImpl(obj)
            num = 2;
            if(obj.Comparator_Output)
                num = num + 2;
            end
        end

     
        
        function varargout = isOutputFixedSizeImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
             if num>0
                varargout = cell(1,num);

                varargout{indx} = true;
                indx = indx + 1;
                varargout{indx} = false;
                indx = indx + 1;
                if(obj.Comparator_Output)
                    varargout{indx} = true;
                    indx = indx + 1;
                    varargout{indx} = true;
                end
             end
        end
        
        
        function varargout = isOutputComplexImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
             if num>0
                varargout = cell(1,num);
                varargout{indx} = false;
                indx = indx + 1;
                varargout{indx} = false;
                indx = indx + 1;
                if(obj.Comparator_Output == true)
                    varargout{indx} = false;
                    indx = indx + 1;
                    varargout{indx} = false;
                end
             end
        end
        
        function varargout = getOutputSizeImpl(obj)
            num = getNumOutputsImpl(obj);
             if num>0
                varargout = cell(1,num);
                varargout{1} = [1,1];
                varargout{2} = [16,1];
                if(obj.Comparator_Output == true)
                    varargout{3} = [1,1];
                    varargout{4} = [1,1];
                end
             end
        end
        
        function varargout = getOutputDataTypeImpl(obj)
           varargout{1} = "uint8";
           if(isequal(obj.Filter_Channel, 'Filter Channel 1') && isequal(obj.Filter1_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format'))
                varargout{2} = "int16";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 1') && isequal(obj.Filter1_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format'))
                varargout{2} = "int32";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 2') && isequal(obj.Filter2_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format'))
                varargout{2} = "int16";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 2') && isequal(obj.Filter2_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format'))
                varargout{2} = "int32";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 3') && isequal(obj.Filter3_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format'))
                varargout{2} = "int16";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 3') && isequal(obj.Filter3_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format'))
                varargout{2} = "int32";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 4') && isequal(obj.Filter4_data_filter_output_rep, 'Filter output is in 16 bits 2 complement format'))
                varargout{2} = "int16";
           elseif(isequal(obj.Filter_Channel, 'Filter Channel 4') && isequal(obj.Filter4_data_filter_output_rep, 'Filter output is in 32 bits 2 complement format'))
                varargout{2} = "int32";
           else
               varargout{2} = "int16";
           end

           if(obj.Comparator_Output == true)
               varargout{3} = "uint32";
               varargout{4} = "uint16";
           end
        end

        function varargout = getOutputNamesImpl(obj)
            indx = 1;
            num = getNumOutputsImpl(obj);
            if (num>0)
                varargout = cell(1,num);

                varargout{indx} = 'DFSTS';
                indx = indx + 1;
                varargout{indx} = 'DFLTX';
                indx = indx + 1;
                if (obj.Comparator_Output == true)
                    varargout{indx} = 'CMPSTS';
                    indx = indx + 1;
                    varargout{indx} = 'CMPD';
                end
            end
            
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'SDFM','Type 2.1'};
        end
      % function validatePropertiesImpl(obj)
      %   if (obj.SOCx_acquisition_Window<16)|| (obj.SOCx_acquisition_Window>512) 
      %     error("Sample window should lie in the range of 16 to 512");
      %   end
      % 
      % end
    end
    
    
    methods (Static)
        function name = getDescriptiveName()
            name = 'SDFM';
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
                addIncludeFiles(buildInfo, 'MW_SDFM.h', includeDir);
                addSourceFiles(buildInfo, 'MW_SDFM.c', srcDir);
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
