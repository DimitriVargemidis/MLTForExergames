#pragma once

#include <vector>
#include <Kinect.h>

#include "D2D_Graphics.h"

class BodyBasics;
class Main;
class D2D_Graphics;
class Frame;


// This class is both UI and Controller because the way the events in the UI are processed make them hard to seperate
// see main window creation and callback method MessageRouter

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
	
	void drawFrames(std::vector<Frame> & frames);

	void setMain(std::shared_ptr<Main> m);

	void setModel(std::shared_ptr<Model> m);

	/// <summary>
	/// Draws one bone of a body (joint to joint)
	/// </summary>
	/// <param name="joints">joint data array</param>
	/// <param name="tab">is 0 if current body data needs to be set, is 1 if saved body data needs to be set</param>
	void ShowJointCoordinates(const std::vector<Joint> & joints, int tab);

	/// <summary>
	/// Set the status bar message
	/// </summary>
	/// <param name="szMessage">message to display</param>
	/// <param name="nShowTimeMsec">time in milliseconds for which to ignore future status messages</param>
	/// <param name="bForce">force status update</param>
	bool                    SetStatusMessage(_In_z_ WCHAR* szMessage, DWORD nShowTimeMsec, bool bForce);

	void setPredictedLabel(int label);
	void changeButtonColor(int state);

private:
	
	ICoordinateMapper*      m_pCoordinateMapper;
	INT64                   m_nNextStatusTime;

	HWND*					hWndApp;
	HWND                    m_hWnd;
	D2D_Graphics			graphics;
	std::shared_ptr<Main> 	main;
	std::shared_ptr<Model> 	model;

	long					time; //test parameter to see how long a button is pressed by a human

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