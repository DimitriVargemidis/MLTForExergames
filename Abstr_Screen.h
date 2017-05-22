#pragma once
#include <vector>
#include <memory>
#include <Kinect.h>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "Gesture.h"

#include "UI_Object.h"
#include "Abstr_UI_Hitbox.h"

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

	virtual void createScreen(int width, int height) = 0;
	virtual void createScrolbar() = 0;		

	void standardInitHitboxes();

	virtual void setShowRecordScreen(bool record) = 0;
	

	virtual void drawFrames(std::vector<Frame> & relframes, std::vector<Frame> & absframes);
	void drawFrame(const Frame & relframes);
	void drawScaledFrame(const Frame & relframes,const float startWidth, const float startHeight, const float endWidth, const float  endHeight, const float absXpos = -1, const float absYpos = -1);

	virtual void drawUI();
	virtual void drawTopUI();
	//void scaleSkeleton(std::vector<D2D1_POINT_2F> & jointPoints, float multiplier);
	//void scaleSkeleton(std::vector<D2D1_POINT_2F> & jointPoints, const float startWidth, const float startHeight, const float endWidth, const float endHeight, const float absXpos = -1, const float absYpos = -1);

	virtual void activateHitboxes(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand);
	virtual void updateHitboxes();

	void setRightHandBusy(const bool & busy);	
	const bool & getRightHandBusy();			

	void setLeftHandBusy(const bool & busy);	
	const bool & getLeftHandBusy();

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

	ID2D1Bitmap*			background = nullptr;

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>>		UI_Hitboxes;	//list of UI control elements
																		
	bool rightHandBusy = false;		//boolean to indicate that the righthand is already controlling a button
	bool leftHandBusy = false;		//boolean to indicate that the lefthand is already controlling a button

	bool drawAbsCoord = true;

	int width;
	int height;
	ICoordinateMapper*  m_pCoordinateMapper;
	int cDepthWidth; 
	int cDepthHeight;

	D2D1_POINT_2F		center;
	bool				initRecording = false;

	float recordWidth = 400;
	float recordHeight= 400;
	
};

