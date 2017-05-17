#ifndef GESTURECLASS_H
#define GESTURECLASS_H

#include <vector>
#include <memory>


class Gesture;

class GestureClass {
private:
	std::vector<Gesture> gestures;
	int gestureClassID = 0;
	std::string name = "GestureClass";

public:
	GestureClass();
	GestureClass(const Gesture & gesture);

	void addGesture(const Gesture & gesture);
	std::vector<Gesture> & getGestures();

	int getGestureClassID();
	void setGestureClassID(int ID);

	std::string getName();
	void setName(std::string & nameToSet);

	void DeleteGestureWithID(int ID);
	std::shared_ptr<Gesture>  getGestureWithID(const int ID);
};

static int appGestureClassID;
static int getAppGestureClassID();

#endif //GESTURECLASS_H
