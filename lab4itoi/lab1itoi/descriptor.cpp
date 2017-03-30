#include "stdafx.h"
#include "descriptor.h"


descriptor::descriptor()
{
	V = nullptr;
	size = 0;
}

descriptor::descriptor(descriptor &a)
{
	V = new double[a.size];
	size = a.size;
	for (int i = 0;i < a.size;i++) {
		V[i] = a.V[i];
	}
}

void descriptor::operator=(descriptor &a)
{
	V = new double[a.size];
	size = a.size;
	for (int i = 0;i < a.size;i++) {
		V[i] = a.V[i];
	}
}

std::unique_ptr<descriptor> descriptor::FromPoint(const image &img, const interest_points::point &pt, const int R) {
	std::unique_ptr<descriptor> res = std::make_unique<descriptor>();
	res->V = new double[R];
	res->size = R;
	int *count = new int[R];
	for (int i = 0;i < R;i++) {
		res->V[i] = 0;
		count[i] = 0;
	}
	for (int i = -R+1;i < R;i++) {
		for (int j = -R+1;j < R;j++) {
			int x = i + pt.x;
			int y = j + pt.y;
			int d = (int)(sqrt(i*i+j*j));
			if (d >= R) continue;
			res->V[d] += img.getPixel(x, y);
			count[d]++;
		}
	}
	for (int i = 0;i < R;i++) {
		if (count[i] == 0) continue;
		res->V[i] /= count[i];
	}
	delete count;
	return res;
}

double descriptor::Distance(const std::unique_ptr<descriptor> &a, const std::unique_ptr<descriptor> &b) {
	double res = 0;
	for (int i = 0;i < a->size && i < b->size;i++) {
		double d = a->V[i] - b->V[i];
		res += d*d;
	}
	return res;
}

descriptor::~descriptor()
{
	delete V;
}
