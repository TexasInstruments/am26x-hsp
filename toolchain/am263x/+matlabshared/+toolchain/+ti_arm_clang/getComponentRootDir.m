function rootDir = getComponentRootDir()
%GETSPPKGROOTDIR Return the root directory of this shared component

%   Copyright 2016 The MathWorks, Inc.

rootDir = fileparts(strtok(mfilename('fullpath'), '+'));
end

