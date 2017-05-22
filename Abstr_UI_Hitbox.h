#pragma once

#include <d2d1.h>
#include <vector>
#include <memory>
#include <Kinect.h>
#include <functional>

#include "UI_CallbackFunctions.h"
#include "UI_Update_CallbackFunctions.h"
#include "ActionTrigger.h"

#include "UI_Object.h"
#include "Model.h"

class UI;

class Abstr_UI_Hitbox
{
public:
	Abstr_UI_Hitbox();
	Abstr_UI_Hitbox(float Xcenter, float Ycenter, float width, float height);
	Abstr_UI_Hitbox(	float Xcenter, float Ycenter, float width, float height, 
						std::function<void(int, int, std::shared_ptr<Model> ,std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model= -1);
	~Abstr_UI_Hitbox();

	virtual void setModel(std::shared_ptr<Model> m);
	virtual std::shared_ptr<Model> getModel();

	void setUI(std::shared_ptr<UI> m);
	std::shared_ptr<UI> getUI();

	void add_UI_Object(std::shared_ptr<UI_Object> object);
	void remove_UI_Object(std::shared_ptr<UI_Object> object); //TO DO define function
	std::vector<std::shared_ptr<UI_Object>> & get_UI_Objects();

	void addInputJoint(JointType joint);
	std::vector<JointType> getInputJoints();

	bool checkCoordInside(D2D1_POINT_2F coord);
	bool checkInputJointType(JointType type);

	void setHover(bool a);
	void setHover(const bool & a, const JointType & joint, const HandState & hand, const D2D1_POINT_2F & jointPoint);
	bool getHover();

	void setKeepHoverState(bool a);
	bool getKeepHoverState();

	void setActiveHand(const bool & hand);
	bool getActiveHand();

	void setActiveHandOutside(const bool & hand);
	bool getActiveHandOutside();

	void setHoverJoint(JointType joint);
	JointType getHoverJoint();

	//act executed when a condition is met, to be overrided by the child
	virtual void HoverOnAction();
	virtual void HoverOffAction();
	virtual void HoverHoldAction();

	virtual void ActiveHandOnAction();
	virtual void ActiveHandOffAction();
	virtual void ActiveHandHoldAction();

	virtual void action(ActionTrigger act, const D2D1_POINT_2F & coord = D2D1::Point2F(-1,-1)) = 0;

	//check whether the right or left hand are controlling this object
	virtual bool isRightHandActive();
	virtual bool isLeftHandActive();

	virtual void setCenterCoordActionArea(D2D1_POINT_2F center);
	D2D1_POINT_2F  getCenterCoordActionArea();
	void setWidth(float width);
	float getWidth();
	void setHeigth(float height);
	float getHeight();

	void setOriginalPos(D2D1_POINT_2F pos);
	D2D1_POINT_2F getOriginalPos();

	virtual void activateFunction() = 0;
	virtual void deactivateFunction();

	void setUpdateFunction(std::function<void(Abstr_UI_Hitbox*)> funct);
	std::function<void(Abstr_UI_Hitbox*)> getupdateFunction();

	void set_ID_ModelObject(int ID);
	int	 get_ID_ModelObject();

	void setFunctionActivation(bool function);
	bool getFunctionActivation();

	void setHandLock(bool hand);
	bool getHandLock();

	void setTotalLock(bool hand);
	bool getTotalLock();

	void setVisible(bool visual);
	bool getVisible();

	virtual void setLastPoint(D2D1_POINT_2F pos) = 0;
	virtual D2D1_POINT_2F getLastPoint() = 0;

	virtual void add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox);
	virtual void clear_UI_elements();

	virtual std::shared_ptr<UI_Object>		getActionIndicator();

	void moveY(float move);
	void moveYoriginalPos(float move);

	void moveAbsY(float pos);
	void moveAbsYoriginalPos(float pos);

	virtual void	setXoffset(float offset);
	virtual float	getXoffset();

	std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)>	activateFunctionCallback = UI_CallbackFunctions::testCallback;
	std::function<void(Abstr_UI_Hitbox*)>										updateFunctionCallback = UI_Update_CallbackFunctions::standardUpdateFunction ;

	virtual void draw();

	virtual void attemptInteraction(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand);

	virtual void updateData();

private:

	std::shared_ptr<Model>	model = nullptr;
	std::shared_ptr<UI>		UI_ptr = nullptr;

	int			ID_ModelObject;					//the id of the model object to which this hitbox is linked, not used when hitbox is not linked to a specific model object

	std::vector<std::shared_ptr<UI_Object>> UI_objects;		//The UI_objects it will affect directly
	std::vector<JointType>					inputJoints;	//The joints that can interact with this hitbox		

	D2D1_POINT_2F centerCoordActionArea;
	D2D1_POINT_2F originalPos;				//the beginposition of the hitbox

	float		widthActionArea;
	float		heightActionArea;

	HandState	Activehandstate = HandState_Closed;
	JointType	HoverJoint = JointType_WristLeft; //the joint that has activated the button

	bool		hover = false;					//active when joint is hover over hitbox
	bool		keepHoverState = false;			//true when you want to keep the current hoverstate
	bool		activeHand = false;				//active when hand is in activehandstate over hitbox
	bool		activeHandBeforeHover = false;	//true when the activeHandState comes over the hitbox befor it hover non-active above the hitbox
	bool		activeHandOutside = false;		//active when hand was in activehandstate over hitbox and is still in this state outside the hitbox
	bool		functionActive = false;			//active when the criteria for activating its model function are met

	int			InertiaCounter = 0;
	const int	InertiaLimit = 4;

	bool		handLock = false;				//whether this hitbox has locked the JointType specified in parameter HoverJoint
	bool		totalLock = false;
	bool		visible = true;



};

