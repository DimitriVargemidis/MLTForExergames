#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include "SVMInterface.h"

const int DIMENSIONS_PER_JOINT = 3;

struct svm_parameter param;     // set by parse_command_line
struct svm_problem prob;        // set by read_problem
struct svm_model *model;
struct svm_node *x_space;

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

const svm_model & SVMInterface::train(const std::vector<ProjectGesture> & projectGestures) {
	// TODO: insert return statement here
	svm_destroy_param(&param);
}

const double SVMInterface::test(const svm_model & model, const Gesture & gesture) {
	int dimensions = gesture.getNumberOfFrames() * gesture.getNumberOfJointsPerFrame() * DIMENSIONS_PER_JOINT;
	svm_node * testnode = new svm_node[dimensions + 1];

	int indexCount{0};
	for (int i = 0; i < gesture.getNumberOfFrames(); i++) {
		for (int j = 0; j < gesture.getNumberOfJointsPerFrame(); j++) {
			testnode[indexCount].index = indexCount;
			testnode[indexCount].value = gesture.getFrames().at(i).getJoints().at(j).Position.X;
			indexCount = indexCount + 1;
			testnode[indexCount].index = indexCount;
			testnode[indexCount].value = gesture.getFrames().at(i).getJoints().at(j).Position.Y;
			indexCount = indexCount + 1;
			testnode[indexCount].index = indexCount;
			testnode[indexCount].value = gesture.getFrames().at(i).getJoints().at(j).Position.Z;
			indexCount = indexCount + 1;
		}
	}

	testnode[indexCount].index = -1;
	testnode[indexCount].value = 0;

	double resultLabel = svm_predict(& model, testnode);
	delete[] testnode;
	
	return resultLabel;
}
