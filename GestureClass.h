#ifndef GESTURECLASS_H
#define GESTURECLASS_H

#include "Gesture.h"
#include <vector>

class GestureClass {
private:
	std::vector<Gesture> gestures;

public:
	void addGesture(const Gesture & gesture);
	const std::vector<Gesture> & getGestures() const;
};

#endif //GESTURECLASS_H
