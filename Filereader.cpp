#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "Frame.h"
#include "Gesture.h"
#include "GestureClass.h"
#include "FilenameChecker.h"
#include "Filewriter.h"
#include "Filereader.h"


std::vector<double> Filereader::convertStringToDoubles(std::string & stringToConvert, const std::string & identifier)
{
	//Erase the identifier; +1 is for removing whitespace after the identifier
	stringToConvert.erase(0, identifier.size() + 1);
	//Parse read string to doubles, put in a vector for easy access.
	std::stringstream strStream(stringToConvert);
	double number;
	std::vector<double> data;
	while (strStream >> number)
	{
		data.push_back(number);
	}
	return data;
}

std::shared_ptr<Project> Filereader::readProjectFromFile(std::string & fileName, std::vector<std::shared_ptr<GestureClass>> * gestureClasses)
{
	std::shared_ptr<Project> project = std::make_shared<Project>();

	std::ostringstream fNameStream;
	fNameStream << Filewriter::SUBDIRECTORY_STRING;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::GESTURECLASS_STRING.size(), Filewriter::GESTURECLASS_STRING))
		{
			std::vector<double> gestureClassData = convertStringToDoubles(fileLine, Filewriter::GESTURECLASS_STRING);
			int label = gestureClassData.at(0);
			int gClassID = gestureClassData.at(1);

			for (int i=0; i < gestureClasses->size(); i++)
			{
				if (gestureClasses->at(i)->getGestureClassID() == gClassID)
				{
					project->addNew(label, gestureClasses->at(i));
					break;
				}
			}
		}
		else if (!fileLine.compare(0, Filewriter::ACTION_STRING.size(), Filewriter::ACTION_STRING))
		{
			std::vector<double> actionData = convertStringToDoubles(fileLine, Filewriter::ACTION_STRING);
			int label = actionData.at(0);
			WORD keycode = actionData.at(1);
			bool hold = actionData.at(2);

			if (project->containsLabel(label))
			{
				project->addAction(label, keycode, hold);
			}
		}
		else if (!fileLine.compare(0, Filewriter::SVM_STRING.size(), Filewriter::SVM_STRING))
		{
			std::vector<double> svmData = convertStringToDoubles(fileLine, Filewriter::SVM_STRING);
			double projectID = svmData.at(0);

			std::ostringstream svmStream;
			svmStream << Filewriter::SUBDIRECTORY_STRING;
			svmStream << projectID;
			svmStream << Filewriter::SVM_MODEL_EXTENSION;
			std::string svmFilename = svmStream.str();

			const char * svmFname = svmFilename.c_str();
			svm_model loadedSVMModel = *(svm_load_model(svmFname));
			
			project->setSVMModel(loadedSVMModel);
		}
		else if (!fileLine.compare(0, Filewriter::NAME_STRING.size(), Filewriter::NAME_STRING))
		{
			fileLine.erase(0, Filewriter::NAME_STRING.size() + 1);
			project->setName(fileLine);
		}
	}

	int projectID = std::stoi(fileName.substr(0, fileName.size()-Filewriter::PROJECT_EXTENSION.size()));
	project->setProjectID(projectID);
	return project;
}

std::shared_ptr<GestureClass> Filereader::readGestureClassFromFile(std::string & fileName)
{
	std::shared_ptr<GestureClass> gestureClass = std::make_shared<GestureClass>();

	std::ostringstream fNameStream;
	fNameStream << Filewriter::SUBDIRECTORY_STRING;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::GESTURE_STRING.size(), Filewriter::GESTURE_STRING))
		{
			std::vector<double> gestureData = convertStringToDoubles(fileLine, Filewriter::GESTURE_STRING);
			
			std::ostringstream stream;
			stream << gestureData.at(0);
			stream << Filewriter::GESTURE_EXTENSION;

			Gesture gesture;
			readGestureFromFile(stream.str(), &gesture);
			gesture.setGestureID(gestureData.at(0));
			gestureClass->addGesture(gesture);
		}
		else if (!fileLine.compare(0, Filewriter::NAME_STRING.size(), Filewriter::NAME_STRING))
		{
			fileLine.erase(0, Filewriter::NAME_STRING.size() + 1);
			gestureClass->setName(fileLine);
		}
	}

	int gestureClassID = std::stoi(fileName.substr(0, fileName.size() - Filewriter::GESTURECLASS_EXTENSION.size()));
	gestureClass->setGestureClassID(gestureClassID);
	return gestureClass;
}

void Filereader::readGestureFromFile(std::string & fileName, Gesture * gesture)
{
	std::ostringstream fNameStream;
	fNameStream << Filewriter::SUBDIRECTORY_STRING;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;
	std::vector<Frame> frames;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::FRAME_STRING.size(), Filewriter::FRAME_STRING))
		{
			std::vector<double> frameData = convertStringToDoubles(fileLine, Filewriter::FRAME_STRING);

			std::vector<Joint> joints;
			for (int i = 0; i < Frame::NB_OF_JOINTS; i++)
			{
				Joint joint;
				joint.JointType = static_cast<JointType>(i);
				joint.TrackingState = TrackingState::TrackingState_Tracked;
				CameraSpacePoint csp;
				int indexJump = i * Frame::DIMENSIONS_PER_JOINT;
				csp.X = frameData.at(indexJump);
				csp.Y = frameData.at(indexJump + 1);
				csp.Z = frameData.at(indexJump + 2);
				joint.Position = csp;
				joints.push_back(joint);
			}
			Frame frame{joints, false};
			frames.push_back(frame);
		}
		else if (!fileLine.compare(0, Filewriter::NAME_STRING.size(), Filewriter::NAME_STRING))
		{
			fileLine.erase(0, Filewriter::NAME_STRING.size() + 1);
			gesture->setName(fileLine);
		}
	}

	for (Frame f : frames)
	{
		gesture->addFrame(f);
	}
}

void Filereader::loadAllProjects(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses)
{
	std::vector<std::string> files = FilenameChecker::getAllFileNames(Filewriter::PROJECT_EXTENSION);
	for (std::string & filename : files)
	{
		std::shared_ptr<Project> project = readProjectFromFile(filename, gestureClasses);
		projects->push_back(project);
	}
}

void Filereader::loadAllGestureClasses(std::vector<std::shared_ptr<GestureClass>> * gestureClasses)
{
	std::vector<std::string> files = FilenameChecker::getAllFileNames(Filewriter::GESTURECLASS_EXTENSION);
	for (std::string & filename : files)
	{
		std::shared_ptr<GestureClass> gestureClass = readGestureClassFromFile(filename);
		gestureClasses->push_back(gestureClass);
	}
}

void Filereader::loadAllData(std::vector<std::shared_ptr<Project>> * projects, std::vector<std::shared_ptr<GestureClass>> * gestureClasses)
{
	loadAllGestureClasses(gestureClasses);
	loadAllProjects(projects, gestureClasses);
}
