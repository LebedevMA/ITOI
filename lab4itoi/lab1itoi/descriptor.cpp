#include "stdafx.h"
#include "descriptor.h"


descriptor::descriptor()
{
}

descriptor::descriptor(const descriptor &a)
{
	V = a.V;
}

void descriptor::operator=(const descriptor &a)
{
	V = a.V;
}

double descriptor::Length()
{
	double len = 0;
	for (int i = 0;i < this->V.size();i++) {
		len += this->V[i] * this->V[i];
	}
	len = sqrt(len);
	return len;
}

void descriptor::Normalize()
{
	double len = this->Length();
	if (len > 0) {
		for (int i = 0;i < this->V.size();i++) {
			this->V[i] /= len;
		}
	}
}

void descriptor::Clamp(double k)
{
	for (int i = 0;i < this->V.size();i++) {
		if (this->V[i] > k) this->V[i] = k;
	}
}

std::unique_ptr<descriptor> descriptor::FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, const int R, const int FramesDividor, const int NumberOfBaskets){
	auto res = std::make_unique<descriptor>();

	int WindowHeight = 2 * R;
	int WindowWidth = 2 * R;

	for (int i = 0;i < FramesDividor*FramesDividor*NumberOfBaskets;i++) {
		res->V.push_back(0);
	}

	for (int i = -R+1;i < R;i++) {
		for (int j = -R+1;j < R;j++) {
			int x = i + pt.x;
			int y = j + pt.y;
			double x1 = Gx.getPixel(x, y);
			double y1 = Gy.getPixel(x, y);
			double phi = atan2(y1, x1);

			double r1 = hypot(x1,y1);

			double phi1 = phi-fmod(phi, 6.284 / NumberOfBaskets);

			double dist1 = phi - phi1;
			double dist2 = 6.284 / NumberOfBaskets - dist1;

			double weight1 = dist1 / (dist1 + dist2);
			double weight2 = dist2 / (dist1 + dist2);

			int idx = phi*(NumberOfBaskets - 1) / 6.284;

			int xx = (R + i)*(FramesDividor -1)/ WindowWidth;
			int yy = (R + j)*(FramesDividor -1)/ WindowHeight;
			int idx2 = xx*FramesDividor + yy;

			int index1 = idx2*NumberOfBaskets + idx;
			int index2 = idx2*NumberOfBaskets + idx+1;
			if (index1 < 0) index1 = 0;
			if (index2 < 0) index2 = 0;
			if (index1 > res->V.size() - 1) index1 = res->V.size() - 1;
			if (index2 > res->V.size() - 1) index2 = res->V.size() - 1;

			res->V[index1] += weight2*r1;
			res->V[index2] += weight1*r1;
		}
	}
	
	res->Normalize();
	res->Clamp(0.2);
	res->Normalize();

	return res;
}

double descriptor::Distance(const descriptor &a, const descriptor &b) {
	double res = 0;
	for (int i = 0;i < a.V.size() && i < b.V.size();i++) {
		double d = a.V[i] - b.V[i];
		res += d*d;
	}
	res = sqrt(res);
	return res;
}

std::unique_ptr<std::unique_ptr<descriptor>[]> descriptor::GetDescriptors(const interest_points &IP, const image &grad1, int N, int R) {
	auto D1 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	auto Gx1 = grad1.convolution(kernel::SobelKx());
	auto Gy1 = grad1.convolution(kernel::SobelKy());

	for (int i = 0;i < N;i++) {
		D1[i] = descriptor::FromPoint(*Gx1, *Gy1, IP.getPoint(i), R, 2, 8);
	}

	return D1;
}

std::unique_ptr<descriptor::line[]> descriptor::Connect(const interest_points &IP, const interest_points &IP2, std::unique_ptr<std::unique_ptr<descriptor>[]> & D1, std::unique_ptr<std::unique_ptr<descriptor>[]> & D2, int N)
{
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
}
