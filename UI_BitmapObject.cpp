
//#include "D2D_Graphics.h"

#include "UI_BitmapObject.h"

extern D2D_Graphics graphics;


UI_BitmapObject::UI_BitmapObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, ID2D1Bitmap* bitmapImage):
	UI_Object(Xcenter, Ycenter, width, height, col), stdBitmap{ bitmapImage }, hoverBitmap{bitmapImage}, handActiveBitmap{bitmapImage}
{
}


UI_BitmapObject::~UI_BitmapObject()
{
}

void UI_BitmapObject::setBitmap(ID2D1Bitmap * bitmapImage)
{
	stdBitmap = bitmapImage;
}

const ID2D1Bitmap * UI_BitmapObject::getBitmap()
{
	return stdBitmap;
}

void UI_BitmapObject::setHoverBitmap(ID2D1Bitmap * bitmapImage)
{
	hoverBitmap = bitmapImage;
}

const ID2D1Bitmap * UI_BitmapObject::getHoverBitmap()
{
	return hoverBitmap;
}

void UI_BitmapObject::setHandActiveBitmap(ID2D1Bitmap * bitmapImage)
{
	handActiveBitmap = bitmapImage;
}

const ID2D1Bitmap * UI_BitmapObject::getHandActiveBitmap()
{
	return handActiveBitmap;
}

void UI_BitmapObject::setObjectState(ObjectState status)
{
	state = status;
}

const ObjectState UI_BitmapObject::getObjectState()
{
	return state;
}


void UI_BitmapObject::draw()
{
	switch (state)
	{
	case standard :
		graphics.drawBitmap(stdBitmap, getCenter(), getWidth(), getHeight());
		break;
	case hover:
		graphics.drawBitmap(hoverBitmap, getCenter(), getWidth(), getHeight());
		break;
	case handActive:
		graphics.drawBitmap(handActiveBitmap, getCenter(), getWidth(), getHeight());
		break;
	}
}
