#ifndef FILENAMECHECKER_H
#define FILENAMECHECKER_H

#include <string>
#include "Filewriter.h"

namespace FilenameChecker
{
	int getHighestIDStored(std::string extension);
	std::vector<std::string> getAllFileNames(std::string extension);
	std::wstring stringToWstring(const std::string & s);
}

#endif //FILENAMECHECKER_H