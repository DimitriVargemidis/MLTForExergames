
//#include "D2D_Graphics.h"

#include "UI_BitmapObject.h"

extern D2D_Graphics graphics;


UI_BitmapObject::UI_BitmapObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, ID2D1Bitmap* bitmapImage):
	UI_Object(Xcenter, Ycenter, width, height, col), bitmap{bitmapImage}
{
}


UI_BitmapObject::~UI_BitmapObject()
{
}

void UI_BitmapObject::setBitmap(ID2D1Bitmap * bitmapImage)
{
	bitmap = bitmapImage;
}

const ID2D1Bitmap * UI_BitmapObject::getBitmap()
{
	return bitmap;
}

void UI_BitmapObject::draw()
{
	graphics.drawBitmap(bitmap, getCenter(), getWidth(), getHeight());
}
