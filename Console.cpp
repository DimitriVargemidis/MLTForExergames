#include "stdafx.h"
#include <iostream>

#include "Console.h"


//This namespace can be used to print out some stuff to a separate console. Useful while debugging.
//Call useConsole() once when initializing the application, then either use a print function
//from this namespace or the 'classic' print functions (for instance printf(...)).
namespace Console
{
	//Call this function once during app initialization to display a console for printing out debug data.
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