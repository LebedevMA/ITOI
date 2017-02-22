#include "stdafx.h"
#include "kernel.h"


kernel::kernel()
{
	width = 0;
	height = 0;
	K = nullptr;
}

kernel::kernel(const int width, const int height)
{
	this->width = width;
	this->height = height;
	K = std::unique_ptr<double[]>(new double[width*height]);
}

kernel::kernel(const int width, const int height, double* K)
{
	this->width = width;
	this->height = height;
	this->K = std::unique_ptr<double[]>(new double[width*height]);
	for (int i = 0;i < width*height;i++) {
		this->K[i] = K[i];
	}
}


kernel::kernel(const kernel &k)
{
	this->width = k.width;
	this->height = k.height;
	this->K = std::unique_ptr<double[]>(new double[width*height]);
	for (int i = 0;i < width*height;i++) {
		this->K[i] = k.K[i];
	}
}

kernel kernel::rotate() {
	kernel kr(*this);
	kr.width = height;
	kr.height = width;
	return kr;
}


kernel kernel::operator+(const kernel k1) {
	kernel out(k1.width, k1.height);
	for (int i = 0;i < k1.height*k1.width;i++) {
		out.K[i] = k1.K[i] + K[i];
	}
	return out;
}

kernel kernel::operator*(const double a) {
	kernel out(width, height);
	for (int i = 0;i < height*width;i++) {
		out.K[i] = K[i] * a;
	}
	return out;
}

kernel kernel::MoveX(const int R) {
	kernel k(1, R);
	for (int i = 0;i < R;i++) {
		k.K[i] = 0;
	}
	if (R >= 0) k.K[0] = 1;
	else k.K[R - 1] = 1;
	return k;
}

kernel kernel::MoveY(const int R) {
	kernel k(1, R);
	for (int i = 0;i < R;i++) {
		k.K[i] = 0;
	}
	if (R < 0) k.K[0] = 1;
	else k.K[R - 1] = 1;
	return k;
}

kernel kernel::Blur() {
	kernel k(3, 3);
	for (int i = 0;i < 9;i++) {
		k.K[i] = 0.1;
	}
	return k;
}

kernel kernel::Blur(const int R) {
	kernel k(R, R);
	for (int i = 0;i < R*R;i++) {
		k.K[i] = 1.0 / (R*R);
	}
	return k;
}

kernel kernel::SobelKx(){
	double Kx[] = { -3,0,3,-10,0,10,-3,0,3 };
	return kernel(3,3,Kx);
}

kernel kernel::SobelKy() {
	double Ky[] = { -3,-10,-3,0,0,0,3,10,3 };
	return kernel(3, 3, Ky);
}

kernel kernel::Gauss(const double sigma) {
	int R = 6 * sigma + 1;
	return Gauss(sigma, R);
}

kernel kernel::Gauss(const double sigma, const int R) {
	kernel k(R, 1);
	double sigma2 = 2 * sigma*sigma;
	double kk = 1 / sqrt(6.28*sigma);
	double sum = 0;
	for (int i = 0;i < R;i++) {
		//int x = i - (R / 2);
		k.K[i] = kk*exp(-(pow(i - (R / 2), 2)) / sigma2);
		sum += k.K[i];
	}
	for (int i = 0;i < R;i++) {
		k.K[i] /= sum;
	}
	return k;
}

kernel kernel::Sharpen(const double sigma, const int R, const double alpha) {
	kernel k(R, 1);
	double sigma2 = 2 * sigma*sigma;
	double kk = 1 / sqrt(6.28*sigma);
	double sum = 0;
	for (int i = 0;i < R;i++) {
		k.K[i] = kk*exp(-(pow(i - (R / 2), 2)) / sigma2);
	}
	for (int i = 0;i < R;i++) {
		k.K[i] = -alpha * k.K[i];
		sum += k.K[i];
	}
	k.K[R / 2 + 1] += 1 + alpha;
	sum += 1 + alpha;
	for (int i = 0;i < R;i++) {
		k.K[i] /= sum;
	}
	return k;
}


kernel::~kernel()
{
}
