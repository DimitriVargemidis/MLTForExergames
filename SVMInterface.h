#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include "Project.h"
#include "libsvm-3.21\svm.h"


namespace SVMInterface {
	const svm_model & train(const std::vector<ProjectGesture> & projectGestures);
	const double test(const svm_model & model, const Gesture & gesture);
};

#endif //SVMINTERFACE_H