% Model         :   PMSM Field Oriented Control
% Description   :   Set Parameters for PMSM Field Oriented Control



%% Set PWM Switching frequency
PWM_frequency 	= 20e3;    %Hz          // converter s/w freq
T_pwm           = 1/PWM_frequency;  %s  // PWM switching time period

%% Set Sample Times
Ts          	= T_pwm;        %sec        // Sample time for control system
Ts_simulink     = T_pwm/2;      %sec        // Simulation time step for model simulation
Ts_motor        = T_pwm/2;      %Sec        // Simulation time step for pmsm
Ts_inverter     = T_pwm/2;      %sec        // Simulation time step for inverter
Ts_speed        = 20*Ts;        %Sec        // Sample time for speed controller

%% Set data type for controller & code-gen
%Uncomment the required datatype and comment out the other
%dataType = fixdt(1,32,24);    % Fixed point code-generation
dataType = 'single';

%% System Parameters // Hardware parameters 

pmsm = mcb_SetPMSMMotorParameters('BLY171D');

%% Open loop reference values
T_Ref_openLoop          = 1;                    % Sec // Time for open-loop start-up
Speed_openLoop_PU       = 0.1;                  % PU  // Per-Unit speed referene for open-loop start-up
Vd_Ref_openLoop_PU      = Speed_openLoop_PU;    % Use 1.2x for Dyno setup and 1x for others

%% Target & Inverter Parameters

% target = mcb_SetProcessorDetails('AM26X',PWM_frequency);

inverter = mcb_SetInverterParameters('BoostXL-DRV8305');
%% Callibration Section

% Enable automatic calibration of ADC offset for current measurement
inverter.ADCOffsetCalibEnable = 1;  % Enable: 1, Disable:0 

% If automatic ADC offset calibration is disabled, uncomment and update the 
% offset values below manually
% inverter.CtSensAOffset = 2295;      % ADC Offset for phase current A 
% inverter.CtSensBOffset = 2286;      % ADC Offset for phase current B

% Update ADC Gain for DRV8305
if pmsm.I_rated < 5
    inverter.ADCGain = 4;   % ADC Range = +- 4.825A wrt 0-4095 counts
    inverter.SPI_Gain_Setting = 0x502A;
    
elseif pmsm.I_rated < 7
    inverter.ADCGain = 2;   % ADC Range = +- 9.650A wrt 0-4095 counts
    inverter.SPI_Gain_Setting = 0x5015;

else     
    inverter.ADCGain = 1;   % ADC Range = +- 19.300A wrt 0-4095 counts       
    inverter.SPI_Gain_Setting = 0x5000;        
    
end

% Voltage output of inverter current sense circuit
inverter.ISenseVoltPerAmp = inverter.ISenseVoltPerAmp * inverter.ADCGain; 

% Update ISenseMax according to set ADC gain
inverter.ISenseMax = inverter.ISenseMax /inverter.ADCGain;

% Max and min ADC counts for current sense offsets
inverter.CtSensOffsetMax = 2500; % Maximum permitted ADC counts for current sense offset
inverter.CtSensOffsetMin = 1500; % Minimum permitted ADC counts for current sense offset


%% Derive Characteristics
pmsm.N_base = mcb_getBaseSpeed(pmsm,inverter); %rpm // Base speed of motor at given Vdc

% mcb_getCharacteristics(pmsm,inverter);       % Uncomment for motor characteristics

%% PU System details // Set base values for pu conversion

PU_System = mcb_SetPUSystem(pmsm,inverter);
%% Set Acceleration
acceleration = 20000/PU_System.N_base;                  % P.U/Sec // Maximum allowable acceleraton

%% Controller design // Get ballpark values!
PI_params = mcb.internal.SetControllerParameters(pmsm,inverter,PU_System,T_pwm,2*Ts,Ts_speed);

% Set SMO parameters
smo = mcb_ComputeSMOParameters(pmsm,Ts,PU_System);

%Updating delays for simulation
PI_params.delay_Currents    = 1; % No of samples delayed for current sensing

% %Uncomment for frequency domain analysis
% mcb_getControlAnalysis(pmsm,inverter,PU_System,PI_params,Ts,Ts_speed);

%% Displaying model variables
disp(pmsm);
disp(inverter);
disp(target);
disp(PU_System);
