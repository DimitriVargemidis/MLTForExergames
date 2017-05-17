#include "UI.h"

#include "UI_CallbackFunctions.h"

void UI_CallbackFunctions::testCallback(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	{
		printf("test callback succesfull ! \n");
	}
}

void UI_CallbackFunctions::deleteGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	std::shared_ptr<GestureClass> gestureClass = model->getGestureClassByID(UI->getScreen()->getGestureClassID());
	if (gestureClass != nullptr)
	{
		gestureClass->DeleteGestureWithID(ID);
		model->setUpdatUI(true);
	}	
	else
	{
		printf("no gestureClass with that ID !!\n");
	}
	
}

void UI_CallbackFunctions::recordGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	UI->getScreen()->setShowRecordScreen(true);
	UI->getScreen()->setDrawAbsCoord(false);
	model->setRefresh(true);
	
	
}

void UI_CallbackFunctions::updateHitboxes(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	model->setUpdatUI(true);
}
