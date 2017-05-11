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

void Model::train()
{
	activeProject->setSVMModel(*(SVMInterface::train(activeProject->getProjectMap())));
	Console::print("SVM trained");
	Filewriter::save(activeProject);
	Console::printsl(" -- Project saved");
	activeProject->setLongestGestureSize();
}

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

void Model::setPredict(bool refresh)
{
	this->predict = refresh;
}

void Model::setTrained(bool train)
{
	trained = train;
}

void Model::addActionToActive(WORD keycode, bool hold)
{
	activeProject->addAction(activeGestureClassLabel, keycode, hold);
}

void Model::addGesture(int label, Gesture gesture)
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
}

std::vector<Frame> Model::getRelevantFramesFromBuffer(int offset)
{
	return std::vector<Frame>(framesBuffer.begin(), framesBuffer.end() - offset);
}

void Model::addToLabelsBuffer(int label)
{
	labelsBuffer.push_back(label);

	if (labelsBuffer.size() > activeProject->getLongestGestureSize())
	{
		resizeLabelsBuffer();
	}
}

bool Model::isGestureExecuted(int checkLabel, int posInBuffer, int recursiveCounter, int badCounter)
{
	//The base label exists and is linked to a posture, so the posture has been executed.
	if (activeProject->containsLabel(checkLabel) && activeProject->getGestureClass(checkLabel)->getGestures().begin()->isPosture())
	{
		return true;
	}

	//Done enough recursive checks to confirm the gesture has been executed.
	if (recursiveCounter >= SVMInterface::NB_OF_LABEL_DIVISIONS)
	{
		return true;
	}

	int nextLabelToCheck = checkLabel - 1;
	for (int i = posInBuffer; i >= 0; i--)
	{
		if (labelsBuffer.at(i) == nextLabelToCheck)
		{
			return isGestureExecuted(nextLabelToCheck, i, recursiveCounter + 1, 0);
		}
	}

	//A label that is one less than the last label found cannot be found in the buffer, but the
	//gesture may still have been executed, so keep checking for the next one.
	if (badCounter > 0)
	{
		return false;
	}

	return isGestureExecuted(nextLabelToCheck, posInBuffer, recursiveCounter + 1, badCounter + 1);
}

void Model::resizeLabelsBuffer()
{
	labelsBuffer = std::vector<int>(labelsBuffer.end() - activeProject->getLongestGestureSize(), labelsBuffer.end());
}

void Model::predictAndExecute(int label)
{
	//Find out what base label the given label belongs to.
	int baseLabel = -1;
	if (activeProject->containsLabel(label))
	{
		baseLabel = label;
	}
	else
	{
		for (int i = SVMInterface::NB_OF_LABEL_DIVISIONS; i > 0; i--)
		{
			if (activeProject->containsLabel(label - i))
			{
				baseLabel = label - i;
				break;
			}
		}
	}

	if (isGestureExecuted(label, labelsBuffer.size() - 1, 1, 0))
	{
		previousPredictedLabel = predictedLabel;
		predictedLabel = baseLabel;
	
		Console::print("Predicted label: ");
		Console::printsl(predictedLabel);
		labelsBuffer.clear();

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
	for (int i = 0; i < gestureClasses.size(); i++)
	{
		if (gestureClasses[i]->getGestureClassID() == ID)
			return gestureClasses[i];
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

	for (const auto & keyValue : activeProject->getProjectMap())
	{
		relFrames.push_back(keyValue.second.first->getGestures().back().getFrames().back());
	}
}

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
					framesBuffer.clear();
					recording = true;
					refresh = false;
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
	displayFrames();
}

void Model::recordGesture(Frame & frame)
{
	if (!initialized)
	{
		frameNeutral.setFrame(frame);
		initialized = true;
		Console::print("Initialized recording");
	}
	
	relFrames.push_back(frame);
	framesBuffer.push_back(frame);

	if (!startedMoving)
	{
		if (! frame.equals(frameNeutral))
		{
			Console::print("Motion detected -- start recording frames");
			startedMoving = true;
			framesBuffer.clear();
		}
		else if (framesBuffer.size() > NOT_MOVING_FRAME_DELAY)
		{
			Console::print("Standing still -- posture recorded");
			addRecordedGesture();
		}
		else
		{
			return;
		}
	}

	if (framesBuffer.size() > NOT_MOVING_FRAME_DELAY &&
		framesBuffer.back().equals(framesBuffer.at(framesBuffer.size() - NOT_MOVING_FRAME_DELAY)))
	{
		addRecordedGesture();
	}
}

void Model::addRecordedGesture()
{
	Gesture gesture{ getRelevantFramesFromBuffer(NOT_MOVING_FRAME_DELAY-10) };
	addGesture(activeGestureClassLabel, gesture);
	recording = false;
	initialized = false;
	startedMoving = false;
	framesBuffer.clear();
	Console::print("Recording stopped");
}
