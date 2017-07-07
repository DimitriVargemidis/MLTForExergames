#pragma once
//author: Christiaan Vanbergen 
#include <vector>

#include "Frame.h"

#include "UI_Object.h"

//A child class of UI_Object that draws a the puppets from all the frames contained in it's vector

//author: Christiaan Vanbergen 
class UI_FramesObject : public UI_Object
{
public:
	UI_FramesObject();
	UI_FramesObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, std::vector<Frame> framesToDraw);
	~UI_FramesObject();

	void draw() override;

private:
	std::vector<Frame> frames;
	
};

