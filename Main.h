#pragma once

#include <vector>


class Model;
class Frame;

class Main
{
public:
	Main();
	~Main();

	void mainCanInitializeKinectSensor();

	void setModelRefresh(boolean refresh);
	boolean getModelRefresh();

	void setModelPredict(boolean refresh);
	boolean getModelPredict();

	/// <summary>
	/// Main processing function
	/// </summary>
	void Main::Update();

	int Run(HINSTANCE hInstance, int nCmdShow);

	void drawFrames(std::vector<Frame> frames);

private:

	Model model;
	UI ui;

	boolean running;

	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	//ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>indicates success or failure</returns>
	HRESULT InitializeDefaultSensor();



};

