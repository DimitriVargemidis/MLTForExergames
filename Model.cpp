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
	gestureClasses.resize(10);
}

Model::~Model()
{
}

void Model::setView(std::shared_ptr<UI> v)
{
	Console::print("setView PRINT TEST 3");
	view = v;
}

void Model::setProject(const Project & projectToSet) {
	if (projectToSet.getProjectGestures().size() == 0) {
		throw std::invalid_argument("The given project does not contain any ProjectClasses.");
	}
	project = projectToSet;
}

const Project & Model::getProject() const {
	return project;
}

void Model::train() {
	project.setSVMModel(*SVMInterface::train(getProject().getProjectGestures()));
}

const double Model::test(const Gesture & gesture) {
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

void Model::ProcessBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	//OutputDebugStringW(L"test string");
	std::vector<Frame> frames;	//the frames that are goint to be drawn

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
				Frame frame(pBody, true);				//create a frame of every tracked body
				frames.push_back(Frame(pBody,true));

				currentGesture.addFrame(frame);

				//the measure button was pressed last	
				if (refresh && !predict)
				{
					WORD	keycode = 0;

					//look out no protection against going out of bounds yet!
					gestureClasses[ActiveGestureClassLabel].addGesture(currentGesture);
					//make a new ProjectGesture and add it to the project (in project elimination of projectgestures with same labels is done)
					
					//temporary hardcoded
					//keycodes see https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx

					switch (ActiveGestureClassLabel)
					{
					case 2:
						keycode = 0x20; //space
						break;
					case 3:
						keycode = 0x25; //left arrow
						break;
					case 4:
						keycode = 0x26; //up arrow
						break;
					case 5:
						keycode = 0x27;	//right arrow
						break;
					case 6:
						keycode = 0x28; //down arrow
						break;
					}

					project.addProjectGesture(ProjectGesture(gestureClasses[ActiveGestureClassLabel],ActiveGestureClassLabel, keycode));

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


					int predictedLabel = static_cast<int>(SVMInterface::test(project.getSVMModel(), currentGesture));
					view->setPredictedLabel(predictedLabel);
					WORD keycode = project.getProjectGestureFromLabel(predictedLabel).getKey();

					if (keycode != 0)
					{
						Keypress::pressKey(keycode);
						//OutputDebugStringW(L"test string\n");
						int a = static_cast<int>(keycode);
						wchar_t buffer[256];
						wsprintfW(buffer, L"%d", a);
						OutputDebugStringW(L"keycode is ");
						OutputDebugStringW(buffer);
						OutputDebugStringW(L"\n");

						//	(std::to_string(static_cast<int>(keycode))).c_str());
					}
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


