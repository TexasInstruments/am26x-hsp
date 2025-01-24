function onPILHardwareChange(hObj, hDlg, tag, ~)
%getPilPort
newVal = hDlg.getWidgetValue(tag);
validateattributes(newVal, {'numeric'}, {'nonempty', 'row'}, '', 'Pil');
PilPref = 'AM26xPILpref';

if(newVal == 0)
    setpref(PilPref, 'PILHardware', 'AM263X');
elseif(newVal == 1)
    setpref(PilPref, 'PILHardware', 'AM263PX');
elseif(newVal == 2)
    setpref(PilPref, 'PILHardware', 'AM261X');
end
