#pragma once
#include "image.h"
#include "image_lite.h"
#include <vector>

class pyramid
{
public:
	struct info {
		int octave;
		double sigma;
		double sigmaR;
		info(int octave, double sigma, double sigmaR) {
			this->octave = octave;
			this->sigma = sigma;
			this->sigmaR = sigmaR;
		}
	};
	std::vector<image_lite *> images;
	std::vector<info *> inform;
	pyramid();
	void Gen(const std::unique_ptr<image> &img, const int scales, const double sigmaS = 1);
	int L(int x, int y, double sigma);
	~pyramid();
};

