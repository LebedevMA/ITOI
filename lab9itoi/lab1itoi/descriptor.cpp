#include "stdafx.h"
#include "descriptor.h"


descriptor::descriptor()
{
}

descriptor::descriptor(const descriptor &a)
{
	V = a.V;
	orientation = a.orientation;
	centerAngle = a.centerAngle;
	centerDistance = a.centerDistance;
	size = a.size;
}

void descriptor::operator=(const descriptor &a)
{
	V = a.V;
	orientation = a.orientation;
	centerAngle = a.centerAngle;
	centerDistance = a.centerDistance;
	size = a.size;
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

double descriptor::PointOrientation(const image & Gx, const image & Gy, const interest_points::point & pt,  int R0, double scale, double sigma0, double sigma1)
{
	int R = R0 * sigma1 / sigma0;
	int NumberOfBaskets = 36;
	auto M = std::make_unique<angle[]>(NumberOfBaskets);
	double step = 6.284 / NumberOfBaskets;
	for (int i = 0;i < NumberOfBaskets;i++) {
		M[i] = angle(step*i, 0);
	}
	for (int i = -R + 1;i < R;i++) {
		for (int j = -R + 1;j < R;j++) {
			if (hypot(i, j) >= R) continue;

			int x = i + pt.x*scale;
			int y = j + pt.y*scale;

			double x1 = Gx.getPixel(x, y);
			double y1 = Gy.getPixel(x, y);
			double phi = 6.284+atan2(y1, x1);
			phi = fmod(phi, 6.284);

			double r1 = hypot(x1, y1);

			double phi1 = phi - fmod(phi, step);

			double dist1 = phi - phi1;
			double dist2 = step - dist1;

			double weight1 = dist1 / (dist1 + dist2);
			double weight2 = dist2 / (dist1 + dist2);

			int idx = (NumberOfBaskets - 1)*phi1/6.284 + 0.5;
			int idx2 = idx+1;

			if (idx < 0) idx = NumberOfBaskets - 1;
			if (idx > NumberOfBaskets - 1) idx = 0;
			if (idx2 < 0) idx2 = NumberOfBaskets - 1;
			if (idx2 > NumberOfBaskets - 1) idx2 = 0;

			M[idx].weight += weight2*r1;
			M[idx2].weight += weight1*r1;
		}
	}

	std::vector<angle> ex;

	for (int i = 0;i < NumberOfBaskets;i++) {
		if (i > 0 && M[i - 1].weight > M[i].weight) continue;
		if (i < (NumberOfBaskets - 1) && M[i + 1].weight > M[i].weight) continue;
		if (i == 0 && M[NumberOfBaskets-1].weight > M[i].weight) continue;
		if (i == (NumberOfBaskets - 1) && M[0].weight > M[i].weight) continue;
		ex.push_back(M[i]);
	}

	std::sort(ex.begin(), ex.end(), [](const angle &a, const angle &b) {return a.weight > b.weight;});

	return ex[0].phi;
}

std::unique_ptr<descriptor> descriptor::FromPoint(const image &Gx, const image &Gy, const interest_points::point &pt, int R0, const int FramesDividor, const int NumberOfBaskets, double scale, double sigma0, double sigma1){
	int R = R0 * sigma1 / sigma0;
	auto res = std::make_unique<descriptor>();

	int WindowHeight = 2 * R;
	int WindowWidth = 2 * R;

	int FrameHeight = WindowHeight / FramesDividor;
	int FrameWidth = WindowWidth / FramesDividor;

	double phi0 = PointOrientation(Gx,Gy,pt,R0,scale,sigma0,sigma1);
	res->orientation = phi0;

	res->size = sigma1/sigma0*scale;

	for (int i = 0;i < FramesDividor*FramesDividor*NumberOfBaskets;i++) {
		res->V.push_back(0);
	}

	double step = 6.284 / NumberOfBaskets;

	for (int i = -R+1;i < R;i++) {
		for (int j = -R+1;j < R;j++) {
			int x = i + pt.x*scale;
			int y = j + pt.y*scale;

			double x1 = Gx.getPixel(x, y);
			double y1 = Gy.getPixel(x, y);
			double phi = 6.284+atan2(y1, x1)-phi0;
			phi = fmod(phi, 6.284);

			double r1 = hypot(x1,y1)*hypot(i, j);

			double phi1 = phi-fmod(phi, step);

			double dist1 = phi - phi1;
			double dist2 = step - dist1;

			double weight1 = dist1 / (dist1 + dist2);
			double weight2 = dist2 / (dist1 + dist2);

			int idx = phi1/step;
			int idx1 = idx + 1;
			idx %= NumberOfBaskets;
			idx1 %= NumberOfBaskets;
			
			int ii = i*cos(phi0) + j*sin(phi0) + 0.5;
			int jj = -i*sin(phi0) + j*cos(phi0) + 0.5;

			int xx = (R + ii)*(FramesDividor -1)/ (double)WindowWidth;
			int yy = (R + jj)*(FramesDividor -1)/ (double)WindowHeight;

			int idxH1 = (xx%FramesDividor)*FramesDividor + yy%FramesDividor;
			int idxH2 = ((xx + 1) % FramesDividor)*FramesDividor + yy%FramesDividor;
			int idxH3 = (xx%FramesDividor)*FramesDividor + (yy+1)% FramesDividor;
			int idxH4 = ((xx+1) % FramesDividor)*FramesDividor + (yy+1)% FramesDividor;

			idxH1 %= FramesDividor*FramesDividor;
			idxH2 %= FramesDividor*FramesDividor;
			idxH3 %= FramesDividor*FramesDividor;
			idxH4 %= FramesDividor*FramesDividor;

			double weightHX = (FrameWidth-x%FrameWidth) / (double)FrameWidth;
			double weightHY = (FrameHeight-y%FrameHeight) / (double)FrameHeight;

			double weightH1 = weightHX*weightHY;
			double weightH2 = (1 - weightHX)*weightHY;
			double weightH3 = weightHX*(1 - weightHY);
			double weightH4 = (1 - weightHX)*(1 - weightHY);
			
			int index11 = idxH1*NumberOfBaskets + idx; index11 %= res->V.size();
			int index12 = idxH1*NumberOfBaskets + idx1; index12 %= res->V.size();
			int index21 = idxH2*NumberOfBaskets + idx; index21 %= res->V.size();
			int index22 = idxH2*NumberOfBaskets + idx1; index22 %= res->V.size();
			int index31 = idxH3*NumberOfBaskets + idx; index31 %= res->V.size();
			int index32 = idxH3*NumberOfBaskets + idx1; index32 %= res->V.size();
			int index41 = idxH4*NumberOfBaskets + idx; index41 %= res->V.size();
			int index42 = idxH4*NumberOfBaskets + idx1; index42 %= res->V.size();
			
			res->V[index11] += weightH1*weight2*r1;
			res->V[index12] += weightH1*weight1*r1;
			res->V[index21] += weightH2*weight2*r1;
			res->V[index22] += weightH2*weight1*r1;
			res->V[index31] += weightH3*weight2*r1;
			res->V[index32] += weightH3*weight1*r1;
			res->V[index41] += weightH4*weight2*r1;
			res->V[index42] += weightH4*weight1*r1;
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

std::unique_ptr<std::unique_ptr<descriptor>[]> descriptor::GetDescriptors(interest_points &IP, int N, int R) {
	auto Gx = std::make_unique<std::unique_ptr<image>[]>(IP.getCount());
	auto Gy = std::make_unique<std::unique_ptr<image>[]>(IP.getCount());

	auto D1 = std::make_unique < std::unique_ptr<descriptor>[]>(N);
	
	for (int i = 0;i < IP.P.getCount();i++) {
		Gx[i] = IP.P.getImage(i).convolution(kernel::SobelKx());
		Gy[i] = IP.P.getImage(i).convolution(kernel::SobelKy());
	}

	for (int i = 0;i < N;i++) {
		double scale = IP.P.getScale(IP.getPoint(i).z);
		double sigma0 = IP.P.getInfo(0).sigma;
		double sigma1 = IP.P.getInfo(IP.getPoint(i).z).sigma;
		D1[i] = descriptor::FromPoint(*Gx[IP.getPoint(i).z], *Gy[IP.getPoint(i).z], IP.getPoint(i), R, 8, 8, scale,sigma0,sigma1);
	}

	return D1;
}

std::unique_ptr<std::vector<descriptor::line>> descriptor::Connect(const interest_points &IP, const interest_points &IP2, std::unique_ptr<std::unique_ptr<descriptor>[]> & D1, std::unique_ptr<std::unique_ptr<descriptor>[]> & D2, int N, double T)
{
	std::vector<line> lines;

	for (int i = 0;i < N;i++) {
		interest_points::point a = IP.getPoint(i);
		for (int j = 0;j < N;j++) {
			interest_points::point b = IP2.getPoint(j);
			double d = descriptor::Distance(*D1[i], *D2[j]);
			if (d > T) continue;
			lines.push_back(line(i, j, d, a, b, *D1[i], *D2[j]));
		}
	}

	std::sort(lines.begin(), lines.end(), [](const line &a, const line &b) {return a.d < b.d;});

	auto flags = std::make_unique<double[]>(2 * N);
	for (int i = 0;i < 2 * N;i++) {
		flags[i] = true;
	}

	std::unique_ptr<std::vector<line>> result = std::make_unique<std::vector<line>>();

	for (int i = 0;i < lines.size();i++) {
		if (flags[lines[i].a] == false) continue;
		if (flags[N + lines[i].b] == false) continue;
		flags[lines[i].a] = false;
		flags[N + lines[i].b] = false;
		result->push_back(lines[i]);
	}

	return result;
}

descriptor::~descriptor()
{
}
