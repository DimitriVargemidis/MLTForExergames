#ifndef MODEL_H
#define MODEL_H

#include "Project.h"
#include "SVMInterface.h"


class Model {
private:
	Project project;

public:
	void setProject(const Project & projectToSet);
	const Project & getProject() const;
	void train();
	const double test();
};

#endif //MODEL_H