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

void Project::setName(std::string nameToSet)
{
	name = nameToSet;
}

std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>& Project::getProjectMap()
{
	return projectMap;
}

void Project::addNew(double label, std::shared_ptr<GestureClass> gestureClass, std::vector<Action> actions)
{
	projectMap.insert(std::pair<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>(
		label, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>(gestureClass, actions)));
}

void Project::replaceGestureClass(double label, std::shared_ptr<GestureClass> gestureClass)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first = gestureClass;
}

void Project::addGesture(double label, Gesture gesture)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.first->addGesture(gesture);
}

void Project::replaceActions(double label, std::vector<Action> actions)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second = actions;
}

void Project::addAction(double label, Action action)
{
	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

void Project::addAction(double label, WORD keycode, bool hold)
{
	Action action;
	action.keycode = keycode;
	action.hold = hold;
	action.active = false;

	projectMapIt = projectMap.find(label);
	projectMapIt->second.second.push_back(action);
}

bool Project::containsLabel(double label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt == projectMap.end() ? false : true;
}

std::shared_ptr<GestureClass> Project::getGestureClass(double label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.first;
}

std::vector<Action> Project::getActions(double label)
{
	projectMapIt = projectMap.find(label);
	return projectMapIt->second.second;
}

bool Project::hasTrainedSVM()
{
	return trainedSVM;
}

void Project::activate(double label)
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

void Project::deactivate(double label)
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
