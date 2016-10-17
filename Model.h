#ifndef MODEL_H
#define MODEL_H

#include "Project.h"
#include "SVMInterface.h"


class Model {
private:
	Project project;

public:
	void setProject(const Project & project);
	Project getProject() const;
	void train();
	double test();
};

#endif //MODEL_H