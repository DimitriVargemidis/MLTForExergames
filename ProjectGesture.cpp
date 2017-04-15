#include <stdexcept>
#include <windows.h>

#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"

#include "ProjectGesture.h"

ProjectGesture::ProjectGesture()
{
	label = -1;
}

//here the reference as GestureClass is usefull because the gestureClasses are kept in the model and 
//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
// key changed to WORD, the format to enter when pressing a key, see keypress.cpp
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

void ProjectGesture::setKey(const WORD keyToSet) {
	//if (keyToSet == 0) {
	///	throw std::invalid_argument("Key 0x00 is not a valid key.");
	//}
	key = keyToSet;
}

const WORD ProjectGesture::getKey() const {
	return key;
}
