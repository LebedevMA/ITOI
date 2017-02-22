#pragma once
#include "image.h"
#include "image_lite.h"
#include <vector>

class pyramid
{
public:
	struct info {
		int oktava;
		double sigma;
		double sigmaR;
		info(int oktava, double sigma, double sigmaR) {
			this->oktava = oktava;
			this->sigma = sigma;
			this->sigmaR = sigmaR;
		}
	};
	std::vector<std::unique_ptr<image_lite>> images;
	std::vector<std::unique_ptr<info>> inform;
	pyramid();
	void Gen(const std::unique_ptr<image> &img, const int scales);
	int L(int x, int y, double sigma);
	~pyramid();
};

