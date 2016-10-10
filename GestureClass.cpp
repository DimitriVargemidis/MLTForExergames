#include "GestureClass.h"
#include <stdexcept>


void GestureClass::addGesture(const Gesture & gesture) {
	if (gesture.getNumberOfJointsPerFrame() == 0) {
		throw std::invalid_argument("The given gesture does not contain any Joints.");
	}
	if ( (gestures.size() != 0) && (gestures.at(0).getNumberOfJointsPerFrame() != gesture.getNumberOfJointsPerFrame()) ) {
		throw std::invalid_argument("The given gesture cannot belong to this GestureClass.");
	}
	gestures.push_back(gesture);
}

std::vector<Gesture> GestureClass::getGestures() const {
	return std::vector<Gesture>();
}
