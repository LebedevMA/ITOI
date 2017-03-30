#include "stdafx.h"
#include "interest_points.h"
#include <map>
#include <memory>
#include <algorithm>


interest_points::interest_points()
{
}

void interest_points::Moravek(image &img, int N, int r, int p, double T) {
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
	int ptscount = 0;

	for (int x = r+p;x < width-r-p;x++) {
		for (int y = r+p;y < height-r-p;y++) {
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
				point pt(x,y,S[x+y*width]);
				pts[ptscount++] = pt;
			}
		}
	}
	
	for (int i = 0;i < ptscount-1;i++) {
		for (int j = i+1;j < ptscount;j++) {
			if (pts[i].s < pts[j].s) {
				point k = pts[i];
				pts[i] = pts[j];
				pts[j] = k;
			}
		}
	}

	for (int i = 0;i < N;i++) {
		points.push_back(pts[i]);
	}
}


void interest_points::Harris(image &img1, int N, int r, int p, double T) {
	image img = *(img1.Sobel());
	int width = img.getWidth();
	int height = img.getHeight();
	double *S = new double[width*height];
	double sum = 0;
	double *M = new double[r*r];
	image *M1 = new image(r, r);
	for (int x = r;x < width - r;x++) {
		for (int y = r;y < height - r;y++) {
			for (int u = 0;u < r;u++) {
				for (int v = 0;v < r;v++) {
					M1->setElement(u*r + v, img.getPixel(x + u, y + v));
				}
			}
			double lmin = M1->lambda();

			S[y*width + x] = lmin;
		}
	}
	delete M;
	delete M1;

	auto pts = std::make_unique<point[]>(width*height);
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

	for (int i = 0;i < ptscount - 1;i++) {
		for (int j = i + 1;j < ptscount;j++) {
			if (pts[i].s < pts[j].s) {
				point k = pts[i];
				pts[i] = pts[j];
				pts[j] = k;
			}
		}
	}

	for (int i = 0;i < N;i++) {
		points.push_back(pts[i]);
	}
}


interest_points::~interest_points()
{
}
