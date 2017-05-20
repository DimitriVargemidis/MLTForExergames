#pragma once

#include "UI_Object.h"

enum ObjectState {
	standard,
	hover,
	handActive
};

class UI_BitmapObject: public UI_Object
{
public:
	UI_BitmapObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, ID2D1Bitmap* bitmapImage);
	~UI_BitmapObject();

	void setBitmap(ID2D1Bitmap* bitmapImage);
	const ID2D1Bitmap* getBitmap();

	void setHoverBitmap(ID2D1Bitmap* bitmapImage);
	const ID2D1Bitmap* getHoverBitmap();

	void setHandActiveBitmap(ID2D1Bitmap* bitmapImage);
	const ID2D1Bitmap* getHandActiveBitmap();

	void setObjectState(ObjectState status);
	const ObjectState getObjectState();

	void draw() override;

private:
	ID2D1Bitmap* stdBitmap;
	ID2D1Bitmap* hoverBitmap;
	ID2D1Bitmap* handActiveBitmap;

	ObjectState state = standard;


	
};

