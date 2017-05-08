#include <Kinect.h>
#include <chrono>
#include <cmath>

#include "Frame.h"


Frame::Frame():
	leftHand { HandState_Open }, rightHand{ HandState_Open }
{
	//Set the (absolute) timestamp
	//setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
	//	std::chrono::system_clock::now().time_since_epoch()).count());
	setTimestamp(0);
}

Frame::Frame(std::vector<Joint> jointVector, bool relative):
	leftHand{HandState_Open},rightHand{HandState_Open}
{
	//Set the (absolute) timestamp
	//setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
	//	std::chrono::system_clock::now().time_since_epoch()).count());
	setTimestamp(0);

	if (relative)
	{
		jointVector = convertToRelativeToJoint(JointType_SpineMid, jointVector);
	}
	joints = jointVector;
}

Frame::Frame(IBody * body, bool relative)
{
	//Set the (absolute) timestamp
	//setTimestamp(std::chrono::duration_cast<std::chrono::milliseconds>(
	//	std::chrono::system_clock::now().time_since_epoch()).count());
	setTimestamp(0);

	//Get the joints from the given IBody
	Joint bodyJoints[JointType_Count];
	body -> GetJoints(_countof(bodyJoints), bodyJoints);
	std::vector<Joint> jointVector(bodyJoints, bodyJoints + sizeof bodyJoints / sizeof bodyJoints[0]);
	
	if (relative)
	{	
		jointVector = convertToRelativeToJoint(JointType_SpineMid, jointVector);
	}
	else
	{
		jointVector = convertToNearAbsToJoint(JointType_SpineMid, jointVector);
	}
	joints = jointVector;

	body->get_HandLeftState(&leftHand);
	body->get_HandRightState(&rightHand);
}

void Frame::setFrame(Frame frame)
{
	joints = frame.getJoints();
}

std::vector<Joint> Frame::convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints)
{
	std::vector<Joint> transformedJoints(joints.size());

	for (int j = 0; j < joints.size(); ++j)
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

std::vector<Joint> Frame::convertToNearAbsToJoint(_JointType center, std::vector<Joint>& joints)
{
	std::vector<Joint> transformedJoints(joints.size());

	for (int j = 0; j < joints.size(); ++j)
	{
		//Convert to coordinates relative to the spine
		CameraSpacePoint still = joints[j].Position;

		//still.X = 0.0;
		still.Y = 0.3;
		still.Z = 2.6;

		if (j != center)
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

bool Frame::equals(Frame frameToCompare) const
{
	if (getJoints().size() != frameToCompare.getJoints().size())
	{
		return false;
	}

	for (int i = 0; i < getJoints().size(); i++)
	{
		if (abs(getJoints().at(i).Position.Y - frameToCompare.getJoints().at(i).Position.Y) > THRESHOLD_EQUALS ||
			abs(getJoints().at(i).Position.X - frameToCompare.getJoints().at(i).Position.X) > THRESHOLD_EQUALS ||
			abs(getJoints().at(i).Position.Z - frameToCompare.getJoints().at(i).Position.Z) > THRESHOLD_EQUALS)
		{
			return false;
		}
	}
	return true;
}

const std::vector<Joint> & Frame::getJoints() const
{
	return joints;
}

const int Frame::getNumberOfJoints() const
{
	return joints.size();
}

const double Frame::getTimestamp() const
{
	return timestamp;
}

void Frame::setTimestamp(double timestampToSet)
{
	timestamp = timestampToSet;
}

const HandState Frame::getRightHand() const
{
	return rightHand;
}

void Frame::setRightHand(HandState right)
{
	rightHand = right;
}

const HandState Frame::getLeftHand() const
{
	return leftHand;
}

void Frame::setLeftHand(HandState left)
{
	leftHand = left;
}
