#include "GestureClass.h"
#include "Gesture.h"
#include "Frame.h"
#include "Action.h"
#include "SVMInterface.h"


svm_model * SVMInterface::train(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap) {
	//Set parameters
	svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.1;//0.5 or 0.01
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 10;//1
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	//Set problem
	svm_problem prob;

	//Get and set the problem size (= number of frames that were input)
	int problemSize{0};
	for (const auto & keyValue : projectMap)
	{
		for(const Gesture & gesture : keyValue.second.first->getGestures())
		{
			problemSize = problemSize + gesture.getFrames().size();
		}
	}

	prob.l = problemSize;

	//Set labels and nodes
	double * labels = new double[problemSize];
	svm_node ** x = new svm_node*[problemSize];
	int rowCount{0};

	for (const auto & keyValue : projectMap) {
		for (const Gesture & g : keyValue.second.first->getGestures()) {
			double fraction = 0;
			int counter = 0;
			int labelOffset = 0;
			for (const Frame & f : g.getFrames())
			{
				if (!g.isPosture())
				{
					fraction = counter / ((double)g.getFrames().size());
					labelOffset = (int)(fraction / THRESHOLD_FRACTION);
					counter++;
				}
				labels[rowCount] = keyValue.first + labelOffset;
				x[rowCount] = f.toArray();
				rowCount++;
			}
		}
	}
	prob.y = labels;
	prob.x = x;

	//Train model
	svm_model * model = svm_train(& prob, & param);
	svm_destroy_param(& param);
	
	return model;
}

int SVMInterface::test(svm_model & model, Frame & frame) {
	svm_node * testnode = frame.toArray();

	int resultLabel = svm_predict(& model, testnode);
	
	delete[] testnode;

	return resultLabel;
}
