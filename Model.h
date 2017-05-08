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
	std::shared_ptr<UI>							view;
	std::shared_ptr<Project>					activeProject;

	std::vector<std::shared_ptr<Project>>		projects;
	std::vector<std::shared_ptr<GestureClass>>	gestureClasses;
	std::vector<double>							labelsBuffer;
	std::vector<Frame>							framesBuffer;

	const int		maxBufferSize = 30;				//The number of labels to be stored in the labels buffer
	int				activeGestureClassLabel = 0;	//temporary label to identify which gestureclass this gesture belongs to
	double			predictedLabel = -1;
	double			previousPredictedLabel = -1;
	bool			refresh = false;				//boolean if the current frame needs to be added to a gestureclass or not
	bool			predict = false;				//boolean if the program is in prediction mode or not
	bool			trained = false;
	
	bool			recording = false;
	bool			startedMoving = false;
	bool			initialized = false;
	Frame			frameNeutral;

	int				currentActiveBody = -1	;	//the index of the body that is being tracked
	int				bodyLostCounter = 0		;	//The counter that tracked how long the tracked body has been lost
	int				bodyLostLimit;

	//used in the Processbody function
	std::vector<Frame>		relFrames;			//The vector with frames that are drawn on the screen
	std::vector<Frame>		absFrames;			//The vector with frames that are drawn on the screen

	IBody *					pBody;				//The body that is being processed
	

public:
	Model();
	~Model();

	void				setView(std::shared_ptr<UI> v);

	std::shared_ptr<Project>	getProject();
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

	void				addActionToActive(WORD keycode, bool hold);
	void				deleteKeyFromActive(WORD keycode);

	void				addGesture(double label, Gesture gesture);

	void				addToFramesBuffer(Frame frame);
	std::vector<Frame>	getRelevantFramesFromBuffer();

	void				addToLabelsBuffer(double label);
	double				getMostFrequentLabel();

	void displayFrames();

	void				processBody(INT64 nTime, int nBodyCount, IBody** ppBodies);
	void				recordGesture(Frame frame);
};

#endif //MODEL_H