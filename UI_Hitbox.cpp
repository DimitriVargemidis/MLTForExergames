
#include "Model.h"

#include "UI_Hitbox.h"

UI_Hitbox::UI_Hitbox()
{
	centerCoordActionArea.x = 100;
	centerCoordActionArea.y = 200;
	widthActionArea = 100;
	heightActionArea = 100;

}


UI_Hitbox::~UI_Hitbox()
{
}

//check whether the given coordinate is within the action area of the ui object
bool UI_Hitbox::checkCoordInside(D2D1_POINT_2F coord)
{
	if (coord.x < centerCoordActionArea.x + widthActionArea / 2 &&
		coord.x > centerCoordActionArea.x - widthActionArea / 2 &&
		coord.y < centerCoordActionArea.y + heightActionArea / 2 &&
		coord.y > centerCoordActionArea.y - heightActionArea / 2)
	{
		return true;
	}

	return false;
}

bool UI_Hitbox::checkInputJointType(JointType type)
{
	for (int i = 0; i < inputJoints.size(); ++i)
	{
		if (inputJoints[i] == type)
		{
			return true;
		}
	}
	return false;
}

void UI_Hitbox::Activate()
{
	try
	{
		UI_objects[0]->changeColor(D2D1::ColorF::Red);
	}
	catch (const std::out_of_range& oor)
	{
		printf("there is no UI_object linked to this UI_hitbox");
	}
}



void UI_Hitbox::setModel(std::shared_ptr<Model> m)
{
	model = m;
}

void UI_Hitbox::add_UI_Object(std::shared_ptr<UI_Object> object)
{
	UI_objects.push_back(object);
}

void UI_Hitbox::remove_UI_Object(std::shared_ptr<UI_Object> object)
{
	//TO DO define this function
}

std::vector<std::shared_ptr<UI_Object>> UI_Hitbox::get_UI_Objects()
{
	return UI_objects;
}

void UI_Hitbox::addInputJoint(JointType joint)
{
	inputJoints.push_back(joint);
}

std::vector<JointType> UI_Hitbox::getInputJoints()
{
	return inputJoints;
}
