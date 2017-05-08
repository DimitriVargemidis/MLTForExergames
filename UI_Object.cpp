#include "UI_Object.h"



UI_Object::UI_Object():
	color{D2D1::ColorF::White}
{
	//temporary hard coded object
	centerCoordImage.x = 100;
	centerCoordImage.y = 200;
	widthImage = 150;
	heightImage = 150;
}

UI_Object::UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col) :
	widthImage{ width }, heightImage{ height }, color{col}
{
	centerCoordImage.x = Xcenter;
	centerCoordImage.y = Ycenter;
}


UI_Object::~UI_Object()
{
}

void UI_Object::changeColor(D2D1::ColorF c)
{
	color = c;
}

void UI_Object::changeColor(float r, float g, float b)
{
	color = D2D1::ColorF(r, g, b, 1.0);
}

D2D1::ColorF UI_Object::getColor()
{
	return color;
}

void UI_Object::setCenter(D2D1_POINT_2F c)
{
	centerCoordImage = c;
}

D2D1_POINT_2F UI_Object::getCenter()
{
	return centerCoordImage;
}

void UI_Object::setWidth(float w)
{
	widthImage = w;
}

float UI_Object::getWidth()
{
	return widthImage;
}

void UI_Object::setHeight(float h)
{
	heightImage = h;
}

float UI_Object::getHeight()
{
	return heightImage;
}

void UI_Object::moveY(float move)
{
	centerCoordImage.y = centerCoordImage.y + move;
}

void UI_Object::moveYabs(float pos)
{
	centerCoordImage.y = pos;
}

void UI_Object::setVisible(const bool & visual)
{
	visible = visual;
}

const int UI_Object::getVisibele()
{
	return visible;
}



