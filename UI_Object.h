#pragma once

#include <d2d1.h>



class UI_Object
{

public:
	UI_Object();
	~UI_Object();

	void changeColor(D2D1::ColorF c);
	void changeColor(float r, float g, float b);
	D2D1::ColorF getColor();

	void setCenter(D2D1_POINT_2F c);
	D2D1_POINT_2F getCenter();

	void setWidth( float w);
	float getWidth();

	void setHeight(float h);
	float getHeight();

private:
	D2D1_POINT_2F centerCoordImage;
	float	widthImage;
	float heightImage;

	D2D1::ColorF color;

};

