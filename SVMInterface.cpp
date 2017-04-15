#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <windows.h>

#include "ProjectGesture.h"
#include "Gesture.h"
#include "Frame.h"

#include "SVMInterface.h"

const int DIMENSIONS_PER_JOINT = 3;

/*
void train(const int problemSize, const int dimensions, double dataset[], double labels[]) {
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
	return retval;
}*/

const svm_model * SVMInterface::train(const std::vector<ProjectGesture> & projectGestures) {
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
	for (const ProjectGesture & pg : projectGestures) {
		problemSize = problemSize + pg.getGestureClass().getGestures().size();
	}
	prob.l = problemSize;

	//Set labels and nodes
	Gesture gesture = projectGestures.at(0).getGestureClass().getGestures().at(0);
	double * labels = new double[problemSize];
	svm_node ** x = new svm_node*[problemSize];
	int rowCount{0};
	for (const ProjectGesture & pg : projectGestures) {
		for (const Gesture & g : pg.getGestureClass().getGestures()) {
			labels[rowCount] = pg.getLabel();
			x[rowCount] = g.toArray();
			rowCount = rowCount + 1;
		}
	}
	prob.y = labels;
	prob.x = x;

	//Train model
	svm_model * model = svm_train(& prob, & param);

	svm_destroy_param(& param);
	return model;
}

const double SVMInterface::test(const svm_model & model, const Gesture & gesture) {
	svm_node * testnode = gesture.toArray();
	double resultLabel = svm_predict(& model, testnode);
	
	delete[] testnode;

	return resultLabel;
}

const svm_node ** SVMInterface::scale(const svm_node ** node) {
	return nullptr;
}
