#ifndef	GESTURE_H
#define	GESTURE_H

#include "Frame.h"
#include <vector>

class Gesture {
private:
	std::vector<Frame> frames;

public:
	const std::vector<Frame> & getFrames() const;
	void addFrame(const Frame & frameToAdd);
	const int getNumberOfFrames() const;
	const int getNumberOfJointsPerFrame() const;
	const bool isStaticGesture() const;
};

#endif //GESTURE_H