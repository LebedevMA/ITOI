#include "stdafx.h"
#include "descriptor.h"


descriptor::descriptor()
{
	V = nullptr;
	size = 0;
}

descriptor::descriptor(descriptor &a)
{
	size = a.size;
	if (size > 0) {
		V = new Gradient[a.size];
		for (int i = 0;i < a.size;i++) {
			V[i] = a.V[i];
		}
	}
	else {
		V = nullptr;
	}
}

void descriptor::operator=(descriptor &a)
{
	size = a.size;
	if (size > 0) {
		V = new Gradient[a.size];
		for (int i = 0;i < a.size;i++) {
			V[i] = a.V[i];
		}
	}
	else {
		V = nullptr;
	}
}

std::unique_ptr<descriptor> descriptor::FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, const int R, const int N){
	auto res = std::make_unique<descriptor>();
	res->V = new Gradient[N];
	res->size = N;

	double max = 0;

	for (int i = -R+1;i < R;i++) {
		for (int j = -R+1;j < R;j++) {
			int x = i + pt.x;
			int y = j + pt.y;
			double x1 = Gx.getPixel(x, y);
			double y1 = Gy.getPixel(x, y);
			double phi = atan(y1 / x1);
			double r1 = sqrt(x1*x1 + y1*y1);
			int idx = (1.571+phi)*8 / 3.142;
			int idx2 = N*((R+i) * 2 * R + (R+j)) / (4 * R*R);
			res->V[idx2].V[idx] += r1;
			if (r1 > max) max = r1;
		}
	}
	if (max > 0) {
		for (int i = 0;i < N;i++) {
			for (int j = 0;j < 8;j++) {
				res->V[i].V[j] /= max;
			}
		}
	}
	return res;
}

double descriptor::Distance(const descriptor &a, const descriptor &b) {
	double res = 0;
	for (int i = 0;i < a.size && i < b.size;i++) {
		double d = Gradient::Distance(a.V[i], b.V[i]);
		res += d*d;
	}
	return res;
}

std::unique_ptr<descriptor::line[]> descriptor::Connect(const interest_points & IP, const interest_points & IP2, const image & grad1, const image & grad2, int N, int R)
{
	auto D1 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	auto D2 = std::make_unique < std::unique_ptr<descriptor>[]>(N);

	auto Gx1 = grad1.convolution(kernel::SobelKx());
	auto Gy1 = grad1.convolution(kernel::SobelKy());
	auto Gx2 = grad2.convolution(kernel::SobelKx());
	auto Gy2 = grad2.convolution(kernel::SobelKy());

	for (int i = 0;i < N;i++) {
		D1[i] = descriptor::FromPoint(*Gx1, *Gy1, IP.getPoint(i), R, R/2);
		D2[i] = descriptor::FromPoint(*Gx2, *Gy2, IP2.getPoint(i), R, R/2);
	}

	auto lines = std::make_unique < line[]>(N*N);

	for (int i = 0;i < N;i++) {
		interest_points::point a = IP.getPoint(i);
		for (int j = 0;j < N;j++) {
			interest_points::point b = IP2.getPoint(j);
			double d = descriptor::Distance(*D1[i], *D2[j]);
			lines[i*N + j] = line(i, j, d, a, b);
		}
	}

	for (int i = 0;i < N*N - 1;i++) {
		for (int j = i + 1;j < N*N;j++) {
			if (lines[i].d > lines[j].d) {
				line k = lines[i];
				lines[i] = lines[j];
				lines[j] = k;
			}
		}
	}

	auto flags = std::make_unique<double[]>(2 * N);
	for (int i = 0;i < 2 * N;i++) {
		flags[i] = true;
	}

	auto result = std::make_unique<line[]>(N);

	for (int i = 0;i < N*N;i++) {
		if (flags[lines[i].a] == false) continue;
		if (flags[N + lines[i].b] == false) continue;
		flags[lines[i].a] = false;
		flags[N + lines[i].b] = false;
		result[lines[i].a] = lines[i];
	}

	return result;
}

descriptor::~descriptor()
{
	delete V;
}
