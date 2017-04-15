#include "Frame.h"
#include <Kinect.h>

Frame::Frame()
{
}

Frame::~Frame()
{
}

Frame::Frame(const std::vector<Joint> & joints) : joints{joints} {
}

Frame::Frame(IBody * body, bool relative) {
	Joint bodyJoints[JointType_Count];
	
	body -> GetJoints(_countof(bodyJoints), bodyJoints);

	std::vector<Joint> jointVector(bodyJoints, bodyJoints + sizeof bodyJoints / sizeof bodyJoints[0]);
	
	if (relative)
	{	
		jointVector = convertToRelativeToJoint(JointType_SpineMid, jointVector);
	}

	//std::vector<Joint> jointVector(bodyJoints);
	joints = jointVector;
}

const std::vector<Joint> & Frame::getJoints() const {
	return joints;
}

const int Frame::getNumberOfJoints() const {
	return joints.size();
}

std::vector<Joint> Frame::convertToRelativeToJoint( _JointType center, std::vector<Joint> & joints)
{
	std::vector<Joint> transformedJoints(joints.size());

	for (int j = 0; j <  joints.size(); ++j)
	{
		//SELFMADE code to convert to coordinates relative to the spine
		//This is not done in the SVMinterface is it?
		CameraSpacePoint still = joints[j].Position;

		if (j == center)
		{
			still.X = 0.0;
			still.Y = 0.3;
			still.Z = 1.8;

		}
		else
		{
			still.X = joints[j].Position.X - joints[1].Position.X;//+ 0.05;
			still.Y = joints[j].Position.Y - joints[1].Position.Y + 0.3;
			still.Z = joints[j].Position.Z - joints[1].Position.Z + 1.8;
		}
		transformedJoints[j] = joints[j];
		transformedJoints[j].Position = still;
		//jointPoints[j] = graphics.BodyToScreen(still, width, height, m_pCoordinateMapper, cDepthWidth, cDepthHeight);
		//ShowJointCoordinates(transformedJoints, 0);		
	}
return transformedJoints;

}
