#ifndef GESTURECLASS_H
#define GESTURECLASS_H

#include <vector>

class Gesture;

class GestureClass {
private:
	std::vector<Gesture> gestures;
	int gestureClassID = 0;

public:
	GestureClass();
	GestureClass(const Gesture gesture);

	void addGesture(const Gesture gesture);
	std::vector<Gesture> getGestures();

	int getGestureClassID();
	void setGestureClassID(int ID);
};

static int appGestureClassID = 0;
static int getAppGestureClassID();

#endif //GESTURECLASS_H
