#ifndef GESTURECLASS_H
#define GESTURECLASS_H

#include <vector>
#include <memory>


class Gesture;

class GestureClass {
private:
	std::vector<std::shared_ptr<Gesture>> gestures;
	int gestureClassID = 0;
	std::string name = "GestureClass";

public:
	GestureClass();
	GestureClass(const std::shared_ptr<Gesture> gesture);

	void addGesture(const std::shared_ptr<Gesture> gesture);
	std::vector<std::shared_ptr<Gesture>> & getGestures();

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
