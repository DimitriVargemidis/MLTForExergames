#include "UI_Object.h"



UI_Object::UI_Object()
{
	//temporary hard coded object
	centerCoordImage.x = 100;
	centerCoordImage.y = 200;
	widthImage = 100;
	heightImage = 100;

	centerCoordActionArea.x = 100;
	centerCoordActionArea.y = 200;
	widthActionArea = 100;
	heightActionArea = 100;



}


UI_Object::~UI_Object()
{
}

//check whether the given coordinate is within the action area of the ui object
bool UI_Object::checkCoordInside(D2D1_POINT_2F coord)
{
	if (coord.x < centerCoordActionArea.x + widthActionArea / 2 &&
		coord.x > centerCoordActionArea.x - widthActionArea / 2 &&
		coord.y < centerCoordActionArea.y + heightActionArea / 2 &&
		coord.y > centerCoordActionArea.y - heightActionArea / 2)
	{
		return true;
	}

	return false;
}
