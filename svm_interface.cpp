#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include "svm_interface.h"
#include "libsvm-3.21\svm.h"
#define Malloc(type,n) (type *)malloc((n)*sizeof(type))

struct svm_parameter param;     // set by parse_command_line
struct svm_problem prob;        // set by read_problem
struct svm_model *model;
struct svm_node *x_space;

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

	/*
	for (int i = 0; i < prob.l; i++) {
		double subset[dimensions];
		for (int j = 0; j < dimensions; j++) {
			subset[j] = dataset[i*dimensions+j];
		}
		double maximum = findMaximum();
	}*/


	svm_node** x = new svm_node*[prob.l];

	for (int row = 0; row < prob.l; row++) {
		svm_node* x_space = new svm_node[dimensions + 1];
		for (int col = 0; col < dimensions; col++) {
			x_space[col].index = col;
			x_space[col].value = dataset[row*dimensions + col];
		}
		x_space[dimensions].index = -1;  //Each row of properties should be terminated with a -1 according to the readme
		x_space[dimensions].value = 0;	 //Value not important
		x[row] = x_space;
	}

	prob.x = x;

	//Train model
	svm_model *model = svm_train(&prob, &param);
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
	delete[] prob.y;
	delete[] prob.x;
	delete[] x_space;
	delete[] testnode;

	return retval;
}