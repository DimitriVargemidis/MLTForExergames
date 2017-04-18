
#include "ProjectGesture.h"
#include "Gesture.h"
#include "Frame.h"

#include "Project.h"

//deleted the reference argument
void Project::addProjectGesture(const ProjectGesture  projectGesture) {

	//if the label of the found projectGesture is -1 no projectGesture is found
	int label = getProjectGestureFromLabel(projectGesture.getLabel()).getLabel();
	if (label == -1)
	{
		projectGestures.push_back(projectGesture);
	}
	
}

const std::vector<ProjectGesture> & Project::getProjectGestures() const {
	return projectGestures;
}

const ProjectGesture & Project::getProjectGestureFromLabel(double label)
{
	for (int i = 0; i < projectGestures.size(); i++)
	{
		if (label == projectGestures[i].getLabel())
		{
			return projectGestures[i];
		}
	}

	return EmptyProjectGesture;
}

void Project::clearProjectGestures() {
	projectGestures.clear();
}

void Project::setSVMModel(const svm_model modelToSet) {
	model = modelToSet;
}

const svm_model & Project::getSVMModel() const {
	return model;
}

const Project & Project::operator=(const Project & projectObject)
{
	return projectObject;
}
