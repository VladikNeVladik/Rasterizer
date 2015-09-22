#include "Includes.h"

//----------------------------------------------------------------------------
//{ MLG MODE
//----------------------------------------------------------------------------

	#define MLG (GetAsyncKeyState('M') && GetAsyncKeyState('L') && GetAsyncKeyState('G'))

//}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Static class members initialization
//----------------------------------------------------------------------------

bool Matrix::check = true;
bool Vector::check = true;

//}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Test cube
//----------------------------------------------------------------------------

    struct Cube
    {
		Cube(double radius, Vector coords,
							Vector v000,
							Vector v001,
							Vector v010,
							Vector v011,
							Vector v100,
							Vector v101,
							Vector v110,
							Vector v111);

		void render(const Renderer* renderer, const COLORREF color);

		Cube& transform(const Matrix& transformationMatrix);

        double radius_;

		Vector coords_;

        Vector v000_, v001_, v010_, v011_, v100_, v101_, v110_, v111_;
    };

	Cube::Cube(double radius, Vector coords,
							  Vector v000,
							  Vector v001,
							  Vector v010,
							  Vector v011,
							  Vector v100,
							  Vector v101,
							  Vector v110,
							  Vector v111) :
		radius_ (radius),
		coords_ (coords),
		v000_ (v000),
		v001_ (v001),
		v010_ (v010),
		v011_ (v011),
		v100_ (v100),
		v101_ (v101),
		v110_ (v110),
		v111_ (v111)
	{}

    void Cube::render(const Renderer* renderer, const COLORREF color)
    {
		renderer->line3d((v000_ * radius_) + coords_, (v001_ * radius_) + coords_, color);
		renderer->line3d((v000_ * radius_) + coords_, (v010_ * radius_) + coords_, color);
		renderer->line3d((v000_ * radius_) + coords_, (v100_ * radius_) + coords_, color);

		renderer->line3d((v001_ * radius_) + coords_, (v011_ * radius_) + coords_, color);
		renderer->line3d((v001_ * radius_) + coords_, (v101_ * radius_) + coords_, color);

		renderer->line3d((v010_ * radius_) + coords_, (v011_ * radius_) + coords_, color);
		renderer->line3d((v010_ * radius_) + coords_, (v110_ * radius_) + coords_, color);

		renderer->line3d((v100_ * radius_) + coords_, (v101_ * radius_) + coords_, color);
		renderer->line3d((v100_ * radius_) + coords_, (v110_ * radius_) + coords_, color);

		renderer->line3d((v111_ * radius_) + coords_, (v011_ * radius_) + coords_, color);
		renderer->line3d((v111_ * radius_) + coords_, (v101_ * radius_) + coords_, color);
		renderer->line3d((v111_ * radius_) + coords_, (v110_ * radius_) + coords_, color);
    }

	Cube& Cube::transform(const Matrix& transformationMatrix)
	{
		v000_ *= transformationMatrix;
		v001_ *= transformationMatrix;
		v010_ *= transformationMatrix;
		v011_ *= transformationMatrix;
		v100_ *= transformationMatrix;
		v101_ *= transformationMatrix;
		v110_ *= transformationMatrix;
		v111_ *= transformationMatrix;

		return *this;
	}

//}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Main
//----------------------------------------------------------------------------

    int main()
    {
		Renderer renderer = Renderer(1000, 800, RGB(0, 0, 0), Vector(500, 400, 0), 400);

        Cube test =
        {   200, Vector(0, 0, 600),
            Vector(-1, -1, -1),
			Vector(-1, -1,  1),
			Vector(-1,  1, -1),
			Vector(-1,  1,  1),
			Vector( 1, -1, -1),
			Vector( 1, -1,  1),
			Vector( 1,  1, -1),
			Vector( 1,  1,  1),
        };

		Matrix rotFront = transformationMatrix(+0.01, +0.01, +0.03);
		Matrix rotBack  = transformationMatrix(-0.01, -0.01, -0.03);

		Matrix zoomPlus  = transformationMatrix(0, 0, 0, Vector(0, 0, 0), Vector(1.10, 1.10, 1.10));
		Matrix zoomMinus = transformationMatrix(0, 0, 0, Vector(0, 0, 0), Vector(0.92, 0.92, 0.92));

        while (!GetAsyncKeyState(VK_ESCAPE))
        {
			if (!MLG)
			{
				if (GetAsyncKeyState('R')) test.transform(rotBack);
				else test.transform(rotFront);

				if (GetAsyncKeyState('Z'))
				{
					if (GetAsyncKeyState(VK_SHIFT)) test.transform(zoomMinus);
					else test.transform(zoomPlus);
				}

				renderer.clear();
				txClearConsole();

				renderer.startRendering();

				test.render(&renderer, TX_WHITE);

				renderer.finishRendering();
			}
			else
			{
				test.transform(rotFront);

				if (rand() % 6 == 0)
				{
					test.transform(rotBack)
						.transform(transformationMatrix(random(-0.1, 0.05),
							random(-0.1, 0.05),
							random(-0.1, 0.05)));

					txSetConsoleAttr(rand() % 16);

					switch (rand() % 5)
					{
					case 0:
					{
						printf("I_AM_THE_TRUE_ILLUMINATI_NOSCOPE_MASTER");

						break;
					}
					case 1:
					{
						printf("TIME TO RECK SOME      SCRUUUUBS    ");

						break;
					}
					case 2:
					{
						printf("SHREK  IS  LOVE,  SHREK  IS  LIFE");

						break;
					}
					case 3:
					{
						printf("WE GOTTA BE SAAANEEG FAASTT-_-_-_-_-_-_=\n                    ");

						break;
					}
					case 4:
					{
						printf("SMOKE WEED EVERYDAY");
						break;
					}
					default: break;
					}
				}

				if (rand() % 3 == 0)
				{
					renderer.clear();
				}

				renderer.startRendering();

				test.render(&renderer, RGB(rand() % 256, rand() % 256, rand() % 256));

				renderer.finishRendering();
			}
        }

        return 0;
    }

//}
//----------------------------------------------------------------------------
