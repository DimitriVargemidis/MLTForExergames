#ifndef FRAME_H
#define FRAME_H

#include "Kinect.h"
#include <vector>


class Frame {
private:
	const float THRESHOLD_EQUALS = 0.20;

	HandState rightHand;
	HandState leftHand;
	std::vector<Joint> joints;
	double timestamp = 0;	//Stores the moment on which the frame is created.

public:
	const static int NB_OF_JOINTS = 25;

	Frame();
	Frame(std::vector<Joint> jointVector, bool relative = true);
	Frame(IBody * body, bool relative = true);

	void setFrame(Frame frame);
	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
	std::vector<Joint> convertToNearAbsToJoint(_JointType center, std::vector<Joint> & joints);
	bool equals(Frame frameToCompare) const;

	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;

	const double getTimestamp() const;
	void setTimestamp(double timestampToSet);

	const HandState getRightHand() const;
	void setRightHand(HandState right);

	const HandState getLeftHand() const;
	void setLeftHand(HandState left);

};

#endif //FRAME_H
