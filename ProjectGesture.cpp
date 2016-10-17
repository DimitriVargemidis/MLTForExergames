#include <stdexcept>
#include "ProjectGesture.h"


ProjectGesture::ProjectGesture(const GestureClass & gestureClass, const double label, const unsigned int key) :
	gestureClass{ gestureClass }, label{ label } {
	setKey(key);
}

const GestureClass & ProjectGesture::getGestureClass() const {
	return gestureClass;
}

void ProjectGesture::setLabel(const double labelToSet) {
	label = labelToSet;
}

const double ProjectGesture::getLabel() const {
	return label;
}

void ProjectGesture::setKey(const unsigned int keyToSet) {
	if (keyToSet == 0) {
		throw std::invalid_argument("Key 0x00 is not a valid key.");
	}
	key = keyToSet;
}

const unsigned int ProjectGesture::getKey() const {
	return key;
}
