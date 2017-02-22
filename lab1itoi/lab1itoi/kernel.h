#pragma once
#include <memory>

class kernel
{
public:
	int width, height;
	std::unique_ptr<double[]> K;
	kernel();
	kernel(const int width, const int height);
	kernel(const int width, const int height, double* K);
	kernel(const kernel &K);

	kernel rotate();

	kernel operator+(const kernel k1);
	kernel operator*(const double a);

	static kernel MoveX(const int R);
	static kernel MoveY(const int R);

	static kernel Blur();
	static kernel Blur(const int R);

	static kernel SobelKx();
	static kernel SobelKy();

	static kernel Gauss(const double sigma, const int R);

	static kernel Sharpen(const double sigma, const int R, const double alpha);

	~kernel();
};

