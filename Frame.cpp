#include <Kinect.h>
#include <cmath>

#include "Frame.h"


Frame::Frame() : leftHand{ HandState_Open }, rightHand{ HandState_Open }
{
}

Frame::Frame(std::vector<Joint> & jointVector, bool relative) : leftHand{ HandState_Open },rightHand{ HandState_Open }
{
	if (relative)
	{
		jointVector = convertToRelativeToJoint(JointType_SpineMid, jointVector);
	}
	joints = jointVector;
}

Frame::Frame(IBody * body, bool relative)
{
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

//Converts the frame to a relative base using the center joint as a relative point.
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

	float Xcenter = joints[center].Position.X;
	float Ycenter = joints[center].Position.Y;

	for (int j = 0; j < joints.size(); ++j)
	{
		CameraSpacePoint still = joints[j].Position;
	
		still.X = Xcenter;
		still.Y = 0;
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

//Convert this frame to an svm_node for easy use when training an SVM model.
//Relies on use of LibSVM (see online). This requires use of older C++ structures,
//so inefficient stuff needs to be done!
svm_node * Frame::toArray() const
{
	svm_node * rawArray{ new svm_node[FRAME_DIMENSIONS + 1] };
	int indexCount{ 0 };
	for (int i = 0; i < NB_OF_JOINTS; i++) {
		rawArray[indexCount].index = indexCount;
		rawArray[indexCount].value = getJoints().at(i).Position.X;
		indexCount = indexCount + 1;
		rawArray[indexCount].index = indexCount;
		rawArray[indexCount].value = getJoints().at(i).Position.Y;
		indexCount = indexCount + 1;
		rawArray[indexCount].index = indexCount;
		rawArray[indexCount].value = getJoints().at(i).Position.Z;
		indexCount = indexCount + 1;
	}
	
	rawArray[indexCount].index = -1;
	rawArray[indexCount].value = 0;
	return rawArray;
}

void Frame::setLabel(int labelToSet)
{
	label = labelToSet;
}

const int Frame::getLabel() const
{
	return label;
}
