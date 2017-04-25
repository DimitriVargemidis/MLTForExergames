#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <iostream>
#include <fstream>
#include <windows.h>

#include "ProjectGesture.h"
#include "Gesture.h"
#include "Frame.h"
#include "Console.h"

#include "SVMInterface.h"


svm_model * SVMInterface::train(std::vector<ProjectGesture> & projectGestures) {
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
	for (ProjectGesture & pg : projectGestures) {
		problemSize = problemSize + pg.getGestureClass().getGestures().size();
	}

Console::print("Problem size: ");
Console::printsl(problemSize);


	prob.l = problemSize;

	//Set labels and nodes
	Gesture gesture = projectGestures.at(0).getGestureClass().getGestures().at(0);
	double * labels = new double[problemSize];
	svm_node ** x = new svm_node*[problemSize];
	int rowCount{0};

Console::print("projectGestures size: ");
Console::printsl((int)projectGestures.size());

	for (ProjectGesture & pg : projectGestures) {

Console::print("Number of gestures in this projectGesture: ");
Console::printsl((int)pg.getGestureClass().getGestures().size());

		for (Gesture & g : pg.getGestureClass().getGestures()) {			
			labels[rowCount] = pg.getLabel();
			x[rowCount] = g.toArray();
			rowCount++;
		}
	}
	prob.y = labels;
	prob.x = x;

	//Train model
	svm_model * model = svm_train(& prob, & param);

	svm_destroy_param(& param);

	//std::ofstream outfile("File_SVM_Model");
	std::ofstream outfile;
	outfile.open("File_TEXT_OUTPUT_TEST");
	outfile << "This is some text." << std::endl;
	outfile << "A new line has begun..." << std::endl;
	outfile.close();


	const char filename[25]{"File_SVM_Model"};

	int saveStatus = svm_save_model(filename, model);
	Console::print("Save status: ");
	Console::printsl(saveStatus);

	svm_model * loadedSVMModel = svm_load_model(filename);
	Console::print("SVM Type: ");
	Console::printsl(svm_get_svm_type(loadedSVMModel));

	Console::print("Number of classes: ");
	const int nbOfClasses = svm_get_nr_class(loadedSVMModel);
	Console::printsl(nbOfClasses);
	
	int * loadedLabels = new int[nbOfClasses];
	svm_get_labels(loadedSVMModel, loadedLabels);
	
	for (int i=0; i<nbOfClasses; i++)
	{
		Console::printsl("Label (index ");
		Console::printsl(i);
		Console::printsl("): ");
		Console::printsl(loadedLabels[i]);
		Console::printsl(" ");
	}

	Console::print("Number of sv: ");
	int nbOfSV = svm_get_nr_sv(loadedSVMModel);
	Console::printsl(nbOfSV);

	return model;
}

double SVMInterface::test(svm_model & model, Gesture & gesture) {
	svm_node * testnode = gesture.toArray();
	double resultLabel = svm_predict(& model, testnode);
	
	delete[] testnode;

	return resultLabel;
}

svm_node ** SVMInterface::scale(svm_node ** node) {
	return nullptr;
}
