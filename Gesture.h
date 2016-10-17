#ifndef	GESTURE_H
#define	GESTURE_H

#include "Frame.h"
#include <vector>

class Gesture {
private:
	std::vector<Frame> frames;

public:
	std::vector<Frame> getFrames() const;
	void addFrame(const Frame & frameToAdd);
	int getNumberOfFrames() const;
	bool isStaticGesture() const;
};

#endif //GESTURE_H