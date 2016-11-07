#include <stdexcept>
#include <vector>
#include "Main.h"
#include "Frame.h"

#include "Model.h"


Model::Model()
{
}

Model::~Model()
{
}

Model::Model(Main * main)
{
	this->main = main;
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
	project.setSVMModel(SVMInterface::train(getProject().getProjectGestures()));
}

const double Model::test(const Gesture & gesture) {
	return SVMInterface::test(getProject().getSVMModel(), gesture);
}

void Model::setRefresh(boolean refresh)
{
	this->refresh = refresh;
}

boolean Model::getRefresh()
{
	return refresh;
}

void Model::setPredict(boolean refresh)
{
	this->predict = refresh;
}

boolean Model::getPredict()
{
	return predict;
}

void Model::ProcessBody(INT64 nTime, int nBodyCount, IBody ** ppBodies)
{
	std::vector<Frame> frames;
	//for (int i = 0; i < nBodyCount; ++i)
	for (int i = 0; i < 1; ++i) //test draw only 1 body that is on camera
	{
		IBody* pBody = ppBodies[i];
		if (pBody)
		{
			BOOLEAN bTracked = false;
			HRESULT hr = pBody->get_IsTracked(&bTracked);

			if (SUCCEEDED(hr) && bTracked)
			{
				
				frames.push_back(Frame(pBody));
				main->drawFrames(frames);
			}
		}

		
	}

	//ADD LATER code to add the 1 Frame from the each gesture class in the current project.

}


