function onAfterCodegenHookAM261x(hCS,buildInfo)
%ONAFTERCODEGENHOOK_BOSE Creates an archive of Embeded Coder Generated Files
%This function generates an archive of all the Simulink generated files
%This file generates a bat file to create a library of Simulink generated C
%files. The Makefiles link command will be replaced by a call to the bat
%file gnerated here.

% Copyright 2015 The MathWorks, Inc.
    coreSelected = codertarget.data.getParameterValue(hCS, 'CpuInfo.CoreSelection');
    OS = codertarget.data.getParameterValue(hCS, 'RTOS');

    if(strcmp(coreSelected, 'Core0') && strcmp(OS, 'Baremetal'))
        linkerCmdFile = '$(LINKER_CMD_DIR)/linker.cmd';
    elseif(strcmp(coreSelected, 'Core0') && strcmp(OS, 'FreeRTOS'))
        linkerCmdFile = '$(LINKER_CMD_DIR)/linkerfreertos.cmd';
    elseif(strcmp(coreSelected, 'Core1'))
        linkerCmdFile = '$(LINKER_CMD_DIR)/linker_core1.cmd';
    end

    % myBuildInfo = RTW.BuildInfo;
    % linkFlags = {'-Wl,-lC:/Users/A0499511/development_matlab_simulink/development_matlab_simulink_am26x/toolchain/am263x/+matlabshared/+toolchain/+ti_arm_clang/linker.cmd'};
    % addLinkFlags(myBuildInfo, linkFlags);
    % linkFlags = {'-Wl,-lC:/Users/A0499511/development_matlab_simulink/development_matlab_simulink_am26x/toolchain/am263x/+matlabshared/+toolchain/+ti_arm_clang/linker.cmd'};
    buildInfo.addLinkFlags(['-Wl,-l',linkerCmdFile]);

    % CCS project creation
    if isequal(get_param(hCS,'GenCodeOnly'), 'off')
        % CCS Project creation starts here
        am263x.createCCSprojectAM261x(hCS,buildInfo);
    end
end
 


