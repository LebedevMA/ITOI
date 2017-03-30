#pragma once
#include "../../share/image.h"
#include "interest_points.h"
#include <memory>

class descriptor
{
	double *V;
	int size;
public:
	double getElement(const int index) { return V[index]; };
	int getSize() { return size; }

	static std::unique_ptr<descriptor> FromPoint(const image &img,const interest_points::point &pt, const int R);

	static double Distance(const std::unique_ptr<descriptor> &a, const std::unique_ptr<descriptor> &b);

	descriptor();
	descriptor(descriptor &a);
	void operator=(descriptor &a);
	~descriptor();
};

