#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>

#include "Project.h"


namespace Filereader
{
	//Converts the given string to a vector of doubles.
	std::vector<double> convertStringToDoubles(std::string & stringToConvert, const std::string & identifier);

	//Load a project from a file with given filename.
	std::shared_ptr<Project> readProjectFromFile(std::string & fileName, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
	//Load a gesture class from a file with given filename.
	std::shared_ptr<GestureClass> readGestureClassFromFile(std::string & fileName);
	//Load a gesture from a file with given filename.
	std::shared_ptr<Gesture> readGestureFromFile(std::string & fileName);

	//Load all saved projects.
	void loadAllProjects(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
	//Load all saved gesture classes.
	void loadAllGestureClasses(std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
	//Load all saved data. This is both all projects and all gesture classes.
	void loadAllData(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
}

#endif //FILEREADER_H