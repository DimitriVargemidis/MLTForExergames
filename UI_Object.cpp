#include "UI_Object.h"


extern D2D_Graphics			graphics;


UI_Object::UI_Object():
	color{D2D1::ColorF::Beige}, fillColor{ D2D1::ColorF::Beige }, borderthickness{ borderthickness }
{
	//temporary hard coded object
	centerCoordImage.x = 100+250;
	centerCoordImage.y = 200;
	widthImage = 150;
	heightImage = 150;

}

UI_Object::UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col) :
	UI_Object{ Xcenter,  Ycenter, width,  height, col ,col, 1}
{
	
}

UI_Object::UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, D2D1::ColorF filler, float borderthick) :
	widthImage{ width }, heightImage{ height }, color{ col }, fillColor{ filler },  borderthickness{ borderthick }
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
	fillColor = c;
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

void UI_Object::setText(const std::wstring & textToDraw)
{
}

const std::wstring & UI_Object::getText()
{
	return NULL;
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

void UI_Object::draw()
{
	graphics.drawRectangle(centerCoordImage, widthImage, heightImage, color);
	graphics.drawRectangle(centerCoordImage, (widthImage-(2*borderthickness)), (heightImage -(2*borderthickness)), fillColor);

}



