#include <stdexcept>
#include <windows.h>

#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"

#include "ProjectGesture.h"

ProjectGesture::ProjectGesture():
	gestureClass{ GestureClass() }
{
	label = -1;
}

//here the reference as GestureClass is usefull because the gestureClasses are kept in the model and 
//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
// key changed to WORD, the format to enter when pressing a key, see keypress.cpp
ProjectGesture::ProjectGesture( GestureClass & gestureClass, const double label, const WORD key) :
	gestureClass{gestureClass}, label{ label } {
	//setKey(key);

	Action a;
	a.keycode = 0x20;
	a.hold = false;

	actions.push_back(a);
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

void ProjectGesture::setActions(std::vector<Action> a)
{
	actions = a;
}

std::vector<Action> & ProjectGesture::getActions()
{
	return actions;
}
/*
void ProjectGesture::setKey(const WORD keyToSet) {
	//if (keyToSet == 0) {
	///	throw std::invalid_argument("Key 0x00 is not a valid key.");
	//}
	key = keyToSet;
}
*/

const WORD ProjectGesture::getKey() const {

	if (actions.size() == 0)
	{
		return 0;
	}

	return actions.front().keycode;
}
