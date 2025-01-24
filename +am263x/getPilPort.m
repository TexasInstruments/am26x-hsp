function port = getPilPort(~)
%getPilPort
PilPref = 'AM26xPILpref';
port = getpref(PilPref, 'COMport');
end
