#pragma once

#include <vector>
#include "Gesture.h"
#include "libsvm-3.21\svm.h"

class SVMInterface {
public:
	void train(const std::vector<Gesture> &gestures);
private:
	void setAllParameters();
	void setProblem(const int problemSize, double * labels, svm_node ** dataset);
	double * getLabels(const std::vector<Gesture> &gestures);
	svm_node ** rescaleData(const std::vector<Gesture> &gestures);

	void train(const int problemSize, const int dimensions, double dataset[], double labels[]);
	double test(const int dimensions, double testData[]);
};
