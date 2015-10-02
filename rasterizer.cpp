#include "Includes.h"

//----------------------------------------------------------------------------
//{ Static class members initialization
//----------------------------------------------------------------------------

bool Matrix::check = true;
bool Vector::check = true;

//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//{ Main
//----------------------------------------------------------------------------

    int main()
    {
		Model cube = Model("resources/cube.txt");

		Renderer renderer = Renderer(1000, 800, RGB(0, 0, 0), transformationMatrix(0, 0, 0, Vector(0, 0, 300)), Vector(500, 400), 200);
		
		Matrix rotFront = transformationMatrix(+0.01, +0.01, +0.03);
		Matrix rotBack  = transformationMatrix(-0.01, -0.01, -0.03);

		while (!GetAsyncKeyState(VK_ESCAPE))
		{
			renderer.clear();
			
			if (GetAsyncKeyState(VK_RETURN))
			{
				if (GetAsyncKeyState(VK_LSHIFT)) renderer.moveCamera(rotBack);
				else renderer.moveCamera(rotFront);
			}

			renderer.startRendering();

			cube.render(&renderer, transformationMatrix());

			renderer.finishRendering();
		}
		

        return 0;
    }

//}
//----------------------------------------------------------------------------
