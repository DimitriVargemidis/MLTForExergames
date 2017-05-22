#pragma once

#include <vector>
#include <Kinect.h>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "UI_Object.h"
#include "UI_TextObject.h"
#include "UI_FramesObject.h"
#include "UI_BitmapObject.h"
#include "UI_CallbackFunctions.h"
#include "UI_Update_CallbackFunctions.h"
#include "Abstr_UI_Hitbox.h"
#include "UI_Hitbox.h"
#include "UI_HitboxSlideButton.h"
#include "UI_HitboxHoverSlideButton.h"
#include "UI_HitboxScrolBar.h"
#include "UI_HitboxLockScrolBar.h"
#include "Abstr_Screen.h"
#include "recordScreen.h"

#include "D2D_Graphics.h"

//class BodyBasics;
class Main;
//class D2D_Graphics;
class Frame;


// This class is both UI and Controller because the way the events in the UI are processed make them hard to seperate
// see main window creation and callback method MessageRouter
/*
#ifndef GRAPHICS 
#define GRAPHICS
	D2D_Graphics			graphics;
#endif
	*/

class UI
{
	static const int        cDepthWidth = 512;
	static const int        cDepthHeight = 424;

public:
	UI();
	~UI();

	//UI(Main * main);

	/// <summary>
	/// Creates the main window and begins processing
	/// </summary>
	/// <param name="hInstance">handle to the application instance</param>
	/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
	int Run(HINSTANCE hInstance, int nCmdShow);

	/// <summary>
	/// Handles window messages, passes most to the class instance to handle
	/// </summary>
	/// <param name="hWnd">window message is for</param>
	/// <param name="uMsg">message</param>
	/// <param name="wParam">message data</param>
	/// <param name="lParam">additional message data</param>
	/// <returns>result of message processing</returns>
	static LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	///Check wether there is a peekmessage
	void checkPeekMsg();

	///Check if the quit message has not yet come
	bool checkQuitMsg();

	void SetCoordinateMapper(ICoordinateMapper*      m_pCoordinateMapper);

	bool checkResource();
	
	//both absolute and relative frames are given to the ui the ui will decide which one will be drawn where
	void drawFrames(std::vector<Frame> & relframes, std::vector<Frame> & absframes); //duplicate
	void drawHandState(HandState handState, const D2D1_POINT_2F& handPosition);

	void updateHitboxes();  //duplicate

	void changeButtonColor(int state);

	void setMain(std::shared_ptr<Main> m);
	void setModel(std::shared_ptr<Model> m); 

	void createScreen(); //duplicate

	void scaleSkeleton(std::vector<D2D1_POINT_2F> & jointPoints, float multiplier);//move


	/// <summary>
	/// Draws one bone of a body (joint to joint)
	/// </summary>
	/// <param name="joints">joint data array</param>
	/// <param name="tab">is 0 if current body data needs to be set, is 1 if saved body data needs to be set</param>
//	void ShowJointCoordinates(const std::vector<Joint> & joints, int tab);

	/// <summary>
	/// Set the status bar message
	/// </summary>
	/// <param name="szMessage">message to display</param>
	/// <param name="nShowTimeMsec">time in milliseconds for which to ignore future status messages</param>
	/// <param name="bForce">force status update</param>
	bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);

	void setPredictedLabel(int label);
	

	void setRightHandBusy(const bool & busy);	
	const bool & getRightHandBusy();			

	void setLeftHandBusy(const bool & busy);	
	const bool & getLeftHandBusy();	

	void setScreen(const std::shared_ptr<Abstr_Screen> & scr);
	const std::shared_ptr<Abstr_Screen> & getScreen();



private:
	
	ICoordinateMapper*      m_pCoordinateMapper;
	INT64                   m_nNextStatusTime;

	HWND*					hWndApp;
	//HWND                    m_hWnd;
	//D2D_Graphics			graphics;
	std::shared_ptr<Main> 	main;
	std::shared_ptr<Model> 	model;

	//to Abstr_Screen
	std::vector<std::shared_ptr<Abstr_UI_Hitbox>>		UI_Hitboxes;	//list of UI control elements
	//std::shared_ptr<UI_HitboxLockScrolBar> scrollbarHitbox = nullptr;	//A seperate pointer to a scrollbar for easy access
	//std::vector<std::shared_ptr<UI_Object>> 	UI_Objects;		//UI elements to be drawn

	std::shared_ptr<Abstr_Screen>	Screen;		//UI elements to be drawn

	bool					waitForKey;		//parameter is true when the interface is waiting for a key
	bool					holdPress = false;		//parameter is true when the next added key will (during prediction) be held aslong as no other gesture is recognized
	bool					drawAbsCoord = true;

	long					time; //test parameter to see how long a button is pressed by a human

	//variables for the drawing of the frame
	RECT rct;
	int width;
	int height;

	//to Abstr_Screen
	//simple way of making sure 1 hand does not activate multiple drag buttons
	//Only for right and left hand avaiable this way
	bool rightHandBusy = false;		//boolean to indicate that the righthand is already controlling a button
	bool leftHandBusy  = false;		//boolean to indicate that the lefthand is already controlling a button



	MSG       msg;
	WNDCLASS  wc;

	/// <summary>
	/// Handle windows messages for the class instance
	/// </summary>
	/// <param name="hWnd">window message is for</param>
	/// <param name="uMsg">message</param>
	/// <param name="wParam">message data</param>
	/// <param name="lParam">additional message data</param>
	/// <returns>result of message processing</returns>
	LRESULT CALLBACK UI::DlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void processKeyInput(MSG msg);


};