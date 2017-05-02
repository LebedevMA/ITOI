#pragma once
#include "descriptor.h"
#include <map>

class hough
{
public:
	struct Result {
		double x;
		double y;
		double size;
		double angle;
		double width;
		double height;

		Result() {
			x = 0;
			y = 0;
			size = 0;
			angle = 0;
			width = 0;
			height = 0;
		}
		Result(double x, double y, double size, double angle, double width, double height) {
			this->x = x;
			this->y = y;
			this->size = size;
			this->angle = angle;
			this->width = width;
			this->height = height;
		}
		Result(const Result &a) {
			x = a.x;
			y = a.y;
			size = a.size;
			angle = a.angle;
			width = a.width;
			height = a.height;
		}
	};
	hough();

	static std::unique_ptr<Result> Find(const std::vector<descriptor::line> &lines, double width1, double height1, double T);

	~hough();
};

