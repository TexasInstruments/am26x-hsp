function port = getPILHardware(~)
%getPilPort
PilPref = 'AM26xPILpref';
port = getpref(PilPref, 'PILHardware');
end
