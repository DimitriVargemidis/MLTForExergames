#include "UI.h"
//author: Christiaan Vanbergen 

#include "UI_FramesObject.h"

extern D2D_Graphics graphics;
extern std::shared_ptr<UI> UI_global;


UI_FramesObject::UI_FramesObject()
{
}

UI_FramesObject::UI_FramesObject(float Xcenter, float Ycenter, float width, float height, D2D1::ColorF col, std::vector<Frame> framesToDraw):
	UI_Object(Xcenter, Ycenter, width, height, col), frames{framesToDraw}
{
}

UI_FramesObject::~UI_FramesObject()
{
}

void UI_FramesObject::draw()
{
	std::shared_ptr<Abstr_Screen> screen = UI_global->getScreen();
	int startWidth = screen->getWidth();
	int startHeight = screen->getHeight();
	ICoordinateMapper* mapper = screen->getCoordinateMapper();
	int depthWidth = screen->getDepthWidth();
	int depthHeight = screen->getDepthHeight();

	for (int j = frames.size()-1; j >= 0 ; --j)
	{
		std::vector<Joint> joints = frames[j].getJoints();
		std::vector<D2D1_POINT_2F> jointPoints(JointType_Count);

		for (int i = 0; i < joints.size(); ++i)
		{
			jointPoints[i] = graphics.BodyToScreen(joints[i].Position,/* startWidth */startHeight, startHeight, mapper, depthWidth, depthHeight);
		}
		D2D1_POINT_2F center = getCenter();

		graphics.scaleSkeleton(jointPoints, startHeight, startHeight, getWidth()-10, getHeight()-10, center.x, center.y-10);
		graphics.DrawBody(joints, jointPoints, j+1,HandState::HandState_Open, HandState::HandState_Open, 25*((getHeight() - 10)/startHeight));
		}
	;
}
