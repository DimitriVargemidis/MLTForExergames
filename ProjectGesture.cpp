#include <stdexcept>
#include <windows.h>

#include "Keypress.h"
#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"

#include "ProjectGesture.h"

ProjectGesture::ProjectGesture() : gestureClass{ GestureClass() }
{
	label = -1;
}

//here the reference as GestureClass is useful because the gestureClasses are kept in the model and 
//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
// key changed to WORD, the format to enter when pressing a key, see keypress.cpp
ProjectGesture::ProjectGesture(GestureClass & gestureClass, const double label, const WORD key, bool holding):
	gestureClass{ gestureClass }, label{ label }
{
	Action a;
	a.keycode = key;
	a.hold = holding;
	a.active = false;

	actions.push_back(a);
}

GestureClass ProjectGesture::getGestureClass()
{
	return gestureClass;
}

void ProjectGesture::setLabel(const double labelToSet)
{
	label = labelToSet;
}

const double ProjectGesture::getLabel() const
{
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
void ProjectGesture::addAction(WORD keycode, bool hold)
{
	Action a;
	a.keycode = keycode;
	a.hold = hold;
	a.active = false;

	actions.push_back(a);
}

/*
void ProjectGesture::setKey(const WORD keyToSet) {
	//if (keyToSet == 0) {
	///	throw std::invalid_argument("Key 0x00 is not a valid key.");
	//}
	key = keyToSet;
}
*/

const WORD ProjectGesture::getKey() const
{
	if (actions.size() == 0)
	{
		return 0;
	}

	return actions.front().keycode;
}

void ProjectGesture::Activate()
{
	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].hold == false)
		{
			Keypress::pressKey(actions[i].keycode);
		}
		if (actions[i].hold == true && actions[i].active == false)
		{
			Keypress::keyDown(actions[i].keycode);
			actions[i].active = true;
		}
	}

}

void ProjectGesture::Deactivate()
 {
	for (int i = 0; i < actions.size(); i++)
	{
		if (actions[i].hold == true && actions[i].active == true)
		{
			Keypress::keyUp(actions[i].keycode);
			actions[i].active = false;
		}
	}


}
