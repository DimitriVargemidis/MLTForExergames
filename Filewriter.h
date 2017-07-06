#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <string>
#include "Project.h"
#include "GestureClass.h"
#include "Gesture.h"


namespace Filewriter
{
	const std::string FRAME_STRING{ "frame" };
	const std::string GESTURE_STRING{ "gesture" };
	const std::string GESTURECLASS_STRING{ "gestclass" };
	const std::string ACTION_STRING{ "action" };
	const std::string LABEL_STRING{ "label" };
	const std::string NAME_STRING{ "name" };
	const std::string DATE_STRING{ "date" };
	const std::string SVM_STRING{ "svm" };

	const std::string GESTURE_EXTENSION{ ".gest" };
	const std::string GESTURECLASS_EXTENSION{ ".gestclass" };
	const std::string PROJECT_EXTENSION{ ".project" };
	const std::string SVM_MODEL_EXTENSION{ ".svmmodel" };

	const std::string SUBDIRECTORY_STRING{ "DATA/" };
	const std::string SUBDIRECTORY_NAME_STRING{ "DATA" };

	//Save the given project to a file.
	void save(std::shared_ptr<Project> & project);
	//Save the given gesture class to a file.
	void save(std::shared_ptr<GestureClass> & gestureClass);
	//Save the given gesture to a file.
	void save(Gesture & gesture);
	//Save the given svm_model to a file.
	void save(svm_model & svmModel, int projectID);

	//Write the given data to a file with a given name.
	void write(std::string & fileName, std::string & data);
	//Overwrite the file with a given name and write the given data to the file.
	void overwrite(std::string & fileName, std::string & data, bool endWithNewline = false);
}

#endif //FILEWRITER_H