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
#include <fstream>

#include "headers/TXLib.h"

#include "headers/mechanics/Matrix.h"
#include "headers/mechanics/Vector.h"

#include "headers/graphics/Rendering.h"
#include "headers/graphics/Model.h"

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

