#ifndef PROJECTGESTURE_H
#define PROJECTGESTURE_H

#include <windows.h>

#include "GestureClass.h"

GestureClass;

class ProjectGesture {
private:
	GestureClass gestureClass;
	double label;
	unsigned int key;

public:
	
	ProjectGesture();
	//here the reference as GestureClass is usefull because the gestureClasses are kept in the model and 
	//When these GestureClasses are changed they are also changed in their projectGestures because they use references to the original object
	// label changed to int, Why did we take double?
	ProjectGesture(const GestureClass & gestureClass, const double label, const unsigned int key = 0x20);
	const GestureClass & getGestureClass() const;
	void setLabel(const double labelToSet);
	const double getLabel() const;
	void setKey(const WORD keyToSet);
	const WORD getKey() const;
};

#endif //PROJECTGESTURE_H