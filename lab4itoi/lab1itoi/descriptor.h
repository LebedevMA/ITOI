#pragma once
#include "../../share/image.h"
#include "../../share/interest_points.h"
#include <memory>

class descriptor
{
public:
	struct Gradient {
		double V[8];
		Gradient() {
			for (int i = 0;i < 8;i++) {
				V[i] = 0;
			}
		}
		Gradient(const Gradient &a) {
			for (int i = 0;i < 8;i++) {
				V[i] = a.V[i];
			}
		}
		static double Distance(const Gradient &a, const Gradient &b) {
			double res = 0;
			for (int i = 0;i < 8;i++) {
				double d = a.V[i] - b.V[i];
				res += d*d;
			}
			return sqrt(res);
		}
	};
private:
	Gradient *V;
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
	Gradient getElement(const int index) { return V[index]; };
	int getSize() { return size; }

	static std::unique_ptr<descriptor> FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, const int R, const int N);

	static double Distance(const descriptor &a, const descriptor &b);

	static std::unique_ptr<line[]> Connect(const interest_points &IP, const interest_points &IP2, const image &grad1, const image &grad2, int N, int R);

	descriptor();
	descriptor(descriptor &a);
	void operator=(descriptor &a);
	~descriptor();
};

