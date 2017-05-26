#pragma once

#include <iostream>
#include <chrono>
typedef std::chrono::high_resolution_clock Clock;

#include "stdafx.h"

#include "Abstr_UI_HitboxSlideButton.h"

class UI;

class UI_HitboxScrolBar:public Abstr_UI_HitboxSlideButton
{
public:
	UI_HitboxScrolBar();
	UI_HitboxScrolBar(	float Xcenter, float Ycenter, float width, float height, float left, float right, float up, float down, float activation = 0.5f,
						std::function<void(int, int, std::shared_ptr<Model>, std::shared_ptr<UI>)> callback = UI_CallbackFunctions::testCallback, int ID_Model = -1);
	~UI_HitboxScrolBar();

	virtual void setModel(std::shared_ptr<Model> m) override;

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
	int		getMoveUpCounter();

	void	setMoveDownCounter(int count);
	int		getMoveDownCounter();

	float	getMoveLimit();

	void	setHeight_UI_element(float height);
	float	getHeight_UI_element();

	void	setWidth_UI_element(float width);
	float	getWidth_UI_element();

	void							setActionText(std::shared_ptr<UI_TextObject> textObject);
	std::shared_ptr<UI_TextObject>	getActionText();

	void							setActionIndicator(std::shared_ptr<UI_Object> actionIndic);
	std::shared_ptr<UI_Object>		getActionIndicator() override;

	void							setTopFiller(std::shared_ptr<UI_Object> topFill);
	std::shared_ptr<UI_Object>		getTopFiller();

	void							setBottomFiller(std::shared_ptr<UI_Object> bottomFill);
	std::shared_ptr<UI_Object>		getBottomFiller();

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> & get_UI_Elements();
	void clear_UI_elements() override;

	virtual void draw() override;

	virtual void attemptInteraction(D2D1_POINT_2F jointPoint, JointType type, HandState leftHand, HandState rightHand) override;

	virtual void updateData() override;



private:

	std::vector<std::shared_ptr<Abstr_UI_Hitbox>> UI_elements;

	//hardcoded for now
	float width_UI_Element = 100;	//width of the UI objects within the scrollbar
	float height_UI_Element = 100;	//height of the UI objects within the scrollbar
	float XoffsetDisplay = 0;		//offset van de 

	//NOT IMPLEMENTED
	//Idea for scroll delay based on the frames in which the hand moves continuesly in the same direction 
	float delayDownCounter = 0;		//The counter to decide when the movement downwards is initiated
	float delayUpCounter = 0;		//The counter to decide when the movement upwards is initiated
	float delayLimit = 5;

	// The counters for the delay based on the distance the user moves within the scrollbar
	float delayMoveDownCounter = 0;		//The counter to decide when the movement downwards is initiated
	float delayMoveUpCounter = 0;		//The counter to decide when the movement upwards is initiated
	float delayMoveLimit;

	//NOT IMPLEMENTED!!
	//idea of building in a delay time before a opposite scroll is performe 
	std::chrono::steady_clock::time_point TimeLastMoveDown = Clock::now();			//The time when the last move down was, to set delay between move up and down
	std::chrono::steady_clock::time_point TimeLastMoveUp= Clock::now();		//The time when the last move up was, to set delay between move up and down
	long long delayTimeAfterMoveLimit = 500;
	
	
	std::shared_ptr<UI_Object> topFiller = nullptr;
	std::shared_ptr<UI_Object>  bottomFiller = nullptr;
	std::shared_ptr<UI_Object> actionIndicator = nullptr;
	std::shared_ptr<UI_TextObject> actionText = nullptr;


};

