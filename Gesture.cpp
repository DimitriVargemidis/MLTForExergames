#include <stdexcept>
#include <memory>
#include "Gesture.h"
#include "Frame.h"

const int DIMENSIONS_PER_JOINT = 3;

const std::vector<Frame> & Gesture::getFrames() const {
	return frames;
}

void Gesture::addFrame(const Frame frameToAdd) {
	if (frameToAdd.getNumberOfJoints() == 0) {
		throw std::invalid_argument("The given frame must contain at least one joint.");
	}
	if ( (getNumberOfFrames() != 0) && (getFrames().at(0).getNumberOfJoints() != frameToAdd.getNumberOfJoints()) ) {
		throw std::invalid_argument("Size of the given frame must match the size of the previously added frames.");
	}
	frames.push_back(frameToAdd);
}

const int Gesture::getNumberOfFrames() const {
	return frames.size();
}

const int Gesture::getNumberOfJointsPerFrame() const {
	return frames.at(0).getNumberOfJoints();
}

const bool Gesture::isStaticGesture() const {
	return (getNumberOfFrames() == 1);
}

svm_node * Gesture::toArray() const {
	const int dimensions{getNumberOfFrames() * getNumberOfJointsPerFrame() * DIMENSIONS_PER_JOINT};
	const int nbOfJoints{getNumberOfFrames() * getNumberOfJointsPerFrame()};
	svm_node * rawArray{new svm_node[dimensions+1]};
	int indexCount{0};
	for (int i = 0; i < getNumberOfFrames(); i++) {
		for (int j = 0; j < nbOfJoints; j++) {
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.X;
			indexCount = indexCount + 1;
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.Y;
			indexCount = indexCount + 1;
			rawArray[indexCount].index = indexCount;
			rawArray[indexCount].value = getFrames().at(i).getJoints().at(j).Position.Z;
			indexCount = indexCount + 1;
		}
	}
	rawArray[indexCount].index = -1;
	rawArray[indexCount].value = 0;
	return rawArray;
}
