#include "stdafx.h"
#include "interest_points.h"
#include <map>
#include <memory>
#include <algorithm>


interest_points::interest_points()
{
}

void interest_points::Moravek(const image &img, int N, int r, int p, double T) {
	int width = img.getWidth();
	int height = img.getHeight();
	auto S = std::make_unique<double[]>(width*height);
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

			S[y*width + x] = *(std::min_element(&S1[0], &S1[7]));
		}
	}

	auto pts = std::make_unique<point[]>(width*height);
	while (true) {

		int ptscount = 0;

		for (int x = r + p;x < width - r - p;x++) {
			for (int y = r + p;y < height - r - p;y++) {
				if (T > 0 && S[x + y*width] < T) continue;
				bool flag = true;
				for (int u = -p;u < p;u++) {
					for (int v = -p;v < p;v++) {
						if (S[x + y*width] < S[x + u + (y + v)*width]) {
							flag = false;
							break;
						}
					}
					if (!flag) break;
				}
				if (flag) {
					point pt(x, y, S[x + y*width]);
					pts[ptscount++] = pt;
				}
			}
		}

		if (ptscount <= N) break;
		p++;
	}

	//std::sort(&pts[0], &pts[ptscount], ptcmp);

	for (int i = 0;i < N;i++) {
		points.push_back(pts[i]);
	}
}


void interest_points::Harris(const image &img1, int N, int r, int p, double T) {

	kernel Ky = kernel::SobelKy();
	kernel Kx = kernel::SobelKx();
	std::unique_ptr<image> Gy = img1.convolution(Ky);
	std::unique_ptr<image> Gx = img1.convolution(Kx);

	int width = img1.getWidth();
	int height = img1.getHeight();
	double *S = new double[width*height];

	for (int x = r;x < width - r;x++) {
		for (int y = r;y < height - r;y++) {
			double lmin = image::lambda(*Gx,*Gy,x,y,r,r);

			S[y*width + x] = lmin;
		}
	}

	auto pts = std::make_unique<point[]>(width*height);
	while (true) {

		int ptscount = 0;

		for (int x = r + p;x < width - r - p;x++) {
			for (int y = r + p;y < height - r - p;y++) {
				if (T > 0 && S[x + y*width] < T) continue;
				bool flag = true;
				for (int u = -p;u < p;u++) {
					for (int v = -p;v < p;v++) {
						if (S[x + y*width] < S[x + u + (y + v)*width]) {
							flag = false;
							break;
						}
					}
					if (!flag) break;
				}
				if (flag) {
					point pt(x, y, S[x + y*width]);
					pts[ptscount++] = pt;
				}
			}
		}

		if (ptscount <= N) break;
		p++;
	}

	//std::sort(&pts[0], &pts[ptscount], ptcmp);

	for (int i = 0;i < N;i++) {
		points.push_back(pts[i]);
	}
}


interest_points::~interest_points()
{
}
