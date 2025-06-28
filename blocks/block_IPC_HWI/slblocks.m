function blkStruct = slblocks
% This function specifies that the library 'mylib'
% should appear in the Library Browser with the 
% name 'My Library'

    Browser.Library = 'IPC_HWI_lib';
    % 'HWI_lib' is the name of the library

    Browser.Name = 'Embedded Coder Support Package for AM26x - IPC HWI Block';
    % This is the library name that appears
    % in the Library Browser

    blkStruct.Browser = Browser;