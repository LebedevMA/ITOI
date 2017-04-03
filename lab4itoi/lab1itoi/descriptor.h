#pragma once
#include "../../share/image.h"
#include "../../share/interest_points.h"
#include <memory>

class descriptor
{
	double *V;
	int size;

	struct line {
		int a, b;
		interest_points::point ptA, ptB;
		double d;
		line() {
			a = 0;
			b = 0;
			d = 0;
		}
		line(int a, int b, double d) {
			this->a = a;
			this->b = b;
			this->d = d;
		}
		line(int a, int b, double d, const interest_points::point &ptA, const interest_points::point &ptB) {
			this->a = a;
			this->b = b;
			this->d = d;
			this->ptA = ptA;
			this->ptB = ptB;
		}
	};

public:
	double getElement(const int index) { return V[index]; };
	int getSize() { return size; }

	static std::unique_ptr<descriptor> FromPoint(const image &img,const interest_points::point &pt, const int R);

	static double Distance(const std::unique_ptr<descriptor> &a, const std::unique_ptr<descriptor> &b);

	static std::unique_ptr<line[]> Connect(const interest_points &IP, const interest_points &IP2, const image &grad1, const image &grad2, int N, int R);

	descriptor();
	descriptor(descriptor &a);
	void operator=(descriptor &a);
	~descriptor();
};

