#pragma once
#include "image.h"

class image_lite
{
public:
	int width;
	int height;
	unsigned char *V;

	image_lite();
	image_lite(const std::unique_ptr<image> &a);

	void draw();

	~image_lite();
};

