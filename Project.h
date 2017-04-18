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

	ProjectGesture EmptyProjectGesture; //used when a projectGesture is requested but none is found

public:
	void addProjectGesture(const ProjectGesture projectGesture); //deleted the reference argument
	const std::vector<ProjectGesture> & getProjectGestures() const;
	const ProjectGesture & getProjectGestureFromLabel(double label);
	void clearProjectGestures();
	//Deleted the reference 
	void setSVMModel(const svm_model modelToSet);
	const svm_model & getSVMModel() const;

	Project& operator=(const Project& projectObject);
};

#endif //PROJECT_H