#include "UI_TextObject.h"

extern D2D_Graphics graphics;

UI_TextObject::UI_TextObject()
{
}

UI_TextObject::UI_TextObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, std::wstring textContent, float size, DWRITE_TEXT_ALIGNMENT alignment) :
	UI_Object(Xcenter, Ycenter, width, height, col), textSize{ size }, text{textContent}, textAlignment{alignment}
{

}


UI_TextObject::~UI_TextObject()
{
}

void UI_TextObject::draw()
{
	graphics.drawText(text, getCenter(), getWidth(), getHeight(), getColor(),textSize, textAlignment);
}

void UI_TextObject::setText(const std::wstring & textToDraw)
{
	text = textToDraw;
}

const std::wstring & UI_TextObject::getText()
{
	return text;
}
