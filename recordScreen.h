#pragma once

#include "Abstr_Screen.h"
class recordScreen :
	public Abstr_Screen
{
public:
	recordScreen();
	recordScreen(int ID);
	~recordScreen();

	virtual void drawFrames(std::vector<Frame> & relframes, std::vector<Frame> & absframes) override;

	virtual void activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand)override;
	virtual void updateHitboxes() override;

	virtual void	createScreen(int width, int height) override;
	virtual void	createScrolbar() override;

	virtual void	setShowRecordScreen(bool record) override;

	virtual void	drawUI() override;
	virtual void	drawTopUI() override;

	virtual void	setRecordCountDown(int count) override;

	virtual	void	setGestureClassID(int ID)override;
	virtual int		getGestureClassID() override;

	virtual void	autoplayGesture(int ID) override;
	virtual void	StopPlayGesture() override;
	
	virtual void	setRecordVisualCenter(D2D1_POINT_2F center) override;

	virtual bool	getShowRecordVisual() override;

	virtual void	setPlayGesture(bool play) override;
	virtual bool	getPlayGesture() override;

	virtual void						setPlayVisual(std::shared_ptr<UI_Hitbox> play) override;
	virtual std::shared_ptr<UI_Hitbox>	getPlayVisual() override;


	virtual std::shared_ptr<UI_HitboxLockScrolBar>	getScrollbar() override;


private:

	int			gestureClassID;						//The ID of the gestureClass that is being edited on this screen

	//scrollbar hitbox
	std::shared_ptr<UI_HitboxLockScrolBar>	scrollbar = nullptr;

	//objects belonging to the Recording action
	bool		showRecordVisual;					//boolean whether or not the recordingvisual and recordCountDown are shown
	UI_Hitbox	recordVisual;						//visual for the box in which the recording takes place
	UI_Hitbox	recordCountDown;					//visual for the count down to starting the recording

	//objects belonging to the play action: replaying selected recordings
	std::shared_ptr<Gesture>		AutoPlayGesture;		//the Gesture that is being replayed in the playVisual
	int								playGestureIndex;		//the index of the frame in AutoPlayGesture that is displayed at this point
	
	bool							playGesture = false;	//boolean whether or not the recording should be displayed
	std::shared_ptr<UI_Hitbox>		playVisual;				//the image of the box in which the recording is played
	float							playWidth	= 400;
	float							playHeight  = 400;
	D2D1_POINT_2F					playCenterpoint = D2D1::Point2F(300, 300);
	
	std::chrono::time_point<std::chrono::steady_clock> playGestureTimer;	//timer decide when to restart the replaying of the same recording

};

