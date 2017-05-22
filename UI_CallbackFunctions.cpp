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
		UI->getScreen()->getScrollbar()->getActionIndicator()->setHorFillPercen(0.0F);
		UI->getScreen()->getScrollbar()->getSelectionBox()->changeColor(D2D1::ColorF(255.0 / 255.0, 102.0 / 255.0, 0));
		UI->getScreen()->getPlayVisual()->get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::LightGray);
		gestureClass->DeleteGestureWithID(ID);
		UI->getScreen()->StopPlayGesture();
		model->setUpdatUI(true);
	}	
	else
	{
		//printf("no gestureClass with that ID !!\n");
	}
	
}

void UI_CallbackFunctions::recordGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	UI->getScreen()->setInitRecording(true);
	UI->getScreen()->setShowRecordScreen(true);
	UI->getScreen()->setDrawAbsCoord(false);
	model->setRefresh(true);
	
	
}

void UI_CallbackFunctions::updateHitboxes(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	model->setUpdatUI(true);
}
