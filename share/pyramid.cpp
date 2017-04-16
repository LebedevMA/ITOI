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
	double scale = 1.0;
	while (p->getWidth() >= 2 && p->getHeight() >= 2) {
		sigmaS = sigma;
		double dsigma = pow(2, 1.0 / scales);
		std::unique_ptr<image> p1;
		for (int i = 0;i < scales+3;i++) {
			sigmaS *= dsigma;
			sigmaR *= dsigma;
			kernel k1 = kernel::Gauss(sqrt(sigmaS*sigmaS - 1));
			p1 = p->convolution(k1);
			p1 = p1->convolution(k1.rotate());
			images.push_back(*p1);
			inform.push_back(info(octave, sigmaS, sigmaR, scale));
			if (i == scales - 1) {
				p = p1->small2();
			}
		}
		//p = p1->small2();
		octave++;
		scale *= 0.5;
	}

	for (int i = 1;i < images.size();i++) {
		dogs.push_back(*image::Difference(images[i - 1], images[i], img.getWidth(), img.getHeight()));
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

double pyramid::DoG(int x, int y, int z)
{
	return dogs[z].getPixel(x, y);
}

bool pyramid::isExtremum(int x, int y, int z)
{
	bool min = true;
	bool max = true;
	double s = 0;
	double a = DoG(x, y, z);

	for (int x1 = -1;x1 <= 1;x1++) {
		for (int y1 = -1;y1 <= 1;y1++) {
			for (int z1 = -1;z1 <= 1;z1++) {
				if (x1 == 0 && y1 == 0 && z1 == 0) continue;
				double b = DoG(x + x1, y + y1, z + z1);
				if (b > a) max = false;
				if (b < a) min = false;
				if (b != a) s++;
			}
		}
	}
	s /= 26;
	if (s < 0.75) return false;
	if (max || min) {
		return true;
	}
	return false;
}

pyramid::~pyramid()
{
}
