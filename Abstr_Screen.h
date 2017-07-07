#pragma once
//author: Christiaan Vanbergen 

#include <vector>
#include <memory>
#include <Kinect.h>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "Gesture.h"

#include "UI_Object.h"
#include "Abstr_UI_Hitbox.h"

//An Abstract class that is used as a template for the creation of other screens
//It contains and creates all hitboxes that draw element on the screen and allow interaction via the kinect interface

//author: Christiaan Vanbergen 
class Abstr_Screen
{
public:
	Abstr_Screen();
	~Abstr_Screen();

	void setModel(std::shared_ptr<Model> m);
	std::shared_ptr<Model> getModel();

	void setUI(std::shared_ptr<UI> ui);
	void setUI(std::shared_ptr<UI> ui, const int width, const int height, ICoordinateMapper*  m_pCoordinateMap, const int cDepthWidth, const int cDepthHeight);
	const std::shared_ptr<UI> & getUI();

	//Most important function that creates all initial hitboxes and UI_objects that are seen on the screen
	virtual void createScreen(int width, int height) = 0;
	//creates the initial scrollbar (defined in RecordScreen class)
	virtual void createScrolbar() = 0;		
	//does a initialises all current hitboxes in the screen with standard settings
	void standardInitHitboxes();

	virtual void setShowRecordScreen(bool record) = 0;
	
	//draws the users puppet in relative or absolute frames
	virtual void drawFrames(std::vector<Frame> & relframes, std::vector<Frame> & absframes);
	//deprecated
	void drawFrame(const Frame & relframes);
	//draws the users scaled puppet in relative frames based on a start width/height and end width/height and can also move the center of the skeleton
	void drawScaledFrame(const Frame & relframes,const float startWidth, const float startHeight, const float endWidth, const float  endHeight, const float absXpos = -1, const float absYpos = -1);

	//draw all the UI_Object in all the Hitboxes belonging to the background
	virtual void drawUI();
	//draw all the UI_Object in all the Hitboxes belonging to the top UI
	virtual void drawTopUI();
	
	//check if any of the UI_hitboxes are interacted with
	virtual void activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand);
	//update all hitboxes
	virtual void updateHitboxes();

	void setRightHandBusy(const bool & busy);	
	const bool & getRightHandBusy();			

	void setLeftHandBusy(const bool & busy);	
	const bool & getLeftHandBusy();

	void setDisplay_UI(const bool abs);
	const bool getDisplay_UI();

	void setDrawAbsCoord(const bool abs);
	const bool getDrawAbsCoord();

	void setWidth(int w);
	int getWidth();

	void setHeight(int h);
	int getHeight();

	void setCoordinateMapper(ICoordinateMapper* mapper);
	ICoordinateMapper* getCoordinateMapper();

	void setDepthWidth(int depthW);
	int getDepthWidth();

	void setDepthHeight(int depthH);
	int getDepthHeight();

	void setInitRecording(bool play);
	bool getInitRecording();

	void setRecordingWidth(float width);
	float getRecordingWidth();

	void setRecordingHeight(float height);
	float getRecordingHeight();

	virtual void setRecordVisualCenter(D2D1_POINT_2F center);

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & get_UI_Hitboxes();

	virtual void setRecordCountDown(int count);

	virtual int		getGestureClassID();
	virtual	void	setGestureClassID(int ID);

	virtual	void	set_UI_GestureID(int ID);
	virtual int		get_UI_GestureID();

	virtual void	autoplayGesture(int ID);
	virtual void	StopPlayGesture();

	virtual std::shared_ptr<UI_HitboxLockScrolBar>	getScrollbar();

	virtual bool	getShowRecordVisual();

	virtual void	setPlayGesture(bool play) ;
	virtual bool	getPlayGesture();

	virtual void						setPlayVisual(std::shared_ptr<UI_Hitbox> play);
	virtual std::shared_ptr<UI_Hitbox>	getPlayVisual();


private:
	std::shared_ptr<Model> 	model;
	std::shared_ptr<UI>		UI_ptr;

	ID2D1Bitmap*			background = nullptr;		//obsolete

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>>		UI_Hitboxes;	//list of UI control elements
																		
	bool rightHandBusy = false;		//boolean to indicate that the righthand is already controlling a button
	bool leftHandBusy = false;		//boolean to indicate that the lefthand is already controlling a button

	bool display_UI = true;			//boolean whether the UI is displayed or not
	bool drawAbsCoord = true;		//boolean whether the users skeleton is drawn with "absolute" coordinates or relative to the skeletons center

	int width;
	int height;
	ICoordinateMapper*  m_pCoordinateMapper;
	int cDepthWidth; 
	int cDepthHeight;

	D2D1_POINT_2F		center;					//center of where the users skeleton is drawn
	bool				initRecording = false;	//whether the recording a gesture is initialised or not

	float recordWidth = 400;
	float recordHeight= 400;
	
};

