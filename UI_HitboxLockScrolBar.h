#pragma once
//author: Christiaan Vanbergen 
#include "UI_HitboxScrolBar.h"

class UI;

//a child class of UI_HitboxScrolbar that defines behavior for a selection box where 1 element resides with which can be interacted, the other elements are locked
//After scrolling each element locks into the position closes to it at that moment with one element in the selectionbox area.

//author: Christiaan Vanbergen 
class UI_HitboxLockScrolBar :
	public UI_HitboxScrolBar
{
public:
	UI_HitboxLockScrolBar();
	UI_HitboxLockScrolBar(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation = 0.5f,
							std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~UI_HitboxLockScrolBar();

	//define functions inherited from the parent
	void draw() override;

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	void add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox) override;

	void findSelectedHitbox();
	void lock_UI_Elements();

	void setRefFrame(const D2D1_POINT_2F ref);
	D2D1_POINT_2F getRefFrame();

	void							setSelectionBox(std::shared_ptr<UI_Object> selectionVisual);
	std::shared_ptr<UI_Object>		getSelectionBox();

private:
	int selectedHitbox;		// the place in the vector of the hitbox that is selected

	D2D1_POINT_2F refFrame;	//deprecated

	std::shared_ptr<UI_Object> selectionBox;	//pointer to the selectionBox UI_Object
};

