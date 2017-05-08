#include "GestureClass.h"
#include "Gesture.h"
#include "Action.h"
#include "SVMInterface.h"


svm_model * SVMInterface::train(std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap) {
	//Set parameters
	svm_parameter param;
	param.svm_type = C_SVC;
	param.kernel_type = RBF;
	param.degree = 3;
	param.gamma = 0.5;
	param.coef0 = 0;
	param.nu = 0.5;
	param.cache_size = 100;
	param.C = 1;
	param.eps = 1e-3;
	param.p = 0.1;
	param.shrinking = 1;
	param.probability = 0;
	param.nr_weight = 0;
	param.weight_label = NULL;
	param.weight = NULL;

	//Set problem
	svm_problem prob;

	//Get and set the problem size (= number of gestures that were input)
	int problemSize{0};
	for (const auto & keyValue : projectMap)
	{
		problemSize = problemSize + keyValue.second.first->getGestures().size();
	}

	prob.l = problemSize;

	//Set labels and nodes
	double * labels = new double[problemSize];
	svm_node ** x = new svm_node*[problemSize];
	int rowCount{0};

	for (const auto & keyValue : projectMap) {

		const double thresholdFraction = 1.0 / 3.0;
		const double decimalsConstant = 10.0;
		const double baseOffset = 1.0 / decimalsConstant;
		double fraction = 0;
		int counter = 1;
		double labelOffset = 0;

		for (Gesture & g : keyValue.second.first->getGestures()) {
			//fraction = counter / g.getFrames().size();
			labelOffset = ((int)(fraction / thresholdFraction)) / decimalsConstant;
			labels[rowCount] = keyValue.first + labelOffset + baseOffset;
			counter++;
			
			x[rowCount] = g.toArray();
			rowCount++;
		}
	}
	prob.y = labels;
	prob.x = x;

	//Train model
	svm_model * model = svm_train(& prob, & param);
	svm_destroy_param(& param);
	
	return model;
}

double SVMInterface::test(svm_model & model, Gesture & gesture) {
	svm_node * testnode = gesture.toArray();
	double resultLabel = svm_predict(& model, testnode);
	
	delete[] testnode;

	return resultLabel;
}
