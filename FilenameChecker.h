#ifndef FILENAMECHECKER_H
#define FILENAMECHECKER_H

#include <string>
#include "Filewriter.h"

namespace FilenameChecker
{
	//Return the highest ID (used as name) of all files with given extension. This is used
	//when starting the program to initialize automatic increments for ID generation.
	int getHighestIDStored(std::string extension);
	//Get all file names that have the given extension.
	std::vector<std::string> getAllFileNames(std::string extension);
	//Get all file names (without extension) that have the given extension.
	std::vector<std::string> getAllFileNamesWithoutExtension(std::string & extension);
	//Convert the given string to a WString. Cumbersome thing to do, but must be done. Thanks, Microsoft!
	std::wstring stringToWstring(const std::string & s);
}

#endif //FILENAMECHECKER_H