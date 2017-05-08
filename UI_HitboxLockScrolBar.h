#pragma once
#include "UI_HitboxScrolBar.h"
class UI_HitboxLockScrolBar :
	public UI_HitboxScrolBar
{
public:
	UI_HitboxLockScrolBar();
	UI_HitboxLockScrolBar(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation = 0.5f,
							std::function<void(int)> callback = [](int) { printf("no particular action defined = wrong constructor used"); }, int ID_Model = -1);
	~UI_HitboxLockScrolBar();

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	void add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox) override;

	void findSelectedHitbox();
	void lock_UI_Elements();

	

private:
	int selectedHitbox;
};

