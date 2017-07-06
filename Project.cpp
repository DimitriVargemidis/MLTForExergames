#include "Gesture.h"
#include "Frame.h"
#include "Keypress.h"

#include "Project.h"


Project::Project() : projectID{ getAppProjectID() }
{
}

//===============
//Getters/setters
//===============

void Project::setSVMModel(svm_model & modelToSet)
{
	model = modelToSet;
	trainedSVM = true;
}

svm_model & Project::getSVMModel()
{
	return model;
}

int Project::getProjectID()
{
	return projectID;
}

void Project::setProjectID(int ID)
{
	projectID = ID;
}

std::string Project::getName()
{
	return name;
}

void Project::setName(std::string & nameToSet)
{
	name = nameToSet;
}

//Important: returns a reference to the project map. Do not return a copy (efficiency).
std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>& Project::getProjectMap()
{
	return projectMap;
}

//Add a new gesture class with a given label/ID, gesture class and action.
//Input:
//	- label: can be any int, as long as it is unique in the projectmap. This functions as an ID to get the needed gesture class.
//	- gesture class: pointer to a gesture class as all gesture classes are stored in the model, not in the project.
//	- actions: a vector of 'actions'. These is (are) the button(s) that should be pressed when the gesture class is performed and recognized.
void Project::addNew(int label, std::shared_ptr<GestureClass> gestureClass, std::vector<Action> & actions)
{
	projectMap.insert(std::pair<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>(
		label, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>(gestureClass, actions)));
}

//Switches out one gesture class for another.
void Project::replaceGestureClass(int label, std::shared_ptr<GestureClass> gestureClass)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first = gestureClass;
}

//Add a gesture to a gesture class with given label/ID.
void Project::addGesture(int label, std::shared_ptr<Gesture> gesture)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first->addGesture(gesture);
}

//Replace all the actions of the gesture class with given label/ID by the action(s) in the given vector.
void Project::replaceActions(int label, std::vector<Action> & actions)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second = actions;
}

//Add the given action to the actions of the gesture class with given label/ID.
void Project::addAction(int label, Action & action)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

//Add an action to the gesture class with given label/ID, but rather than giving an Action object, give the
//key that should be pressed and if that button should be pressed shortly or continuously.
void Project::addAction(int label, WORD keycode, bool hold)
{
	Action action;
	action.keycode = keycode;
	action.hold = hold;
	action.active = false;

	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

//Check if the given label/ID exists and is used within this project.
bool Project::containsLabel(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt == projectMap.end() ? false : true;
}

//Return the gesture class with given label/ID.
std::shared_ptr<GestureClass> Project::getGestureClass(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.first;
}

//Get all the actions related to the gesture class with given label/ID.
std::vector<Action> & Project::getActions(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.second;
}

//Checks if an SVM model is already trained.
bool Project::hasTrainedSVM()
{
	return trainedSVM;
}

int Project::getLongestGestureSize()
{
	return longestGestureSize;
}

void Project::setLongestGestureSize()
{
	for (const auto & key : projectMap)
	{
		for (const auto & gesture : key.second.first->getGestures())
		{
			int gestureSize = gesture->getFrames().size();
			if (gestureSize > longestGestureSize)
			{
				longestGestureSize = gestureSize;
			}
		}
	}
	longestGestureSize = longestGestureSize * GESTURE_SIZE_MARGIN;
}

//Activate the actions of the gesture class with given label/ID.
void Project::activate(int label)
{
	if (containsLabel(label))
	{
		for (auto & action : getActions(label))
		{
			if (!action.active)
			{
				if (action.hold)
				{
					Keypress::keyDown(action.keycode);
				}
				else
				{
					Keypress::pressKey(action.keycode);
				}
				action.active = true;
			}
		}
	}
}

//Deactive the actions of the gesture class with given label/ID. Only necessary if the gesture class
//contains any actions that require a key to be pressed and hold. This check is done automatically.
void Project::deactivate(int label)
{
	if (containsLabel(label))
	{
		for (auto & action : getActions(label))
		{
			if (action.active)
			{
				if (action.hold)
				{
					Keypress::keyUp(action.keycode);
				}
				action.active = false;
			}
		}
	}
}


int getAppProjectID()
{
	appProjectID++;
	return appProjectID;
}
