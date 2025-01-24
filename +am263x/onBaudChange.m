function onBaudChange(hObj, hDlg, tag, ~)
%getPilPort
newVal = hDlg.getWidgetValue(tag);
% validateattributes(newVal, {'numeric'},{'nonempty', 'row'}, '', 'Pilbaud');
PilPref = 'AM26xPILpref';
setpref(PilPref, 'Baud', newVal);
end
