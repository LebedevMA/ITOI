#pragma once
#include "../../share/image.h"
#include "../../share/interest_points.h"
#include <memory>
#include <vector>
#include <algorithm>

class descriptor
{
private:
	std::vector<double> V;


public:
	double orientation;
	double centerDistance;
	double centerAngle;
	double size;

	struct angle {
		double phi;
		double weight;
		angle() {
			this->phi = 0;
			this->weight = 0;
		}
		angle(double phi, double weight) {
			this->phi = phi;
			this->weight = weight;
		}
	};
	struct line {
		int a, b;
		interest_points::point ptA, ptB;
		std::unique_ptr<descriptor> dA, dB;
		double d;
		line() {
			a = 0;
			b = 0;
			d = 0;
			dA = nullptr;
			dB = nullptr;
		}
		line(int a, int b, double d) {
			this->a = a;
			this->b = b;
			this->d = d;
			dA = nullptr;
			dB = nullptr;
		}
		line(int a, int b, double d, const interest_points::point &ptA, const interest_points::point &ptB) {
			this->a = a;
			this->b = b;
			this->d = d;
			this->ptA = ptA;
			this->ptB = ptB;
			dA = nullptr;
			dB = nullptr;
		}
		line(int a, int b, double d, const interest_points::point &ptA, const interest_points::point &ptB, const descriptor &dA, const descriptor &dB) {
			this->a = a;
			this->b = b;
			this->d = d;
			this->ptA = ptA;
			this->ptB = ptB;
			this->dA = std::make_unique<descriptor>(dA);
			this->dB = std::make_unique<descriptor>(dB);
		}
		line(const line &A) {
			this->a = A.a;
			this->b = A.b;
			this->d = A.d;
			this->ptA = A.ptA;
			this->ptB = A.ptB;
			if (A.dA != nullptr) this->dA = std::make_unique<descriptor>(*(A.dA));else this->dA = nullptr;
			if (A.dB != nullptr) this->dB = std::make_unique<descriptor>(*(A.dB));else this->dB = nullptr;
		}

		line &operator=(const line &A) {
			line res;
			res.a = A.a;
			res.b = A.b;
			res.d = A.d;
			res.ptA = A.ptA;
			res.ptB = A.ptB;
			if (A.dA != nullptr) res.dA = std::make_unique<descriptor>(*(A.dA));else res.dA = nullptr;
			if (A.dB != nullptr) res.dB = std::make_unique<descriptor>(*(A.dB));else res.dB = nullptr;
			return res;
		}
	};

public:
	double getElement(const int index) const{ return V[index]; };
	int getSize() const{ return V.size(); }

	double Length();
	void Normalize();
	void Clamp(double k);

	static double PointOrientation(const image &Gx, const image &Gy, const interest_points::point &pt, int R, double scale, double sigma0, double sigma1);

	static std::unique_ptr<descriptor> FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, int R, const int FramesDividor, const int NumberOfBaskets, double scale, double sigma0, double sigma1);
	static double Distance(const descriptor &a, const descriptor &b);

	static std::unique_ptr<std::unique_ptr<descriptor>[]> GetDescriptors(interest_points &IP, int N, int R);

	static std::unique_ptr<std::vector<line>> Connect(const interest_points &IP, const interest_points &IP2, std::unique_ptr<std::unique_ptr<descriptor>[]> & D1, std::unique_ptr<std::unique_ptr<descriptor>[]> & D2, int N, double T);

	descriptor();
	descriptor(const descriptor &a);
	void operator=(const descriptor &a);
	~descriptor();
};

