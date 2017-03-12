#pragma once
#include "image.h"

class image_lite
{
	int width;
	int height;
	unsigned char *V;
public:

	image_lite();
	image_lite(const std::unique_ptr<image> &a);

	int getWidth() { return width; }
	void setWidth(int width) { this->width = width; }
	int getHeight() { return height; }
	void setHeight(int height) { this->height = height; }

	unsigned char getElement(int index) { return V[index]; }
	void setElement(int index, unsigned char value) { V[index] = value; }

	void draw();

	~image_lite();
};

