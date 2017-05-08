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

	std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> projectMap;
	std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>>::iterator projectMapIt;

public:
	Project();

	void setSVMModel(svm_model & modelToSet);
	svm_model & getSVMModel();

	int getProjectID();
	void setProjectID(int ID);
	
	std::string getName();
	void setName(std::string nameToSet);

	std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & getProjectMap();

	void addNew(double label, std::shared_ptr<GestureClass> gestureClass = nullptr, std::vector<Action> actions = std::vector<Action>());
	void replaceGestureClass(double label, std::shared_ptr<GestureClass> gestureClass = nullptr);
	void addGesture(double label, Gesture gesture);
	void replaceActions(double label, std::vector<Action> actions = std::vector<Action>());
	void addAction(double label, Action action);
	void addAction(double label, WORD keycode, bool hold);
	bool containsLabel(double label);

	std::shared_ptr<GestureClass> getGestureClass(double label);
	std::vector<Action> getActions(double label);

	bool hasTrainedSVM();

	void activate(double label);
	void deactivate(double label);
};

static int appProjectID;
static int getAppProjectID();

#endif //PROJECT_H