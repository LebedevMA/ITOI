#include "stdafx.h"
#include "pyramid.h"


pyramid::pyramid()
{
}

void pyramid::Gen(const std::unique_ptr<image> &img, const int scales, const double sigma) {
	kernel k = kernel::Gauss(sigma);
	std::unique_ptr<image> p = img->convolution(k);
	p = p->convolution(k.rotate());
	images.clear();
	double sigmaS = sigma;
	double sigmaR = sigma;
	double octave = 1;
	while (p->width >= 2 && p->height >= 2) {
		sigmaS = 1;
		double dsigma = pow(2, 1.0 / scales);
		for (int i = 0;i <= scales;i++) {
			sigmaS *= dsigma;
			sigmaR *= dsigma;
			kernel k1 = kernel::Gauss(sqrt(sigmaS*sigmaS - 1));
			std::unique_ptr<image> p1 = p->convolution(k1);
			p1 = p1->convolution(k1);
			images.push_back(new image_lite(p1));
			inform.push_back(new info(octave, sigmaS, sigmaR));
		}
		p = p->small2();
		octave++;
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
		delete images.back();
		images.pop_back();
	}
}
