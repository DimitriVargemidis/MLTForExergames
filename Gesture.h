#ifndef	GESTURE_H
#define	GESTURE_H

#include <vector>
#include "libsvm-3.21\svm.h"
//#include "Frame.h"

class Frame;

class Gesture {
private:
	std::vector<Frame> frames;

public:
	const std::vector<Frame> & getFrames() const;
	void addFrame(const Frame frameToAdd);
	const int getNumberOfFrames() const;
	const int getNumberOfJointsPerFrame() const;
	const bool isStaticGesture() const;
	svm_node * toArray() const;
};

#endif //GESTURE_H