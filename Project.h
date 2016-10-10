#ifndef PROJECT_H
#define PROJECT_H

#include "ProjectGesture.h"
#include "libsvm-3.21\svm.h"
#include <vector>

class Project {
private:
	std::vector<ProjectGesture> projectGestures;
	svm_model model;

public:
	void addProjectGesture(const ProjectGesture & projectGesture);
	std::vector<ProjectGesture> getProjectGestures() const;
	void clearProjectGestures();
	void setSVMModel(const svm_model & modelToSet);
	svm_model getSVMModel() const;
};

#endif //PROJECT_H