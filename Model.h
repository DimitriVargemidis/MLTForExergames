#ifndef MODEL_H
#define MODEL_H

#include <memory>

#include "stdafx.h"
#include "Project.h"
#include "SVMInterface.h"


class Main;
class UI;
class GestureClass;


class Model 
{
private:
	std::shared_ptr<UI>		view;

	Project project;
	std::vector<GestureClass> gestureClasses; //initialised with 2 gesture classes for now


	int				ActiveGestureClassLabel = 0;	//temporary label to identify which gestureclass this gesture belongs to
	bool			refresh = false;			//boolean if the current frame needs to be added to a gestureclass or not
	bool			predict = false;			//boolean if the program is in prediction mode or not
	bool			trained = false;

	WORD	        lastkey = 0;

public:
	Model();
	~Model();

	void				setView(std::shared_ptr<UI> v);

	void				setProject(const Project & projectToSet);
	const Project &		getProject() const;
	void				train();
	const double		test(const Gesture & gesture);

	
	double				SVMInputData[54];
	int					SVMInputDataIndex = 0;

	void				setActiveLabel(int label);
	int					getActiveLabel();
	
	void				setRefresh(bool refresh);
	bool				getRefresh();

	void				setPredict(bool refresh);
	bool				getPredict();

	void				ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);
};

#endif //MODEL_H