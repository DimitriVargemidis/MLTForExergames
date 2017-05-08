#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

#include "ActionTrigger.h"


namespace UI_CallbackFunctions
{
	typedef void (CALLBACK *UI_Action)(const ActionTrigger act, std::vector<std::shared_ptr<UI_Object>> UI_Objects, UI_Hitbox *hitbox);


	void CALLBACK dragButton(const ActionTrigger act, std::vector<std::shared_ptr<UI_Object>> UI_Objects, UI_Hitbox *hitbox);
};

