#include <stdexcept>
#include <memory>
#include "Gesture.h"
#include "Frame.h"


Gesture::Gesture(Frame & frameToAdd) : gestureID{ getAppGestureID() }
{
	addFrame(frameToAdd);
}

Gesture::Gesture(std::vector<Frame> & framesToAdd) : gestureID{ getAppGestureID() }, frames{ framesToAdd }
{
}

std::vector<Frame> & Gesture::getFrames()
{
	return frames;
}

//Add the given frame to this gesture.
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

//Clear all frames of this gesture.
void Gesture::clearFrames()
{
	frames.clear();
}

//Return the number of frames this gesture has.
const int Gesture::getNumberOfFrames() const
{
	return frames.size();
}

//Returns true if this gesture object is a posture, thus contains less than POSTURE_THRESHOLD frames.
const bool Gesture::isPosture() const
{
	return frames.size() < POSTURE_THRESHOLD;
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

void Gesture::setName(std::string & nameToSet)
{
	name = nameToSet;
}

//Set the vector with labels that specify this gesture.
void Gesture::setLabelOrder(std::vector<int> labelOrderToSet)
{
	labelOrder = labelOrderToSet;
}

//Add a label to the label order vector.
void Gesture::addLabel(int labelToAdd)
{
	labelOrder.push_back(labelToAdd);
}

std::vector<int> & Gesture::getLabelOrder()
{
	return labelOrder;
}


int getAppGestureID()
{
	appGestureID++;
	return appGestureID;
}
