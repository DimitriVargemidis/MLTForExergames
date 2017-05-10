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
	//model->getGestureClassByID(ID)->DeleteGestureWithID(ID);
	printf("implement the delete function! ! \n");
}
