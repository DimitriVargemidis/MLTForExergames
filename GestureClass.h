#ifndef GESTURECLASS_H
#define GESTURECLASS_H

//#include "Gesture.h"
#include <vector>

class Gesture;

class GestureClass {
private:
	std::vector<Gesture> gestures;

public:
	GestureClass();
	GestureClass(const Gesture gesture);

	void addGesture(const Gesture gesture);
	const std::vector<Gesture> & getGestures() const;
};

#endif //GESTURECLASS_H
