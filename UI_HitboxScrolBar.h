#pragma once

#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"

#include "Abstr_UI_HitboxSlideButton.h"
class UI_HitboxScrolBar:public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxScrolBar();
	UI_HitboxScrolBar(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation = 0.5f,
						std::function<void(int)> callback = [](int) { printf("no particular action defined = wrong constructor used"); }, int ID_Model = -1);
	~UI_HitboxScrolBar();

	virtual void moveLeftAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveRightAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveDownAction(D2D1_POINT_2F ref, float move) override;
	virtual void moveUpAction(D2D1_POINT_2F ref, float move) override;

	void action(ActionTrigger act, const D2D1_POINT_2F & coord) override;

	virtual void activateFunction() override ;

	virtual bool isRightHandActive() override;
	virtual bool isLeftHandActive() override;

	void	add_UI_Element(std::shared_ptr<Abstr_UI_Hitbox> hitbox);

	void	setMoveUpCounter(int count);
	int		 getMoveUpCounter();

	void	setMoveDownCounter(int count);
	int		getMoveDownCounter();

	float	getMoveLimit();

	void	setHeight_UI_element(float height);
	float	getHeight_UI_element();

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> get_UI_Elements();


private:

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> UI_elements;

	//hardcoded for now
	float width_UI_Element = 100;	//width of the UI objects within the scrollbar
	float height_UI_Element = 100;	//height of the UI objects within the scrollbar
	float XoffsetDisplay = 0;		//offset van de 

	float delayDownCounter = 0;		//The counter to decide when the movement downwards is initiated
	float delayUpCounter = 0;		//The counter to decide when the movement upwards is initiated
	float delayLimit = 5;

	float delayMoveDownCounter = 0;		//The counter to decide when the movement downwards is initiated
	float delayMoveUpCounter = 0;		//The counter to decide when the movement upwards is initiated
	float delayMoveLimit;

	std::chrono::steady_clock::time_point TimeLastMoveDown = Clock::now();			//The time when the last move down was, to set delay between move up and down
	std::chrono::steady_clock::time_point TimeLastMoveUp= Clock::now();		//The time when the last move up was, to set delay between move up and down
	long long delayTimeAfterMoveLimit = 500;


};

