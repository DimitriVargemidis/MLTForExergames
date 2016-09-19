#pragma once
#include "windows.h"
#include "NuiApi.h"

class HeightAnalyser {
public:
	double computePersonHeight(const NUI_SKELETON_DATA & skeleton);
	double computeProportion(const NUI_SKELETON_DATA & skeleton);
private:
	double computeLengthBetweenPoints(Vector4 point1, Vector4 point2);
	double computeSumOfLengthArray(double lengthArray[5]);
};