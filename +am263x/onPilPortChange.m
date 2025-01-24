function onPilPortChange(hObj, hDlg, tag, ~)
%ONDEVICEADDRESSCHANGE
newVal = hDlg.getWidgetValue(tag);
validateattributes(newVal, {'char'}, {'nonempty', 'row'}, '', 'PilComPort');
PilPref = 'AM26xPILpref';
setpref(PilPref, 'COMport', newVal);
end

