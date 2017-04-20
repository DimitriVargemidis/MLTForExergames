#include "stdafx.h"
#include <iostream>

#include "Console.h"


namespace Console
{
	void useConsole()
	{
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	}

	void print(std::string stringToPrint)
	{
		std::cout << std::endl << stringToPrint;
	}

	void printsl(std::string stringToPrint)
	{
		std::cout << stringToPrint;
	}

	void print(wchar_t stringToPrint[])
	{
		std::wcout << std::endl << stringToPrint;
	}

	void printsl(wchar_t stringToPrint[])
	{
		std::wcout << stringToPrint;
	}

	void print(bool boolToPrint)
	{
		std::cout << std::endl << boolToPrint;
	}

	void printsl(bool boolToPrint)
	{
		std::cout << boolToPrint;
	}

	void print(int numberToPrint)
	{
		std::cout << std::endl << numberToPrint;
	}

	void printsl(int numberToPrint)
	{
		std::cout << numberToPrint;
	}

	void print(double numberToPrint)
	{
		std::cout << std::endl << numberToPrint;
	}

	void printsl(double numberToPrint)
	{
		std::cout << numberToPrint;
	}
}