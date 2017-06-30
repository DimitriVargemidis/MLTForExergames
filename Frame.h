#ifndef FRAME_H
#define FRAME_H

#include <vector>

#include "Kinect.h"
#include "libsvm-3.21\svm.h"


class Frame {
private:
	//Variable used for storing the label specifying the (SVM) class this frame belongs to.
	int label = -1;
	//Variable registering the state of the right hand.
	HandState rightHand;
	//Variable registering the state of the left hand.
	HandState leftHand;
	//A vector of joints. Mind the order of the joints as they need to correspond to other frames!
	//Check the Microsoft Kinect libraries for more info on joints.
	std::vector<Joint> joints;

public:
	//Constant that decides how much difference there can be between two joints (for each dimension) before it is recognized as being different.
	//(Unit: meters)
	const float THRESHOLD_EQUALS = 0.10;
	//The number of dimensions that is stored for each joint.
	const static int DIMENSIONS_PER_JOINT = 3;
	//The number of joints the Kinect camera supports.
	const static int NB_OF_JOINTS = 25;
	//The number of dimensions required to consider one frame as a single data point.
	const static int FRAME_DIMENSIONS = DIMENSIONS_PER_JOINT * NB_OF_JOINTS;

	Frame();
	Frame(std::vector<Joint> & jointVector, bool relative = true);
	Frame(IBody * body, bool relative = true);

	void setFrame(Frame frame);
	
	//Converts the frame to a relative base using the center joint as a relative point.
	std::vector<Joint> convertToRelativeToJoint(_JointType center, std::vector<Joint> & joints);
	std::vector<Joint> convertToNearAbsToJoint(_JointType center, std::vector<Joint> & joints);
	bool equals(Frame frameToCompare) const;

	const std::vector<Joint> & getJoints() const;
	const int getNumberOfJoints() const;

	const HandState getRightHand() const;
	void setRightHand(HandState right);

	const HandState getLeftHand() const;
	void setLeftHand(HandState left);

	//Convert this frame to an svm_node for easy use when training an SVM model.
	svm_node * toArray() const;

	void setLabel(int labelToSet);
	const int getLabel() const;

};

#endif //FRAME_H
