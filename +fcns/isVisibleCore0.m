function res=isVisibleCore0(hObj)
      
    coreSelected = codertarget.data.getParameterValue(hObj, 'CpuInfo.CoreSelection');

    if(strcmp(coreSelected, 'Core0'))
        res = false;
    else
        res = true;
    end
    
    if(strcmp(coreSelected, 'Core0'))
        setpref('MultiCore', 'Core', 'Core0');
    elseif(strcmp(coreSelected, 'Core1'))
        setpref('MultiCore', 'Core', 'Core1');
    elseif(strcmp(coreSelected, 'Core2'))
        setpref('MultiCore', 'Core', 'Core2');
    elseif(strcmp(coreSelected, 'Core3'))
        setpref('MultiCore', 'Core', 'Core3');
    else
        setpref('MultiCore', 'Core', 'Core0');
    end
end
