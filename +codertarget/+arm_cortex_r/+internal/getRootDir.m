function rootDir = getSharedRootDir()
%GETROOTDIR Return the root directory of AM26x folder


rootDir = fileparts(strtok(mfilename('fullpath'), '+'));

end