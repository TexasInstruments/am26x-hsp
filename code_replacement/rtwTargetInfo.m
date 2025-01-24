function rtwTargetInfo(cm)

cm.registerTargetInfo(@loc_register_crl);

function this = loc_register_crl

this(1) = RTW.TflRegistry;
this(1).Name = 'AM263P_TMU';
this(1).TableList = {'tmu_optimization'};
this(1).BaseTfl = '';
this(1).TargetHWDeviceType = {'*'};
this(1).Description = 'TMU librar support for math functions in AM263PX';
