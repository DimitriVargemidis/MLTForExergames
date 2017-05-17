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
	Console::print("Project saved");
}

double Model::test(Frame & frame)
{
	return SVMInterface::test(activeProject->getSVMModel(), frame);
}

void Model::setActiveLabel(int label)
{
	activeGestureClassLabel = label;
}

int Model::getActiveLabel()
{
	return activeGestureClassLabel;
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

bool Model::getPredict()
{
	return predict;
}

void Model::setTrained(bool train)
{
	trained = train;
}

bool Model::getTrained()
{
	return trained;
}

void Model::setRecording(bool record)
{
	recording = record;
}

bool Model::getRecording()
{
	return recording;
}

void Model::addActionToActive(WORD keycode, bool hold)
{
	activeProject->addAction(activeGestureClassLabel, keycode, hold);
}

void Model::addGesture(double label, Gesture gesture)
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

std::vector<Frame> Model::getRelevantFramesFromBuffer(int offset)
{
	std::vector<Frame>::const_iterator first = framesBuffer.begin();
	std::vector<Frame>::const_iterator last = framesBuffer.end() - offset;
	return std::vector<Frame>(first, last);
}

void Model::addToLabelsBuffer(double label)
{
	if (label < 1)
	{
		return;
	}

	labelsBuffer.push_back(label);
	predictedLabel = label;
	/*
	for (const auto & item : activeProject->getProjectMap())
	{
		if (item.first == (SVMInterface::NB_OF_LABEL_DIVISIONS - 1))
		{
			for (int i = labelsBuffer.size(); i > 0; i--)
			{
				if (labelsBuffer.at(i) == SVMInterface::NB_OF_LABEL_DIVISIONS - 2)
				{
					for (int j = i; j > 0; j--)
					{

					}
				}
			}
		}
	}*/
	
}

double Model::getMostFrequentLabel()
{
	std::sort(labelsBuffer.begin(), labelsBuffer.end());
	double currentDouble = labelsBuffer[0];
	double mostDouble = labelsBuffer[0];
	int currentCount = 0;
	int mostCount = 0;
	for (const auto & c : labelsBuffer)
	{
		if (c == currentDouble)
		{
			currentCount++;
		}
		else
		{
			if (currentCount > mostCount)
			{
				mostDouble = currentDouble;
				mostCount = currentCount;
			}
			currentDouble = c;
			currentCount = 1;
		}
	}
	return mostDouble;
}

std::shared_ptr<GestureClass>	 Model::getGestureClassByID(const int & ID)
{
	for (int i = 0; i < gestureClasses.size(); i++)
	{
		if (gestureClasses[i] != nullptr)
		{
			if (gestureClasses[i]->getGestureClassID() == ID)
				return gestureClasses[i];
		}
		
	}

	//give the last gestureClass back
	//return gestureClasses[gestureClasses.size()-1];
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
					if (!countDownInitiated)
					{
						countDownInitiated = true;
						countDownRef = Clock::now();
						printf("Get ready to record\n");
						countDown = 4;
						updateUI = true;
					}
					else
					{
						auto t2 = Clock::now();
						long time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - countDownRef).count();

						if (time >= 1000 && countDown > 3)
						{
							printf("3 after %ld \n", time);
							--countDown;
							updateUI = true;
						}
						else if (time >= 2000 && countDown > 2)
						{
							printf("2 after %ld \n", time);
							--countDown;
							updateUI = true;
						}	
						else if (time >= 3000 && countDown > 1)
						{
							printf("1 after %ld \n", time);
							--countDown;
							updateUI = true;
						}	
						else if ((time >= 4000) )
						{
							
							printf("GO after %ld \n", time);
							countDown = 0;
							
							framesBuffer.clear();
							recording = true;
							refresh = false;
							updateUI = true;
							countDownInitiated = false;
						}

					}
				}

				if (predict)
				{
					if (!trained)						//If the model is not yet trained, train it
					{
						train();
						trained = true;
					}
					addToLabelsBuffer(SVMInterface::test(activeProject->getSVMModel(), relFrame));
					view->setPredictedLabel(predictedLabel);
					labelsBuffer.clear(); //##################################################################
				}
			}
			else if (i == currentActiveBody)			//If the current tracked body is lost for this frame
			{
				bodyLostCounter++;						//Increment the lostBodyCounter
				if (bodyLostCounter > bodyLostLimit)	//If the limit is reached, reset the currentActiveBody
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

void Model::recordGesture(Frame frame)
{
	if (!initialized)
	{
		frameNeutral.setFrame(frame);
		initialized = true;
		Console::print("Initialized recording");
	}
	
	relFrames.push_back(frame);

	if (!startedMoving)
	{
		if (! frame.equals(frameNeutral))
		{
			Console::print("Motion detected -- start recording frames");
			startedMoving = true;
		}
		else
		{
			return;
		}
	}
					
	framesBuffer.push_back(frame);

	if (framesBuffer.size() > notMovingFrameDelay &&
		framesBuffer.back().equals(framesBuffer.at(framesBuffer.size() - notMovingFrameDelay)))
	{
		Gesture gesture{getRelevantFramesFromBuffer(notMovingFrameDelay)};
		addGesture(activeGestureClassLabel, gesture);
		recording = false;
		initialized = false;
		startedMoving = false;
		Console::print("Recording stopped");
		updateUI = true;
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

