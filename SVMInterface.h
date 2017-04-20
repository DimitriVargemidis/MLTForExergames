#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include "libsvm-3.21\svm.h"

class ProjectGesture;
class Gesture;

namespace SVMInterface {
	svm_model * train(std::vector<ProjectGesture> & projectGestures);
	double test(svm_model & model, Gesture & gesture);
	svm_node ** scale(svm_node ** node);
};

#endif //SVMINTERFACE_H