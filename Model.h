#ifndef MODEL_H
#define MODEL_H

#include "Project.h"
#include "SVMInterface.h"


class Model {
private:
	Main* main;

	Project project;

	bool refresh = false;
	bool start_1 = true;
	bool start_2 = true;
	bool predict = false;


public:
	Model();
	~Model();

	Model(Main * main);

	void				setProject(const Project & projectToSet);
	const Project &		getProject() const;
	void				train();
	const double		test(const Gesture & gesture);

	void				setRefresh(boolean refresh);
	boolean				getRefresh();

	void				setPredict(boolean refresh);
	boolean				getPredict();

	void				ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);
};

#endif //MODEL_H