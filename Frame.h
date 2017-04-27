#ifndef FRAME_H
#define FRAME_H

#include "Kinect.h"
#include <vector>


class Frame {
private:
	const float THRESHOLD_EQUALS = 0.15;

	std::vector<Joint> joints;
	double timestamp = 0;	//Stores the moment on which the frame is created.

public:
	const static int NB_OF_JOINTS = 25;

	Frame();
	Frame(std::vector<Joint> jointVector, bool relative = true);
	Frame(IBody * body, bool relative = true);

	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
	bool equals(Frame frameToCompare) const;

	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;

	const double getTimestamp() const;
	void setTimestamp(double timestampToSet);
};

#endif //FRAME_H
