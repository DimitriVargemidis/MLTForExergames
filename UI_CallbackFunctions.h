#pragma once
#include <Windows.h>
#include <vector>
#include <memory>

#include "Model.h"



namespace UI_CallbackFunctions
{
	void testCallback(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);

	void deleteGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);


};

