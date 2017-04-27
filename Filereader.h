#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>

#include "Project.h"


namespace Filereader
{
	/*
	void read(std::string fileName);
	std::vector<double> readNumbers(std::string & fileName);

	Gesture createGestureFromData(std::vector<double> & data, int nbOfFrames);
	*/
	std::vector<double> convertStringToDoubles(std::string & stringToConvert, const std::string & identifier);

	void readProjectFromFile(std::string & fileName, Project * project, std::vector<GestureClass> * gestureClasses);
	void readGestureClassFromFile(std::string & fileName, GestureClass * gestureClass);
	void readGestureFromFile(std::string & fileName, Gesture * gesture);
}

#endif //FILEREADER_H