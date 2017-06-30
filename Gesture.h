#ifndef	GESTURE_H
#define	GESTURE_H

#include <vector>


class Frame;

class Gesture {
private:
	//A vector containing all frames that belong to this gesture(/posture).
	std::vector<Frame> frames;
	//A vector that contains all indetification labels in the correct order. If
	//all of these labels are predicted in the correct order, this gesture is
	//considered to be 'recognized'.
	std::vector<int> labelOrder;
	//ID of this gesture. This is unique within the project, but can be reused in other project objects.
	int gestureID = 0;
	//The name of this gesture. Not yet used, but can be useful to display it to the user when used.
	std::string name = "Gesture";

public:
	//Constant that decides if a gesture object really is a 'gesture' or a 'posture'.
	const int POSTURE_THRESHOLD = 20;

	Gesture(Frame & frameToAdd);
	Gesture(std::vector<Frame> & framesToAdd = std::vector<Frame>());

	std::vector<Frame> & getFrames();
	//Add the given frame to this gesture.
	void addFrame(Frame & frameToAdd);
	//Clear all frames of this gesture.
	void clearFrames();
	//Return the number of frames this gesture has.
	const int getNumberOfFrames() const;
	//Returns true if this gesture object is a posture, thus contains less than POSTURE_THRESHOLD frames.
	const bool isPosture() const;

	int getGestureID();
	void setGestureID(int ID);

	std::string getName();
	void setName(std::string & nameToSet);

	//Set the vector with labels that specify this gesture.
	void setLabelOrder(std::vector<int> labelOrderToSet);
	//Add a label to the label order vector.
	void addLabel(int labelToAdd);
	std::vector<int> & getLabelOrder();

	
};

//Unique ID for this gesture over all projects. Used as a filename when saving the data of this gesture.
static int appGestureID;
static int getAppGestureID();

#endif //GESTURE_H