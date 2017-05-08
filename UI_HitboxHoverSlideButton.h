#pragma once
#include "Abstr_UI_HitboxSlideButton.h"
class UI_HitboxHoverSlideButton :
	public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxHoverSlideButton();
	UI_HitboxHoverSlideButton(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation= 0.5f,
								std::function<void(int)> callback = [](int) { printf("no particular action defined = wrong constructor used"); }, int ID_Model = -1);
	~UI_HitboxHoverSlideButton();

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	virtual void moveLeftAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveRightAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveDownAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveUpAction(D2D1_POINT_2F ref, float move) override;

	virtual void activateFunction() override;
};

