#ifndef	GESTURE_H
#define	GESTURE_H

#include <vector>


class Frame;

class Gesture {
private:
	
	std::vector<Frame> frames;
	std::vector<int> labelOrder;
	int gestureID = 0;
	std::string name = "Gesture";

public:

	const int POSTURE_THRESHOLD = 20;

	Gesture(Frame & frameToAdd);
	Gesture(std::vector<Frame> & framesToAdd = std::vector<Frame>());

	std::vector<Frame> & getFrames();
	void addFrame(Frame & frameToAdd);
	void clearFrames();
	const int getNumberOfFrames() const;
	const bool isPosture() const;

	int getGestureID();
	void setGestureID(int ID);

	std::string getName();
	void setName(std::string & nameToSet);

	void setLabelOrder(std::vector<int> labelOrderToSet);
	void addLabel(int labelToAdd);
	std::vector<int> & getLabelOrder();

	
};

static int appGestureID;
static int getAppGestureID();

#endif //GESTURE_H