#ifndef PROJECT_H
#define PROJECT_H

#include "ProjectGesture.h"
#include <vector>

class Project {
private:
	std::vector<ProjectGesture> projectGestures;

public:
	void addProjectGesture(const ProjectGesture & projectGesture);
	std::vector<ProjectGesture> getProjectGestures() const;
	void clearProjectGestures();
};

#endif //PROJECT_H