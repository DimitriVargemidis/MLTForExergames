#include <fstream>
#include <sstream>

#include "Console.h"
#include "Frame.h"
#include "Filewriter.h"


void Filewriter::save(Gesture & gesture)
{
	std::ostringstream sstream;
	sstream << gesture.getGestureID();
	std::string fileName = sstream.str();
	fileName.append(gestureExtension);
	overwrite(fileName, std::string());

	for (const Frame & frame : gesture.getFrames())
	{
		std::string frameLine;
		frameLine.append(frameString);
		frameLine.append(" ");
		std::ostringstream frameSStream;
		for (const Joint & joint : frame.getJoints())
		{
			frameSStream << joint.Position.X << " ";
			frameSStream << joint.Position.Y << " ";
			frameSStream << joint.Position.Z << " ";
		}
		frameSStream << frame.getTimestamp();
		frameLine.append(frameSStream.str());

		write(fileName, frameLine);
	}
}

void Filewriter::save(GestureClass & gestureClass)
{
	std::ostringstream sstream;
	sstream << gestureClass.getGestureClassID();
	std::string fileName = sstream.str();
	fileName.append(gestureClassExtension);
	overwrite(fileName, std::string());

	for (Gesture & gesture : gestureClass.getGestures())
	{
		save(gesture);
		
		std::string gestureLine;
		gestureLine.append(gestureString);
		gestureLine.append(" ");

		std::ostringstream gestureSStream;
		gestureSStream << gesture.getGestureID();
		gestureLine.append(gestureSStream.str());
		write(fileName, gestureLine);
	}
}

void Filewriter::save(Project & project)
{
	std::ostringstream sstream;
	sstream << project.getProjectID();
	std::string fileName = sstream.str();
	fileName.append(projectExtension);
	overwrite(fileName, std::string());

	for (ProjectGesture & projectGesture : project.getProjectGestures())
	{
		save(projectGesture.getGestureClass());
		
		std::string projectLine;
		projectLine.append(projectGestureString);
		projectLine.append(" ");
		
		std::ostringstream projectSStream;
		projectSStream << projectGesture.getLabel() << " ";
		projectSStream << projectGesture.getGestureClass().getGestureClassID();
		projectLine.append(projectSStream.str());

		write(fileName, projectLine);

		for (Action & action : projectGesture.getActions())
		{
			std::string actionLine;
			actionLine.append(actionString);
			actionLine.append(" ");

			std::ostringstream actionSStream;
			actionSStream << projectGesture.getLabel() << " ";
			actionSStream << std::to_string(action.keycode) << " ";
			actionSStream << std::to_string(action.hold);
			actionLine.append(actionSStream.str());

			write(fileName, actionLine);
		}
	}
}

void Filewriter::write(std::string & fileName, std::string & data)
{
	std::ostringstream fNameStream;
	fNameStream << subDirectoryString << fileName;
	std::ofstream file(fNameStream.str(), std::fstream::app); //app: 'append', adds to the same file instead of overwriting.
	file << data << std::endl;
}

void Filewriter::overwrite(std::string & fileName, std::string & data, bool endWithNewline)
{
	std::ostringstream fNameStream;
	fNameStream << subDirectoryString << fileName;
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


