#pragma once
#include <memory>

class kernel
{
	int width, height;
	std::unique_ptr<double[]> K;
public:
	kernel();
	kernel(const int width, const int height);
	kernel(const int width, const int height, double* K);
	kernel(const kernel &K);

	int getWidth() { return width; }
	void setWidth(int width) { this->width = width; }
	int getHeight() { return height; }
	void setHeight(int height) { this->height = height; }

	double getElement(int index) { return K[index]; }
	void setElement(int index, double value) { K[index] = value; }

	kernel rotate();

	kernel operator+(const kernel &k1);
	kernel operator*(const double a);

	static kernel MoveX(const int R);
	static kernel MoveY(const int R);

	static kernel Blur();
	static kernel Blur(const int R);

	static kernel SobelKx();
	static kernel SobelKy();

	static kernel Gauss(const double sigma);
	static kernel Gauss(const double sigma, const int R);

	static kernel Sharpen(const double sigma, const int R, const double alpha);

	~kernel();
};

