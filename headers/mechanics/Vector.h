#pragma once

//----------------------------------------------------------------------------
//{ Vector
//----------------------------------------------------------------------------

	class Vector
	{
		public:

			static bool check;

			// Constructors && destrutor:

				Vector(double x = 0, double y = 0, double z = 0);

				Vector(const Vector& vector);
				Vector(const Matrix& matrix);

				~Vector();

			// Getters && setters:

				const Matrix& toMatrix() const;

				double& x() const;
				double& y() const;
				double& z() const;

			// Functions:

				// Debugging:

					bool ok() const;
					void print() const;

				// Length:

					double length() const;

				// Normalization:

					Vector  normalized() const;
					Vector& normalize();

				// Perspective:

					Vector  perspectived(double parallax = 1) const;
					Vector& perspective (double parallax = 1);

			// Operators:

				Vector& operator=(const Vector& vector);
				Vector& operator=(const Matrix& matrix);

				Vector& operator+=(const Vector& vector);
				Vector& operator-=(const Vector& vector);
				Vector& operator*=(const double koefficient);
				Vector& operator/=(const double koefficient);

				Vector operator+(const Vector& vector) const;
				Vector operator-(const Vector& vector) const;
				Vector operator*(const double koefficient) const;
				Vector operator/(const double koefficient) const;

                // Cross product:

                Vector& operator^=(const Vector& vector);
                Vector  operator^ (const Vector& vector) const;

				double operator*(const Vector& vector) const;

				Vector& operator*=(const Matrix& matrix);
				Vector  operator* (const Matrix& matrix) const;

		private:

			Matrix matrix_;
	};

	//----------------------------------------------------------------------------
	//{ Constructors && destructor:
	//----------------------------------------------------------------------------

		Vector::Vector(double x /*= 0*/, double y /*= 0*/, double z /*= 0*/) :
			matrix_ (Matrix(1, 3, x, y, z))
		{
			assert(ok());
		}

		Vector::Vector(const Vector& vector) :
			matrix_ (Matrix(vector.toMatrix()))
		{
			assert(ok());
		}

		Vector::Vector(const Matrix& matrix) :
			matrix_ (Matrix(matrix))
		{
			assert(ok());
		}

		Vector::~Vector()
		{
		}

	//}
	//-----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//{ Getters && setters
	//----------------------------------------------------------------------------

		const Matrix& Vector::toMatrix() const
		{
			assert(ok());

			return matrix_;
		}

		double& Vector::x() const
		{
			return matrix_[0][0];
		}

		double& Vector::y() const
		{
			return matrix_[0][1];
		}

		double& Vector::z() const
		{
			return matrix_[0][2];
		}

	//}
	//-----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//{ Functions
	//----------------------------------------------------------------------------

		// Debugging:

			bool Vector::ok() const
			{
				if (check == false) return true;

				if (!matrix_.ok())
				{
					printf("Vector::ok(): matrix_ is not ok.");
					return false;
				}

				if (matrix_.getSizeX() != 1)
				{
					printf("Vector::ok(): invalid sizeX\n");
					return false;
				}

				if (matrix_.getSizeY() != 3)
				{
					printf("Vector::ok(): invalid sizeY\n");
					return false;
				}

				return true;
			}

			void Vector::print() const
			{
				assert(ok());

				matrix_.print();
			}

		// Length

			double Vector::length() const
			{
				assert(ok());

				return x() * x() + y() * y() + z() * z();
			}

		// Normalization:

			Vector Vector::normalized() const
			{
				double vectorLength = length();

				if (vectorLength == 0) return *this;

				return { x() / vectorLength, y() / vectorLength, z() / vectorLength };
			}

			Vector& Vector::normalize()
			{
				double vectorLength = length();

				if (vectorLength == 0) return *this;

				x() /= vectorLength;
				y() /= vectorLength;
				z() /= vectorLength;

				return *this;
			}

		// Perspective:

			Vector Vector::perspectived(double parallax /*= 1*/) const
			{
				if (z() == 0) return *this;

				return Vector(parallax * x() / z(), parallax * y() / z(), 1);
			}

			Vector& Vector::perspective(double parallax /*= 1*/)
			{
				if (z() == 0) return *this;

				x() *= parallax;
				x() /= z();

				y() *= parallax;
				y() /= z();

				z() = 1;

				return *this;
			}

	//}
	//-----------------------------------------------------------------------------


	//----------------------------------------------------------------------------
	//{ Operators
	//----------------------------------------------------------------------------

		// Operator =

			Vector& Vector::operator=(const Vector& vector)
			{
                if (&vector.toMatrix() != &matrix_)
                {
                    assert(vector.ok());

                    matrix_ = vector.toMatrix();

                    assert(ok());

                }

				return *this;
			}

			Vector& Vector::operator=(const Matrix& matrix)
			{
                if (&matrix != &matrix_)
                {
                    assert(ok());
                    assert(matrix.ok());

                    assert(matrix.getSizeX() == 1 &&
                           matrix.getSizeY() == 3);

                    matrix_ = matrix;

                    assert(ok());
                }

				return *this;
			}

		// Vector arithmetics:

			// Assignment:

				Vector& Vector::operator+=(const Vector& vector)
				{
					assert(ok());
					assert(vector.ok());

					matrix_ += vector.toMatrix();

					assert(ok());

					return *this;
				}

				Vector& Vector::operator-=(const Vector& vector)
				{
					assert(ok());
					assert(vector.ok());

					matrix_ -= vector.toMatrix();

					assert(ok());

					return *this;
				}

				Vector& Vector::operator*=(const double koefficient)
				{
					assert(ok());

					matrix_ *= koefficient;

					assert(ok());

					return *this;
				}

				Vector& Vector::operator/=(const double koefficient)
				{
					assert(ok());

					matrix_ /= koefficient;
					assert(koefficient != 0);

					assert(ok());

					return *this;
				}

			// Arithmetical:

				Vector Vector::operator+(const Vector& vector) const
				{
					assert(ok());
					assert(vector.ok());

					Vector toReturn = *this;
					toReturn += vector;

					assert(toReturn.ok());

					return toReturn;
				}

				Vector Vector::operator-(const Vector& vector) const
				{
					assert(ok());
					assert(vector.ok());

					Vector toReturn = *this;
					toReturn -= vector;

					assert(toReturn.ok());

					return toReturn;
				}

				Vector Vector::operator*(const double koefficient) const
				{
					assert(ok());

					Vector toReturn = *this;
					toReturn *= koefficient;

					assert(toReturn.ok());

					return toReturn;
				}

				Vector Vector::operator/(const double koefficient) const
				{
					assert(ok());
					assert(koefficient != 0);

					Vector toReturn = *this;
					toReturn /= koefficient;

					assert(toReturn.ok());

					return toReturn;
				}

			// Scalar multiplication (dot product):

				double Vector::operator*(const Vector& vector) const
				{
					return x() * vector.x() +
						   y() * vector.y() +
						   z() * vector.z();
				}

            // Cross product:

                Vector& Vector::operator^=(const Vector& vector)
                {
                    assert(ok());
                    assert(vector.ok());

                    *this = *this ^ vector;

                    return *this;
                }

                Vector Vector::operator^(const Vector& vector) const
                {
                    assert(ok());
                    assert(vector.ok());

                    return Vector(y() * vector.z() - z() * vector.y(),
                                  z() * vector.x() - x() * vector.z(),
                                  x() * vector.y() - y() * vector.x());
                }

			// Matrix multiplication:

				Vector& Vector::operator*=(const Matrix& matrix)
				{
					assert(ok());
					assert(matrix.ok());

					if (matrix.getSizeX() == 4 && matrix.getSizeY() == 4)
					{
						Matrix newMatrix = Matrix(1, 4, x(), y(), z(), 1);

						newMatrix = matrix * newMatrix;

						x() = newMatrix[0][0];
						y() = newMatrix[0][1];
						z() = newMatrix[0][2];
					}
					else matrix_ = matrix * matrix_;

					assert(ok());

					return *this;
				}

				Vector Vector::operator*(const Matrix& matrix) const
				{
					assert(ok());
					assert(matrix.ok());

					Vector toReturn = *this;

					toReturn *= matrix;

					assert(toReturn.ok());

					return toReturn;
				}

	//}
	//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Matrix generating functions prototypes:
//----------------------------------------------------------------------------

	Matrix transformationMatrix(double angleX = 0,
								double angleY = 0,
								double angleZ = 0,
								Vector shift	 = Vector(0, 0, 0),
								Vector extension = Vector(1, 1, 1));

//}
//-----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Matrix generating functions:
//----------------------------------------------------------------------------

	Matrix transformationMatrix(double angleX /*= 0*/,
								double angleY /*= 0*/,
								double angleZ /*= 0*/,
								Vector shift	 /*= Vector(0, 0, 0)*/,
								Vector extension /*= Vector(1, 1, 1)*/)
	{
		Matrix toReturn = identityMatrix(4);

		Matrix rotation = rotationMatrix(angleX, angleY, angleZ);

		for (size_t x = 0; x < 3; x++)
		{
			for (size_t y = 0; y < 3; y++)
			{
				toReturn[x][y] = rotation[x][y];
			}
		}

		toReturn[3][0] = shift.x();
		toReturn[3][1] = shift.y();
		toReturn[3][2] = shift.z();

		toReturn[0][0] *= extension.x();
		toReturn[1][1] *= extension.y();
		toReturn[2][2] *= extension.z();

		assert(toReturn.ok());

		return toReturn;
	}

//}
//-----------------------------------------------------------------------------
