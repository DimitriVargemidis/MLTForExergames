#pragma once

#include "UI_Object.h"

class UI_TextObject: public UI_Object
{
public:
	UI_TextObject();
	UI_TextObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, std::wstring t, float size , DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_CENTER);
	~UI_TextObject();

	void draw() override;

	void setText(const std::wstring & textToDraw) override;
	const std::wstring & getText() override;

private:
	std::wstring text;

	float textSize;
	DWRITE_TEXT_ALIGNMENT textAlignment;
};

