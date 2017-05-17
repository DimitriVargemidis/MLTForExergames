#ifndef FRAME_H
#define FRAME_H

#include <vector>

#include "Kinect.h"
#include "libsvm-3.21\svm.h"


class Frame {
private:
	HandState rightHand;
	HandState leftHand;
	std::vector<Joint> joints;

public:
	const float THRESHOLD_EQUALS = 0.12;
	const static int DIMENSIONS_PER_JOINT = 3;
	const static int NB_OF_JOINTS = 25;
	const static int FRAME_DIMENSIONS = DIMENSIONS_PER_JOINT * NB_OF_JOINTS;

	Frame();
	Frame(std::vector<Joint> & jointVector, bool relative = true);
	Frame(IBody * body, bool relative = true);

	void setFrame(Frame frame);
	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
	std::vector<Joint> convertToNearAbsToJoint(_JointType center, std::vector<Joint> & joints);
	bool equals(Frame frameToCompare) const;

	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;

	const HandState getRightHand() const;
	void setRightHand(HandState right);

	const HandState getLeftHand() const;
	void setLeftHand(HandState left);

	svm_node * toArray() const;

};

#endif //FRAME_H
