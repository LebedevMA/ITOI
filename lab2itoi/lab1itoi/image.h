#pragma once
#include <memory>
#include "kernel.h"

//using namespace std;

class image
{
public:
	enum effect {BLACK, REPT, MIRROR};

	int width;
	int height;
	double *V;
	effect KE;

	image();
	image(const int width, const int height);
	image(const image *a);
	void load(System::String ^filename);
	void save(System::String ^filename);
	void draw();

	void Normalize();

	void setKE(const effect ke);

	double getPixel(const int x, const int y);
	void setPixel(const int x, const int y, const double c);

	std::unique_ptr<image> convolution(const kernel &k);

	std::unique_ptr<image> Sobel(const kernel Ky, const kernel Kx);

	std::unique_ptr<image> small2();

	~image();
};

