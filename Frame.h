#ifndef FRAME_H
#define FRAME_H

#include "Kinect.h"
#include <vector>


class Frame {
private:
	std::vector<Joint> joints;

public:
	Frame(std::vector<Joint> joints);
	std::vector<Joint> getJoints() const;
	int getNumberOfJoints() const;
};

#endif //FRAME_H
