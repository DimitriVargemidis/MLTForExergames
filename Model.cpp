
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
	
}

const double Model::test() {
	return 0.0;
}
