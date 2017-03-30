#pragma once
#include "../../share/image.h"
#include <vector>

class interest_points
{
public:
	class point {
	public:
		int x;
		int y;
		double s;
		point() {
			this->x = 0;
			this->y = 0;
			this->s = 0;
		}
		point(int x, int y, double s) {
			this->x = x;
			this->y = y;
			this->s = s;
		};
	};
private:
	std::vector<point> points;
public:
	interest_points();
	~interest_points();

	point getPoint(int index) { return points[index]; };
	int getCount() { return points.size(); }

	void Moravek(image &img, int N, int r, int p, double T = 0);
	void Harris(image &img, int N, int r, int p, double T = 0);
};

