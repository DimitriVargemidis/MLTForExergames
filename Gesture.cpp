#include "Gesture.h"
#include <stdexcept>


std::vector<std::vector<Joint>> Gesture::getJoints() const {
	return joints;
}

void Gesture::addFrame(const std::vector<Joint> & frameJoints) {
	if (frameJoints.size() == 0) {
		throw std::invalid_argument("The given frame must contain at least one joint.");
	}
	if ( (joints.size() != 0) && (getNumberOfJointsPerFrame() != frameJoints.size()) ) {
		throw std::invalid_argument("Size of the given frame must match the size of the previously added frames.");
	}
	joints.push_back(frameJoints);
}

bool Gesture::isStaticGesture() const {
	return (joints.size() == 1);
}

int Gesture::getNumberOfJointsPerFrame() const {
	return joints.at(0).size();
}
