function rootDir = getComponentRootDir()
%GETSPPKGROOTDIR Return the root directory of this shared component



rootDir = fileparts(strtok(mfilename('fullpath'), '+'));
end

