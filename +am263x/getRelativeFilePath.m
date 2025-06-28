function relativePath = getRelativeFilePath(basePath, targetPath)
    % getRelativeFilePath Computes the relative path from basePath to targetPath.
    % This function determines the relative path needed to navigate from
    % the directory specified by basePath to the directory specified by targetPath.
    % It calculates how many directories to go up from basePath and then how
    % to navigate down into targetPath.
    % If the top-most directory does not match, it returns targetPath as is.
    %
    % Parameters:
    %   basePath - The base directory path from which the relative path is calculated.
    %   targetPath - The target directory path to which the relative path is calculated.
    %
    % Returns:
    %   relativePath - A string representing the relative path from basePath to targetPath,
    %                  or targetPath itself if the top-most directories don't match.

    %   Copyright 2024 The MathWorks, Inc.
	
    % Split the paths into components using MATLAB's built-in strsplit function
    basePathParts = strsplit(basePath, filesep);
    targetPathParts = strsplit(targetPath, filesep);
    
    % Check if the top-most directories match
    if ~isempty(basePathParts) && ~isempty(targetPathParts) && strcmp(basePathParts{1}, targetPathParts{1})
        % Find the common base path
        minLength = min(length(basePathParts), length(targetPathParts));
        commonIndex = 0;
        
        for i = 1:minLength
            if strcmp(basePathParts{i}, targetPathParts{i})
                commonIndex = i;
            else
                break;
            end
        end
        
        % Calculate the number of directories to go up from basePath
        numDirsUp = length(basePathParts) - commonIndex;
        upPath = repmat(['..', filesep], 1, numDirsUp);
        
        % Calculate the path to go down into targetPath
        downPath = strjoin(targetPathParts(commonIndex+1:end), filesep);
        
        % Combine to get the relative path
        if isempty(downPath)
            relativePath = upPath(1:end-1); % Remove trailing file separator
        else
            relativePath = [upPath, downPath];
        end
    else
        % If the top-most directories do not match, return targetPath as is
        relativePath = targetPath;
    end
end