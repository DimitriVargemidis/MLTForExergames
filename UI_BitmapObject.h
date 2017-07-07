#pragma once
//author: Christiaan Vanbergen 

#include "UI_Object.h"

//the different states that decide what bitmap is drawn
enum ObjectState {
	standard,
	hover,
	handActive
};

//This class is a child of UI_Object and draws different bitmaps within the rectangle defined by the parent dependent on the ObjectState

//author: Christiaan Vanbergen 
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
	ID2D1Bitmap* stdBitmap;			//bitmap drawn in the ObjectState.standard
	ID2D1Bitmap* hoverBitmap;		//bitmap drawn in the ObjectState.hover
	ID2D1Bitmap* handActiveBitmap;	//bitmap drawn in the ObjectState.handActive

	ObjectState state = standard;


	
};

