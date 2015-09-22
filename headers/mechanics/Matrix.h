#pragma once

//----------------------------------------------------------------------------
//{ Matrix
//----------------------------------------------------------------------------

    class Matrix
    {
        public:

            static bool check;

            // Constructor && destructor:

                template <typename ... Components>
                Matrix(size_t sizeX, size_t sizeY, Components... components);

                Matrix(const Matrix& matrix);

                ~Matrix();

            // Getters && setters:

                size_t getSizeX() const;
                size_t getSizeY() const;

            // Functions:

                bool ok() const;
                void print() const;

				Matrix  transponded() const;
				Matrix& transpond();

            // Operators:

                Matrix& operator=(const Matrix& matrix);

                double* operator[](const size_t x) const;

                Matrix& operator+=(const Matrix& matrix);
                Matrix& operator-=(const Matrix& matrix);

                Matrix operator+(const Matrix& matrix) const;
                Matrix operator-(const Matrix& matrix) const;

                Matrix& operator*=(double koefficient);
                Matrix& operator/=(double koefficient);

                Matrix operator*(double koefficient) const;
                Matrix operator/(double koefficient) const;

                Matrix  operator* (const Matrix& matrix) const;
				Matrix& operator*=(const Matrix& matrix);

        protected:

			size_t sizeX_;
			size_t sizeY_;

			double** components_;

		private:

            template <typename First, typename... Rest>
            void expandedConstructorIteration(size_t x, size_t y, First component, Rest... components);

            void expandedConstructorIteration(size_t x, size_t y);
    };


    //----------------------------------------------------------------------------
    //{ Constructor && detructor:
    //----------------------------------------------------------------------------

        template <typename ... Components>
        Matrix::Matrix(size_t sizeX, size_t sizeY, Components... components) :
            sizeX_      (sizeX),
            sizeY_      (sizeY),
            components_ (NULL)
        {
            components_ = (double**) calloc(sizeX_, sizeof(*components_));
            assert(components_);

            for (size_t x = 0; x < sizeX_; x++)
            {
                components_[x] = (double*) calloc(sizeY_, sizeof(*components_[x]));
                assert(components_[x]);
            }

            expandedConstructorIteration(0, 0, components...);

            assert(ok());
        }

        template <typename First, typename... Rest>
        void Matrix::expandedConstructorIteration(size_t x, size_t y, First component, Rest... components)
        {
            assert(x < sizeX_ && y < sizeY_);

            components_[x][y] = component;

            x++;
            if (x >= sizeX_)
            {
                x = 0;
                y++;
            }

            expandedConstructorIteration(x, y, components...);
        }

        void Matrix::expandedConstructorIteration(size_t x, size_t y) {}

        Matrix::Matrix(const Matrix& matrix) :
            sizeX_ (matrix.getSizeX()),
            sizeY_ (matrix.getSizeY()),
            components_ (NULL)
        {
            components_ = (double**) calloc(sizeX_, sizeof(*components_));
            assert(components_);

            for (size_t x = 0; x < sizeX_; x++)
            {
                components_[x] = (double*) calloc(sizeY_, sizeof(*components_[x]));
                assert(components_[x]);
            }

            for (size_t x = 0; x < sizeX_; x++)
            {
                for (size_t y = 0; y < sizeY_; y++)
                {
                    components_[x][y] = matrix[x][y];
                }
            }

            assert(ok());
        }

        Matrix::~Matrix()
        {
            for (size_t x = 0; x < sizeX_; x++)
            {
                free(components_[x]);
            }

            free(components_);
        }

    //}
    //----------------------------------------------------------------------------


    //----------------------------------------------------------------------------
    //{ Getters && setters:
    //----------------------------------------------------------------------------

        size_t Matrix::getSizeX() const
        {
            assert(ok());

            return sizeX_;
        }

        size_t Matrix::getSizeY() const
        {
            assert(ok());

            return sizeY_;
        }

    //}
    //----------------------------------------------------------------------------


    //----------------------------------------------------------------------------
    //{ Functions:
    //----------------------------------------------------------------------------

        bool Matrix::ok() const
        {
            if (check == false) return true;

            if (components_ == NULL)
            {
                printf("components_ is a NULL pointer.\n");
                return false;
            }

            for (size_t x = 0; x < sizeX_; x++)
            {
                if (components_[x] == NULL)
                {
                    printf("components_[%d] is a NULL pointer.\n", x);
                    return false;
                }
            }

            return true;
        }

        void Matrix::print() const
        {
            assert(ok());

            puts("");

            for (size_t y = 0; y < sizeY_; y++)
            {
                for (size_t x = 0; x < sizeX_; x++)
                {
                    printf("%7.3f ", components_[x][y]);
                }

                puts("");
            }

        }

		Matrix Matrix::transponded() const
		{
			assert(ok());

			Matrix toReturn = Matrix(sizeY_, sizeX_);

			for (size_t x = 0; x < sizeX_; x++)
			{
				assert(x < sizeX_);

				for (size_t y = 0; y < sizeY_; y++)
				{
					assert(y < sizeY_);
				
					toReturn[y][x] = components_[x][y];
				}
			}

			return toReturn;
		}

		Matrix& Matrix::transpond()
		{
			assert(ok());

			*this = transponded();

			return *this;
		}

    //}
    //----------------------------------------------------------------------------


    //----------------------------------------------------------------------------
    //{ Operators:
    //----------------------------------------------------------------------------

        // Operator =

            Matrix& Matrix::operator=(const Matrix& matrix)
            {
                assert(matrix.ok());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    free(components_[x]);
                }

                free(components_);

                sizeX_ = matrix.getSizeX();
                sizeY_ = matrix.getSizeY();

                components_ = (double**) calloc(sizeX_, sizeof(*components_));
                assert(components_);

                for (size_t x = 0; x < sizeX_; x++)
                {
                    components_[x] = (double*) calloc(sizeY_, sizeof(*components_[x]));
                    assert(components_[x]);
                }

                for (size_t x = 0; x < sizeX_; x++)
                {
                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        components_[x][y] = matrix[x][y];
                    }
                }

                assert(ok());

                return *this;
            }

        // Access:

            double* Matrix::operator[](const size_t x) const
            {
                assert(x < sizeX_);

                return components_[x];
            }

        // Addition/distraction

            Matrix& Matrix::operator+=(const Matrix& matrix)
            {
                assert(ok());
                assert(matrix.ok());

                assert(sizeX_ == matrix.getSizeX() && sizeY_ == matrix.getSizeY());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        components_[x][y] += matrix[x][y];
                    }
                }

                assert(ok());

                return *this;
            }

            Matrix& Matrix::operator-=(const Matrix& matrix)
            {
                assert(ok());
                assert(matrix.ok());

                assert(sizeX_ == matrix.getSizeX() && sizeY_ == matrix.getSizeY());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        components_[x][y] -= matrix[x][y];
                    }
                }

                assert(ok());

                return *this;
            }

            Matrix Matrix::operator+(const Matrix& matrix) const
            {
                assert(ok());
                assert(matrix.ok());

                Matrix toReturn = *this;
                toReturn += matrix;

                return toReturn;
            }

            Matrix Matrix::operator-(const Matrix& matrix) const
            {
                assert(ok());
                assert(matrix.ok());

                Matrix toReturn = *this;
                toReturn -= matrix;

                return toReturn;
            }

        // Multiplication/ division on scalar

            Matrix& Matrix::operator*=(double koefficient)
            {
                assert(ok());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        components_[x][y] *= koefficient;
                    }
                }

                assert(ok());

                return *this;
            }

            Matrix& Matrix::operator/=(double koefficient)
            {
                assert(ok());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        components_[x][y] /= koefficient;
                    }
                }

                assert(ok());

                return *this;
            }

            Matrix Matrix::operator*(double koefficient) const
            {
                assert(ok());

                Matrix toReturn = *this;
                toReturn *= koefficient;

                return toReturn;
            }

            Matrix Matrix::operator/(double koefficient) const
            {
                assert(ok());

                Matrix toReturn = *this;
                toReturn /= koefficient;

                return toReturn;
            }

        // Matrix multiplication

            Matrix Matrix::operator*(const Matrix& matrix) const
            {
                assert(ok());
                assert(matrix.ok());

                assert(sizeX_ == matrix.getSizeY());

                Matrix toReturn = Matrix(matrix.getSizeX(), sizeY_, 0);

                for (size_t x = 0; x < toReturn.getSizeX(); x++)
                {
                    for (size_t y = 0; y < toReturn.getSizeY(); y++)
                    {
                        for (size_t i = 0; i < sizeX_; i++)
                        {
                            toReturn[x][y] += components_[i][y] * matrix[x][i];
                        }
                    }
                }

                return toReturn;
            }

			Matrix& Matrix::operator*=(const Matrix& matrix) 
			{
				*this = *this + matrix;

				return *this;
			}

    //}
    //----------------------------------------------------------------------------

//}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
//{ Matrix generating functions:
//----------------------------------------------------------------------------

	Matrix identityMatrix(const size_t rowsCount)
	{
		Matrix toReturn = Matrix(rowsCount, rowsCount);

		for (size_t i = 0; i < rowsCount; i++)
		{
			toReturn[i][i] = 1;
		}

		return toReturn;
	}

	Matrix rotationMatrixX(double angle)
	{
		return Matrix(3, 3, 1,			0,			 0,
							0, cos(angle), -sin(angle),
							0, sin(angle), cos(angle));
	}

	Matrix rotationMatrixY(double angle)
	{
		return Matrix(3, 3, cos(angle), 0, -sin(angle),
									 0, 1,			 0,
							sin(angle), 0,  cos(angle));
	}

	Matrix rotationMatrixZ(double angle)
	{
		return Matrix(3, 3, cos(angle), -sin(angle), 0,
							sin(angle),  cos(angle), 0,
									 0,			  0, 1);
	}

	Matrix rotationMatrix(double angleX, double angleY, double angleZ)
	{
		return rotationMatrixX(angleX) *
			   rotationMatrixY(angleY) *
			   rotationMatrixZ(angleZ);
	}

//}
//----------------------------------------------------------------------------

