#include <stdexcept>
#include "Model.h"


void Model::setProject(const Project & projectToSet) {
	if (projectToSet.getProjectGestures().size() == 0) {
		throw std::invalid_argument("The given project does not contain any ProjectClasses.");
	}
	project = projectToSet;
}

const Project & Model::getProject() const {
	return project;
}

void Model::train() {
	project.setSVMModel(*SVMInterface::train(getProject().getProjectGestures()));
}

const double Model::test(const Gesture & gesture) {
	return SVMInterface::test(getProject().getSVMModel(), gesture);
}
