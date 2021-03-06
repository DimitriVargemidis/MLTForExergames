#include <stdexcept>
#include <vector>
#include <windows.h>
#include <string>
#include <algorithm>

#include "Main.h"
#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"
#include "Keypress.h"
#include "Console.h"
#include "Filewriter.h"
#include "Filereader.h"
#include "Model.h"


void Model::updateCountDown()
{
	if (!countDownInitiated)
	{
		countDownInitiated = true;
		countDownRef = Clock::now();
		countDown = 4;
		updateUI = true;
	}
	else
	{
		auto t2 = Clock::now();
		long time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - countDownRef).count();

		if (time >= 1000 && countDown > 3)
		{
			--countDown;
			updateUI = true;		//The UI will update its state based on the countDown parameter
		}
		else if (time >= 2000 && countDown > 2)
		{
			--countDown;
			updateUI = true;		//The UI will update its state based on the countDown parameter
		}
		else if (time >= 3000 && countDown > 1)
		{
			--countDown;
			updateUI = true;		//The UI will update its state based on the countDown parameter
		}
		else if ((time >= 4000 && countDown > 0))
		{
			--countDown;
			framesBuffer.clear();
			recording = true;
			updateUI = true;		//The UI will update its state based on the countDown parameter
		}
		else if (recording == false && countDown == 0)
		{
			--countDown;
			countDownRef = Clock::now();
			updateUI = true;		//The UI will update its state based on the countDown parameter
		}
		else if (time >= 500 && countDown == -1)
		{
			--countDown;
			refresh = false;
			countDownInitiated = false;
			updateUI = true;
		}
	}
}

Model::Model()
{
	Filereader::loadAllData(&projects, &gestureClasses);
	if (projects.empty())		//If no project is loaded, else do nothing.
	{
		projects.push_back(std::make_shared<Project>());
	}
	activeProject = projects.back();
}

Model::~Model()
{
}

void Model::setView(std::shared_ptr<UI> v)
{
	view = v;
}

std::shared_ptr<Project> Model::getProject()
{
	return activeProject;
}

//Train an SVM model using all gesture classes stored in the currently active project.
//After this function is called, all project data is saved into a folder called DATA.
void Model::train()
{
	activeProject->setSVMModel(*(SVMInterface::train(activeProject->getProjectMap())));
	Filewriter::save(activeProject);
	activeProject->setLongestGestureSize();
}

//Use the trained SVM model to predict the label of the given frame.
int Model::test(Frame & frame)
{
	return SVMInterface::test(activeProject->getSVMModel(), frame);
}

void Model::setActiveLabel(int label)
{
	activeGestureClassLabel = label;
}

void Model::setRefresh(bool refresh)
{
	this->refresh = refresh;
}

bool Model::getRefresh()
{
	return refresh;
}

void Model::setPredict(bool refresh)
{
	this->predict = refresh;
}

void Model::setTrained(bool train)
{
	trained = train;
}

void Model::setRecording(bool record)
{
	recording = record;
}

bool Model::getRecording()
{
	return recording;
}

//Add an action to the currently active project and gesture class of that project.
void Model::addActionToActive(WORD keycode, bool hold)
{
	activeProject->addAction(activeGestureClassLabel, keycode, hold);
}

//Add a gesture to the currently active project and to the gesture class with the given label/ID.
void Model::addGesture(int label, std::shared_ptr<Gesture> gesture)
{
	if (activeProject->containsLabel(label))
	{
		activeProject->addGesture(label, gesture);
	}
	else
	{
		gestureClasses.push_back(std::make_shared<GestureClass>(gesture));
		activeProject->addNew(label, gestureClasses.back());
	}
	updateUI = true;
}

//Get a subset of frames from the framesBuffer, depending on the given offset.
std::vector<Frame> Model::getRelevantFramesFromBuffer(int offset)
{
	return std::vector<Frame>(framesBuffer.begin(), framesBuffer.end() - offset);
}

//Add a new prediction (label) to the buffer.
void Model::addToLabelsBuffer(int label)
{
	labelsBuffer.push_back(label);

	if (labelsBuffer.size() > activeProject->getLongestGestureSize())
	{
		resizeLabelsBuffer();
	}
}

//Resize the labels buffer when it is full. This depends on the recorded gesture with the longest duration.
void Model::resizeLabelsBuffer()
{
	labelsBuffer = std::vector<int>(labelsBuffer.end() - activeProject->getLongestGestureSize(), labelsBuffer.end());
}

//Recursive function that checks if a gesture is executed. This is the most important part of the gesture recognition algorithms.
bool Model::isGestureExecuted(std::shared_ptr<Gesture> & gesture, int posInBuffer, int recursiveCounter)
{
	int labelOrderPosition = SVMInterface::NB_OF_LABEL_DIVISIONS - recursiveCounter;
	int labelToCheck = gesture->getLabelOrder().at(labelOrderPosition);

	for (int i = posInBuffer; i >= 0; i--)
	{
		if (labelsBuffer.at(i) == labelToCheck)
		{
			if (labelOrderPosition <= 1)
			{
				return true;
			}
			return isGestureExecuted(gesture, i - 1, recursiveCounter + 1);
		}
	}

	return false;
}

//Checks for all gesture classes if they are recognized. Executes linked actions to the recognized gesture class.
void Model::predictAndExecute(int label)
{
	int executedLabel = -1;
	for (const auto & keyValue : activeProject->getProjectMap())
	{
		for (std::shared_ptr<Gesture> gesture : keyValue.second.first->getGestures())
		{
			if (isGestureExecuted(gesture, labelsBuffer.size() - 1, 1))
			{
				executedLabel = keyValue.first;
			}
		}
	}

	if (executedLabel != -1)
	{
		previousPredictedLabel = predictedLabel;
		predictedLabel = executedLabel;

		Console::print("Predicted label: ");
		Console::printsl(predictedLabel);

		if (activeProject->getGestureClass(predictedLabel)->getGestures().front()->isPosture()) {
			//Remove the last three labels for postures, necessary when a mixture of gestures and postures is used.
			labelsBuffer.pop_back();
			labelsBuffer.pop_back();
			labelsBuffer.pop_back();
		}
		else {
			//Clear the entire buffer if a gesture is recognized.
			labelsBuffer.clear();
		}
		
		if (previousPredictedLabel < 0)
		{
			activeProject->activate(predictedLabel);
		}
		else if (previousPredictedLabel != predictedLabel)
		{
			activeProject->deactivate(previousPredictedLabel);
			activeProject->activate(predictedLabel);
		}
		else if (previousPredictedLabel == predictedLabel)
		{
			activeProject->activate(predictedLabel);
		}
	}
}

std::shared_ptr<GestureClass> Model::getGestureClassByID(const int & ID)
{
	for (int i = 0; i < gestureClasses.size(); ++i)
	{
		if (gestureClasses[i] != nullptr)
		{
			if (gestureClasses[i]->getGestureClassID() == ID)
				return gestureClasses[i];
		}
	}
	return nullptr;
}

void Model::displayFrames()
{
	if (view->checkResource())
	{
		view->drawFrames(relFrames, absFrames);
	}
	relFrames.clear();
	absFrames.clear();
}

//===========================================================================================================================
//Forms part of the main loop of the program. This loop can NOT be blocking, otherwise the UI will not refresh and the sensor
//will not collect new data!
//===========================================================================================================================
void Model::processBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	//Go through all the bodies that are being seen now. If a body is tracked, its frame is made and added to the frames vector
	for (int i = 0; i < nBodyCount; ++i)
	{
		pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (currentActiveBody == -1 && SUCCEEDED(hr) && bTracked)	//No body is tracked at the moment
			{
				currentActiveBody = i;
			}

			if (SUCCEEDED(hr) && bTracked && i == currentActiveBody)
			{
				Frame relFrame(pBody);					//Create a frame of every tracked body
				Frame absFrame(pBody, false);

				relFrames.push_back(relFrame);
				absFrames.push_back(absFrame);
				
				if (recording)
				{
					recordGesture(relFrame);
					break;
				}

				framesBuffer.push_back(relFrame);

				if (refresh && !predict)				//The measure button was pressed last
				{
					updateCountDown();
				}

				if (predict)
				{
					if (!trained)						//If the model is not yet trained, train it
					{
						train();
						trained = true;
					}
					int label = SVMInterface::test(activeProject->getSVMModel(), relFrame);
					addToLabelsBuffer(label);
					predictAndExecute(label);
					view->setPredictedLabel(label);
				}
			}
			else if (i == currentActiveBody)			//If the current tracked body is lost for this frame
			{
				bodyLostCounter++;						//Increment the lostBodyCounter
				if (bodyLostCounter > BODY_LOST_LIMIT)	//If the limit is reached, reset the currentActiveBody
				{
					currentActiveBody = -1;
					bodyLostCounter = 0;
				}
			}
		}
	}

	if (updateUI)
	{
		view->updateHitboxes();
		updateUI = false;
	}
	
	displayFrames();
}

//Used for recording gestures with the given frame being the newest frame retrieved from the sensor.
void Model::recordGesture(Frame & frame)
{
	if (!initialized)
	{
		frameNeutral.setFrame(frame);
		initialized = true;
	}
	framesBuffer.push_back(frame);

	if (!startedMoving)
	{
		if (! frame.equals(frameNeutral))
		{
			startedMoving = true;
			if (framesBuffer.size() > 20)
			{
				std::vector<Frame> latestFrames = getRelevantFramesFromBuffer(10);
				framesBuffer.clear();
				for (Frame frame : latestFrames)
				{
					framesBuffer.push_back(frame);
				}
			}
		}
		else if (framesBuffer.size() > NOT_MOVING_FRAME_DELAY)
		{
			addRecordedGesture();
		}
		else
		{
			return;
		}
	}

	if (framesBuffer.size() > NOT_MOVING_FRAME_DELAY &&
		framesBuffer.back().equals(framesBuffer.at(framesBuffer.size() - NOT_MOVING_FRAME_DELAY)) &&
		framesBuffer.back().equals(framesBuffer.at(framesBuffer.size() - NOT_MOVING_FRAME_DELAY/2)))
	{
		addRecordedGesture();
	}
}

bool Model::getCountDownInit()
{
	return countDownInitiated;
}

int Model::getCountDown()
{
	return countDown;
}

void Model::setUpdatUI(bool update)
{
	updateUI = update;
}

bool Model::getUpdateUI()
{
	return updateUI;
}

//This function is called when recording of a gesture is done.
void Model::addRecordedGesture()
{
	std::shared_ptr<Gesture> gesture = std::make_shared<Gesture>( getRelevantFramesFromBuffer(NOT_MOVING_FRAME_DELAY-20) );
	addGesture(activeGestureClassLabel, gesture);
	recording = false;
	initialized = false;
	startedMoving = false;
	framesBuffer.clear();
}
