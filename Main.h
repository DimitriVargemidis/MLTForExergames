#pragma once
class Main
{
public:
	Main();
	~Main();

	void mainCanInitializeKinectSensor();

	void activatePrediction();

private:

	// Current Kinect
	IKinectSensor*          m_pKinectSensor;
	ICoordinateMapper*      m_pCoordinateMapper;

	// Body reader
	IBodyFrameReader*       m_pBodyFrameReader;

	/// <summary>
	/// Initializes the default Kinect sensor
	/// </summary>
	/// <returns>indicates success or failure</returns>
	HRESULT InitializeDefaultSensor();


};

