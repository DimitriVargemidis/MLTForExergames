#pragma once

#include "Gesture.h"
#include "libsvm-3.21\svm.h"

class SVMInterface {
public:
	void train(Gesture * gestures);
private:
	void train(const int problemSize, const int dimensions, double dataset[], double labels[]);
	double test(const int dimensions, double testData[]);
};
