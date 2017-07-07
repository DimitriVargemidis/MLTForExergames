#pragma once
//author: Christiaan Vanbergen 

class Abstr_UI_Hitbox;

//namespace containing callback function that can be assigned to hitboxes to be called when the hitbox updates his information
//Here functions can be added as needed for new UI elements on new kinect user interfaces screens

//author: Christiaan Vanbergen 
namespace UI_Update_CallbackFunctions
{
	//Empty placeholder
	void standardUpdateFunction(Abstr_UI_Hitbox * hitbox);
	//Function for going through the recording stages and eventually stopping the recording of a gesture
	void stopRecord(Abstr_UI_Hitbox * hitbox);
	//Removes all elements from the scrollbar and replaces them with new elements based on the information in the Model
	void updateScrolbarGestures(Abstr_UI_Hitbox * hitbox);
	//changes colors of the replay screen box
	void updatePlayVisual(Abstr_UI_Hitbox * hitbox);


};

