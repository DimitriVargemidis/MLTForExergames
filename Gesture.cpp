#include <stdexcept>
#include <memory>
#include "Gesture.h"
#include "Frame.h"


Gesture::Gesture(std::vector<Frame> framesToAdd) : gestureID{ getAppGestureID() }, frames{ framesToAdd }
{
}

const std::vector<Frame> & Gesture::getFrames() const
{
	return frames;
}

void Gesture::addFrame(Frame & frameToAdd)
{
	//Error check: something went wrong with the joints of this frame
	if (frameToAdd.getNumberOfJoints() == 0)
	{
		throw std::invalid_argument("The given frame must contain at least one joint.");
	}
	//Error check: the number of joints does not correspond with other frames of this gesture.
	if ( (getNumberOfFrames() != 0) && (getFrames().at(0).getNumberOfJoints() != frameToAdd.getNumberOfJoints()) )
	{
		throw std::invalid_argument("Size of the given frame must match the size of the previously added frames.");
	}
	//Add the frame
	frames.push_back(frameToAdd);
}

void Gesture::clearFrames()
{
	frames.clear();
}

const int Gesture::getNumberOfFrames() const
{
	return frames.size();
}

const int Gesture::getNumberOfJointsPerFrame() const
{
	return frames.at(0).getNumberOfJoints();
}

const bool Gesture::isPosture() const
{
	return (getNumberOfFrames() == 1);
}


//Call this method only when all frames are added.
void Gesture::applyRelativeTimestamps()
{
	double timestampOfFirstFrame = getFrames().at(0).getTimestamp();
	for(auto & frame : frames)
	{
		frame.setTimestamp(frame.getTimestamp() - timestampOfFirstFrame);
	}
}

svm_node * Gesture::toArray()
{
	const int dimensions{getNumberOfFrames() * (getNumberOfJointsPerFrame() * DIMENSIONS_PER_JOINT /*+ 1*/)}; //+ 1 for the timestamp as a feature
	const int nbOfJoints{getNumberOfFrames() * getNumberOfJointsPerFrame()};
	svm_node * rawArray{new svm_node[dimensions+1]};
	int indexCount{0};
	for (int i = 0; i < getNumberOfFrames(); i++) {
		for (int j = 0; j < nbOfJoints; j++) {
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.X;
			indexCount = indexCount + 1;
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.Y;
			indexCount = indexCount + 1;
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.Z;
			indexCount = indexCount + 1;
		}
		/*
		rawArray[indexCount].index = indexCount;
		rawArray[indexCount].value = getFrames().at(i).getTimestamp();
		indexCount = indexCount + 1;
		*/
	}
	rawArray[indexCount].index = -1;
	rawArray[indexCount].value = 0;
	return rawArray;
}

int Gesture::getGestureID()
{
	return gestureID;
}

void Gesture::setGestureID(int ID)
{
	gestureID = ID;
}

std::string Gesture::getName()
{
	return name;
}

void Gesture::setName(std::string nameToSet)
{
	name = nameToSet;
}


int getAppGestureID()
{
	appGestureID++;
	return appGestureID;
}
