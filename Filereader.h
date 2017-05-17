#ifndef FILEREADER_H
#define FILEREADER_H

#include <string>
#include <vector>

#include "Project.h"


namespace Filereader
{
	std::vector<double> convertStringToDoubles(std::string & stringToConvert, const std::string & identifier);

	std::shared_ptr<Project> readProjectFromFile(std::string & fileName, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
	std::shared_ptr<GestureClass> readGestureClassFromFile(std::string & fileName);
	std::shared_ptr<Gesture> readGestureFromFile(std::string & fileName);

	void loadAllProjects(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
	void loadAllGestureClasses(std::vector<std::shared_ptr<GestureClass>> * gestureClasses);

	void loadAllData(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses);
}

#endif //FILEREADER_H