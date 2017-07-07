#pragma once
//namespace with generic code to  process key events in the program

namespace Keypress
{
	int pressKey(const WORD keycode);
	int keyDown(const WORD keycode);
	int keyUp(const WORD keycode);
}