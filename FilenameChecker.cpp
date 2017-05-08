#include <string>
#include <vector>
#include <windows.h>
#include <stdio.h>

#include "FilenameChecker.h"


int FilenameChecker::getHighestIDStored(std::string extension)
{
	std::vector<std::string> filenames = getAllFileNamesWithoutExtension(extension);

	int convertedInt = 0;
	int maximumInt = 0;
	for (std::string str : filenames)
	{
		convertedInt = std::stoi(str);
		if (convertedInt > maximumInt)
		{
			maximumInt = convertedInt;
		}
	}
	return maximumInt;
}

std::vector<std::string> FilenameChecker::getAllFileNames(std::string extension)
{
	std::vector<std::string> names;
	
	std::string tempPath = Filewriter::subDirectoryString + "*" + extension;
	std::wstring strTemp = stringToWstring(tempPath);
	LPCWSTR path = strTemp.c_str();

	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(path, &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				
				WCHAR * wchar{ fd.cFileName };

				//convert from wide char to narrow char array
				char ch[260];
				char DefChar = ' ';
				WideCharToMultiByte(CP_ACP, 0, wchar, -1, ch, 260, &DefChar, NULL);

				//std::string using the char* constructor.
				std::string ss(ch);
				names.push_back(ss);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

std::vector<std::string> FilenameChecker::getAllFileNamesWithoutExtension(std::string & extension)
{
	std::vector<std::string> filenames = getAllFileNames(extension);
	for (std::string & s : filenames)
	{
		s = s.substr(0, s.size() - extension.size());
	}
	return filenames;
}

std::wstring FilenameChecker::stringToWstring(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}