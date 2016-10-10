#pragma once

#include <vector>
#include "Gesture.h"
#include "GestureClass.h"
#include "libsvm-3.21\svm.h"

void train(const std::vector<GestureClass> & gestureClass);
double test(const Gesture & gesture);
