#pragma once

//----------------------------------------------------------------------------
//{ Prototypes
//----------------------------------------------------------------------------

	void waitUntilSpaceButtonIsPressed();

//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//{ Defines
//----------------------------------------------------------------------------

	#define WAIT waitUntilSpaceButtonIsPressed();

//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//{ Includes
//----------------------------------------------------------------------------

#include <stdio.h>

#include "headers/TXLib.h"

#include "headers/Matrix.h"
#include "headers/Vector.h"

#include "headers/Triangle.h"

#include "headers/Model.h"
#include "headers/Rendering.h"

//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//{ Additional function, used in defines:
//----------------------------------------------------------------------------

	void waitUntilSpaceButtonIsPressed()
	{
		bool spaceButtonPressed = (GetAsyncKeyState(VK_SPACE)) ? true : false;
		bool ready = !spaceButtonPressed;

		while (true)
		{
			if (!GetAsyncKeyState(VK_SPACE) && spaceButtonPressed) ready = true;

			if (ready && GetAsyncKeyState(VK_SPACE)) break;
		}
	}

	//}
	//----------------------------------------------------------------------------

