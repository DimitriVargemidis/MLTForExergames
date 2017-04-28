#include <stdexcept>
#include <vector>
#include <windows.h>
#include <string>



#include "Main.h"
#include "ProjectGesture.h"
#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"
#include "Keypress.h"
#include "Console.h"
#include "Filewriter.h"
#include "Filereader.h"
#include "Model.h"


Model::Model() : activeProject(Project())
{
	gestureClasses.resize(30);
	bodyLostLimit = 30;			//the amount of frames that the body is able to be lost
}

Model::~Model()
{
}

void Model::setView(std::shared_ptr<UI> v)
{
	view = v;
}

void Model::setProject(Project & projectToSet) {
	if (projectToSet.getProjectGestures().size() == 0) {
		throw std::invalid_argument("The given project does not contain any ProjectClasses.");
	}
	activeProject = projectToSet;
}

Project Model::getProject() {
	return activeProject;
}

void Model::train() {
	activeProject.setSVMModel(*SVMInterface::train(getProject().getProjectGestures()));
}

double Model::test(Gesture & gesture) {
	return SVMInterface::test(getProject().getSVMModel(), gesture);
}

void Model::setActiveLabel(int label)
{
	ActiveGestureClassLabel = label;
}

int Model::getActiveLabel()
{
	return ActiveGestureClassLabel;
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

void Model::addKeyToActive(WORD keycode, bool hold)
{
	activeProject.getProjectGestureFromLabel(ActiveGestureClassLabel).addAction(keycode, hold);

}

void Model::ProcessBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	//OutputDebugStringW(L"test string");
	relFrames.clear();	//the frames that are going to be drawn
	absFrames.clear();

	// go through all the bodies that are being seen now if a body is tracked than it's frame is made and added to the frames vector
	for (int i = 0; i < nBodyCount; ++i)
	{
		pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (currentActiveBody == -1 && SUCCEEDED(hr) && bTracked) //no body is tracked at the moment
			{
				currentActiveBody = i;
				printf("currentActiveBody is %d",i);
			}

			if (SUCCEEDED(hr) && bTracked && i == currentActiveBody)
			{
				Gesture currentGesture;
				Frame relFrame(pBody);				//create a frame of every tracked body
				Frame absFrame(pBody, false);

				relFrames.push_back(relFrame);
				absFrames.push_back(absFrame);

				currentGesture.addFrame(relFrame);

				//ONLY FOR TESTING -- DELETE AFTERWARDS
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

				//the measure button was pressed last	
				if (refresh && !predict)
				{
					//look out no protection against going out of bounds yet!
					gestureClasses[ActiveGestureClassLabel].addGesture(currentGesture);
	
					//the check whether the projectGesture allready exists is done inside project
					activeProject.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel));

					refresh = false;
				}

				//adding the last frame of the last gesture of each gestureClass in the vector to the frames vector that is drawn
				for (int j = 0; j < gestureClasses.size(); ++j)
				{
					if (gestureClasses[j].getGestures().size() != 0)
					{
						relFrames.push_back(gestureClasses[j].getGestures().back().getFrames().back());
					}
				}

				if (predict)
				{
					//if the model is not yet trained, train it
					if (!trained)
					{
						//Filewriter::save(activeProject);
						//std::string filename = "1.project";
						//Filereader::readProjectFromFile(filename, &activeProject, &gestureClasses);
						activeProject.setSVMModel(*(SVMInterface::train(activeProject.getProjectGestures())));
						trained = true;
					}



					double predictedLabel = SVMInterface::test(activeProject.getSVMModel(), currentGesture);
					view->setPredictedLabel(static_cast<int>(predictedLabel));

					auto timeNow = Clock::now();
					auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timer).count();

					if (diff > buttondelay)
					{
						timer = Clock::now();
						
						//code to check the timing = should be OK
						/*
						wchar_t buffer[256];
						wsprintfW(buffer, L"%ld", diff);
						OutputDebugStringW(L"time between this action and previous action ");
						OutputDebugStringW(buffer);
						OutputDebugStringW(L" milliseconds \n");
						*/


						if (lastActiveProjectGesture != nullptr)
						{

							if (lastActiveProjectGesture->getLabel() != predictedLabel)
							{
								//WORD keycode = project.getProjectGestureFromLabel(predictedLabel).getKey();
								lastActiveProjectGesture->Deactivate();
								lastActiveProjectGesture = &(activeProject.getProjectGestureFromLabel(predictedLabel));
							}
							lastActiveProjectGesture->Activate();
							view->changeButtonColor(predictedLabel);
						
						}
						else
						{
							lastActiveProjectGesture = &(activeProject.getProjectGestureFromLabel(predictedLabel));
							lastActiveProjectGesture->Activate();
						}
					}
					/*
					
					if (keycode != 0)
					{
						Keypress::pressKey(keycode);
						//int a = static_cast<int>(keycode);
						//wchar_t buffer[256];
						//wsprintfW(buffer, L"%d", a);
						//Console::print("Keycode is ");
						//Console::printsl(buffer);
					}
					*/
				}
			}
			else if (i == currentActiveBody)			//if the current tracked body is lost for this frame
			{
				bodyLostCounter++;						//increment he lostBodyCounter
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
	//ADD LATER code to add the 1 Frame from the each gesture class in the current project.
}


