#include "Model.h"
#include <stdexcept>


void Model::setProject(const Project & projectToSet) {
	if (project.getProjectGestures.size() == 0) {
		throw std::invalid_argument("The given project does not contain any ProjectClasses.");
	}
	project = projectToSet;
}

Project Model::getProject() const {
	return Project();
}

void Model::train() {
	
}

double Model::test() {
	return 0.0;
}
