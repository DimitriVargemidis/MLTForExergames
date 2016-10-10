#ifndef	GESTURE_H
#define	GESTURE_H

#include "Kinect.h"
#include <vector>

class Gesture {
private:
	std::vector< std::vector<Joint> > joints;

public:
	std::vector< std::vector<Joint> > getJoints() const;
	void addFrame(const std::vector<Joint> & frameJoints);
	bool isStaticGesture() const;
	int getNumberOfJointsPerFrame() const;
};

#endif //GESTURE_H