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
#include "Model.h"


Model::Model() : project(Project())
{
	gestureClasses.resize(30);
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
	project = projectToSet;
}

Project Model::getProject() {
	return project;
}

void Model::train() {
	project.setSVMModel(*SVMInterface::train(getProject().getProjectGestures()));
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

void Model::ProcessBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	//OutputDebugStringW(L"test string");
	std::vector<Frame> frames;	//the frames that are going to be drawn

	// go through all the bodies that are being seen now if a body is tracked than it's frame is made and added to the frames vector
	for (int i = 0; i < nBodyCount; ++i)
	{
		IBody* pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				Gesture currentGesture;
				Frame frame(pBody);				//create a frame of every tracked body
				frames.push_back(Frame(pBody));

				currentGesture.addFrame(frame);

				//ONLY FOR TESTING -- DELETE AFTERWARDS
				/*
				if (counter % 30 == 0)
				{
					oldFrame = newFrame;
					newFrame = std::make_shared<Frame>(frame);
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
					WORD	keycode = 0;

					//look out no protection against going out of bounds yet!
					gestureClasses[ActiveGestureClassLabel].addGesture(currentGesture);
					//make a new ProjectGesture and add it to the project (in project elimination of projectgestures with same labels is done)
					



					//temporary hardcoded
					//keycodes see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
					ProjectGesture projectGest(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x41,true);

					switch (ActiveGestureClassLabel)
					{
					case 1:
						//do nothing
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0));
						break;
					case 2:
						//space
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x20));
						break;
					case 3:
						//left arrow
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x25,true ));
						break;
					case 4:
						//up arrow
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x26));
						break;
					case 5:
						//right arrow
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x27,true));
						break;
					case 6:
						//down arrow
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x28,true));
						break;
					case 7:
						//button A
						//ProjectGesture projectGest(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x41);
						//left arrow
						projectGest.addAction(0x25, true);
						project.addProjectGesture(projectGest);

						break;
					case 8:
						//button A
						//ProjectGesture projectGest(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x41);
						//right arrow
						projectGest.addAction(0x27, true);
						project.addProjectGesture(projectGest);
					case 9:
						//button A
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x41, true));
						break;
					case 10:
						//button Z
						project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel], ActiveGestureClassLabel, 0x5A, true));
						break;
					}

					//project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel],ActiveGestureClassLabel, keycode));

					refresh = false;
				}

				//adding the last frame of the last gesture of each gestureClass in the vector to the frames vector that is drawn
				for (int j = 0; j < gestureClasses.size(); ++j)
				{
					if (gestureClasses[j].getGestures().size() != 0)
					{
						frames.push_back(gestureClasses[j].getGestures().back().getFrames().back());
					}
				}

				if (predict)
				{
					//if the model is not yet trained, train it
					if (!trained)
					{
						project.setSVMModel(*(SVMInterface::train(project.getProjectGestures())));
						trained = true;
					}



					double predictedLabel = SVMInterface::test(project.getSVMModel(), currentGesture);
					view->setPredictedLabel(static_cast<int>(predictedLabel));

					auto timeNow = Clock::now();
					auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(timeNow - timer).count();

					if (diff > 0)
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
								lastActiveProjectGesture = &(project.getProjectGestureFromLabel(predictedLabel));
							}
							lastActiveProjectGesture->Activate();
							view->changeButtonColor(predictedLabel);
							//Sleep(50);
							//lastActiveProjectGesture->Deactivate();
						}
						else
						{
							lastActiveProjectGesture = &(project.getProjectGestureFromLabel(predictedLabel));
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
		}
	}

	if (view->checkResource())
	{
			view->drawFrames(frames);
	}
	//ADD LATER code to add the 1 Frame from the each gesture class in the current project.
}


