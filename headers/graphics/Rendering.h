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

				Renderer(unsigned int windowWidth, unsigned int windowHeight, COLORREF backgroundColor, const Matrix& startCamera, const Vector& shift, double parallax);

			// Functions:

				// Debugging:

					bool ok() const;

				// Camera stuff:

					Renderer& moveCamera(Matrix movement);

				// Rendering:

					void  startRendering() const;
					void finishRendering() const;

					void clear() const;

					void pixel(const int x, const int y, COLORREF color) const;
					void pixel3d(const Vector& point, COLORREF color) const;

					void line(int x0, int y0, int x1, int y1, const COLORREF color) const;
					void line3d(const Vector& point0, const Vector& point1, const COLORREF color) const;

					void triangle3d(const Vector& point0, const Vector& point1, const Vector& point2, const Vector& normal, COLORREF color) const;
					void triangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) const;

		private:

			unsigned int windowWidth_;
			unsigned int windowHeight_;

			COLORREF backgroundColor_;

			Matrix camera_;

			Vector shift_;

			double parallax_;

	};


	//----------------------------------------------------------------------------
	//{ Constructor && destructor
	//----------------------------------------------------------------------------

		Renderer::Renderer(unsigned int windowWidth, unsigned int windowHeight, COLORREF backgroundColor, const Matrix& startCamera, const Vector& shift, double parallax) :
			windowWidth_  (windowWidth),
			windowHeight_ (windowHeight),
			backgroundColor_ (backgroundColor),
			camera_		     (startCamera),
			shift_			 (shift),
			parallax_		 (parallax)
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

				if (camera_.getSizeX() != 4 || camera_.getSizeY() != 4)
				{
					everythingOk = false;
					printf("Renderer::ok(): Camera matrix has invalid size (it should be 4x4).\n");
				}

				if (!camera_.ok())
				{
					everythingOk = false;
					printf("Renderer::ok(): Camera matrix is not ok.\n");
				}

				return true;
			}

		//}
		//----------------------------------------------------------------------------


		//----------------------------------------------------------------------------
		//{ Camera
		//----------------------------------------------------------------------------

			Renderer& Renderer::moveCamera(Matrix movement)
			{
				assert(ok());
				assert(movement.ok());
				assert(movement.getSizeX() == 4 && movement.getSizeY() == 4);

				camera_ *= movement;

				assert(ok());

				return *this;
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
					SetPixel(txDC(), x, y, color);
				}

				void Renderer::pixel3d(const Vector& point, COLORREF color) const
				{
					assert(point.ok());

					Vector fixedPoint = (point  * camera_).perspectived(parallax_) + shift_;

					pixel(static_cast<int>(fixedPoint.x()), static_cast<int>(fixedPoint.y()), color);
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

				void Renderer::line3d(const Vector& point0, const Vector& point1, const COLORREF color) const
				{
					assert(point0.ok());
					assert(point1.ok());

					Vector fixedPoint0 = (point0 * camera_).perspectived(parallax_) + shift_;
					Vector fixedPoint1 = (point1 * camera_).perspectived(parallax_) + shift_;

					line(static_cast<int>(fixedPoint0.x()), static_cast<int>(fixedPoint0.y()),
						 static_cast<int>(fixedPoint1.x()), static_cast<int>(fixedPoint1.y()), color);
				}

			// Triangle:

				void Renderer::triangle3d(const Vector& point0, const Vector& point1, const Vector& point2, const Vector& normal, COLORREF color) const
				{
					assert(normal.ok());
					assert(point0.ok());
					assert(point1.ok());
					assert(point2.ok());

					if ((normal * camera_ - Vector(camera_[3][0], camera_[3][1], camera_[3][2])).z() > 0)
					{
						Vector fixedPoint0 = (point0 * camera_).perspectived(parallax_) + shift_;
						Vector fixedPoint1 = (point1 * camera_).perspectived(parallax_) + shift_;
						Vector fixedPoint2 = (point2 * camera_).perspectived(parallax_) + shift_;

						triangle(static_cast<int>(fixedPoint0.x()), static_cast<int>(fixedPoint0.y()),
								 static_cast<int>(fixedPoint1.x()), static_cast<int>(fixedPoint1.y()),
								 static_cast<int>(fixedPoint2.x()), static_cast<int>(fixedPoint2.y()), color);
					}
					
				}

				void Renderer::triangle(int x0, int y0, int x1, int y1, int x2, int y2, COLORREF color) const
				{	
					// Sorting points:

						if (x0 > x1)
						{
							std::swap(x0, x1);
							std::swap(y0, y1);
						}

						if (x1 > x2)
						{
							std::swap(x1, x2);
							std::swap(y1, y2);
						} 
						
						if (x0 > x1)
						{
							std::swap(x0, x1);
							std::swap(y0, y1);
						}

					// Creating useful variables:
						
						int dX = x1 - x0;
						int dX2 = dX << 1;

						int error2dX = 0;
						int deltaError = -(y1 - y0) << 1;

						int yLong = y0;
						int dXLong = x2 - x0;
						int dX2Long = dXLong << 1;

						int error2dXLong = 0;
						int deltaErrorLong = -(y2 - y0) << 1;
						
					// First triangle half:
						
						if (x0 != x1)
						{
							for (int x01 = x0, y01 = y0; x01 <= x1; x01++, error2dX += deltaError, error2dXLong += deltaErrorLong)
							{
								for (; error2dX < -dX; error2dX += dX2, y01++) pixel(x01, y01, color);
								for (; error2dX >  dX; error2dX -= dX2, y01--) pixel(x01, y01, color);

								for (; error2dXLong < -dXLong; error2dXLong += dX2Long, yLong++) pixel(x01, yLong, color);
								for (; error2dXLong >  dXLong; error2dXLong -= dX2Long, yLong--) pixel(x01, yLong, color);

								if (y01 <= yLong)
								{
									for (int fillingY = y01; fillingY <= yLong; fillingY++)
									{
										pixel(x01, fillingY, color);
									}
								}
								else
								{
									for (int fillingY = y01; fillingY >= yLong; fillingY--)
									{
										pixel(x01, fillingY, color);
									}
								}
							}
						}
						else
						{
							if (y0 <= y1) for (int y = y0; y <= y1; y++) pixel(x0, y, color);
							else		  for (int y = y0; y >= y1; y--) pixel(x0, y, color);
						}

					// Second triangle half

						dX = x2 - x1;
						dX2 = dX << 1;

						error2dX = 0;
						deltaError = -(y2 - y1) << 1;

						if (x1 != x2)
						{
							for (int x12 = x1, y12 = y1; x12 <= x2; x12++, error2dX += deltaError, error2dXLong += deltaErrorLong)
							{
								for (; error2dX < -dX; error2dX += dX2, y12++) pixel(x12, y12, color);
								for (; error2dX >  dX; error2dX -= dX2, y12--) pixel(x12, y12, color);

								for (; error2dXLong < -dXLong; error2dXLong += dX2Long, yLong++) pixel(x12, yLong, color);
								for (; error2dXLong >  dXLong; error2dXLong -= dX2Long, yLong--) pixel(x12, yLong, color);

								if (y12 <= yLong)
								{
									for (int fillingY = y12; fillingY <= yLong; fillingY++)
									{
										pixel(x12, fillingY, color);
									}
								}
								else
								{
									for (int fillingY = y12; fillingY >= yLong; fillingY--)
									{
										pixel(x12, fillingY, color);
									}
								}
							}
						}
						else
						{
							if (y1 <= y2) for (int y = y1; y <= y2; y++) pixel(x1, y, color);
							else		  for (int y = y1; y >= y2; y--) pixel(x1, y, color);
						}
				}

		//}
		//----------------------------------------------------------------------------

	//}
	//----------------------------------------------------------------------------

//}
//----------------------------------------------------------------------------
