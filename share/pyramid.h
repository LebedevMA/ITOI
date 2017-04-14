#pragma once
#include "image.h"
#include <vector>

class pyramid
{
public:
	struct info {
		int octave;
		double sigma;
		double sigmaR;
		double scale;
		info() {
			this->octave = 0;
			this->sigma = 0;
			this->sigmaR = 0;
			this->scale = 0;
		}
		info(int octave, double sigma, double sigmaR, double scale) {
			this->octave = octave;
			this->sigma = sigma;
			this->sigmaR = sigmaR;
			this->scale = scale;
		}
	};
private:
	std::vector<image> images;
	std::vector<image> dogs;
	std::vector<info> inform;
public:
	pyramid();

	image &getImage(int index) { return images[index]; }
	int getCount() const{ return images.size(); }
	info &getInfo(int index) { return inform[index]; }
	double getScale(int index) { return inform[index].scale; }

	void Gen(const image &img, const int scales, const double sigmaS = 1);
	int L(int x, int y, double sigma);

	double DoG(int x, int y, int z);
	bool isExtremum(int x, int y, int z);

	~pyramid();
};

