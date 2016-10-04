#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include "SVMInterface.h"

struct svm_parameter param;     // set by parse_command_line
struct svm_problem prob;        // set by read_problem
struct svm_model *model;
struct svm_node *x_space;

//Train the SVM to learn gestures.
//	@param	A vector containing all gesture objects that are to be used
//			for computing a model for the SVM.
//	@result The model instance of this object is initialized.
//	@result	The parameter instance of this object is initialized.
//	@result	The problem instance of this object is initialized.
void SVMInterface::train(const std::vector<Gesture> &gestures) {
	setAllParameters();

	//setProblem(size(gestures), getLabels(gestures));
	model = svm_train(&prob, &param);
}

//Return the pointer that points to an array cointaining the labels of all
//given gestures in the same order.
//	@param	A vector containing all gestures.
//	@result	The pointer of an array containing all labels.
double * SVMInterface::getLabels(const std::vector<Gesture> &gestures) {
	double * labels;
	labels = new double[size(gestures)];
	Gesture gesture;
	for (int index = 0; index < size(gestures); index++) {
		gesture = gestures[index];
		labels[index] = gesture.getLabel();
	}
	return labels;
}

//Set all parameters, used when computing a model.
void SVMInterface::setAllParameters() {
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
}

//Set the problem object of this object with a given problem size and a given
//vector containing all labels.
//	@param problemSize: The number of gestures used for computing the model
//	@param labels: A vector containing the label of each gesture that was given to
//					compute a model in the same order the gestures are given.
void SVMInterface::setProblem(const int problemSize, double * labels, svm_node ** dataset) {
	prob.l = problemSize;
	prob.y = labels;
	prob.x = dataset;
}

void train(const int problemSize, const int dimensions, double dataset[], double labels[]) {
	/*
	==================
	Set all parameters
	==================
	*/
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


	/*
	==================
	Problem definition
	==================
	*/
	//Set the number of training data
	prob.l = problemSize;

	//Set the array containing the labels of all training data
	prob.y = labels;

	svm_node** x = new svm_node*[prob.l];

	for (int row = 0; row < prob.l; row++) {
		svm_node* xRow = new svm_node[dimensions + 1];
		for (int col = 0; col < dimensions; col++) {
			xRow[col].index = col;
			xRow[col].value = dataset[row*dimensions + col];
		}
		xRow[dimensions].index = -1;  //Each row of properties should be terminated with a -1 according to the readme
		xRow[dimensions].value = 0;	 //Value not important
		x[row] = xRow;
	}

	prob.x = x;

	//Train model
	model = svm_train(&prob, &param);
}

double test(const int dimensions, double testData[]) {
	svm_node* testnode = new svm_node[dimensions+1];

	for (int i = 0; i < dimensions; i++) {
		testnode[i].index = i;
		testnode[i].value = testData[i];
	}
	testnode[dimensions].index = -1;
	testnode[dimensions].value = 0;

	double retval = svm_predict(model, testnode);
	
	svm_destroy_param(&param);
	/*delete[] prob.y;
	delete[] prob.x;
	delete[] testnode;
	*/
	return retval;
}