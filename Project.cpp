#include "Gesture.h"
#include "Frame.h"
#include "Keypress.h"

#include "Project.h"


Project::Project() : projectID{ getAppProjectID() }
{
}

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

std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>& Project::getProjectMap()
{
	return projectMap;
}

void Project::addNew(int label, std::shared_ptr<GestureClass> gestureClass, std::vector<Action> & actions)
{
	projectMap.insert(std::pair<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>(
		label, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>(gestureClass, actions)));
}

void Project::replaceGestureClass(int label, std::shared_ptr<GestureClass> gestureClass)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first = gestureClass;
}

void Project::addGesture(int label, std::shared_ptr<Gesture> gesture)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first->addGesture(gesture);
}

void Project::replaceActions(int label, std::vector<Action> & actions)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second = actions;
}

void Project::addAction(int label, Action & action)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

void Project::addAction(int label, WORD keycode, bool hold)
{
	Action action;
	action.keycode = keycode;
	action.hold = hold;
	action.active = false;

	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

bool Project::containsLabel(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt == projectMap.end() ? false : true;
}

std::shared_ptr<GestureClass> Project::getGestureClass(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.first;
}

std::vector<Action> & Project::getActions(int label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.second;
}

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
