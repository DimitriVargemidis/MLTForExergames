#include "HeightAnalyser.h"
#include <math.h>

double HeightAnalyser::computePersonHeight(const NUI_SKELETON_DATA & skeleton)
{
	Vector4 head, neck, spine, waist, hipLeft, kneeLeft, ankleLeft;
	double lengthArray[5];

	head = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HEAD];
	neck = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER];
	spine = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE];
	waist = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_CENTER];
	hipLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_HIP_LEFT];
	kneeLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT];
	ankleLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT];

	lengthArray[0] = computeLengthBetweenPoints(head, neck);
	lengthArray[1] = computeLengthBetweenPoints(neck, spine);
	lengthArray[2] = computeLengthBetweenPoints(spine, waist);
	lengthArray[3] = computeLengthBetweenPoints(hipLeft, kneeLeft);
	lengthArray[4] = computeLengthBetweenPoints(kneeLeft, ankleLeft);
	//lengthArray[0] = computeLengthBetweenPoints(head, ankleLeft);

	double temp = computeSumOfLengthArray(lengthArray);
	temp = (temp + 15.0);
	return temp;
}

double HeightAnalyser::computeProportion(const NUI_SKELETON_DATA & skeleton)
{

	Vector4 neck, spine, kneeLeft, ankleLeft;

	neck = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SHOULDER_CENTER];
	spine = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_SPINE];
	kneeLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_KNEE_LEFT];
	ankleLeft = skeleton.SkeletonPositions[NUI_SKELETON_POSITION_ANKLE_LEFT];




	return (computeLengthBetweenPoints(kneeLeft, ankleLeft));
}

double HeightAnalyser::computeLengthBetweenPoints(Vector4 point1, Vector4 point2)
{
	
	LONG x1, y1, x2, y2;
	USHORT z1, z2;

	NuiTransformSkeletonToDepthImage(point1, &x1, &y1, &z1);
	NuiTransformSkeletonToDepthImage(point2, &x2, &y2, &z2);

	float Z1_temp, Z2_temp;

	Z1_temp = ((z1/100) + 5.133) / 0.81-10;
	Z2_temp = ((z2/100) + 5.133) / 0.81-10;

	//Z1_temp = z2;
	//Z2_temp = z2;

	return sqrt(
		pow(x1 - x2, 2) +
		pow(y1 - y2, 2) +
		pow(Z1_temp - Z2_temp, 2));

	/*
	sqrt(
		pow((point1.x - point2.x)*100, 2) +
		pow((point1.y - point2.y)*100, 2) +
		pow((point1.z - point1.z),2));
		*/
}

double HeightAnalyser::computeSumOfLengthArray(double lengthArray[5])
{
	double sum = 0.0;
	int i;
	for (i = 0; i < 5; i++)
	{
		sum += lengthArray[i];
	}
	return  sum;
}

