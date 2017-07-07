#pragma once
//author: Christiaan Vanbergen 
#include <Windows.h>
#include <vector>
#include <memory>

#include "Model.h"

//namespace containing callback functions that can be assigned to hitbox elements to be called when the hitbox is activated.
//Here functions can be added as needed for new functionalities that can be activated through new kinect user interfaces screens

//author: Christiaan Vanbergen 
namespace UI_CallbackFunctions
{
	//placeholder callback function
	void testCallback(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);
	//callback function to delete a Gesture 
	void deleteGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);
	//callback function that start the recording of a gesture
	void recordGesture(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);
	//Obsolete
	void updateHitboxes(int ID, int HitBoxIndex, std::shared_ptr<Model> model, std::shared_ptr<UI> UI);
};

