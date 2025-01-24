
 
classdef EPWM_AM263X < matlab.System & coder.ExternalDependency
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
        
         %Timer Period
         Timer_Period =0
         %Phase Shift Value
         Phase_Shift_Value = 0;
         %CMPA Value
         CMPAValue= 0;
         %CMPB Value
         CMPBValue= 0;
         %CMPC Value
         CMPCValue= 0;
         %CMPD Value
         CMPDValue= 0;
         %XCMP1 Active
         XCMP1_Active = 0;
         %XCMP2 Active
         XCMP2_Active = 0;
         %XCMP3 Active
         XCMP3_Active = 0;
         %XCMP4 Active
         XCMP4_Active = 0;
         %XCMP5 Active
         XCMP5_Active = 0;
         %XCMP6 Active
         XCMP6_Active = 0;
         %XCMP7 Active
         XCMP7_Active = 0;
         %XCMP8 Active
         XCMP8_Active = 0;
         %XCMP1 Shadow 1
         XCMP1_SHADOW1 = 0;
         %XCMP2 Shadow 1
         XCMP2_SHADOW1 = 0;
         %XCMP3 Shadow 1
         XCMP3_SHADOW1 = 0;
         %XCMP4 Shadow 1
         XCMP4_SHADOW1 = 0;
         %XCMP5 Shadow 1
         XCMP5_SHADOW1 = 0;
         %XCMP6 Shadow 1
         XCMP6_SHADOW1 = 0;
         %XCMP7 Shadow 1
         XCMP7_SHADOW1 = 0;
         %XCMP8 Shadow 1
         XCMP8_SHADOW1 = 0;
         %XCMP1 Shadow 2
         XCMP1_SHADOW2 = 0;
         %XCMP2 Shadow 2
         XCMP2_SHADOW2 = 0;
         %XCMP3 Shadow 2
         XCMP3_SHADOW2 = 0;
         %XCMP4 Shadow 2
         XCMP4_SHADOW2 = 0;
         %XCMP5 Shadow 2
         XCMP5_SHADOW2 = 0;
         %XCMP6 Shadow 2
         XCMP6_SHADOW2 = 0;
         %XCMP7 Shadow 2
         XCMP7_SHADOW2 = 0;
         %XCMP8 Shadow 2
         XCMP8_SHADOW2 = 0;
         %XCMP1 Shadow 3
         XCMP1_SHADOW3 = 0;
         %XCMP2 Shadow 3
         XCMP2_SHADOW3 = 0;
         %XCMP3 Shadow 3
         XCMP3_SHADOW3 = 0;
         %XCMP4 Shadow 3
         XCMP4_SHADOW3 = 0;
         %XCMP5 Shadow 3
         XCMP5_SHADOW3 = 0;
         %XCMP6 Shadow 3
         XCMP6_SHADOW3 = 0;
         %XCMP7 Shadow 3
         XCMP7_SHADOW3 = 0;
         %XCMP8 Shadow 3
         XCMP8_SHADOW3 = 0;

         
         
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.

        %Module
        ePWMModule = 'EPWM0';
        %Group
        ePWMGroup = 'Group 0';

        %Time Base Module

        %Emulation Mode
        EmulationMode = 'Stop after next Time Base Counter increaments or decreaments';
        %Time base clock (TBCLK) prescaler divider
        TBCLK = 'Divide clock by 1'; 
        %High speed time base clock (HSPCLKDIV) prescaler divide
        HSPCLKDIV = 'Divide clock by 1'; 
        %Timer Initial Period
        Timer_Initial_Period=0
        %Timer Period Units
        TimerPeriodUnits = 'Clock Cycles';
        % Specify Timer Period
        SpecifyTimerPeriod = 'Specify Via Dialog Box';
        %ePWMLink
        ePWMLink = 'Disable Linking';
        % Enable Time Base Period Global Load Mode
        Enable_TimeBase_Global_Load_Mode (1, 1) logical = false
        % Timebase Period Global Load Register
        GlobalRegisterLoad = 'EPWM_GL_REGISTER_TBPRD_TBPRDHR';
        % Set EPWM period load mode
        PeriodLoadMode = 'Using Shadow registers';
        % Set EPWM period load event (PRDLD)
        PeriodLoadEvent = 'Shadow to Active load when counter reaches zero';
        % Counter Mode
        CounterMode = 'Stop Freeze'; 
        %Counter Value
        CounterInitialValue = 0;
        % Enable phase shift load
        Phase_Shift_Load_Enable (1, 1) logical = false
        % Set sync in source
        SyncInPulseSrc = 'Disable Sync-in';
        % Set sync out pulse source

        %Software force generated EPWM sync-out pulse
        Software_Force_EPWM_SyncOut_Pulse (1, 1) logical = false
        %Counter zero event generates EPWM sync-out pulse
        Counter_Zero_EPWM_SyncOut_Pulse (1, 1) logical = false
        %Counter equal to CMPB event generates EPWM sync-out pulse
        Counter_CMPB_EPWM_SyncOut_Pulse (1, 1) logical = false
        %Counter equal to CMPC event generates EPWM sync-out pulse
        Counter_CMPC_EPWM_SyncOut_Pulse (1, 1) logical = false
        %Counter equal to CMPD event generates EPWM sync-out pulse
        Counter_CMPD_EPWM_SyncOut_Pulse (1, 1) logical = false
        %DCA Event1 sync signal generates EPWM sync-out pulse
        DCA_Event1_Sync_Signal (1, 1) logical = false
        %DCB Event1 sync signal generates EPWM sync-out signal
        DCB_Event1_Sync_Signal (1, 1) logical = false
        

        % One shot sync out trigger
        OneShotTrigger  = 'Trigger is one-shot sync';
        % Force sync pulse
        Force_Sync_Pulse (1, 1) logical = false

        % Counter Compare Module

        % ePWMLink CMPA
        ePWMLinkCMPA  = 'Link with ePWM0';     
        % CMPA units
        CMPAUnits  = 'Clock Cycles';
        %Specify CMPA via
        SpecifyCMPAVia = 'Specify Via Dialog Box';
        %CMPA Initial Value
        CMPA_Initial_Value = 0
        %Enable Global Load
        Enable_Global_Load_CMPA (1, 1) logical = false
        %Global Load Register Compare A
        GlobalLoadRegisterCMPA = 'EPWM_GL_REGISTER_CMPA_CMPAHR';
        %Enable Shadow Load Counter Compare A
        Enable_Shadow_Load_CMPA (1, 1) logical = true
        %Counter Compare A shadow load event
        SHDWAMODE = 'Load when counter equals zero'; 
        %Counter CMPA Module
        CounterCMPAModule = 0;
        %Enable linking with counter CMPA
        Enable_LinkCounterCMPA (1, 1) logical = false;
       


        %ePWMLink CMPB
        ePWMLinkCMPB = 'Link with ePWM0';
        %CMPB units
        CMPBUnits = 'Clock Cycles';           
        %Specify CMPB via
        SpecifyCMPBVia = 'Specify Via Dialog Box'; 
        %CMPB Initial Value
        CMPB_Initial_Value = 0
        %Enable Global Load
        Enable_Global_Load_CMPB (1, 1) logical = false
        GlobalLoadRegisterCMPB = 'EPWM_GL_REGISTER_CMPB_CMPBHR';
        %Enable Shadow Load Counter Compare B
        Enable_Shadow_Load_CMPB (1, 1) logical = true
        %Counter Compare B shadow load event
        SHDWBMODE = 'Load when counter equals zero';  
        %Counter CMPB Module
        CounterCMPBModule = 4;
        %Enable linking with counter CMPB
        Enable_LinkCounterCMPB (1, 1) logical = false;
        

        %ePWMLink CMPC
        ePWMLinkCMPC = 'Link with ePWM0';
        %CMPC units
        CMPCUnits = 'Clock Cycles';
        %Specify CMPC via
        SpecifyCMPCVia = 'Specify Via Dialog Box';      
        %CMPC Initial Value
        CMPC_Initial_Value = 0
        %Enable Global Load
        Enable_Global_Load_CMPC (1, 1) logical = false
        GlobalLoadRegisterCMPC = 'EPWM_GL_REGISTER_CMPC';
        %Enable Shadow Load Counter Compare C
        Enable_Shadow_Load_CMPC (1, 1) logical = true
        %Counter Compare C shadow load event
        SHDWCMODE = 'Load when counter equals zero';
        %Counter CMPC Module
        CounterCMPCModule = 10;
        %Enable linking with counter CMPC
        Enable_LinkCounterCMPC (1, 1) logical = false;

        %ePWMLink CMPD
        ePWMLinkCMPD = 'Link with ePWM0';
        %CMPD units
        CMPDUnits  = 'Clock Cycles';
        %Specify CMPD via
        SpecifyCMPDVia = 'Specify Via Dialog Box';  
        %CMPD Initial Value
        CMPD_Initial_Value = 0
        %Enable Global Load
        Enable_Global_Load_CMPD (1, 1) logical = false
        GlobalLoadRegisterCMPD  = 'EPWM_GL_REGISTER_CMPD';
        %Enable Shadow Load Counter Compare D
        Enable_Shadow_Load_CMPD (1, 1) logical = true
        %Counter Compare D shadow load event
        SHDWDMODE = 'Load when counter equals zero';
        %Counter CMPD Module
        CounterCMPDModule = 14;
        %Enable linking with counter CMPD
        Enable_LinkCounterCMPD (1, 1) logical = false;
        


        %Action Qualifier

        %Enable Continuous Software Force Global Load
        Enable_Software_Force_Global_Load (1, 1) logical = false
        %Continuous Software Force Global Load Register
        ContSWForceGlobalLoadRegister = 'EPWM_GL_REGISTER_AQCSFRC';
        %Continuous Software Force Shadow Mode
        SoftwareForceShadowMode = 'Shadow mode load when counter equals zero';
        %T1 Trigger Source
        T1TriggerSource = 'Digital compare event A 1';
        %T2 Trigger Source
        T2TriggerSource = 'Digital compare event A 1';
        
      
        
        %Action Qualifier module
        AQ_ModuleA = 0;
        %EPWMXA Global Load Enable
        EnableePWMXAGlobalLoad (1, 1) logical = false
        %EPWMXA Global Load Register
        ePWMXAGlobalLoadRegister = 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2';
        %EPWMXA Shadow Mode Enable
        Enable_ePWMXA_Shadow_Mode (1, 1) logical = false
        %EPWMXA Shadow Load Event
        ePWMXAShadowLoadEvent = 'Load when counter equals zero';
        %EPWMXA One-Time SW Force Action
        ePWMXAOneTimeSWForceAction ='No change in output pins';
        %EPWMXA Continuous SW Force Action
        ePWMXAContinuousSWForceAction = 'Software forcing disabled';
        %Events Configured for EPWMXA Output
        %Time base counter equals zero
        ePWMXA_TBCTR_Zero (1, 1) logical = false
        %Time base counter equals period
        ePWMXA_TBCTR_Period (1, 1) logical = false
        %Time base counter up equals CMPA
        ePWMXA_TBCTR_Up_CMPA (1, 1) logical = false
        %Time base counter down equals CMPA
        ePWMXA_TBCTR_Down_CMPA (1, 1) logical = false
        %Time base counter up equals CMPB
        ePWMXA_TBCTR_Up_CMPB (1, 1) logical = false
        %Time base counter down equals CMPB
        ePWMXA_TBCTR_Down_CMPB (1, 1) logical = false
        %T1 event on count up
        ePWMXA_T1_Event_Count_Up (1, 1) logical = false
        %T1 event on count down
        ePWMXA_T1_Event_Count_Down (1, 1) logical = false
        %T2 event on count up
        ePWMXA_T2_Event_Count_Up (1, 1) logical = false
        %T2 event on count down
        ePWMXA_T2_Event_Count_Down (1, 1) logical = false

        %EPWMA Output Event Configuration
       
        %EPWMXA Action when counter=ZERO
        ePWMXATbctrZero = 'No change in output pins';
        %EPWMXA Action when counter=period (PRD)
        ePWMXATbctrPrd = 'No change in output pins';
        %EPWMXA Action when counter=CMPA on up-count (CMPAU)
        ePWMXATbctrCMPAUp = 'No change in output pins'; 
        %EPWMXA Action when counter=CMPA on down-count (CMPAD)
        ePWMXATbctrCMPADown  = 'No change in output pins';
        %EPWMXA Action when counter=CMPB on up-count (CMPBU)
        ePWMXATbctrCMPBUp = 'No change in output pins';     
        %EPWMXA Action when counter=CMPB on down-count (CMPBD)
        ePWMXATbctrCMPBDown = 'No change in output pins'; 
        %EPWMXA Action when T1 event on Count Up
        ePWMXAT1Up = 'No change in output pins';
        %EPWMXA Action when T1 event on Count Down
        ePWMXAT1Down = 'No change in output pins';
        %EPWMXA Action when T2 event on Count Up
        ePWMXAT2Up = 'No change in output pins';
        %EPWMXA Action when T2 event on Count Down
        ePWMXAT2Down = 'No change in output pins';
        

        %EPWMXB OutputOutput Configuration

        %EPWMB Action Qualifier Module
        AQ_ModuleB = 2;
        %EPWMXB Global Load Enable
        EnableePWMXBGlobalLoad (1, 1) logical = false
        %EPWMXB Global Load Register
        ePWMXBGlobalLoadRegister = 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2';
        %EPWMXB Shadow Mode Enable
        Enable_ePWMXB_Shadow_Mode (1, 1) logical = false
        %EPWMXB Shadow Load Event
        ePWMXBShadowLoadEvent = 'Load when counter equals zero';
        %EPWMXB One-Time SW Force Action
        ePWMXBOneTimeSWForceAction = 'No change in output pins';
        %EPWMXB Continuous SW Force Action
        ePWMXBContinuousSWForceAction = 'Software forcing disabled';
        %Events Configured for EPWMXA Output
        %Time base counter equals zero
        ePWMXB_TBCTR_Zero (1, 1) logical = false
        %Time base counter equals period
        ePWMXB_TBCTR_Period (1, 1) logical = false
        %Time base counter up equals CMPA
        ePWMXB_TBCTR_Up_CMPA (1, 1) logical = false
        %Time base counter down equals CMPA
        ePWMXB_TBCTR_Down_CMPA (1, 1) logical = false
        %Time base counter up equals CMPB
        ePWMXB_TBCTR_Up_CMPB (1, 1) logical = false
        %Time base counter down equals CMPB
        ePWMXB_TBCTR_Down_CMPB (1, 1) logical = false
        %T1 event on count up
        ePWMXB_T1_Event_Count_Up (1, 1) logical = false
        %T1 event on count down
        ePWMXB_T1_Event_Count_Down (1, 1) logical = false
        %T2 event on count up
        ePWMXB_T2_Event_Count_Up (1, 1) logical = false
        %T2 event on count down
        ePWMXB_T2_Event_Count_Down (1, 1) logical = false

        %EPWMXB OutputEvent Output Configuration

        %EPWMXB Action when counter=ZERO
        ePWMXBTbctrZero = 'No change in output pins';
        %EPWMXB Action when counter=period (PRD)
        ePWMXBTbctrPrd = 'No change in output pins';
        %EPWMXB Action when counter=CMPA on up-count (CMPAU)
        ePWMXBTbctrCMPAUp = 'No change in output pins';
        %EPWMXB Action when counter=CMPA on down-count (CMPAD)
        ePWMXBTbctrCMPADown = 'No change in output pins';
        %EPWMXB Action when counter=CMPB on up-count (CMPBU)
        ePWMXBTbctrCMPBUp  = 'No change in output pins';     
        %EPWMXB Action when counter=CMPB on down-count (CMPBD)
        ePWMXBTbctrCMPBDown = 'No change in output pins';
        %EPWMXB Action when T1 event on Count Up
        ePWMXBT1Up  = 'No change in output pins';
        %EPWMXB Action when T1 event on Count Down
        ePWMXBT1Down  = 'No change in output pins';
        %EPWMXB Action when T2 event on Count Up
        ePWMXBT2Up  = 'No change in output pins';
        %EPWMXB Action when T2 event on Count Down
        ePWMXBT2Down = 'No change in output pins';


        %Trip Zone

        %Use Advanced EPWM Trip Zone Actions
        Use_Advanced_EPWM_Trip_Zone_Actions (1,1) logical= false
        %TZA Event
        TZAEvent = 'High impedance output';
        %TZB Event
        TZBEvent = 'High impedance output';
        %DCAEVT1 Event
        DCAEVT1Event = 'High impedance output';
        %DCAEVT2 Event
        DCAEVT2Event = 'High impedance output';
        %DCBEVT1 Event
        DCBEVT1Event = 'High impedance output';
        %DCBEVT2 Event
        DCBEVT2Event = 'High impedance output';
        %TZB_D Event (Adv)
        TZBDEvent = 'High impedance output';
        %TZB_U Event (Adv)
        TZBUEvent = 'High impedance output';
        %TZA_D Event (Adv)
        TZADEvent = 'High impedance output';
        %TZA_U Event (Adv)
        TZAUEvent = 'High impedance output';
        %DCAEVT1_U Event (Adv)
        DCAEVT1UEvent = 'High impedance output';
        %DCAEVT1_D Event (Adv)
        DCAEVT1DEvent = 'High impedance output';
        %DCAEVT2_U Event (Adv)
        DCAEVT2UEvent = 'High impedance output';
        %DCAEVT2_D Event (Adv)
        DCAEVT2DEvent = 'High impedance output';
        %DCBEVT1_U Event (Adv)
        DCBEVT1UEvent = 'High impedance output';
        %DCBEVT1_D Event (Adv)
        DCBEVT1DEvent = 'High impedance output';
        %DCBEVT2_U Event (Adv)
        DCBEVT2UEvent = 'High impedance output';
        %DCBEVT2_D Event (Adv)
        DCBEVT2DEvent = 'High impedance output';
        %One-Shot Source

        %Enable one-shot trip-zone1 (TZ1)
        One_Shot_TZ1(1, 1) logical  = false
        %Enable one-shot trip-zone2 (TZ2)
        One_Shot_TZ2 (1, 1) logical = false
        %Enable one-shot trip-zone3 (TZ3)
        One_Shot_TZ3 (1,1)  logical = false
        %Enable one-shot trip-zone4 (TZ4)
        One_Shot_TZ4 (1,1)  logical = false
        %Enable one-shot trip-zone5 (TZ5)
        One_Shot_TZ5 (1,1)  logical = false
        %Enable one-shot trip-zone6 (TZ6)
        One_Shot_TZ6 (1,1)  logical = false
        %Enable one-shot digital compare A event 1 (DCAEVT1)
        One_Shot_DCAEVT1 (1,1)  logical = false
        %Enable one-shot digital compare B event 1 (DCBEVT1)
        One_Shot_DCBEVT1 (1,1)  logical = false
        %CBC Source

        %Enable cyclic trip-zone1
        TZ1_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic trip-zone2
        TZ2_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic trip-zone3
        TZ3_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic trip-zone4
        TZ4_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic trip-zone5
        TZ5_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic trip-zone6
        TZ6_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic digital compare A event 2 (DCAEVT2)
        DCAEVT2_Cycle_By_Cycle (1,1) logical = false
        %Enable cyclic digital compare B event 2 (DCBEVT2)
        DCBEVT2_Cycle_By_Cycle (1,1) logical = false
        %CBC Latch Clear Signal
        CBCLatchClearSignal = 'Clear CBC Pulse when counter reaches zero';
        %TZ Interrupt Source (ORed)

        %Enable trip-zone cycle-by-cycle interrupt
        Trip_Zones_Cycle_By_Cycle_interrupt (1,1 )logical= false
        %Enable trip-zone one-shot interrupt
        Trip_Zones_One_Shot_interrupt (1,1) logical=false
        %Enable Digital comparator output A event 1 interrupt
        Digital_CompareA_Event1_interrupt (1,1) logical=false
        %Enable Digital comparator output A event 2 interrupt
        Digital_CompareA_Event2_interrupt (1,1) logical = false
        %Enable Digital comparator output B event 1 interrupt
        Digital_CompareB_Event1_interrupt (1,1) logical=false
        %Enable Digital comparator output B event 2 interrupt
        Digital_CompareB_Event2_interrupt (1,1) logical = false

        % Digital Compare Module

        %Source for digital compare A high signal
        DCAHigh = 'Trip 1';
        %Source for digital compare A low signal
        DCALow = 'Trip 1';
        %Digital compare output A event 1 selection
        DCOutput1ACondition = 'Event is disabled';
        %Digital compare output A event 2 selection
        DCOutput2ACondition  = 'Event is disabled';
        %Generate ADC SOC (DCAEVT1)
        ADC_SOC_Generate_DCAEVT1 (1, 1) logical = false
        %Generate SYNCOUT (DCAEVT1)
        SYNCOUT_Generate_DCAEVT1 (1, 1) logical = false
        %Digital compare A event 1 synch mode (DCAEVT1)
        SynchModeDCAEVT1 = 'DC Input signal is synced with TBCLK';
        %Digital compare A event 1 signal source (DCAEVT1)
        SignalSourceDCAEVT1  = 'Signal source is unfiltered';
        %Digital compare A event 1 cycle-by-cycle latch mode
        CBCLatchModeDCAEVT1 = 'DC CBC latch is disabled';
        %Event to clear digital compare A event 1 CBC latch
        CBCLatchClearEventDCAEVT1 = 'Clear CBC latch when counter equals zero';
        %Digital compare A event 2 synch mode (DCAEVT2)
        SynchModeDCAEVT2 = 'DC Input signal is synced with TBCLK';
        %Digital compare A event 2 signal source (DCAEVT2)
        SignalSourceDCAEVT2 = 'Signal source is unfiltered';
        %Digital compare A event 2 cycle-by-cycle latch mode
        CBCLatchModeDCAEVT2 = 'DC CBC latch is disabled';
        %Event to clear Digital compare A event 2 CBC latch
        CBCLatchClearEventDCAEVT2 = 'Clear CBC latch when counter equals zero';

        %Source for digital compare B high signal
        DCBHigh = 'Trip 1';
        %Source for digital compare B low signal
        DCBLow  = 'Trip 1';
        %Digital compare output B event 1 selection
        DCOutput1BCondition = 'Event is disabled';
        %Digital compare output B event 2 selection
        DCOutput2BCondition = 'Event is disabled';
        %Generate ADC SOC (DCBEVT1)
        ADC_SOC_Generate_DCBEVT1 (1, 1) logical = false
        %Generate SYNCOUT (DCBEVT1)
        SYNCOUT_Generate_DCBEVT1 (1, 1) logical = false
        %Digital compare B event 1 synch mode (DCBEVT1)
        SynchModeDCBEVT1  = 'DC Input signal is synced with TBCLK';
        %Digital compare B event 1 signal source (DCBEVT1)
        SignalSourceDCBEVT1 = 'Signal source is unfiltered';
        %Digital compare B event 1 cycle-by-cycle latch mode
        CBCLatchModeDCBEVT1 = 'DC CBC latch is disabled';
        %Event to clear digital compare B event 1 CBC latch
        CBCLatchClearEventDCBEVT1 = 'Clear CBC latch when counter equals zero';
        %Digital compare B event 2 synch mode (DCBEVT2)
        SynchModeDCBEVT2 = 'DC Input signal is synced with TBCLK';
        %Digital compare B event 2 signal source (DCBEVT2)
        SignalSourceDCBEVT2 = 'Signal source is unfiltered';
        %Digital compare B event 2 cycle-by-cycle latch mode
        CBCLatchModeDCBEVT2 = 'DC CBC latch is disabled';
        %Event to clear Digital compare B event 2 CBC latch
        CBCLatchClearEventDCBEVT2 = 'Clear CBC latch when counter equals zero';

        %DC filter input event source
        DCFilterInputEventSource = 'DC filter signal source is DCAEVT1';
        %Use blanking window
        Blanking_Window_Usage (1, 1) logical = false
        %Blanking window start event
        BlankingWindowStartEvent = 'Time base counter equals period';
        %Blanking window offset
        Blanking_Window_Offset = 0
        %Blanking window length
        Blanking_Window_Length = 0
        %Enable Blanking window inverted
        Invert_Blanking_Window (1, 1) logical = false
        %Enable counter capture
        Enable_DC_Counter_Capture (1, 1) logical = false
        %Enable counter capture shadow mode
        Enable_Counter_Capture_Shadow_Mode (1, 1) logical = false
        % DC counter capture shadow load event
        DCCounterCaptureEnableEvent =  'Time base counter equals period';
        %Enable usage of edge filter
        Use_Edge_Filter (1, 1) logical = false
        %Edge filter mode
        EdgeFilterMode = 'Digital Compare Edge Filter low';
        %Edge count to generate event
        EdgeFilterEdgeCount = 'No edge is required to generate event';
        %Enable edge filter valley capture
        Enable_Edge_Filter_Valley_Capture (1, 1) logical = false
        %Trigger event for edge filter valley capture signal
        EdgeFilterCounterCaptureSignal = 'Valley capture triggered by software';
        %Enable start valley capture
        Start_Valley_Capture_Logic (1, 1) logical = false
        %Set edge count for start valley capture logic
        StartValleyCapture = '0 Event';
        %Set edge count for stop valley capture logic
        StopValleyCapture = '0 Event';
        %Enable delayed output of edge filter
        Select_Delayed_Output_of_Edge_Filter (1, 1) logical = false
        %Set software valley delay value
        Software_Valley_Delay_Value = 0
        %Set valley delay divider
        ValleyDelayDivider = 'Delay value equals offset value defined by software';
        %Start mixed event for Time Base Counter Zero
        BlankingWindow_MixedEvent_TBCTRZero (1, 1) logical = false;
        %Start mixed event for Time Base Counter Period
        BlankingWindow_MixedEvent_TBCTRPrd (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPA increasing
        BlankingWindow_MixedEvent_TBCTRCMPA_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPA decreasing
        BlankingWindow_MixedEvent_TBCTRCMPA_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPB increasing
        BlankingWindow_MixedEvent_TBCTRCMPB_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPB decreasing
        BlankingWindow_MixedEvent_TBCTRCMPB_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPC increasing
        BlankingWindow_MixedEvent_TBCTRCMPC_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPC decreasing
        BlankingWindow_MixedEvent_TBCTRCMPC_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPD increasing
        BlankingWindow_MixedEvent_TBCTRCMPD_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPD decreasing
        BlankingWindow_MixedEvent_TBCTRCMPD_Dec (1, 1) logical = false;
        %Use Event Detection
        Use_Event_Detection (1, 1) logical = false;
        %Capture Input
        Capture_Input = 'Trip 1';
        %Invert Capture Input Polarity
        Invert_Capture_Input_Polarity = 'Capture input is not inverted';
        %Capture gate
        Capture_gate = 'Trip 1';
        %Capture gate polarity select
        Capture_gate_polarity_select = 'Capture gate is always on';
        %Combinational capture input Trip 1
        Combinational_capture_input_Trip1 (1, 1) logical = false;
        %Combinational capture input Trip 2
        Combinational_capture_input_Trip2 (1, 1) logical = false;
        %Combinational capture input Trip 3
        Combinational_capture_input_Trip3 (1, 1) logical = false;
        %Combinational capture input Trip 4
        Combinational_capture_input_Trip4 (1, 1) logical = false;
        %Combinational capture input Trip 5
        Combinational_capture_input_Trip5 (1, 1) logical = false;
        %Combinational capture input Trip 6
        Combinational_capture_input_Trip6 (1, 1) logical = false;
        %Combinational capture input Trip 7
        Combinational_capture_input_Trip7 (1, 1) logical = false;
        %Combinational capture input Trip 8
        Combinational_capture_input_Trip8 (1, 1) logical = false;
        %Combinational capture input Trip 9
        Combinational_capture_input_Trip9 (1, 1) logical = false;
        %Combinational capture input Trip 10
        Combinational_capture_input_Trip10 (1, 1) logical = false;
        %Combinational capture input Trip 11
        Combinational_capture_input_Trip11 (1, 1) logical = false;
        %Combinational capture input Trip 12
        Combinational_capture_input_Trip12 (1, 1) logical = false;
        %Combinational capture input Trip 13
        Combinational_capture_input_Trip13 (1, 1) logical = false;
        %Combinational capture input Trip 14
        Combinational_capture_input_Trip14 (1, 1) logical = false;
        %Combinational capture input Trip 15
        Combinational_capture_input_Trip15 (1, 1) logical = false;
        %Combinational capture gate Trip 1
        Combinational_capture_gate_Trip1 (1, 1) logical = false;
        %Combinational capture gate Trip 2
        Combinational_capture_gate_Trip2 (1, 1) logical = false;
        %Combinational capture gate Trip 3
        Combinational_capture_gate_Trip3 (1, 1) logical = false;
        %Combinational capture gate Trip 4
        Combinational_capture_gate_Trip4 (1, 1) logical = false;
        %Combinational capture gate Trip 5
        Combinational_capture_gate_Trip5 (1, 1) logical = false;
        %Combinational capture gate Trip 6
        Combinational_capture_gate_Trip6 (1, 1) logical = false;
        %Combinational capture gate Trip 7
        Combinational_capture_gate_Trip7 (1, 1) logical = false;
        %Combinational capture gate Trip 8
        Combinational_capture_gate_Trip8 (1, 1) logical = false;
        %Combinational capture gate Trip 9
        Combinational_capture_gate_Trip9 (1, 1) logical = false;
        %Combinational capture gate Trip 10
        Combinational_capture_gate_Trip10 (1, 1) logical = false;
        %Combinational capture gate Trip 11
        Combinational_capture_gate_Trip11 (1, 1) logical = false;
        %Combinational capture gate Trip 12
        Combinational_capture_gate_Trip12 (1, 1) logical = false;
        %Combinational capture gate Trip 13
        Combinational_capture_gate_Trip13 (1, 1) logical = false;
        %Combinational capture gate Trip 14
        Combinational_capture_gate_Trip14 (1, 1) logical = false;
        %Combinational capture gate Trip 15
        Combinational_capture_gate_Trip15 (1, 1) logical = false;

        

        %Dead-Band Module

        %Rising Edge Delay Input
        RisingEdgeDelayInput = 'Input signal is ePWMA';
        %Falling Edge Delay Input
        FallingEdgeDelayInput = 'Input signal is ePWMA';
        %Rising Edge Delay Polarity
        RisingEdgeDelayPolarity = 'DB polarity is not inverted'; 
        %Falling Edge Delay Polarity
        FallingEdgeDelayPolarity = 'DB polarity is not inverted';       
        %Enable Rising Edge Delay
        Enable_Rising_Edge_Delay (1, 1) logical = false
        %Rising Edge Delay Value
        Rising_Edge_Delay_Value=0
        %Enable Falling Edge Delay
        Enable_Falling_Edge_Delay(1, 1) logical = false
        %Falling Edge Delay Value
        Falling_Edge_Delay_Value=0
        %Swap Output for EPWMxA
        Swap_Output_for_EPWMxA (1, 1) logical = false 
        %Swap Output for EPWMxB
        Swap_Output_for_EPWMxB (1,1) logical= false
        %Enable Deadband Control Global Load
        Enable_Deadband_Control_Global_Load (1,1) logical=false
        % Global Control Register to Load into
        DeadbandGlobalLoad = 'EPWM_GL_REGISTER_DBCTL';
        %Enable Deadband Control Shadow Mode
        EnableDeadbandControlShadowMode (1,1) logical=false
        %Deadband Control Shadow Load Event
        DeadbandControlShadowMode = 'Load when counter equals zero';
        %Enable RED Global Load
        Enable_RED_Global_Load (1,1)logical= false
        %RED Global Load Regsiter
        REDGlobalLoadRegister = 'EPWM_GL_REGISTER_DBRED_DBREDHR';
        %Enable RED Shadow Mode
        Enable_RED_Shadow_Mode (1,1)logical=false
        %RED Shadow Load Event
        REDShadowLoadEvent = 'Load when counter equals zero';     
        %Enable FED Global Load
        Enable_FED_Global_Load(1,1) logical=false
        %FED Global Load Register
        FEDGlobalLoadRegister = 'EPWM_GL_REGISTER_DBFED_DBFEDHR';
        %Enable FED Shadow Mode
        Enable_FED_Shadow_Mode (1,1) logical=false
        %FED Shadow Load Event
        FEDShadowLoadEvent = 'Load when counter equals zero';      
        %Dead Band Counter Clock Rate
        DeadBandCounterClockRate = 'Deadband counter runs at TBCLK rate';

        %Chopper Module

        %Use Chopper
        Use_Chopper(1,1)logical=false
        %Chopper Duty Cycle 
        ChopperDutyCycle = '1/8 Duty';  
        %Chopper Frequency
        ChopperFrequency = 'SYSCLOCKOUT/1';  
        %Chopper First Pulse Width 
        ChopperFirstPulseWidth  = '1';

        %EPWM Event-Trigger

        %Enable EPWM interrupt
        Enable_EPWM_Interrupt (1, 1) logical = false
        %Event for interrupt generation
        InterruptEventSources  = 'Time base counter equal to zero';
        %Event count to generate interrupt
        InterruptEventCount = 'Disabled';
        %Enable interrupt event count initial value
        Enable_Interrupt_Event_Count_Initial_Value (1, 1) logical = false
        %Initial value for event counter
        InterruptEventCountInitialValue = 'Initialize event counter to 0';
        %Force interrupt event count value
        Force_Interrupt_Event_Count_Value (1, 1) logical = false;
        %Start mixed event for Time Base Counter Zero
        EventTrigger_MixedEvent_TBCTRZero (1, 1) logical = false;
        %Start mixed event for Time Base Counter Period
        EventTrigger_MixedEvent_TBCTRPrd (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPA increasing
        EventTrigger_MixedEvent_TBCTRCMPA_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPA decreasing
        EventTrigger_MixedEvent_TBCTRCMPA_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPB increasing
        EventTrigger_MixedEvent_TBCTRCMPB_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPB decreasing
        EventTrigger_MixedEvent_TBCTRCMPB_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPC increasing
        EventTrigger_MixedEvent_TBCTRCMPC_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPC decreasing
        EventTrigger_MixedEvent_TBCTRCMPC_Dec (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPD increasing
        EventTrigger_MixedEvent_TBCTRCMPD_Inc (1, 1) logical = false;
        %Start mixed event for Time Base Counter CMPD decreasing
        EventTrigger_MixedEvent_TBCTRCMPD_Dec (1, 1) logical = false;
        %Start mixed event for DCxEVT1
        EventTrigger_MixedEvent_DCxEVT1 (1, 1) logical = false;

        %ADC SOC Trigger

        %Enable ADC start of conversion for module A
        SOCA_Trigger_Enable (1, 1) logical = false
        %Start of conversion for module A event selection
        SOCATriggerSource = 'Event is based on DCxEVT1';
        %Number of event for start of conversion A to be generated
        SOCATriggerEventCount = 'Disabled';
        %Enable start of conversion A event count
        SOCA_Trigger_Event_Count_Init_Enable (1, 1) logical = false
        %Start of conversion A event count initial value
        SOCATriggerEventCountInitValue  = 'Initialize event counter to 0';
        %Force of start of conversion A event count
        Force_SOCA_Trigger_Event_Count (1, 1) logical = false
        %Enable ADC start of conversion for module B
        SOCB_Trigger_Enable (1, 1) logical = false
        %Start of conversion for module B event selection
        SOCBTriggerSource = 'Event is based on DCxEVT1';
        %Number of event for start of conversion B to be generated
        SOCBTriggerEventCount = 'Disabled';
        %Enable start of conversion B event count
        SOCB_Trigger_Event_Count_Init_Enable (1, 1) logical = false
        %Start of conversion B event count initial value
        SOCBTriggerEventCountInitValue = 'Initialize event counter to 0';
        %Force of start of conversion B event count
        Force_SOCB_Trigger_Event_Count (1, 1) logical = false
        %SOCA mixed event for Time Base Counter Zero
        SOCATrigger_MixedEvent_TBCTRZero (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter Period
        SOCATrigger_MixedEvent_TBCTRPrd (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPA increasing
        SOCATrigger_MixedEvent_TBCTRCMPA_Inc (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPA decreasing
        SOCATrigger_MixedEvent_TBCTRCMPA_Dec (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPB increasing
        SOCATrigger_MixedEvent_TBCTRCMPB_Inc (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPB decreasing
        SOCATrigger_MixedEvent_TBCTRCMPB_Dec (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPC increasing
        SOCATrigger_MixedEvent_TBCTRCMPC_Inc (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPC decreasing
        SOCATrigger_MixedEvent_TBCTRCMPC_Dec (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPD increasing
        SOCATrigger_MixedEvent_TBCTRCMPD_Inc (1, 1) logical = false;
        %SOCA mixed event for Time Base Counter CMPD decreasing
        SOCATrigger_MixedEvent_TBCTRCMPD_Dec (1, 1) logical = false;
        %SOCA mixed event for DCxEVT1
        SOCATrigger_MixedEvent_DCxEVT1 (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter Zero
        SOCBTrigger_MixedEvent_TBCTRZero (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter Period
        SOCBTrigger_MixedEvent_TBCTRPrd (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPA increasing
        SOCBTrigger_MixedEvent_TBCTRCMPA_Inc (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPA decreasing
        SOCBTrigger_MixedEvent_TBCTRCMPA_Dec (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPB increasing
        SOCBTrigger_MixedEvent_TBCTRCMPB_Inc (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPB decreasing
        SOCBTrigger_MixedEvent_TBCTRCMPB_Dec (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPC increasing
        SOCBTrigger_MixedEvent_TBCTRCMPC_Inc (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPC decreasing
        SOCBTrigger_MixedEvent_TBCTRCMPC_Dec (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPD increasing
        SOCBTrigger_MixedEvent_TBCTRCMPD_Inc (1, 1) logical = false;
        %SOCB mixed event for Time Base Counter CMPD decreasing
        SOCBTrigger_MixedEvent_TBCTRCMPD_Dec (1, 1) logical = false;
        %SOCB mixed event for DCxEVT1
        SOCBTrigger_MixedEvent_DCxEVT1 (1, 1) logical = false;

       

        %HRPWM Module

        %Enable high resolution pulse width modulator support
        Enable_HRPWM_Support (1, 1) logical = false;
        %Enable Automatic HRMSTEP Scaling
        Enable_Automatic_HRMSTEP_Scaling (1, 1) logical = false;
        %Enable high resolution PWM timing calculator
        Enable_HRPWM_Timing_Calculator (1, 1) logical = false;
        %High resolution PWM (HRPWM) control mode on ePWMxA
        ControlModeChannelA = 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge';
        %High resolution PWM (HRPWM) control mode on ePWMxB
        ControlModeChannelB = 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge';
        %Enable SFO Library
        Enable_SFO (1, 1) logical = false;
        %MEP Scale Factor
        MEP_Scale = 0;
        %HRPWM Timing Calculator

        %EPWMCLK (MHz)
        HRPWM_Timing_Calculator_EPWMCLK = 100
        %TBCLK (MHz)
        HRPWM_Timing_Calculator_TBCLK = 100
        %Required Period (nsec)
        HRPWM_Timing_Calculator_Required_Period = 800
        %Required Duty Cycle (%)
        HRPWM_Timing_Calculator_Required_Duty_Cycle = 50
        %Required Rising Edge Delay (nsec)
        HRPWM_Timing_Calculator_Required_Rising_Edge_Delay = 0
        %Required Falling Edge Delay (nsec)
        HRPWM_Timing_Calculator_Required_Falling_Edge_Delay = 0
        %Approximated MEP step size (psec)
        HRPWM_Timing_Calculator_Approximate_MEP_Step_Size = 180
        %Approximated MAX MEP steps
        HRPWM_Timing_Calculator_Approximate_MAX_MEP_Steps = 55
        %Coarse steps
        HRPWM_Timing_Calculator_Coarse_Steps = 40
        %Period in coarse steps
        HRPWM_Timing_Calculator_Period_Coarse_Steps = 80
        %Calculated CMPA/CMPB
        HRPWM_Timing_Calculator_CMPA_CMPB = 40
        %Calculated CMPAHR/CMPBHR
        HRPWM_Timing_Calculator_CMPAHR_CMPBHR = 128
        %Calculated TBPRD
        HRPWM_Timing_Calculator_TBPRD = 0
        %Calculated TBPRDHR
        HRPWM_Timing_Calculator_TBPRDHR = 0
        %Calculated DBRED
        HRPWM_Timing_Calculator_RED = 0
        %Calculated DBREDHR
        HRPWM_Timing_Calculator_DBREDHR = 128
        %Calculated FED
        HRPWM_Timing_Calculator_FED = 0
        %Calculated DBFEDHR
        HRPWM_Timing_Calculator_DBFEDHR = 128

        %HRPWM Phase Control

        %High resolution PWM (HRPWM) phase value
        HRPWM_High_Resolution_Phase = 0
        %PWMSYNC source select
        HRPWMSyncSource = 'Counter equals period';
        %Enable high resolution phase load
        High_Resolution_Phase_Load_Enable (1, 1) logical = false

        %HRPWM Duty Control Channel A

        %High resolution PWM (HRPWM) edge control mode on ePWMxA
        HRPWMChannelAEdgeMode = 'HRPWM is disabled';
        %High resolution CMPA (CMPAHR)
        High_Resolution_CMPA = 1
        %CMPAHR shadow to active load event
        CMPAHRShadowToActiveLoadEvent = 'Load when counter equals zero';

        %HRPWM Duty Control Channel B

        %High resolution PWM (HRPWM) edge control mode on ePWMxB
        HRPWMChannelBEdgeMode = 'HRPWM is disabled';
        %High resolution CMPB (CMPBHR)
        High_Resolution_CMPB = 1
        %CMPBHR shadow to active load event
        CMPBHRShadowToActiveLoadEvent = 'Load when counter equals zero';

        %HRPWM Period Control

        %High resolution period enable
        HRPWM_High_Resolution_Period_Enable (1, 1) logical = false;
        %High resolution period
        HRPWM_High_Resolution_Period = 0
        
        %HRPWM Deadband Control

        %Deadband edge mode
        HRPWMDeadbandEdgeMode = 'HRPWM is disabled'
        %High Resolution rising edge delay value
        HRPWM_High_Resolution_RED = 0
        %Event for shadow to active load of DBREDHR
        ShadowActiveLoadDBREDHR = 'Load when counter equals zero';
        %Swap A and B outputs
        Swap_A_B_Outputs (1, 1) logical = false
        %EPWMxB output select
        EPWMxBOutputSelect = 'ePWMxB output is normal';
        %High resolution falling edge delay value
        HRPWM_High_Resolution_FED = 0
        %Event for shadow to active load of DBFEDHR
        ShadowActiveLoadDBFEDHR = 'Load when counter equals zero';

        %EPWM XCMP Mode

        %Enable EPWM XCMP Mode
        Enable_XCMP_Mode (1, 1) logical = false;
        %Split EPWM XCMP Registers
        Enable_Split_XCMP (1,1) logical = false;
        %Allocate EPWM XCMP Registers to CMPA
        Allocate_XCMP_CMPA = 'Allocate 0 XCMP registers to CMPA';
        %Allocate EPWM XCMP Registers to CMPB
        Allocate_XCMP_CMPB = 'Allocate XCMP5 register to CMPB';

        %XCMP ACTIVE

        %Specify XCMPA Active Values
        SpecifyXCMPActive = 'Specify via Dialog Box';
        %XCMP1 Active Initial Value
        XCMP1_Active_Initial = 0;
        %XCMP2 Active Initial Value
        XCMP2_Active_Initial = 0;
        %XCMP3 Active Initial Value
        XCMP3_Active_Initial = 0;
        %XCMP4 Active Initial Value
        XCMP4_Active_Initial = 0;
        %XCMP5 Active Initial Value
        XCMP5_Active_Initial = 0;
        %XCMP6 Active Initial Value
        XCMP6_Active_Initial = 0;
        %XCMP7 Active Initial Value
        XCMP7_Active_Initial = 0;
        %XCMP8 Active Initial Value
        XCMP8_Active_Initial = 0;

        %XCMP SHADOW1

        %Specify XCMPA Shadow 1 Values
        SpecifyXCMPShadow1 = 'Specify via Dialog Box';
        %XCMP1 Shadow 1 Initial Value
        XCMP1_SHADOW1_Initial = 0;
        %XCMP2 Shadow 1 Initial Value
        XCMP2_SHADOW1_Initial = 0;
        %XCMP3 Shadow 1 Initial Value
        XCMP3_SHADOW1_Initial = 0;
        %XCMP4 Shadow 1 Initial Value
        XCMP4_SHADOW1_Initial = 0;
        %XCMP5 Shadow 1 Initial Value
        XCMP5_SHADOW1_Initial = 0;
        %XCMP6 Shadow 1 Initial Value
        XCMP6_SHADOW1_Initial = 0;
        %XCMP7 Shadow 1 Initial Value
        XCMP7_SHADOW1_Initial = 0;
        %XCMP8 Shadow 1 Initial Value
        XCMP8_SHADOW1_Initial = 0;

        %XCMP SHADOW2

        %Specify XCMPA Shadow 2 Values
        SpecifyXCMPShadow2 = 'Specify via Dialog Box';
        %XCMP1 Shadow 2 Initial Value
        XCMP1_SHADOW2_Initial = 0;
        %XCMP2 Shadow 2 Initial Value
        XCMP2_SHADOW2_Initial = 0;
        %XCMP3 Shadow 2 Initial Value
        XCMP3_SHADOW2_Initial = 0;
        %XCMP4 Shadow 2 Initial Value
        XCMP4_SHADOW2_Initial = 0;
        %XCMP5 Shadow 2 Initial Value
        XCMP5_SHADOW2_Initial = 0;
        %XCMP6 Shadow 2 Initial Value
        XCMP6_SHADOW2_Initial = 0;
        %XCMP7 Shadow 2 Initial Value
        XCMP7_SHADOW2_Initial = 0;
        %XCMP8 Shadow 2 Initial Value
        XCMP8_SHADOW2_Initial = 0;

        %XCMP SHADOW3
        
        %Specify XCMPA Shadow 3 Values
        SpecifyXCMPShadow3 = 'Specify via Dialog Box';
        %XCMP1 Shadow 3 Initial Value
        XCMP1_SHADOW3_Initial = 0;
        %XCMP2 Shadow 3 Initial Value
        XCMP2_SHADOW3_Initial = 0;
        %XCMP3 Shadow 3 Initial Value
        XCMP3_SHADOW3_Initial = 0;
        %XCMP4 Shadow 3 Initial Value
        XCMP4_SHADOW3_Initial = 0;
        %XCMP5 Shadow 3 Initial Value
        XCMP5_SHADOW3_Initial = 0;
        %XCMP6 Shadow 3 Initial Value
        XCMP6_SHADOW3_Initial = 0;
        %XCMP7 Shadow 3 Initial Value
        XCMP7_SHADOW3_Initial = 0;
        %XCMP8 Shadow 3 Initial Value
        XCMP8_SHADOW3_Initial = 0;

        %XCMP ACTION QUALIFIER ACTIVE

        %EPWMXA Tbctr equals XCMP1 ACTIVE
        EPWMXA_tbctr_XCMP1_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP2 ACTIVE
        EPWMXA_tbctr_XCMP2_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP3 ACTIVE
        EPWMXA_tbctr_XCMP3_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP4 ACTIVE
        EPWMXA_tbctr_XCMP4_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP5 ACTIVE
        EPWMXA_tbctr_XCMP5_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP6 ACTIVE
        EPWMXA_tbctr_XCMP6_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP7 ACTIVE
        EPWMXA_tbctr_XCMP7_active = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP8 ACTIVE
        EPWMXA_tbctr_XCMP8_active = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP5 ACTIVE
        EPWMXB_tbctr_XCMP5_active = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP6 ACTIVE
        EPWMXB_tbctr_XCMP6_active = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP7 ACTIVE
        EPWMXB_tbctr_XCMP7_active = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP8 ACTIVE
        EPWMXB_tbctr_XCMP8_active = 'No change in output pins';

        %XCMP ACTION QUALIFIER_SHADOW1
       
        %EPWMXA Tbctr equals XCMP1 Shadow 1 
        EPWMXA_tbctr_XCMP1_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP2 Shadow 1 
        EPWMXA_tbctr_XCMP2_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP3 Shadow 1 
        EPWMXA_tbctr_XCMP3_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP4 Shadow 1 
        EPWMXA_tbctr_XCMP4_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP5 Shadow 1 
        EPWMXA_tbctr_XCMP5_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP6 Shadow 1 
        EPWMXA_tbctr_XCMP6_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP7 Shadow 1 
        EPWMXA_tbctr_XCMP7_SHADOW1 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP8 Shadow 1 
        EPWMXA_tbctr_XCMP8_SHADOW1 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP5 Shadow 1 
        EPWMXB_tbctr_XCMP5_SHADOW1 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP6 Shadow 1 
        EPWMXB_tbctr_XCMP6_SHADOW1 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP7 Shadow 1 
        EPWMXB_tbctr_XCMP7_SHADOW1 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP8 Shadow 1 
        EPWMXB_tbctr_XCMP8_SHADOW1 = 'No change in output pins';

        %XCMP ACTION QUALIFIER_SHADOW2
        
        %EPWMXA Tbctr equals XCMP1 Shadow 2
        EPWMXA_tbctr_XCMP1_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP2 Shadow 2 
        EPWMXA_tbctr_XCMP2_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP3 Shadow 2 
        EPWMXA_tbctr_XCMP3_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP4 Shadow 2 
        EPWMXA_tbctr_XCMP4_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP5 Shadow 2 
        EPWMXA_tbctr_XCMP5_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP6 Shadow 2 
        EPWMXA_tbctr_XCMP6_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP7 Shadow 2 
        EPWMXA_tbctr_XCMP7_SHADOW2 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP8 Shadow 2 
        EPWMXA_tbctr_XCMP8_SHADOW2 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP5 Shadow 2 
        EPWMXB_tbctr_XCMP5_SHADOW2 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP6 Shadow 2 
        EPWMXB_tbctr_XCMP6_SHADOW2 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP7 Shadow 2 
        EPWMXB_tbctr_XCMP7_SHADOW2 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP8 Shadow 2 
        EPWMXB_tbctr_XCMP8_SHADOW2 = 'No change in output pins';

        %EPWMXA Tbctr equals XCMP1 Shadow 3
        EPWMXA_tbctr_XCMP1_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP2 Shadow 3 
        EPWMXA_tbctr_XCMP2_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP3 Shadow 3 
        EPWMXA_tbctr_XCMP3_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP4 Shadow 3 
        EPWMXA_tbctr_XCMP4_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP5 Shadow 3 
        EPWMXA_tbctr_XCMP5_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP6 Shadow 3 
        EPWMXA_tbctr_XCMP6_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP7 Shadow 3 
        EPWMXA_tbctr_XCMP7_SHADOW3 = 'No change in output pins';
        %EPWMXA Tbctr equals XCMP8 Shadow 3 
        EPWMXA_tbctr_XCMP8_SHADOW3 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP5 Shadow 3 
        EPWMXB_tbctr_XCMP5_SHADOW3 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP6 Shadow 3 
        EPWMXB_tbctr_XCMP6_SHADOW3 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP7 Shadow 3 
        EPWMXB_tbctr_XCMP7_SHADOW3 = 'No change in output pins';
        %EPWMXB Tbctr equals XCMP8 Shadow 3 
        EPWMXB_tbctr_XCMP8_SHADOW3 = 'No change in output pins';

        %XMINMAX

        %XMIN Active
        XMIN_Active = 0;
        %XMAX Active
        XMAX_Active = 0;
        %XMIN Shadow 1
        XMIN_Shadow1 = 0;
        %XMAX Shadow 1
        XMAX_Shadow1 = 0;
        %XMIN Shadow 2
        XMIN_Shadow2 = 0;
        %XMAX Shadow 2
        XMAX_Shadow2 = 0;
        %XMIN Shadow 3
        XMIN_Shadow3 = 0;
        %XMAX Shadow 3
        XMAX_Shadow3 = 0;

        %XTBPRD

        %XTbPrd Active
        XTBPRD_Active = 0;
        %XTbPrd Shadow 1
        XTBPRD_Shadow1 = 0;
        %XTbPrd Shadow 2
        XTBPRD_Shadow2 = 0;
        %XTbPrd Shadow 3
        XTBPRD_Shadow3 = 0;

        %XLOADCTL

        %Load Mode
        Load_Mode = 'Load Once';
        %Shadow Level
        Shadow_Level = 'Shadow level zero';
        %Shadow Register Set Load Once
        Shadow_register_load_once = 'Do not load any shadow register set';
        %Repeat Count Shadow Buffer 2
        Repeat_Count_Shadow_Buffer2 = 0;
        %Repeat Count Shadow Buffer 3
        Repeat_Count_Shadow_Buffer3 = 0;

        %Digital Compare

        %Combination Input Sources (DCA High)

         %DCA High Combinational Trip 1 Input
         DCAHigh_CombTrip1_Input (1,1) logical = false;
         %DCA High Combinational Trip 2 Input
         DCAHigh_CombTrip2_Input (1,1) logical = false;
         %DCA High Combinational Trip 3 Input
         DCAHigh_CombTrip3_Input (1,1) logical = false;
         %DCA High Combinational Trip 4 Input
         DCAHigh_CombTrip4_Input (1,1) logical = false;
         %DCA High Combinational Trip 5 Input
         DCAHigh_CombTrip5_Input (1,1) logical = false;
         %DCA High Combinational Trip 6 Input
         DCAHigh_CombTrip6_Input (1,1) logical = false;
         %DCA High Combinational Trip 7 Input
         DCAHigh_CombTrip7_Input (1,1) logical = false;
         %DCA High Combinational Trip 8 Input
         DCAHigh_CombTrip8_Input (1,1) logical = false;
         %DCA High Combinational Trip 9 Input
         DCAHigh_CombTrip9_Input (1,1) logical = false;
         %DCA High Combinational Trip 10 Input
         DCAHigh_CombTrip10_Input (1,1) logical = false;
         %DCA High Combinational Trip 11 Input
         DCAHigh_CombTrip11_Input (1,1) logical = false;
         %DCA High Combinational Trip 12 Input
         DCAHigh_CombTrip12_Input (1,1) logical = false;
         %DCA High Combinational Trip 13 Input
         DCAHigh_CombTrip13_Input (1,1) logical = false;
         %DCA High Combinational Trip 14 Input
         DCAHigh_CombTrip14_Input (1,1) logical = false;
         %DCA High Combinational Trip 15 Input
         DCAHigh_CombTrip15_Input (1,1) logical = false;

         %DCA Low Combinational Trip 1 Input
         DCALow_CombTrip1_Input (1,1) logical = false;
         %DCA Low Combinational Trip 2 Input
         DCALow_CombTrip2_Input (1,1) logical = false;
         %DCA Low Combinational Trip 3 Input
         DCALow_CombTrip3_Input (1,1) logical = false;
         %DCA Low Combinational Trip 4 Input
         DCALow_CombTrip4_Input (1,1) logical = false;
         %DCA Low Combinational Trip 5 Input
         DCALow_CombTrip5_Input (1,1) logical = false;
         %DCA Low Combinational Trip 6 Input
         DCALow_CombTrip6_Input (1,1) logical = false;
         %DCA Low Combinational Trip 7 Input
         DCALow_CombTrip7_Input (1,1) logical = false;
         %DCA Low Combinational Trip 8 Input
         DCALow_CombTrip8_Input (1,1) logical = false;
         %DCA Low Combinational Trip 9 Input
         DCALow_CombTrip9_Input (1,1) logical = false;
         %DCA Low Combinational Trip 10 Input
         DCALow_CombTrip10_Input (1,1) logical = false;
         %DCA Low Combinational Trip 11 Input
         DCALow_CombTrip11_Input (1,1) logical = false;
         %DCA Low Combinational Trip 12 Input
         DCALow_CombTrip12_Input (1,1) logical = false;
         %DCA Low Combinational Trip 13 Input
         DCALow_CombTrip13_Input (1,1) logical = false;
         %DCA Low Combinational Trip 14 Input
         DCALow_CombTrip14_Input (1,1) logical = false;
         %DCA Low Combinational Trip 15 Input
         DCALow_CombTrip15_Input (1,1) logical = false;

         %DCB Low Combinational Trip 1 Input
         DCBLow_CombTrip1_Input (1,1) logical = false;
         %DCB Low Combinational Trip 2 Input
         DCBLow_CombTrip2_Input (1,1) logical = false;
         %DCB Low Combinational Trip 3 Input
         DCBLow_CombTrip3_Input (1,1) logical = false;
         %DCB Low Combinational Trip 4 Input
         DCBLow_CombTrip4_Input (1,1) logical = false;
         %DCB Low Combinational Trip 5 Input
         DCBLow_CombTrip5_Input (1,1) logical = false;
         %DCB Low Combinational Trip 6 Input
         DCBLow_CombTrip6_Input (1,1) logical = false;
         %DCB Low Combinational Trip 7 Input
         DCBLow_CombTrip7_Input (1,1) logical = false;
         %DCB Low Combinational Trip 8 Input
         DCBLow_CombTrip8_Input (1,1) logical = false;
         %DCB Low Combinational Trip 9 Input
         DCBLow_CombTrip9_Input (1,1) logical = false;
         %DCB Low Combinational Trip 10 Input
         DCBLow_CombTrip10_Input (1,1) logical = false;
         %DCB Low Combinational Trip 11 Input
         DCBLow_CombTrip11_Input (1,1) logical = false;
         %DCB Low Combinational Trip 12 Input
         DCBLow_CombTrip12_Input (1,1) logical = false;
         %DCB Low Combinational Trip 13 Input
         DCBLow_CombTrip13_Input (1,1) logical = false;
         %DCB Low Combinational Trip 14 Input
         DCBLow_CombTrip14_Input (1,1) logical = false;
         %DCB Low Combinational Trip 15 Input
         DCBLow_CombTrip15_Input (1,1) logical = false;

         %DCB High Combinational Trip 1 Input
         DCBHigh_CombTrip1_Input (1,1) logical = false;
         %DCB High Combinational Trip 2 Input
         DCBHigh_CombTrip2_Input (1,1) logical = false;
         %DCB High Combinational Trip 3 Input
         DCBHigh_CombTrip3_Input (1,1) logical = false;
         %DCB High Combinational Trip 4 Input
         DCBHigh_CombTrip4_Input (1,1) logical = false;
         %DCB High Combinational Trip 5 Input
         DCBHigh_CombTrip5_Input (1,1) logical = false;
         %DCB High Combinational Trip 6 Input
         DCBHigh_CombTrip6_Input (1,1) logical = false;
         %DCB High Combinational Trip 7 Input
         DCBHigh_CombTrip7_Input (1,1) logical = false;
         %DCB High Combinational Trip 8 Input
         DCBHigh_CombTrip8_Input (1,1) logical = false;
         %DCB High Combinational Trip 9 Input
         DCBHigh_CombTrip9_Input (1,1) logical = false;
         %DCB High Combinational Trip 10 Input
         DCBHigh_CombTrip10_Input (1,1) logical = false;
         %DCB High Combinational Trip 11 Input
         DCBHigh_CombTrip11_Input (1,1) logical = false;
         %DCB High Combinational Trip 12 Input
         DCBHigh_CombTrip12_Input (1,1) logical = false;
         %DCB High Combinational Trip 13 Input
         DCBHigh_CombTrip13_Input (1,1) logical = false;
         %DCB High Combinational Trip 14 Input
         DCBHigh_CombTrip14_Input (1,1) logical = false;
         %DCB High Combinational Trip 15 Input
         DCBHigh_CombTrip15_Input (1,1) logical = false;

        
    end

    properties(Constant,Hidden)
        ePWMModuleSet = matlab.system.StringSet({'EPWM0', 'EPWM1', 'EPWM2', 'EPWM3', 'EPWM4', 'EPWM5', 'EPWM6', 'EPWM7', 'EPWM8', 'EPWM9', 'EPWM10', 'EPWM11', 'EPWM12', 'EPWM13', 'EPWM14', 'EPWM15', 'EPWM16', 'EPWM17', 'EPWM18', 'EPWM19', 'EPWM20', 'EPWM21', 'EPWM22', 'EPWM23', 'EPWM24', 'EPWM25', 'EPWM26', 'EPWM27', 'EPWM28', 'EPWM29', 'EPWM30', 'EPWM31'});
        EmulationModeSet = matlab.system.StringSet({'Stop after next Time Base Counter increaments or decreaments', 'Stop when counter completes whole cycle', 'Free run'});
        TBCLKSet = matlab.system.StringSet({'Divide clock by 1', 'Divide clock by 2', 'Divide clock by 4', 'Divide clock by 8', 'Divide clock by 16', 'Divide clock by 32', 'Divide clock by 64', 'Divide clock by 128'});
        HSPCLKDIVSet = matlab.system.StringSet({'Divide clock by 1', 'Divide clock by 2', 'Divide clock by 4', 'Divide clock by 6','Divide clock by 8', 'Divide clock by 10','Divide clock by 12','Divide clock by 14'});
        TimerPeriodUnitsSet = matlab.system.StringSet({'Clock Cycles', 'Seconds'});
        SpecifyTimerPeriodSet = matlab.system.StringSet({'Specify Via Dialog Box', 'Input Port'});
        ePWMLinkSet = matlab.system.StringSet({'Disable Linking', 'Link with ePWM0', 'Link with ePWM1', 'Link with ePWM2', 'Link with ePWM3', 'Link with ePWM4', 'Link with ePWM5', 'Link with ePWM6', 'Link with ePWM7', 'Link with ePWM8', 'Link with ePWM9', 'Link with ePWM10', 'Link with ePWM11', 'Link with ePWM12', 'Link with ePWM13', 'Link with ePWM14', 'Link with ePWM15', 'Link with ePWM16', 'Link with ePWM17', 'Link with ePWM18', 'Link with ePWM19', 'Link with ePWM20', 'Link with ePWM21', 'Link with ePWM22', 'Link with ePWM23'});
        GlobalRegisterLoadSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        PeriodLoadModeSet = matlab.system.StringSet({'Using Shadow registers', 'Direct access'});
        PeriodLoadEventSet = matlab.system.StringSet({'Shadow to Active load when counter reaches zero', 'Shadow to Active load when counter reaches zero and sync occurs', 'Shadow to Active load when sync occurs'});
        CounterModeSet = matlab.system.StringSet({'Up count', 'Down count', 'Up-Down Count' , 'Stop Freeze'});
        SyncInPulseSrcSet = matlab.system.StringSet({'Disable Sync-in', 'Sync-in source is EPWM0 sync-out signal', 'Sync-in source is EPWM1 sync-out signal', 'Sync-in source is EPWM2 sync-out signal', 'Sync-in source is EPWM3 sync-out signal', 'Sync-in source is EPWM4 sync-out signal', 'Sync-in source is EPWM5 sync-out signal', 'Sync-in source is EPWM6 sync-out signal', 'Sync-in source is EPWM7 sync-out signal', 'Sync-in source is EPWM8 sync-out signal', 'Sync-in source is EPWM9 sync-out signal', 'Sync-in source is EPWM10 sync-out signal', 'Sync-in source is EPWM11 sync-out signal', 'Sync-in source is EPWM12 sync-out signal', 'Sync-in source is EPWM13 sync-out signal', 'Sync-in source is EPWM14 sync-out signal', 'Sync-in source is EPWM15 sync-out signal', 'Sync-in source is EPWM16 sync-out signal', 'Sync-in source is EPWM17 sync-out signal', 'Sync-in source is EPWM18 sync-out signal', 'Sync-in source is EPWM19 sync-out signal', 'Sync-in source is EPWM20 sync-out signal', 'Sync-in source is EPWM21 sync-out signal', 'Sync-in source is EPWM22 sync-out signal', 'Sync-in source is EPWM23 sync-out signal', 'Sync-in source is ECAP0 sync-out signal', 'Sync-in source is ECAP1 sync-out signal', 'Sync-in source is ECAP2 sync-out signal', 'Sync-in source is ECAP3 sync-out signal', 'Sync-in source is ECAP4 sync-out signal', 'Sync-in source is ECAP5 sync-out signal', 'Sync-in source is ECAP6 sync-out signal', 'Sync-in source is ECAP7 sync-out signal', 'Sync-in source is ECAP8 sync-out signal', 'Sync-in source is ECAP9 sync-out signal', 'Sync-in source is Input XBAR Out4 signal', 'Sync-in source is Input XBAR Out20 signal', 'Sync-in source is C2K Timesync XBAR PWM Output 0 signal', 'Sync-in source is C2K Timesync XBAR PWM Output 1 signal', 'Sync-in source is FSIRX0 Trigger0 signal', 'Sync-in source is FSIRX0 Trigger1 signal', 'Sync-in source is FSIRX0 Trigger2 signal', 'Sync-in source is FSIRX0 Trigger3 signal', 'Sync-in source is FSIRX1 Trigger0 signal', 'Sync-in source is FSIRX1 Trigger1 signal', 'Sync-in source is FSIRX1 Trigger2 signal', 'Sync-in source is FSIRX1 Trigger3 signal', 'Sync-in source is FSIRX2 Trigger0 signal', 'Sync-in source is FSIRX2 Trigger1 signal', 'Sync-in source is FSIRX2 Trigger2 signal', 'Sync-in source is FSIRX2 Trigger3 signal', 'Sync-in source is FSIRX3 Trigger0 signal', 'Sync-in source is FSIRX3 Trigger1 signal', 'Sync-in source is FSIRX3 Trigger2 signal', 'Sync-in source is FSIRX3 Trigger3 signal'});
        OneShotTriggerSet = matlab.system.StringSet({'Trigger is one-shot sync', 'Trigger is one-shot reload'});
        ePWMLinkCMPASet = matlab.system.StringSet({'Link with ePWM0', 'Link with ePWM1', 'Link with ePWM2', 'Link with ePWM3', 'Link with ePWM4', 'Link with ePWM5', 'Link with ePWM6', 'Link with ePWM7', 'Link with ePWM8', 'Link with ePWM9', 'Link with ePWM10', 'Link with ePWM11', 'Link with ePWM12', 'Link with ePWM13', 'Link with ePWM14', 'Link with ePWM15', 'Link with ePWM16', 'Link with ePWM17', 'Link with ePWM18', 'Link with ePWM19', 'Link with ePWM20', 'Link with ePWM21', 'Link with ePWM22', 'Link with ePWM23'});
        CMPAUnitsSet = matlab.system.StringSet({'Clock Cycles', 'Seconds'});
        SpecifyCMPAViaSet = matlab.system.StringSet({'Specify Via Dialog Box', 'Input Port'});
        GlobalLoadRegisterCMPASet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        SHDWAMODESet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ePWMLinkCMPBSet = matlab.system.StringSet({'Link with ePWM0', 'Link with ePWM1', 'Link with ePWM2', 'Link with ePWM3', 'Link with ePWM4', 'Link with ePWM5', 'Link with ePWM6', 'Link with ePWM7', 'Link with ePWM8', 'Link with ePWM9', 'Link with ePWM10', 'Link with ePWM11', 'Link with ePWM12', 'Link with ePWM13', 'Link with ePWM14', 'Link with ePWM15', 'Link with ePWM16', 'Link with ePWM17', 'Link with ePWM18', 'Link with ePWM19', 'Link with ePWM20', 'Link with ePWM21', 'Link with ePWM22', 'Link with ePWM23'});
        CMPBUnitsSet = matlab.system.StringSet({'Clock Cycles', 'Seconds'});
        SpecifyCMPBViaSet = matlab.system.StringSet({'Specify Via Dialog Box', 'Input Port'});
        GlobalLoadRegisterCMPBSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        SHDWBMODESet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ePWMLinkCMPCSet = matlab.system.StringSet({'Link with ePWM0', 'Link with ePWM1', 'Link with ePWM2', 'Link with ePWM3', 'Link with ePWM4', 'Link with ePWM5', 'Link with ePWM6', 'Link with ePWM7', 'Link with ePWM8', 'Link with ePWM9', 'Link with ePWM10', 'Link with ePWM11', 'Link with ePWM12', 'Link with ePWM13', 'Link with ePWM14', 'Link with ePWM15', 'Link with ePWM16', 'Link with ePWM17', 'Link with ePWM18', 'Link with ePWM19', 'Link with ePWM20', 'Link with ePWM21', 'Link with ePWM22', 'Link with ePWM23'});
        CMPCUnitsSet = matlab.system.StringSet({'Clock Cycles', 'Seconds'});
        SpecifyCMPCViaSet = matlab.system.StringSet({'Specify Via Dialog Box', 'Input Port'});
        GlobalLoadRegisterCMPCSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        SHDWCMODESet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ePWMLinkCMPDSet = matlab.system.StringSet({'Link with ePWM0', 'Link with ePWM1', 'Link with ePWM2', 'Link with ePWM3', 'Link with ePWM4', 'Link with ePWM5', 'Link with ePWM6', 'Link with ePWM7', 'Link with ePWM8', 'Link with ePWM9', 'Link with ePWM10', 'Link with ePWM11', 'Link with ePWM12', 'Link with ePWM13', 'Link with ePWM14', 'Link with ePWM15', 'Link with ePWM16', 'Link with ePWM17', 'Link with ePWM18', 'Link with ePWM19', 'Link with ePWM20', 'Link with ePWM21', 'Link with ePWM22', 'Link with ePWM23'});
        CMPDUnitsSet = matlab.system.StringSet({'Clock Cycles', 'Seconds'});
        SpecifyCMPDViaSet = matlab.system.StringSet({'Specify Via Dialog Box', 'Input Port'});
        GlobalLoadRegisterCMPDSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        SHDWDMODESet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ContSWForceGlobalLoadRegisterSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        SoftwareForceShadowModeSet = matlab.system.StringSet({'Shadow mode load when counter equals zero', 'Shadow mode load when counter equals period', 'Shadow mode load when counter equals zero or period', 'Immediate mode'});
        T1TriggerSourceSet = matlab.system.StringSet({'Digital compare event A 1', 'Digital compare event A 2', 'Digital compare event B 1', 'Digital compare event B 2', 'Trip zone 1', 'Trip zone 2', 'Trip zone 3', 'ePWM sync', 'Digital compare filter event'});
        T2TriggerSourceSet = matlab.system.StringSet({'Digital compare event A 1', 'Digital compare event A 2', 'Digital compare event B 1', 'Digital compare event B 2', 'Trip zone 1', 'Trip zone 2', 'Trip zone 3', 'ePWM sync', 'Digital compare filter event'});
        ePWMXAGlobalLoadRegisterSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        ePWMXAShadowLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ePWMXAOneTimeSWForceActionSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        ePWMXAContinuousSWForceActionSet = matlab.system.StringSet({'Software forcing disabled', 'Set output pins to low', 'Set output pins to high'});
        ePWMXATbctrZeroSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXATbctrPrdSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXATbctrCMPAUpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXATbctrCMPADownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXATbctrCMPBUpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXATbctrCMPBDownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXAT1UpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXAT1DownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXAT2UpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXAT2DownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBGlobalLoadRegisterSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        ePWMXBShadowLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period', 'Freeze shadow to active load', 'Load on sync or when counter equals zero', 'Load on sync or when counter equals period', 'Load on sync or when counter equals zero or period', 'Load on sync only'});
        ePWMXBOneTimeSWForceActionSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        ePWMXBContinuousSWForceActionSet = matlab.system.StringSet({'Software forcing disabled', 'Set output pins to low', 'Set output pins to high'});
        ePWMXBTbctrZeroSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBTbctrPrdSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBTbctrCMPAUpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBTbctrCMPADownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBTbctrCMPBUpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBTbctrCMPBDownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBT1UpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBT1DownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBT2UpSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        ePWMXBT2DownSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle the output pins'});
        TZAEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        TZBEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT1EventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT2EventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT1EventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT2EventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        TZBDEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        TZBUEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        TZADEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        TZAUEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT1UEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT1DEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT2UEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCAEVT2DEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT1UEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT1DEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT2UEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        DCBEVT2DEventSet = matlab.system.StringSet({'High impedance output', 'High voltage state', 'Low voltage state', 'Disable action'});
        CBCLatchClearSignalSet = matlab.system.StringSet({'Clear CBC Pulse when counter reaches zero', 'Clear CBC Pulse when counter reaches period', 'Clear CBC Pulse when counter reaches zero or period'});
        DCAHighSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        DCALowSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        DCOutput1AConditionSet = matlab.system.StringSet({'Event is disabled', 'Event when DCxH low', 'Event when DCxH high', 'Event when DCxL low', 'Event when DCxL high', 'Event when DCxL high DCxH low'});
        DCOutput2AConditionSet = matlab.system.StringSet({'Event is disabled', 'Event when DCxH low', 'Event when DCxH high', 'Event when DCxL low', 'Event when DCxL high', 'Event when DCxL high DCxH low'});
        SynchModeDCAEVT1Set = matlab.system.StringSet({'DC Input signal is synced with TBCLK', 'DC Input signal is not synced with TBCLK'});
        SignalSourceDCAEVT1Set = matlab.system.StringSet({'Signal source is unfiltered', 'Signal source is filtered'});
        CBCLatchModeDCAEVT1Set = matlab.system.StringSet({'DC CBC latch is disabled', 'DC CBC latch is enabled'});
        CBCLatchClearEventDCAEVT1Set = matlab.system.StringSet({'Clear CBC latch when counter equals zero', 'Clear CBC latch when counter equals period', 'Clear CBC latch when counter equals zero or period'});
        SynchModeDCAEVT2Set = matlab.system.StringSet({'DC Input signal is synced with TBCLK', 'DC Input signal is not synced with TBCLK'});
        SignalSourceDCAEVT2Set = matlab.system.StringSet({'Signal source is unfiltered', 'Signal source is filtered'});
        CBCLatchModeDCAEVT2Set = matlab.system.StringSet({'DC CBC latch is disabled', 'DC CBC latch is enabled'});
        CBCLatchClearEventDCAEVT2Set = matlab.system.StringSet({'Clear CBC latch when counter equals zero', 'Clear CBC latch when counter equals period', 'Clear CBC latch when counter equals zero or period'});
        DCBHighSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        DCBLowSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        DCOutput1BConditionSet = matlab.system.StringSet({'Event is disabled', 'Event when DCxH low', 'Event when DCxH high', 'Event when DCxL low', 'Event when DCxL high', 'Event when DCxL high DCxH low'});
        DCOutput2BConditionSet = matlab.system.StringSet({'Event is disabled', 'Event when DCxH low', 'Event when DCxH high', 'Event when DCxL low', 'Event when DCxL high', 'Event when DCxL high DCxH low'});
        SynchModeDCBEVT1Set = matlab.system.StringSet({'DC Input signal is synced with TBCLK', 'DC Input signal is not synced with TBCLK'});
        SignalSourceDCBEVT1Set = matlab.system.StringSet({'Signal source is unfiltered', 'Signal source is filtered'});
        CBCLatchModeDCBEVT1Set = matlab.system.StringSet({'DC CBC latch is disabled', 'DC CBC latch is enabled'});
        CBCLatchClearEventDCBEVT1Set = matlab.system.StringSet({'Clear CBC latch when counter equals zero', 'Clear CBC latch when counter equals period', 'Clear CBC latch when counter equals zero or period'});
        SynchModeDCBEVT2Set = matlab.system.StringSet({'DC Input signal is synced with TBCLK', 'DC Input signal is not synced with TBCLK'});
        SignalSourceDCBEVT2Set = matlab.system.StringSet({'Signal source is unfiltered', 'Signal source is filtered'});
        CBCLatchModeDCBEVT2Set = matlab.system.StringSet({'DC CBC latch is disabled', 'DC CBC latch is enabled'});
        CBCLatchClearEventDCBEVT2Set = matlab.system.StringSet({'Clear CBC latch when counter equals zero', 'Clear CBC latch when counter equals period', 'Clear CBC latch when counter equals zero or period'});
        DCFilterInputEventSourceSet = matlab.system.StringSet({'DC filter signal source is DCAEVT1', 'DC filter signal source is DCAEVT2', 'DC filter signal source is DCBEVT1', 'DC filter signal source is DCBEVT2'});
        BlankingWindowStartEventSet = matlab.system.StringSet({'Time base counter equals period', 'Time base counter equals zero', 'Time base counter equals zero or period', 'Time base counter equals mixed event'});
        
        EdgeFilterModeSet = matlab.system.StringSet({'Digital Compare Edge Filter low', 'Digital Compare Edge Filter high', 'Digital Compare Edge Filter both'});
        EdgeFilterEdgeCountSet = matlab.system.StringSet({'No edge is required to generate event', '1 edge is reguired for event generation', '2 edges are required for event generation', '3 edges are required for event generation', '4 edges are required for event generation', '5 edges are required for event generation', '6 edges are required for event generation', '7 edges are required for event generation'});
        EdgeFilterCounterCaptureSignalSet = matlab.system.StringSet({'Valley capture triggered by software', 'Valley capture triggered by when counter is equal to zero', 'Valley capture triggered by when counter is equal to period', 'Valley capture triggered when counter is equal to zero or period', 'Valley capture triggered by DCAEVT1', 'Valley capture triggered by DCAEVT2', 'Valley capture triggered by DCBEVT1', 'Valley capture triggered by DCBEVT2'});
        StartValleyCaptureSet = matlab.system.StringSet({'0 Event', '1 Events', '2 Events', '3 Events', '4 Events', '5 Events', '6 Events', '7 Events', '8 Events', '9 Events', '10 Events', '11 Events', '12 Events', '13 Events', '14 Events', '15 Events'});
        StopValleyCaptureSet = matlab.system.StringSet({'0 Event', '1 Events', '2 Events', '3 Events', '4 Events', '5 Events', '6 Events', '7 Events', '8 Events', '9 Events', '10 Events', '11 Events', '12 Events', '13 Events', '14 Events', '15 Events'});
        ValleyDelayDividerSet = matlab.system.StringSet({'Delay value equals offset value defined by software', 'Delay value equals sum of hardware counter value and the offset value defined by software', 'Delay value equals sum of hardware counter value shifted by 1 and the offset value defined by software', 'Delay value equals sum of hardware counter value shifted by 2 and the offset value defined by software', 'Delay value equals sum of hardware counter value shifted by 4 and the offset value defined by software'});
        Capture_InputSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        Invert_Capture_Input_PolaritySet = matlab.system.StringSet({'Capture input is not inverted', 'Capture input is inverted'});
        Capture_gateSet = matlab.system.StringSet({'Trip 1', 'Trip 2', 'Trip 3', 'Trip 4', 'Trip 5', 'Trip 6', 'Trip 7', 'Trip 8', 'Trip 9', 'Trip 10', 'Trip 11', 'Trip 12', 'Trip 13', 'Trip 14', 'Trip 15', 'All Trips'});
        Capture_gate_polarity_selectSet = matlab.system.StringSet({'Capture gate is always on', 'Capture gate is always off', 'Capture gate input is CAPGATE.sync', 'Capture gate input is CAPGATE.sync inverted'});
        RisingEdgeDelayInputSet = matlab.system.StringSet({'Input signal is ePWMA', 'Input signal is ePWMB'});
        FallingEdgeDelayInputSet = matlab.system.StringSet({'Input signal is ePWMA', 'Input signal is ePWMB'});
        RisingEdgeDelayPolaritySet = matlab.system.StringSet({'DB polarity is not inverted', 'DB polarity is inverted'});
        FallingEdgeDelayPolaritySet = matlab.system.StringSet({'DB polarity is not inverted', 'DB polarity is inverted'});
        DeadbandGlobalLoadSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        DeadbandControlShadowModeSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equlas period', 'Load when counter equals zero or period', 'Freeze shadow to active load'});
        REDGlobalLoadRegisterSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        REDShadowLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equlas period', 'Load when counter equals zero or period', 'Freeze shadow to active load'});
        FEDGlobalLoadRegisterSet = matlab.system.StringSet({'EPWM_GL_REGISTER_TBPRD_TBPRDHR', 'EPWM_GL_REGISTER_CMPA_CMPAHR', 'EPWM_GL_REGISTER_CMPB_CMPBHR', 'EPWM_GL_REGISTER_CMPC', 'EPWM_GL_REGISTER_CMPD', 'EPWM_GL_REGISTER_DBRED_DBREDHR', 'EPWM_GL_REGISTER_DBFED_DBFEDHR', 'EPWM_GL_REGISTER_DBCTL', 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2', 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2', 'EPWM_GL_REGISTER_AQCSFRC'});
        FEDShadowLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equlas period', 'Load when counter equals zero or period', 'Freeze shadow to active load'});
        DeadBandCounterClockRateSet = matlab.system.StringSet({'Deadband counter runs at TBCLK rate', 'Deadband counter runs at 2*TBCLK rate'});
        ChopperDutyCycleSet = matlab.system.StringSet({'1/8 Duty', '2/8 Duty', '3/8 Duty', '4/8 Duty', '5/8 Duty', '6/8 Duty', '7/8 Duty'});
        ChopperFrequencySet = matlab.system.StringSet({'SYSCLOCKOUT/1', 'SYSCLOCKOUT/2', 'SYSCLOCKOUT/3', 'SYSCLOCKOUT/4', 'SYSCLOCKOUT/5', 'SYSCLOCKOUT/6', 'SYSCLOCKOUT/7', 'SYSCLOCKOUT/8'});
        ChopperFirstPulseWidthSet = matlab.system.StringSet({'1', '2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12', '13', '14', '15', '16'});
        InterruptEventSourcesSet = matlab.system.StringSet({'Time base counter equal to zero', 'Time base counter equal to period', 'Time base counter based on mixed events', 'Time base counter equal to CMPA when timer is incrementing', 'Time base counter equal to CMPC when timer is incrementing', 'Time base counter equal to CMPA when timer is decrementing', 'Time base counter equal to CMPC when timer is decrementing', 'Time base counter equal to CMPB when timer is incrementing', 'Time base counter equal to CMPD when timer is incrementing', 'Time base counter equal to CMPB when timer is decrementing', 'Time base counter equal to CMPD when timer is decrementing'});
        InterruptEventCountSet = matlab.system.StringSet({'Disabled', '1 event generates interrupt', '2 events generates interrupt', '3 events generates interrupt', '4 events generates interrupt', '5 events generates interrupt', '6 events generates interrupt', '7 events generates interrupt', '8 events generates interrupt', '9 events generates interrupt', '10 events generates interrupt', '11 events generates interrupt', '12 events generates interrupt', '13 events generates interrupt', '14 events generates interrupt', '15 events generates interrupt'});
        InterruptEventCountInitialValueSet = matlab.system.StringSet({'Initialize event counter to 0', 'Initialize event counter to 1', 'Initialize event counter to 2', 'Initialize event counter to 3', 'Initialize event counter to 4', 'Initialize event counter to 5', 'Initialize event counter to 6', 'Initialize event counter to 7', 'Initialize event counter to 8', 'Initialize event counter to 9', 'Initialize event counter to 10', 'Initialize event counter to 11', 'Initialize event counter to 12', 'Initialize event counter to 13', 'Initialize event counter to 14', 'Initialize event counter to 15'});
        SOCATriggerSourceSet = matlab.system.StringSet({'Event is based on DCxEVT1', 'Time base counter equal to zero', 'Time base counter equal to period', 'Time base counter equal to mixed event', 'Time base counter equal to CMPA when timer is incrementing', 'Time base counter equal to CMPC when timer is incrementing', 'Time base counter equal to CMPA when timer is decrementing', 'Time base counter equal to CMPC when timer is decrementing', 'Time base counter equal to CMPB when timer is incrementing', 'Time base counter equal to CMPD when timer is incrementing', 'Time base counter equal to CMPB when timer is decrementing', 'Time base counter equal to CMPD when timer is decrementing'});
        SOCATriggerEventCountSet = matlab.system.StringSet({'Disabled', '1 event generates interrupt', '2 events generates interrupt', '3 events generates interrupt', '4 events generates interrupt', '5 events generates interrupt', '6 events generates interrupt', '7 events generates interrupt', '8 events generates interrupt', '9 events generates interrupt', '10 events generates interrupt', '11 events generates interrupt', '12 events generates interrupt', '13 events generates interrupt', '14 events generates interrupt', '15 events generates interrupt'});
        SOCATriggerEventCountInitValueSet = matlab.system.StringSet({'Initialize event counter to 0', 'Initialize event counter to 1', 'Initialize event counter to 2', 'Initialize event counter to 3', 'Initialize event counter to 4', 'Initialize event counter to 5', 'Initialize event counter to 6', 'Initialize event counter to 7', 'Initialize event counter to 8', 'Initialize event counter to 9', 'Initialize event counter to 10', 'Initialize event counter to 11', 'Initialize event counter to 12', 'Initialize event counter to 13', 'Initialize event counter to 14', 'Initialize event counter to 15'});
        SOCBTriggerSourceSet = matlab.system.StringSet({'Event is based on DCxEVT1', 'Time base counter equal to zero', 'Time base counter equal to period', 'Time base counter equal to mixed event', 'Time base counter equal to CMPA when timer is incrementing', 'Time base counter equal to CMPC when timer is incrementing', 'Time base counter equal to CMPA when timer is decrementing', 'Time base counter equal to CMPC when timer is decrementing', 'Time base counter equal to CMPB when timer is incrementing', 'Time base counter equal to CMPD when timer is incrementing', 'Time base counter equal to CMPB when timer is decrementing', 'Time base counter equal to CMPD when timer is decrementing'});
        SOCBTriggerEventCountSet = matlab.system.StringSet({'Disabled', '1 event generates interrupt', '2 events generates interrupt', '3 events generates interrupt', '4 events generates interrupt', '5 events generates interrupt', '6 events generates interrupt', '7 events generates interrupt', '8 events generates interrupt', '9 events generates interrupt', '10 events generates interrupt', '11 events generates interrupt', '12 events generates interrupt', '13 events generates interrupt', '14 events generates interrupt', '15 events generates interrupt'});
        SOCBTriggerEventCountInitValueSet = matlab.system.StringSet({'Initialize event counter to 0', 'Initialize event counter to 1', 'Initialize event counter to 2', 'Initialize event counter to 3', 'Initialize event counter to 4', 'Initialize event counter to 5', 'Initialize event counter to 6', 'Initialize event counter to 7', 'Initialize event counter to 8', 'Initialize event counter to 9', 'Initialize event counter to 10', 'Initialize event counter to 11', 'Initialize event counter to 12', 'Initialize event counter to 13', 'Initialize event counter to 14', 'Initialize event counter to 15'});
        ControlModeChannelASet = matlab.system.StringSet({'CMPAHR/CMPBHR or TBPRDHR controls MEP edge', 'TBPHSR controls MEP edge'});
        ControlModeChannelBSet = matlab.system.StringSet({'CMPAHR/CMPBHR or TBPRDHR controls MEP edge', 'TBPHSR controls MEP edge'});
        HRPWMSyncSourceSet = matlab.system.StringSet({'Counter equals period', 'Counter equals zero', 'Counter equals COMPC when counting up', 'Counter equals COMPC when counting down', 'Counter equals COMPD when counting up', 'Counter equals COMPD when counting down'});
        HRPWMChannelAEdgeModeSet = matlab.system.StringSet({'HRPWM is disabled', 'MEP controls rising edge', 'MEP controls falling edge', 'MEP controls both rising and falling edge'});
        CMPAHRShadowToActiveLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period'});
        HRPWMChannelBEdgeModeSet = matlab.system.StringSet({'HRPWM is disabled', 'MEP controls rising edge', 'MEP controls falling edge', 'MEP controls both rising and falling edge'});
        CMPBHRShadowToActiveLoadEventSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period'});
        HRPWMDeadbandEdgeModeSet = matlab.system.StringSet({'HRPWM is disabled', 'MEP controls rising edge delay', 'MEP controls falling edge delay', 'MEP controls both falling and rising edge delay'});
        ShadowActiveLoadDBREDHRSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period'});
        ShadowActiveLoadDBFEDHRSet = matlab.system.StringSet({'Load when counter equals zero', 'Load when counter equals period', 'Load when counter equals zero or period'});
        EPWMxBOutputSelectSet = matlab.system.StringSet({'ePWMxB output is normal', 'ePWMxB output is inverted'});
        Allocate_XCMP_CMPASet = matlab.system.StringSet({'Allocate 0 XCMP registers to CMPA', 'Allocate XCMP1 register to CMPA', 'Allocate XCMP1-XCMP2 registers to CMPA', 'Allocate XCMP1-XCMP3 registers to CMPA', 'Allocate XCMP1-XCMP4 registers to CMPA', 'Allocate XCMP1-XCMP5 registers to CMPA', 'Allocate XCMP1-XCMP6 registers to CMPA', 'Allocate XCMP1-XCMP7 registers to CMPA', 'Allocate XCMP1-XCMP8 registers to CMPA'});
        Allocate_XCMP_CMPBSet = matlab.system.StringSet({'Allocate XCMP5 register to CMPB', 'Allocate XCMP5-XCMP6 registers to CMPB', 'Allocate XCMP5-XCMP7 registers to CMPB', 'Allocate XCMP5-XCMP8 registers to CMPB'});
        SpecifyXCMPActiveSet = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        SpecifyXCMPShadow1Set = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        SpecifyXCMPShadow2Set = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        SpecifyXCMPShadow3Set = matlab.system.StringSet({'Specify via Dialog Box', 'Specify via Input Port'});
        EPWMXA_tbctr_XCMP1_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP2_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP3_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP4_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP5_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP6_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP7_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP8_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP5_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP6_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP7_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP8_activeSet = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP1_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP2_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP3_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP4_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP5_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP6_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP7_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP8_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP5_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP6_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP7_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP8_SHADOW1Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP1_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP2_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP3_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP4_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP5_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP6_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP7_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP8_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP5_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP6_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP7_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP8_SHADOW2Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP1_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP2_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP3_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP4_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP5_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP6_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP7_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXA_tbctr_XCMP8_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP5_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP6_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP7_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        EPWMXB_tbctr_XCMP8_SHADOW3Set = matlab.system.StringSet({'No change in output pins', 'Set output pins to low', 'Set output pins to high', 'Toggle output pins'});
        Load_ModeSet = matlab.system.StringSet({'Load Once', 'Load multiple times'});
        Shadow_LevelSet = matlab.system.StringSet({'Shadow level zero', 'Shadow level one', 'Shadow level two', 'Shadow level three'});
        Shadow_register_load_onceSet = matlab.system.StringSet({'Do not load any shadow register set', 'Load shadow register set 1', 'Load shadow register set 2', 'Load shadow register set 3'});  
    end
    
    

    properties (Dependent, Access=protected)

        ePWMModuleEnum;
        EmulationModeEnum;
        TBCLKEnum;
        HSPCLKDIVEnum;
        TimerPeriodUnitsEnum;
        SpecifyTimerPeriodEnum;
        ePWMLinkEnum;
        GlobalRegisterLoadEnum;
        PeriodLoadModeEnum;
        PeriodLoadEventEnum;
        CounterModeEnum;
        SyncInPulseSrcEnum;
        OneShotTriggerEnum;
        ePWMLinkCMPAEnum;
        CMPAUnitsEnum;
        SpecifyCMPAViaEnum;
        GlobalLoadRegisterCMPAEnum;
        SHDWAMODEEnum;
        ePWMLinkCMPBEnum;
        CMPBUnitsEnum;
        SpecifyCMPBViaEnum;
        GlobalLoadRegisterCMPBEnum;
        SHDWBMODEEnum;
        ePWMLinkCMPCEnum;
        CMPCUnitsEnum;
        SpecifyCMPCViaEnum;
        GlobalLoadRegisterCMPCEnum;
        SHDWCMODEEnum;
        ePWMLinkCMPDEnum;
        CMPDUnitsEnum;
        SpecifyCMPDViaEnum;
        GlobalLoadRegisterCMPDEnum;
        SHDWDMODEEnum;
        ContSWForceGlobalLoadRegisterEnum;
        SoftwareForceShadowModeEnum;
        T1TriggerSourceEnum;
        T2TriggerSourceEnum;
        ePWMXAGlobalLoadRegisterEnum;
        ePWMXAShadowLoadEventEnum;
        ePWMXAOneTimeSWForceActionEnum;
        ePWMXAContinuousSWForceActionEnum;
        ePWMXATbctrZeroEnum;
        ePWMXATbctrPrdEnum;
        ePWMXATbctrCMPAUpEnum;
        ePWMXATbctrCMPADownEnum;
        ePWMXATbctrCMPBUpEnum;
        ePWMXATbctrCMPBDownEnum;
        ePWMXAT1UpEnum;
        ePWMXAT1DownEnum;
        ePWMXAT2UpEnum;
        ePWMXAT2DownEnum;
        ePWMXBGlobalLoadRegisterEnum;
        ePWMXBShadowLoadEventEnum;
        ePWMXBOneTimeSWForceActionEnum;
        ePWMXBContinuousSWForceActionEnum;
        ePWMXBTbctrZeroEnum;
        ePWMXBTbctrPrdEnum;
        ePWMXBTbctrCMPAUpEnum;
        ePWMXBTbctrCMPADownEnum;
        ePWMXBTbctrCMPBUpEnum;
        ePWMXBTbctrCMPBDownEnum;
        ePWMXBT1UpEnum;
        ePWMXBT1DownEnum;
        ePWMXBT2UpEnum;
        ePWMXBT2DownEnum;
        TZAEventEnum;
        TZBEventEnum;
        DCAEVT1EventEnum;
        DCAEVT2EventEnum;
        DCBEVT1EventEnum;
        DCBEVT2EventEnum;
        TZBDEventEnum;
        TZBUEventEnum;
        TZADEventEnum;
        TZAUEventEnum;
        DCAEVT1UEventEnum;
        DCAEVT1DEventEnum;
        DCAEVT2UEventEnum;
        DCAEVT2DEventEnum;
        DCBEVT1UEventEnum;
        DCBEVT1DEventEnum;
        DCBEVT2UEventEnum;
        DCBEVT2DEventEnum;
        CBCLatchClearSignalEnum;
        DCAHighEnum;
        DCALowEnum;
        DCOutput1AConditionEnum;
        DCOutput2AConditionEnum;
        SynchModeDCAEVT1Enum;
        SignalSourceDCAEVT1Enum;
        CBCLatchModeDCAEVT1Enum;
        CBCLatchClearEventDCAEVT1Enum;
        SynchModeDCAEVT2Enum;
        SignalSourceDCAEVT2Enum;
        CBCLatchModeDCAEVT2Enum;
        CBCLatchClearEventDCAEVT2Enum;
        DCBHighEnum;
        DCBLowEnum;
        DCOutput1BConditionEnum;
        DCOutput2BConditionEnum;
        SynchModeDCBEVT1Enum;
        SignalSourceDCBEVT1Enum;
        CBCLatchModeDCBEVT1Enum;
        CBCLatchClearEventDCBEVT1Enum;
        SynchModeDCBEVT2Enum;
        SignalSourceDCBEVT2Enum;
        CBCLatchModeDCBEVT2Enum;
        CBCLatchClearEventDCBEVT2Enum;
        DCFilterInputEventSourceEnum;
        BlankingWindowStartEventEnum;
        
        EdgeFilterModeEnum;
        EdgeFilterEdgeCountEnum;
        EdgeFilterCounterCaptureSignalEnum;
        StartValleyCaptureEnum;
        StopValleyCaptureEnum;
        ValleyDelayDividerEnum;
        Capture_InputEnum;
        Invert_Capture_Input_PolarityEnum;
        Capture_gateEnum;
        Capture_gate_polarity_selectEnum;
        RisingEdgeDelayInputEnum;
        FallingEdgeDelayInputEnum;
        RisingEdgeDelayPolarityEnum;
        FallingEdgeDelayPolarityEnum;
        DeadbandGlobalLoadEnum;
        DeadbandControlShadowModeEnum;
        REDGlobalLoadRegisterEnum;
        REDShadowLoadEventEnum;
        FEDGlobalLoadRegisterEnum;
        FEDShadowLoadEventEnum;
        DeadBandCounterClockRateEnum;
        ChopperDutyCycleEnum;
        ChopperFrequencyEnum;
        ChopperFirstPulseWidthEnum;
        InterruptEventSourcesEnum;
        InterruptEventCountEnum;
        InterruptEventCountInitialValueEnum;
        SOCATriggerSourceEnum;
        SOCATriggerEventCountEnum;
        SOCATriggerEventCountInitValueEnum;
        SOCBTriggerSourceEnum;
        SOCBTriggerEventCountEnum;
        SOCBTriggerEventCountInitValueEnum;
        ControlModeChannelAEnum;
        ControlModeChannelBEnum;
        HRPWMSyncSourceEnum;
        HRPWMChannelAEdgeModeEnum;
        CMPAHRShadowToActiveLoadEventEnum;
        HRPWMChannelBEdgeModeEnum;
        CMPBHRShadowToActiveLoadEventEnum;
        HRPWMDeadbandEdgeModeEnum;
        ShadowActiveLoadDBREDHREnum;
        ShadowActiveLoadDBFEDHREnum;
        EPWMxBOutputSelectEnum;
        Allocate_XCMP_CMPAEnum;
        Allocate_XCMP_CMPBEnum;
        SpecifyXCMPActiveEnum;
        SpecifyXCMPShadow1Enum;
        SpecifyXCMPShadow2Enum;
        SpecifyXCMPShadow3Enum;
        EPWMXA_tbctr_XCMP1_activeEnum;
        EPWMXA_tbctr_XCMP2_activeEnum;
        EPWMXA_tbctr_XCMP3_activeEnum;
        EPWMXA_tbctr_XCMP4_activeEnum;
        EPWMXA_tbctr_XCMP5_activeEnum;
        EPWMXA_tbctr_XCMP6_activeEnum;
        EPWMXA_tbctr_XCMP7_activeEnum;
        EPWMXA_tbctr_XCMP8_activeEnum;
        EPWMXB_tbctr_XCMP5_activeEnum;
        EPWMXB_tbctr_XCMP6_activeEnum;
        EPWMXB_tbctr_XCMP7_activeEnum;
        EPWMXB_tbctr_XCMP8_activeEnum;
        EPWMXA_tbctr_XCMP1_SHADOW1Enum;
        EPWMXA_tbctr_XCMP2_SHADOW1Enum;
        EPWMXA_tbctr_XCMP3_SHADOW1Enum;
        EPWMXA_tbctr_XCMP4_SHADOW1Enum;
        EPWMXA_tbctr_XCMP5_SHADOW1Enum;
        EPWMXA_tbctr_XCMP6_SHADOW1Enum;
        EPWMXA_tbctr_XCMP7_SHADOW1Enum;
        EPWMXA_tbctr_XCMP8_SHADOW1Enum;
        EPWMXB_tbctr_XCMP5_SHADOW1Enum;
        EPWMXB_tbctr_XCMP6_SHADOW1Enum;
        EPWMXB_tbctr_XCMP7_SHADOW1Enum;
        EPWMXB_tbctr_XCMP8_SHADOW1Enum;
        EPWMXA_tbctr_XCMP1_SHADOW2Enum;
        EPWMXA_tbctr_XCMP2_SHADOW2Enum;
        EPWMXA_tbctr_XCMP3_SHADOW2Enum;
        EPWMXA_tbctr_XCMP4_SHADOW2Enum;
        EPWMXA_tbctr_XCMP5_SHADOW2Enum;
        EPWMXA_tbctr_XCMP6_SHADOW2Enum;
        EPWMXA_tbctr_XCMP7_SHADOW2Enum;
        EPWMXA_tbctr_XCMP8_SHADOW2Enum;
        EPWMXB_tbctr_XCMP5_SHADOW2Enum;
        EPWMXB_tbctr_XCMP6_SHADOW2Enum;
        EPWMXB_tbctr_XCMP7_SHADOW2Enum;
        EPWMXB_tbctr_XCMP8_SHADOW2Enum;
        EPWMXA_tbctr_XCMP1_SHADOW3Enum;
        EPWMXA_tbctr_XCMP2_SHADOW3Enum;
        EPWMXA_tbctr_XCMP3_SHADOW3Enum;
        EPWMXA_tbctr_XCMP4_SHADOW3Enum;
        EPWMXA_tbctr_XCMP5_SHADOW3Enum;
        EPWMXA_tbctr_XCMP6_SHADOW3Enum;
        EPWMXA_tbctr_XCMP7_SHADOW3Enum;
        EPWMXA_tbctr_XCMP8_SHADOW3Enum;
        EPWMXB_tbctr_XCMP5_SHADOW3Enum;
        EPWMXB_tbctr_XCMP6_SHADOW3Enum;
        EPWMXB_tbctr_XCMP7_SHADOW3Enum;
        EPWMXB_tbctr_XCMP8_SHADOW3Enum;
        Load_ModeEnum;
        Shadow_LevelEnum;
        Shadow_register_load_onceEnum;
    end

    properties (Access = private)
        % Pre-computed constants.
        
        baseaddr=0;
        module_instance=0;
        
    end
    
    methods
        % Constructor
        function obj = EPWM(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.ePWMModuleEnum(obj)
            if isequal(obj.ePWMModule,'EPWM0')
                ret = uint16(0);
            elseif isequal(obj.ePWMModule, 'EPWM1')
                ret = uint16(1);
            elseif isequal(obj.ePWMModule, 'EPWM2')
                ret = uint16(2);
            elseif isequal(obj.ePWMModule, 'EPWM3')
                ret = uint16(3);
            elseif isequal(obj.ePWMModule, 'EPWM4')
                ret = uint16(4);
            elseif isequal(obj.ePWMModule, 'EPWM5')
                ret = uint16(5);
            elseif isequal(obj.ePWMModule, 'EPWM6')
                ret = uint16(6);
            elseif isequal(obj.ePWMModule, 'EPWM7')
                ret = uint16(7);
            elseif isequal(obj.ePWMModule, 'EPWM8')
                ret = uint16(8);
            elseif isequal(obj.ePWMModule, 'EPWM9')
                ret = uint16(9);
            elseif isequal(obj.ePWMModule, 'EPWM10')
                ret = uint16(10);
            elseif isequal(obj.ePWMModule, 'EPWM11')
                ret = uint16(11);
            elseif isequal(obj.ePWMModule, 'EPWM12')
                ret = uint16(12);
            elseif isequal(obj.ePWMModule, 'EPWM13')
                ret = uint16(13);
            elseif isequal(obj.ePWMModule, 'EPWM14')
                ret = uint16(14);
            elseif isequal(obj.ePWMModule, 'EPWM15')
                ret = uint16(15);
            elseif isequal(obj.ePWMModule, 'EPWM16')
                ret = uint16(16);
            elseif isequal(obj.ePWMModule, 'EPWM17')
                ret = uint16(17);
            elseif isequal(obj.ePWMModule, 'EPWM18')
                ret = uint16(18);
            elseif isequal(obj.ePWMModule, 'EPWM19')
                ret = uint16(19);
            elseif isequal(obj.ePWMModule, 'EPWM20')
                ret = uint16(20);
            elseif isequal(obj.ePWMModule, 'EPWM21')
                ret = uint16(21);
            elseif isequal(obj.ePWMModule, 'EPWM22')
                ret = uint16(22);
            elseif isequal(obj.ePWMModule, 'EPWM23')
                ret = uint16(23);
            elseif isequal(obj.ePWMModule, 'EPWM24')
                ret = uint16(24);
            elseif isequal(obj.ePWMModule, 'EPWM25')
                ret = uint16(25);
            elseif isequal(obj.ePWMModule, 'EPWM26')
                ret = uint16(26);
            elseif isequal(obj.ePWMModule, 'EPWM27')
                ret = uint16(27);
            elseif isequal(obj.ePWMModule, 'EPWM28')
                ret = uint16(28);
            elseif isequal(obj.ePWMModule, 'EPWM29')
                ret = uint16(29);
            elseif isequal(obj.ePWMModule, 'EPWM30')
                ret = uint16(30);
            elseif isequal(obj.ePWMModule, 'EPWM31')
                ret = uint16(31);
            else
                ret = uint16(0);

            end
        end

        function ret = get.EmulationModeEnum(obj)
            if isequal(obj.EmulationMode, 'Stop after next time base counter increaments or decreaments')
                ret = uint16(0);
            elseif isequal(obj.EmulationMode, 'Stop when counter completes whole cycle')
                ret = uint16(1);
            elseif isequal(obj.EmulationMode, 'Free run')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TBCLKEnum(obj)
            if isequal(obj.TBCLK, 'Divide clock by 1')
                ret = uint16(0);
            elseif isequal(obj.TBCLK, 'Divide clock by 2')
                ret = uint16(1);
            elseif isequal(obj.TBCLK, 'Divide clock by 4')
                ret = uint16(2);
            elseif isequal(obj.TBCLK, 'Divide clock by 8')
                ret = uint16(3);
            elseif isequal(obj.TBCLK, 'Divide clock by 16')
                ret = uint16(4);
            elseif isequal(obj.TBCLK, 'Divide clock by 32')
                ret = uint16(5);
            elseif isequal(obj.TBCLK, 'Divide clock by 64')
                ret = uint16(6);
            elseif isequal(obj.TBCLK, 'Divide clock by 128')
                ret = uint16(7);
            else
                ret = uint16(0);
            end
        end

        function ret = get.HSPCLKDIVEnum(obj)
            if isequal(obj.HSPCLKDIV, 'Divide clock by 1')
                ret = uint16(0);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 2')
                ret = uint16(1);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 4')
                ret = uint16(2);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 6')
                ret = uint16(3);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 8')
                ret = uint16(4);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 10')
                ret = uint16(5);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 12')
                ret = uint16(6);
            elseif isequal(obj.HSPCLKDIV, 'Divide clock by 14')
                ret = uint16(7);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TimerPeriodUnitsEnum(obj)
            if isequal(obj.TimerPeriodUnits, 'Clock Cycles')
                ret = uint16(0);
            elseif isequal(obj.TimerPeriodUnits, 'Seconds')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SpecifyTimerPeriodEnum(obj)
            if isequal(obj.SpecifyTimerPeriod, 'Specify via Dialog Box')
                ret = uint16(0);
            elseif isequal(obj.SpecifyTimerPeriod, 'Input Port')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMLinkEnum(obj)
            if isequal(obj.ePWMLink, 'Disable Linking')
                ret = uint16(0);
            elseif isequal(obj.ePWMLink, 'Link with ePWM0')
                ret = uint16(1);
            elseif isequal(obj.ePWMLink, 'Link with ePWM1')
                ret = uint16(2);
            elseif isequal(obj.ePWMLink, 'Link with ePWM2')
                ret = uint16(3);
            elseif isequal(obj.ePWMLink, 'Link with ePWM3')
                ret = uint16(4);
            elseif isequal(obj.ePWMLink, 'Link with ePWM4')
                ret = uint16(5);
            elseif isequal(obj.ePWMLink, 'Link with ePWM5')
                ret = uint16(6);
            elseif isequal(obj.ePWMLink, 'Link with ePWM6')
                ret = uint16(7);
            elseif isequal(obj.ePWMLink, 'Link with ePWM7')
                ret = uint16(8);
            elseif isequal(obj.ePWMLink, 'Link with ePWM8')
                ret = uint16(9);
            elseif isequal(obj.ePWMLink, 'Link with ePWM9')
                ret = uint16(10);
            elseif isequal(obj.ePWMLink, 'Link with ePWM10')
                ret = uint16(11);
            elseif isequal(obj.ePWMLink, 'Link with ePWM11')
                ret = uint16(12);
            elseif isequal(obj.ePWMLink, 'Link with ePWM12')
                ret = uint16(13);
            elseif isequal(obj.ePWMLink, 'Link with ePWM13')
                ret = uint16(14);
            elseif isequal(obj.ePWMLink, 'Link with ePWM14')
                ret = uint16(15);
            elseif isequal(obj.ePWMLink, 'Link with ePWM15')
                ret = uint16(16);
            elseif isequal(obj.ePWMLink, 'Link with ePWM16')
                ret = uint16(17);
            elseif isequal(obj.ePWMLink, 'Link with ePWM17')
                ret = uint16(18);
            elseif isequal(obj.ePWMLink, 'Link with ePWM18')
                ret = uint16(19);
            elseif isequal(obj.ePWMLink, 'Link with ePWM19')
                ret = uint16(20);
            elseif isequal(obj.ePWMLink, 'Link with ePWM20')
                ret = uint16(21);
            elseif isequal(obj.ePWMLink, 'Link with ePWM21')
                ret = uint16(22);
            elseif isequal(obj.ePWMLink, 'Link with ePWM22')
                ret = uint16(23);
            elseif isequal(obj.ePWMLink, 'Link with ePWM23')
                ret = uint16(24);
            else
                ret = uint16(0);
            end
        end

        function ret = get.GlobalRegisterLoadEnum(obj)
            if isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.GlobalRegisterLoad, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.PeriodLoadModeEnum(obj)
            if isequal(obj.PeriodLoadMode, 'Using Shadow registers')
                ret = uint16(0);
            elseif isequal(obj.PeriodLoadMode, 'Direct access')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.PeriodLoadEventEnum(obj)
            if isequal(obj.PeriodLoadEvent, 'Shadow to Active load when counter reaches zero')
                ret = uint16(0);
            elseif isequal(obj.PeriodLoadEvent, 'Shadow to Active load when counter reaches zero and sync occurs')
                ret = uint16(1);
            elseif isequal(obj.PeriodLoadEvent, 'Shadow to Active load when sync occurs')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CounterModeEnum(obj)
            if isequal(obj.CounterMode, 'Up count')
                ret = uint16(0);
            elseif isequal(obj.CounterMode, 'Down count')
                ret = uint16(1);
            elseif isequal(obj.CounterMode, 'Up-Down Count')
                ret = uint16(2);
            elseif isequal(obj.CounterMode, 'Stop Freeze')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SyncInPulseSrcEnum(obj)
            if isequal(obj.SyncInPulseSrc, 'Disable Sync-in')
                ret = 0x0;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM0 sync-out signal')
                ret = 0x1;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM1 sync-out signal')
                ret = 0x2;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM2 sync-out signal')
                ret = 0x3;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM3 sync-out signal')
                ret = 0x4;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM4 sync-out signal')
                ret = 0x5;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM5 sync-out signal')
                ret = 0x6;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM6 sync-out signal')
                ret = 0x7;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM7 sync-out signal')
                ret = 0x8;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM8 sync-out signal')
                ret = 0x9;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM9 sync-out signal')
                ret = 0xA;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM10 sync-out signal')
                ret = 0xB;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM11 sync-out signal')
                ret = 0xC;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM12 sync-out signal')
                ret = 0xD;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM13 sync-out signal')
                ret = 0xE;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM14 sync-out signal')
                ret = 0xF;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM15 sync-out signal')
                ret = 0x10;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM16 sync-out signal')
                ret = 0x11;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM17 sync-out signal')
                ret = 0x12;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM18 sync-out signal')
                ret = 0x13;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM19 sync-out signal')
                ret = 0x14;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM20 sync-out signal')
                ret = 0x15;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM21 sync-out signal')
                ret = 0x16;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM22 sync-out signal')
                ret = 0x17;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is EPWM23 sync-out signal')
                ret = 0x18;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP0 sync-out signal')
                ret = 0x40;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP1 sync-out signal')
                ret = 0x41;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP2 sync-out signal')
                ret = 0x42;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP3 sync-out signal')
                ret = 0x43;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP4 sync-out signal')
                ret = 0x44;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP5 sync-out signal')
                ret = 0x45;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP6 sync-out signal')
                ret = 0x46;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP7 sync-out signal')
                ret = 0x47;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP8 sync-out signal')
                ret = 0x48;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP9 sync-out signal')
                ret = 0x49;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP10 sync-out signal')
                ret = 0x4A;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP11 sync-out signal')
                ret = 0x4B;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP12 sync-out signal')
                ret = 0x4C;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP13 sync-out signal')
                ret = 0x4D;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP14 sync-out signal')
                ret = 0x4E;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is ECAP15 sync-out signal')
                ret = 0x4F;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is Input XBAR Out4 signal')
                ret = 0x50;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is Input XBAR Out20 signal')
                ret = 0x51;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is C2K Timesync XBAR PWM Output 0 signal')
                ret = 0x58;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is C2K Timesync XBAR PWM Output 1 signal')
                ret = 0x59;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX0 Trigger0 signal')
                ret = 0x60;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX0 Trigger1 signal')
                ret = 0x61;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX0 Trigger2 signal')
                ret = 0x62;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX0 Trigger3 signal')
                ret = 0x63;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX1 Trigger0 signal')
                ret = 0x64;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX1 Trigger1 signal')
                ret = 0x65;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX1 Trigger2 signal')
                ret = 0x66;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX1 Trigger3 signal')
                ret = 0x67;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX2 Trigger0 signal')
                ret = 0x68;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX2 Trigger1 signal')
                ret = 0x69;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX2 Trigger2 signal')
                ret = 0x6A;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX2 Trigger3 signal')
                ret = 0x6B;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX3 Trigger0 signal')
                ret = 0x6C;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX3 Trigger1 signal')
                ret = 0x6D;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX3 Trigger2 signal')
                ret = 0x6E;
            elseif isequal(obj.SyncInPulseSrc, 'Sync-in source is FSIRX3 Trigger3 signal')
                ret = 0x6F;
            else
                ret = 0x0;
            end
        end

        function ret = get.OneShotTriggerEnum(obj)
            if isequal(obj.OneShotTrigger, 'Trigger is one-shot sync')
                ret = uint16(0);
            elseif isequal(obj.OneShotTrigger, 'Trigger is one-shot reload')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMLinkCMPAEnum(obj)
            if isequal(obj.ePWMLinkCMPA, 'Link with ePWM0')
                ret = uint16(0);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM1')
                ret = uint16(1);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM2')
                ret = uint16(2);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM3')
                ret = uint16(3);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM4')
                ret = uint16(4);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM5')
                ret = uint16(5);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM6')
                ret = uint16(6);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM7')
                ret = uint16(7);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM8')
                ret = uint16(8);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM9')
                ret = uint16(9);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM10')
                ret = uint16(10);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM11')
                ret = uint16(11);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM12')
                ret = uint16(12);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM13')
                ret = uint16(13);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM14')
                ret = uint16(14);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM15')
                ret = uint16(15);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM16')
                ret = uint16(16);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM17')
                ret = uint16(17);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM18')
                ret = uint16(18);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM19')
                ret = uint16(19);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM20')
                ret = uint16(20);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM21')
                ret = uint16(21);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM22')
                ret = uint16(22);
            elseif isequal(obj.ePWMLinkCMPA, 'Link with ePWM23')
                ret = uint16(23);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CMPAUnitsEnum(obj)
            if isequal(obj.CMPAUnits, 'Clock Cycles')
                ret = uint16(0);
            elseif isequal(obj.CMPAUnits, 'Seconds')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SpecifyCMPAViaEnum(obj)
            if isequal(obj.SpecifyCMPAVia, 'Specify via Dialog Box')
                ret = uint16(0);
            elseif isequal(obj.SpecifyCMPAVia, 'Input Port')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.GlobalLoadRegisterCMPAEnum(obj)
            if isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.GlobalLoadRegisterCMPA, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
            
        end

        function ret = get.SHDWAMODEEnum(obj)
            if isequal(obj.SHDWAMODE, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.SHDWAMODE, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.SHDWAMODE, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.SHDWAMODE, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.SHDWAMODE, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.SHDWAMODE, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.SHDWAMODE, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.SHDWAMODE, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMLinkCMPBEnum(obj)
            if isequal(obj.ePWMLinkCMPB, 'Link with ePWM0')
                ret = uint16(0);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM1')
                ret = uint16(1);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM2')
                ret = uint16(2);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM3')
                ret = uint16(3);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM4')
                ret = uint16(4);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM5')
                ret = uint16(5);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM6')
                ret = uint16(6);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM7')
                ret = uint16(7);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM8')
                ret = uint16(8);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM9')
                ret = uint16(9);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM10')
                ret = uint16(10);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM11')
                ret = uint16(11);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM12')
                ret = uint16(12);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM13')
                ret = uint16(13);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM14')
                ret = uint16(14);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM15')
                ret = uint16(15);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM16')
                ret = uint16(16);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM17')
                ret = uint16(17);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM18')
                ret = uint16(18);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM19')
                ret = uint16(19);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM20')
                ret = uint16(20);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM21')
                ret = uint16(21);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM22')
                ret = uint16(22);
            elseif isequal(obj.ePWMLinkCMPB, 'Link with ePWM23')
                ret = uint16(23);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CMPBUnitsEnum(obj)
            if isequal(obj.CMPBUnits, 'Clock Cycles')
                ret = uint16(0);
            elseif isequal(obj.CMPBUnits, 'Seconds')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SpecifyCMPBViaEnum(obj)
            if isequal(obj.SpecifyCMPBVia, 'Specify via Dialog Box')
                ret = uint16(0);
            elseif isequal(obj.SpecifyCMPBVia, 'Input Port')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.GlobalLoadRegisterCMPBEnum(obj)
            if isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.GlobalLoadRegisterCMPB, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.SHDWBMODEEnum(obj)
            if isequal(obj.SHDWBMODE, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.SHDWBMODE, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.SHDWBMODE, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.SHDWBMODE, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.SHDWBMODE, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.SHDWBMODE, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.SHDWBMODE, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.SHDWBMODE, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMLinkCMPCEnum(obj)
            if isequal(obj.ePWMLinkCMPC, 'Link with ePWM0')
                ret = uint16(0);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM1')
                ret = uint16(1);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM2')
                ret = uint16(2);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM3')
                ret = uint16(3);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM4')
                ret = uint16(4);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM5')
                ret = uint16(5);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM6')
                ret = uint16(6);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM7')
                ret = uint16(7);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM8')
                ret = uint16(8);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM9')
                ret = uint16(9);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM10')
                ret = uint16(10);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM11')
                ret = uint16(11);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM12')
                ret = uint16(12);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM13')
                ret = uint16(13);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM14')
                ret = uint16(14);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM15')
                ret = uint16(15);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM16')
                ret = uint16(16);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM17')
                ret = uint16(17);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM18')
                ret = uint16(18);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM19')
                ret = uint16(19);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM20')
                ret = uint16(20);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM21')
                ret = uint16(21);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM22')
                ret = uint16(22);
            elseif isequal(obj.ePWMLinkCMPC, 'Link with ePWM23')
                ret = uint16(23);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CMPCUnitsEnum(obj)
            if isequal(obj.CMPCUnits, 'Clock Cycles')
                ret = uint16(0);
            elseif isequal(obj.CMPCUnits, 'Seconds')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SpecifyCMPCViaEnum(obj)
            if isequal(obj.SpecifyCMPCVia, 'Specify via Dialog Box')
                ret = uint16(0);
            elseif isequal(obj.SpecifyCMPCVia, 'Input Port')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.GlobalLoadRegisterCMPCEnum(obj)
            if isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.GlobalLoadRegisterCMPC, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.SHDWCMODEEnum(obj)
            if isequal(obj.SHDWCMODE, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.SHDWCMODE, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.SHDWCMODE, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.SHDWCMODE, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.SHDWCMODE, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.SHDWCMODE, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.SHDWCMODE, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.SHDWCMODE, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMLinkCMPDEnum(obj)
            if isequal(obj.ePWMLinkCMPD, 'Link with ePWM0')
                ret = uint16(0);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM1')
                ret = uint16(1);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM2')
                ret = uint16(2);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM3')
                ret = uint16(3);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM4')
                ret = uint16(4);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM5')
                ret = uint16(5);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM6')
                ret = uint16(6);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM7')
                ret = uint16(7);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM8')
                ret = uint16(8);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM9')
                ret = uint16(9);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM10')
                ret = uint16(10);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM11')
                ret = uint16(11);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM12')
                ret = uint16(12);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM13')
                ret = uint16(13);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM14')
                ret = uint16(14);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM15')
                ret = uint16(15);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM16')
                ret = uint16(16);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM17')
                ret = uint16(17);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM18')
                ret = uint16(18);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM19')
                ret = uint16(19);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM20')
                ret = uint16(20);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM21')
                ret = uint16(21);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM22')
                ret = uint16(22);
            elseif isequal(obj.ePWMLinkCMPD, 'Link with ePWM23')
                ret = uint16(23);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CMPDUnitsEnum(obj)
            if isequal(obj.CMPDUnits, 'Clock Cycles')
                ret = uint16(0);
            elseif isequal(obj.CMPDUnits, 'Seconds')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SpecifyCMPDViaEnum(obj)
            if isequal(obj.SpecifyCMPDVia, 'Specify via Dialog Box')
                ret = uint16(0);
            elseif isequal(obj.SpecifyCMPDVia, 'Input Port')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.GlobalLoadRegisterCMPDEnum(obj)
            if isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.GlobalLoadRegisterCMPD, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.SHDWDMODEEnum(obj)
            if isequal(obj.SHDWDMODE, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.SHDWDMODE, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.SHDWDMODE, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.SHDWDMODE, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.SHDWDMODE, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.SHDWDMODE, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.SHDWDMODE, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.SHDWDMODE, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ContSWForceGlobalLoadRegisterEnum(obj)
            if isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.ContSWForceGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.SoftwareForceShadowModeEnum(obj)
            if isequal(obj.SoftwareForceShadowMode, 'Shadow mode load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.SoftwareForceShadowMode, 'Shadow mode load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.SoftwareForceShadowMode, 'Shadow mode load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.SoftwareForceShadowMode, 'Immediate mode')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.T1TriggerSourceEnum(obj)
            if isequal(obj.T1TriggerSource, 'Digital compare event A 1')
                ret = uint16(0);
            elseif isequal(obj.T1TriggerSource, 'Digital compare event A 2')
                ret = uint16(1);
            elseif isequal(obj.T1TriggerSource, 'Digital compare event B 1')
                ret = uint16(2);
            elseif isequal(obj.T1TriggerSource, 'Digital compare event B 2')
                ret = uint16(3);
            elseif isequal(obj.T1TriggerSource, 'Trip zone 1')
                ret = uint16(4);
            elseif isequal(obj.T1TriggerSource, 'Trip zone 2')
                ret = uint16(5);
            elseif isequal(obj.T1TriggerSource, 'Trip zone 3')
                ret = uint16(6);
            elseif isequal(obj.T1TriggerSource, 'ePWM sync')
                ret = uint16(7);
            elseif isequal(obj.T1TriggerSource, 'Digital compare filter event')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.T2TriggerSourceEnum(obj)
            if isequal(obj.T2TriggerSource, 'Digital compare event A 1')
                ret = uint16(0);
            elseif isequal(obj.T2TriggerSource, 'Digital compare event A 2')
                ret = uint16(1);
            elseif isequal(obj.T2TriggerSource, 'Digital compare event B 1')
                ret = uint16(2);
            elseif isequal(obj.T2TriggerSource, 'Digital compare event B 2')
                ret = uint16(3);
            elseif isequal(obj.T2TriggerSource, 'Trip zone 1')
                ret = uint16(4);
            elseif isequal(obj.T2TriggerSource, 'Trip zone 2')
                ret = uint16(5);
            elseif isequal(obj.T2TriggerSource, 'Trip zone 3')
                ret = uint16(6);
            elseif isequal(obj.T2TriggerSource, 'ePWM sync')
                ret = uint16(7);
            elseif isequal(obj.T2TriggerSource, 'Digital compare filter event')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAGlobalLoadRegisterEnum(obj)
            if isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.ePWMXAGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.ePWMXAShadowLoadEventEnum(obj)
            if isequal(obj.ePWMXAShadowLoadEvent, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.ePWMXAShadowLoadEvent, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end
 
        function ret = get.ePWMXAOneTimeSWForceActionEnum(obj)
            if isequal(obj.ePWMXAOneTimeSWForceAction, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAOneTimeSWForceAction, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAOneTimeSWForceAction, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAOneTimeSWForceAction, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAContinuousSWForceActionEnum(obj)
            if isequal(obj.ePWMXAContinuousSWForceAction, 'Software forcing disabled')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAContinuousSWForceAction, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAContinuousSWForceAction, 'Set output pins to high')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrZeroEnum(obj)
            if isequal(obj.ePWMXATbctrZero, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrZero, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrZero, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrZero, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrPrdEnum(obj)
            if isequal(obj.ePWMXATbctrPrd, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrPrd, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrPrd, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrPrd, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrCMPAUpEnum(obj)
            if isequal(obj.ePWMXATbctrCMPAUp, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrCMPAUp, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrCMPAUp, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrCMPAUp, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrCMPADownEnum(obj)
            if isequal(obj.ePWMXATbctrCMPADown, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrCMPADown, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrCMPADown, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrCMPADownp, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrCMPBUpEnum(obj)
            if isequal(obj.ePWMXATbctrCMPBUp, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrCMPBUp, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrCMPBUp, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrCMPBUp, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXATbctrCMPBDownEnum(obj)
            if isequal(obj.ePWMXATbctrCMPBDown, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXATbctrCMPBDown, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXATbctrCMPBDown, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXATbctrCMPBDown, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAT1UpEnum(obj)
            if isequal(obj.ePWMXAT1Up, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAT1Up, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAT1Up, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAT1Up, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAT1DownEnum(obj)
            if isequal(obj.ePWMXAT1Down, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAT1Down, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAT1Down, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAT1Down, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAT2UpEnum(obj)
            if isequal(obj.ePWMXAT2Up, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAT2Up, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAT2Up, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAT2Up, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXAT2DownEnum(obj)
            if isequal(obj.ePWMXAT2Down, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXAT2Down, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXAT2Down, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXAT2Down, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBGlobalLoadRegisterEnum(obj)
            if isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = 0x1;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = 0x2;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = 0x4;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPC')
                ret = 0x8;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPD')
                ret = 0x10;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = 0x20;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = 0x40;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_DBCTL')
                ret = 0x80;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = 0x100;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = 0x200;
            elseif isequal(obj.ePWMXBGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = 0x400;
            else
                ret = 0x10;
            end
        end

        function ret = get.ePWMXBShadowLoadEventEnum(obj)
            if isequal(obj.ePWMXBShadowLoadEvent, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load when counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Freeze shadow to active load')
                ret = uint16(3);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load on sync or when counter equals zero')
                ret = uint16(4);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load on sync or when counter equals period')
                ret = uint16(5);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load on sync or when counter equals zero or period')
                ret = uint16(6);
            elseif isequal(obj.ePWMXBShadowLoadEvent, 'Load on sync only')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end
 
        function ret = get.ePWMXBOneTimeSWForceActionEnum(obj)
            if isequal(obj.ePWMXBOneTimeSWForceAction, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBOneTimeSWForceAction, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBOneTimeSWForceAction, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBOneTimeSWForceAction, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBContinuousSWForceActionEnum(obj)
            if isequal(obj.ePWMXBContinuousSWForceAction, 'Software forcing disabled')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBContinuousSWForceAction, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBContinuousSWForceAction, 'Set output pins to high')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrZeroEnum(obj)
            if isequal(obj.ePWMXBTbctrZero, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrZero, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrZero, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrZero, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrPrdEnum(obj)
            if isequal(obj.ePWMXBTbctrPrd, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrPrd, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrPrd, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrPrd, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrCMPAUpEnum(obj)
            if isequal(obj.ePWMXBTbctrCMPAUp, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrCMPAUp, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrCMPAUp, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrCMPAUp, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrCMPADownEnum(obj)
            if isequal(obj.ePWMXBTbctrCMPADown, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrCMPADown, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrCMPADown, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrCMPADown, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrCMPBUpEnum(obj)
            if isequal(obj.ePWMXBTbctrCMPBUp, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrCMPBUp, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrCMPBUp, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrCMPBUp, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBTbctrCMPBDownEnum(obj)
            if isequal(obj.ePWMXBTbctrCMPBDown, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBTbctrCMPBDown, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBTbctrCMPBDown, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBTbctrCMPBDown, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBT1UpEnum(obj)
            if isequal(obj.ePWMXBT1Up, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBT1Up, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBT1Up, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBT1Up, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end


        function ret = get.ePWMXBT1DownEnum(obj)
            if isequal(obj.ePWMXBT1Down, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBT1Down, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBT1Down, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBT1Down, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBT2UpEnum(obj)
            if isequal(obj.ePWMXBT2Up, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBT2Up, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBT2Up, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBT2Up, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ePWMXBT2DownEnum(obj)
            if isequal(obj.ePWMXBT2Down, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.ePWMXBT2Down, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.ePWMXBT2Down, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.ePWMXBT2Down, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZAEventEnum(obj)
            if isequal(obj.TZAEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZAEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZAEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZAEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZBEventEnum(obj)
            if isequal(obj.TZBEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZBEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZBEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZBEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT1EventEnum(obj)
            if isequal(obj.DCAEVT1Event, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT1Event, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT1Event, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT1Event, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT2EventEnum(obj)
            if isequal(obj.DCAEVT2Event, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT2Event, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT2Event, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT2Event, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT1EventEnum(obj)
            if isequal(obj.DCBEVT1Event, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT1Event, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT1Event, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCBEVT1Event, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT2EventEnum(obj)
            if isequal(obj.DCBEVT2Event, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT2Event, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT2Event, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCBEVT2Event, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZBDEventEnum(obj)
            if isequal(obj.TZBDEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZBDEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZBDEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZBDEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZBUEventEnum(obj)
            if isequal(obj.TZBUEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZBUEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZBUEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZBUEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZADEventEnum(obj)
            if isequal(obj.TZADEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZADEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZADEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZADEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.TZAUEventEnum(obj)
            if isequal(obj.TZAUEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.TZAUEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.TZAUEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.TZAUEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT1UEventEnum(obj)
            if isequal(obj.DCAEVT1UEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT1UEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT1UEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT1UEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT1DEventEnum(obj)
            if isequal(obj.DCAEVT1DEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT1DEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT1DEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT1DEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT2UEventEnum(obj)
            if isequal(obj.DCAEVT2UEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT2UEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT2UEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT2UEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAEVT2DEventEnum(obj)
            if isequal(obj.DCAEVT2DEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCAEVT2DEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCAEVT2DEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT2DEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT1UEventEnum(obj)
            if isequal(obj.DCBEVT1UEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT1UEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT1UEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCBEVT1UEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT1DEventEnum(obj)
            if isequal(obj.DCBEVT1DEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT1DEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT1DEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCAEVT1DEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT2UEventEnum(obj)
            if isequal(obj.DCBEVT2UEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT2UEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT2UEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCBEVT2UEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBEVT2DEventEnum(obj)
            if isequal(obj.DCAEVT2DEvent, 'High impedance output')
                ret = uint16(0);
            elseif isequal(obj.DCBEVT2DEvent, 'High voltage state')
                ret = uint16(1);
            elseif isequal(obj.DCBEVT2DEvent, 'Low voltage state')
                ret = uint16(2);
            elseif isequal(obj.DCBEVT2DEvent, 'Disable action')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CBCLatchClearSignalEnum(obj)
            if isequal(obj.CBCLatchClearSignal, 'Clear CBC Pulse when counter reaches zero')
                ret = uint16(0);
            elseif isequal(obj.CBCLatchClearSignal, 'Clear CBC Pulse when counter reaches period')
                ret = uint16(1);
            elseif isequal(obj.CBCLatchClearSignal, 'Clear CBC Pulse when counter reaches zero or period')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCAHighEnum(obj)
            if isequal(obj.DCAHigh, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.DCAHigh, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.DCAHigh, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.DCAHigh, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.DCAHigh, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.DCAHigh, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.DCAHigh, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.DCAHigh, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.DCAHigh, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.DCAHigh, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.DCAHigh, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.DCAHigh, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.DCAHigh, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.DCAHigh, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.DCAHigh, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.DCAHigh, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCALowEnum(obj)
            if isequal(obj.DCALow, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.DCALow, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.DCALow, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.DCALow, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.DCALow, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.DCALow, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.DCALow, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.DCALow, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.DCALow, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.DCALow, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.DCALow, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.DCALow, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.DCALow, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.DCALow, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.DCALow, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.DCALow, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCOutput1AConditionEnum(obj)
            if isequal(obj.DCOutput1ACondition, 'Event is disabled')
                ret = uint16(0);
            elseif isequal(obj.DCOutput1ACondition, 'Event when DCxH low')
                ret = uint16(1);
            elseif isequal(obj.DCOutput1ACondition, 'Event when DCxH high')
                ret = uint16(2);
            elseif isequal(obj.DCOutput1ACondition, 'Event when DCxL low')
                ret = uint16(3);
            elseif isequal(obj.DCOutput1ACondition, 'Event when DCxL high')
                ret = uint16(4);
            elseif isequal(obj.DCOutput1ACondition, 'Event when DCxL high DCxH low')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCOutput2AConditionEnum(obj)
            if isequal(obj.DCOutput2ACondition, 'Event is disabled')
                ret = uint16(0);
            elseif isequal(obj.DCOutput2ACondition, 'Event when DCxH low')
                ret = uint16(1);
            elseif isequal(obj.DCOutput2ACondition, 'Event when DCxH high')
                ret = uint16(2);
            elseif isequal(obj.DCOutput2ACondition, 'Event when DCxL low')
                ret = uint16(3);
            elseif isequal(obj.DCOutput2ACondition, 'Event when DCxL high')
                ret = uint16(4);
            elseif isequal(obj.DCOutput2ACondition, 'Event when DCxL high DCxH low')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SynchModeDCAEVT1Enum(obj)
            if isequal(obj.SynchModeDCAEVT1, 'DC input signal is synced with TBCLK')
                ret = uint16(0);
            elseif isequal(obj.SynchModeDCAEVT1, 'DC input signal is not synced with TBCLK')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SignalSourceDCAEVT1Enum(obj)
            if isequal(obj.SignalSourceDCAEVT1, 'Signal source is unfiltered')
                ret = uint16(0);
            elseif isequal(obj.SignalSourceDCAEVT1, 'Signal source is filtered')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CBCLatchModeDCAEVT1Enum(obj)
            if isequal(obj.CBCLatchModeDCAEVT1, 'DC CBC latch is disabled')
                ret = 0x0;
            elseif isequal(obj.CBCLatchModeDCAEVT1, 'DC CBC latch is enabled')
                ret = 0x1;
            else
                ret = 0x0;
            end
        end

        function ret = get.CBCLatchClearEventDCAEVT1Enum(obj)
            if isequal(obj.CBCLatchClearEventDCAEVT1, 'Clear CBC latch when counter equals zero')
                ret = 0x0;
            elseif isequal(obj.CBCLatchClearEventDCAEVT1, 'Clear CBC latch when counter equals period')
                ret = 0x1;
            elseif isequal(obj.CBCLatchClearEventDCAEVT1, 'Clear CBC latch when counter equals zero or period')
                ret = 0x2;
            else
                ret = 0x0;
            end
        end

        function ret = get.SynchModeDCAEVT2Enum(obj)
            if isequal(obj.SynchModeDCAEVT2, 'DC input signal is synced with TBCLK')
                ret = uint16(0);
            elseif isequal(obj.SynchModeDCAEVT2, 'DC input signal is not synced with TBCLK')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SignalSourceDCAEVT2Enum(obj)
            if isequal(obj.SignalSourceDCAEVT2, 'Signal source is unfiltered')
                ret = uint16(0);
            elseif isequal(obj.SignalSourceDCAEVT2, 'Signal source is filtered')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CBCLatchModeDCAEVT2Enum(obj)
            if isequal(obj.CBCLatchModeDCAEVT2, 'DC CBC latch is disabled')
                ret = 0x0;
            elseif isequal(obj.CBCLatchModeDCAEVT2, 'DC CBC latch is enabled')
                ret = 0x1;
            else
                ret = 0x0;
            end
        end

        function ret = get.CBCLatchClearEventDCAEVT2Enum(obj)
            if isequal(obj.CBCLatchClearEventDCAEVT2, 'Clear CBC latch when counter equals zero')
                ret = 0x0;
            elseif isequal(obj.CBCLatchClearEventDCAEVT2, 'Clear CBC latch when counter equals period')
                ret = 0x1;
            elseif isequal(obj.CBCLatchClearEventDCAEVT2, 'Clear CBC latch when counter equals zero or period')
                ret = 0x2;
            else
                ret = 0x0;
            end
        end

        function ret = get.DCBHighEnum(obj)
            if isequal(obj.DCBHigh, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.DCBHigh, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.DCBHigh, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.DCBHigh, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.DCBHigh, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.DCBHigh, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.DCBHigh, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.DCBHigh, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.DCBHigh, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.DCBHigh, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.DCBHigh, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.DCBHigh, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.DCBHigh, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.DCBHigh, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.DCBHigh, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.DCBHigh, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCBLowEnum(obj)
            if isequal(obj.DCBLow, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.DCBLow, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.DCBLow, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.DCBLow, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.DCBLow, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.DCBLow, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.DCBLow, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.DCBLow, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.DCBLow, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.DCBLow, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.DCBLow, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.DCBLow, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.DCBLow, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.DCBLow, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.DCBLow, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.DCBLow, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCOutput1BConditionEnum(obj)
            if isequal(obj.DCOutput1BCondition, 'Event is disabled')
                ret = uint16(0);
            elseif isequal(obj.DCOutput1BCondition, 'Event when DCxH low')
                ret = uint16(1);
            elseif isequal(obj.DCOutput1BCondition, 'Event when DCxH high')
                ret = uint16(2);
            elseif isequal(obj.DCOutput1BCondition, 'Event when DCxL low')
                ret = uint16(3);
            elseif isequal(obj.DCOutput1BCondition, 'Event when DCxL high')
                ret = uint16(4);
            elseif isequal(obj.DCOutput1BCondition, 'Event when DCxL high DCxH low')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.DCOutput2BConditionEnum(obj)
            if isequal(obj.DCOutput2BCondition, 'Event is disabled')
                ret = uint16(0);
            elseif isequal(obj.DCOutput2BCondition, 'Event when DCxH low')
                ret = uint16(1);
            elseif isequal(obj.DCOutput2BCondition, 'Event when DCxH high')
                ret = uint16(2);
            elseif isequal(obj.DCOutput2BCondition, 'Event when DCxL low')
                ret = uint16(3);
            elseif isequal(obj.DCOutput2BCondition, 'Event when DCxL high')
                ret = uint16(4);
            elseif isequal(obj.DCOutput2BCondition, 'Event when DCxL high DCxH low')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SynchModeDCBEVT1Enum(obj)
            if isequal(obj.SynchModeDCBEVT1, 'DC input signal is synced with TBCLK')
                ret = uint16(0);
            elseif isequal(obj.SynchModeDCBEVT1, 'DC input signal is not synced with TBCLK')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SignalSourceDCBEVT1Enum(obj)
            if isequal(obj.SignalSourceDCBEVT1, 'Signal source is unfiltered')
                ret = uint16(0);
            elseif isequal(obj.SignalSourceDCBEVT1, 'Signal source is filtered')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CBCLatchModeDCBEVT1Enum(obj)
            if isequal(obj.CBCLatchModeDCBEVT1, 'DC CBC latch is disabled')
                ret = 0x0;
            elseif isequal(obj.CBCLatchModeDCBEVT1, 'DC CBC latch is enabled')
                ret = 0x1;
            else
                ret = 0x0;
            end
        end

        function ret = get.CBCLatchClearEventDCBEVT1Enum(obj)
            if isequal(obj.CBCLatchClearEventDCBEVT1, 'Clear CBC latch when counter equals zero')
                ret = 0x0;
            elseif isequal(obj.CBCLatchClearEventDCBEVT1, 'Clear CBC latch when counter equals period')
                ret = 0x1;
            elseif isequal(obj.CBCLatchClearEventDCBEVT1, 'Clear CBC latch when counter equals zero or period')
                ret = 0x2;
            else
                ret = 0x0;
            end
        end

        function ret = get.SynchModeDCBEVT2Enum(obj)
            if isequal(obj.SynchModeDCBEVT2, 'DC input signal is synced with TBCLK')
                ret = uint16(0);
            elseif isequal(obj.SynchModeDCBEVT2, 'DC input signal is not synced with TBCLK')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SignalSourceDCBEVT2Enum(obj)
            if isequal(obj.SignalSourceDCBEVT2, 'Signal source is unfiltered')
                ret = uint16(0);
            elseif isequal(obj.SignalSourceDCBEVT2, 'Signal source is filtered')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.CBCLatchModeDCBEVT2Enum(obj)
            if isequal(obj.CBCLatchModeDCBEVT2, 'DC CBC latch is disabled')
                ret = 0x0;
            elseif isequal(obj.CBCLatchModeDCBEVT2, 'DC CBC latch is enabled')
                ret = 0x1;
            else
                ret = 0x0;
            end
        end

        function ret = get.CBCLatchClearEventDCBEVT2Enum(obj)
            if isequal(obj.CBCLatchClearEventDCBEVT2, 'Clear CBC latch when counter equals zero')
                ret = 0x0;
            elseif isequal(obj.CBCLatchClearEventDCBEVT2, 'Clear CBC latch when counter equals period')
                ret = 0x1;
            elseif isequal(obj.CBCLatchClearEventDCBEVT2, 'Clear CBC latch when counter equals zero or period')
                ret = 0x2;
            else
                ret = 0x0;
            end
        end

        function ret = get.DCFilterInputEventSourceEnum(obj)
            if isequal(obj.DCFilterInputEventSource, 'DC filter signal source is DCAEVT1')
                ret = uint16(0);
            elseif isequal(obj.DCFilterInputEventSource, 'DC filter signal source is DCAEVT2')
                ret = uint16(1);
            elseif isequal(obj.DCFilterInputEventSource, 'DC filter signal source is DCBEVT1')
                ret = uint16(2);
            elseif isequal(obj.DCFilterInputEventSource, 'DC filter signal source is DCBEVT2')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.BlankingWindowStartEventEnum(obj)
            if isequal(obj.BlankingWindowStartEvent, 'Time base counter equals period')
                ret = uint16(0);
            elseif isequal(obj.BlankingWindowStartEvent, 'Time base counter equals zero')
                ret = uint16(1);
            elseif isequal(obj.BlankingWindowStartEvent, 'Time base counter equals zero or period')
                ret = uint16(2);
            elseif isequal(obj.BlankingWindowStartEvent, 'Time base counter equals mixed event')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        
        function ret = get.EdgeFilterModeEnum(obj)
            if isequal(obj.EdgeFilterMode, 'Digital Compare Edge Filter low')
                ret = uint16(0);
            elseif isequal(obj.EdgeFilterMode, 'Digital Compare Edge Filter high')
                ret = uint16(1);
            elseif isequal(obj.EdgeFilterMode, 'Digital Compare Edge Filter both')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EdgeFilterEdgeCountEnum(obj)
            if isequal(obj.EdgeFilterEdgeCount, 'No edge is required to generate event')
                ret = uint16(0);
            elseif isequal(obj.EdgeFilterEdgeCount, '1 edge is required for event generation')
                ret = uint16(1);
            elseif isequal(obj.EdgeFilterEdgeCount, '2 edges are required for event generation')
                ret = uint16(2);
            elseif isequal(obj.EdgeFilterEdgeCount, '3 edges are required for event generation')
                ret = uint16(3);
            elseif isequal(obj.EdgeFilterEdgeCount, '4 edges are required for event generation')
                ret = uint16(4);
            elseif isequal(obj.EdgeFilterEdgeCount, '5 edges are required for event generation')
                ret = uint16(5);
            elseif isequal(obj.EdgeFilterEdgeCount, '6 edges are required for event generation')
                ret = uint16(6);
            elseif isequal(obj.EdgeFilterEdgeCount, '7 edges are required for event generation')
                ret = uint16(7);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EdgeFilterCounterCaptureSignalEnum(obj)
            if isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by software')
                ret = uint16(0);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by when counter is equal to zero')
                ret = uint16(1);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by when counter is equal to period')
                ret = uint16(2);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by when counter is equal to zero or period')
                ret = uint16(3);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by DCAEVT1')
                ret = uint16(4);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by DCAEVT2')
                ret = uint16(5);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by DCBEVT1')
                ret = uint16(6);
            elseif isequal(obj.EdgeFilterCounterCaptureSignal, 'Valley capture triggered by DCBEVT2')
                ret = uint16(7);
            else
                ret = uint16(0);
            end
        end

        function ret = get.StartValleyCaptureEnum(obj)
            if isequal(obj.StartValleyCapture, '0 Event')
                ret = uint16(0);
            elseif isequal(obj.StartValleyCapture, '1 Events')
                ret = uint16(1);
            elseif isequal(obj.StartValleyCapture, '2 Events')
                ret = uint16(2);
            elseif isequal(obj.StartValleyCapture, '3 Events')
                ret = uint16(3);
            elseif isequal(obj.StartValleyCapture, '4 Events')
                ret = uint16(4);
            elseif isequal(obj.StartValleyCapture, '5 Events')
                ret = uint16(5);
            elseif isequal(obj.StartValleyCapture, '6 Events')
                ret = uint16(6);
            elseif isequal(obj.StartValleyCapture, '7 Events')
                ret = uint16(7);
            elseif isequal(obj.StartValleyCapture, '8 Events')
                ret = uint16(8);
            elseif isequal(obj.StartValleyCapture, '9 Events')
                ret = uint16(9);
            elseif isequal(obj.StartValleyCapture, '10 Events')
                ret = uint16(10);
            elseif isequal(obj.StartValleyCapture, '11 Events')
                ret = uint16(11);
            elseif isequal(obj.StartValleyCapture, '12 Events')
                ret = uint16(12);
            elseif isequal(obj.StartValleyCapture, '13 Events')
                ret = uint16(13);
            elseif isequal(obj.StartValleyCapture, '14 Events')
                ret = uint16(14);
            elseif isequal(obj.StartValleyCapture, '15 Events')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.StopValleyCaptureEnum(obj)
            if isequal(obj.StopValleyCapture, '0 Event')
                ret = uint16(0);
            elseif isequal(obj.StopValleyCapture, '1 Events')
                ret = uint16(1);
            elseif isequal(obj.StopValleyCapture, '2 Events')
                ret = uint16(2);
            elseif isequal(obj.StopValleyCapture, '3 Events')
                ret = uint16(3);
            elseif isequal(obj.StopValleyCapture, '4 Events')
                ret = uint16(4);
            elseif isequal(obj.StopValleyCapture, '5 Events')
                ret = uint16(5);
            elseif isequal(obj.StopValleyCapture, '6 Events')
                ret = uint16(6);
            elseif isequal(obj.StopValleyCapture, '7 Events')
                ret = uint16(7);
            elseif isequal(obj.StopValleyCapture, '8 Events')
                ret = uint16(8);
            elseif isequal(obj.StopValleyCapture, '9 Events')
                ret = uint16(9);
            elseif isequal(obj.StopValleyCapture, '10 Events')
                ret = uint16(10);
            elseif isequal(obj.StopValleyCapture, '11 Events')
                ret = uint16(11);
            elseif isequal(obj.StopValleyCapture, '12 Events')
                ret = uint16(12);
            elseif isequal(obj.StopValleyCapture, '13 Events')
                ret = uint16(13);
            elseif isequal(obj.StopValleyCapture, '14 Events')
                ret = uint16(14);
            elseif isequal(obj.StopValleyCapture, '15 Events')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ValleyDelayDividerEnum(obj)
            if isequal(obj.ValleyDelayDivider, 'Delay value equals offset value defined by software')
                ret = uint16(0);
            elseif isequal(obj.ValleyDelayDivider, 'Delay value equals sum of hardware counter value and the offset value defined by software')
                ret = uint16(1);
            elseif isequal(obj.ValleyDelayDivider, 'Delay value equals sum of hardware counter value shifted by 1 and the offset value defined by software')
                ret = uint16(2);
            elseif isequal(obj.ValleyDelayDivider, 'Delay value equals sum of hardware counter value shifted by 2 and the offset value defined by software')
                ret = uint16(3);
            elseif isequall(obj.ValleyDelayDivider, 'Delay value equals sum of hardware counter value shifted by 4 and the offset value defined by software')
                ret = uint16(4);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Capture_InputEnum(obj)
            if isequal(obj.Capture_Input, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.Capture_Input, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.Capture_Input, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.Capture_Input, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.Capture_Input, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.Capture_Input, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.Capture_Input, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.Capture_Input, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.Capture_Input, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.Capture_Input, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.Capture_Input, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.Capture_Input, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.Capture_Input, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.Capture_Input, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.Capture_Input, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.Capture_Input, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Capture_gateEnum(obj)
            if isequal(obj.Capture_gate, 'Trip 1')
                ret = uint16(0);
            elseif isequal(obj.Capture_gate, 'Trip 2')
                ret = uint16(1);
            elseif isequal(obj.Capture_gate, 'Trip 3')
                ret = uint16(2);
            elseif isequal(obj.Capture_gate, 'Trip 4')
                ret = uint16(3);
            elseif isequal(obj.Capture_gate, 'Trip 5')
                ret = uint16(4);
            elseif isequal(obj.Capture_gate, 'Trip 6')
                ret = uint16(5);
            elseif isequal(obj.Capture_gate, 'Trip 7')
                ret = uint16(6);
            elseif isequal(obj.Capture_gate, 'Trip 8')
                ret = uint16(7);
            elseif isequal(obj.Capture_gate, 'Trip 9')
                ret = uint16(8);
            elseif isequal(obj.Capture_gate, 'Trip 10')
                ret = uint16(9);
            elseif isequal(obj.Capture_gate, 'Trip 11')
                ret = uint16(10);
            elseif isequal(obj.Capture_gate, 'Trip 12')
                ret = uint16(11);
            elseif isequal(obj.Capture_gate, 'Trip 13')
                ret = uint16(12);
            elseif isequal(obj.Capture_gate, 'Trip 14')
                ret = uint16(13);
            elseif isequal(obj.Capture_gate, 'Trip 15')
                ret = uint16(14);
            elseif isequal(obj.Capture_gate, 'All Trips')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Invert_Capture_Input_PolarityEnum(obj)
            if isequal(obj.Invert_Capture_Input_Polarity, 'Capture input is not inverted')
                ret = uint16(0);
            elseif isequal(obj.Invert_Capture_Input_Polarity, 'Capture input is inverted')
                ret = uint16(1);
            end
        end

        function ret = get.Capture_gate_polarity_selectEnum(obj)
            if isequal(obj.Capture_gate_polarity_select, 'Capture gate is always on')
                ret = uint16(0);
            elseif isequal(obj.Capture_gate_polarity_select, 'Capture gate is always off')
                ret = uint16(1);
            elseif isequal(obj.Capture_gate_polarity_select, 'Capture gate input is CAPGATE.sync')
                ret = uint16(2);
            elseif isequal(obj.Capture_gate_polarity_select, 'Capture gate input is CAPGATE.sync inverted')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.RisingEdgeDelayInputEnum(obj)
            if isequal(obj.RisingEdgeDelayInput, 'Input signal is ePWMA')
                ret = uint16(0);
            elseif isequal(obj.RisingEdgeDelayInput, 'Input signal is ePWMB')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.FallingEdgeDelayInputEnum(obj)
            if isequal(obj.FallingEdgeDelayInput, 'Input signal is ePWMA')
                ret = uint16(0);
            elseif isequal(obj.FallingEdgeDelayInput, 'Input signal is ePWMB')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.RisingEdgeDelayPolarityEnum(obj)
            if isequal(obj.RisingEdgeDelayPolarity, 'DB polarity is not inverted')
                ret = uint16(0);
            elseif isequal(obj.RisingEdgeDelayPolarity, 'DB polarity is inverted')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.FallingEdgeDelayPolarityEnum(obj)
            if isequal(obj.FallingEdgeDelayPolarity, 'DB polarity is not inverted')
                ret = uint16(0);
            elseif isequal(obj.FallingEdgeDelayPolarity, 'DB polarity is inverted')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end


       function ret = get.DeadbandGlobalLoadEnum(obj)
            if isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = uint16(0);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = uint16(1);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = uint16(2);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_CMPC')
                ret = uint16(3);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_CMPD')
                ret = uint16(4);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = uint16(5);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = uint16(6);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_DBCTL')
                ret = uint16(7);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = uint16(8);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = uint16(9);
            elseif isequal(obj.DeadbandGlobalLoad, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = uint16(10);
            else
                ret = uint16(0);
            end
       end

       function ret = get.DeadbandControlShadowModeEnum(obj)
           if isequal(obj.DeadbandControlShadowMode, 'Load when counter equals zero')
               ret = uint16(0);
           elseif isequal(obj.DeadbandControlShadowMode, 'Load when counter equals period')
               ret = uint16(1);
           elseif isequal(obj.DeadbandControlShadowMode, 'Load when counter equals zero or period')
               ret = uint16(2);
           elseif isequal(obj.DeadbandControlShadowMode, 'Freeze shadow to active load')
               ret = uint16(3);
           else
               ret = uint16(0);
           end
       end

       function ret = get.REDGlobalLoadRegisterEnum(obj)
            if isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = uint16(0);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = uint16(1);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = uint16(2);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPC')
                ret = uint16(3);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPD')
                ret = uint16(4);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = uint16(5);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = uint16(6);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBCTL')
                ret = uint16(7);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = uint16(8);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = uint16(9);
            elseif isequal(obj.REDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = uint16(10);
            else
                ret = uint16(0);
            end
       end

       function ret = get.REDShadowLoadEventEnum(obj)
           if isequal(obj.REDShadowLoadEvent, 'Load when counter equals zero')
               ret = uint16(0);
           elseif isequal(obj.REDShadowLoadEvent, 'Load when counter equals period')
               ret = uint16(1);
           elseif isequal(obj.REDShadowLoadEvent, 'Load when counter equals zero or period')
               ret = uint16(2);
           elseif isequal(obj.REDShadowLoadEvent, 'Freeze shadow to active load')
               ret = uint16(3);
           else
               ret = uint16(0);
           end
       end

       function ret = get.FEDGlobalLoadRegisterEnum(obj)
            if isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_TBPRD_TBPRDHR')
                ret = uint16(0);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPA_CMPAHR')
                ret = uint16(1);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPB_CMPBHR')
                ret = uint16(2);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPC')
                ret = uint16(3);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_CMPD')
                ret = uint16(4);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBRED_DBREDHR')
                ret = uint16(5);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBFED_DBFEDHR')
                ret = uint16(6);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_DBCTL')
                ret = uint16(7);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLA_AQCTLA2')
                ret = uint16(8);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCTLB_AQCTLB2')
                ret = uint16(9);
            elseif isequal(obj.FEDGlobalLoadRegister, 'EPWM_GL_REGISTER_AQCSFRC')
                ret = uint16(10);
            else
                ret = uint16(0);
            end
       end

       function ret = get.FEDShadowLoadEventEnum(obj)
           if isequal(obj.FEDShadowLoadEvent, 'Load when counter equals zero')
               ret = uint16(0);
           elseif isequal(obj.FEDShadowLoadEvent, 'Load when counter equals period')
               ret = uint16(1);
           elseif isequal(obj.FEDShadowLoadEvent, 'Load when counter equals zero or period')
               ret = uint16(2);
           elseif isequal(obj.FEDShadowLoadEvent, 'Freeze shadow to active load')
               ret = uint16(3);
           else
               ret = uint16(0);
           end
       end

       function ret = get.DeadBandCounterClockRateEnum(obj)
           if isequal(obj.DeadBandCounterClockRate, 'Deadband counter runs at TBCLK rate')
               ret = uint16(0);
           elseif isequal(obj.DeadBandCounterClockRate, 'Deadband counter runs at 2*TBCLK rate')
               ret = uint16(1);
           else
               ret = uint16(0);
           end
       end

       function ret = get.ChopperDutyCycleEnum(obj)
           if isequal(obj.ChopperDutyCycle, '1/8 Duty')
               ret = uint16(0);
           elseif isequal(obj.ChopperDutyCycle, '2/8 Duty')
               ret = uint16(1);
           elseif isequal(obj.ChopperDutyCycle, '3/8 Duty')
               ret = uint16(2);
           elseif isequal(obj.ChopperDutyCycle, '4/8 Duty')
               ret = uint16(3);
           elseif isequal(obj.ChopperDutyCycle, '5/8 Duty')
               ret = uint16(4);
           elseif isequal(obj.ChopperDutyCycle, '6/8 Duty')
               ret = uint16(5);
           elseif isequal(obj.ChopperDutyCycle, '7/8 Duty')
               ret = uint16(6);
           else
               ret = uint16(0);
           end
       end

       function ret = get.ChopperFrequencyEnum(obj)
           if isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/1')
               ret = uint16(0);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/2')
               ret = uint16(1);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/3')
               ret = uint16(2);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/4')
               ret = uint16(3);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/5')
               ret = uint16(4);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/6')
               ret = uint16(5);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/7')
               ret = uint16(6);
           elseif isequal(obj.ChopperFrequency, 'SYSCLOCKOUT/8')
               ret = uint16(7);
           else
               ret = uint16(0);
           end
       end

       function ret = get.ChopperFirstPulseWidthEnum(obj)
           if isequal(obj.ChopperFirstPulseWidth, '1')
               ret = uint16(0);
           elseif isequal(obj.ChopperFirstPulseWidth, '2')
               ret = uint16(1);
           elseif isequal(obj.ChopperFirstPulseWidth, '3')
               ret = uint16(2);
           elseif isequal(obj.ChopperFirstPulseWidth, '4')
               ret = uint16(3);
           elseif isequal(obj.ChopperFirstPulseWidth, '5')
               ret = uint16(4);
           elseif isequal(obj.ChopperFirstPulseWidth, '6')
               ret = uint16(5);
           elseif isequal(obj.ChopperFirstPulseWidth, '7')
               ret = uint16(6);
           elseif isequal(obj.ChopperFirstPulseWidth, '8')
               ret = uint16(7);
           elseif isequal(obj.ChopperFirstPulseWidth, '9')
               ret = uint16(8);
           elseif isequal(obj.ChopperFirstPulseWidth, '10')
               ret = uint16(9);
           elseif isequal(obj.ChopperFirstPulseWidth, '11')
               ret = uint16(10);
           elseif isequal(obj.ChopperFirstPulseWidth, '12')
               ret = uint16(11);
           elseif isequal(obj.ChopperFirstPulseWidth, '13')
               ret = uint16(12);
           elseif isequal(obj.ChopperFirstPulseWidth, '14')
               ret = uint16(13);
           elseif isequal(obj.ChopperFirstPulseWidth, '15')
               ret = uint16(14);
           elseif isequal(obj.ChopperFirstPulseWidth, '16')
               ret = uint16(15);
           else
               ret = uint16(0);
           end
       end


       function ret = get.InterruptEventSourcesEnum(obj)
           if isequal(obj.InterruptEventSources, 'Time base counter equal to zero')
               ret = uint16(1);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to period')
               ret = uint16(2);
           elseif isequal(obj.InterruptEventSources, 'Time base counter based on mixed events')
               ret = uint16(3);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPA when timer is incrementing')
               ret = uint16(4);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPC when timer is incrementing')
               ret = uint16(8);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPA when timer is decrementing')
               ret = uint16(5);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPC when timer is decrementing')
               ret = uint16(10);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPB when timer is incrementing')
               ret = uint16(6);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPD when timer is incrementing')
               ret = uint16(12);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPB when timer is decrementing')
               ret = uint16(7);
           elseif isequal(obj.InterruptEventSources, 'Time base counter equal to CMPD when timer is decrementing')
               ret = uint16(14);
           else
               ret = uint16(1);
           end
       end

       function ret = get.InterruptEventCountEnum(obj)
           if isequal(obj.InterruptEventCount, 'Disabled')
               ret = uint16(0);
           elseif isequal(obj.InterruptEventCount, '1 event generates interrupt')
               ret = uint16(1);
           elseif isequal(obj.InterruptEventCount, '2 events generates interrupt')
               ret = uint16(2);
           elseif isequal(obj.InterruptEventCount, '3 events generates interrupt')
               ret = uint16(3);
           elseif isequal(obj.InterruptEventCount, '4 event generates interrupt')
               ret = uint16(4);
           elseif isequal(obj.InterruptEventCount, '5 events generates interrupt')
               ret = uint16(5);
           elseif isequal(obj.InterruptEventCount, '6 events generates interrupt')
               ret = uint16(6);
           elseif isequal(obj.InterruptEventCount, '7 event generates interrupt')
               ret = uint16(7);
           elseif isequal(obj.InterruptEventCount, '8 events generates interrupt')
               ret = uint16(8);
           elseif isequal(obj.InterruptEventCount, '9 events generates interrupt')
               ret = uint16(9);
           elseif isequal(obj.InterruptEventCount, '10 event generates interrupt')
               ret = uint16(10);
           elseif isequal(obj.InterruptEventCount, '11 events generates interrupt')
               ret = uint16(11);
           elseif isequal(obj.InterruptEventCount, '12 events generates interrupt')
               ret = uint16(12);
           elseif isequal(obj.InterruptEventCount, '13 event generates interrupt')
               ret = uint16(13);
           elseif isequal(obj.InterruptEventCount, '14 events generates interrupt')
               ret = uint16(14);
           elseif isequal(obj.InterruptEventCount, '15 events generates interrupt')
               ret = uint16(15);
           else
               ret = uint16(0);
           end
       end

       function ret = get.InterruptEventCountInitialValueEnum(obj)
           if isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 0')
               ret = uint16(0);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 1')
               ret = uint16(1);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 2')
               ret = uint16(2);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 3')
               ret = uint16(3);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 4')
               ret = uint16(4);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 5')
               ret = uint16(5);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 6')
               ret = uint16(6);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 7')
               ret = uint16(7);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 8')
               ret = uint16(8);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 9')
               ret = uint16(9);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 10')
               ret = uint16(10);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 11')
               ret = uint16(11);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 12')
               ret = uint16(12);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 13')
               ret = uint16(13);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 14')
               ret = uint16(14);
           elseif isequal(obj.InterruptEventCountInitialValue, 'Initialize event counter to 15')
               ret = uint16(15);
           else
               ret = uint16(0);
           end
       end


      function ret = get.SOCATriggerSourceEnum(obj)
           if isequal(obj.SOCATriggerSource, 'Event is based on DCxEVT1')
               ret = uint16(0);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to zero')
               ret = uint16(1);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to period')
               ret = uint16(2);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to mixed event')
               ret = uint16(3);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPA when timer is incrementing')
               ret = uint16(4);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPC when timer is incrementing')
               ret = uint16(5);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPA when timer is decrementing')
               ret = uint16(6);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPC when timer is decrementing')
               ret = uint16(7);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPB when timer is incrementing')
               ret = uint16(8);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPD when timer is incrementing')
               ret = uint16(9);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPB when timer is decrementing')
               ret = uint16(10);
           elseif isequal(obj.SOCATriggerSource, 'Time base counter equal to CMPD when timer is decrementing')
               ret = uint16(11);
           else
               ret = uint16(0);
           end
      end



      function ret = get.SOCATriggerEventCountEnum(obj)
          if isequal(obj.SOCATriggerEventCount, 'Disabled')
               ret = uint16(0);
           elseif isequal(obj.SOCATriggerEventCount, '1 event generates interrupt')
               ret = uint16(1);
           elseif isequal(obj.SOCATriggerEventCount, '2 events generates interrupt')
               ret = uint16(2);
           elseif isequal(obj.SOCATriggerEventCount, '3 events generates interrupt')
               ret = uint16(3);
           elseif isequal(obj.SOCATriggerEventCount, '4 event generates interrupt')
               ret = uint16(4);
           elseif isequal(obj.SOCATriggerEventCount, '5 events generates interrupt')
               ret = uint16(5);
           elseif isequal(obj.SOCATriggerEventCount, '6 events generates interrupt')
               ret = uint16(6);
           elseif isequal(obj.SOCATriggerEventCount, '7 event generates interrupt')
               ret = uint16(7);
           elseif isequal(obj.SOCATriggerEventCount, '8 events generates interrupt')
               ret = uint16(8);
           elseif isequal(obj.SOCATriggerEventCount, '9 events generates interrupt')
               ret = uint16(9);
           elseif isequal(obj.SOCATriggerEventCount, '10 event generates interrupt')
               ret = uint16(10);
           elseif isequal(obj.SOCATriggerEventCount, '11 events generates interrupt')
               ret = uint16(11);
           elseif isequal(obj.SOCATriggerEventCount, '12 events generates interrupt')
               ret = uint16(12);
           elseif isequal(obj.SOCATriggerEventCount, '13 event generates interrupt')
               ret = uint16(13);
           elseif isequal(obj.SOCATriggerEventCount, '14 events generates interrupt')
               ret = uint16(14);
           elseif isequal(obj.SOCATriggerEventCount, '15 events generates interrupt')
               ret = uint16(15);
           else
               ret = uint16(0);
           end
      end

      function ret = get.SOCATriggerEventCountInitValueEnum(obj)
          if isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 0')
              ret = uint16(0);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 1')
              ret = uint16(1);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 2')
              ret = uint16(2);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 3')
              ret = uint16(3);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 4')
              ret = uint16(4);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 5')
              ret = uint16(5);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 6')
              ret = uint16(6);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 7')
              ret = uint16(7);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 8')
              ret = uint16(8);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 9')
              ret = uint16(9);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 10')
              ret = uint16(10);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 11')
              ret = uint16(11);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 12')
              ret = uint16(12);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 13')
              ret = uint16(13);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 14')
              ret = uint16(14);
          elseif isequal(obj.SOCATriggerEventCountInitValue, 'Initialize event counter to 15')
              ret = uint16(15);
          else
              ret = uint16(0);
          end
      end
      
     function ret = get.SOCBTriggerSourceEnum(obj)
           if isequal(obj.SOCBTriggerSource, 'Event is based on DCxEVT1')
               ret = uint16(0);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to zero')
               ret = uint16(1);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to period')
               ret = uint16(2);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to mixed event')
               ret = uint16(3);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPA when timer is incrementing')
               ret = uint16(4);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPC when timer is incrementing')
               ret = uint16(5);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPA when timer is decrementing')
               ret = uint16(6);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPC when timer is decrementing')
               ret = uint16(7);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPB when timer is incrementing')
               ret = uint16(8);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPD when timer is incrementing')
               ret = uint16(9);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPB when timer is decrementing')
               ret = uint16(10);
           elseif isequal(obj.SOCBTriggerSource, 'Time base counter equal to CMPD when timer is decrementing')
               ret = uint16(11);
           else
               ret = uint16(0);
           end
      end



      function ret = get.SOCBTriggerEventCountEnum(obj)
          if isequal(obj.SOCBTriggerEventCount, 'Disabled')
               ret = uint16(0);
           elseif isequal(obj.SOCBTriggerEventCount, '1 event generates interrupt')
               ret = uint16(1);
           elseif isequal(obj.SOCBTriggerEventCount, '2 events generates interrupt')
               ret = uint16(2);
           elseif isequal(obj.SOCBTriggerEventCount, '3 events generates interrupt')
               ret = uint16(3);
           elseif isequal(obj.SOCBTriggerEventCount, '4 event generates interrupt')
               ret = uint16(4);
           elseif isequal(obj.SOCBTriggerEventCount, '5 events generates interrupt')
               ret = uint16(5);
           elseif isequal(obj.SOCBTriggerEventCount, '6 events generates interrupt')
               ret = uint16(6);
           elseif isequal(obj.SOCBTriggerEventCount, '7 event generates interrupt')
               ret = uint16(7);
           elseif isequal(obj.SOCBTriggerEventCount, '8 events generates interrupt')
               ret = uint16(8);
           elseif isequal(obj.SOCBTriggerEventCount, '9 events generates interrupt')
               ret = uint16(9);
           elseif isequal(obj.SOCBTriggerEventCount, '10 event generates interrupt')
               ret = uint16(10);
           elseif isequal(obj.SOCBTriggerEventCount, '11 events generates interrupt')
               ret = uint16(11);
           elseif isequal(obj.SOCBTriggerEventCount, '12 events generates interrupt')
               ret = uint16(12);
           elseif isequal(obj.SOCBTriggerEventCount, '13 event generates interrupt')
               ret = uint16(13);
           elseif isequal(obj.SOCBTriggerEventCount, '14 events generates interrupt')
               ret = uint16(14);
           elseif isequal(obj.SOCBTriggerEventCount, '15 events generates interrupt')
               ret = uint16(15);
           else
               ret = uint16(0);
           end
      end

      function ret = get.SOCBTriggerEventCountInitValueEnum(obj)
          if isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 0')
              ret = uint16(0);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 1')
              ret = uint16(1);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 2')
              ret = uint16(2);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 3')
              ret = uint16(3);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 4')
              ret = uint16(4);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 5')
              ret = uint16(5);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 6')
              ret = uint16(6);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 7')
              ret = uint16(7);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 8')
              ret = uint16(8);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 9')
              ret = uint16(9);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 10')
              ret = uint16(10);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 11')
              ret = uint16(11);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 12')
              ret = uint16(12);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 13')
              ret = uint16(13);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 14')
              ret = uint16(14);
          elseif isequal(obj.SOCBTriggerEventCountInitValue, 'Initialize event counter to 15')
              ret = uint16(15);
          else
              ret = uint16(0);
          end
      end


       function ret = get.ControlModeChannelAEnum(obj)
            if isequal(obj.ControlModeChannelA, 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge')
                ret = uint16(0);
            elseif isequal(obj.ControlModeChannelA, 'TBPHSR controls MEP edge')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
       end


        function ret = get.ControlModeChannelBEnum(obj)
            if isequal(obj.ControlModeChannelB, 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge')
                ret = uint16(0);
            elseif isequal(obj.ControlModeChannelB, 'TBPHSR controls MEP edge')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.HRPWMSyncSourceEnum(obj)
            if isequal(obj.HRPWMSyncSource, 'Counter equals period')
                ret = uint16(0);
            elseif isequal(obj.HRPWMSyncSource, 'Counter equals zero')
                ret = uint16(1);
            elseif isequal(obj.HRPWMSyncSource, 'Counter equals COMPC when counting up')
                ret = uint16(4);
            elseif isequal(obj.HRPWMSyncSource, 'Counter equals COMPC when counting down')
                ret = uint16(5);
            elseif isequal(obj.HRPWMSyncSource, 'Counter equals COMPD when counting up')
                ret = uint16(6);
            elseif isequal(obj.HRPWMSyncSource, 'Counter equals COMPD when counting down')
                ret = uint16(7);
           else
                ret = uint16(0);
            end
        end

        function ret = get.HRPWMChannelAEdgeModeEnum(obj)
            if isequal(obj.HRPWMChannelAEdgeMode, 'HRPWM is disabled')
                ret = uint16(0);
            elseif isequal(obj.HRPWMChannelAEdgeMode, 'MEP controls rising edge')
                ret = uint16(1);
            elseif isequal(obj.HRPWMChannelAEdgeMode, 'MEP controls falling edge')
                ret = uint16(2);
            elseif isequal(obj.HRPWMChannelAEdgeMode, 'MEP controls both rising and falling edge')
                ret = uint16(3);
           else
                ret = uint16(1);
            end
        end

        function ret = get.CMPAHRShadowToActiveLoadEventEnum(obj)
            if isequal(obj.CMPAHRShadowToActiveLoadEvent, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.CMPAHRShadowToActiveLoadEvent, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.CMPAHRShadowToActiveLoadEvent, 'Load when counter equals zero or period')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.HRPWMChannelBEdgeModeEnum(obj)
            if isequal(obj.HRPWMChannelBEdgeMode, 'HRPWM is disabled')
                ret = uint16(0);
            elseif isequal(obj.HRPWMChannelBEdgeMode, 'MEP controls rising edge')
                ret = uint16(1);
            elseif isequal(obj.HRPWMChannelBEdgeMode, 'MEP controls falling edge')
                ret = uint16(2);
            elseif isequal(obj.HRPWMChannelBEdgeMode, 'MEP controls both rising and falling edge')
                ret = uint16(3);
           else
                ret = uint16(1);
            end
        end

        function ret = get.CMPBHRShadowToActiveLoadEventEnum(obj)
            if isequal(obj.CMPBHRShadowToActiveLoadEvent, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.CMPBHRShadowToActiveLoadEvent, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.CMPBHRShadowToActiveLoadEvent, 'Load when counter equals zero or period')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.HRPWMDeadbandEdgeModeEnum(obj)
            if isequal(obj.HRPWMDeadbandEdgeMode, 'HRPWM is disabled')
                ret = uint16(0);
            elseif isequal(obj.HRPWMDeadbandEdgeMode, 'MEP controls rising edge delay')
                ret = uint16(1);
            elseif isequal(obj.HRPWMDeadbandEdgeMode, 'MEP controls falling edge delay')
                ret = uint16(2);
            elseif isequal(obj.HRPWMDeadbandEdgeMode, 'MEP controls both falling and rising edge delay')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ShadowActiveLoadDBREDHREnum(obj)
            if isequal(obj.ShadowActiveLoadDBREDHR, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.ShadowActiveLoadDBREDHR, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.ShadowActiveLoadDBREDHR, 'Load when counter equals zero or period')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ShadowActiveLoadDBFEDHREnum(obj)
            if isequal(obj.ShadowActiveLoadDBFEDHR, 'Load when counter equals zero')
                ret = uint16(0);
            elseif isequal(obj.ShadowActiveLoadDBFEDHR, 'Load when counter equals period')
                ret = uint16(1);
            elseif isequal(obj.ShadowActiveLoadDBFEDHR, 'Load when counter equals zero or period')
                ret = uint16(2);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMxBOutputSelectEnum(obj)
            if isequal(obj.EPWMxBOutputSelect, 'ePWMxB output is normal')
                ret = uint16(0);
            elseif isequal(obj.EPWMxBOutputSelect, 'ePWMxB output is inverted')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Allocate_XCMP_CMPAEnum(obj)
            if isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')
                ret = uint16(0);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA')
                ret = uint16(1);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA')
                ret = uint16(2);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA')
                ret = uint16(3);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA')
                ret = uint16(4);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA')
                ret = uint16(5);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA')
                ret = uint16(6);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA')
                ret = uint16(7);
            elseif isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP8 registers to CMPA')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Allocate_XCMP_CMPBEnum(obj)
            if isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB')
                ret = uint16(5);
            elseif isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB')
                ret = uint16(6);
            elseif isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB')
                ret = uint16(7);
            elseif isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP8 registers to CMPB')
                ret = uint16(8);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP1_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP1_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP2_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP2_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP3_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP3_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP4_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP4_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP5_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP5_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP6_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP6_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP7_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP7_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP8_activeEnum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP8_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

         function ret = get.EPWMXB_tbctr_XCMP5_activeEnum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP5_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP6_activeEnum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP6_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP7_activeEnum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP7_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP8_activeEnum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP8_active, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_active, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_active, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_active, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP1_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP2_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP3_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP4_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP5_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP6_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP7_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP8_SHADOW1Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP5_SHADOW1Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP6_SHADOW1Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP7_SHADOW1Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP8_SHADOW1Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW1, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW1, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW1, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW1, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP1_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP2_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP3_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP4_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP5_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP6_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP7_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP8_SHADOW2Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP5_SHADOW2Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP6_SHADOW2Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP7_SHADOW2Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP8_SHADOW2Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW2, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW2, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW2, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW2, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP1_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP1_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP2_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP2_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP3_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP3_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP4_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP4_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP5_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP5_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP6_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP6_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP7_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP7_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXA_tbctr_XCMP8_SHADOW3Enum(obj)
            if isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXA_tbctr_XCMP8_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP5_SHADOW3Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP5_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP6_SHADOW3Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP6_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP7_SHADOW3Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP7_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.EPWMXB_tbctr_XCMP8_SHADOW3Enum(obj)
            if isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW3, 'No change in output pins')
                ret = uint16(0);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW3, 'Set output pins to low')
                ret = uint16(1);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW3, 'Set output pins to high')
                ret = uint16(2);
            elseif isequal(obj.EPWMXB_tbctr_XCMP8_SHADOW3, 'Toggle output pins')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Load_ModeEnum(obj)
            if isequal(obj.Load_Mode, 'Load Once')
                ret = uint16(0);
            elseif isequal(obj.Load_Mode, 'Load multiple times')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Shadow_LevelEnum(obj)
            if isequal(obj.Shadow_Level, 'Shadow level zero')
                ret = uint16(0);
            elseif isequal(obj.Shadow_Level, 'Shadow level one')
                ret = uint16(1);
            elseif isequal(obj.Shadow_Level, 'Shadow level two')
                ret = uint16(2);
            elseif isequal(obj.Shadow_Level, 'Shadow level three')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

    

    function ret = get.Shadow_register_load_onceEnum(obj)
        if isequal(obj.Shadow_register_load_once, 'Do not load any shadow rtegister set')
            ret = uint16(0);
        elseif isequal(obj.Shadow_register_load_once, 'Load shadow register set 1')
            ret = uint16(1);
        elseif isequal(obj.Shadow_register_load_once, 'Load shadow register set 2')
            ret = uint16(2);
        elseif isequal(obj.Shadow_register_load_once, 'Load shadow register set 3')
            ret = uint16(3);
        else
            ret = uint16(0);
        end
    end
     
    function ret = get.SpecifyXCMPActiveEnum(obj)
        if isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box')
            ret = uint16(0);
        elseif isequal(obj.SpecifyXCMPActive, 'Specify via Input Port')
            ret = uint16(1);
        else
            ret = uint16(0);
        end     
    end

    function ret = get.SpecifyXCMPShadow1Enum(obj)
        if isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box')
            ret = uint16(0);
        elseif isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port')
            ret = uint16(1);
        else
            ret = uint16(0);
        end     
    end

    function ret = get.SpecifyXCMPShadow2Enum(obj)
        if isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box')
            ret = uint16(0);
        elseif isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port')
            ret = uint16(1);
        else
            ret = uint16(0);
        end     
    end

    function ret = get.SpecifyXCMPShadow3Enum(obj)
        if isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box')
            ret = uint16(0);
        elseif isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port')
            ret = uint16(1);
        else
            ret = uint16(0);
        end     
    end
end

     methods (Static, Access=protected)

         function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','Type 5 EPWM',...
                'Text', sprintf('%s\n\n%s','EPWM Block'),...
                'ShowSourceLink', false);
         end
         
        function simMode = getSimulateUsingImpl(~)
            simMode = 'Interpreted execution';
        end
        
        function isVisible = showSimulateUsingImpl
            isVisible = false;
        end
        
        function groups = getPropertyGroupsImpl

          
            TimeBase = matlab.system.display.SectionGroup(...
             'Title', 'Time Base', ...
             "PropertyList", {'ePWMModule','ePWMLink','TimerPeriodUnits','SpecifyTimerPeriod','Timer_Period', 'Timer_Initial_Period',...
             'Enable_TimeBase_Global_Load_Mode', 'PeriodLoadMode', 'PeriodLoadEvent','EmulationMode', 'TBCLK', 'HSPCLKDIV', 'CounterMode', 'CounterInitialValue', 'Phase_Shift_Load_Enable', 'Phase_Shift_Value', 'SyncInPulseSrc','Software_Force_EPWM_SyncOut_Pulse','Counter_Zero_EPWM_SyncOut_Pulse', 'Counter_CMPB_EPWM_SyncOut_Pulse', 'Counter_CMPC_EPWM_SyncOut_Pulse', 'Counter_CMPD_EPWM_SyncOut_Pulse', 'DCA_Event1_Sync_Signal', 'DCB_Event1_Sync_Signal', 'OneShotTrigger'});
            
            CounterCompare = matlab.system.display.SectionGroup(...
              'Title', 'Counter Compare', ...
              "PropertyList", {'Enable_LinkCounterCMPA', 'ePWMLinkCMPA', 'CMPAUnits', 'SpecifyCMPAVia', 'CMPAValue', 'CMPA_Initial_Value', 'Enable_Global_Load_CMPA', 'Enable_Shadow_Load_CMPA', 'SHDWAMODE', 'Enable_LinkCounterCMPB', 'ePWMLinkCMPB', 'CMPBUnits', 'SpecifyCMPBVia', 'CMPBValue', 'CMPB_Initial_Value', 'Enable_Global_Load_CMPB', 'Enable_Shadow_Load_CMPB', 'SHDWBMODE', ...
              'Enable_LinkCounterCMPC', 'ePWMLinkCMPC', 'CMPCUnits', 'SpecifyCMPCVia', 'CMPCValue', 'CMPC_Initial_Value', 'Enable_Global_Load_CMPC', 'Enable_Shadow_Load_CMPC', 'SHDWCMODE', 'Enable_LinkCounterCMPD', 'ePWMLinkCMPD', 'CMPDUnits', 'SpecifyCMPDVia', 'CMPDValue', 'CMPD_Initial_Value', 'Enable_Global_Load_CMPD', 'Enable_Shadow_Load_CMPD', 'SHDWDMODE'});
            
            ActionQualifier = matlab.system.display.SectionGroup(...
                'Title', 'Action Qualifier', ...
                "PropertyList", {'Enable_Software_Force_Global_Load', 'SoftwareForceShadowMode', 'T1TriggerSource', 'T2TriggerSource', 'EnableePWMXAGlobalLoad', 'Enable_ePWMXA_Shadow_Mode', 'ePWMXAShadowLoadEvent', 'ePWMXAOneTimeSWForceAction', 'ePWMXAContinuousSWForceAction', 'ePWMXATbctrZero', 'ePWMXATbctrPrd', 'ePWMXATbctrCMPAUp', 'ePWMXATbctrCMPADown', 'ePWMXATbctrCMPBUp', 'ePWMXATbctrCMPBDown', 'ePWMXAT1Up', 'ePWMXAT1Down', 'ePWMXAT2Up', 'ePWMXAT2Down', ...
                'EnableePWMXBGlobalLoad', 'Enable_ePWMXB_Shadow_Mode', 'ePWMXBShadowLoadEvent', 'ePWMXBOneTimeSWForceAction', 'ePWMXBContinuousSWForceAction', 'ePWMXBTbctrZero', 'ePWMXBTbctrPrd', 'ePWMXBTbctrCMPAUp', 'ePWMXBTbctrCMPADown', 'ePWMXBTbctrCMPBUp', 'ePWMXBTbctrCMPBDown', 'ePWMXBT1Up', 'ePWMXBT1Down', 'ePWMXBT2Up', 'ePWMXBT2Down'});

            TripZone = matlab.system.display.SectionGroup(...
                'Title', 'Trip Zone', ...
                "PropertyList", {'Use_Advanced_EPWM_Trip_Zone_Actions', 'TZAEvent', 'TZBEvent', 'DCAEVT1Event', 'DCAEVT2Event', 'DCBEVT1Event', 'DCBEVT2Event', 'TZBDEvent', 'TZBUEvent', 'TZADEvent', 'TZAUEvent', 'DCAEVT1UEvent', 'DCAEVT1DEvent', 'DCAEVT2UEvent', 'DCAEVT2DEvent', 'DCBEVT1UEvent', 'DCBEVT1DEvent', 'DCBEVT2UEvent', 'DCBEVT2DEvent', 'One_Shot_TZ1', 'One_Shot_TZ2', 'One_Shot_TZ3', 'One_Shot_TZ4', 'One_Shot_TZ5', 'One_Shot_TZ6', 'One_Shot_DCAEVT1', 'One_Shot_DCBEVT1', 'TZ1_Cycle_By_Cycle', 'TZ2_Cycle_By_Cycle', 'TZ3_Cycle_By_Cycle', 'TZ4_Cycle_By_Cycle', 'TZ5_Cycle_By_Cycle', 'TZ6_Cycle_By_Cycle', 'DCAEVT2_Cycle_By_Cycle', 'DCBEVT2_Cycle_By_Cycle', ...
                'CBCLatchClearSignal', 'Trip_Zones_Cycle_By_Cycle_interrupt', 'Trip_Zones_One_Shot_interrupt', 'Digital_CompareA_Event1_interrupt', 'Digital_CompareA_Event2_interrupt', 'Digital_CompareB_Event1_interrupt', 'Digital_CompareB_Event2_interrupt'});
            
            DigitalCompare = matlab.system.display.SectionGroup(...
                'Title', 'Digital Compare', ...
                "PropertyList", {'DCAHigh', 'DCAHigh_CombTrip1_Input', 'DCAHigh_CombTrip2_Input', 'DCAHigh_CombTrip3_Input', 'DCAHigh_CombTrip4_Input', 'DCAHigh_CombTrip5_Input','DCAHigh_CombTrip6_Input', 'DCAHigh_CombTrip7_Input', 'DCAHigh_CombTrip8_Input', 'DCAHigh_CombTrip9_Input', 'DCAHigh_CombTrip10_Input', 'DCAHigh_CombTrip11_Input', 'DCAHigh_CombTrip12_Input', 'DCAHigh_CombTrip13_Input', 'DCAHigh_CombTrip14_Input', 'DCAHigh_CombTrip15_Input', 'DCALow', 'DCALow_CombTrip1_Input', 'DCALow_CombTrip2_Input', 'DCALow_CombTrip3_Input', 'DCALow_CombTrip4_Input', 'DCALow_CombTrip5_Input','DCALow_CombTrip6_Input', 'DCALow_CombTrip7_Input', 'DCALow_CombTrip8_Input', 'DCALow_CombTrip9_Input', 'DCALow_CombTrip10_Input', 'DCALow_CombTrip11_Input', 'DCALow_CombTrip12_Input', 'DCALow_CombTrip13_Input', 'DCALow_CombTrip14_Input', 'DCALow_CombTrip15_Input', 'DCOutput1ACondition', 'DCOutput2ACondition', 'SynchModeDCAEVT1', 'SignalSourceDCAEVT1', 'CBCLatchModeDCAEVT1', 'CBCLatchClearEventDCAEVT1', 'SynchModeDCAEVT2', 'SignalSourceDCAEVT2', 'CBCLatchModeDCAEVT2', 'CBCLatchClearEventDCAEVT2', ...
                'DCBHigh', 'DCBHigh_CombTrip1_Input', 'DCBHigh_CombTrip2_Input', 'DCBHigh_CombTrip3_Input', 'DCBHigh_CombTrip4_Input', 'DCBHigh_CombTrip5_Input','DCBHigh_CombTrip6_Input', 'DCBHigh_CombTrip7_Input', 'DCBHigh_CombTrip8_Input', 'DCBHigh_CombTrip9_Input', 'DCBHigh_CombTrip10_Input', 'DCBHigh_CombTrip11_Input', 'DCBHigh_CombTrip12_Input', 'DCBHigh_CombTrip13_Input', 'DCBHigh_CombTrip14_Input', 'DCBHigh_CombTrip15_Input', 'DCBLow', 'DCBLow_CombTrip1_Input', 'DCBLow_CombTrip2_Input', 'DCBLow_CombTrip3_Input', 'DCBLow_CombTrip4_Input', 'DCBLow_CombTrip5_Input','DCBLow_CombTrip6_Input', 'DCBLow_CombTrip7_Input', 'DCBLow_CombTrip8_Input', 'DCBLow_CombTrip9_Input', 'DCBLow_CombTrip10_Input', 'DCBLow_CombTrip11_Input', 'DCBLow_CombTrip12_Input', 'DCBLow_CombTrip13_Input', 'DCBLow_CombTrip14_Input', 'DCBLow_CombTrip15_Input', 'DCOutput1BCondition', 'DCOutput2BCondition', 'SynchModeDCBEVT1', 'SignalSourceDCBEVT1', 'CBCLatchModeDCBEVT1', 'CBCLatchClearEventDCBEVT1', 'SynchModeDCBEVT2', 'SignalSourceDCBEVT2', 'CBCLatchModeDCBEVT2', 'CBCLatchClearEventDCBEVT2', 'DCFilterInputEventSource', ...
                'Blanking_Window_Usage', 'BlankingWindowStartEvent', 'BlankingWindow_MixedEvent_TBCTRZero', 'BlankingWindow_MixedEvent_TBCTRPrd', 'BlankingWindow_MixedEvent_TBCTRCMPA_Inc', 'BlankingWindow_MixedEvent_TBCTRCMPA_Dec', 'BlankingWindow_MixedEvent_TBCTRCMPB_Inc', 'BlankingWindow_MixedEvent_TBCTRCMPB_Dec', 'BlankingWindow_MixedEvent_TBCTRCMPC_Inc', 'BlankingWindow_MixedEvent_TBCTRCMPC_Dec', 'BlankingWindow_MixedEvent_TBCTRCMPD_Inc', 'BlankingWindow_MixedEvent_TBCTRCMPD_Dec', 'Blanking_Window_Offset', 'Blanking_Window_Length', 'Invert_Blanking_Window', 'Enable_DC_Counter_Capture', 'Enable_Counter_Capture_Shadow_Mode', 'DCCounterCaptureEnableEvent', 'Use_Edge_Filter', 'EdgeFilterMode', 'EdgeFilterEdgeCount', ...
                'Enable_Edge_Filter_Valley_Capture', 'EdgeFilterCounterCaptureSignal', 'Start_Valley_Capture_Logic', 'StartValleyCapture', 'StopValleyCapture', 'Select_Delayed_Output_of_Edge_Filter', 'Software_Valley_Delay_Value', 'ValleyDelayDivider', 'Use_Event_Detection', 'Capture_Input', 'Invert_Capture_Input_Polarity', 'Capture_gate', 'Capture_gate_polarity_select', 'Combinational_capture_input_Trip1', 'Combinational_capture_input_Trip2', 'Combinational_capture_input_Trip3', 'Combinational_capture_input_Trip4', 'Combinational_capture_input_Trip5', 'Combinational_capture_input_Trip6', 'Combinational_capture_input_Trip7', 'Combinational_capture_input_Trip8', 'Combinational_capture_input_Trip9', 'Combinational_capture_input_Trip10', 'Combinational_capture_input_Trip11', 'Combinational_capture_input_Trip12', 'Combinational_capture_input_Trip13', 'Combinational_capture_input_Trip14', 'Combinational_capture_input_Trip15', 'Combinational_capture_gate_Trip1', 'Combinational_capture_gate_Trip2', 'Combinational_capture_gate_Trip3', 'Combinational_capture_gate_Trip4', 'Combinational_capture_gate_Trip5', 'Combinational_capture_gate_Trip6', 'Combinational_capture_gate_Trip7', 'Combinational_capture_gate_Trip8', 'Combinational_capture_gate_Trip9', 'Combinational_capture_gate_Trip10', 'Combinational_capture_gate_Trip11', 'Combinational_capture_gate_Trip12', 'Combinational_capture_gate_Trip13', 'Combinational_capture_gate_Trip14', 'Combinational_capture_gate_Trip15'});

            DeadBand = matlab.system.display.SectionGroup(...
                'Title', 'Dead Band', ...
                "PropertyList", {'RisingEdgeDelayInput', 'FallingEdgeDelayInput', 'RisingEdgeDelayPolarity', 'FallingEdgeDelayPolarity', 'DeadBandCounterClockRate', 'Enable_Rising_Edge_Delay', 'Rising_Edge_Delay_Value', 'Enable_Falling_Edge_Delay', 'Falling_Edge_Delay_Value', 'Swap_Output_for_EPWMxA', 'Swap_Output_for_EPWMxB', 'EnableDeadbandControlShadowMode', 'DeadbandControlShadowMode', 'Enable_Deadband_Control_Global_Load', ...
                'Enable_RED_Shadow_Mode', 'REDShadowLoadEvent', 'Enable_RED_Global_Load', 'Enable_FED_Shadow_Mode', 'FEDShadowLoadEvent', 'Enable_FED_Global_Load'});
            
            Chopper = matlab.system.display.SectionGroup(...
                'Title', 'Chopper', ...
                "PropertyList", {'Use_Chopper', 'ChopperDutyCycle', 'ChopperFrequency', 'ChopperFirstPulseWidth'});

            EventTrigger = matlab.system.display.SectionGroup(...
                'Title', 'Event Trigger', ...
                "PropertyList", {'Enable_EPWM_Interrupt', 'InterruptEventSources', 'EventTrigger_MixedEvent_TBCTRZero',  'EventTrigger_MixedEvent_TBCTRPrd', 'EventTrigger_MixedEvent_TBCTRCMPA_Inc', 'EventTrigger_MixedEvent_TBCTRCMPA_Dec', 'EventTrigger_MixedEvent_TBCTRCMPB_Inc', 'EventTrigger_MixedEvent_TBCTRCMPB_Dec', 'EventTrigger_MixedEvent_TBCTRCMPC_Inc', 'EventTrigger_MixedEvent_TBCTRCMPC_Dec', 'EventTrigger_MixedEvent_TBCTRCMPD_Inc', 'EventTrigger_MixedEvent_TBCTRCMPD_Dec', 'EventTrigger_MixedEvent_DCxEVT1','InterruptEventCount', 'Enable_Interrupt_Event_Count_Initial_Value', 'InterruptEventCountInitialValue', ...
                'SOCA_Trigger_Enable', 'SOCATriggerSource', 'SOCATrigger_MixedEvent_TBCTRZero',  'SOCATrigger_MixedEvent_TBCTRPrd', 'SOCATrigger_MixedEvent_TBCTRCMPA_Inc', 'SOCATrigger_MixedEvent_TBCTRCMPA_Dec', 'SOCATrigger_MixedEvent_TBCTRCMPB_Inc', 'SOCATrigger_MixedEvent_TBCTRCMPB_Dec', 'SOCATrigger_MixedEvent_TBCTRCMPC_Inc', 'SOCATrigger_MixedEvent_TBCTRCMPC_Dec', 'SOCATrigger_MixedEvent_TBCTRCMPD_Inc', 'SOCATrigger_MixedEvent_TBCTRCMPD_Dec', 'SOCATrigger_MixedEvent_DCxEVT1', 'SOCATriggerEventCount', 'SOCA_Trigger_Event_Count_Init_Enable', 'SOCATriggerEventCountInitValue', 'SOCB_Trigger_Enable', 'SOCBTriggerSource', 'SOCBTrigger_MixedEvent_TBCTRZero',  'SOCBTrigger_MixedEvent_TBCTRPrd', 'SOCBTrigger_MixedEvent_TBCTRCMPA_Inc', 'SOCBTrigger_MixedEvent_TBCTRCMPA_Dec', 'SOCBTrigger_MixedEvent_TBCTRCMPB_Inc', 'SOCBTrigger_MixedEvent_TBCTRCMPB_Dec', 'SOCBTrigger_MixedEvent_TBCTRCMPC_Inc', 'SOCBTrigger_MixedEvent_TBCTRCMPC_Dec', 'SOCBTrigger_MixedEvent_TBCTRCMPD_Inc', 'SOCBTrigger_MixedEvent_TBCTRCMPD_Dec', 'SOCBTrigger_MixedEvent_DCxEVT1','SOCBTriggerEventCount', ...
                'SOCB_Trigger_Event_Count_Init_Enable', 'SOCBTriggerEventCountInitValue'});

            HRPWM = matlab.system.display.SectionGroup(...
            'Title', 'HRPWM', ...
            'PropertyList',  {'Enable_HRPWM_Support','Enable_Automatic_HRMSTEP_Scaling','Enable_SFO', 'MEP_Scale', 'ControlModeChannelA','ControlModeChannelB',...
            'HRPWM_High_Resolution_Phase', 'High_Resolution_Phase_Load_Enable', 'HRPWMChannelAEdgeMode','High_Resolution_CMPA','CMPAHRShadowToActiveLoadEvent','HRPWMChannelBEdgeMode','High_Resolution_CMPB','CMPBHRShadowToActiveLoadEvent', ...
            'HRPWM_High_Resolution_Period_Enable','HRPWM_High_Resolution_Period','HRPWMDeadbandEdgeMode','HRPWM_High_Resolution_RED','ShadowActiveLoadDBREDHR','HRPWM_High_Resolution_FED','ShadowActiveLoadDBFEDHR','Swap_A_B_Outputs','EPWMxBOutputSelect'});

            XCMP = matlab.system.display.SectionGroup(...
                'Title', 'XCMP', ...
                'PropertyList', {'Enable_XCMP_Mode', 'Enable_Split_XCMP', 'Allocate_XCMP_CMPA', 'Allocate_XCMP_CMPB', 'SpecifyXCMPActive', 'XCMP1_Active', 'XCMP2_Active', 'XCMP3_Active', 'XCMP4_Active', 'XCMP5_Active', 'XCMP6_Active', 'XCMP7_Active', 'XCMP8_Active', ...
                'XCMP1_Active_Initial', 'XCMP2_Active_Initial', 'XCMP3_Active_Initial', 'XCMP4_Active_Initial', 'XCMP5_Active_Initial', 'XCMP6_Active_Initial', 'XCMP7_Active_Initial', 'XCMP8_Active_Initial', 'SpecifyXCMPShadow1','XCMP1_SHADOW1', 'XCMP2_SHADOW1', 'XCMP3_SHADOW1', 'XCMP4_SHADOW1', 'XCMP5_SHADOW1', 'XCMP6_SHADOW1', 'XCMP7_SHADOW1', 'XCMP8_SHADOW1', 'XCMP1_SHADOW1_Initial', 'XCMP2_SHADOW1_Initial', 'XCMP3_SHADOW1_Initial', 'XCMP4_SHADOW1_Initial', 'XCMP5_SHADOW1_Initial', 'XCMP6_SHADOW1_Initial', 'XCMP7_SHADOW1_Initial', 'XCMP8_SHADOW1_Initial', 'SpecifyXCMPShadow2', 'XCMP1_SHADOW2', 'XCMP2_SHADOW2', 'XCMP3_SHADOW2', 'XCMP4_SHADOW2', 'XCMP5_SHADOW2', 'XCMP6_SHADOW2', 'XCMP7_SHADOW2', 'XCMP8_SHADOW2', 'XCMP1_SHADOW2_Initial', 'XCMP2_SHADOW2_Initial', 'XCMP3_SHADOW2_Initial', 'XCMP4_SHADOW2_Initial', 'XCMP5_SHADOW2_Initial', 'XCMP6_SHADOW2_Initial', 'XCMP7_SHADOW2_Initial', 'XCMP8_SHADOW2_Initial', ...
                'SpecifyXCMPShadow3','XCMP1_SHADOW3', 'XCMP2_SHADOW3', 'XCMP3_SHADOW3', 'XCMP4_SHADOW3', 'XCMP5_SHADOW3', 'XCMP6_SHADOW3', 'XCMP7_SHADOW3', 'XCMP8_SHADOW3', 'XCMP1_SHADOW3_Initial', 'XCMP2_SHADOW3_Initial', 'XCMP3_SHADOW3_Initial', 'XCMP4_SHADOW3_Initial', 'XCMP5_SHADOW3_Initial', 'XCMP6_SHADOW3_Initial', 'XCMP7_SHADOW3_Initial', 'XCMP8_SHADOW3_Initial', 'EPWMXA_tbctr_XCMP1_active', 'EPWMXA_tbctr_XCMP2_active', 'EPWMXA_tbctr_XCMP3_active', 'EPWMXA_tbctr_XCMP4_active', 'EPWMXA_tbctr_XCMP5_active', 'EPWMXA_tbctr_XCMP6_active', 'EPWMXA_tbctr_XCMP7_active', 'EPWMXA_tbctr_XCMP8_active', ...
                'EPWMXB_tbctr_XCMP5_active', 'EPWMXB_tbctr_XCMP6_active', 'EPWMXB_tbctr_XCMP7_active', 'EPWMXB_tbctr_XCMP8_active', 'EPWMXA_tbctr_XCMP1_SHADOW1', 'EPWMXA_tbctr_XCMP2_SHADOW1', 'EPWMXA_tbctr_XCMP3_SHADOW1', 'EPWMXA_tbctr_XCMP4_SHADOW1', 'EPWMXA_tbctr_XCMP5_SHADOW1', 'EPWMXA_tbctr_XCMP6_SHADOW1', 'EPWMXA_tbctr_XCMP7_SHADOW1', 'EPWMXA_tbctr_XCMP8_SHADOW1', 'EPWMXB_tbctr_XCMP5_SHADOW1', 'EPWMXB_tbctr_XCMP6_SHADOW1', 'EPWMXB_tbctr_XCMP7_SHADOW1', 'EPWMXB_tbctr_XCMP8_SHADOW1', ...
                'EPWMXA_tbctr_XCMP1_SHADOW2', 'EPWMXA_tbctr_XCMP2_SHADOW2', 'EPWMXA_tbctr_XCMP3_SHADOW2', 'EPWMXA_tbctr_XCMP4_SHADOW2', 'EPWMXA_tbctr_XCMP5_SHADOW2', 'EPWMXA_tbctr_XCMP6_SHADOW2', 'EPWMXA_tbctr_XCMP7_SHADOW2', 'EPWMXA_tbctr_XCMP8_SHADOW2', 'EPWMXB_tbctr_XCMP5_SHADOW2', 'EPWMXB_tbctr_XCMP6_SHADOW2', 'EPWMXB_tbctr_XCMP7_SHADOW2', 'EPWMXB_tbctr_XCMP8_SHADOW2', ...
                'EPWMXA_tbctr_XCMP1_SHADOW3', 'EPWMXA_tbctr_XCMP2_SHADOW3', 'EPWMXA_tbctr_XCMP3_SHADOW3', 'EPWMXA_tbctr_XCMP4_SHADOW3', 'EPWMXA_tbctr_XCMP5_SHADOW3', 'EPWMXA_tbctr_XCMP6_SHADOW3', 'EPWMXA_tbctr_XCMP7_SHADOW3', 'EPWMXA_tbctr_XCMP8_SHADOW3', 'EPWMXB_tbctr_XCMP5_SHADOW3', 'EPWMXB_tbctr_XCMP6_SHADOW3', 'EPWMXB_tbctr_XCMP7_SHADOW3', 'EPWMXB_tbctr_XCMP8_SHADOW3', ...
                'XMIN_Active', 'XMAX_Active', 'XMIN_Shadow1', 'XMAX_Shadow1', 'XMIN_Shadow2', 'XMAX_Shadow2', 'XMIN_Shadow3', 'XMAX_Shadow3',  'XTBPRD_Active', 'XTBPRD_Shadow1', 'XTBPRD_Shadow2', 'XTBPRD_Shadow3',  'Load_Mode', 'Shadow_Level', 'Shadow_register_load_once', 'Repeat_Count_Shadow_Buffer2', 'Repeat_Count_Shadow_Buffer3'});
                


            groups = [TimeBase, CounterCompare, ActionQualifier, TripZone, DigitalCompare, DeadBand, Chopper, EventTrigger, HRPWM, XCMP];

            
        end  
    end

    
    methods (Access=protected)
        function setupImpl(obj, varargin) 
            if isempty(coder.target)
                % Place simulation setup code here
            else
                % Call C-function implementing device initialization
                % coder.cinclude('etpwm.h');
                % coder.ceval('sink_init');
                coder.cinclude('MW_EPWM.h');             
                switch uint16(obj.ePWMModuleEnum)
                    case 0
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM0_U_BASE');
                    case 1
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM1_U_BASE');
                    case 2
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM2_U_BASE');
                    case 3
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM3_U_BASE');
                    case 4
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM4_U_BASE');
                    case 5
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM5_U_BASE');
                    case 6
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM6_U_BASE');
                    case 7
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM7_U_BASE');
                    case 8
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM8_U_BASE');
                    case 9
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM9_U_BASE');
                    case 10
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM10_U_BASE');
                    case 11
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM11_U_BASE');
                    case 12
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM12_U_BASE');
                    case 13
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM13_U_BASE');
                    case 14
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM14_U_BASE');
                    case 15
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM15_U_BASE');
                    case 16
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM16_U_BASE');
                    case 17
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM17_U_BASE');
                    case 18
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM18_U_BASE');
                    case 19
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM19_U_BASE');
                    case 20
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM20_U_BASE');
                    case 21
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM21_U_BASE');
                    case 22
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM22_U_BASE');
                    case 23
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM23_U_BASE');
                    case 24
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM24_U_BASE');
                    case 25
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM25_U_BASE');
                    case 26
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM26_U_BASE');
                    case 27
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM27_U_BASE');
                    case 28
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM28_U_BASE');
                    case 29
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM29_U_BASE');
                    case 30
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM30_U_BASE');
                    case 31
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM31_U_BASE');
                    otherwise
                        obj.baseaddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_G0_EPWM0_U_BASE');
                end
                switch uint16(obj.ePWMModuleEnum)
                    case 0
                        obj.module_instance = coder.opaque('uint32_t', '0');
                    case 1
                        obj.module_instance = coder.opaque('uint32_t', '1');
                    case 2
                        obj.module_instance = coder.opaque('uint32_t', '2');
                    case 3
                        obj.module_instance = coder.opaque('uint32_t', '3');
                    case 4
                        obj.module_instance = coder.opaque('uint32_t', '4');
                    case 5
                        obj.module_instance = coder.opaque('uint32_t', '5');
                    case 6
                        obj.module_instance = coder.opaque('uint32_t', '6');
                    case 7
                        obj.module_instance = coder.opaque('uint32_t', '7');
                    case 8
                        obj.module_instance = coder.opaque('uint32_t', '8');
                    case 9
                        obj.module_instance = coder.opaque('uint32_t', '9');
                    case 10
                        obj.module_instance = coder.opaque('uint32_t', '10');
                    case 11
                        obj.module_instance = coder.opaque('uint32_t', '11');
                    case 12
                        obj.module_instance = coder.opaque('uint32_t', '12');
                    case 13
                        obj.module_instance = coder.opaque('uint32_t', '13');
                    case 14
                        obj.module_instance = coder.opaque('uint32_t', '14');
                    case 15
                        obj.module_instance = coder.opaque('uint32_t', '15');
                    case 16
                        obj.module_instance = coder.opaque('uint32_t', '16');
                    case 17
                        obj.module_instance = coder.opaque('uint32_t', '17');
                    case 18
                        obj.module_instance = coder.opaque('uint32_t', '18');
                    case 19
                        obj.module_instance = coder.opaque('uint32_t', '19');
                    case 20
                        obj.module_instance = coder.opaque('uint32_t', '20');
                    case 21
                        obj.module_instance = coder.opaque('uint32_t', '21');
                    case 22
                        obj.module_instance = coder.opaque('uint32_t', '22');
                    case 23
                        obj.module_instance = coder.opaque('uint32_t', '23');
                    case 24
                        obj.module_instance = coder.opaque('uint32_t', '24');
                    case 25
                        obj.module_instance = coder.opaque('uint32_t', '25');
                    case 26
                        obj.module_instance = coder.opaque('uint32_t', '26');
                    case 27
                        obj.module_instance = coder.opaque('uint32_t', '27');
                    case 28
                        obj.module_instance = coder.opaque('uint32_t', '28');
                    case 29
                        obj.module_instance = coder.opaque('uint32_t', '29');
                    case 30
                        obj.module_instance = coder.opaque('uint32_t', '30');
                    case 31
                        obj.module_instance = coder.opaque('uint32_t', '31');
                    otherwise
                        obj.module_instance = coder.opaque('uint32_t', '0');
                end
                
               
                %Time-Base Module

                if(isequal(obj.SpecifyTimerPeriod, 'Input Port'))
                    coder.ceval('EPWM_setTimeBasePeriod', obj.baseaddr, obj.Timer_Initial_Period);
                else
                    coder.ceval('EPWM_setTimeBasePeriod', obj.baseaddr, obj.Timer_Period);
                end

                epwm_timebase_struct = struct('base', obj.baseaddr, 'instance', obj.module_instance, 'emulation_mode', uint16(obj.EmulationModeEnum), 'tbclk', uint16(obj.TBCLKEnum), 'hsclkdiv', uint16(obj.HSPCLKDIVEnum), 'timebaseperiod', uint16(obj.Timer_Period), 'enable_timebase_global_load_mode', obj.Enable_TimeBase_Global_Load_Mode, 'globalLoadRegister', uint16(obj.GlobalRegisterLoadEnum), 'period_load', uint16(obj.PeriodLoadModeEnum), 'periodShadowLoadMode', uint16(obj.PeriodLoadEventEnum), 'timeBaseCountValue', uint16(obj.CounterInitialValue), 'counterMode', uint16(obj.CounterModeEnum), 'phaseShiftLoadEnable', obj.Phase_Shift_Load_Enable, 'phaseCount', uint16(obj.Phase_Shift_Value), 'SoftwareForceSyncOut', obj.Software_Force_EPWM_SyncOut_Pulse, 'CounterZeroSyncOut', obj.Counter_Zero_EPWM_SyncOut_Pulse, 'CounterCompareBSyncOut', obj.Counter_CMPB_EPWM_SyncOut_Pulse, 'CounterCompareCSyncOut', obj.Counter_CMPC_EPWM_SyncOut_Pulse, 'CounterCompareDSyncOut', obj.Counter_CMPD_EPWM_SyncOut_Pulse, 'DCAEVT1SyncOut' , obj.DCA_Event1_Sync_Signal, 'DCAEVT2SyncOut' , obj.DCB_Event1_Sync_Signal, 'syncInPulseSource', uint16(obj.SyncInPulseSrcEnum), 'oneShotTrigger', uint16(obj.OneShotTriggerEnum));
                coder.cstructname(epwm_timebase_struct, 'TimeBaseStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_timebase_setup', coder.ref(epwm_timebase_struct));

                %Counter-Compare Module
                
                if(isequal(obj.SpecifyCMPAVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPAModule, obj.CMPA_Initial_Value);     
                else
                     coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPAModule, obj.CMPAValue);
                end
                if(isequal(obj.SpecifyCMPBVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPBModule, obj.CMPB_Initial_Value);
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPBModule, obj.CMPBValue);
                end
                if(isequal(obj.SpecifyCMPCVia, 'Input Port'))
                       coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPCModule, obj.CMPC_Initial_Value);
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPCModule, obj.CMPCValue);
                end
                if(isequal(obj.SpecifyCMPDVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPDModule, obj.CMPD_Initial_Value);
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPDModule, obj.CMPDValue);
                end

                epwm_countercompare_struct = struct('base', obj.baseaddr, 'counter_cmpa_module', uint16(obj.CounterCMPAModule), 'EnableGlobalLoadCMPA', obj.Enable_Global_Load_CMPA, 'GlobalLoadRegisterCMPA', uint16(obj.GlobalLoadRegisterCMPAEnum), 'EnableShadowLoadCMPA', obj.Enable_Shadow_Load_CMPA, 'ShadowAMode', uint16(obj.SHDWAMODEEnum), 'Enable_LinkCMPA', obj.Enable_LinkCounterCMPA, 'CMPACurrentLink', uint16(obj.ePWMLinkCMPAEnum), 'counter_cmpa_value', uint16(obj.CMPAValue), 'counter_cmpb_module', uint16(obj.CounterCMPBModule), 'EnableGlobalLoadCMPB', obj.Enable_Global_Load_CMPB, 'GlobalLoadRegisterCMPB', uint16(obj.GlobalLoadRegisterCMPBEnum), 'EnableShadowLoadCMPB', obj.Enable_Shadow_Load_CMPB, 'ShadowBMode', uint16(obj.SHDWBMODEEnum), 'Enable_LinkCMPB', obj.Enable_LinkCounterCMPB, 'CMPBCurrentLink', uint16(obj.ePWMLinkCMPBEnum), 'counter_cmpb_value', uint16(obj.CMPBValue), 'counter_cmpc_module', uint16(obj.CounterCMPCModule), 'EnableGlobalLoadCMPC', obj.Enable_Global_Load_CMPC, 'GlobalLoadRegisterCMPC', uint16(obj.GlobalLoadRegisterCMPCEnum), 'EnableShadowLoadCMPC', obj.Enable_Shadow_Load_CMPC, 'ShadowCMode', uint16(obj.SHDWCMODEEnum), 'Enable_LinkCMPC', obj.Enable_LinkCounterCMPC, 'CMPCCurrentLink', uint16(obj.ePWMLinkCMPCEnum), 'counter_cmpc_value', uint16(obj.CMPCValue), 'counter_cmpd_module', uint16(obj.CounterCMPDModule), 'EnableGlobalLoadCMPD', obj.Enable_Global_Load_CMPD, 'GlobalLoadRegisterCMPD', uint16(obj.GlobalLoadRegisterCMPDEnum), 'EnableShadowLoadCMPD', obj.Enable_Shadow_Load_CMPD, 'ShadowDMode', uint16(obj.SHDWDMODEEnum), 'Enable_LinkCMPD', obj.Enable_LinkCounterCMPD, 'CMPDCurrentLink', uint16(obj.ePWMLinkCMPDEnum), 'counter_cmpd_value', uint16(obj.CMPDValue));
                coder.cstructname(epwm_countercompare_struct, 'CounterCompareStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_counter_compare_setup', coder.ref(epwm_countercompare_struct));
                
                %Action Qualifier
                epwm_actionqualifier_struct = struct('base', obj.baseaddr, 'Enable_softwareForceGlobalLoad', obj.Enable_Software_Force_Global_Load, 'GlobalLoadRegisterContSWForce', uint16(obj.ContSWForceGlobalLoadRegisterEnum), 'cont_force_shadow_mode', uint16(obj.SoftwareForceShadowModeEnum), 'AQ_T1_TriggerSource', uint16(obj.T1TriggerSourceEnum), 'AQ_T2_TriggerSource', uint16(obj.T2TriggerSourceEnum), 'Enable_ePWMXAGlobalLoad', obj.EnableePWMXAGlobalLoad, 'ePWMXAGlobalLoadRegister', uint16(obj.ePWMXAGlobalLoadRegisterEnum), 'Enable_ePWMXAShadowMode', obj.Enable_ePWMXA_Shadow_Mode, 'AQ_ModuleA', uint16(obj.AQ_ModuleA), 'ePWMXAShadowLoadEvent', uint16(obj.ePWMXAShadowLoadEventEnum), 'ePWMXA_OneTimeSWForceAction', uint16(obj.ePWMXAOneTimeSWForceActionEnum), 'ePWMXA_ContSWForceAction', uint16(obj.ePWMXAContinuousSWForceActionEnum), 'ePWMXA_TbctrZero', uint16(obj.ePWMXATbctrZeroEnum), 'ePWMXA_TbctrPrd', uint16(obj.ePWMXATbctrPrdEnum), 'ePWMXA_TbctrCMPAUp', uint16(obj.ePWMXATbctrCMPAUpEnum), 'ePWMXA_TbctrCMPADown', uint16(obj.ePWMXATbctrCMPADownEnum), 'ePWMXA_TbctrCMPBUp', uint16(obj.ePWMXATbctrCMPBUpEnum), 'ePWMXA_TbctrCMPBDown', uint16(obj.ePWMXATbctrCMPBDownEnum), 'ePWMXA_TbctrT1Up', uint16(obj.ePWMXAT1UpEnum), 'ePWMXA_TbctrT1Down', uint16(obj.ePWMXAT1DownEnum), 'ePWMXA_TbctrT2Up', uint16(obj.ePWMXAT2UpEnum), 'ePWMXA_TbctrT2Down', uint16(obj.ePWMXAT2DownEnum), 'Enable_ePWMXBGlobalLoad', obj.EnableePWMXBGlobalLoad, 'ePWMXBGlobalLoadRegister', uint16(obj.ePWMXBGlobalLoadRegisterEnum), 'Enable_ePWMXBShadowMode', obj.Enable_ePWMXB_Shadow_Mode, 'AQ_ModuleB', uint16(obj.AQ_ModuleB), 'ePWMXBShadowLoadEvent', uint16(obj.ePWMXBShadowLoadEventEnum), 'ePWMXB_OneTimeSWForceAction', uint16(obj.ePWMXBOneTimeSWForceActionEnum), 'ePWMXB_ContSWForceAction', uint16(obj.ePWMXBContinuousSWForceActionEnum), 'ePWMXB_TbctrZero', uint16(obj.ePWMXBTbctrZeroEnum), 'ePWMXB_TbctrPrd', uint16(obj.ePWMXBTbctrPrdEnum), 'ePWMXB_TbctrCMPAUp', uint16(obj.ePWMXBTbctrCMPAUpEnum), 'ePWMXB_TbctrCMPADown', uint16(obj.ePWMXBTbctrCMPADownEnum), 'ePWMXB_TbctrCMPBUp', uint16(obj.ePWMXBTbctrCMPBUpEnum), 'ePWMXB_TbctrCMPBDown', uint16(obj.ePWMXBTbctrCMPBDownEnum), 'ePWMXB_TbctrT1Up', uint16(obj.ePWMXBT1UpEnum), 'ePWMXB_TbctrT1Down', uint16(obj.ePWMXBT1DownEnum), 'ePWMXB_TbctrT2Up', uint16(obj.ePWMXBT2UpEnum), 'ePWMXB_TbctrT2Down', uint16(obj.ePWMXBT2DownEnum));
                coder.cstructname(epwm_actionqualifier_struct, 'ActionQualifierStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_action_qualifier_setup', coder.ref(epwm_actionqualifier_struct));

                %Trip Zone
                epwm_tripzone_struct = struct('base', obj.baseaddr, 'advanced_epwm_tripzone_actions', obj.Use_Advanced_EPWM_Trip_Zone_Actions, 'TZB_D', uint16(obj.TZBDEventEnum) , 'TZB_U', uint16(obj.TZBUEventEnum) , 'TZA_D', uint16(obj.TZADEventEnum) , 'TZA_U', uint16(obj.TZAUEventEnum) , 'DCAEVT1_U', uint16(obj.DCAEVT1UEventEnum) , 'DCAEVT1_D', uint16(obj.DCAEVT1DEventEnum) , 'DCAEVT2_U', uint16(obj.DCAEVT2UEventEnum) , 'DCAEVT2_D', uint16(obj.DCAEVT2DEventEnum) ,'DCBEVT1_U', uint16(obj.DCBEVT1UEventEnum), 'DCBEVT1_D', uint16(obj.DCBEVT1DEventEnum) , 'DCBEVT2_U', uint16(obj.DCBEVT2UEventEnum) , 'DCBEVT2_D', uint16(obj.DCBEVT2DEventEnum) , 'TZA_Event', uint16(obj.TZAEventEnum) , 'TZB_Event', uint16(obj.TZBEventEnum) , 'DCAEVT1_Event', uint16(obj.DCAEVT1EventEnum) , 'DCAEVT2_Event', uint16(obj.DCAEVT2EventEnum) , 'DCBEVT1_Event', uint16(obj.DCBEVT1EventEnum), 'DCBEVT2_Event', uint16(obj.DCBEVT2EventEnum), 'enable_EPWM_TZ_SIGNAL_OSHT1', obj.One_Shot_TZ1, 'enable_EPWM_TZ_SIGNAL_OSHT2', obj.One_Shot_TZ2, 'enable_EPWM_TZ_SIGNAL_OSHT3', obj.One_Shot_TZ3, 'enable_EPWM_TZ_SIGNAL_OSHT4', obj.One_Shot_TZ4, 'enable_EPWM_TZ_SIGNAL_OSHT5', obj.One_Shot_TZ5, 'enable_EPWM_TZ_SIGNAL_OSHT6', obj.One_Shot_TZ6, 'enable_EPWM_TZ_SIGNAL_DCAEVT1', obj.One_Shot_DCAEVT1, 'enable_EPWM_TZ_SIGNAL_DCBEVT1', obj.One_Shot_DCBEVT1, 'enable_EPWM_TZ_SIGNAL_CBC1', obj.TZ1_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_CBC2', obj.TZ2_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_CBC3', obj.TZ3_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_CBC4', obj.TZ4_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_CBC5', obj.TZ5_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_CBC6', obj.TZ6_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_DCAEVT2', obj.DCAEVT2_Cycle_By_Cycle, 'enable_EPWM_TZ_SIGNAL_DCBEVT2', obj.DCBEVT2_Cycle_By_Cycle, 'CBC_Clear_Event', uint16(obj.CBCLatchClearSignalEnum), 'CBC_interrupt', obj.Trip_Zones_Cycle_By_Cycle_interrupt, 'OneShot_interrupt', obj.Trip_Zones_One_Shot_interrupt, 'DCAEVT1_interrupt', obj.Digital_CompareA_Event1_interrupt, 'DCAEVT2_interrupt', obj.Digital_CompareA_Event2_interrupt, 'DCBEVT1_interrupt', obj.Digital_CompareB_Event1_interrupt, 'DCBEVT2_interrupt', obj.Digital_CompareB_Event2_interrupt);
                coder.cstructname(epwm_tripzone_struct, 'TripZoneStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_trip_zone_setup', coder.ref(epwm_tripzone_struct));
                
                %Digital Compare
                epwm_digitalcompare_struct = struct('base', obj.baseaddr, 'DCFilter_InputEventSource', uint16(obj.DCFilterInputEventSourceEnum), 'EnableBlankingWindow', obj.Blanking_Window_Usage, 'BWStartEvent', uint16(obj.BlankingWindowStartEventEnum), 'Enable_MixedEvent_TBCTRZero', obj.BlankingWindow_MixedEvent_TBCTRZero, 'Enable_MixedEvent_TBCTRPeriod', obj.BlankingWindow_MixedEvent_TBCTRPrd, 'Enable_MixedEvent_TBCTRCMPAIncreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPA_Inc, 'Enable_MixedEvent_TBCTRCMPADecreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPA_Dec, 'Enable_MixedEvent_TBCTRCMPBIncreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPB_Inc, 'Enable_MixedEvent_TBCTRCMPBDecreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPB_Dec, 'Enable_MixedEvent_TBCTRCMPCIncreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPC_Inc, 'Enable_MixedEvent_TBCTRCMPCDecreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPC_Dec, 'Enable_MixedEvent_TBCTRCMPDIncreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPD_Inc, 'Enable_MixedEvent_TBCTRCMPDDecreasing', obj.BlankingWindow_MixedEvent_TBCTRCMPD_Dec, 'BW_Offset', obj.Blanking_Window_Offset, 'BW_Length', obj.Blanking_Window_Length, 'Enable_InvertBW', obj.Invert_Blanking_Window, 'Enable_DCCounterCapture', obj.Enable_DC_Counter_Capture, 'Enable_ShadowMode', obj.Enable_Counter_Capture_Shadow_Mode, 'Enable_EdgeFilter', obj.Use_Edge_Filter, 'edgeMode', uint16(obj.EdgeFilterModeEnum), 'edgeCount', uint16(obj.EdgeFilterEdgeCountEnum), 'Enable_EdgeFilterValleyCapture', obj.Enable_Edge_Filter_Valley_Capture, 'trigger', uint16(obj.EdgeFilterCounterCaptureSignalEnum), 'startCount', uint16(obj.StartValleyCaptureEnum), 'stopCount', uint16(obj.StopValleyCaptureEnum), 'selectDelayedOutputEdgeFilter', obj.Select_Delayed_Output_of_Edge_Filter, 'DelayOffset', obj.Software_Valley_Delay_Value, 'delayMode', uint16(obj.ValleyDelayDividerEnum), 'TripInputDCAH', uint16(obj.DCAHighEnum), 'TripInputDCAL', uint16(obj.DCALowEnum), 'Compare1A_Condition', uint16(obj.DCOutput1AConditionEnum), 'Compare2A_Condition', uint16(obj.DCOutput2AConditionEnum), 'Enable_ADCSOCDCAEVT1', obj.ADC_SOC_Generate_DCAEVT1, 'Enable_SYNCOUTDCAEVT1', obj.SYNCOUT_Generate_DCAEVT1, 'SynchModeDCAEVT1', uint16(obj.SynchModeDCAEVT1Enum), 'SignalSourceDCAEVT1',uint16(obj.SignalSourceDCAEVT1Enum), 'CBCLatchModeDCAEVT1', uint16(obj.CBCLatchModeDCAEVT1Enum), 'CBCLatchClearDCAEVT1', uint16(obj.CBCLatchClearEventDCAEVT1Enum), 'SynchModeDCAEVT2', uint16(obj.SynchModeDCAEVT2Enum), 'SignalSourceDCAEVT2', uint16(obj.SignalSourceDCAEVT2Enum), 'CBCLatchModeDCAEVT2', uint16(obj.CBCLatchModeDCAEVT2Enum), 'CBCLatchClearDCAEVT2', uint16(obj.CBCLatchClearEventDCAEVT2Enum), 'TripInputDCBH', uint16(obj.DCBHighEnum), 'TripInputDCBL', uint16(obj.DCBLowEnum), 'Compare1B_Condition', uint16(obj.DCOutput1BConditionEnum), 'Compare2B_Condition', uint16(obj.DCOutput2BConditionEnum), 'Enable_ADCSOCDCBEVT1', obj.ADC_SOC_Generate_DCBEVT1, 'Enable_SYNCOUTDCBEVT1', obj.SYNCOUT_Generate_DCBEVT1, 'SynchModeDCBEVT1', uint16(obj.SynchModeDCBEVT1Enum), 'SignalSourceDCBEVT1', uint16(obj.SignalSourceDCBEVT1Enum), 'CBCLatchModeDCBEVT1', uint16(obj.CBCLatchModeDCBEVT1Enum), 'CBCLatchClearDCBEVT1', uint16(obj.CBCLatchClearEventDCBEVT1Enum), 'SynchModeDCBEVT2', uint16(obj.SynchModeDCBEVT2Enum), 'SignalSourceDCBEVT2',uint16(obj.SignalSourceDCBEVT2Enum), 'CBCLatchModeDCBEVT2',uint16(obj.CBCLatchModeDCBEVT2Enum), 'CBCLatchClearDCBEVT2', uint16(obj.CBCLatchClearEventDCBEVT2Enum), 'Enable_Edge_Detection', obj.Use_Event_Detection, 'Capture_Input', uint16(obj.Capture_InputEnum), 'Invert_Capture_Input_Polarity', uint8(obj.Invert_Capture_Input_PolarityEnum), 'Capture_Gate', uint16(obj.Capture_gateEnum), 'Capture_Gate_Polarity_Sel', uint8(obj.Capture_gate_polarity_selectEnum), 'Capture_Input_Trip1', obj.Combinational_capture_input_Trip1, 'Capture_Input_Trip2', obj.Combinational_capture_input_Trip2, 'Capture_Input_Trip3', obj.Combinational_capture_input_Trip3, 'Capture_Input_Trip4', obj.Combinational_capture_input_Trip4, 'Capture_Input_Trip5', obj.Combinational_capture_input_Trip5, 'Capture_Input_Trip6', obj.Combinational_capture_input_Trip6, 'Capture_Input_Trip7', obj.Combinational_capture_input_Trip7, 'Capture_Input_Trip8', obj.Combinational_capture_input_Trip8, 'Capture_Input_Trip9', obj.Combinational_capture_input_Trip9, 'Capture_Input_Trip10', obj.Combinational_capture_input_Trip10, 'Capture_Input_Trip11', obj.Combinational_capture_input_Trip11, 'Capture_Input_Trip12', obj.Combinational_capture_input_Trip12, 'Capture_Input_Trip13', obj.Combinational_capture_input_Trip13, 'Capture_Input_Trip14', obj.Combinational_capture_input_Trip14, 'Capture_Input_Trip15', obj.Combinational_capture_input_Trip15, 'Capture_Gate_Trip1', obj.Combinational_capture_gate_Trip1, 'Capture_Gate_Trip2', obj.Combinational_capture_gate_Trip2, 'Capture_Gate_Trip3', obj.Combinational_capture_gate_Trip3, 'Capture_Gate_Trip4', obj.Combinational_capture_gate_Trip4, 'Capture_Gate_Trip5', obj.Combinational_capture_gate_Trip5, 'Capture_Gate_Trip6', obj.Combinational_capture_gate_Trip6, 'Capture_Gate_Trip7', obj.Combinational_capture_gate_Trip7, 'Capture_Gate_Trip8', obj.Combinational_capture_gate_Trip8, 'Capture_Gate_Trip9', obj.Combinational_capture_gate_Trip9, 'Capture_Gate_Trip10', obj.Combinational_capture_gate_Trip10, 'Capture_Gate_Trip11', obj.Combinational_capture_gate_Trip11, 'Capture_Gate_Trip12', obj.Combinational_capture_gate_Trip12, 'Capture_Gate_Trip13', obj.Combinational_capture_gate_Trip13, 'Capture_Gate_Trip14', obj.Combinational_capture_gate_Trip14, 'Capture_Gate_Trip15', obj.Combinational_capture_gate_Trip15, 'DCAHighCombTrip1', obj.DCAHigh_CombTrip1_Input, 'DCAHighCombTrip2', obj.DCAHigh_CombTrip2_Input, 'DCAHighCombTrip3', obj.DCAHigh_CombTrip3_Input, 'DCAHighCombTrip4', obj.DCAHigh_CombTrip4_Input, 'DCAHighCombTrip5', obj.DCAHigh_CombTrip5_Input, 'DCAHighCombTrip6', obj.DCAHigh_CombTrip6_Input, 'DCAHighCombTrip7', obj.DCAHigh_CombTrip7_Input, 'DCAHighCombTrip8', obj.DCAHigh_CombTrip8_Input, 'DCAHighCombTrip9', obj.DCAHigh_CombTrip9_Input, 'DCAHighCombTrip10', obj.DCAHigh_CombTrip10_Input, 'DCAHighCombTrip11', obj.DCAHigh_CombTrip11_Input, 'DCAHighCombTrip12', obj.DCAHigh_CombTrip12_Input, 'DCAHighCombTrip13', obj.DCAHigh_CombTrip13_Input, 'DCAHighCombTrip14', obj.DCAHigh_CombTrip14_Input, 'DCAHighCombTrip15', obj.DCAHigh_CombTrip15_Input, 'DCALowCombTrip1', obj.DCALow_CombTrip1_Input, 'DCALowCombTrip2', obj.DCALow_CombTrip2_Input, 'DCALowCombTrip3', obj.DCALow_CombTrip3_Input, 'DCALowCombTrip4', obj.DCALow_CombTrip4_Input, 'DCALowCombTrip5', obj.DCALow_CombTrip5_Input, 'DCALowCombTrip6', obj.DCALow_CombTrip6_Input, 'DCALowCombTrip7', obj.DCALow_CombTrip7_Input, 'DCALowCombTrip8', obj.DCALow_CombTrip8_Input, 'DCALowCombTrip9', obj.DCALow_CombTrip9_Input, 'DCALowCombTrip10', obj.DCALow_CombTrip10_Input, 'DCALowCombTrip11', obj.DCALow_CombTrip11_Input, 'DCALowCombTrip12', obj.DCALow_CombTrip12_Input, 'DCALowCombTrip13', obj.DCALow_CombTrip13_Input, 'DCALowCombTrip14', obj.DCALow_CombTrip14_Input, 'DCALowCombTrip15', obj.DCALow_CombTrip15_Input, 'DCBHighCombTrip1', obj.DCBHigh_CombTrip1_Input, 'DCBHighCombTrip2', obj.DCBHigh_CombTrip2_Input, 'DCBHighCombTrip3', obj.DCBHigh_CombTrip3_Input, 'DCBHighCombTrip4', obj.DCBHigh_CombTrip4_Input, 'DCBHighCombTrip5', obj.DCBHigh_CombTrip5_Input, 'DCBHighCombTrip6', obj.DCBHigh_CombTrip6_Input, 'DCBHighCombTrip7', obj.DCBHigh_CombTrip7_Input, 'DCBHighCombTrip8', obj.DCBHigh_CombTrip8_Input, 'DCBHighCombTrip9', obj.DCBHigh_CombTrip9_Input, 'DCBHighCombTrip10', obj.DCBHigh_CombTrip10_Input, 'DCBHighCombTrip11', obj.DCBHigh_CombTrip11_Input, 'DCBHighCombTrip12', obj.DCBHigh_CombTrip12_Input, 'DCBHighCombTrip13', obj.DCBHigh_CombTrip13_Input, 'DCBHighCombTrip14', obj.DCBHigh_CombTrip14_Input, 'DCBHighCombTrip15', obj.DCBHigh_CombTrip15_Input, 'DCBLowCombTrip1', obj.DCBLow_CombTrip1_Input, 'DCBLowCombTrip2', obj.DCBLow_CombTrip2_Input, 'DCBLowCombTrip3', obj.DCBLow_CombTrip3_Input, 'DCBLowCombTrip4', obj.DCBLow_CombTrip4_Input, 'DCBLowCombTrip5', obj.DCBLow_CombTrip5_Input, 'DCBLowCombTrip6', obj.DCBLow_CombTrip6_Input, 'DCBLowCombTrip7', obj.DCBLow_CombTrip7_Input, 'DCBLowCombTrip8', obj.DCBLow_CombTrip8_Input, 'DCBLowCombTrip9', obj.DCBLow_CombTrip9_Input, 'DCBLowCombTrip10', obj.DCBLow_CombTrip10_Input, 'DCBLowCombTrip11', obj.DCBLow_CombTrip11_Input, 'DCBLowCombTrip12', obj.DCBLow_CombTrip12_Input, 'DCBLowCombTrip13', obj.DCBLow_CombTrip13_Input, 'DCBLowCombTrip14', obj.DCBLow_CombTrip14_Input, 'DCBLowCombTrip15', obj.DCBLow_CombTrip15_Input);
                coder.cstructname(epwm_digitalcompare_struct, 'DigitalCompareStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_digital_compare_setup', coder.ref(epwm_digitalcompare_struct));                                                                                                                                                                                                                                              

                %Deadband
                epwm_deadband_struct = struct('base', obj.baseaddr, 'RisingEdgeDelayInput', uint16(obj.RisingEdgeDelayInputEnum), 'FallingEdgeDelayInput', uint16(obj.FallingEdgeDelayInputEnum), 'RED_DeadBandPolarity', uint16(obj.RisingEdgeDelayPolarityEnum), 'FED_DeadBandPolarity', uint16(obj.FallingEdgeDelayPolarityEnum), 'Enable_RisingEdgeDelay', obj.Enable_Rising_Edge_Delay, 'Rising_Edge_Delay_Value', obj.Rising_Edge_Delay_Value, 'Enable_FallingEdgeDelay', obj.Enable_Falling_Edge_Delay, 'Falling_Edge_Delay_Value', obj.Falling_Edge_Delay_Value, 'Swap_Output_ePWMXA', obj.Swap_Output_for_EPWMxA, 'Swap_Output_ePWMXB', obj.Swap_Output_for_EPWMxB, 'Enable_DeadbandControlGlobalLoad', obj.Enable_Deadband_Control_Global_Load, 'DeadbandGlobalLoadRegister', uint16(obj.DeadbandGlobalLoadEnum), 'Enable_DeadbandControlShadowMode', obj.EnableDeadbandControlShadowMode, 'DeadbandLoadMode', uint16(obj.DeadbandControlShadowModeEnum), 'Enable_REDGlobalLoad', obj.Enable_RED_Global_Load, 'REDGlobalLoadRegister', uint16(obj.REDGlobalLoadRegisterEnum), 'Enable_REDShadowMode', obj.Enable_RED_Shadow_Mode, 'RisingEdgeLoadMode', uint16(obj.REDShadowLoadEventEnum), 'Enable_FEDGlobalLoad', obj.Enable_FED_Global_Load, 'FEDGlobalLoadRegister', uint16(obj.FEDGlobalLoadRegisterEnum), 'Enable_FEDShadowMode', obj.Enable_FED_Shadow_Mode, 'FallingEdgeLoadMode', uint16(obj.FEDShadowLoadEventEnum), 'DeadBandClockRate', uint16(obj.DeadBandCounterClockRateEnum));
                coder.cstructname(epwm_deadband_struct, 'DeadBandStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_deadband_setup', coder.ref(epwm_deadband_struct));

                %Chopper
                epwm_chopper_struct = struct('base', obj.baseaddr, 'UseChopper', obj.Use_Chopper, 'DutyCycle', uint16(obj.ChopperDutyCycleEnum), 'freqDiv', uint16(obj.ChopperFrequencyEnum), 'FirstPulseWidth', uint16(obj.ChopperFirstPulseWidthEnum));
                coder.cstructname(epwm_chopper_struct, 'ChopperStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_chopper_setup', coder.ref(epwm_chopper_struct));
               
                %Event Trigger
                epwm_eventtrigger_struct = struct('base', obj.baseaddr, 'Enable_EPWMInterrupt', obj.Enable_EPWM_Interrupt, 'InterruptEventSource', uint16(obj.InterruptEventSourcesEnum), 'Enable_EventMixedSourceTbctrZero', obj.EventTrigger_MixedEvent_TBCTRZero, 'Enable_EventMixedSourceTbctrPeriod', obj.EventTrigger_MixedEvent_TBCTRPrd, 'Enable_EventMixedSourceTbctrCMPAInc', obj.EventTrigger_MixedEvent_TBCTRCMPA_Inc, 'Enable_EventMixedSourceTbctrCMPADec', obj.EventTrigger_MixedEvent_TBCTRCMPA_Dec, 'Enable_EventMixedSourceTbctrCMPBInc', obj.EventTrigger_MixedEvent_TBCTRCMPB_Inc, 'Enable_EventMixedSourceTbctrCMPBDec', obj.EventTrigger_MixedEvent_TBCTRCMPB_Dec, 'Enable_EventMixedSourceTbctrCMPCInc', obj.EventTrigger_MixedEvent_TBCTRCMPC_Inc, 'Enable_EventMixedSourceTbctrCMPCDec', obj.EventTrigger_MixedEvent_TBCTRCMPC_Dec, 'Enable_EventMixedSourceTbctrCMPDInc', obj.EventTrigger_MixedEvent_TBCTRCMPD_Inc, 'Enable_EventMixedSourceTbctrCMPDDec', obj.EventTrigger_MixedEvent_TBCTRCMPD_Dec, 'Enable_EventMixedSourceDCxEVT1', obj.EventTrigger_MixedEvent_DCxEVT1, 'eventCount', uint16(obj.InterruptEventCountEnum), 'Enable_InterruptEventCountInitialValue', obj.Enable_Interrupt_Event_Count_Initial_Value, 'EventCountInitValue', uint16(obj.InterruptEventCountInitialValueEnum), 'SOCATriggerEnable', obj.SOCA_Trigger_Enable, 'SOCASource', uint16(obj.SOCATriggerSourceEnum), 'Enable_SOCAMixedSourceTbctrZero', obj.SOCATrigger_MixedEvent_TBCTRZero, 'Enable_SOCAMixedSourceTbctrPeriod', obj.SOCATrigger_MixedEvent_TBCTRPrd, 'Enable_SOCAMixedSourceTbctrCMPAInc', obj.SOCATrigger_MixedEvent_TBCTRCMPA_Inc, 'Enable_SOCAMixedSourceTbctrCMPADec', obj.SOCATrigger_MixedEvent_TBCTRCMPA_Dec, 'Enable_SOCAMixedSourceTbctrCMPBInc', obj.SOCATrigger_MixedEvent_TBCTRCMPB_Inc, 'Enable_SOCAMixedSourceTbctrCMPBDec', obj.SOCATrigger_MixedEvent_TBCTRCMPB_Dec, 'Enable_SOCAMixedSourceTbctrCMPCInc', obj.SOCATrigger_MixedEvent_TBCTRCMPC_Inc, 'Enable_SOCAMixedSourceTbctrCMPCDec', obj.SOCATrigger_MixedEvent_TBCTRCMPC_Dec, 'Enable_SOCAMixedSourceTbctrCMPDInc', obj.SOCATrigger_MixedEvent_TBCTRCMPD_Inc, 'Enable_SOCAMixedSourceTbctrCMPDDec', obj.SOCATrigger_MixedEvent_TBCTRCMPD_Dec, 'Enable_SOCAMixedSourceDCxEVT1', obj.SOCATrigger_MixedEvent_DCxEVT1, 'SOCA_TriggerEventCount', uint16(obj.SOCATriggerEventCountEnum), 'Enable_SOCATriggerEventCountInit', obj.SOCA_Trigger_Event_Count_Init_Enable, 'SOCATriggerInitValue', uint16(obj.SOCATriggerEventCountInitValueEnum), 'SOCA_ForceInterruptEvent', obj.Force_SOCA_Trigger_Event_Count, 'SOCBTriggerEnable', obj.SOCB_Trigger_Enable, 'SOCBSource', uint16(obj.SOCBTriggerSourceEnum), 'Enable_SOCBMixedSourceTbctrZero', obj.SOCBTrigger_MixedEvent_TBCTRZero, 'Enable_SOCBMixedSourceTbctrPeriod', obj.SOCBTrigger_MixedEvent_TBCTRPrd, 'Enable_SOCBMixedSourceTbctrCMPAInc', obj.SOCBTrigger_MixedEvent_TBCTRCMPA_Inc, 'Enable_SOCBMixedSourceTbctrCMPADec', obj.SOCBTrigger_MixedEvent_TBCTRCMPA_Dec, 'Enable_SOCBMixedSourceTbctrCMPBInc', obj.SOCBTrigger_MixedEvent_TBCTRCMPB_Inc, 'Enable_SOCBMixedSourceTbctrCMPBDec', obj.SOCBTrigger_MixedEvent_TBCTRCMPB_Dec, 'Enable_SOCBMixedSourceTbctrCMPCInc', obj.SOCBTrigger_MixedEvent_TBCTRCMPC_Inc, 'Enable_SOCBMixedSourceTbctrCMPCDec', obj.SOCBTrigger_MixedEvent_TBCTRCMPC_Dec, 'Enable_SOCBMixedSourceTbctrCMPDInc', obj.SOCBTrigger_MixedEvent_TBCTRCMPD_Inc, 'Enable_SOCBMixedSourceTbctrCMPDDec',obj.SOCBTrigger_MixedEvent_TBCTRCMPD_Dec, 'Enable_SOCBMixedSourceDCxEVT1', obj.SOCBTrigger_MixedEvent_DCxEVT1, 'SOCB_TriggerEventCount', uint16(obj.SOCBTriggerEventCountEnum), 'Enable_SOCBTriggerEventCountInit', obj.SOCB_Trigger_Event_Count_Init_Enable, 'SOCBTriggerInitValue', uint16(obj.SOCBTriggerEventCountInitValueEnum), 'SOCB_ForceInterruptEvent', obj.Force_SOCB_Trigger_Event_Count, 'Enable_RisingEdgeDelay', obj.Enable_Rising_Edge_Delay);
                coder.cstructname(epwm_eventtrigger_struct, 'EventTriggerStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_event_trigger_setup', coder.ref(epwm_eventtrigger_struct));
              
                %HRPWM
                epwm_hrpwm_struct = struct('base', obj.baseaddr, 'Enable_HRPWMSupport', obj.Enable_HRPWM_Support, 'Enable_AutomaticHRMSTEPScaling', obj.Enable_Automatic_HRMSTEP_Scaling, 'ControlModeChannelA', uint16(obj.ControlModeChannelAEnum), 'ControlModeChannelB', uint16(obj.ControlModeChannelBEnum), 'hrPhaseCount', obj.HRPWM_High_Resolution_Phase, 'HRPWMSyncSource', uint16(obj.HRPWMSyncSourceEnum), 'HighResolutionPhaseEnable', obj.High_Resolution_Phase_Load_Enable, 'ChannelAEdgeMode', uint16(obj.HRPWMChannelAEdgeModeEnum), 'HighResolutionCMPA', obj.High_Resolution_CMPA, 'CMPAHRLoadEvent', uint16(obj.CMPAHRShadowToActiveLoadEventEnum), 'ChannelBEdgeMode', uint16(obj.HRPWMChannelBEdgeModeEnum), 'HighResolutionCMPB', obj.High_Resolution_CMPB, 'CMPBHRLoadEvent', uint16(obj.CMPBHRShadowToActiveLoadEventEnum), 'Enable_HRPWMHighResolutionPeriodLoad', obj.HRPWM_High_Resolution_Period_Enable, 'hrPeriodCount', obj.HRPWM_High_Resolution_Period, 'DeadbandEdgeMode', uint16(obj.HRPWMDeadbandEdgeModeEnum), 'hrREDDelay', obj.HRPWM_High_Resolution_RED, 'REDLoadMode', uint16(obj.ShadowActiveLoadDBREDHREnum), 'Swap_AB_Outputs', obj.Swap_A_B_Outputs, 'EPWMxBOutputPath', uint16(obj.EPWMxBOutputSelectEnum), 'hrFEDDelay', obj.HRPWM_High_Resolution_FED, 'FEDLoadMode', uint16(obj.ShadowActiveLoadDBFEDHREnum), 'Enable_SFOLibrary', obj.Enable_SFO, 'MEP_SCALE', int16(obj.MEP_Scale));
                coder.cstructname(epwm_hrpwm_struct, 'HRPWMStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_hrpwm_setup', coder.ref(epwm_hrpwm_struct));
                
                %XCMP
                if(obj.Enable_XCMP_Mode)
                    if(isequal(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 0, obj.XCMP1_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 4, obj.XCMP2_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 8, obj.XCMP3_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 12, obj.XCMP4_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 16, obj.XCMP5_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 20, obj.XCMP6_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 24, obj.XCMP7_Active_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 28, obj.XCMP8_Active_Initial);
                   else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 0, obj.XCMP1_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 4, obj.XCMP2_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 8, obj.XCMP3_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 12, obj.XCMP4_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 16, obj.XCMP5_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 20, obj.XCMP6_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 24, obj.XCMP7_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 28, obj.XCMP8_Active);
    
                    end
                end
                if(obj.Enable_XCMP_Mode && (obj.Shadow_LevelEnum >= 1))
                    if(isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 128, obj.XCMP1_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 132, obj.XCMP2_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 136, obj.XCMP3_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 140, obj.XCMP4_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 144, obj.XCMP5_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 148, obj.XCMP6_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 152, obj.XCMP7_SHADOW1_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 156, obj.XCMP8_SHADOW1_Initial);
                           
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 128, obj.XCMP1_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 132, obj.XCMP2_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 136, obj.XCMP3_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 140, obj.XCMP4_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 144, obj.XCMP5_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 148, obj.XCMP6_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 152, obj.XCMP7_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 156, obj.XCMP8_SHADOW1);
    
                    end
                end

                if(obj.Enable_XCMP_Mode && (obj.Shadow_LevelEnum >= 2))
                    if(isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 256, obj.XCMP1_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 260, obj.XCMP2_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 264, obj.XCMP3_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 268, obj.XCMP4_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 272, obj.XCMP5_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 276, obj.XCMP6_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 280, obj.XCMP7_SHADOW2_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 284, obj.XCMP8_SHADOW2_Initial);
                           
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 256, obj.XCMP1_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 260, obj.XCMP2_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 264, obj.XCMP3_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 268, obj.XCMP4_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 272, obj.XCMP5_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 276, obj.XCMP6_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 280, obj.XCMP7_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 284, obj.XCMP8_SHADOW2);
    
                    end
                end

                if(obj.Enable_XCMP_Mode && (obj.Shadow_LevelEnum >= 3))
                    if(isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 384, obj.XCMP1_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 388, obj.XCMP2_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 392, obj.XCMP3_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 396, obj.XCMP4_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 400, obj.XCMP5_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 404, obj.XCMP6_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 408, obj.XCMP7_SHADOW3_Initial);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 412, obj.XCMP8_SHADOW3_Initial);
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 384, obj.XCMP1_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 388, obj.XCMP2_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 392, obj.XCMP3_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 396, obj.XCMP4_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 400, obj.XCMP5_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 404, obj.XCMP6_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 408, obj.XCMP7_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 412, obj.XCMP8_SHADOW3);
    
                    end
                end

                epwm_xcmp_struct = struct('base', obj.baseaddr, 'instance', obj.module_instance, 'enable_XCMP', obj.Enable_XCMP_Mode, 'enable_Split', obj.Enable_Split_XCMP, 'xcmp_cmpa', uint16(obj.Allocate_XCMP_CMPAEnum), 'xcmp_cmpb', uint16(obj.Allocate_XCMP_CMPBEnum), 'load_mode', uint16(obj.Load_ModeEnum), 'shadow_level', uint16(obj.Shadow_LevelEnum), 'shadow_register_load_once', uint16(obj.Shadow_register_load_onceEnum), 'rpt_cnt_shdw_buffer2', obj.Repeat_Count_Shadow_Buffer2, 'rpt_cnt_shdw_buffer3', obj.Repeat_Count_Shadow_Buffer3, 'xcmp1_active', uint16(obj.XCMP1_Active), 'xcmp2_active', uint16(obj.XCMP2_Active), 'xcmp3_active', uint16(obj.XCMP3_Active), 'xcmp4_active', uint16(obj.XCMP4_Active), 'xcmp5_active', uint16(obj.XCMP5_Active), 'xcmp6_active', uint16(obj.XCMP6_Active), 'xcmp7_active', uint16(obj.XCMP7_Active), 'xcmp8_active', uint16(obj.XCMP8_Active), 'xcmp1_shadow1', obj.XCMP1_SHADOW1, 'xcmp2_shadow1', obj.XCMP2_SHADOW1, 'xcmp3_shadow1', obj.XCMP3_SHADOW1, 'xcmp4_shadow1', obj.XCMP4_SHADOW1, 'xcmp5_shadow1', obj.XCMP5_SHADOW1, 'xcmp6_shadow1', obj.XCMP6_SHADOW1, 'xcmp7_shadow1', obj.XCMP7_SHADOW1, 'xcmp8_shadow1', obj.XCMP8_SHADOW1, 'xcmp1_shadow2', obj.XCMP1_SHADOW2, 'xcmp2_shadow2', obj.XCMP2_SHADOW2, 'xcmp3_shadow2', obj.XCMP3_SHADOW2, 'xcmp4_shadow2', obj.XCMP4_SHADOW2, 'xcmp5_shadow2', obj.XCMP5_SHADOW2, 'xcmp6_shadow2', obj.XCMP6_SHADOW2, 'xcmp7_shadow2', obj.XCMP7_SHADOW2, 'xcmp8_shadow2', obj.XCMP8_SHADOW2, 'xcmp1_shadow3', obj.XCMP1_SHADOW3, 'xcmp2_shadow3', obj.XCMP2_SHADOW3, 'xcmp3_shadow3', obj.XCMP3_SHADOW3, 'xcmp4_shadow3', obj.XCMP4_SHADOW3, 'xcmp5_shadow3', obj.XCMP5_SHADOW3, 'xcmp6_shadow3', obj.XCMP6_SHADOW3, 'xcmp7_shadow3', obj.XCMP7_SHADOW3, 'xcmp8_shadow3', obj.XCMP8_SHADOW3, 'xtbprd_active', obj.XTBPRD_Active, 'xtbprd_shadow1', obj.XTBPRD_Shadow1, 'xtbprd_shadow2', obj.XTBPRD_Shadow2, 'xtbprd_shadow3', obj.XTBPRD_Shadow3, 'xmin_active', obj.XMIN_Active, 'xmax_active', obj.XMAX_Active, 'xmin_shadow1', obj.XMIN_Shadow1, 'xmax_shadow1', obj.XMAX_Shadow1, 'xmin_shadow2', obj.XMIN_Shadow2, 'xmax_shadow2', obj.XMAX_Shadow2, 'xmin_shadow3', obj.XMIN_Shadow3, 'xmax_shadow3', obj.XMAX_Shadow3, 'epwmxa_tbctr_xcmp1_active', uint16(obj.EPWMXA_tbctr_XCMP1_activeEnum), 'epwmxa_tbctr_xcmp2_active', uint16(obj.EPWMXA_tbctr_XCMP2_activeEnum), 'epwmxa_tbctr_xcmp3_active', uint16(obj.EPWMXA_tbctr_XCMP3_activeEnum), 'epwmxa_tbctr_xcmp4_active', uint16(obj.EPWMXA_tbctr_XCMP4_activeEnum), 'epwmxa_tbctr_xcmp5_active', uint16(obj.EPWMXA_tbctr_XCMP5_activeEnum), 'epwmxa_tbctr_xcmp6_active', uint16(obj.EPWMXA_tbctr_XCMP6_activeEnum), 'epwmxa_tbctr_xcmp7_active', uint16(obj.EPWMXA_tbctr_XCMP7_activeEnum), 'epwmxa_tbctr_xcmp8_active', uint16(obj.EPWMXA_tbctr_XCMP8_activeEnum), 'epwmxb_tbctr_xcmp5_active', uint16(obj.EPWMXB_tbctr_XCMP5_activeEnum), 'epwmxb_tbctr_xcmp6_active', uint16(obj.EPWMXB_tbctr_XCMP6_activeEnum), 'epwmxb_tbctr_xcmp7_active', uint16(obj.EPWMXB_tbctr_XCMP7_activeEnum), 'epwmxb_tbctr_xcmp8_active', uint16(obj.EPWMXB_tbctr_XCMP8_activeEnum), 'epwmxa_tbctr_xcmp1_shadow1', uint16(obj.EPWMXA_tbctr_XCMP1_SHADOW1Enum), 'epwmxa_tbctr_xcmp2_shadow1', uint16(obj.EPWMXA_tbctr_XCMP2_SHADOW1Enum), 'epwmxa_tbctr_xcmp3_shadow1', uint16(obj.EPWMXA_tbctr_XCMP3_SHADOW1Enum), 'epwmxa_tbctr_xcmp4_shadow1', uint16(obj.EPWMXA_tbctr_XCMP4_SHADOW1Enum), 'epwmxa_tbctr_xcmp5_shadow1', uint16(obj.EPWMXA_tbctr_XCMP5_SHADOW1Enum), 'epwmxa_tbctr_xcmp6_shadow1', uint16(obj.EPWMXA_tbctr_XCMP6_SHADOW1Enum), 'epwmxa_tbctr_xcmp7_shadow1', uint16(obj.EPWMXA_tbctr_XCMP7_SHADOW1Enum), 'epwmxa_tbctr_xcmp8_shadow1', uint16(obj.EPWMXA_tbctr_XCMP8_SHADOW1Enum), 'epwmxb_tbctr_xcmp5_shadow1', uint16(obj.EPWMXB_tbctr_XCMP5_SHADOW1Enum), 'epwmxb_tbctr_xcmp6_shadow1', uint16(obj.EPWMXB_tbctr_XCMP6_SHADOW1Enum), 'epwmxb_tbctr_xcmp7_shadow1', uint16(obj.EPWMXB_tbctr_XCMP7_SHADOW1Enum), 'epwmxb_tbctr_xcmp8_shadow1', uint16(obj.EPWMXB_tbctr_XCMP8_SHADOW1Enum), 'epwmxa_tbctr_xcmp1_shadow2', uint16(obj.EPWMXA_tbctr_XCMP1_SHADOW2Enum), 'epwmxa_tbctr_xcmp2_shadow2', uint16(obj.EPWMXA_tbctr_XCMP2_SHADOW2Enum), 'epwmxa_tbctr_xcmp3_shadow2', uint16(obj.EPWMXA_tbctr_XCMP3_SHADOW2Enum), 'epwmxa_tbctr_xcmp4_shadow2', uint16(obj.EPWMXA_tbctr_XCMP4_SHADOW2Enum), 'epwmxa_tbctr_xcmp5_shadow2', uint16(obj.EPWMXA_tbctr_XCMP5_SHADOW2Enum), 'epwmxa_tbctr_xcmp6_shadow2', uint16(obj.EPWMXA_tbctr_XCMP6_SHADOW2Enum), 'epwmxa_tbctr_xcmp7_shadow2', uint16(obj.EPWMXA_tbctr_XCMP7_SHADOW2Enum), 'epwmxa_tbctr_xcmp8_shadow2', uint16(obj.EPWMXA_tbctr_XCMP8_SHADOW2Enum), 'epwmxb_tbctr_xcmp5_shadow2', uint16(obj.EPWMXB_tbctr_XCMP5_SHADOW2Enum), 'epwmxb_tbctr_xcmp6_shadow2', uint16(obj.EPWMXB_tbctr_XCMP6_SHADOW2Enum), 'epwmxb_tbctr_xcmp7_shadow2', uint16(obj.EPWMXB_tbctr_XCMP7_SHADOW2Enum), 'epwmxb_tbctr_xcmp8_shadow2', uint16(obj.EPWMXB_tbctr_XCMP8_SHADOW2Enum), 'epwmxa_tbctr_xcmp1_shadow3', uint16(obj.EPWMXA_tbctr_XCMP1_SHADOW3Enum), 'epwmxa_tbctr_xcmp2_shadow3', uint16(obj.EPWMXA_tbctr_XCMP2_SHADOW3Enum), 'epwmxa_tbctr_xcmp3_shadow3', uint16(obj.EPWMXA_tbctr_XCMP3_SHADOW3Enum), 'epwmxa_tbctr_xcmp4_shadow3', uint16(obj.EPWMXA_tbctr_XCMP4_SHADOW3Enum), 'epwmxa_tbctr_xcmp5_shadow3', uint16(obj.EPWMXA_tbctr_XCMP5_SHADOW3Enum), 'epwmxa_tbctr_xcmp6_shadow3', uint16(obj.EPWMXA_tbctr_XCMP6_SHADOW3Enum), 'epwmxa_tbctr_xcmp7_shadow3', uint16(obj.EPWMXA_tbctr_XCMP7_SHADOW3Enum), 'epwmxa_tbctr_xcmp8_shadow3', uint16(obj.EPWMXA_tbctr_XCMP8_SHADOW3Enum), 'epwmxb_tbctr_xcmp5_shadow3', uint16(obj.EPWMXB_tbctr_XCMP5_SHADOW3Enum), 'epwmxb_tbctr_xcmp6_shadow3', uint16(obj.EPWMXB_tbctr_XCMP6_SHADOW3Enum), 'epwmxb_tbctr_xcmp7_shadow3', uint16(obj.EPWMXB_tbctr_XCMP7_SHADOW3Enum), 'epwmxb_tbctr_xcmp8_shadow3', uint16(obj.EPWMXB_tbctr_XCMP8_SHADOW3Enum));
                coder.cstructname(epwm_xcmp_struct, 'XCMPStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_xcmp_setup', coder.ref(epwm_xcmp_struct));
            end
        end
        
        
        function stepImpl(obj, varargin)  %#ok<INUSD>

           if isempty(coder.target)
            % Place simulation output code here 
           else
            % Call C-function implementing device output
            %coder.ceval('sink_output',u);
                
                value = 1;
                if(isequal(obj.SpecifyTimerPeriod, 'Input Port'))
                    coder.ceval('EPWM_setTimeBasePeriod', obj.baseaddr, varargin{value});
                    value = value + 1;
                else
                    coder.ceval('EPWM_setTimeBasePeriod', obj.baseaddr, obj.Timer_Period);
                end
                if(isequal(obj.SpecifyCMPAVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPAModule, varargin{value});
                        value = value + 1;
                else
                     coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPAModule, obj.CMPAValue);
                end
                if(isequal(obj.SpecifyCMPBVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPBModule, varargin{value});
                        value = value + 1;
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPBModule, obj.CMPBValue);
                end
                if(isequal(obj.SpecifyCMPCVia, 'Input Port'))
                       coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPCModule, varargin{value});
                        value = value + 1;
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPCModule, obj.CMPCValue);
                end
                if(isequal(obj.SpecifyCMPDVia, 'Input Port'))
                        coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPDModule, varargin{value});
                        value = value + 1;
                else
                    coder.ceval('EPWM_setCounterCompareValue', obj.baseaddr, obj.CounterCMPDModule, obj.CMPDValue);
                end
               
                if(obj.Enable_XCMP_Mode)
                    if(isequal(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 0, varargin{value}(1));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 4, varargin{value}(2));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 8, varargin{value}(3));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 12, varargin{value}(4));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 16, varargin{value}(5));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 20, varargin{value}(6));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 24, varargin{value}(7));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 28, varargin{value}(8));
                            value = value + 1;
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 0, obj.XCMP1_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 4, obj.XCMP2_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 8, obj.XCMP3_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 12, obj.XCMP4_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 16, obj.XCMP5_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 20, obj.XCMP6_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 24, obj.XCMP7_Active);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 28, obj.XCMP8_Active);
    
                    end
                end
                if(obj.Enable_XCMP_Mode)
                    if(isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 128, varargin{value}(1));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 132, varargin{value}(2));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 136, varargin{value}(3));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 140, varargin{value}(4));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 144, varargin{value}(5));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 148, varargin{value}(6));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 152, varargin{value}(7));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 156, varargin{value}(8));
                            value = value + 1;
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 128, obj.XCMP1_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 132, obj.XCMP2_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 136, obj.XCMP3_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 140, obj.XCMP4_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 144, obj.XCMP5_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 148, obj.XCMP6_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 152, obj.XCMP7_SHADOW1);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 156, obj.XCMP8_SHADOW1);
    
                    end
                end

                if(obj.Enable_XCMP_Mode)
                    if(isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 256, varargin{value}(1));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 260, varargin{value}(2));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 264, varargin{value}(3));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 268, varargin{value}(4));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 272, varargin{value}(5));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 276, varargin{value}(6));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 280, varargin{value}(7));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 284, varargin{value}(8));
                            value = value + 1;
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 256, obj.XCMP1_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 260, obj.XCMP2_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 264, obj.XCMP3_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 268, obj.XCMP4_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 272, obj.XCMP5_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 276, obj.XCMP6_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 280, obj.XCMP7_SHADOW2);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 284, obj.XCMP8_SHADOW2);
    
                    end
                end

                if(obj.Enable_XCMP_Mode)
                    if(isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 384, varargin{value}(1));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 388, varargin{value}(2));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 392, varargin{value}(3));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 396, varargin{value}(4));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 400, varargin{value}(5));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 404, varargin{value}(6));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 408, varargin{value}(7));
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 412, varargin{value}(8));
                            value = value + 1;
                    else
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 384, obj.XCMP1_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 388, obj.XCMP2_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 392, obj.XCMP3_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 396, obj.XCMP4_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 400, obj.XCMP5_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 404, obj.XCMP6_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 408, obj.XCMP7_SHADOW3);
                            coder.ceval('EPWM_setXCMPRegValue', obj.baseaddr, 412, obj.XCMP8_SHADOW3);
    
                    end
                end
               
                epwm_step_struct = struct('base', obj.baseaddr, 'counterMode', uint16(obj.CounterModeEnum), 'phaseShiftLoadEnable', obj.Phase_Shift_Load_Enable , 'phaseCount', uint16(obj.Phase_Shift_Value), 'HighResolutionCMPA', obj.High_Resolution_CMPA, 'HighResolutionCMPB', obj.High_Resolution_CMPB, 'Enable_HRPWMSupport', obj.Enable_HRPWM_Support, 'ChannelAEdgeMode', uint16(obj.HRPWMChannelAEdgeModeEnum) , 'ChannelBEdgeMode', uint16(obj.HRPWMChannelBEdgeModeEnum), 'ControlModeChannelA', uint16(obj.ControlModeChannelAEnum), 'ControlModeChannelB', uint16(obj.ControlModeChannelBEnum), 'hrPhaseCount', obj.HRPWM_High_Resolution_Phase, 'HighResolutionPhaseEnable', obj.High_Resolution_Phase_Load_Enable, 'Enable_HRPWMHighResolutionPeriodLoad', obj.HRPWM_High_Resolution_Period_Enable, 'hrPeriodCount', obj.HRPWM_High_Resolution_Period, 'Enable_AutomaticHRMSTEPScaling', obj.Enable_Automatic_HRMSTEP_Scaling, 'Enable_SFOLibrary', obj.Enable_SFO, 'Enable_XCMP', obj.Enable_XCMP_Mode, 'shadow_level', uint16(obj.Shadow_LevelEnum));
                coder.cstructname(epwm_step_struct, 'EPWMStepStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_step', coder.ref(epwm_step_struct));
           end
        end

            
        function releaseImpl(obj) %#ok<MANU>
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('sink_terminate');
                epwm_release_struct = struct('base', obj.baseaddr);
                coder.cstructname(epwm_release_struct, 'EPWMReleaseStruct', 'extern', 'HeaderFile', 'MW_EPWM.h');
                coder.ceval('epwm_release', coder.ref(epwm_release_struct));
            end
        end

        function flag = isInactivePropertyImpl(obj,propertyName)

            if (strcmp(propertyName,'Phase_Shift_Value') && (obj.Phase_Shift_Load_Enable==false))
                flag=true;
            elseif (strcmp(propertyName, 'SHDWAMODE') && (obj.Enable_Shadow_Load_CMPA==false))
                flag=true;
            elseif (strcmp(propertyName, 'SHDWBMODE') && (obj.Enable_Shadow_Load_CMPB==false))
                flag=true;
            elseif (strcmp(propertyName, 'SHDWCMODE') && (obj.Enable_Shadow_Load_CMPC==false))
                flag=true;
            elseif (strcmp(propertyName, 'SHDWDMODE') && (obj.Enable_Shadow_Load_CMPD==false))
                flag=true;
            elseif (strcmp(propertyName, 'ePWMXAShadowLoadEvent') && (obj.Enable_ePWMXA_Shadow_Mode==false))
                flag=true;
            elseif (strcmp(propertyName, 'ePWMXBShadowLoadEvent') && (obj.Enable_ePWMXB_Shadow_Mode==false))
                flag=true;
             elseif (strcmp(propertyName,'TZAEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'TZBEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'DCAEVT1Event') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'DCAEVT2Event') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'DCBEVT1Event') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'DCBEVT2Event') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==true))
                flag = true;
            elseif (strcmp(propertyName,'TZBDEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag = true;
            elseif (strcmp(propertyName,'TZBUEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag = true;
            elseif (strcmp(propertyName,'TZADEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'TZAUEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;  
            elseif (strcmp(propertyName,'DCAEVT1UEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;  
            elseif (strcmp(propertyName,'DCAEVT1DEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCAEVT2UEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCAEVT2DEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCBEVT1UEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCBEVT1DEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCBEVT2UEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'DCBEVT2DEvent') && (obj.Use_Advanced_EPWM_Trip_Zone_Actions==false))
                flag= true;
            elseif (strcmp(propertyName,'BlankingWindowStartEvent') && (obj.Blanking_Window_Usage==false))
                flag= true;
            elseif(strcmp(propertyName,'Blanking_Window_Offset') && (obj.Blanking_Window_Usage==false))
                flag=true;
            elseif(strcmp(propertyName,'Blanking_Window_Length') && (obj.Blanking_Window_Usage==false))
                flag=true;
            elseif(strcmp(propertyName,'Invert_Blanking_Window') && (obj.Blanking_Window_Usage==false))
                flag = true;
            elseif((strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRZero') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRPrd') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPA_Inc') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPA_Dec') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPB_Inc') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPB_Dec') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPC_Inc')|| strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPC_Dec') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPD_Inc') || strcmp(propertyName, 'BlankingWindow_MixedEvent_TBCTRCMPD_Dec')) && (isequal(obj.BlankingWindowStartEvent, 'Time base counter equals period') || isequal(obj.BlankingWindowStartEvent, 'Time base counter equals zero') || isequal(obj.BlankingWindowStartEvent, 'Time base counter equals zero or period')))
                flag=true;
            elseif(strcmp(propertyName,'Enable_Counter_Capture_Shadow_Mode') && (obj.Enable_DC_Counter_Capture==false))
                flag=true;
            elseif(strcmp(propertyName,'DCCounterCaptureEnableEvent') && (obj.Enable_DC_Counter_Capture==false))
                flag=true;
            elseif(strcmp(propertyName, 'EdgeFilterMode') && (obj.Use_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'EdgeFilterEdgeCount') && (obj.Use_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'Enable_Edge_Filter_Valley_Capture') && (obj.Use_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'Select_Delayed_Output_of_Edge_Filter') && (obj.Use_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'EdgeFilterCounterCaptureSignal') && (obj.Enable_Edge_Filter_Valley_Capture==false))
                flag=true;
            elseif(strcmp(propertyName, 'Start_Valley_Capture_Logic') && (obj.Enable_Edge_Filter_Valley_Capture==false))
                flag=true;
            elseif(strcmp(propertyName, 'StartValleyCapture') && (obj.Enable_Edge_Filter_Valley_Capture==false))
                flag=true;
            elseif(strcmp(propertyName, 'StopValleyCapture') && (obj.Enable_Edge_Filter_Valley_Capture==false))
                flag=true;
            elseif(strcmp(propertyName, 'Software_Valley_Delay_Value') && (obj.Select_Delayed_Output_of_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'ValleyDelayDivider') && (obj.Select_Delayed_Output_of_Edge_Filter==false))
                flag=true;
            elseif(strcmp(propertyName, 'Capture_Input') && (obj.Use_Event_Detection==false))
                flag=true;
            elseif(strcmp(propertyName, 'Invert_Capture_Input_Polarity') && (obj.Use_Event_Detection==false))
                flag=true;
            elseif(strcmp(propertyName, 'Capture_gate') && (obj.Use_Event_Detection==false))
                flag = true;
            elseif(strcmp(propertyName, 'Capture_gate_polarity_select') && (obj.Use_Event_Detection==false))
                flag = true;   
            elseif((strcmp(propertyName, 'Combinational_capture_input_Trip1') || strcmp(propertyName, 'Combinational_capture_input_Trip2') || strcmp(propertyName, 'Combinational_capture_input_Trip3') || strcmp(propertyName, 'Combinational_capture_input_Trip4') || strcmp(propertyName, 'Combinational_capture_input_Trip5') || strcmp(propertyName, 'Combinational_capture_input_Trip6') || strcmp(propertyName, 'Combinational_capture_input_Trip7') || strcmp(propertyName, 'Combinational_capture_input_Trip8') || strcmp(propertyName, 'Combinational_capture_input_Trip9') || strcmp(propertyName, 'Combinational_capture_input_Trip10') || strcmp(propertyName, 'Combinational_capture_input_Trip11') || strcmp(propertyName, 'Combinational_capture_input_Trip12') || strcmp(propertyName, 'Combinational_capture_input_Trip13') || strcmp(propertyName, 'Combinational_capture_input_Trip14') || strcmp(propertyName, 'Combinational_capture_input_Trip15')) && (obj.Use_Event_Detection==false || ~(isequal(obj.Capture_Input, 'All Trips'))))
                flag=true;
            elseif((strcmp(propertyName, 'Combinational_capture_gate_Trip1') || strcmp(propertyName, 'Combinational_capture_gate_Trip2') || strcmp(propertyName, 'Combinational_capture_gate_Trip3') || strcmp(propertyName, 'Combinational_capture_gate_Trip4') || strcmp(propertyName, 'Combinational_capture_gate_Trip5') || strcmp(propertyName, 'Combinational_capture_gate_Trip6') || strcmp(propertyName, 'Combinational_capture_gate_Trip7') || strcmp(propertyName, 'Combinational_capture_gate_Trip8') || strcmp(propertyName, 'Combinational_capture_gate_Trip9') || strcmp(propertyName, 'Combinational_capture_gate_Trip10') || strcmp(propertyName, 'Combinational_capture_gate_Trip11') || strcmp(propertyName, 'Combinational_capture_gate_Trip12') || strcmp(propertyName, 'Combinational_capture_gate_Trip13') || strcmp(propertyName, 'Combinational_capture_gate_Trip14') || strcmp(propertyName, 'Combinational_capture_gate_Trip15')) && (obj.Use_Event_Detection==false || ~(isequal(obj.Capture_gate, 'All Trips'))))
                flag=true;
            elseif(strcmp(propertyName, 'Rising_Edge_Delay_Value') && (obj.Enable_Rising_Edge_Delay==false))
                flag=true;
            elseif(strcmp(propertyName,'Falling_Edge_Delay_Value') && (obj.Enable_Falling_Edge_Delay==false))
                flag=true;
            elseif(strcmp(propertyName,'DeadbandControlShadowMode') && (obj.EnableDeadbandControlShadowMode==false))
                flag=true;
            elseif(strcmp(propertyName,'REDShadowLoadEvent') && (obj.Enable_RED_Shadow_Mode==false))
                flag=true;
            elseif(strcmp(propertyName,'FEDShadowLoadEvent') && (obj.Enable_FED_Shadow_Mode==false))
                flag=true;
            elseif(strcmp(propertyName,'ChopperDutyCycle') && (obj.Use_Chopper==false))
                flag=true;
            elseif(strcmp(propertyName,'ChopperFrequency') && (obj.Use_Chopper==false))
                flag=true;
            elseif(strcmp(propertyName,'ChopperFirstPulseWidth') && (obj.Use_Chopper==false))
                flag=true;
            elseif(strcmp(propertyName, 'InterruptEventSources') && (obj.Enable_EPWM_Interrupt==false))
                flag=true;
            elseif((strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRZero') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRPrd') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPA_Inc') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPA_Dec') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPB_Inc') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPB_Dec') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPC_Inc')|| strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPC_Dec') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPD_Inc') || strcmp(propertyName, 'EventTrigger_MixedEvent_TBCTRCMPD_Dec') || strcmp(propertyName, 'EventTrigger_MixedEvent_DCxEVT1')) && (~isequal(obj.InterruptEventSources, 'Time base counter based on mixed events')))
                flag=true;
            elseif(strcmp(propertyName, 'InterruptEventCount') && (obj.Enable_EPWM_Interrupt==false))
                flag=true;
            elseif(strcmp(propertyName, 'Enable_Interrupt_Event_Count_Initial_Value') && (obj.Enable_EPWM_Interrupt==false))
                flag=true;
            elseif(strcmp(propertyName, 'InterruptEventCountInitialValue') && (obj.Enable_Interrupt_Event_Count_Initial_Value==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCATriggerSource') && (obj.SOCA_Trigger_Enable==false))
                flag=true;
             elseif((strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRZero') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRPrd') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPA_Inc') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPA_Dec') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPB_Inc') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPB_Dec') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPC_Inc')|| strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPC_Dec') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPD_Inc') || strcmp(propertyName, 'SOCATrigger_MixedEvent_TBCTRCMPD_Dec') || strcmp(propertyName, 'SOCATrigger_MixedEvent_DCxEVT1')) && (~isequal(obj.SOCATriggerSource, 'Time base counter equal to mixed event')))
                flag=true;
            elseif(strcmp(propertyName, 'SOCATriggerEventCount') && (obj.SOCA_Trigger_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCA_Trigger_Event_Count_Init_Enable') && (obj.SOCA_Trigger_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCATriggerEventCountInitValue') && (obj.SOCA_Trigger_Event_Count_Init_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCBTriggerSource') && (obj.SOCB_Trigger_Enable==false))
                flag=true;
            elseif((strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRZero') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRPrd') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPA_Inc') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPA_Dec') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPB_Inc') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPB_Dec') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPC_Inc')|| strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPC_Dec') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPD_Inc') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_TBCTRCMPD_Dec') || strcmp(propertyName, 'SOCBTrigger_MixedEvent_DCxEVT1')) && (~isequal(obj.SOCBTriggerSource, 'Time base counter equal to mixed event')))
                flag=true;
            elseif(strcmp(propertyName, 'SOCBTriggerEventCount') && (obj.SOCB_Trigger_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCB_Trigger_Event_Count_Init_Enable') && (obj.SOCB_Trigger_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'SOCBTriggerEventCountInitValue') && (obj.SOCB_Trigger_Event_Count_Init_Enable==false))
                flag=true;
            elseif(strcmp(propertyName, 'Enable_Automatic_HRMSTEP_Scaling') && (obj.Enable_HRPWM_Support==false))
                flag=true;
            elseif(strcmp(propertyName, 'Enable_SFO') && ((obj.Enable_HRPWM_Support==false) || (obj.Enable_Automatic_HRMSTEP_Scaling==false)))
                flag = true;
            elseif(strcmp(propertyName, 'MEP_Scale') && ((obj.Enable_HRPWM_Support==false) || (obj.Enable_Automatic_HRMSTEP_Scaling==false) || (obj.Enable_SFO==false)))
                flag = true;
            elseif(strcmp(propertyName, 'ControlModeChannelA') && (obj.Enable_HRPWM_Support==false))
                flag=true;
            elseif(strcmp(propertyName, 'ControlModeChannelB') && (obj.Enable_HRPWM_Support==false))
                flag=true;
            elseif(strcmp(propertyName, 'High_Resolution_CMPA') && (isequal(obj.HRPWMChannelAEdgeMode, 'HRPWM is disabled')))
                flag = true;
            elseif(strcmp(propertyName, 'CMPAHRShadowToActiveLoadEvent') && (isequal(obj.HRPWMChannelAEdgeMode, 'HRPWM is disabled')))
                flag = true;
            elseif(strcmp(propertyName, 'High_Resolution_CMPB') && (isequal(obj.HRPWMChannelBEdgeMode, 'HRPWM is disabled')))
                flag = true;
            elseif(strcmp(propertyName, 'CMPBHRShadowToActiveLoadEvent') && (isequal(obj.HRPWMChannelBEdgeMode, 'HRPWM is disabled')))
                flag = true;
            elseif(strcmp(propertyName, 'HRPWM_High_Resolution_Period') && (obj.HRPWM_High_Resolution_Period_Enable==false))
                flag = true;
            elseif(strcmp(propertyName, 'HRPWMDeadbandEdgeMode') && (obj.Enable_HRPWM_Support==false))
                flag = true;
            elseif((strcmp(propertyName, 'HRPWM_High_Resolution_RED') || strcmp(propertyName, 'ShadowActiveLoadDBREDHR')) && ((obj.Enable_HRPWM_Support==false) || (isequal(obj.HRPWMDeadbandEdgeMode, 'HRPWM is disabled') || isequal(obj.HRPWMDeadbandEdgeMode, 'MEP controls falling edge delay'))))
                flag = true;
            elseif((strcmp(propertyName, 'HRPWM_High_Resolution_FED') || strcmp(propertyName, 'ShadowActiveLoadDBFEDHR')) && ((obj.Enable_HRPWM_Support==false) || (isequal(obj.HRPWMDeadbandEdgeMode, 'HRPWM is disabled') || isequal(obj.HRPWMDeadbandEdgeMode, 'MEP controls rising edge delay'))))
                flag = true;
            elseif((strcmp(propertyName, 'Swap_A_B_Outputs') || strcmp(propertyName, 'EPWMxBOutputSelect')) && ((obj.Enable_HRPWM_Support==false) || isequal(obj.HRPWMDeadbandEdgeMode, 'HRPWM is disabled')))
                flag = true;
            elseif((strcmp(propertyName, 'High_Resolution_Phase_Load_Enable') || strcmp(propertyName, 'HRPWM_High_Resolution_Phase')) && (isequal(obj.ControlModeChannelA, 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge') && isequal(obj.ControlModeChannelB, 'CMPAHR/CMPBHR or TBPRDHR controls MEP edge')))
                flag = true;
            elseif(strcmp(propertyName, 'Enable_Split_XCMP') && (obj.Enable_XCMP_Mode==false))
                flag = true;
            elseif(strcmp(propertyName, 'Allocate_XCMP_CMPA') && (obj.Enable_XCMP_Mode==false))
                flag = true;
            elseif(strcmp(propertyName, 'Allocate_XCMP_CMPB') && ((obj.Enable_XCMP_Mode==false) || (obj.Enable_Split_XCMP==false)))
                flag = true;
            elseif(strcmp(propertyName, 'SpecifyXCMPActive') && (obj.Enable_XCMP_Mode == false))
                flag = true;
            elseif(strcmp(propertyName, 'SpecifyXCMPShadow1') && ((obj.Enable_XCMP_Mode == false) || isequal(obj.Shadow_Level, 'Shadow level zero')))
                flag = true;
            elseif(strcmp(propertyName, 'SpecifyXCMPShadow2') && ((obj.Enable_XCMP_Mode == false) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif(strcmp(propertyName, 'SpecifyXCMPShadow3') && ((obj.Enable_XCMP_Mode == false) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA'))))
                flag = true;
             elseif((strcmp(propertyName, 'XCMP1_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA'))))
                flag = true;
             elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP1_active')) && ((obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP1_SHADOW1')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP2_active')) && ((obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP2_SHADOW1')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP3_active')) && ((obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP3_SHADOW1')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP4_active')) && ((obj.Enable_XCMP_Mode == false) || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP4_SHADOW1')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA')))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP5_active') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP5_active')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP5_SHADOW1') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP5_SHADOW1')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP6_active') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP6_active')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP6_SHADOW1') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP6_SHADOW1')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP7_active') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP7_active')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP7_SHADOW1') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP7_SHADOW1')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_Active')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_Active_Initial')) && (isequal(obj.SpecifyXCMPActive, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP8_active') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP8_active')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB')))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW1')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW1_Initial')) && (isequal(obj.SpecifyXCMPShadow1, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP8_SHADOW1') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP8_SHADOW1')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || (isequal(obj.Shadow_Level, 'Shadow level zero'))))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP1_SHADOW2')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP2_SHADOW2')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP3_SHADOW2')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP4_SHADOW2')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP5_SHADOW2') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP5_SHADOW2')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif(( strcmp(propertyName, 'EPWMXA_tbctr_XCMP6_SHADOW2') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP6_SHADOW2')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP7_SHADOW2') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP7_SHADOW2')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW2')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW2_Initial')) && (isequal(obj.SpecifyXCMPShadow2, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP8_SHADOW2') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP8_SHADOW2')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP1_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP1_SHADOW3')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP2_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP2_SHADOW3')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP3_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP3_SHADOW3')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP4_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP4_SHADOW3')) && ((obj.Enable_XCMP_Mode == false)  || (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA')) || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP5_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP5_SHADOW3') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP5_SHADOW3')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA'))) || (isequal(obj.Shadow_Level, 'Shadow level zero')) || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP6_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP6_SHADOW3') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP6_SHADOW3')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP7_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP7_SHADOW3') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP7_SHADOW3')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW3')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Input Port') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XCMP8_SHADOW3_Initial')) && (isequal(obj.SpecifyXCMPShadow3, 'Specify via Dialog Box') || (obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'EPWMXA_tbctr_XCMP8_SHADOW3') || strcmp(propertyName, 'EPWMXB_tbctr_XCMP8_SHADOW3')) && ((obj.Enable_XCMP_Mode == false) || ((obj.Enable_Split_XCMP==false) && (isequal(obj.Allocate_XCMP_CMPA, 'Allocate 0 XCMP registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1 register to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP2 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP3 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP4 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP5 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP6 registers to CMPA') || isequal(obj.Allocate_XCMP_CMPA, 'Allocate XCMP1-XCMP7 registers to CMPA'))) || ((obj.Enable_Split_XCMP==true) && (isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5 register to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP6 registers to CMPB') || isequal(obj.Allocate_XCMP_CMPB, 'Allocate XCMP5-XCMP7 registers to CMPB'))) || isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two')))
                flag = true;
            elseif((strcmp(propertyName, 'XMIN_Shadow1') || strcmp(propertyName, 'XMAX_Shadow1') || strcmp(propertyName, 'XTBPRD_Shadow1')) && (isequal(obj.Shadow_Level, 'Shadow level zero') || (obj.Enable_XCMP_Mode == false)))
                flag = true;
            elseif((strcmp(propertyName, 'XMIN_Shadow2') || strcmp(propertyName, 'XMAX_Shadow2') || strcmp(propertyName, 'XTBPRD_Shadow2')) && (isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || (obj.Enable_XCMP_Mode == false)))
                flag = true;
            elseif((strcmp(propertyName, 'XMIN_Shadow3') || strcmp(propertyName, 'XMAX_Shadow3') || strcmp(propertyName, 'XTBPRD_Shadow3')) && (isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two') || (obj.Enable_XCMP_Mode == false)))
                flag = true;
            elseif(strcmp(propertyName, 'Repeat_Count_Shadow_Buffer2') && (isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || (obj.Enable_XCMP_Mode == false)))
                flag = true;
            elseif(strcmp(propertyName, 'Repeat_Count_Shadow_Buffer3') && (isequal(obj.Shadow_Level, 'Shadow level zero') || isequal(obj.Shadow_Level, 'Shadow level one') || isequal(obj.Shadow_Level, 'Shadow level two') || (obj.Enable_XCMP_Mode == false)))
                flag = true;
            elseif((strcmp(propertyName, 'XMIN_Active') || strcmp(propertyName, 'XMAX_Active') || strcmp(propertyName, 'XTBPRD_Active') || strcmp(propertyName, 'Load_Mode') || strcmp(propertyName, 'Shadow_Level') || strcmp(propertyName, 'Shadow_register_load_once')) && (obj.Enable_XCMP_Mode==false))
                flag = true;
            elseif((strcmp(propertyName, 'ePWMLinkCMPA')) && (obj.Enable_LinkCounterCMPA == false))
                flag = true;
            elseif((strcmp(propertyName, 'ePWMLinkCMPB')) && (obj.Enable_LinkCounterCMPB == false))
                flag = true;
            elseif((strcmp(propertyName, 'ePWMLinkCMPC')) && (obj.Enable_LinkCounterCMPC == false))
                flag = true;
            elseif((strcmp(propertyName, 'ePWMLinkCMPD')) && (obj.Enable_LinkCounterCMPD == false))
                flag = true;
            elseif(strcmp(propertyName, 'CMPA_Initial_Value') && isequal(obj.SpecifyCMPAVia, 'Specify Via Dialog Box'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPAValue') && isequal(obj.SpecifyCMPAVia, 'Input Port'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPB_Initial_Value') && isequal(obj.SpecifyCMPBVia, 'Specify Via Dialog Box'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPBValue') && isequal(obj.SpecifyCMPBVia, 'Input Port'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPC_Initial_Value') && isequal(obj.SpecifyCMPCVia, 'Specify Via Dialog Box'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPCValue') && isequal(obj.SpecifyCMPCVia, 'Input Port'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPD_Initial_Value') && isequal(obj.SpecifyCMPDVia, 'Specify Via Dialog Box'))
                flag = true;
            elseif(strcmp(propertyName, 'CMPDValue') && isequal(obj.SpecifyCMPDVia, 'Input Port'))
                flag = true;
            elseif(strcmp(propertyName, 'Timer_Period') && isequal(obj.SpecifyTimerPeriod, 'Input Port'))
                flag = true;
            elseif(strcmp(propertyName, 'Timer_Initial_Period') && isequal(obj.SpecifyTimerPeriod, 'Specify Via Dialog Box'))
                flag = true;
            elseif((strcmp(propertyName, 'DCAHigh_CombTrip1_Input') | strcmp(propertyName, 'DCAHigh_CombTrip2_Input') | strcmp(propertyName, 'DCAHigh_CombTrip3_Input') | strcmp(propertyName, 'DCAHigh_CombTrip4_Input') | strcmp(propertyName, 'DCAHigh_CombTrip5_Input') | strcmp(propertyName, 'DCAHigh_CombTrip6_Input') | strcmp(propertyName, 'DCAHigh_CombTrip7_Input') | strcmp(propertyName, 'DCAHigh_CombTrip8_Input') | strcmp(propertyName, 'DCAHigh_CombTrip9_Input') | strcmp(propertyName, 'DCAHigh_CombTrip10_Input') | strcmp(propertyName, 'DCAHigh_CombTrip11_Input') | strcmp(propertyName, 'DCAHigh_CombTrip12_Input') | strcmp(propertyName, 'DCAHigh_CombTrip13_Input') | strcmp(propertyName, 'DCAHigh_CombTrip14_Input') | strcmp(propertyName, 'DCAHigh_CombTrip15_Input')) && (~(isequal(obj.DCAHigh, 'All Trips'))))
                flag = true;
            elseif((strcmp(propertyName, 'DCALow_CombTrip1_Input') | strcmp(propertyName, 'DCALow_CombTrip2_Input') | strcmp(propertyName, 'DCALow_CombTrip3_Input') | strcmp(propertyName, 'DCALow_CombTrip4_Input') | strcmp(propertyName, 'DCALow_CombTrip5_Input') | strcmp(propertyName, 'DCALow_CombTrip6_Input') | strcmp(propertyName, 'DCALow_CombTrip7_Input') | strcmp(propertyName, 'DCALow_CombTrip8_Input') | strcmp(propertyName, 'DCALow_CombTrip9_Input') | strcmp(propertyName, 'DCALow_CombTrip10_Input') | strcmp(propertyName, 'DCALow_CombTrip11_Input') | strcmp(propertyName, 'DCALow_CombTrip12_Input') | strcmp(propertyName, 'DCALow_CombTrip13_Input') | strcmp(propertyName, 'DCALow_CombTrip14_Input') | strcmp(propertyName, 'DCALow_CombTrip15_Input')) && (~(isequal(obj.DCALow, 'All Trips'))))
                flag = true;
            elseif((strcmp(propertyName, 'DCBHigh_CombTrip1_Input') | strcmp(propertyName, 'DCBHigh_CombTrip2_Input') | strcmp(propertyName, 'DCBHigh_CombTrip3_Input') | strcmp(propertyName, 'DCBHigh_CombTrip4_Input') | strcmp(propertyName, 'DCBHigh_CombTrip5_Input') | strcmp(propertyName, 'DCBHigh_CombTrip6_Input') | strcmp(propertyName, 'DCBHigh_CombTrip7_Input') | strcmp(propertyName, 'DCBHigh_CombTrip8_Input') | strcmp(propertyName, 'DCBHigh_CombTrip9_Input') | strcmp(propertyName, 'DCBHigh_CombTrip10_Input') | strcmp(propertyName, 'DCBHigh_CombTrip11_Input') | strcmp(propertyName, 'DCBHigh_CombTrip12_Input') | strcmp(propertyName, 'DCBHigh_CombTrip13_Input') | strcmp(propertyName, 'DCBHigh_CombTrip14_Input') | strcmp(propertyName, 'DCBHigh_CombTrip15_Input')) && (~(isequal(obj.DCBHigh, 'All Trips'))))
                flag = true;
            elseif((strcmp(propertyName, 'DCBLow_CombTrip1_Input') | strcmp(propertyName, 'DCBLow_CombTrip2_Input') | strcmp(propertyName, 'DCBLow_CombTrip3_Input') | strcmp(propertyName, 'DCBLow_CombTrip4_Input') | strcmp(propertyName, 'DCBLow_CombTrip5_Input') | strcmp(propertyName, 'DCBLow_CombTrip6_Input') | strcmp(propertyName, 'DCBLow_CombTrip7_Input') | strcmp(propertyName, 'DCBLow_CombTrip8_Input') | strcmp(propertyName, 'DCBLow_CombTrip9_Input') | strcmp(propertyName, 'DCBLow_CombTrip10_Input') | strcmp(propertyName, 'DCBLow_CombTrip11_Input') | strcmp(propertyName, 'DCBLow_CombTrip12_Input') | strcmp(propertyName, 'DCBLow_CombTrip13_Input') | strcmp(propertyName, 'DCBLow_CombTrip14_Input') | strcmp(propertyName, 'DCBLow_CombTrip15_Input')) && (~(isequal(obj.DCBLow, 'All Trips'))))
                flag = true;
            else
                flag = false;
            
            end
        end

        function InputPorts = getNumInputsImpl(obj)
            InputPorts = 0;
            if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyCMPAVia, 'Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyCMPBVia, 'Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyCMPCVia, 'Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyCMPDVia, 'Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                InputPorts = InputPorts + 1;
            end
            if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                InputPorts = InputPorts + 1;
            end
        end
        function varargout = getInputNamesImpl(obj)
            index = 1;
            num = getNumInputs(obj);
        if num > 0
            varargout = cell(1,num);
            if (num == 1)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                    varargout{index} = 'TBPRD';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyCMPAVia, 'Input Port'))
                    varargout{index} = 'CMPA';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyCMPBVia, 'Input Port'))
                    varargout{index} = 'CMPB';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyCMPCVia, 'Input Port'))
                    varargout{index} = 'CMPC';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyCMPDVia, 'Input Port'))
                    varargout{index} = 'CMPD';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                    varargout{index} = 'XCMP Active';
                    index = index + 1;
                elseif(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                    varargout{index} = 'XCMP SHDW1'; 
                    index = index + 1;
                elseif(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                    varargout{index} = 'XCMP SHDW2'; 
                    index = index + 1;
                elseif(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                    varargout{index} = 'XCMP SHDW3'; 
                    index = index + 1;    
                end
            elseif (num == 2)
               if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
            elseif (num == 3)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
           elseif (num == 4)
               if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
           elseif (num ==5)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
            elseif (num==6)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
            elseif(num==7)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
            elseif (n==8)
                if(strcmp(obj.SpecifyTimerPeriod, 'Input Port'))
                   varargout{index} = 'TBPRD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPAVia,'Input Port'))
                   varargout{index} = 'CMPA';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPBVia,'Input Port'))
                   varargout{index} = 'CMPB';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPCVia,'Input Port'))
                   varargout{index} = 'CMPC';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyCMPDVia,'Input Port'))
                   varargout{index} = 'CMPD';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPActive, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Active';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow1, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow1';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow2, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow2';
                   index = index + 1;
               end
               if(strcmp(obj.SpecifyXCMPShadow3, 'Specify via Input Port'))
                   varargout{index} = 'XCMP Shadow3';
                   index = index + 1;
               end
             elseif (n==9)
                varargout{index} = 'TBPRD';
                index = index + 1;
                varargout{index} = 'CMPA';
                index = index + 1;
                varargout{index} = 'CMPB';
                index = index + 1;
                varargout{index} = 'CMPC';
                index = index + 1;
                varargout{index} = 'CMPD';
                index = index + 1;
                varargout{index} = 'XCMP ACTIVE';
                index = index + 1;
                varargout{index} = 'XCMP SHDW1';
                index = index + 1;
                varargout{index} = 'XCMP SHDW2';
                index = index + 1;
                varargout{index} = 'XCMP SHDW3';
            end
        end
        end

        function OutData = isOutputFixedSizeImpl(obj)
            OutData = true;
        end
        
        function OutputPorts = getNumOutputsImpl(obj)
            OutputPorts = 0;
        end
        
        function out = getOutputSizeImpl(obj) 
            % Return size for each output port
            out = 0;
        end

        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'EPWM','Type 5'};
        end
           
    end

            
            
methods (Static)
        function name = getDescriptiveName()
            name = 'EPWM';
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
                addIncludeFiles(buildInfo, 'MW_EPWM.h', includeDir);
                addSourceFiles(buildInfo, 'MW_EPWM.c', srcDir);
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

