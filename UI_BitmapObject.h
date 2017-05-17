#pragma once

#include "UI_Object.h"

class UI_BitmapObject: public UI_Object
{
public:
	UI_BitmapObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, ID2D1Bitmap* bitmapImage);
	~UI_BitmapObject();

	void setBitmap(ID2D1Bitmap* bitmapImage);
	const ID2D1Bitmap* getBitmap();

	void draw() override;

private:
	ID2D1Bitmap* bitmap;
};

