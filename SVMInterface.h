#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include <map>
#include "libsvm-3.21\svm.h"

#include "GestureClass.h"


namespace SVMInterface {
	const int DIMENSIONS_PER_JOINT = 3;

	svm_model * train(std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap);
	double test(svm_model & model, Gesture & gesture);
};

#endif //SVMINTERFACE_H