function hLib = profilingTimer(GCLK)
% PROFILINGTIMER Create Code Replacement Library Entry for function
%  code_profile_read_timer.  Enables profiling during PIL Simulation

% Copyright 2016 The MathWorks, Inc.

hLib = RTW.TflTable;
%---------- entry: code_profile_read_timer ----------- 
hEnt = RTW.TflCFunctionEntry;

[fldrPath,~,~]=fileparts(mfilename("fullpath"));
targetroot = fullfile(fldrPath,'..','..','..');

TargetHardware = getpref('AM26xPILpref', 'PILHardware');
if(isequal(TargetHardware, 'AM263X'))
    hEnt.setTflCFunctionEntryParameters( ...
              'Key', 'code_profile_read_timer', ...
              'Priority', 100, ...
              'ImplementationName', 'profileReadTimer', ...
              'SaturationMode', 'RTW_SATURATE_ON_OVERFLOW', ...
              'ImplementationSourceFile', 'profiler_timer_am263x.c', ...
              'ImplementationHeaderFile', 'profiler_timer_am263x.h', ...
              'ImplementationHeaderPath', fullfile(targetroot, 'include'), ...
              'ImplementationSourcePath', fullfile(targetroot, 'src'));

            hEnt.EntryInfo.CountDirection = 'RTW_TIMER_UP';
            hEnt.EntryInfo.TicksPerSecond = GCLK;

    % Conceptual Args

            arg = hEnt.getTflArgFromString('y1','uint32');
            arg.IOType = 'RTW_IO_OUTPUT';
            hEnt.addConceptualArg(arg);

    % Implementation Args 

           arg = hEnt.getTflArgFromString('y1','uint32');
           arg.IOType = 'RTW_IO_OUTPUT';
           hEnt.Implementation.setReturn(arg); 

           hLib.addEntry( hEnt );
elseif(isequal(TargetHardware, 'AM263PX'))
        hEnt.setTflCFunctionEntryParameters( ...
              'Key', 'code_profile_read_timer', ...
              'Priority', 100, ...
              'ImplementationName', 'profileReadTimer', ...
              'SaturationMode', 'RTW_SATURATE_ON_OVERFLOW', ...
              'ImplementationSourceFile', 'profiler_timer_am263px.c', ...
              'ImplementationHeaderFile', 'profiler_timer_am263px.h', ...
              'ImplementationHeaderPath', fullfile(targetroot, 'include'), ...
              'ImplementationSourcePath', fullfile(targetroot, 'src'));
    
            hEnt.EntryInfo.CountDirection = 'RTW_TIMER_UP';
            hEnt.EntryInfo.TicksPerSecond = GCLK;
		    
    % Conceptual Args
    
            arg = hEnt.getTflArgFromString('y1','uint32');
            arg.IOType = 'RTW_IO_OUTPUT';
            hEnt.addConceptualArg(arg);
    
    % Implementation Args 
    
           arg = hEnt.getTflArgFromString('y1','uint32');
           arg.IOType = 'RTW_IO_OUTPUT';
           hEnt.Implementation.setReturn(arg); 
    
           hLib.addEntry( hEnt );
end

