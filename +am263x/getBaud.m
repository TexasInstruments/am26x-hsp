function baud = getBaud(~)
%getPilPort
PilPref = 'AM26xPILpref';
baud = getpref(PilPref, 'Baud');
end
