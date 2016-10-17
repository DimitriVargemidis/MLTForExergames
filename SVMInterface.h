#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include "Project.h"
#include "libsvm-3.21\svm.h"
#include <vector>

namespace SVMInterface {
	svm_model train(const Project & project);
	double test(const svm_model * model, const Gesture & gesture);
};

#endif //SVMINTERFACE_H