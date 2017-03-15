#include "stdafx.h"
#include "pyramid.h"


pyramid::pyramid()
{
}

void pyramid::Gen(const image &img, const int scales, const double sigma) {
	kernel k = kernel::Gauss(sqrt(sigma*sigma - 0.25));
	std::unique_ptr<image> p = img.convolution(k);
	p = p->convolution(k.rotate());
	images.clear();
	double sigmaS = sigma;
	double sigmaR = sigma;
	double octave = 1;
	while (p->getWidth() >= 2 && p->getHeight() >= 2) {
		sigmaS = sigma;
		double dsigma = pow(2, 1.0 / scales);
		std::unique_ptr<image> p1;
		for (int i = 0;i < scales;i++) {
			sigmaS *= dsigma;
			sigmaR *= dsigma;
			kernel k1 = kernel::Gauss(sqrt(sigmaS*sigmaS - 1));
			p1 = p->convolution(k1);
			p1 = p1->convolution(k1.rotate());
			images.push_back(image_lite(p1));
			inform.push_back(info(octave, sigmaS, sigmaR));
		}
		p = p1->small2();
		octave++;
	}
}

int pyramid::L(int x, int y, double sigma) {
	int width0 = images[0].getWidth();
	int height0 = images[0].getHeight();
	for (int i = 0;i < images.size()-1;i++) {
		if (sigma >= inform[i].sigmaR && sigma <= inform[i + 1].sigmaR) {
			int x1 = x * images[i].getWidth() * (1.0 / width0);
			int y1 = y * images[i].getHeight() * (1.0 / height0);
			return images[i].getElement(x1 + y1*(images[i].getWidth()));
		}
	}
	return 0;
}

pyramid::~pyramid()
{
}
