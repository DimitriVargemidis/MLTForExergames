#ifndef PROJECTGESTURE_H
#define PROJECTGESTURE_H

#include "GestureClass.h"


class ProjectGesture {
private:
	GestureClass gestureClass;
	double label;
	unsigned int key;

public:
	ProjectGesture(const GestureClass & gestureClass, const double & label, const unsigned int & key = 0x20);
	
	GestureClass getGestureClass() const;
	void setLabel(const double & labelToSet);
	double getLabel() const;
	void setKey(const unsigned int & keyToSet);
	unsigned int getKey() const;
};

#endif //PROJECTGESTURE_H