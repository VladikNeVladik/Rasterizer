#pragma once

//----------------------------------------------------------------------------
//{ Includes
//----------------------------------------------------------------------------

	#include <utility>

//}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Renderer
//----------------------------------------------------------------------------

	class Renderer
	{
		public:

			// Constructor && destructor:
	
				Renderer(unsigned int windowWidth, unsigned int windowHeight, COLORREF backgroundColor, Vector shift, double parallax);

			// Functions:

				// Debugging:

					bool ok() const;

				// Rendering:

					void  startRendering() const;
					void finishRendering() const;

					void clear() const;

					void pixel(const int x, const int y, COLORREF color) const;

					void line(int x0, int y0, int x1, int y1, const COLORREF color) const;
					void line3d(const Vector start, const Vector finish, const COLORREF color) const;

		private:

			unsigned int windowWidth_;
			unsigned int windowHeight_;

			COLORREF backgroundColor_;

			Vector shift_;
			double parallax_;

	};


	//----------------------------------------------------------------------------
	//{ Constructor && destructor
	//----------------------------------------------------------------------------

		Renderer::Renderer(unsigned int windowWidth, unsigned int windowHeight, COLORREF backgroundColor, Vector shift, double parallax) :
			windowWidth_  (windowWidth),
			windowHeight_ (windowHeight),
			backgroundColor_ (backgroundColor),
			shift_	  (shift),
			parallax_ (parallax)
		{
			txCreateWindow(windowWidth, windowHeight);
			txTextCursor(false);

			assert(ok());
		}

	//}
	//----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//{ Functions
	//----------------------------------------------------------------------------

		//----------------------------------------------------------------------------
		//{ Debugging
		//----------------------------------------------------------------------------

			bool Renderer::ok() const
			{
				bool everythingOk = true;

				if (!shift_.ok())
				{
					everythingOk = false;
					printf("Renderer::ok(): coordinate system shift_ is not ok.\n");
				}

				return true;
			}

		//}
		//----------------------------------------------------------------------------


		//----------------------------------------------------------------------------
		//{ Rendering
		//----------------------------------------------------------------------------

			// Additional:

				void Renderer::startRendering() const
				{
					txBegin();
				}

				void Renderer::finishRendering() const
				{
					txEnd();
				}

				void Renderer::clear() const
			{
				assert(ok());

				txSetFillColor(backgroundColor_);
				txClear();
			}
				
			// Pixel:
				
				void Renderer::pixel(const int x, const int y, COLORREF color) const
			{
				txSetPixel(x, y, color);
			}

			// Line:
				
				void Renderer::line(int x0, int y0, int x1, int y1, const COLORREF color) const
				{
					bool swappedXandY = false;

					if (abs(y1 - y0) > abs(x1 - x0))
					{
						std::swap(y0, x0);
						std::swap(y1, x1);

						swappedXandY = true;
					}

					if (x1 < x0)
					{
						std::swap(x0, x1);
						std::swap(y0, y1);
					}
	
					int dX = x1 - x0;
					int dY = y1 - y0;
					int dX2 = dX << 1;

					int error2dX = 0;
					int deltaError = -(dY << 1);

					for (int x = x0, y = y0; x <= x1; x++, error2dX += deltaError)
					{
						if (swappedXandY) txSetPixel(y, x, color);
						else pixel(x, y, color);

						if (error2dX < -dX)
						{
							error2dX += dX2;
							y++;
						}
						else if (error2dX > dX)
						{
							error2dX -= dX2;
							y--;
						}
					}
				}

				void Renderer::line3d(const Vector start, const Vector finish, const COLORREF color) const
				{
					assert(start.ok());
					assert(finish.ok());
					
					Vector fixedStart  =  start.perspectived(parallax_) + shift_;
					Vector fixedFinish = finish.perspectived(parallax_) + shift_;

					txSetColor(color);

					line(static_cast<int>(fixedStart .x()), static_cast<int>(fixedStart .y()),
						 static_cast<int>(fixedFinish.x()), static_cast<int>(fixedFinish.y()), color);
				}

		//}
		//----------------------------------------------------------------------------

	//}
	//----------------------------------------------------------------------------

//}
//----------------------------------------------------------------------------