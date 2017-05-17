#include <stdexcept>
#include "Gesture.h"
#include "Frame.h"

#include "GestureClass.h"


GestureClass::GestureClass() : gestureClassID{ getAppGestureClassID() }
{
}

GestureClass::GestureClass(const std::shared_ptr<Gesture> gesture) : gestureClassID{ getAppGestureClassID() }
{
	addGesture(gesture);
}

void GestureClass::addGesture(const std::shared_ptr<Gesture> gesture) {
	gestures.push_back(gesture);
}

std::vector<std::shared_ptr<Gesture>> & GestureClass::getGestures() {
	return gestures;
}

int GestureClass::getGestureClassID()
{
	return gestureClassID;
}

void GestureClass::setGestureClassID(int ID)
{
	gestureClassID = ID;
}

std::string GestureClass::getName()
{
	return name;
}

void GestureClass::setName(std::string & nameToSet)
{
	name = nameToSet;
}

void GestureClass::DeleteGestureWithID(int ID)
{
	for (int i = 0; i < gestures.size(); i++)
	{
			if (gestures[i]->getGestureID() == ID)
			{
				gestures.erase(gestures.begin() + i);
				return;
			}
	}
}

std::shared_ptr<Gesture>   GestureClass::getGestureWithID(const int ID)
{
	for (int i = 0; i < gestures.size(); i++)
	{
		if (gestures[i]->getGestureID() == ID)
		{
			return gestures[i];
		}
	}
	//return the last if nothing is found
	return nullptr;
}

int getAppGestureClassID()
{
	appGestureClassID++;
	return appGestureClassID;
}
