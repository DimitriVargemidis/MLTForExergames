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
	std::vector<int>							labelsBuffer;
	std::vector<Frame>							framesBuffer;

	const int		BODY_LOST_LIMIT = 30;				//The amount of frames that the body is able to be lost
	const int		NOT_MOVING_FRAME_DELAY = 38;

	int				activeGestureClassLabel = 0;	//temporary label to identify which gestureclass this gesture belongs to
	int				predictedLabel = -1;
	int				previousPredictedLabel = -1;
	bool			refresh = false;				//boolean if the current frame needs to be added to a gestureclass or not

	bool			predict = false;				//boolean if the program is in prediction mode or not
	bool			trained = false;

	bool			recording = false;
	bool			startedMoving = false;
	bool			initialized = false;
	Frame			frameNeutral;

	int				currentActiveBody = -1	;	//the index of the body that is being tracked
	int				bodyLostCounter = 0		;	//The counter that tracked how long the tracked body has been lost

	//Used in the Processbody function
	std::vector<Frame>		relFrames;			//The vector with frames that are drawn on the screen
	std::vector<Frame>		absFrames;			//The vector with frames that are drawn on the screen

	IBody *					pBody;				//The body that is being processed

	std::chrono::time_point<std::chrono::steady_clock> countDownRef;
	bool			countDownInitiated = false;		//boolean if the count down is already initiated
	int				countDown = 3;
	void			updateCountDown();

	bool			updateUI = false;


public:
	Model();
	~Model();

	void				setView(std::shared_ptr<UI> v);

	std::shared_ptr<Project>	getProject();
	void				train();
	int					test(Frame & frame);

	void				setActiveLabel(int label);
	void				setRefresh(bool refresh);
	bool				getRefresh();
	void				setPredict(bool refresh);
	void				setTrained(bool train);

	void				setRecording(bool record);
	bool				getRecording();

	void				addActionToActive(WORD keycode, bool hold);
	void				addGesture(int label, std::shared_ptr<Gesture> gesture);

	std::vector<Frame>	getRelevantFramesFromBuffer(int offset);

	void				addToLabelsBuffer(int label);
	bool				isGestureExecuted(int offsetLabel, int posInBuffer, int recursiveCounter, int badCounter);
	void				resizeLabelsBuffer();
	void				predictAndExecute(int label);

	std::shared_ptr<GestureClass>		getGestureClassByID(const int & ID);

	void displayFrames();

	void				processBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

	bool				getCountDownInit();
	int					getCountDown();

	void				setUpdatUI(bool update);
	bool				getUpdateUI();

	void				recordGesture(Frame & frame);
	void				addRecordedGesture();

};

#endif //MODEL_H