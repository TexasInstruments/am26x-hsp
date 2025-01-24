function root = getTargetRoot()
	root = fileparts(strtok(mfilename('fullpath'), '+'));
end