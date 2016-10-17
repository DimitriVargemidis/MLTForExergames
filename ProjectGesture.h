#ifndef PROJECTGESTURE_H
#define PROJECTGESTURE_H

#include "GestureClass.h"


class ProjectGesture {
private:
	GestureClass gestureClass;
	double label;
	unsigned int key;

public:
	ProjectGesture(const GestureClass & gestureClass, const double label, const unsigned int key = 0x20);
	const GestureClass & getGestureClass() const;
	void setLabel(const double labelToSet);
	const double getLabel() const;
	void setKey(const unsigned int keyToSet);
	const unsigned int getKey() const;
};

#endif //PROJECTGESTURE_H