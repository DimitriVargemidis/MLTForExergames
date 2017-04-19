#ifndef FRAME_H
#define FRAME_H

#include "Kinect.h"
#include <vector>


class Frame {
private:
	std::vector<Joint> joints;
	double timestamp = 0;	//Stores the moment on which the frame is created.

public:
	Frame(IBody * body, bool relative);

	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;
	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
	
	const double getTimestamp() const;
	void setTimestamp(double timestampToSet);
};

#endif //FRAME_H
