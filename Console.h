#ifndef CONSOLE_H
#define CONSOLE_H

#include <string>


//This namespace can be used to print out some stuff to a separate console. Useful while debugging.
//Call useConsole() once when initializing the application, then either use a print function
//from this namespace or the 'classic' print functions (for instance printf(...)).
namespace Console
{
	//Call this function once during app initialization to display a console for printing out debug data.
	void useConsole();
	void print(std::string stringToPrint);
	void printsl(std::string stringToPrint);
	void print(wchar_t stringToPrint[]);
	void printsl(wchar_t stringToPrint[]);
	void print(int numberToPrint);
	void printsl(int numberToPrint);
	void print(double numberToPrint);
	void printsl(double numberToPrint);
}

#endif //CONSOLE_H