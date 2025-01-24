function out = updateAM263PToolchain(hObj)
%UPDATETOOLCHAIN Summary of this function goes here
%   Detailed explanation goes here
	cs = hObj.getConfigSet;
	out = codertarget.data.getParameterValue(cs,'RTOS');
	if(strcmp(out,'FreeRTOS'))
		%set the FreeRTOS toolchain
		set_param(cs,'Toolchain','TI ARM Clang Compiler AM263Px FreeRTOS | gmake');
	else
		%set the Baremetal toolchain
		set_param(cs,'Toolchain','TI ARM Clang Compiler AM263Px | gmake');
	end
end

