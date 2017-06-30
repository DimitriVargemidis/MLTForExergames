#pragma once
#include <memory>
#include <windows.h>

#include "stdafx.h"


struct Action
{
	//The button that needs to be pressed. Check Microsoft's datasheets for a table of keycodes.
	WORD keycode;
	//True if the button must be pressed and hold. False if the button must be pressed and released.
	bool hold;
	//True if the button is being pressed (virtually).
	bool active;
};