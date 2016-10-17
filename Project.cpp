#include "Project.h"


void Project::addProjectGesture(const ProjectGesture & projectGesture) {
	projectGestures.push_back(projectGesture);
}

std::vector<ProjectGesture> Project::getProjectGestures() const {
	return std::vector<ProjectGesture>();
}

void Project::clearProjectGestures() {
	projectGestures.clear();
}

void Project::setSVMModel(const svm_model & modelToSet) {
	model = modelToSet;
}

svm_model Project::getSVMModel() const {
	return model;
}
