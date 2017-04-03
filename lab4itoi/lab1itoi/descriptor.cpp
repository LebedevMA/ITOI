#include "stdafx.h"
#include "descriptor.h"


descriptor::descriptor()
{
	V = nullptr;
	size = 0;
}

descriptor::descriptor(descriptor &a)
{
	V = new double[a.size];
	size = a.size;
	for (int i = 0;i < a.size;i++) {
		V[i] = a.V[i];
	}
}

void descriptor::operator=(descriptor &a)
{
	V = new double[a.size];
	size = a.size;
	for (int i = 0;i < a.size;i++) {
		V[i] = a.V[i];
	}
}

std::unique_ptr<descriptor> descriptor::FromPoint(const image &img, const interest_points::point &pt, const int R) {
	std::unique_ptr<descriptor> res = std::make_unique<descriptor>();
	res->V = new double[R];
	res->size = R;
	int *count = new int[R];
	for (int i = 0;i < R;i++) {
		res->V[i] = 0;
		count[i] = 0;
	}

	for (int i = -R+1;i < R;i++) {
		for (int j = -R+1;j < R;j++) {
			int x = i + pt.x;
			int y = j + pt.y;
			int d = (int)(sqrt(i*i+j*j));
			if (d >= R) continue;
			res->V[d] += img.getPixel(x, y);
			count[d]++;
		}
	}
	double weight = 1;
	for (int i = 0;i < R;i++) {
		if (count[i] == 0) continue;
		res->V[i] /= count[i];
		res->V[i] *= weight;
		weight *= 1.2;
	}
	delete count;
	return res;
}

double descriptor::Distance(const std::unique_ptr<descriptor> &a, const std::unique_ptr<descriptor> &b) {
	double res = 0;
	for (int i = 0;i < a->size && i < b->size;i++) {
		double d = a->V[i] - b->V[i];
		res += d*d;
	}
	return res;
}

std::unique_ptr<descriptor::line[]> descriptor::Connect(const interest_points & IP, const interest_points & IP2, const image & grad1, const image & grad2, int N, int R)
{
	auto D1 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	auto D2 = std::make_unique < std::unique_ptr<descriptor>[]>(N);

	for (int i = 0;i < N;i++) {
		D1[i] = descriptor::FromPoint(grad1, IP.getPoint(i), R);
		D2[i] = descriptor::FromPoint(grad2, IP2.getPoint(i), R);
	}

	auto lines = std::make_unique < line[]>(N*N);

	for (int i = 0;i < N;i++) {
		interest_points::point a = IP.getPoint(i);
		for (int j = 0;j < N;j++) {
			interest_points::point b = IP2.getPoint(j);
			double d = descriptor::Distance(D1[i], D2[j]);
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
