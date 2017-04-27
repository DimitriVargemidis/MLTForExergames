#pragma once
#include <Kinect.h>
#include <Windows.h>
#include <d2d1.h>



class UI_Object
{

public:
	UI_Object();
	~UI_Object();

	bool checkCoordInside(D2D1_POINT_2F coord);

private:
	D2D1_POINT_2F centerCoordImage;
	int	widthImage;
	int heightImage;

	D2D1_POINT_2F centerCoordActionArea;
	int	widthActionArea;
	int heightActionArea;




};

