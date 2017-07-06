#ifndef PROJECT_H
#define PROJECT_H

#include "libsvm-3.21\svm.h"
#include <vector>
#include <map>

#include "Action.h"
#include "GestureClass.h"


class Project {
private:

	//Stores the computed SVM model, if done already. Else, the contents of this variable should not be accessed.
	svm_model		model;
	//Flag that stores if an SVM model is already trained or not. 
	bool			trainedSVM = false;
	//The ID of this projcect. This is increased/set automatically on creation of an object of this class.
	int				projectID = 0;
	//The name of this project. Not yet used in the program, but is saved into the data file.
	std::string		name = "Project";
	//Stores the size of the longest gesture. This is used to know when the labels buffer is full and should be cleared.
	//(unit: number of frames)
	int				longestGestureSize = 1;

	//Variable for the project map. Maps IDs/labels to a pair of gesture classes and actions.
	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> projectMap;
	//Variable for the iterator of the project map. This is only used to boost performance.
	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>::iterator projectMapIt;

public:
	//The factor that decides how many labels can be stored in the buffer before it is considered to be full.
	const double	GESTURE_SIZE_MARGIN = 1.3;

	//Constructor
	Project();

	void setSVMModel(svm_model & modelToSet);
	svm_model & getSVMModel();

	int getProjectID();
	void setProjectID(int ID);
	
	std::string getName();
	void setName(std::string & nameToSet);

	//Important: returns a reference to the project map. Do not return a copy (efficiency).
	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & getProjectMap();

	//Add a new gesture class with a given label/ID, gesture class and action.
	//Input:
	//	- label: can be any int, as long as it is unique in the projectmap. This functions as an ID to get the needed gesture class.
	//	- gesture class: pointer to a gesture class as all gesture classes are stored in the model, not in the project.
	//	- actions: a vector of 'actions'. These is (are) the button(s) that should be pressed when the gesture class is performed and recognized.
	void addNew(int label, std::shared_ptr<GestureClass> gestureClass = nullptr, std::vector<Action> & actions = std::vector<Action>());

	//Switches out one gesture class for another.
	void replaceGestureClass(int label, std::shared_ptr<GestureClass> gestureClass = nullptr);

	//Add a gesture to a gesture class with given label/ID.
	void addGesture(int label, std::shared_ptr<Gesture> gesture);

	//Replace all the actions of the gesture class with given label/ID by the action(s) in the given vector.
	void replaceActions(int label, std::vector<Action> & actions = std::vector<Action>());

	//Add the given action to the actions of the gesture class with given label/ID.
	void addAction(int label, Action & action);

	//Add an action to the gesture class with given label/ID, but rather than giving an Action object, give the
	//key that should be pressed and if that button should be pressed shortly or continuously.
	void addAction(int label, WORD keycode, bool hold);

	//Check if the given label/ID exists and is used within this project.
	bool containsLabel(int label);

	//Return the gesture class with given label/ID.
	std::shared_ptr<GestureClass> getGestureClass(int label);

	//Get all the actions related to the gesture class with given label/ID.
	std::vector<Action> & getActions(int label);

	//Checks if an SVM model is already trained.
	bool hasTrainedSVM();

	int getLongestGestureSize();
	void setLongestGestureSize();

	//Activate the actions of the gesture class with given label/ID.
	void activate(int label);

	//Deactive the actions of the gesture class with given label/ID. Only necessary if the gesture class
	//contains any actions that require a key to be pressed and hold. This check is done automatically.
	void deactivate(int label);
};

//ID of this project. Used for storing data into a folder with a unique name.
static int appProjectID;
static int getAppProjectID();

#endif //PROJECT_H