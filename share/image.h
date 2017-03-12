#pragma once
#include <memory>
#include "kernel.h"

//using namespace std;

class image
{
public:
	enum effect { BLACK, REPT, MIRROR };
private:
	int width;
	int height;
	effect KE;
	double *V;
public:


	image();
	image(const int width, const int height);
//	image(const image *a);
	image(const image &a);
	image(image &&a);
	void operator=(const image &a);
	void load(System::String ^filename);
	void save(System::String ^filename);
	void draw();
	void draw(System::Drawing::Graphics ^g);

	void Normalize();

	const int getWidth() { return width; }
	void setWidth(int width) { this->width = width; }
	const int getHeight() { return height; }
	void setHeight(int height) { this->height = height; }

	double getElement(int index) { return V[index]; }
	void setElement(int index, double value) { V[index] = value; }

	effect getKE() { return KE; }
	void setKE(const effect ke);

	double getPixel(const int x, const int y);
	void setPixel(const int x, const int y, const double c);


	std::unique_ptr<image> convolution(kernel &k);

	std::unique_ptr<image> Sobel();

	std::unique_ptr<image> small2();

	~image();
};

