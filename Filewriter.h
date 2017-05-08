#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include "Project.h"
#include "GestureClass.h"
#include "Gesture.h"


namespace Filewriter
{
	const std::string frameString{ "frame" };
	const std::string gestureString{ "gesture" };
	const std::string gestureClassString{ "gestclass" };
	const std::string actionString{ "action" };
	const std::string labelString{ "label" };
	const std::string nameString{ "name" };
	const std::string dateString{ "date" };
	const std::string svmString{ "svm" };

	const std::string gestureExtension{ ".gest" };
	const std::string gestureClassExtension{ ".gestclass" };
	const std::string projectExtension{ ".project" };
	const std::string svmModelExtension{ ".svmmodel" };

	const std::string subDirectoryString{ "DATA/" };
	const std::string subDirectoryNameString{ "DATA" };

	void save(std::shared_ptr<Project> & project);
	void save(std::shared_ptr<GestureClass> & gestureClass);
	void save(Gesture & gesture);
	void save(svm_model & svmModel, int projectID);

	void write(std::string & fileName, std::string & data);
	void overwrite(std::string & fileName, std::string & data, bool endWithNewline = false);
}

#endif //FILEWRITER_H