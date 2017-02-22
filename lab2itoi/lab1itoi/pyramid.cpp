#include "stdafx.h"
#include "pyramid.h"


pyramid::pyramid()
{
}

void pyramid::Gen(const std::unique_ptr<image> &img, const int scales) {
	std::unique_ptr<image> p = std::unique_ptr<image>(img.get());
	images.clear();
	double sigma = 1;
	double sigmaR = 1;
	double oktava = 1;
	while (p->width >= 2 && p->height >= 2) {
		sigma = 1;
		double dsigma = sigma / scales;
		for (int i = 0;i < scales;i++) {
			p = p->convolution(kernel::Gauss(sigma));
			images.push_back(std::unique_ptr<image_lite>(new image_lite(p)));
			inform.push_back(std::unique_ptr<info>(new info(oktava, sigma, sigmaR)));
			sigma += dsigma;
			sigmaR += dsigma;
		}
		p = p->small2();
		oktava++;
	}
}

int pyramid::L(int x, int y, double sigma) {
	int width0 = images[0]->width;
	int height0 = images[0]->height;
	for (int i = 0;i < images.size()-1;i++) {
		if (sigma >= inform[i]->sigmaR && sigma <= inform[i + 1]->sigmaR) {
			int x1 = x * images[i]->width * (1.0 / width0);
			int y1 = y * images[i]->height * (1.0 / height0);
			return images[i]->V[x1 + y1*(images[i]->width)];
		}
	}
	return 0;
}

pyramid::~pyramid()
{
	while (images.size() > 0) {
		images.back().release();
		images.pop_back();
	}
}
