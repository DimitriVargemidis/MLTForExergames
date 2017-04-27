#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>

#include "Console.h"
#include "Frame.h"
#include "Gesture.h"
#include "GestureClass.h"
#include "Filewriter.h"
#include "Filereader.h"

/*
void Filereader::read(std::string fileName)
{
	std::ifstream file(fileName);
	std::string fileLine;

	std::string frameString{"frame"};
	std::string buttonString{"button"};

	std::vector<GestureClass> gestureClasses(15);

	int numbersPerFrame = Frame::NB_OF_JOINTS * Gesture::DIMENSIONS_PER_JOINT + 1;
	int numberOfFrames;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, frameString.size(), frameString))
		{
			std::vector<double> data = convertStringToDoubles(fileLine, frameString);
			numberOfFrames = (data.size() - 1) / numbersPerFrame;
			if (numbersPerFrame*numberOfFrames + 1 == data.size())
			{
				GestureClass gestureClass;
				createGestureFromData(data, numberOfFrames);
			}
		}
		else if (!fileLine.compare(0, buttonString.size(), buttonString))
		{
			std::vector<double> data = convertStringToDoubles(fileLine, buttonString);
		}
		else
		{
			Console::print("Something went wrong. Can't handle this line.");
		}
	}
}

std::vector<double> Filereader::readNumbers(std::string & fileName)
{
	std::ifstream infile(fileName);
	std::vector<double> data;
	double number;
	while (infile >> number)
	{
		data.push_back(number);
	}
	return data;
}

Gesture Filereader::createGestureFromData(std::vector<double> & data, int nbOfFrames)
{
	Gesture gesture;
	int jointCounter = 0;
	int frameShiftCounter = 0;
	for (int j = 0; j < nbOfFrames; j++)
	{
		frameShiftCounter = j * (Frame::NB_OF_JOINTS * Gesture::DIMENSIONS_PER_JOINT + 1);
		std::vector<Joint> joints;
		for (int i = 0; i < Frame::NB_OF_JOINTS; i++)
		{
			Joint joint{};
			joint.JointType = static_cast<JointType>(i);
			joint.TrackingState = TrackingState::TrackingState_Tracked;
			jointCounter = Gesture::DIMENSIONS_PER_JOINT * i;
			CameraSpacePoint csp;
			csp.X = data.at(frameShiftCounter + jointCounter + 1);
			csp.Y = data.at(frameShiftCounter + jointCounter + 2);
			csp.Z = data.at(frameShiftCounter + jointCounter + 3);
			joint.Position = csp;
			joints.push_back(joint);
		}
		Frame frame{joints};
		gesture.addFrame(frame);
	}
	return gesture;
}
*/

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

void Filereader::readProjectFromFile(std::string & fileName, Project * project, std::vector<GestureClass> * gestureClasses)
{
	//Project project;
	std::vector<ProjectGesture> projectGestures;

	std::ifstream file(fileName);
	std::string fileLine;

	while (std::getline(file, fileLine))
	{
		if (!fileLine.compare(0, Filewriter::projectGestureString.size(), Filewriter::projectGestureString))
		{
			std::vector<double> projectGestureData = convertStringToDoubles(fileLine, Filewriter::projectGestureString);

			//Get the GestureClass from the file.
			std::ostringstream stream;
			stream << projectGestureData.at(1);
			stream << Filewriter::gestureClassExtension;

			readGestureClassFromFile(stream.str(), &(*gestureClasses)[projectGestureData.at(0)]);
			(*gestureClasses)[projectGestureData.at(0)].setGestureClassID(projectGestureData.at(1));
			

			ProjectGesture projectGesture{ (*gestureClasses)[projectGestureData.at(0)] };
			projectGesture.setLabel(projectGestureData.at(0));
			project->addProjectGesture(projectGesture);
		}

		else if (!fileLine.compare(0, Filewriter::actionString.size(), Filewriter::actionString))
		{
			std::vector<double> actionData = convertStringToDoubles(fileLine, Filewriter::actionString);
			double label = actionData.at(0);

			for (ProjectGesture pg : project->getProjectGestures())
			{
				if (pg.getLabel() == label)
				{
					pg.addAction(actionData.at(1), actionData.at(2));
				}
			}
		}
		else
		{
			Console::print("Something went wrong. Can't handle this line.");
		}
	}

	for (ProjectGesture pg : projectGestures)
	{
		Console::print("pg added to project");
		project->addProjectGesture(pg);
	}

	int projectID = std::stoi(fileName.substr(0, fileName.size()-Filewriter::projectExtension.size()));
	project->setProjectID(projectID);
	Console::print("END OF LOADING PROJECT");
	//return project;
}

void Filereader::readGestureClassFromFile(std::string & fileName, GestureClass * gestureClass)
{
	std::ifstream file(fileName);
	std::string fileLine;

	//GestureClass gestureClass;
	std::vector<Gesture> gestures;

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
			gestures.push_back(gesture);
		}
		else
		{
			Console::print("Something went wrong. Can't handle this line.");
		}
	}

	for (Gesture g : gestures)
	{
		gestureClass->addGesture(g);
	}
	//return gestureClass;
}

void Filereader::readGestureFromFile(std::string & fileName, Gesture * gesture)
{
	std::ifstream file(fileName);
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
		else
		{
			Console::print("Something went wrong. Can't handle this line.");
		}
	}

	//Gesture gesture;
	for (Frame f : frames)
	{
		gesture->addFrame(f);
	}
	//return gesture;
}
