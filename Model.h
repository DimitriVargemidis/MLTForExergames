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
	//The view associated to this model (Model-View pattern).
	std::shared_ptr<UI>							view;
	//A pointer to the active project. Do not make a copy!
	std::shared_ptr<Project>					activeProject;
	//A vector of all projects. Currently not used. Can be used to load multiple projects and let the user choose which one to work with.
	std::vector<std::shared_ptr<Project>>		projects;
	//A vector of all gesture classes, including the ones from different projects. Allows to mix, match and reuse gesture classes from different projects.
	std::vector<std::shared_ptr<GestureClass>>	gestureClasses;
	//A vector of all recently predicted labels. Used to evaluate if a gesture (class) is recognized.
	std::vector<int>							labelsBuffer;
	//A vector of frames, used to keep track of frames while recording a gesture.
	std::vector<Frame>							framesBuffer;

	//The amount of frames that the body is able to be lost.
	const int		BODY_LOST_LIMIT = 30;
	//The number of frames that the user should stand still for the application to recognize it as standing still.
	const int		NOT_MOVING_FRAME_DELAY = 38;
	//Temporary label to identify which gestureclass this gesture belongs to.
	int				activeGestureClassLabel = 0;
	//Variable that stores the latest predicted label.
	int				predictedLabel = -1;
	//Variable that stores the previously predicted label.
	int				previousPredictedLabel = -1;
	//Checks if the current frame needs to be added to a gestureclass or not.
	bool			refresh = false;
	//Checks if the program is in prediction mode or not.
	bool			predict = false;
	//Checks if the SVM model is trained.
	bool			trained = false;

	//Checks if the recording phase is active.
	bool			recording = false;
	//Checks if the user started moving during the recording phase.
	bool			startedMoving = false;
	//Checks if initialization is done during the recording phase.
	bool			initialized = false;
	//Variable storing the 'neutral' frame. Used for comparison to check if the used has moved.
	Frame			frameNeutral;

	//The index of the body that is being tracked.
	int				currentActiveBody = -1;
	//The counter that tracked how long the tracked body has been lost.
	int				bodyLostCounter = 0	;

	//Used in the processBody function
	std::vector<Frame>		relFrames;			//The vector with frames that are drawn on the screen
	std::vector<Frame>		absFrames;			//The vector with frames that are drawn on the screen

	IBody *					pBody;				//The body that is being processed

	//Timer used for the countdown.
	std::chrono::time_point<std::chrono::steady_clock> countDownRef;
	//Checks if the count down is already initiated.
	bool			countDownInitiated = false;
	int				countDown = 3;
	void			updateCountDown();
	//Checks if the UI needs to be updated.
	bool			updateUI = false;


public:
	Model();
	~Model();

	void				setView(std::shared_ptr<UI> v);

	std::shared_ptr<Project>	getProject();
	//Train an SVM model using all gesture classes stored in the currently active project.
	void				train();
	//Use the trained SVM model to predict the label of the given frame.
	int					test(Frame & frame);

	void				setActiveLabel(int label);
	void				setRefresh(bool refresh);
	bool				getRefresh();
	void				setPredict(bool refresh);
	void				setTrained(bool train);

	void				setRecording(bool record);
	bool				getRecording();

	//Add an action to the currently active project and gesture class of that project.
	void				addActionToActive(WORD keycode, bool hold);
	//Add a gesture to the currently active project and to the gesture class with the given label/ID.
	void				addGesture(int label, std::shared_ptr<Gesture> gesture);

	//Get a subset of frames from the framesBuffer, depending on the given offset.
	std::vector<Frame>	getRelevantFramesFromBuffer(int offset);

	//Add a new prediction (label) to the buffer.
	void				addToLabelsBuffer(int label);
	//Resize the labels buffer when it is full. This depends on the recorded gesture with the longest duration.
	void				resizeLabelsBuffer();
	//Recursive function that checks if a gesture is executed. This is the most important part of the gesture recognition algorithms.
	bool				isGestureExecuted(std::shared_ptr<Gesture> & gesture, int posInBuffer, int recursiveCounter);
	//Checks for all gesture classes if they are recognized. Executes linked actions to the recognized gesture class.
	void				predictAndExecute(int label);

	std::shared_ptr<GestureClass>		getGestureClassByID(const int & ID);

	void displayFrames();

	//Forms part of the main loop of the program. This loop can NOT be blocking, otherwise the UI will not refresh and the sensor
	//will not collect new data!
	void processBody(INT64 nTime, int nBodyCount, IBody** ppBodies);

	bool getCountDownInit();
	int	getCountDown();

	void setUpdatUI(bool update);
	bool getUpdateUI();
	
	//Used for recording gestures with the given frame being the newest frame retrieved from the sensor.
	void recordGesture(Frame & frame);
	//This function is called when recording of a gesture is done.
	void addRecordedGesture();

};

#endif //MODEL_H