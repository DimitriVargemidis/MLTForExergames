#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>

namespace Console
{
	void useConsole();
	void print(std::string stringToPrint);
	void printsl(std::string stringToPrint);
	void print(wchar_t stringToPrint[]);
	void printsl(wchar_t stringToPrint[]);
	void print(bool boolToPrint);
	void printsl(bool boolToPrint);
	void print(int numberToPrint);
	void printsl(int numberToPrint);
	void print(double numberToPrint);
	void printsl(double numberToPrint);
}

#endif //CONSOLE_H