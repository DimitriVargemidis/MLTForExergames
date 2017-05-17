#include <fstream>
#include <sstream>

#include "Console.h"
#include "Frame.h"
#include "Filewriter.h"


void Filewriter::save(std::shared_ptr<Project> & project)
{
	std::ostringstream sstream;
	sstream << project->getProjectID();
	std::string fileName = sstream.str();
	fileName.append(PROJECT_EXTENSION);
	overwrite(fileName, std::string());

	std::string nameLine = Filewriter::NAME_STRING + " " + project->getName();
	write(fileName, nameLine);

	if (project->hasTrainedSVM())
	{
		save(project->getSVMModel(), project->getProjectID());

		std::ostringstream svmStream;
		svmStream << Filewriter::SVM_STRING << " ";
		svmStream << project->getProjectID();
		std::string svmLine = svmStream.str();
		write(fileName, svmLine);
	}

	for (auto & keyValue : project->getProjectMap())
	{
		save(keyValue.second.first);

		std::string projectLine;
		projectLine.append(GESTURECLASS_STRING);
		projectLine.append(" ");

		std::ostringstream projectSStream;
		projectSStream << keyValue.first << " ";
		projectSStream << keyValue.second.first->getGestureClassID();
		projectLine.append(projectSStream.str());

		write(fileName, projectLine);

		for (const Action & action : keyValue.second.second)
		{
			std::string actionLine;
			actionLine.append(ACTION_STRING);
			actionLine.append(" ");

			std::ostringstream actionSStream;
			actionSStream << keyValue.first << " ";
			actionSStream << std::to_string(action.keycode) << " ";
			actionSStream << std::to_string(action.hold);
			actionLine.append(actionSStream.str());

			write(fileName, actionLine);
		}
	}
}

void Filewriter::save(std::shared_ptr<GestureClass> & gestureClass)
{
	std::ostringstream sstream;
	sstream << gestureClass->getGestureClassID();
	std::string fileName = sstream.str();
	fileName.append(Filewriter::GESTURECLASS_EXTENSION);
	overwrite(fileName, std::string());

	std::string nameLine = Filewriter::NAME_STRING + " " + gestureClass->getName();
	write(fileName, nameLine);

	for (Gesture & gesture : gestureClass->getGestures())
	{
		save(gesture);

		std::string gestureLine;
		gestureLine.append(Filewriter::GESTURE_STRING);
		gestureLine.append(" ");

		std::ostringstream gestureSStream;
		gestureSStream << gesture.getGestureID();
		gestureLine.append(gestureSStream.str());
		write(fileName, gestureLine);
	}
}

void Filewriter::save(Gesture & gesture)
{
	std::ostringstream sstream;
	sstream << gesture.getGestureID();
	std::string fileName = sstream.str();
	fileName.append(GESTURE_EXTENSION);
	overwrite(fileName, std::string());

	std::string nameLine = Filewriter::NAME_STRING + " " + gesture.getName();
	write(fileName, nameLine);

	for (const Frame & frame : gesture.getFrames())
	{
		std::string frameLine;
		frameLine.append(FRAME_STRING);
		frameLine.append(" ");
		std::ostringstream frameSStream;
		for (const Joint & joint : frame.getJoints())
		{
			frameSStream << joint.Position.X << " ";
			frameSStream << joint.Position.Y << " ";
			frameSStream << joint.Position.Z << " ";
		}
		frameLine.append(frameSStream.str());

		write(fileName, frameLine);
	}
}

void Filewriter::save(svm_model & svmModel, int projectID)
{
	std::ostringstream sstream;
	sstream << Filewriter::SUBDIRECTORY_STRING;
	sstream << projectID;
	sstream << Filewriter::SVM_MODEL_EXTENSION;
	std::string filename = sstream.str();
	
	char fname[25];
	strcpy(fname, filename.c_str());
	
	svm_save_model(fname, &svmModel);
}

void Filewriter::write(std::string & fileName, std::string & data)
{
	std::ostringstream fNameStream;
	fNameStream << SUBDIRECTORY_STRING << fileName;
	std::ofstream file(fNameStream.str(), std::fstream::app); //app: 'append', adds to the same file instead of overwriting.
	file << data << std::endl;
}

void Filewriter::overwrite(std::string & fileName, std::string & data, bool endWithNewline)
{
	std::ostringstream fNameStream;
	fNameStream << SUBDIRECTORY_STRING << fileName;
	std::ofstream file(fNameStream.str());
	if (endWithNewline)
	{
		file << data << std::endl;
	}
	else
	{
		file << data;
	}
}


