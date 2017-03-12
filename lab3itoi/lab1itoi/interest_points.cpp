#include "stdafx.h"
#include "interest_points.h"
#include "CHARNUM.h"
#include <map>


interest_points::interest_points()
{
}

void interest_points::Moravek(image &img, int N, int r, int p) {
	int width = img.getWidth();
	int height = img.getHeight();
	double *S = new double[width*height];
	double sum = 0;
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

			S[y*width + x] = S1[0];
			for (int i = 1;i < 8;i++) {
				if (S1[i] < S[y*width + x]) S[y*width + x] = S1[i];
			}
			sum += S[y*width + x];
		}
	}

	std::map<double,point> pts;

	for (int x = r+p;x < width-r-p;x++) {
		for (int y = r+p;y < height-r-p;y++) {
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
				pts.insert(std::pair<double,point>(pt.s, pt));
			}
		}
	}
	
	std::map<double,point>::reverse_iterator it = pts.rbegin();
	for (int i = 0;i < N;i++) {
		points.push_back(it->second);
		it++;
	}
	it = pts.rbegin();

	delete S;
}

void interest_points::Harris(image &img, int N, int r, int p) {
	int width = img.getWidth();
	int height = img.getHeight();
	double *S = new double[width*height];
	double sum = 0;
	double *M = new double[r*r];
	for (int x = r;x < width - r;x++) {
		for (int y = r;y < height - r;y++) {
			for (int u = 0;u < r;u++) {
				for (int v = 0;v < r;v++) {
					M[u*r + v] = img.getPixel(x + u, y + v);
				}
			}
			double lmin = 0;
			try {
				lmin = lmbdmin(M, r);
			}
			catch (...) {}
			S[y*width + x] = lmin;
		}
	}
	delete M;

	std::map<double, point> pts;

	for (int x = r + p;x < width - r - p;x++) {
		for (int y = r + p;y < height - r - p;y++) {
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
				pts.insert(std::pair<double, point>(pt.s, pt));
			}
		}
	}

	std::map<double, point>::reverse_iterator it = pts.rbegin();
	for (int i = 0;i < N;i++) {
		points.push_back(it->second);
		it++;
	}
	it = pts.rbegin();

	delete S;
}

double interest_points::lmbdmin(double *m, int n) {
	CHARNUM::matr M(n, n);
	CHARNUM::matr X(n, 1);
	for (int i = 0;i < n;i++) {
		X.v[i][0] = i+1;
		for (int j = 0;j < n;j++) {
			M.v[i][j] = m[i*n + j];
		}
	}
	double lmbd = CHARNUM::charnumA(&M, &X, 0.0001, 0);
	return lmbd;
}

interest_points::~interest_points()
{
}
