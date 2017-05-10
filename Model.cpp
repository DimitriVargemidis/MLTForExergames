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
	if (projects.empty())		//If no project is saved and/or loaded, else do nothing.
	{
		projects.push_back(std::make_shared<Project>());
	}
	activeProject = projects.back();

	bodyLostLimit = 30;			//the amount of frames that the body is able to be lost
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
	Filewriter::save(activeProject);
}

double Model::test(Gesture & gesture)
{
	return SVMInterface::test(activeProject->getSVMModel(), gesture);
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
}

void Model::addToFramesBuffer(Frame frame)
{

}

std::vector<Frame> Model::getRelevantFramesFromBuffer()
{
	std::vector<Frame>::const_iterator first = framesBuffer.begin();
	std::vector<Frame>::const_iterator last = framesBuffer.end() - 30;
	return std::vector<Frame>(first, last);
}

void Model::addToLabelsBuffer(double label)
{
	if (label < 0)
	{
		return;
	}

	labelsBuffer.push_back(label);

	if (labelsBuffer.size() > maxBufferSize)
	{
		previousPredictedLabel = predictedLabel;
		predictedLabel = getMostFrequentLabel();

		if (predictedLabel != previousPredictedLabel)
		{
			activeProject->deactivate(predictedLabel);
		}
		activeProject->activate(predictedLabel);
		
		labelsBuffer.clear();
	}
}

double Model::getMostFrequentLabel()
{
	std::sort(labelsBuffer.begin(), labelsBuffer.end());
	double currentDouble = labelsBuffer[0];
	double mostDouble = labelsBuffer[0];
	int currentCount = 0;
	int mostCount = 0;
	for (auto c : labelsBuffer)
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
		if (gestureClasses[i]->getGestureClassID() == ID)
			return gestureClasses[i];
	}
}

void Model::displayFrames()
{
	relFrames.clear();
	absFrames.clear();

	for (const auto & keyValue : activeProject->getProjectMap())
	{
		relFrames.push_back(keyValue.second.first->getGestures().back().getFrames().back());
	}
}

void Model::processBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	displayFrames();

	// go through all the bodies that are being seen now if a body is tracked than it's frame is made and added to the frames vector
	for (int i = 0; i < nBodyCount; ++i)
	{
		pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (currentActiveBody == -1 && SUCCEEDED(hr) && bTracked)	//no body is tracked at the moment
			{
				currentActiveBody = i;
			}

			if (SUCCEEDED(hr) && bTracked && i == currentActiveBody)
			{
				Gesture currentGesture;
				Frame relFrame(pBody);									//create a frame of every tracked body
				Frame absFrame(pBody, false);

				relFrames.push_back(relFrame);
				absFrames.push_back(absFrame);
				
				if (recording)
				{
					recordGesture(relFrame);
					break;
				}

				framesBuffer.push_back(relFrame);

				if (refresh && !predict)								//the measure button was pressed last
				{
					framesBuffer.clear();
					recording = true;

					//recordGesture(nTime, nBodyCount, ppBodies);
					//currentGesture.addFrame(relFrame);
					//addGesture(activeGestureClassLabel, currentGesture);
					
					//adding the last frame of the last gesture of each gestureClass in the vector to the frames vector that is drawn
					//relFrames.push_back(currentGesture.getFrames().back());
					refresh = false;
				}

				if (predict)
				{
					if (!trained)						//If the model is not yet trained, train it
					{
						train();
						trained = true;
					}
					currentGesture.addFrame(relFrame);
					addToLabelsBuffer(SVMInterface::test(activeProject->getSVMModel(), currentGesture));
					view->setPredictedLabel(static_cast<int>(predictedLabel));
				}
			}
			else if (i == currentActiveBody)			//If the current tracked body is lost for this frame
			{
				bodyLostCounter++;						//Increment the lostBodyCounter
				if (bodyLostCounter > bodyLostLimit)	//if the limit is reached reset the currentActiveBody
				{
					currentActiveBody = -1;
					bodyLostCounter = 0;
				}
			}
		}
	}

	if (view->checkResource())
	{
		view->drawFrames(relFrames, absFrames);
	}
}

void Model::recordGesture(Frame frame)
{
	if (!initialized)
	{
		frameNeutral.setFrame(frame);
		initialized = true;
		Console::print("Initialized");
	}
	
	relFrames.push_back(frame);

	if (!startedMoving)
	{
		if (! frame.equals(frameNeutral))
		{
			Console::print("+");
			startedMoving = true;
		}
		else
		{
			return;
		}
	}
					
	framesBuffer.push_back(frame);

	if (framesBuffer.size() > 100 && framesBuffer.back().equals(framesBuffer.at(framesBuffer.size() - 100)))
	{
		Gesture gesture{getRelevantFramesFromBuffer()};
		addGesture(activeGestureClassLabel, gesture);
		recording = false;
		initialized = false;
		startedMoving = false;
		Console::print("Recording stopped");
	}
}




//ONLY FOR TESTING -- DELETE AFTERWARDS
//THIS CODE WILL BE USED FOR DETECTING MOVEMENT
/*
if (counter % 30 == 0)
{
oldFrame = newFrame;
newFrame = std::make_shared<Frame>(relFrame);
Console::printsl(oldFrame->equals(*newFrame));
counter = 1;
}
else
{
counter++;
}
*/
//ONLY FOR TESTING -- END

