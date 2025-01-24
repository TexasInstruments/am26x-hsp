function onHardwareSelectHook(hCS)
%ONHARDWARESELECT Hook point on target selection


     if isempty(hCS.getModel)
         return;
     end

	% Set BuildConfiguration default to Faster Runs

set_param(hCS, 'ERTCustomFileTemplate', 'AM263x_codertarget_file_process.tlc');

end

