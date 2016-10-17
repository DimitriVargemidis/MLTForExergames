#include "Frame.h"

Frame::Frame(const std::vector<Joint> & joints) : joints{joints} {
}

Frame::Frame(IBody * body) {
	Joint bodyJoints[JointType_Count];
	body -> GetJoints(_countof(bodyJoints), bodyJoints);

	std::vector<Joint> jointVector(bodyJoints, bodyJoints + sizeof bodyJoints / sizeof bodyJoints[0]);
	joints = jointVector;
}

const std::vector<Joint> & Frame::getJoints() const {
	return joints;
}

const int Frame::getNumberOfJoints() const {
	return joints.size();
}
