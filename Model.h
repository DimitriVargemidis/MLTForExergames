#ifndef MODEL_H
#define MODEL_H

#include "Project.h"
#include "SVMInterface.h"


class Model {
private:
	Project project;

	bool refresh = false;
	bool start_1 = true;
	bool start_2 = true;
	bool predict = false;


	D2D1_POINT_2F*	savedJointPoints;
	Joint*			savedJoints;

	D2D1_POINT_2F*	savedJointPoints_2;
	Joint*			savedJoints_2;


	double			SVMInputData[54];
	int				SVMInputDataIndex = 0;

	double			SVMLabels[6];
	int				SVMLabelsIndex = 0;

public:
	void setProject(const Project & projectToSet);
	const Project & getProject() const;
	void train();
	const double test();


};

#endif //MODEL_H