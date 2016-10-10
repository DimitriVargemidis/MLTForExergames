#include "Frame.h"

Frame::Frame(std::vector<Joint> joints) : joints{joints} {
}

std::vector<Joint> Frame::getJoints() const {
	return joints;
}

int Frame::getNumberOfJoints() const {
	return joints.size();
}
