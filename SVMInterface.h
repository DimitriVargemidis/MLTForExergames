#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include <map>
#include "libsvm-3.21\svm.h"

#include "GestureClass.h"
#include "Frame.h"


namespace SVMInterface {

	const int		NB_OF_LABEL_DIVISIONS{ 4 };
	const double	THRESHOLD_FRACTION{ 1.0 / NB_OF_LABEL_DIVISIONS };

	svm_model * train(std::map<int, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap);
	int test(svm_model & model, Frame & frame);
};

#endif //SVMINTERFACE_H