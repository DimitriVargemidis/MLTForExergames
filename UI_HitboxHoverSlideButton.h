#pragma once
#include "Abstr_UI_HitboxSlideButton.h"

class UI;

class UI_HitboxHoverSlideButton :
	public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxHoverSlideButton();
	UI_HitboxHoverSlideButton(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation= 0.5f,
								std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1, float XImageOffset = 0);
	~UI_HitboxHoverSlideButton();

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	virtual void	moveLeftAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveRightAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveDownAction(D2D1_POINT_2F ref, float move) override;
	virtual void	moveUpAction(D2D1_POINT_2F ref, float move) override;

	virtual void	activateFunction() override;

	virtual void	setXoffset(float offset) override;
	virtual float	getXoffset() override;
	
	void setMoveOn(bool move);
	bool getMoveOn();
private:

	float orginalWidth;
	float Xoffset;

	bool moveOn = false;

};

