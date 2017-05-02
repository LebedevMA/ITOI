#pragma once
#include "../../share/image.h"
#include "../../share/pyramid.h"
#include "../../share/interest_points.h"
#include "descriptor.h"
#include "hough.h"


	ref class Engine
	{
	public:
		ref struct Result {
			System::String ^fileName;
			double x;
			double y;
			double size;
			double angle;
			double width;
			double height;
			double weight;
			Result() {
				fileName = "";
				x = 0;
				y = 0;
				size = 0;
				angle = 0;
				width = 0;
				height = 0;
				weight = 0;
			}
			Result(System::String ^fileName, double x, double y, double size, double angle, double width, double height, double weight) {
				this->fileName = fileName;
				this->x = x;
				this->y = y;
				this->size = size;
				this->angle = angle;
				this->width = width;
				this->height = height;
				this->weight = weight;
			}
			Result(Result ^a) {
				this->fileName = a->fileName;
				x = a->x;
				y = a->y;
				size = a->size;
				angle = a->angle;
				width = a->width;
				height = a->height;
				this->weight = a->weight;
			}
		};
		Engine();

		static System::Collections::Generic::List<Result ^> ^FindContaining(System::String ^inputImage, System::String ^inputDirectory, int N, double T, double T1);
	};

