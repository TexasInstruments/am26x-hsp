classdef ADC < matlab.System & ...
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
        
        % SOCx acquisition Window
        SOCx_acquisition_Window uint32=16;
        % PPB Calibration Offset
        PPB_Calibration_Offset=0
        % PPB Reference Offset
        PPB_Reference_Offset=0
        %PPB High Trip Limit
        PPB_High_Trip_Limit=0
        %PPB Low Trip Limit
        PPB_Low_Trip_Limit=0 
        
        
        
    end
    
    properties (Nontunable)
        % Public, non-tunable properties.
        
       %Sample Time
       SampleTime = 0.001;
       %ADC Module
       ADCModule = 'ADC0';
       %Prescaler
       Prescaler = '1.0';   
       % ADC Resolution Mode
       ResolutionMode = 'Twelve bit conversion';
       % ADC Signal Mode
       SignalMode = 'Sample on single pin with VREFLO';
       %SOC trigger number
       SOCTriggerNo = 'SOC0';
       %SOCx trigger source
       SOCxTriggerSource = 'Software Source';
       %ADCINT will trigger SOCx
       INTWillTriggerSOCx = 'No ADCINT Trigger';
       %Data type
       DataType = 'Double';
       % Post Interrupt at EOC trigger
       PostInterruptAtEOCTrigger (1, 1) logical = false
       % Interrupt selection
       InterruptSelection = 'ADC Interrupt Number 1';
       % Interrupt Source
       InterruptSOCSource = 'SOC0';
       % Enable ADC Converter
       ADCEnableConverter (1, 1) logical = false
       % ADCINT1 continousmode
       ADCINT1CONTINOUSMODE (1, 1) logical = false
       % ADCINT2 continousmode
       ADCINT2CONTINOUSMODE (1, 1) logical = false
       %ADCINT3 continousmode
       ADCINT3CONTINOUSMODE (1, 1) logical = false
       %ADCINT4 continousmode
       ADCINT4CONTINOUSMODE (1, 1) logical = false
       % Conversion Channels
       Channels = 'Channel 0';
       % Conversion Channels
       Channels1 ='Channels 0 and 1';
       % ADC SOC Priority
       SOCPriority  = 'All Round Robin';
       % ADC Pulse Mode
       PulseMode = 'Pulse generation at End of Acquisition Window';
      % ADC Event TRIP HIGH
      ADCEventTripHigh (1, 1) logical = false
      % ADC Event TRIP LOW
      ADCEventTripLow (1,1) logical= false
      %ADC Event Zero Crossing
      ADCEventZeroCrossing (1,1)logical= false
      % ADC Enable Interrupt Event TRIP HIGH
      ADCEnableInterruptEventTripHigh (1, 1) logical = false
      % ADC Enable Interrupt Event TRIP LOW
      ADCEnableInterruptEventTripLow (1,1) logical= false
      %ADC Enable Interrupt Event Zero Crossing
      ADCEnableInterruptEventZeroCrossing (1,1)logical= false  
      %  Enable interupt
      ADCEnableInterrupt (1,1) logical=false

      % PPB Block
      PPBBlock = 'PPB Block 1';
      % PPB SOC/EOC Number
      PPBSocEocNO = 'SOC0'
      % PPB Enable Two's complement
      PPBEnableTwoComplement (1,1) logical = false
      % PPB Enable PPB Event CBC Clear
      PPBEnablePPBEventCBCClear (1,1) logical = false
      %PPB Enable Event : ADC Event Trip High
      Enable_Event_ADC_Trip_High(1,1) logical = false
      %PPB Enable Event : ADC Event Trip Low
      Enable_Event_ADC_Trip_Low(1,1) logical = false
      %PPB Enable Event : ADC Event Zero Crossing
      Enable_Event_ADC_Zero_Crossing(1,1) logical = false
      %PPB Enable Interrupt Event : ADC Event Trip High
      Enable_Interrupt_Event_ADC_Trip_High(1,1) logical = false
      %PPB Enable Interrupt Event : ADC Event Trip Low
      Enable_Interrupt_Event_ADC_Trip_Low(1,1) logical = false
      %PPB Enable Interrupt Event : ADC Event Zero Crossing
      Enable_Interrupt_Event_ADC_Zero_Crossing(1,1) logical = false

      % Burst Mode Trigger Signal
      BurstModeTrigger = 'Software Source';
      % Burst Size 
      BurstSize = '1 SOC long';
      % Enable Burst Mode
      ADCEnableBurstMode (1,1) logical = false

    end
    
   properties (Access = private)
        % Pre-computed constants.
        ConfigBaseAddr=0;
        ResultBaseAddr=0;
        Delay=0;
   end

   properties(Constant,Hidden)
        ADCModuleSet = matlab.system.StringSet({'ADC0', 'ADC1', 'ADC2', 'ADC3', 'ADC4'});
        PrescalerSet = matlab.system.StringSet({'1.0', '2.0', '2.5', '3.0', '3.5', '4.0', '4.5', '5', '5.5', '6.0', '6.5', '7.0', '7.5', '8.0', '8.5'});
        ResolutionModeSet = matlab.system.StringSet({'Twelve bit conversion'});
        SignalModeSet = matlab.system.StringSet({'Sample on single pin with VREFLO', 'Sample on pair of pins'});
        SOCTriggerNoSet = matlab.system.StringSet({'SOC0', 'SOC1', 'SOC2', 'SOC3', 'SOC4', 'SOC5', 'SOC6', 'SOC7', 'SOC8', 'SOC9', 'SOC10', 'SOC11', 'SOC12', 'SOC13', 'SOC14', 'SOC15'});
        SOCxTriggerSourceSet = matlab.system.StringSet({'Software Source', 'RTI Timer0', 'RTI Timer1', 'RTI Timer2', 'RTI Timer3', 'Input XBAR Out5', 'EPWM0 SOCA', 'EPWM0 SOCB', 'EPWM1 SOCA', 'EPWM1 SOCB', 'EPWM2 SOCA', 'EPWM2 SOCB', 'EPWM3 SOCA', 'EPWM3 SOCB', 'EPWM4 SOCA', 'EPWM4 SOCB', 'EPWM5 SOCA', 'EPWM5 SOCB', 'EPWM6 SOCA', 'EPWM6 SOCB', 'EPWM7 SOCA', 'EPWM7 SOCB', 'EPWM8 SOCA', 'EPWM8 SOCB', 'EPWM9 SOCA', 'EPWM9 SOCB', 'EPWM10 SOCA', 'EPWM10 SOCB', 'EPWM11 SOCA', 'EPWM11 SOCB', 'EPWM12 SOCA', 'EPWM12 SOCB', 'EPWM13 SOCA', 'EPWM13 SOCB', 'EPWM14 SOCA', 'EPWM14 SOCB', 'EPWM15 SOCA', 'EPWM15 SOCB', 'EPWM16 SOCA', 'EPWM16 SOCB', 'EPWM17 SOCA', 'EPWM17 SOCB', 'EPWM18 SOCA', 'EPWM18 SOCB', 'EPWM19 SOCA', 'EPWM19 SOCB', 'EPWM20 SOCA', 'EPWM20 SOCB', 'EPWM21 SOCA', 'EPWM21 SOCB', 'EPWM22 SOCA', 'EPWM22 SOCB', 'EPWM23 SOCA', 'EPWM23 SOCB', 'EPWM24 SOCA', 'EPWM24 SOCB', 'EPWM25 SOCA', 'EPWM25 SOCB', 'EPWM26 SOCA','EPWM26 SOCB','EPWM27 SOCA', 'EPWM27 SOCB', 'EPWM28 SOCA', 'EPWM28 SOCB', 'EPWM29 SOCA', 'EPWM29 SOCB', 'EPWM30 SOCA', 'EPWM30 SOCB', 'EPWM31 SOCA','EPWM31 SOCB'});
        INTWillTriggerSOCxSet = matlab.system.StringSet({'No ADCINT Trigger', 'ADCINT1 will trigger SOC', 'ADCINT2 will trigger SOC'});
        DataTypeSet = matlab.system.StringSet({'Double', 'Single', 'INT8', 'Unsigned INT8', 'INT16', 'Unsigned INT16', 'INT32', 'Unsigned INT32'});
        InterruptSelectionSet = matlab.system.StringSet({'ADC Interrupt Number 1', 'ADC Interrupt Number 2', 'ADC Interrupt Number 3', 'ADC Interrupt Number 4'});
        ChannelsSet = matlab.system.StringSet({'Channel 0', 'Channel 1', 'Channel 2', 'Channel 3', 'Channel 4', 'Channel 5'});
        Channels1Set = matlab.system.StringSet({'Channels 0 and 1', 'Channels 1 and 0', 'Channels 2 and 3', 'Channels 3 and 2', 'Channels 4 and 5', 'Channels 5 and 4'});
        SOCPrioritySet = matlab.system.StringSet({'All Round Robin', 'SOC0 High Priority', 'SOC0-SOC1 High Priority', 'SOC0-SOC2 High Priority', 'SOC0-SOC3 High Priority', 'SOC0-SOC4 High Priority', 'SOC0-SOC5 High Priority', 'SOC0-SOC6 High Priority', 'SOC0-SOC7 High Priority', 'SOC0-SOC8 High Priority', 'SOC0-SOC9 High Priority', 'SOC0-SOC10 High Priority', 'SOC0-SOC11 High Priority', 'SOC0-SOC12 High Priority', 'SOC0-SOC13 High Priority', 'SOC0-SOC14 High Priority', 'All High Priority'});
        PulseModeSet = matlab.system.StringSet({'Pulse generation at End of Acquisition Window', 'Pulse generation at End of Conversion'});
        PPBBlockSet = matlab.system.StringSet({'PPB Block 1', 'PPB Block 2', 'PPB Block 3', 'PPB Block 4'});
        PPBSocEocNOSet = matlab.system.StringSet({'SOC0', 'SOC1', 'SOC2', 'SOC3', 'SOC4', 'SOC5', 'SOC6', 'SOC7', 'SOC8', 'SOC9', 'SOC10', 'SOC11', 'SOC12', 'SOC13', 'SOC14', 'SOC15'});
        BurstModeTriggerSet = matlab.system.StringSet({'Software Source', 'RTI Timer0', 'RTI Timer1', 'RTI Timer2', 'RTI Timer3', 'Input XBAR Out5', 'EPWM0 SOCA', 'EPWM0 SOCB', 'EPWM1 SOCA', 'EPWM1 SOCB', 'EPWM2 SOCA', 'EPWM2 SOCB', 'EPWM3 SOCA', 'EPWM3 SOCB', 'EPWM4 SOCA', 'EPWM4 SOCB', 'EPWM5 SOCA', 'EPWM5 SOCB', 'EPWM6 SOCA', 'EPWM6 SOCB', 'EPWM7 SOCA', 'EPWM7 SOCB', 'EPWM8 SOCA', 'EPWM8 SOCB', 'EPWM9 SOCA', 'EPWM9 SOCB', 'EPWM10 SOCA', 'EPWM10 SOCB', 'EPWM11 SOCA', 'EPWM11 SOCB', 'EPWM12 SOCA', 'EPWM12 SOCB', 'EPWM13 SOCA', 'EPWM13 SOCB', 'EPWM14 SOCA', 'EPWM14 SOCB', 'EPWM15 SOCA', 'EPWM15 SOCB', 'EPWM16 SOCA', 'EPWM16 SOCB', 'EPWM17 SOCA', 'EPWM17 SOCB', 'EPWM18 SOCA', 'EPWM18 SOCB', 'EPWM19 SOCA', 'EPWM19 SOCB', 'EPWM20 SOCA', 'EPWM20 SOCB', 'EPWM21 SOCA', 'EPWM21 SOCB', 'EPWM22 SOCA', 'EPWM22 SOCB', 'EPWM23 SOCA', 'EPWM23 SOCB', 'EPWM24 SOCA', 'EPWM24 SOCB', 'EPWM25 SOCA', 'EPWM25 SOCB', 'EPWM26 SOCA', 'EPWM26 SOCB', 'EPWM27 SOCA', 'EPWM27 SOCB', 'EPWM28 SOCA', 'EPWM28 SOCB', 'EPWM29 SOCA', 'EPWM29 SOCB', 'EPWM30 SOCA', 'EPWM30 SOCB', 'EPWM31 SOCA', 'EPWM31 SOCB', 'ECAP0 SOCEVT', 'ECAP1 SOCEVT', 'ECAP2 SOCEVT', 'ECAP3 SOCEVT', 'ECAP4 SOCEVT', 'ECAP5 SOCEVT', 'ECAP6 SOCEVT', 'ECAP7 SOCEVT', 'ECAP SOCEVT8', 'ECAP9 SOCEVT'});
        BurstSizeSet = matlab.system.StringSet({'1 SOC long', '2 SOC long', '3 SOC long', '4 SOC long', '5 SOC long', '6 SOC long', '7 SOC long', '8 SOC long', '9 SOC long', '10 SOC long', '11 SOC long', '12 SOC long', '13 SOC long', '14 SOC long', '15 SOC long', '16 SOC long'});
        InterruptSOCSourceSet = matlab.system.StringSet({'SOC0', 'SOC1', 'SOC2', 'SOC3', 'SOC4', 'SOC5', 'SOC6', 'SOC7', 'SOC8', 'SOC9', 'SOC10', 'SOC11', 'SOC12', 'SOC13', 'SOC14', 'SOC15'});
   end

   properties (Dependent, Access=protected)
    ADCModuleEnum;
    PrescalerEnum;
    ResolutionModeEnum;
    SignalModeEnum;
    SOCTriggerNoEnum;
    SOCxTriggerSourceEnum;
    INTWillTriggerSOCxEnum;
    DataTypeEnum;
    InterruptSelectionEnum;
    InterruptSOCSourceEnum;
    ChannelsEnum;
    Channels1Enum;
    SOCPriorityEnum;
    PulseModeEnum;
    PPBBlockEnum;
    PPBSocEocNOEnum;
    BurstModeTriggerEnum;
    BurstSizeEnum;
   end

    methods
        % Constructor
        function obj = ADC(varargin)
            % Support name-value pair arguments when constructing the object.
            setProperties(obj,nargin,varargin{:});
        end

        function ret = get.ADCModuleEnum(obj)
            if isequal(obj.ADCModule,'ADC0')
                ret = uint16(0);
            elseif isequal(obj.ADCModule, 'ADC1')
                ret = uint16(1);
            elseif isequal(obj.ADCModule, 'ADC2')
                ret = uint16(2);
            elseif isequal(obj.ADCModule, 'ADC3')
                ret = uint16(3);
            elseif isequal(obj.ADCModule, 'ADC4')
                ret = uint16(4);
            else
                ret = uint16(0);
            end
        end

        function ret = get.PrescalerEnum(obj)
            if isequal(obj.Prescaler,'1.0')
                ret = uint16(0);
            elseif isequal(obj.Prescaler, '2.0')
                ret = uint16(2);
            elseif isequal(obj.Prescaler, '2.5')
                ret = uint16(3);
            elseif isequal(obj.Prescaler, '3.0')
                ret = uint16(4);
            elseif isequal(obj.Prescaler, '3.5')
                ret = uint16(5);
            elseif isequal(obj.Prescaler, '4.0')
                ret = uint16(6);
            elseif isequal(obj.Prescaler, '4.5')
                ret = uint16(7);
            elseif isequal(obj.Prescaler, '5.0')
                ret = uint16(8);
            elseif isequal(obj.Prescaler, '5.5')
                ret = uint16(9);
            elseif isequal(obj.Prescaler, '6.0')
                ret = uint16(10);
            elseif isequal(obj.Prescaler, '6.5')
                ret = uint16(11);
            elseif isequal(obj.Prescaler, '7.0')
                ret = uint16(12);
            elseif isequal(obj.Prescaler, '7.5')
                ret = uint16(13);
            elseif isequal(obj.Prescaler, '8.0')
                ret = uint16(14);
            elseif isequal(obj.Prescaler, '8.5')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ResolutionModeEnum(obj)
            if isequal(obj.ResolutionMode,'Twelve bit conversion')
                ret = uint16(0);
            else
                ret = uint16(1);
            end
        end

        function ret = get.SignalModeEnum(obj)
            if isequal(obj.SignalMode,'Sample on single pin with VREFLO')
                ret = uint16(0);
            elseif isequal(obj.SignalMode,'Sample on pair of pins')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SOCTriggerNoEnum(obj)
            if isequal(obj.SOCTriggerNo,'SOC0')
                ret = uint16(0);
            elseif isequal(obj.SOCTriggerNo,'SOC1')
                ret = uint16(1);
            elseif isequal(obj.SOCTriggerNo,'SOC2')
                ret = uint16(2);
            elseif isequal(obj.SOCTriggerNo,'SOC3')
                ret = uint16(3);
            elseif isequal(obj.SOCTriggerNo,'SOC4')
                ret = uint16(4);
            elseif isequal(obj.SOCTriggerNo,'SOC5')
                ret = uint16(5);
            elseif isequal(obj.SOCTriggerNo,'SOC6')
                ret = uint16(6);
            elseif isequal(obj.SOCTriggerNo,'SOC7')
                ret = uint16(7);
            elseif isequal(obj.SOCTriggerNo,'SOC8')
                ret = uint16(8);
            elseif isequal(obj.SOCTriggerNo,'SOC9')
                ret = uint16(9);
            elseif isequal(obj.SOCTriggerNo,'SOC10')
                ret = uint16(10);
            elseif isequal(obj.SOCTriggerNo,'SOC11')
                ret = uint16(11);
            elseif isequal(obj.SOCTriggerNo,'SOC12')
                ret = uint16(12);
            elseif isequal(obj.SOCTriggerNo,'SOC13')
                ret = uint16(13);
            elseif isequal(obj.SOCTriggerNo,'SOC14')
                ret = uint16(14);
            elseif isequal(obj.SOCTriggerNo,'SOC15')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SOCxTriggerSourceEnum(obj)
            if isequal(obj.SOCxTriggerSource,'Software Source')
                ret = 0X00;
            elseif isequal(obj.SOCxTriggerSource,'RTI Timer0')
                ret = 0X01;
            elseif isequal(obj.SOCxTriggerSource,'RTI Timer1')
                ret = 0X02;
            elseif isequal(obj.SOCxTriggerSource,'RTI Timer2')
                ret = 0X03;
            elseif isequal(obj.SOCxTriggerSource,'RTI Timer3')
                ret = 0X04;
            elseif isequal(obj.SOCxTriggerSource,'Input XBAR Out5')
                ret = 0X05;
            elseif isequal(obj.SOCxTriggerSource,'EPWM0 SOCA')
                ret = 0X08;
            elseif isequal(obj.SOCxTriggerSource,'EPWM0 SOCB')
                ret = 0X09;
            elseif isequal(obj.SOCxTriggerSource,'EPWM1 SOCA')
                ret = 0X0A;
            elseif isequal(obj.SOCxTriggerSource,'EPWM1 SOCB')
                ret = 0X0B;
            elseif isequal(obj.SOCxTriggerSource,'EPWM2 SOCA')
                ret = 0X0C;
            elseif isequal(obj.SOCxTriggerSource,'EPWM2 SOCB')
                ret = 0X0D;
            elseif isequal(obj.SOCxTriggerSource,'EPWM3 SOCA')
                ret = 0X0E;
            elseif isequal(obj.SOCxTriggerSource,'EPWM3 SOCB')
                ret = 0X0F;
            elseif isequal(obj.SOCxTriggerSource,'EPWM4 SOCA')
                ret = 0X10;
            elseif isequal(obj.SOCxTriggerSource,'EPWM4 SOCB')
                ret = 0X11;
            elseif isequal(obj.SOCxTriggerSource,'EPWM5 SOCA')
                ret = 0X12;
            elseif isequal(obj.SOCxTriggerSource,'EPWM5 SOCB')
                ret = 0X13;
            elseif isequal(obj.SOCxTriggerSource,'EPWM6 SOCA')
                ret = 0X14;
            elseif isequal(obj.SOCxTriggerSource,'EPWM6 SOCB')
                ret = 0X15;
            elseif isequal(obj.SOCxTriggerSource,'EPWM7 SOCA')
                ret = 0X16;
            elseif isequal(obj.SOCxTriggerSource,'EPWM7 SOCB')
                ret = 0X17;
            elseif isequal(obj.SOCxTriggerSource,'EPWM8 SOCA')
                ret = 0X18;
            elseif isequal(obj.SOCxTriggerSource,'EPWM8 SOCB')
                ret = 0X19;
            elseif isequal(obj.SOCxTriggerSource,'EPWM9 SOCA')
                ret = 0X1A;
            elseif isequal(obj.SOCxTriggerSource,'EPWM9 SOCB')
                ret = 0X1B;
            elseif isequal(obj.SOCxTriggerSource,'EPWM10 SOCA')
                ret = 0X1C;
            elseif isequal(obj.SOCxTriggerSource,'EPWM10 SOCB')
                ret = 0X1D;
            elseif isequal(obj.SOCxTriggerSource,'EPWM11 SOCA')
                ret = 0X1E;
            elseif isequal(obj.SOCxTriggerSource,'EPWM11 SOCB')
                ret = 0X1F;
            elseif isequal(obj.SOCxTriggerSource,'EPWM12 SOCA')
                ret = 0X20;
            elseif isequal(obj.SOCxTriggerSource,'EPWM12 SOCB')
                ret = 0X21;
            elseif isequal(obj.SOCxTriggerSource,'EPWM13 SOCA')
                ret = 0X22;
            elseif isequal(obj.SOCxTriggerSource,'EPWM13 SOCB')
                ret = 0X23;
            elseif isequal(obj.SOCxTriggerSource,'EPWM14 SOCA')
                ret = 0X24;
            elseif isequal(obj.SOCxTriggerSource,'EPWM14 SOCB')
                ret = 0X25;
            elseif isequal(obj.SOCxTriggerSource,'EPWM15 SOCA')
                ret = 0X26;
            elseif isequal(obj.SOCxTriggerSource,'EPWM15 SOCB')
                ret = 0X27;
            elseif isequal(obj.SOCxTriggerSource,'EPWM16 SOCA')
                ret = 0X28;
            elseif isequal(obj.SOCxTriggerSource,'EPWM16 SOCB')
                ret = 0X29;
            elseif isequal(obj.SOCxTriggerSource,'EPWM17 SOCA')
                ret = 0X2A;
            elseif isequal(obj.SOCxTriggerSource,'EPWM17 SOCB')
                ret = 0X2B;
            elseif isequal(obj.SOCxTriggerSource,'EPWM18 SOCA')
                ret = 0X2C;
            elseif isequal(obj.SOCxTriggerSource,'EPWM18 SOCB')
                ret = 0X2D;
            elseif isequal(obj.SOCxTriggerSource,'EPWM19 SOCA')
                ret = 0X2E;
            elseif isequal(obj.SOCxTriggerSource,'EPWM19 SOCB')
                ret = 0X2F;
            elseif isequal(obj.SOCxTriggerSource,'EPWM20 SOCA')
                ret = 0X30;
            elseif isequal(obj.SOCxTriggerSource,'EPWM20 SOCB')
                ret = 0X31;
            elseif isequal(obj.SOCxTriggerSource,'EPWM21 SOCA')
                ret = 0X32;
            elseif isequal(obj.SOCxTriggerSource,'EPWM21 SOCB')
                ret = 0X33;
            elseif isequal(obj.SOCxTriggerSource,'EPWM22 SOCA')
                ret = 0X34;
            elseif isequal(obj.SOCxTriggerSource,'EPWM22 SOCB')
                ret = 0X35;
            elseif isequal(obj.SOCxTriggerSource,'EPWM23 SOCA')
                ret = 0X36;
            elseif isequal(obj.SOCxTriggerSource,'EPWM23 SOCB')
                ret = 0X37;
            elseif isequal(obj.SOCxTriggerSource,'EPWM24 SOCA')
                ret = 0X38;
            elseif isequal(obj.SOCxTriggerSource,'EPWM24 SOCB')
                ret = 0X39;
            elseif isequal(obj.SOCxTriggerSource,'EPWM25 SOCA')
                ret = 0X3A;
            elseif isequal(obj.SOCxTriggerSource,'EPWM25 SOCB')
                ret = 0X3B;
            elseif isequal(obj.SOCxTriggerSource,'EPWM26 SOCA')
                ret = 0X3C;
            elseif isequal(obj.SOCxTriggerSource,'EPWM26 SOCB')
                ret = 0X3D;
            elseif isequal(obj.SOCxTriggerSource,'EPWM27 SOCA')
                ret = 0X3E;
            elseif isequal(obj.SOCxTriggerSource,'EPWM27 SOCB')
                ret = 0X3F;
            elseif isequal(obj.SOCxTriggerSource,'EPWM28 SOCA')
                ret = 0X40;
            elseif isequal(obj.SOCxTriggerSource,'EPWM28 SOCB')
                ret = 0X41;
            elseif isequal(obj.SOCxTriggerSource,'EPWM29 SOCA')
                ret = 0X42;
            elseif isequal(obj.SOCxTriggerSource,'EPWM29 SOCB')
                ret = 0X43;
            elseif isequal(obj.SOCxTriggerSource,'EPWM30 SOCA')
                ret = 0X44;
            elseif isequal(obj.SOCxTriggerSource,'EPWM30 SOCB')
                ret = 0X45;
            elseif isequal(obj.SOCxTriggerSource,'EPWM31 SOCA')
                ret = 0X46;
            elseif isequal(obj.SOCxTriggerSource,'EPWM31 SOCB')
                ret = 0X47;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP0 SOCEVT')
                ret = 0X48;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP1 SOCEVT')
                ret = 0X49;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP2 SOCEVT')
                ret = 0X4A;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP3 SOCEVT')
                ret = 0X4B;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP4 SOCEVT')
                ret = 0X4C;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP5 SOCEVT')
                ret = 0X4D;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP6 SOCEVT')
                ret = 0X4E;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP7 SOCEVT')
                ret = 0X4F;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP8 SOCEVT')
                ret = 0X50;
            elseif isequal(obj.SOCxTriggerSource, 'ECAP9 SOCEVT')
                ret = 0X51;
            else
                ret = 0X00;
            end
        end

            
       function ret = get.INTWillTriggerSOCxEnum(obj)
            if isequal(obj.INTWillTriggerSOCx,'No ADCINT Trigger')
                ret = uint16(0);
            elseif isequal(obj.INTWillTriggerSOCx, 'ADCINT1 will trigger SOC')
                ret = uint16(1);
            elseif isequal(obj.INTWillTriggerSOCx, 'ADCINT2 will trigger SOC')
                ret = uint16(2);
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

        function ret = get.InterruptSelectionEnum(obj)
            if isequal(obj.InterruptSelection,'ADC Interrupt Number 1')
                ret = uint16(0);
            elseif isequal(obj.InterruptSelection, 'ADC Interrupt Number 2')
                ret = uint16(1);
            elseif isequal(obj.InterruptSelection, 'ADC Interrupt Number 3')
                ret = uint16(2);
            elseif isequal(obj.InterruptSelection, 'ADC Interrupt Number 4')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end

        function ret = get.ChannelsEnum(obj)
            if isequal(obj.Channels,'Channel 0')
                ret = uint16(0);
            elseif isequal(obj.Channels, 'Channel 1')
                ret = uint16(1);
            elseif isequal(obj.Channels, 'Channel 2')
                ret = uint16(2);
            elseif isequal(obj.Channels, 'Channel 3')
                ret = uint16(3);
            elseif isequal(obj.Channels, 'Channel 4')
                ret = uint16(4);
            elseif isequal(obj.Channels, 'Channel 5')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.Channels1Enum(obj)
            if isequal(obj.Channels1,'Channels 0 and 1')
                ret = uint16(0);
            elseif isequal(obj.Channels1, 'Channels 1 and 0')
                ret = uint16(1);
            elseif isequal(obj.Channels1, 'Channels 2 and 3')
                ret = uint16(2);
            elseif isequal(obj.Channels1, 'Channels 3 and 2')
                ret = uint16(3);
            elseif isequal(obj.Channels1, 'Channels 4 and 5')
                ret = uint16(4);
            elseif isequal(obj.Channels1, 'Channels 5 and 4')
                ret = uint16(5);
            else
                ret = uint16(0);
            end
        end

        function ret = get.SOCPriorityEnum(obj)
            if isequal(obj.SOCPriority,'All Round Robin')
                ret = uint16(0);
            elseif isequal(obj.SOCPriority, 'SOC0 High Priority')
                ret = uint16(1);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC1 High Priority')
                ret = uint16(2);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC2 High Priority')
                ret = uint16(3);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC3 High Priority')
                ret = uint16(4);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC4 High Priority')
                ret = uint16(5);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC5 High Priority')
                ret = uint16(6);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC6 High Priority')
                ret = uint16(7);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC7 High Priority')
                ret = uint16(8);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC8 High Priority')
                ret = uint16(9);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC9 High Priority')
                ret = uint16(10);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC10 High Priority')
                ret = uint16(11);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC11 High Priority')
                ret = uint16(12);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC12 High Priority')
                ret = uint16(13);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC13 High Priority')
                ret = uint16(14);
            elseif isequal(obj.SOCPriority, 'SOC0-SOC14 High Priority')
                ret = uint16(15);
            elseif isequal(obj.SOCPriority, 'All High Priority')
                ret = uint16(16);
            else
                ret = uint16(0);
            end
        end

        function ret = get.PulseModeEnum(obj)
            if isequal(obj.PulseMode,'Pulse generation at end of Acquisition Window')
                ret = uint16(0);
            elseif isequal(obj.PulseMode,'Pulse generation at End of Conversion')
                ret = uint16(1);
            else
                ret = uint16(0);
            end
        end

        function ret = get.PPBBlockEnum(obj)
            if isequal(obj.PPBBlock, 'PPB Block 1')
                ret = uint16(0);
            elseif isequal(obj.PPBBlock, 'PPB Block 2')
                ret = uint16(1);
            elseif isequal(obj.PPBBlock, 'PPB Block 3')
                ret = uint16(2);
            elseif isequal(obj.PPBBlock, 'PPB Block 4')
                ret = uint16(3);
            else
                ret = uint16(0);
            end
        end


        function ret = get.PPBSocEocNOEnum(obj)
            if isequal(obj.PPBSocEocNO, 'SOC0')
                ret = uint16(0);
            elseif isequal(obj.PPBSocEocNO, 'SOC1')
                ret = uint16(1);
            elseif isequal(obj.PPBSocEocNO, 'SOC2')
                ret = uint16(2);
            elseif isequal(obj.PPBSocEocNO, 'SOC3')
                ret = uint16(3);
            elseif isequal(obj.PPBSocEocNO, 'SOC4')
                ret = uint16(4);
            elseif isequal(obj.PPBSocEocNO, 'SOC5')
                ret = uint16(5);
            elseif isequal(obj.PPBSocEocNO, 'SOC6')
                ret = uint16(6);
            elseif isequal(obj.PPBSocEocNO, 'SOC7')
                ret = uint16(7);
            elseif isequal(obj.PPBSocEocNO, 'SOC8')
                ret = uint16(8);
            elseif isequal(obj.PPBSocEocNO, 'SOC9')
                ret = uint16(9);
            elseif isequal(obj.PPBSocEocNO, 'SOC10')
                ret = uint16(10);
            elseif isequal(obj.PPBSocEocNO, 'SOC11')
                ret = uint16(11);
            elseif isequal(obj.PPBSocEocNO, 'SOC12')
                ret = uint16(12);
            elseif isequal(obj.PPBSocEocNO,' SOC13')
                ret = uint16(13);
            elseif isequal(obj.PPBSocEocNO,' SOC14')
                ret = uint16(14);
            elseif isequal(obj.PPBSocEocNO, 'SOC15')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.BurstModeTriggerEnum(obj)
            if isequal(obj.BurstModeTrigger,'Software Source')
                ret = 0X00;
            elseif isequal(obj.BurstModeTrigger,'RTI Timer0')
                ret = 0X01;
            elseif isequal(obj.BurstModeTrigger,'RTI Timer1')
                ret = 0X02;
            elseif isequal(obj.BurstModeTrigger,'RTI Timer2')
                ret = 0X03;
            elseif isequal(obj.BurstModeTrigger,'RTI Timer3')
                ret = 0X04;
            elseif isequal(obj.BurstModeTrigger,'Input XBAR Out5')
                ret = 0X05;
            elseif isequal(obj.BurstModeTrigger,'EPWM0 SOCA')
                ret = 0X08;
            elseif isequal(obj.BurstModeTrigger,'EPWM0 SOCB')
                ret = 0X09;
            elseif isequal(obj.BurstModeTrigger,'EPWM1 SOCA')
                ret = 0X0A;
            elseif isequal(obj.BurstModeTrigger,'EPWM1 SOCB')
                ret = 0X0B;
            elseif isequal(obj.BurstModeTrigger,'EPWM2 SOCA')
                ret = 0X0C;
            elseif isequal(obj.BurstModeTrigger,'EPWM2 SOCB')
                ret = 0X0D;
            elseif isequal(obj.BurstModeTrigger,'EPWM3 SOCA')
                ret = 0X0E;
            elseif isequal(obj.BurstModeTrigger,'EPWM3 SOCB')
                ret = 0X0F;
            elseif isequal(obj.BurstModeTrigger,'EPWM4 SOCA')
                ret = 0X10;
            elseif isequal(obj.BurstModeTrigger,'EPWM4 SOCB')
                ret = 0X11;
            elseif isequal(obj.BurstModeTrigger,'EPWM5 SOCA')
                ret = 0X12;
            elseif isequal(obj.BurstModeTrigger,'EPWM5 SOCB')
                ret = 0X13;
            elseif isequal(obj.BurstModeTrigger,'EPWM6 SOCA')
                ret = 0X14;
            elseif isequal(obj.BurstModeTrigger,'EPWM6 SOCB')
                ret = 0X15;
            elseif isequal(obj.BurstModeTrigger,'EPWM7 SOCA')
                ret = 0X16;
            elseif isequal(obj.BurstModeTrigger,'EPWM7 SOCB')
                ret = 0X17;
            elseif isequal(obj.BurstModeTrigger,'EPWM8 SOCA')
                ret = 0X18;
            elseif isequal(obj.BurstModeTrigger,'EPWM8 SOCB')
                ret = 0X19;
            elseif isequal(obj.BurstModeTrigger,'EPWM9 SOCA')
                ret = 0X1A;
            elseif isequal(obj.BurstModeTrigger,'EPWM9 SOCB')
                ret = 0X1B;
            elseif isequal(obj.BurstModeTrigger,'EPWM10 SOCA')
                ret = 0X1C;
            elseif isequal(obj.BurstModeTrigger,'EPWM10 SOCB')
                ret = 0X1D;
            elseif isequal(obj.BurstModeTrigger,'EPWM11 SOCA')
                ret = 0X1E;
            elseif isequal(obj.BurstModeTrigger,'EPWM11 SOCB')
                ret = 0X1F;
            elseif isequal(obj.BurstModeTrigger,'EPWM12 SOCA')
                ret = 0X20;
            elseif isequal(obj.BurstModeTrigger,'EPWM12 SOCB')
                ret = 0X21;
            elseif isequal(obj.BurstModeTrigger,'EPWM13 SOCA')
                ret = 0X22;
            elseif isequal(obj.BurstModeTrigger,'EPWM13 SOCB')
                ret = 0X23;
            elseif isequal(obj.BurstModeTrigger,'EPWM14 SOCA')
                ret = 0X24;
            elseif isequal(obj.BurstModeTrigger,'EPWM14 SOCB')
                ret = 0X25;
            elseif isequal(obj.BurstModeTrigger,'EPWM15 SOCA')
                ret = 0X26;
            elseif isequal(obj.BurstModeTrigger,'EPWM15 SOCB')
                ret = 0X27;
            elseif isequal(obj.BurstModeTrigger,'EPWM16 SOCA')
                ret = 0X28;
            elseif isequal(obj.BurstModeTrigger,'EPWM16 SOCB')
                ret = 0X29;
            elseif isequal(obj.BurstModeTrigger,'EPWM17 SOCA')
                ret = 0X2A;
            elseif isequal(obj.BurstModeTrigger,'EPWM17 SOCB')
                ret = 0X2B;
            elseif isequal(obj.BurstModeTrigger,'EPWM18 SOCA')
                ret = 0X2C;
            elseif isequal(obj.BurstModeTrigger,'EPWM18 SOCB')
                ret = 0X2D;
            elseif isequal(obj.BurstModeTrigger,'EPWM19 SOCA')
                ret = 0X2E;
            elseif isequal(obj.BurstModeTrigger,'EPWM19 SOCB')
                ret = 0X2F;
            elseif isequal(obj.BurstModeTrigger,'EPWM20 SOCA')
                ret = 0X30;
            elseif isequal(obj.BurstModeTrigger,'EPWM20 SOCB')
                ret = 0X31;
            elseif isequal(obj.BurstModeTrigger,'EPWM21 SOCA')
                ret = 0X32;
            elseif isequal(obj.BurstModeTrigger,'EPWM21 SOCB')
                ret = 0X33;
            elseif isequal(obj.BurstModeTrigger,'EPWM22 SOCA')
                ret = 0X34;
            elseif isequal(obj.BurstModeTrigger,'EPWM22 SOCB')
                ret = 0X35;
            elseif isequal(obj.BurstModeTrigger,'EPWM23 SOCA')
                ret = 0X36;
            elseif isequal(obj.BurstModeTrigger,'EPWM23 SOCB')
                ret = 0X37;
            elseif isequal(obj.BurstModeTrigger,'EPWM24 SOCA')
                ret = 0X38;
            elseif isequal(obj.BurstModeTrigger,'EPWM24 SOCB')
                ret = 0X39;
            elseif isequal(obj.BurstModeTrigger,'EPWM25 SOCA')
                ret = 0X3A;
            elseif isequal(obj.BurstModeTrigger,'EPWM25 SOCB')
                ret = 0X3B;
            elseif isequal(obj.BurstModeTrigger,'EPWM26 SOCA')
                ret = 0X3C;
            elseif isequal(obj.BurstModeTrigger,'EPWM26 SOCB')
                ret = 0X3D;
            elseif isequal(obj.BurstModeTrigger,'EPWM27 SOCA')
                ret = 0X3E;
            elseif isequal(obj.BurstModeTrigger,'EPWM27 SOCB')
                ret = 0X3F;
            elseif isequal(obj.BurstModeTrigger,'EPWM28 SOCA')
                ret = 0X40;
            elseif isequal(obj.BurstModeTrigger,'EPWM28 SOCB')
                ret = 0X41;
            elseif isequal(obj.BurstModeTrigger,'EPWM29 SOCA')
                ret = 0X42;
            elseif isequal(obj.BurstModeTrigger,'EPWM29 SOCB')
                ret = 0X43;
            elseif isequal(obj.BurstModeTrigger,'EPWM30 SOCA')
                ret = 0X44;
            elseif isequal(obj.BurstModeTrigger,'EPWM30 SOCB')
                ret = 0X45;
            elseif isequal(obj.BurstModeTrigger,'EPWM31 SOCA')
                ret = 0X46;
            elseif isequal(obj.BurstModeTrigger,'EPWM31 SOCB')
                ret = 0X47;
            elseif isequal(obj.BurstModeTrigger, 'ECAP0 SOCEVT')
                ret = 0X48;
            elseif isequal(obj.BurstModeTrigger, 'ECAP1 SOCEVT')
                ret = 0X49;
            elseif isequal(obj.BurstModeTrigger, 'ECAP2 SOCEVT')
                ret = 0X4A;
            elseif isequal(obj.BurstModeTrigger, 'ECAP3 SOCEVT')
                ret = 0X4B;
            elseif isequal(obj.BurstModeTrigger, 'ECAP4 SOCEVT')
                ret = 0X4C;
            elseif isequal(obj.BurstModeTrigger, 'ECAP5 SOCEVT')
                ret = 0X4D;
            elseif isequal(obj.BurstModeTrigger, 'ECAP6 SOCEVT')
                ret = 0X4E;
            elseif isequal(obj.BurstModeTrigger, 'ECAP7 SOCEVT')
                ret = 0X4F;
            elseif isequal(obj.BurstModeTrigger, 'ECAP8 SOCEVT')
                ret = 0X50;
            elseif isequal(obj.BurstModeTrigger, 'ECAP9 SOCEVT')
                ret = 0X51;
            else
                ret = 0X00;
            end
        end

        function ret = get.BurstSizeEnum(obj)
            if isequal(obj.BurstSize, '1 SOC long')
                ret = uint16(1);
            elseif isequal(obj.BurstSize, '2 SOC long')
                ret = uint16(2);
            elseif isequal(obj.BurstSize, '3 SOC long')
                ret = uint16(3);
            elseif isequal(obj.BurstSize, '4 SOC long')
                ret = uint16(4);
            elseif isequal(obj.BurstSize, '5 SOC long')
                ret = uint16(5);
            elseif isequal(obj.BurstSize, '6 SOC long')
                ret = uint16(6);
            elseif isequal(obj.BurstSize, '7 SOC long')
                ret = uint16(7);
            elseif isequal(obj.BurstSize, '8 SOC long')
                ret = uint16(8);
            elseif isequal(obj.BurstSize, '9 SOC long')
                ret = uint16(9);
            elseif isequal(obj.BurstSize, '10 SOC long')
                ret = uint16(10);
            elseif isequal(obj.BurstSize, '11 SOC long')
                ret = uint16(11);
            elseif isequal(obj.BurstSize, '12 SOC long')
                ret = uint16(12);
            elseif isequal(obj.BurstSize, '13 SOC long')
                ret = uint16(13);
            elseif isequal(obj.BurstSize, '14 SOC long')
                ret = uint16(14);
            elseif isequal(obj.BurstSize, '15 SOC long')
                ret = uint16(15);
            else
                ret = uint16(0);
            end
        end

        function ret = get.InterruptSOCSourceEnum(obj)
            if isequal(obj.InterruptSOCSource, 'SOC0')
                ret = uint16(0);
            elseif isequal(obj.InterruptSOCSource, 'SOC1')
                ret = uint16(1);
            elseif isequal(obj.InterruptSOCSource, 'SOC2')
                ret = uint16(2);
            elseif isequal(obj.InterruptSOCSource, 'SOC3')
                ret = uint16(3);
            elseif isequal(obj.InterruptSOCSource, 'SOC4')
                ret = uint16(4);
            elseif isequal(obj.InterruptSOCSource, 'SOC5')
                ret = uint16(5);
            elseif isequal(obj.InterruptSOCSource, 'SOC6')
                ret = uint16(6);
            elseif isequal(obj.InterruptSOCSource, 'SOC7')
                ret = uint16(7);
            elseif isequal(obj.InterruptSOCSource, 'SOC8')
                ret = uint16(8);
            elseif isequal(obj.InterruptSOCSource, 'SOC9')
                ret = uint16(9);
            elseif isequal(obj.InterruptSOCSource, 'SOC10')
                ret = uint16(10);
            elseif isequal(obj.InterruptSOCSource, 'SOC11')
                ret = uint16(11);
            elseif isequal(obj.InterruptSOCSource, 'SOC12')
                ret = uint16(12);
            elseif isequal(obj.InterruptSOCSource, 'SOC13')
                ret = uint16(13);
            elseif isequal(obj.InterruptSOCSource, 'SOC14')
                ret = uint16(14);
            elseif isequal(obj.InterruptSOCSource, 'SOC15')
                ret = uint16(15);
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

    methods (Static, Access = protected)

      function header = getHeaderImpl()
            header = matlab.system.display.Header(...s
                'Title','Type 3P1 ADC',...
                'Text', sprintf('%s\n%s','Configure the ADC Block to output data collected from the ADC pins on the Processor.'),...
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
             'PropertyList',  {'ADCModule','Prescaler','ResolutionMode','SOCPriority','SignalMode','SOCTriggerNo','SOCx_acquisition_Window','SOCxTriggerSource','INTWillTriggerSOCx','DataType', ...
             'ADCEnableConverter', 'PulseMode', 'PostInterruptAtEOCTrigger','InterruptSelection','InterruptSOCSource','ADCINT1CONTINOUSMODE','ADCINT2CONTINOUSMODE','ADCINT3CONTINOUSMODE','ADCINT4CONTINOUSMODE', 'SampleTime'});

             Input_Channels = matlab.system.display.SectionGroup(...
             'Title', ' Input_Channels', ...
             'PropertyList',  {'Channels','Channels1'});

             PPB_Configurations = matlab.system.display.SectionGroup(...
             'Title' , 'PPB_Configurations', ...
             'PropertyList', {'PPBBlock', 'PPBSocEocNO', 'Enable_Event_ADC_Trip_High', 'Enable_Event_ADC_Trip_Low', 'Enable_Event_ADC_Zero_Crossing', 'Enable_Interrupt_Event_ADC_Trip_High', 'Enable_Interrupt_Event_ADC_Trip_Low', 'Enable_Interrupt_Event_ADC_Zero_Crossing' ,'PPBEnableTwoComplement', 'PPBEnablePPBEventCBCClear', 'PPB_Calibration_Offset', 'PPB_Reference_Offset', 'PPB_High_Trip_Limit', 'PPB_Low_Trip_Limit'});

             Burst_Mode_Configurations = matlab.system.display.SectionGroup(...
             'Title', 'Burst_Mode_Configurations', ...
             'PropertyList', {'BurstModeTrigger', 'BurstSize', 'ADCEnableBurstMode'});

             
             groups = [General, Input_Channels, PPB_Configurations, Burst_Mode_Configurations];
            
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
               
              
                coder.cinclude('delay.h');
                coder.cinclude('MW_adc.h');
                switch uint16(obj.ADCModuleEnum)
                    case 0
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC0_U_BASE');
                    case 1
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC1_U_BASE');
                    case 2
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC2_U_BASE');
                    case 3
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC3_U_BASE');
                    case 4
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC4_U_BASE');
                    otherwise
                        obj.ConfigBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC0_U_BASE');
                end
                switch uint16(obj.ADCModuleEnum)
                    case 0
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC0_RESULT_U_BASE');
                    case 1
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC1_RESULT_U_BASE');
                    case 2
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC2_RESULT_U_BASE');
                    case 3
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC3_RESULT_U_BASE');
                    case 4
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC4_RESULT_U_BASE');
                    otherwise
                        obj.ResultBaseAddr = coder.opaque('uint32_t', 'CSL_CONTROLSS_ADC0_RESULT_U_BASE');
                end

                switch uint16(obj.PrescalerEnum)
                    case 0
                        obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(0+2)/2);
                    case 2
                         obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(2+2)/2);
                    case 3
                         obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(3+2)/2);
                    case 4
                        obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(4+2)/2);
                    case 5
                         obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(5+2)/2);
                    case 6
                          obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(6+2)/2);
                    case 7
                         obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(7+2)/2);
                    case 8
                          obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(8+2)/2);
                    case 9
                          obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(9+2)/2);
                    case 10
                          obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(10+2)/2);
                    case 11
                           obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(11+2)/2);
                    case 12
                            obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(12+2)/2);
                    case 13
                            obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(13+2)/2);
                    case 14
                           obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(14+2)/2);
                    case 15
                            obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(15+2)/2);
                    
                    otherwise
                       obj.Delay= obj.SOCx_acquisition_Window + uint32(10.5*(0+2)/2); 
                end


                adc_setup_struct = struct('base', obj.ConfigBaseAddr, 'module', uint16(obj.ADCModuleEnum), 'clkPrescale', uint16(obj.PrescalerEnum), 'resolution', uint16(obj.ResolutionModeEnum), 'signalMode', uint16(obj.SignalModeEnum), 'priMode', uint16(obj.SOCPriorityEnum), 'socNumber', uint16(obj.SOCTriggerNoEnum), 'soc_trigger_source', obj.SOCxTriggerSourceEnum, 'channels', uint16(obj.ChannelsEnum), 'channels1', uint16(obj.Channels1Enum), 'sampleWindow', obj.SOCx_acquisition_Window, 'interrupt_trigger', uint16(obj.INTWillTriggerSOCxEnum), 'interrupt_selectionnumber', uint16(obj.InterruptSelectionEnum), 'interrupt_soc', uint16(obj.InterruptSOCSourceEnum), 'enable_adc_continuousmode1', obj.ADCINT1CONTINOUSMODE, 'enable_adc_continuousmode2' , obj.ADCINT2CONTINOUSMODE, 'enable_adc_continuousmode3', obj.ADCINT3CONTINOUSMODE, 'enable_adc_continuousmode4', obj.ADCINT4CONTINOUSMODE, 'enable_adc', obj.ADCEnableConverter, 'pulse_mode' , uint16(obj.PulseModeEnum), 'ppbBlock', uint16(obj.PPBBlockEnum), 'ppbsocNumber' , uint16(obj.PPBSocEocNOEnum), 'calibration_offset', obj.PPB_Calibration_Offset, 'reference_offset', obj.PPB_Reference_Offset, 'high_trip_limit' ,obj.PPB_High_Trip_Limit, 'low_trip_limit', obj.PPB_Low_Trip_Limit, 'Enable_PPB_Two_Complement', obj.PPBEnableTwoComplement, 'Enable_PPB_Event_CBC_Clear', obj.PPBEnablePPBEventCBCClear, 'Enable_PPB_Event_ADC_Trip_High', obj.Enable_Event_ADC_Trip_High, 'Enable_PPB_Event_ADC_Trip_Low', obj.Enable_Event_ADC_Trip_Low , 'Enable_PPB_Event_ADC_Zero_Crossing', obj.Enable_Event_ADC_Zero_Crossing, 'Enable_PPB_Interrupt_Event_ADC_Trip_High', obj.Enable_Interrupt_Event_ADC_Trip_High, 'Enable_PPB_Interrupt_Event_ADC_Trip_Low', obj.Enable_Interrupt_Event_ADC_Trip_Low, 'Enable_PPB_Interrupt_Event_ADC_Zero_Crossing', obj.Enable_Interrupt_Event_ADC_Zero_Crossing, 'burstmode_trigger', obj.BurstModeTriggerEnum, 'burstSize', uint16(obj.BurstSizeEnum), 'Enable_BurstMode', obj.ADCEnableBurstMode, 'result_base', obj.ResultBaseAddr, 'delay', obj.Delay, 'Enable_Interrupt', obj.PostInterruptAtEOCTrigger, 'ADC_Module', uint16(obj.ADCModuleEnum));
                coder.cstructname(adc_setup_struct, 'ADCSetupStruct', 'extern', 'HeaderFile', 'MW_adc.h');
                coder.ceval('ADC_init', coder.ref(adc_setup_struct));
               
           end
        end
        function y = stepImpl(obj)  
             x = uint16(0);
             
            if isempty(coder.target)
                % Place simulation output code here
            else

                x = coder.ceval('ADC_step', obj.ConfigBaseAddr, uint16(obj.InterruptSelectionEnum), obj.SOCxTriggerSourceEnum, obj.ResultBaseAddr, uint16(obj.SOCTriggerNoEnum), uint16(obj.PPBBlockEnum), obj.Delay, uint16(obj.DataTypeEnum), obj.PostInterruptAtEOCTrigger);
                if isequal(obj.DataType, 'DOUBLE')
                    y = double(x);
                elseif isequal(obj.DataType,'SINGLE')
                    y =single(x);
                elseif isequal(obj.DataType, 'INT8')
                    y =int8(x);
                elseif isequal(obj.DataType, 'Unsigned INT8')
                    y =uint8(x);
                elseif isequal(obj.DataType, 'INT16')
                    y =int16(x);
                elseif isequal(obj.DataType, 'Unsigned INT16')
                    y =uint16(x);
                elseif isequal(obj.DataType, 'INT32')
                    y =int32(x);
                elseif isequal(obj.DataType, 'Unsigned INT32')
                    y =uint32(x);
                else
                    y =uint16(x);
                end
            end    
        end
        
        function releaseImpl(obj)
            if isempty(coder.target)
                % Place simulation termination code here
            else
                % Call C-function implementing device termination
                %coder.ceval('source_terminate');
                adc_release_struct = struct('base', obj.ConfigBaseAddr,  'interrupt_selectionnumber', uint16(obj.InterruptSelectionEnum));
                coder.cstructname(adc_release_struct, 'ADCReleaseStruct', 'extern', 'HeaderFile', 'MW_adc.h');
                coder.ceval('ADC_release', coder.ref(adc_release_struct));
                
             end
        end
        
        function flag = isInactivePropertyImpl(obj,propertyName)
            if (strcmp(propertyName,'InterruptSelection') && (obj.PostInterruptAtEOCTrigger==false)) 
                flag=true;
            elseif strcmp(propertyName,'Channels')
                flag = ~(ismember(obj.SignalMode, {'Sample on single pin with VREFLO'}));
           elseif strcmp(propertyName,'Channels1')
                flag= ~(ismember(obj.SignalMode, {'Sample on pair of pins'}));
            elseif (strcmp(propertyName, 'ADCINT1CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==false)) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT2CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==false)) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT3CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==false)) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT4CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==false)) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT1CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==true) && (isequal(obj.InterruptSelection, 'ADC Interrupt Number 2') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 3') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 4'))) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT2CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==true) && (isequal(obj.InterruptSelection, 'ADC Interrupt Number 1') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 3') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 4'))) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT3CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==true) && (isequal(obj.InterruptSelection, 'ADC Interrupt Number 1') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 2') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 4'))) 
               flag= true;
            elseif (strcmp(propertyName, 'ADCINT4CONTINOUSMODE') && (obj.PostInterruptAtEOCTrigger==true) && (isequal(obj.InterruptSelection, 'ADC Interrupt Number 1') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 2') || isequal(obj.InterruptSelection, 'ADC Interrupt Number 3'))) 
               flag= true;
            elseif (strcmp(propertyName, 'InterruptSOCSource') && (obj.PostInterruptAtEOCTrigger==false))
                flag = true;
            else
                flag= false;
            end
        end
    end
    
    
    methods (Access=protected)
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
           if isequal(obj.DataType, 'DOUBLE')
               varargout{1} = "double";
           elseif isequal(obj.DataType, 'SINGLE')
               varargout{1}= "single";
           elseif isequal(obj.DataType, 'INT8')
               varargout {1} ="int8";
           elseif isequal(obj.DataType, 'Unsigned INT8')
                 varargout{1}="uint8";
           elseif isequal(obj.DataType, 'INT16')
                 varargout{1}="int16";
           elseif isequal(obj.DataType, 'INT32')
                 varargout{1}="int32";
           elseif isequal(obj.DataType, 'Unsigned INT32')
                 varargout{1}="uint32";
             else
                 varargout{1}="uint16";
           end
        end
        
        function icon = getIconImpl(~)
            % Define a string as the icon for the System block in Simulink.
            icon = {'ADC','Type 3P1'};
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
            name = 'ADC';
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
                addIncludeFiles(buildInfo, 'MW_adc.h', includeDir);
                addSourceFiles(buildInfo, 'MW_adc.c', srcDir);
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
