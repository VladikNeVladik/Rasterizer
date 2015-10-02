#pragma once

//----------------------------------------------------------------------------
//{ Triangle
//----------------------------------------------------------------------------

	struct Triangle
	{
		public:
			
			// These are just numbers of points in vertecies array
			unsigned int point0, point1, point2;

			COLORREF color;

			Vector normal;
	};

//}
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
//{ Model
//----------------------------------------------------------------------------

	class Model
	{
		public:

			// Constructor && destructor:

				Model(const char* filename);
				~Model();

			// Functions:

				bool ok() const;

				void render(const Renderer* renderer, const Matrix& transformation) const;

		private:

            size_t pointCount_;
			Vector*	  points_;

			size_t triangleCount_;
			Triangle* triangles_;
	};

	//----------------------------------------------------------------------------
    //{ Constructor && destructor
    //----------------------------------------------------------------------------

        Model::Model(const char* filename) :
            pointCount_    (0),
            points_        (NULL),
            triangleCount_ (0),
            triangles_     (NULL)
        {
            // Checking input:

                assert(filename);

            // Opening file:

				std::ifstream modelFile = std::ifstream(filename);
				assert(modelFile.good());

            // Getting arrays lengths:

				modelFile >> pointCount_;
				modelFile >> triangleCount_;
				
            // Creating arrays:

                points_ = (Vector*) calloc(pointCount_, sizeof(*points_));
                assert(points_);

                triangles_ = (Triangle*) calloc(triangleCount_, sizeof(*triangles_));
                assert(triangles_);

            // Filling point array:

				double currentX = 0, currentY = 0, currentZ = 0;
                for (size_t i = 0; i < pointCount_; i++)
                {
					assert(i < pointCount_);

					modelFile >> currentX;
					modelFile >> currentY;
					modelFile >> currentZ;

                    points_[i] = Vector(currentX, currentY, currentZ);
                }

            // Filling triangle array:

                unsigned int currentPoint0 = 0, currentPoint1 = 0, currentPoint2 = 0;
				COLORREF color = 0;
				for (size_t i = 0; i < triangleCount_; i++)
				{
					assert(i < triangleCount_);

					std::dec(modelFile);

					modelFile >> currentPoint0;
					modelFile >> currentPoint1;
					modelFile >> currentPoint2;

					std::hex(modelFile);

					modelFile >> color;
					
					assert(currentPoint0 < pointCount_);
					assert(currentPoint1 < pointCount_);
					assert(currentPoint2 < pointCount_);

					triangles_[i] = 
					{
						currentPoint0,
						currentPoint1,
						currentPoint2,
						color,
						((points_[currentPoint1] - points_[currentPoint0]) ^
						(points_[currentPoint2] - points_[currentPoint0])).normalize()
					};
				}

			// Closing file:

				modelFile.close();

            // Checking output:

                assert(ok());
        }

		Model::~Model()
		{
			free(points_);
			free(triangles_);
		}

    //}
    //----------------------------------------------------------------------------

	//----------------------------------------------------------------------------
	//{ Functions
	//----------------------------------------------------------------------------

		bool Model::ok() const
		{
			bool everythingOk = true;

			if (points_ == NULL)
			{
				puts("Model::ok(): points_ is a NULL pointer");
				everythingOk = false;
			}

			if (triangles_ == NULL)
			{
				puts("Model::ok(): triangles_ is a NULL pointer");
				everythingOk = false;
			}

			return everythingOk;
		}

		void Model::render(const Renderer* renderer, const Matrix& transformation) const
		{
			assert(ok());
			assert(renderer->ok());
			assert(transformation.ok());

			assert(transformation.getSizeX() == 4 && transformation.getSizeY() == 4);

			for (size_t currentTriangle = 0; currentTriangle < triangleCount_; currentTriangle++)
			{
				assert(currentTriangle < triangleCount_);

				renderer->triangle3d
				(
					points_[triangles_[currentTriangle].point0] * transformation,
					points_[triangles_[currentTriangle].point1] * transformation,
					points_[triangles_[currentTriangle].point2] * transformation,
					triangles_[currentTriangle].normal * transformation - Vector(transformation[3][0], transformation[3][1], transformation[3][2]),
					triangles_[currentTriangle].color
				);
			}
		}

	//}
	//----------------------------------------------------------------------------

//}
//----------------------------------------------------------------------------
