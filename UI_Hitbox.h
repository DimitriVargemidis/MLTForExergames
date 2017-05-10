#pragma once

//#include <d2d1.h>
//#include <vector>
//#include <memory>
//#include <Kinect.h>

//#include "ActionTrigger.h"

//#include "UI_Object.h"

//class Model;

#include "Abstr_UI_Hitbox.h"

class UI;

class UI_Hitbox : public Abstr_UI_Hitbox
{
public:
	UI_Hitbox();
	UI_Hitbox(	float Xcenter, float Ycenter, float width, float height,
				std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~UI_Hitbox();
	/*

	void setModel(std::shared_ptr<Model> m);
	std::shared_ptr<Model> getModel();
	
	void add_UI_Object(std::shared_ptr<UI_Object> object);
	void remove_UI_Object(std::shared_ptr<UI_Object> object); //TO DO define function
	std::vector<std::shared_ptr<UI_Object>> get_UI_Objects();

	void addInputJoint(JointType joint);
	std::vector<JointType> getInputJoints();

	bool checkCoordInside(D2D1_POINT_2F coord);
	bool checkInputJointType(JointType type);

	void setHover(bool a);
	void setHover(const bool & a, const JointType & joint, const HandState & hand, const D2D1_POINT_2F & jointPoint);
	bool getHover();

	void setHoverJoint(JointType joint);
	JointType getHoverJoint();

	//act executed when a condition is met, to be overrided by the child
	virtual void HoverOnAction();
	virtual void HoverOffAction();
	virtual void HoverHoldAction();

	virtual void ActiveHandOnAction();
	virtual void ActiveHandOffAction();
	virtual void ActiveHandHoldAction();
	*/ 
	virtual void action(ActionTrigger act, const D2D1_POINT_2F & coord ) override;
	/*
	//check whether the right or left hand are controlling this object
	bool isRightHandActive();
	bool isLeftHandActive();

	void setCenterCoordActionArea(D2D1_POINT_2F center);
	D2D1_POINT_2F  getCenterCoordActionArea();
	*/
	virtual void activateFunction() override;

	virtual void setLastPoint(D2D1_POINT_2F pos);
	virtual D2D1_POINT_2F getLastPoint();
	/*
	virtual void deactivateFunction();

	void setFunctionActivation(bool function);
	bool getFunctionActivation();
	*/

private:
	/*
	std::shared_ptr<Model> model;

	std::vector<std::shared_ptr<UI_Object>> UI_objects;	//The UI_objects it will affect directly
	std::vector<JointType> inputJoints;					//The joints that can interact with this hitbox		

	D2D1_POINT_2F centerCoordActionArea;
	float	widthActionArea;
	float heightActionArea;

	HandState Activehandstate = HandState_Closed;
	JointType HoverJoint = JointType_WristLeft; //the joint that has activated the button

	bool hover = false;				//active when joint is hover over hitbox
	bool activeHand = false;		//active when hand is in activehandstate over hitbox
	bool activeHandOutside = false;	//active when hand was in activehandstate over hitbox and is still in this state outside the hitbox
	bool functionActive = false;	//active when the criteria for activating its model function are met

	int  InertiaCounter = 0;
	const int InertiaLimit = 4;
	*/
};
