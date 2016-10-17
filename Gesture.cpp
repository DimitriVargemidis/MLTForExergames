#include "Gesture.h"
#include <stdexcept>


std::vector<Frame> Gesture::getFrames() const {
	return frames;
}

void Gesture::addFrame(const Frame & frameToAdd) {
	if (frameToAdd.getNumberOfJoints() == 0) {
		throw std::invalid_argument("The given frame must contain at least one joint.");
	}
	if ( (getNumberOfFrames() != 0) && (getFrames().at(0).getNumberOfJoints != frameToAdd.getNumberOfJoints()) ) {
		throw std::invalid_argument("Size of the given frame must match the size of the previously added frames.");
	}
	frames.push_back(frameToAdd);
}

int Gesture::getNumberOfFrames() const {
	return frames.size();
}

bool Gesture::isStaticGesture() const {
	return (getNumberOfFrames() == 1);
}
