#ifndef GESTURECLASS_H
#define GESTURECLASS_H

#include <vector>
#include <memory>


class Gesture;

class GestureClass {
private:
	//A vector collecting all gestures belonging to this gesture class.
	std::vector<std::shared_ptr<Gesture>> gestures;
	//The ID of this gesture class on project level. This can be the same ID as one of the gesture
	//classes of different projects.
	int gestureClassID = 0;
	//The name of this gesture class can be customized and displayed to the user for
	//reference. This is not yet used in the application.
	std::string name = "GestureClass";

public:
	GestureClass();
	GestureClass(const std::shared_ptr<Gesture> gesture);

	//Add a gesture to this gesture class.
	void addGesture(const std::shared_ptr<Gesture> gesture);
	std::vector<std::shared_ptr<Gesture>> & getGestures();

	int getGestureClassID();
	void setGestureClassID(int ID);

	std::string getName();
	void setName(std::string & nameToSet);

	void DeleteGestureWithID(int ID);
	std::shared_ptr<Gesture>  getGestureWithID(const int ID);
};

//The ID of this gesture class. This ID is unique considering all gesture classes of all projects and
//is used to save data to a file with a non-conflicting name.
static int appGestureClassID;
static int getAppGestureClassID();

#endif //GESTURECLASS_H
