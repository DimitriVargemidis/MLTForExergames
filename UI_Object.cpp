#include "UI_Object.h"
//author: Christiaan Vanbergen 

extern D2D_Graphics			graphics;


UI_Object::UI_Object():
	borderColor{D2D1::ColorF::Beige}, fillColor{ D2D1::ColorF::Beige }, borderthickness{ borderthickness }
{
	//temporary hard coded object
	centerCoordImage.x = 100+250;
	centerCoordImage.y = 200;
	widthImage = 150;
	heightImage = 150;

}

UI_Object::UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col) :
	UI_Object{ Xcenter,  Ycenter, width,  height, col ,col, 0}
{
	
}

UI_Object::UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, D2D1::ColorF filler, float borderthick) :
	widthImage{ width }, heightImage{ height }, borderColor{ col }, fillColor{ filler },  borderthickness{ borderthick }
{
	centerCoordImage.x = Xcenter;
	centerCoordImage.y = Ycenter;
}


UI_Object::~UI_Object()
{
}

void UI_Object::changeColor(D2D1::ColorF c)
{
	c.a = alpha;
	borderColor = c;
	fillColor = c;

}

void UI_Object::changeColor(float r, float g, float b)
{
	borderColor = D2D1::ColorF(r, g, b, alpha);
}

D2D1::ColorF UI_Object::getColor()
{
	return borderColor;
}

void UI_Object::changeBorderColor(D2D1::ColorF c)
{
	c.a = alpha;
	borderColor = c;
}

void UI_Object::changeFillColor(D2D1::ColorF c)
{
	c.a = alpha;
	fillColor = c;
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

void UI_Object::setHorFillPercen(float percent)
{
	HorFillPercen = percent;
}

float UI_Object::getHorFillPercen()
{
	return HorFillPercen;
}

void UI_Object::setAlpha(float a)
{
	alpha = a;
	borderColor.a = alpha;
	fillColor.a = alpha;
}

float UI_Object::getAlpha()
{
	return alpha;
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
	//graphics.drawRectangle(centerCoordImage, widthImage, heightImage, borderColor);

	graphics.drawRectangle(centerCoordImage.x-(widthImage/2), centerCoordImage.y - (heightImage / 2), centerCoordImage.x - (widthImage / 2) + widthImage*HorFillPercen, centerCoordImage.y + (heightImage / 2), borderColor);
	if (borderthickness != 0 && HorFillPercen == 1.0F)
	{
		graphics.drawRectangle(centerCoordImage, (widthImage - (2 * borderthickness)), (heightImage - (2 * borderthickness)), fillColor);
	}
}

void UI_Object::setObjectState(ObjectState status)
{
}

const ObjectState UI_Object::getObjectState()
{
	return ObjectState();
}



