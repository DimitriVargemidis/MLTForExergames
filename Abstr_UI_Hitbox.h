#pragma once
//author: Christiaan Vanbergen 

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

// A Abstract class that is the basis for all other hitbox classes

//author: Christiaan Vanbergen 
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
	void remove_UI_Object(std::shared_ptr<UI_Object> object); //not used
	std::vector<std::shared_ptr<UI_Object>> & get_UI_Objects();

	void addInputJoint(JointType joint);
	std::vector<JointType> getInputJoints();

	//checks if the given coordinate is within the border of the hitbox rectangle
	bool checkCoordInside(D2D1_POINT_2F coord);
	//checks if the given jointType is allowed to interact with the hitbox
	bool checkInputJointType(JointType type);

	//mayor function checking which state the hitbox will go into and active actions accordingly
	//called by attemptInteraction function
	void setHover(const bool & a, const JointType & joint, const HandState & hand, const D2D1_POINT_2F & jointPoint);
	void setHover(bool a);
	bool getHover();

	void setKeepHoverState(bool a);
	bool getKeepHoverState();

	void setActiveHand(const bool & hand);
	bool getActiveHand();

	void setActiveHandBeforeHover(bool a);
	bool getActiveHandBeforeHover();

	void setActiveHandOutside(const bool & hand);
	bool getActiveHandOutside();

	void setHoverJoint(JointType joint);
	JointType getHoverJoint();

	//obsolete (all 3)
	virtual void HoverOnAction();
	virtual void HoverOffAction();
	virtual void HoverHoldAction();
	//obsolete (all 3)
	virtual void ActiveHandOnAction();
	virtual void ActiveHandOffAction();
	virtual void ActiveHandHoldAction();

	//Called whenever the hitbox changes state, uses a switch case to do the right action in the right situation
	//This method is overriden by every child of the abstract class
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

	//Activate and deactivate the function of the hitbox.
	virtual void activateFunction() = 0;
	virtual void deactivateFunction();

	//Set and get the callback function that is used when the hitbox is updated
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

	void setFading(bool fade);
	bool getFading();

	float getFadeProgress();

	virtual void setLastPoint(D2D1_POINT_2F pos) = 0;
	virtual D2D1_POINT_2F getLastPoint() = 0;

	//add and clear the selectable UI_elements in the scrollbar
	virtual void add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox);
	virtual void clear_UI_elements();

	virtual std::shared_ptr<UI_Object>		getActionIndicator();

	//move the hitbox current center or original center with increment "move" in the Y direction
	void moveY(float move);
	void moveYoriginalPos(float move);

	//move the hitbox current center or original center to an absolute position "pos" on the screen in the Y direction
	void moveAbsY(float pos);
	void moveAbsYoriginalPos(float pos);

	virtual void	setXoffset(float offset);
	virtual float	getXoffset();				

	//draw the UI_Objects that are associated to this hitbox
	virtual void draw();
	//called every frame and is the main method that 
	virtual void attemptInteraction(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand);
	//call the update callbackfunction
	virtual void updateData();

	//the callbackfunction called when the hitbox is activated
	std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)>	activateFunctionCallback = UI_CallbackFunctions::testCallback;
	//the callbackfunction called when the hitbox needs to update its information
	std::function<void(Abstr_UI_Hitbox*)>	updateFunctionCallback = UI_Update_CallbackFunctions::standardUpdateFunction ;	

private:
	//pointer to the Model object and the UI object
	std::shared_ptr<Model>	model = nullptr;
	std::shared_ptr<UI>		UI_ptr = nullptr;


	int			ID_ModelObject;								//the id of the model object to which this hitbox is linked, not used when hitbox is not linked to a specific model object, -1 is no link to a model object

	std::vector<std::shared_ptr<UI_Object>> UI_objects;		//The UI_objects it will affect directly
	std::vector<JointType>					inputJoints;	//The joints that can interact with this hitbox		

	D2D1_POINT_2F centerCoordActionArea;					//current center position of the hitbox
	D2D1_POINT_2F originalPos;								//the begin center position of the hitbox

	float		widthActionArea;			
	float		heightActionArea;

	HandState	Activehandstate = HandState_Closed;	//the state in which the hand has to be to activate the hitbox
	JointType	HoverJoint = JointType_WristLeft;	//the joint that has activated the button

	bool		hover = false;						//active when a valid joint is hovering over hitbox
	bool		keepHoverState = false;				//true when you want to keep the current hoverstate
	bool		activeHand = false;					//active when the hand is in activehandstate over hitbox
	bool		activeHandBeforeHover = false;		//true when the activeHandState comes over the hitbox before it hovers non-active above the hitbox
	bool		activeHandOutside = false;			//active when hand was in activehandstate over hitbox and is still in this state outside the hitbox
	bool		functionActive = false;				//active when the criteria for activating its model function are met

	int			InertiaCounter = 0;					//counter to define some inertia in the deactivation of the interaction with a hitbox	
	const int	InertiaLimit = 4;					//the limit of the InertiaCounter

	bool		handLock = false;					//whether this hitbox has locked the JointType specified in parameter HoverJoint
	bool		totalLock = false;					//whether the hitbox is locked to all kinds of interactions
	bool		visible = true;						//whether the UI_Objects contained in this hitbox are visible or not
	bool		fading = false;						//whether the UI_Objects contained in this hitbox are 

	int			fadeCounter = 0;					//counter for the fading of the UI_Objects contained in this hitbox
	const int	fadeLimit = 30;						//the limit of the fadecounter, setting when all objects have been faded



};

