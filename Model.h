#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"
#include "Project.h"
#include "SVMInterface.h"
#include "Frame.h"



class Main;
class UI;
class GestureClass;


class Model 
{
private:
	std::shared_ptr<UI>		view;

	Project project;
	std::vector<GestureClass> gestureClasses; //initialised with 2 gesture classes for now
	ProjectGesture* lastActiveProjectGesture = nullptr; //the ProjectGesture that was last activated


	int				ActiveGestureClassLabel = 0;	//temporary label to identify which gestureclass this gesture belongs to
	bool			refresh = false;			//boolean if the current frame needs to be added to a gestureclass or not
	bool			predict = false;			//boolean if the program is in prediction mode or not
	bool			trained = false;

	std::chrono::time_point<std::chrono::steady_clock>		timer = Clock::now();

	WORD	        lastkey = 0;

	//ONLY FOR TESTING -- DELETE AFTERWARDS
	/*
	std::shared_ptr<Frame> oldFrame = std::make_shared<Frame>(Frame());
	std::shared_ptr<Frame> newFrame = std::make_shared<Frame>(Frame());
	int counter = 1;
	*/
	//ONLY FOR TESTING -- END

public:
	Model();
	~Model();

	void				setView(std::shared_ptr<UI> v);

	void				setProject(Project & projectToSet);
	Project				getProject();
	void				train();
	double				test(Gesture & gesture);

	
	double				SVMInputData[54];
	int					SVMInputDataIndex = 0;

	void				setActiveLabel(int label);
	int					getActiveLabel();
	
	void				setRefresh(bool refresh);
	bool				getRefresh();

	void				setPredict(bool refresh);
	bool				getPredict();

	void				setTrained(bool train);
	bool				getTrained();

	void				ProcessBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

};

#endif //MODEL_H