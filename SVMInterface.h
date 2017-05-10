#ifndef SVMINTERFACE_H
#define SVMINTERFACE_H

#include <vector>
#include <map>
#include "libsvm-3.21\svm.h"

#include "GestureClass.h"
#include "Frame.h"


namespace SVMInterface {

	const int		nbOfLabelDivisions{ 8 };
	const double	thresholdFraction{ 1.0 / nbOfLabelDivisions };
	const double	decimalsConstant{ 10.0 };

	svm_model * train(std::map<double, std::pair<std::shared_ptr<GestureClass>, std::vector<Action>>> & projectMap);
	double test(svm_model & model, Frame & frame);
};

#endif //SVMINTERFACE_H