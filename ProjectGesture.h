#ifndef PROJECTGESTURE_H
#define PROJECTGESTURE_H

#include <windows.h>

#include "Action.h"
#include "GestureClass.h"

GestureClass;

class ProjectGesture {
private:
	GestureClass gestureClass; //? keep constant?
	double label;
	//WORD key;
	std::vector<Action> actions;

public:
	
	ProjectGesture();
	//here the reference as GestureClass is usefull because the gestureClasses are kept in the model and 
	//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
	// label changed to int, Why did we take double?
	ProjectGesture( GestureClass & g, const double label, const WORD key = 0x20);
	//ProjectGesture(const GestureClass & gestureClass, const double label, const WORD key = 0x20);
	GestureClass getGestureClass();
	void setLabel(const double labelToSet);
	const double getLabel() const;
	void setActions(std::vector<Action> a);
	std::vector<Action> & getActions();
	//void setKey(const WORD keyToSet);
	const WORD getKey() const;
};

#endif //PROJECTGESTURE_H