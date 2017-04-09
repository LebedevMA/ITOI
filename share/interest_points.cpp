#include "stdafx.h"
#include "interest_points.h"
#include <map>
#include <memory>
#include <algorithm>


interest_points::interest_points()
{
}

void interest_points::Moravek(const image &img, int N, int r, double T) {
	int width = img.getWidth();
	int height = img.getHeight();
	auto S = std::make_unique<double[]>(width*height);
	double max = 0;
	for (int x = r;x < width -r;x++) {
		for (int y = r;y < height - r;y++) {
			double S1[] = {0,0,0,0,0,0,0,0};
			for (int u = -r;u < r;u++) {
				for (int v = -r;v < r;v++) {
					double d[8];
					double pxl1 = img.getPixel(x + u, y + v);
					d[0] = pxl1 - img.getPixel(x+u, y+v-1);
					d[1] = pxl1 - img.getPixel(x+u+1, y+v);
					d[2] = pxl1 - img.getPixel(x+u, y+v+1);
					d[3] = pxl1 - img.getPixel(x+u-1, y+v);

					d[4] = pxl1 - img.getPixel(x + u + 1, y + v - 1);
					d[5] = pxl1 - img.getPixel(x + u + 1, y + v + 1);
					d[6] = pxl1 - img.getPixel(x + u - 1, y + v + 1);
					d[7] = pxl1 - img.getPixel(x + u - 1, y + v - 1);

					for (int i = 0;i < 8;i++) {
						S1[i] += d[i] * d[i];
					}
				}
			}

			S[y*width + x] = *(std::min_element(&S1[0], &S1[8]));
			if (S[y*width + x] > max) max = S[y*width + x];
		}
	}
	/*
	for (int i = 0;i < width*height;i++) {
		S[i] /= max;
	} */

	std::vector<point> pts;

	for (int x = r;x < width - r;x+=2) {
		for (int y = r;y < height - r;y+=2) {
			if (S[x + y*width] >= T) {
				point pt(x, y, S[x + y*width]);
				pts.push_back(pt);
			}
		}
	}

	std::sort(pts.begin(), pts.end(), [](auto &a, auto &b) { return a.s > b.s; });

	Filtration(pts, pts.size() / 2, N);
}


void interest_points::Harris(const image &img1, int N, int r, double T) {

	kernel Ky = kernel::SobelKy();
	kernel Kx = kernel::SobelKx();
	std::unique_ptr<image> Gy = img1.convolution(Ky);
	std::unique_ptr<image> Gx = img1.convolution(Kx);

	int width = img1.getWidth();
	int height = img1.getHeight();
	auto S = std::make_unique<double[]>(width*height);

	for (int x = r;x < width - r;x++) {
		for (int y = r;y < height - r;y++) {
			double lmin = image::lambda(*Gx,*Gy,x,y,r,r);

			S[y*width + x] = lmin;
		}
	}

	std::vector<point> pts;

	for (int x = r;x < width - r;x+=2) {
		for (int y = r;y < height - r;y+=2) {
			if (S[x + y*width] >= T) {
				point pt(x, y, S[x + y*width]);
				pts.push_back(pt);
			}
		}
	}
	
	std::sort(pts.begin(), pts.end(), [](auto &a, auto &b) { return a.s > b.s; });

	Filtration(pts, pts.size()/2, N);
}

void interest_points::Filtration(const std::vector<point>& pts, int ptscount, const int N)
{
	auto radiuses = std::make_unique<int[]>(ptscount);
	int maxradius = 0;
	for (int i = 0;i < ptscount;i++) {
		int radius = INT_MAX;
		for (int j = 0;j < i;j++) {
			int x0 = pts[j].x - pts[i].x;
			int y0 = pts[j].y - pts[i].y;
			int radius1 = (int)sqrt(x0*x0 + y0*y0);
			if (radius1 < radius) {
				radius = radius1;
			}
		}
		radiuses[i] = radius;
		if (i > 0 && radius > maxradius) maxradius = radius;
	}
	radiuses[0] = maxradius + 1;

	auto flags = std::make_unique<bool[]>(ptscount);
	for (int i = 0;i < ptscount;i++) {
		flags[i] = false;
	}

	auto pts1 = std::make_unique<point[]>(N);
	int ptscount1 = 1;
	int radius = maxradius;
	pts1[0] = pts[0];
	flags[0] = true;
	while (ptscount1 < N) {
		for (int i = 0;i < ptscount;i++) {
			if (flags[i]) continue;
			if (radiuses[i] >= radius) {
				pts1[ptscount1++] = pts[i];
				flags[i] = true;
			}
			if (ptscount1 >= N) break;
		}
		radius--;
	}

	for (int i = 0;i < N;i++) {
		points.push_back(pts1[i]);
	}
}


interest_points::~interest_points()
{
}
