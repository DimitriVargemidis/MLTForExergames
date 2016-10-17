#pragma once
class UI
{
public:
	UI();
	~UI();

	/// <summary>
	/// Creates the main window and begins processing
	/// </summary>
	/// <param name="hInstance">handle to the application instance</param>
	/// <param name="nCmdShow">whether to display minimized, maximized, or normally</param>
	int Run(HINSTANCE hInstance, int nCmdShow);

private:


};