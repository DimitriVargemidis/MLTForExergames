#pragma once

class BodyBasics;
class Main;
class D2D_Graphics;

class UI
{
public:
	UI();
	~UI();

	UI(Main * main);

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
	LRESULT CALLBACK MessageRouter(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	///Check wether there is a peekmessage
	void checkPeekMsg();

	///Check if the quit message has not yet come
	bool checkQuitMsg();

	void SetCoordinateMapper(ICoordinateMapper*      m_pCoordinateMapper);

	bool checkResource();
	
	void drawFrames(std::vector<Frame> frames);

private:
	
	ICoordinateMapper*      m_pCoordinateMapper;

	HWND					hWndApp;
	HWND                    m_hWnd;
	D2D_Graphics			graphics;
	Main*					main;

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

};