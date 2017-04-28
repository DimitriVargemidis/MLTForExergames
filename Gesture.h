#ifndef	GESTURE_H
#define	GESTURE_H

#include <vector>
#include "libsvm-3.21\svm.h"


class Frame;

class Gesture {
private:
	std::vector<Frame> frames;
	int gestureID = 0;

public:
	const static int DIMENSIONS_PER_JOINT = 3;

	Gesture();
	const std::vector<Frame> & getFrames() const;
	void addFrame(Frame & frameToAdd);
	const int getNumberOfFrames() const;
	const int getNumberOfJointsPerFrame() const;
	const bool isStaticGesture() const;
	void applyRelativeTimestamps();
	svm_node * toArray();

	int getGestureID();
	void setGestureID(int ID);
};

static int appGestureID;
static int getAppGestureID();

#endif //GESTURE_H