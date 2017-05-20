#pragma once

#include <d2d1.h>
#include "D2D_Graphics.h"

/*
#ifndef GRAPHICS_UI_OBJECT
#define GRAPHICS_UI_OBJECT
extern D2D_Graphics			graphics;
#endif
*/
enum ObjectState;

class UI_Object
{

public:
	UI_Object();
	UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col);
	UI_Object(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, D2D1::ColorF filler , float borderthick);
	~UI_Object();

	void changeColor(D2D1::ColorF c);
	void changeColor(float r, float g, float b);
	D2D1::ColorF getColor();

	void changeBorderColor(D2D1::ColorF c);
	void changeFillColor(D2D1::ColorF c);

	void setCenter(D2D1_POINT_2F c);
	D2D1_POINT_2F getCenter();

	void setWidth( float w);
	float getWidth();

	void setHeight(float h);
	float getHeight();

	void setHorFillPercen(float percent);
	float getHorFillPercen();

	virtual void setText(const std::wstring & textToDraw);
	virtual const std::wstring & getText();

	void moveY(float move);

	void moveYabs(float pos);

	void setVisible(const bool & visual);
	const int  getVisibele();

	virtual void draw();

	virtual void setObjectState(ObjectState status) ;
	virtual const ObjectState getObjectState();

private:
	D2D1_POINT_2F centerCoordImage;
	float	widthImage;
	float	heightImage;
	float	borderthickness;				//thickness of the border
	float   HorFillPercen = 1.0F;					//the parcent of the object that is drawn from left to right (from 0 to 1), standard is full

	D2D1::ColorF borderColor;
	D2D1::ColorF fillColor;

	bool visible = true;

	

};

