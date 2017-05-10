#pragma once
#include "Abstr_UI_HitboxSlideButton.h"

class UI;

class UI_HitboxSlideButton :
	public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxSlideButton();
	UI_HitboxSlideButton(float Xcenter, float Ycenter, float width, float height,
							float left = 100.0, float right = 0.0, float up =0.0, float down = 0.0, float activation = 0.5,
		std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~UI_HitboxSlideButton();

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	virtual void activateFunction() override;

	virtual void moveLeftAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveRightAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveDownAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveUpAction(D2D1_POINT_2F ref, float move) override;

private:
	bool activeOutside = false;

};


