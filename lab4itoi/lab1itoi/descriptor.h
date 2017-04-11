#pragma once
#include "../../share/image.h"
#include "../../share/interest_points.h"
#include <memory>

class descriptor
{
private:
	double *V;
	int size;

public:
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

	static descriptor &FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, const int R, const int NumberOfFrames, const int NumberOfBaskets);

	static double Distance(const descriptor &a, const descriptor &b);

	static std::unique_ptr<descriptor[]> GetDescriptors(const interest_points &IP, const image &grad1, int N, int R);

	static std::unique_ptr<line[]> Connect(const interest_points &IP, const interest_points &IP2, std::unique_ptr<descriptor[]> & D1, std::unique_ptr<descriptor[]> & D2, int N);

	descriptor();
	descriptor(descriptor &a);
	void operator=(descriptor &a);
	~descriptor();
};

