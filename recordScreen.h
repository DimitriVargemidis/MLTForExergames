#pragma once

#include "Abstr_Screen.h"
class recordScreen :
	public Abstr_Screen
{
public:
	recordScreen();
	recordScreen(int ID);
	~recordScreen();

	virtual void	createScreen(int width, int height) override;
	virtual void	createScrolbar() override;

	virtual void	setShowRecordScreen(bool record) override;

	virtual void	drawUI() override;
	virtual void	drawTopUI() override;

	virtual void	setRecordCountDown(int count) override;

	virtual	void	setGestureClassID(int ID)override;
	virtual int		getGestureClassID() override;

	virtual void	autoplayGesture(int ID) override;
	virtual void	StopPlayGesture() override;
	



private:
	bool		showRecordVisual;
	UI_Hitbox	recordVisual;
	UI_Hitbox	recordCountDown;

	int			gestureClassID;

	std::shared_ptr<Gesture>	AutoPlayGesture;
	int			playGestureIndex;
	bool		playGesture = false;

};

