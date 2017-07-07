//
// keystroke.c - Pauses, then simulates a key press
// and release of the "A" key.
//
// Written by Ted Burke - last updated 17-4-2012
//
// To compile with MinGW:
//
//      gcc -o keystroke.exe keystroke.c
//
// To run the program:
//
//      keystroke.exe
//
// ...then switch to e.g. a Notepad window and wait
// 5 seconds for the A key to be magically pressed.
//

// Because the SendInput function is only supported in
// Windows 2000 and later, WINVER needs to be set as
// follows so that SendInput gets defined when windows.h
// is included below.
#define WINVER 0x0500
#include <windows.h>
#include "Keypress.h"

int Keypress::pressKey(const WORD keycode)
{
	if (keycode == 0)
	{
		return 1;
	}
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the key with code "keycode"
	ip.ki.wVk = keycode; 
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	Sleep(50);

	// Release the key with code "keycode"
	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));

	// Exit normally
	return 0;
}

int Keypress::keyDown(const WORD keycode)
{
	if (keycode == 0)
	{
		return 1;
	}
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Press the key with code "keycode"
	ip.ki.wVk = keycode; 
	ip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &ip, sizeof(INPUT));

	// Exit normally
	return 0;
}

int Keypress::keyUp(const WORD keycode)
{
	if (keycode == 0)
	{
		return 1;
	}
	// This structure will be used to create the keyboard
	// input event.
	INPUT ip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	// Release the key with code "keycode"
	ip.ki.wVk = keycode; 

	ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &ip, sizeof(INPUT));

	// Exit normally
	return 0;
}
