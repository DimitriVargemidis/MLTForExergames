#ifndef FRAME_H
#define FRAME_H

#include "Kinect.h"
#include <vector>


class Frame {
private:
	std::vector<Joint> joints;

public:
	Frame();
	~Frame();

	Frame(const std::vector<Joint> & joints);
	Frame(IBody * body, bool relative);
	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;

	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
};

#endif //FRAME_H
