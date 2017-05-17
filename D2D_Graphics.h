//This class is based on the code from the Microsoft library examples called BodyBasics-D2D
//Code is adapted and expanded to meet requirements
#pragma once

#pragma comment(lib, "dwrite")

#include <Kinect.h>
#include <memory>
#include <vector>
#include <dwrite.h>
#include "resource.h"

class D2D_Graphics
{
public:
	const float c_JointThickness = 12.0f;
	const float c_BoneThickness = 25.0f;
	const float c_InferredBoneThickness = 8.0f;
	const float c_HandSize = 20.0f;
	
	D2D_Graphics();
	~D2D_Graphics();
	;
	ID2D1HwndRenderTarget* GetRenderTarget();

	void SetRenderTarget(ID2D1HwndRenderTarget* renderTarget);

	//Ensure necessary Direct2d resources are created
	//Returns: S_OK if successful, otherwise an error code
	HRESULT EnsureDirect2DResources(HWND m_hWnd);

	//Dispose Direct2d resources 
	void DiscardDirect2DResources();

	//Initialize the D2D1 factory
	void InitD2D();

	//Converts a body point to screen space
	//"bodyPoint": body point to tranform
	//"width": width (in pixels) of output buffer
	//"height": height (in pixels) of output buffer
	//Returns: point in screen-space
	D2D1_POINT_2F BodyToScreen(const CameraSpacePoint & bodyPoint, int width, int height, ICoordinateMapper * m_pCoordinateMapper, int cDepthWidth, int cDepthHeight);

	//Draws a body 
	//"pJoints": joint data
	//"pJointPoints": joint positions converted to screen space
	void DrawBody(const std::vector<Joint> pJoints, const std::vector<D2D1_POINT_2F> pJointPoints, const int colorID = 0);


	//Draws a hand symbol if the hand is tracked: red circle = closed, green circle = opened; blue circle = lasso
	//"handState": state of the hand
	//"handPosition": position of the hand
	void drawHand(const D2D1_POINT_2F& handPosition);

	//Draws one bone of a body (joint to joint)
	//"pJoints": joint data
	//"pJointPoints": joint positions converted to screen space
	//"pJointPoints": joint positions converted to screen space
	//"joint0": one joint of the bone to draw
	//"joint1": other joint of the bone to draw
	void DrawBone(const std::vector<Joint> pJoints, const std::vector<D2D1_POINT_2F> pJointPoints, JointType joint0, JointType joint1, float boneThickness);

	//Release D2Dfactory
	void CleanD2D();

	void changeColor(int colorID);
	void changeButtonColor(int state);

	void					drawRectangle(D2D1_POINT_2F center, float width, float height, D2D1::ColorF color);
	void					drawText(std::wstring text, D2D1_POINT_2F center, float width = 100, float height = 50, D2D1::ColorF color = D2D1::ColorF::AliceBlue, float fontSize = 50, DWRITE_TEXT_ALIGNMENT alignment = DWRITE_TEXT_ALIGNMENT_CENTER);

	ID2D1Bitmap	*			createBitmap(std::wstring filename, int srcWidth, int srcHeight);
	void					drawBitmap(RGBQUAD * image, int srcWidth, int srcHeight, D2D1_POINT_2F center, float width, float height);
	void					drawBitmap(ID2D1Bitmap * bitmap, D2D1_POINT_2F center, float width, float height);

	void					scaleSkeleton(std::vector<D2D1_POINT_2F> & jointPoints, float multiplier);
	void					scaleSkeleton(std::vector<D2D1_POINT_2F> & jointPoints, const float startWidth, const float startHeight, const float endWidth, const float endHeight, const float absXpos = -1, const float absYpos = -1);


private:

	void drawTorso(const std::vector<D2D1_POINT_2F> pJointPoints);
	void drawHead(const std::vector<D2D1_POINT_2F> pJointPoints);

	ID2D1Factory*           m_pD2DFactory = NULL;

	// Direct2D Text resources
	IDWriteTextFormat*		m_pTextFormat = NULL;
	IDWriteFactory*			m_pDWriteFactory = NULL;

	//Body/hand drawing
	ID2D1HwndRenderTarget*  m_pRenderTarget = NULL;
	ID2D1SolidColorBrush*   m_pBrushJointTracked = NULL;
	ID2D1SolidColorBrush*   m_pBrushJointInferred = NULL;
	ID2D1SolidColorBrush*   m_pBrushBoneTracked = NULL;
	ID2D1SolidColorBrush*   m_pBrushBoneInferred = NULL;
	ID2D1SolidColorBrush*   m_pBrushHandClosed = NULL;
	ID2D1SolidColorBrush*   m_pBrushHandOpen = NULL;
	ID2D1SolidColorBrush*   m_pBrushHandLasso = NULL;

	//Button highlight code
	ID2D1SolidColorBrush*   m_pBrushUpButton = NULL;
	ID2D1SolidColorBrush*   m_pBrushDownButton = NULL;
	ID2D1SolidColorBrush*   m_pBrushRightButton = NULL;
	ID2D1SolidColorBrush*   m_pBrushLeftButton = NULL;
	ID2D1SolidColorBrush*   m_pBrushAButton = NULL;

	ID2D1SolidColorBrush*   m_pBodyBrush = NULL;
	
	ID2D1LinearGradientBrush*		m_pBodyGradient = NULL;
	
	ID2D1PathGeometry*		m_pTorso = NULL;
	D2D_COLOR_F				m_pBodyColor;
};

