#pragma once

//----------------------------------------------------------------------------
//{ Matrix
//----------------------------------------------------------------------------

	/*!
	@brief Класс, содержащий матрицу. Позволяет выполнять преразнообразнейшие операции над матрицами 

	@usage @code
		Matrix getReadyForSWAG = Matrix(3, 3, 1, 2, 3
											  4, 5, 6,
											  7, 8, 9);

		Matrix swag = ((getReadyForSWAG + getReadyForSWAG - getReadyForSWAG) * getReadyForSWAG[2][2] / getReadyForSWAG[2][2]).transposed();
		assert(swag.ok());
		
		swag.print();

	@endcode
	*/
    class Matrix
    {
        public:

			//! @brief check == true <=> ok() checkss everything, check == false <=> ok() returns true in any case
            static bool check;

            // Constructor && destructor:

				/*!
					@brief   Конструирует матрицу размером sizeX x sizeY
				
					@param   sizeX		X-размер матрицы
					@param   sizeY		Y-размер матрицы
					@param   components Элементы, вставляемые в матрицу (идёт вставка по строкам (порядок вставки: (0, 0), (1, 0), ...))
				 
					@usage @code
						Matrix matrix = Matrix(3, 3, 1, 2, 3
													 4, 5, 6
													 7, 8, 9);      

						// Создалась матрица размером 3x3 с элементами [1 2 3]
						//											   [4 5 6]
						//											   [7 8 9]

						Matrix matrixWithZeroes = Matrix(2, 2, 4);
						 
						// Создалась матрица размера 2x2 с элементами [4 0]
						//											  [0 0]
					@endcode
				*/
				template <typename ... Components>
                Matrix(size_t sizeX, size_t sizeY, Components... components);

				/*!
				@brief   Конструирует матрицу по образу и подобию другой матрицы

				@param matrix Матрица-прародитель, по обрузу и подобию которой будет создан новый объект(лучший мир)  

				@usage @code
					Matrix ancestor = Matrix(1, 1, 616);

					Matrix child = ancestor; // Конструктор копирования вызван
				@endcode
				*/
                Matrix(const Matrix& matrix);

				/*!
				@brief   Изничтожает матрицу. Насовсем
				*/
                ~Matrix();

            // Getters && setters:
			
				 
                size_t getSizeX() const;
                size_t getSizeY() const;

            // Functions:

                bool ok() const;
                void print() const;

				Matrix  transposed() const;
				Matrix& transpose();

            // Operators:

                Matrix& operator=(const Matrix& matrix);

                double* operator[](const size_t x) const;
                //double& operator[](const size_t x, const size_t y) const;

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
                assert(x < sizeX_);

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

		void Matrix::expandedConstructorIteration(size_t x, size_t y) { x; y; }

        Matrix::Matrix(const Matrix& matrix) :
            sizeX_ (matrix.getSizeX()),
            sizeY_ (matrix.getSizeY()),
            components_ (NULL)
        {
            components_ = (double**) calloc(sizeX_, sizeof(*components_));
            assert(components_);

            for (size_t x = 0; x < sizeX_; x++)
            {
                assert(x < sizeX_);

                components_[x] = (double*) calloc(sizeY_, sizeof(*components_[x]));
                assert(components_[x]);
            }

            for (size_t x = 0; x < sizeX_; x++)
            {
                assert(x < sizeX_);

                for (size_t y = 0; y < sizeY_; y++)
                {
                    assert(y < sizeY_);

                    components_[x][y] = matrix[x][y];
                }
            }

            assert(ok());
        }

        Matrix::~Matrix()
        {
            for (size_t x = 0; x < sizeX_; x++)
            {
                assert(x < sizeX_);

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
                assert(x < sizeX_);

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
                assert(y < sizeY_);

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    printf("%10.6f ", components_[x][y]);
                }

                puts("");
            }

        }

		Matrix Matrix::transposed() const
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

		Matrix& Matrix::transpose()
		{
			assert(ok());

			*this = transposed();

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
				if (this == &matrix) return *this;

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    free(components_[x]);
                }

                free(components_);

                sizeX_ = matrix.getSizeX();
                sizeY_ = matrix.getSizeY();

                components_ = (double**) calloc(sizeX_, sizeof(*components_));
                assert(components_);

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    components_[x] = (double*) calloc(sizeY_, sizeof(*components_[x]));
                    assert(components_[x]);
                }

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        assert(y < sizeY_);

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

            /*
            double& Matrix::operator[](const size_t x, const size_t y) const
            {
                assert(x < sizeX_ && y < sizeY_);

                return components_[x][y];
            }
            */

        // Addition/distraction

            Matrix& Matrix::operator+=(const Matrix& matrix)
            {
                assert(ok());
                assert(matrix.ok());

                assert(sizeX_ == matrix.getSizeX() && sizeY_ == matrix.getSizeY());

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        assert(y < sizeY_);

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
                    assert(x < sizeX_);

                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        assert(y < sizeY_);

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
                    assert(x < sizeX_);

                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        assert(y < sizeY_);

                        components_[x][y] *= koefficient;
                    }
                }

                assert(ok());

                return *this;
            }

            Matrix& Matrix::operator/=(double koefficient)
            {
                assert(ok());
				assert(koefficient != 0);

                for (size_t x = 0; x < sizeX_; x++)
                {
                    assert(x < sizeX_);

                    for (size_t y = 0; y < sizeY_; y++)
                    {
                        assert(y < sizeY_);

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
				assert(koefficient != 0);

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
                    assert(x < toReturn.getSizeX());

                    for (size_t y = 0; y < toReturn.getSizeY(); y++)
                    {
                        assert(y < toReturn.getSizeY());

                        for (size_t i = 0; i < sizeX_; i++)
                        {
                            assert(i < sizeX_);

                            toReturn[x][y] += components_[i][y] * matrix[x][i];
                        }
                    }
                }

                return toReturn;
            }

			Matrix& Matrix::operator*=(const Matrix& matrix)
			{
				*this = *this * matrix;

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
            assert(i < toReturn.getSizeX() && i < toReturn.getSizeY());

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

