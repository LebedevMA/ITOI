#pragma once
#include "image.h"
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

	point getPoint(int index) const { return points[index]; };
	int getCount() const { return points.size(); }

	void Moravek(const image &img, int N, int r, double T = 0);
	void Harris(const image &img, int N, int r, double T = 0);

	void Filtration(const std::unique_ptr<point[]> &pts, const int ptscount, const int N);

	static bool ptcmp(const point &a, const point &b) {
		return (a.s > b.s);
	}
};

