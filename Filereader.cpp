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
	fNameStream << Filewriter::subDirectoryString;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::gestureClassString.size(), Filewriter::gestureClassString))
		{
			std::vector<double> gestureClassData = convertStringToDoubles(fileLine, Filewriter::gestureClassString);
			double label = gestureClassData.at(0);
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
		else if (!fileLine.compare(0, Filewriter::actionString.size(), Filewriter::actionString))
		{
			std::vector<double> actionData = convertStringToDoubles(fileLine, Filewriter::actionString);
			double label = actionData.at(0);
			WORD keycode = actionData.at(1);
			bool hold = actionData.at(2);

			if (project->containsLabel(label))
			{
				project->addAction(label, keycode, hold);
			}
		}
		else if (!fileLine.compare(0, Filewriter::svmString.size(), Filewriter::svmString))
		{
			std::vector<double> svmData = convertStringToDoubles(fileLine, Filewriter::svmString);
			double projectID = svmData.at(0);

			std::ostringstream svmStream;
			svmStream << Filewriter::subDirectoryString;
			svmStream << projectID;
			svmStream << Filewriter::svmModelExtension;
			std::string svmFilename = svmStream.str();

			const char * svmFname = svmFilename.c_str();
			svm_model loadedSVMModel = *(svm_load_model(svmFname));
			
			project->setSVMModel(loadedSVMModel);
		}
		else if (!fileLine.compare(0, Filewriter::nameString.size(), Filewriter::nameString))
		{
			fileLine.erase(0, Filewriter::nameString.size() + 1);
			project->setName(fileLine);
		}
	}

	int projectID = std::stoi(fileName.substr(0, fileName.size()-Filewriter::projectExtension.size()));
	project->setProjectID(projectID);
	return project;
}

std::shared_ptr<GestureClass> Filereader::readGestureClassFromFile(std::string & fileName)
{
	std::shared_ptr<GestureClass> gestureClass = std::make_shared<GestureClass>();

	std::ostringstream fNameStream;
	fNameStream << Filewriter::subDirectoryString;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::gestureString.size(), Filewriter::gestureString))
		{
			std::vector<double> gestureData = convertStringToDoubles(fileLine, Filewriter::gestureString);
			
			std::ostringstream stream;
			stream << gestureData.at(0);
			stream << Filewriter::gestureExtension;

			Gesture gesture;
			readGestureFromFile(stream.str(), &gesture);
			gesture.setGestureID(gestureData.at(0));
			gestureClass->addGesture(gesture);
		}
		else if (!fileLine.compare(0, Filewriter::nameString.size(), Filewriter::nameString))
		{
			fileLine.erase(0, Filewriter::nameString.size() + 1);
			gestureClass->setName(fileLine);
		}
	}

	int gestureClassID = std::stoi(fileName.substr(0, fileName.size() - Filewriter::gestureClassExtension.size()));
	gestureClass->setGestureClassID(gestureClassID);
	return gestureClass;
}

void Filereader::readGestureFromFile(std::string & fileName, Gesture * gesture)
{
	std::ostringstream fNameStream;
	fNameStream << Filewriter::subDirectoryString;
	fNameStream << fileName;
	std::ifstream file(fNameStream.str());
	std::string fileLine;
	std::vector<Frame> frames;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::frameString.size(), Filewriter::frameString))
		{
			std::vector<double> frameData = convertStringToDoubles(fileLine, Filewriter::frameString);

			std::vector<Joint> joints;
			for (int i = 0; i < Frame::NB_OF_JOINTS; i++)
			{
				Joint joint;
				joint.JointType = static_cast<JointType>(i);
				joint.TrackingState = TrackingState::TrackingState_Tracked;
				CameraSpacePoint csp;
				int indexJump = i * Gesture::DIMENSIONS_PER_JOINT;
				csp.X = frameData.at(indexJump);
				csp.Y = frameData.at(indexJump + 1);
				csp.Z = frameData.at(indexJump + 2);
				joint.Position = csp;
				joints.push_back(joint);
			}
			Frame frame{joints, false};
			frame.setTimestamp(frameData.at(frameData.size()-1));
			frames.push_back(frame);
		}
		else if (!fileLine.compare(0, Filewriter::nameString.size(), Filewriter::nameString))
		{
			fileLine.erase(0, Filewriter::nameString.size() + 1);
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
	std::vector<std::string> files = FilenameChecker::getAllFileNames(Filewriter::projectExtension);
	for (std::string & filename : files)
	{
		std::shared_ptr<Project> project = readProjectFromFile(filename, gestureClasses);
		projects->push_back(project);
	}
}

void Filereader::loadAllGestureClasses(std::vector<std::shared_ptr<GestureClass>> * gestureClasses)
{
	std::vector<std::string> files = FilenameChecker::getAllFileNames(Filewriter::gestureClassExtension);
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
