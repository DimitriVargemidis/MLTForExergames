#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include "libsvm-3.21\svm.h"

class ProjectGesture;
class Gesture;

namespace SVMInterface {
	const svm_model * train(const std::vector<ProjectGesture> & projectGestures);
	const double test(const svm_model & model, const Gesture & gesture);
	const svm_node ** scale(const svm_node ** node);
};

#endif //SVMINTERFACE_H