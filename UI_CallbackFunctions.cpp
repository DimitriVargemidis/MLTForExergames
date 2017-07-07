#include "UI.h"
//author: Christiaan Vanbergen 

#include "UI_CallbackFunctions.h"

void UI_CallbackFunctions::testCallback(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	{
		printf("test callback succesfull ! \n");
	}
}


void UI_CallbackFunctions::deleteGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	//look for the GestureClass object that is selected by the UI screen in the Model
	std::shared_ptr<GestureClass> gestureClass = model->getGestureClassByID(UI->getScreen()->getGestureClassID());
	if (gestureClass != nullptr)
	{
		//if found, change coloring of the associated UI elements
		UI->getScreen()->getScrollbar()->getActionIndicator()->setHorFillPercen(0.0F);
		UI->getScreen()->getScrollbar()->getSelectionBox()->changeColor(D2D1::ColorF(255.0 / 255.0, 102.0 / 255.0, 0));
		UI->getScreen()->getPlayVisual()->get_UI_Objects()[0]->changeBorderColor(D2D1::ColorF::LightGray);
		
		//delete the gesture in the model, update the UI screen
		gestureClass->DeleteGestureWithID(ID);
		UI->getScreen()->StopPlayGesture();
		model->setUpdatUI(true);
	}	
	
}

void UI_CallbackFunctions::recordGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	//set all flags necessary to start recording to true
	UI->getScreen()->setInitRecording(true);	
	UI->getScreen()->setShowRecordScreen(true);
	UI->getScreen()->setDrawAbsCoord(false);
	//start the recording in the model
	model->setRefresh(true);
	
	
}

void UI_CallbackFunctions::updateHitboxes(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI)
{
	model->setUpdatUI(true);
}
