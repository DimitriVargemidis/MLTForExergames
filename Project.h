#ifndef PROJECT_H
#define PROJECT_H

#include "libsvm-3.21\svm.h"
#include <vector>

#include "Action.h"
#include "ProjectGesture.h"


class Project {
private:
	std::vector<ProjectGesture> projectGestures;
	svm_model model;
	int projectID = 0;

	ProjectGesture EmptyProjectGesture; //used when a projectGesture is requested but none is found

public:
	Project();

	void addProjectGesture(ProjectGesture projectGesture);
	std::vector<ProjectGesture> getProjectGestures();
	ProjectGesture & getProjectGestureFromLabel(double label);
	void clearProjectGestures();

	void setSVMModel(svm_model & modelToSet);
	svm_model & getSVMModel();

	const Project & operator=(const Project & projectObject);

	int getProjectID();
	void setProjectID(int ID);
};

static int appProjectID;
static int getAppProjectID();

#endif //PROJECT_H