#pragma once
#include <vector>
#include <memory>
#include <Kinect.h>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "UI_Object.h"
#include "Abstr_UI_Hitbox.h"

class Abstr_Screen
{
public:
	Abstr_Screen();
	~Abstr_Screen();

	void setModel(std::shared_ptr<Model> m);

	void activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand);
	void drawUI();

private:
	std::shared_ptr<Model> 	model;

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>>		UI_Hitboxes;	//list of UI control elements
	std::vector<std::shared_ptr<UI_Object>> 			UI_Objects;		//UI elements to be drawn

																		//simple way of making sure 1 hand does not activate multiple drag buttons
																		//Only for right and left hand avaiable this way
	bool rightHandBusy = false;		//boolean to indicate that the righthand is already controlling a button
	bool leftHandBusy = false;		//boolean to indicate that the lefthand is already controlling a button

};

