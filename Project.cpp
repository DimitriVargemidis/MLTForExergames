#include "Project.h"


void Project::addProjectGesture(const ProjectGesture & projectGesture) {
	projectGestures.push_back(projectGesture);
}

const std::vector<ProjectGesture> & Project::getProjectGestures() const {
	return projectGestures;
}

void Project::clearProjectGestures() {
	projectGestures.clear();
}

void Project::setSVMModel(const svm_model & modelToSet) {
	model = modelToSet;
}

const svm_model & Project::getSVMModel() const {
	return model;
}
