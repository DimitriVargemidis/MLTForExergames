#ifndef PROJECT_H
#define PROJECT_H

#include "libsvm-3.21\svm.h"
#include <vector>
#include <map>

#include "Action.h"
#include "GestureClass.h"


class Project {
private:
	svm_model		model;
	int				projectID = 0;
	std::string		name = "Project";
	bool			trainedSVM = false;
	int				longestGestureSize = 1;

	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> projectMap;
	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>::iterator projectMapIt;

public:
	const double	GESTURE_SIZE_MARGIN = 1.3;

	Project();

	void setSVMModel(svm_model & modelToSet);
	svm_model & getSVMModel();

	int getProjectID();
	void setProjectID(int ID);
	
	std::string getName();
	void setName(std::string & nameToSet);

	std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & getProjectMap();

	void addNew(int label, std::shared_ptr<GestureClass> gestureClass = nullptr, std::vector<Action> & actions = std::vector<Action>());
	void replaceGestureClass(int label, std::shared_ptr<GestureClass> gestureClass = nullptr);
	void addGesture(int label, Gesture & gesture);
	void replaceActions(int label, std::vector<Action> & actions = std::vector<Action>());
	void addAction(int label, Action & action);
	void addAction(int label, WORD keycode, bool hold);
	bool containsLabel(int label);

	std::shared_ptr<GestureClass> getGestureClass(int label);
	std::vector<Action> & getActions(int label);

	bool hasTrainedSVM();

	int getLongestGestureSize();
	void setLongestGestureSize();

	void activate(int label);
	void deactivate(int label);
};

static int appProjectID;
static int getAppProjectID();

#endif //PROJECT_H