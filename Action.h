#pragma once
#include <memory>
#include <windows.h>

#include "stdafx.h"

struct Action
{
	WORD keycode;
	bool hold;
};