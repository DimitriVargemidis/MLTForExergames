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
		std::cout << stringToPrint << std::endl;
	}

	void printsl(std::string stringToPrint)
	{
		std::cout << stringToPrint;
	}

	void print(int numberToPrint)
	{
		std::cout << numberToPrint << std::endl;
	}

	void printsl(int numberToPrint)
	{
		std::cout << numberToPrint;
	}

	void print(double numberToPrint)
	{
		std::cout << numberToPrint << std::endl;
	}

	void printsl(double numberToPrint)
	{
		std::cout << numberToPrint;
	}
}