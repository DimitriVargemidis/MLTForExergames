#pragma once

//Abstract class --> to do: add 'virtual' when children of this class are implemented
class Gesture{
private:
	double label;
public:
	double getLabel();
	void setLabel();
	
};