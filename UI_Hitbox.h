#pragma once

#include <d2d1.h>
#include <vector>
#include <memory>

#include "UI_Object.h"

class Model;

class UI_Hitbox
{
public:
	UI_Hitbox();
	~UI_Hitbox();

	void setModel(std::shared_ptr<Model> m);
	
	void add_UI_Object(std::shared_ptr<UI_Object> object);
	void remove_UI_Object(std::shared_ptr<UI_Object> object); //TO DO define function
	std::vector<std::shared_ptr<UI_Object>> get_UI_Objects();

	void addInputJoint(JointType joint);
	std::vector<JointType> getInputJoints();

	bool checkCoordInside(D2D1_POINT_2F coord);
	bool checkInputJointType(JointType type);

	void Activate();

private:

	std::shared_ptr<Model> model;

	std::vector<std::shared_ptr<UI_Object>> UI_objects;	//The UI_objects it will affect directly
	std::vector<JointType> inputJoints;					//The joints that can interact with this hitbox		

	D2D1_POINT_2F centerCoordActionArea;
	int	widthActionArea;
	int heightActionArea;
};

