#pragma once

#include "libsvm-3.21\svm.h"

void train(const int problemSize, const int dimensions, double dataset[], double labels[]);
double test(const int dimensions, double testData[]);
