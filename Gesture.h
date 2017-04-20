#ifndef	GESTURE_H
#define	GESTURE_H

#include <vector>
#include "libsvm-3.21\svm.h"

class Frame;

class Gesture {
private:
	std::vector<Frame> frames;

public:
	const std::vector<Frame> & getFrames() const;
	void addFrame(Frame & frameToAdd);
	const int getNumberOfFrames() const;
	const int getNumberOfJointsPerFrame() const;
	const bool isStaticGesture() const;
	void applyRelativeTimestamps();
	svm_node * toArray();
};

#endif //GESTURE_H