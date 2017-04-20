#include <stdexcept>
#include "Gesture.h"
#include "Frame.h"

#include "GestureClass.h"


GestureClass::GestureClass()
{
}

GestureClass::GestureClass(const Gesture gesture)
{
	addGesture(gesture);
}

void GestureClass::addGesture(const Gesture gesture) {
	if (gesture.getNumberOfFrames() == 0) {
		throw std::invalid_argument("The given gesture does not contain any Frames.");
	}
	if ((gestures.size() != 0) && (gestures.at(0).getNumberOfFrames() != gesture.getNumberOfFrames())) {
		throw std::invalid_argument("The given gesture cannot belong to this GestureClass (diff nb of Frames).");
	}
	if ( (gestures.size() != 0) && (gestures.at(0).getNumberOfJointsPerFrame() != gesture.getNumberOfJointsPerFrame()) ) {
		throw std::invalid_argument("The given gesture cannot belong to this GestureClass (diff nb of Joints).");
	}
	gestures.push_back(gesture);
}

std::vector<Gesture> GestureClass::getGestures() {
	return gestures;
}
