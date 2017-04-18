#include "Frame.h"
#include <Kinect.h>
#include <chrono>


Frame::Frame()
{
}

Frame::~Frame()
{
}

Frame::Frame(const std::vector<Joint> & joints) : joints{joints} {
	setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()).count());
}

Frame::Frame(IBody * body, bool relative) {
	Joint bodyJoints[JointType_Count];
	
	body -> GetJoints(_countof(bodyJoints), bodyJoints);

	std::vector<Joint> jointVector(bodyJoints, bodyJoints + sizeof bodyJoints / sizeof bodyJoints[0]);
	
	if (relative)
	{	
		jointVector = convertToRelativeToJoint(JointType_SpineMid, jointVector);
	}

	joints = jointVector;
}

const std::vector<Joint> & Frame::getJoints() const {
	return joints;
}

const int Frame::getNumberOfJoints() const {
	return joints.size();
}

std::vector<Joint> Frame::convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints)
{
	std::vector<Joint> transformedJoints(joints.size());

	for (int j = 0; j <  joints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		CameraSpacePoint still = joints[j].Position;
		
		still.X = 0.0;
		still.Y = 0.3;
		still.Z = 1.8;
		
		if(j != center)
		{
			still.X = still.X + joints[j].Position.X - joints[1].Position.X;
			still.Y = still.Y + joints[j].Position.Y - joints[1].Position.Y;
			still.Z = still.Z + joints[j].Position.Z - joints[1].Position.Z;
		}
		
		transformedJoints[j] = joints[j];
		transformedJoints[j].Position = still;
	}
	return transformedJoints;
}

const double Frame::getTimestamp() const
{
	return timestamp;
}

void Frame::setTimestamp(double timestampToSet)
{
	timestamp = timestampToSet;
}
