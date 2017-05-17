#pragma once
#include <vector>

#include "Frame.h"

#include "UI_Object.h"

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

