#ifndef PROJECTGESTURE_H
#define PROJECTGESTURE_H

#include <windows.h>

#include "Action.h"
#include "GestureClass.h"

GestureClass;

class ProjectGesture {
private:
	GestureClass & gestureClass;
	double label;
	std::vector<Action> actions;
	int projectGestureID = 0;

public:
	
	ProjectGesture();
	//here the reference as GestureClass is useful because the gestureClasses are kept in the model and 
	//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
	// label changed to int, Why did we take double?
	ProjectGesture(GestureClass & g, const double label, const WORD key = 0, bool holding = false);
	ProjectGesture(GestureClass & g);

	GestureClass getGestureClass();
	void setLabel(const double labelToSet);
	const double getLabel() const;
	void setActions(std::vector<Action> a);
	std::vector<Action> & getActions();
	void addAction(WORD keycode, bool hold);
	//void setKey(const WORD keyToSet);
	const WORD getKey() const;

	void Activate();
	void Deactivate();

	int getProjectGestureID();
	void setProjectGestureID(int ID);
};

static int appProjectGestureID = 0;
static int getAppProjectGestureID();

#endif //PROJECTGESTURE_H