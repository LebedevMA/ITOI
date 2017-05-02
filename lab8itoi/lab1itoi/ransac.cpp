#include "stdafx.h"
#include "ransac.h"


ransac::ransac()
{
}

ransac::matrix ransac::Apply(const Result & R, const ransac::matrix &X1)
{
	matrix X(X1);
	X.N = 3; X.V[2][0] = 1;

	matrix T3(3, 3);
	T3.V[0][0] = R.h00;T3.V[0][1] = R.h01;T3.V[0][2] = R.h02;
	T3.V[1][0] = R.h10;T3.V[1][1] = R.h11;T3.V[1][2] = R.h12;
	T3.V[2][0] = R.h20;T3.V[2][1] = R.h21;T3.V[2][2] = R.h22;

	X = multiply(T3, X); 

	return X;
}

ransac::matrix ransac::Apply(const Result & R, double x, double y)
{
	matrix X1(2, 1);
	X1.V[0][0] = x;
	X1.V[1][0] = y;
	return Apply(R,X1);
}

ransac::Result ransac::Hypothesis(const descriptor::line & Line, const descriptor::line & Line1, const descriptor::line & Line2, const descriptor::line & Line3)
{
	Result R;

	double x1 = Line.ptB.x;
	double y1 = Line.ptB.y;

	double x2 = Line.ptA.x;
	double y2 = Line.ptA.y;


	double x11 = Line1.ptB.x;
	double y11 = Line1.ptB.y;

	double x21 = Line1.ptA.x;
	double y21 = Line1.ptA.y;


	double x12 = Line2.ptB.x;
	double y12 = Line2.ptB.y;
	double x22 = Line2.ptA.x;
	double y22 = Line2.ptA.y;
	

	double x13 = Line3.ptB.x;
	double y13 = Line3.ptB.y;
	double x23 = Line3.ptA.x;
	double y23 = Line3.ptA.y;
	
	{
		matrix A(8, 9);
		A.V[0][0] = x1;
		A.V[0][1] = y1;
		A.V[0][2] = 1;
		A.V[0][6] = -x1*x2;
		A.V[0][7] = -y1*x2;
		A.V[0][8] = -x2;
		A.V[1][3] = x1;
		A.V[1][4] = y1;
		A.V[1][5] = 1;
		A.V[1][6] = -x1*y2;
		A.V[1][7] = -y1*y2;
		A.V[1][8] = -y2;
		A.V[2][0] = x11;
		A.V[2][1] = y11;
		A.V[2][2] = 1;
		A.V[2][6] = -x11*x21;
		A.V[2][7] = -y11*x21;
		A.V[2][8] = -x21;
		A.V[3][3] = x11;
		A.V[3][4] = y11;
		A.V[3][5] = 1;
		A.V[3][6] = -x11*y21;
		A.V[3][7] = -y11*y21;
		A.V[3][8] = -y21;
		A.V[4][0] = x12;
		A.V[4][1] = y12;
		A.V[4][2] = 1;
		A.V[4][6] = -x12*x22;
		A.V[4][7] = -y12*x22;
		A.V[4][8] = -x22;
		A.V[5][3] = x12;
		A.V[5][4] = y12;
		A.V[5][5] = 1;
		A.V[5][6] = -x12*y22;
		A.V[5][7] = -y12*y22;
		A.V[5][8] = -y22;
		A.V[6][0] = x13;
		A.V[6][1] = y13;
		A.V[6][2] = 1;
		A.V[6][6] = -x13*x23;
		A.V[6][7] = -y13*x23;
		A.V[6][8] = -x23;
		A.V[7][3] = x13;
		A.V[7][4] = y13;
		A.V[7][5] = 1;
		A.V[7][6] = -x13*y23;
		A.V[7][7] = -y13*y23;
		A.V[7][8] = -y23;

			matrix B(9, 1);
			B.V[0][0] = 0;
			B.V[1][0] = 0;
			B.V[2][0] = 0;
			B.V[3][0] = 0;
			B.V[4][0] = 0;
			B.V[5][0] = 0;
			B.V[6][0] = 0;
			B.V[7][0] = 0;
			B.V[8][0] = 0;

			matrix t = SVD(A);

			double k = 1.0 / t.V[8][0];

			R.h00 = k*t.V[0][0];
			R.h01 = k*t.V[1][0];
			R.h02 = k*t.V[2][0];
			R.h10 = k*t.V[3][0];
			R.h11 = k*t.V[4][0];
			R.h12 = k*t.V[5][0];
			R.h20 = k*t.V[6][0];
			R.h21 = k*t.V[7][0];
			R.h22 = k*t.V[8][0];
	}

	return R;
}

ransac::Result ransac::Run(const std::vector<descriptor::line>& lines, double T)
{
	System::Random ^rand = gcnew System::Random();

	std::vector<variant> variants;

	for (int i = 0;i < lines.size();i++) {
		int idx1, idx2, idx3, idx4;
		idx1 = rand->Next(0, lines.size());
		do {
			idx2 = rand->Next(0, lines.size());
		} while (idx2 == idx1);
		do {
			idx3 = rand->Next(0, lines.size());
		} while (idx3 == idx1 || idx3 == idx2);
		do {
			idx4 = rand->Next(0, lines.size());
		} while (idx4 == idx1 || idx4 == idx2 || idx4 == idx3);


		descriptor::line Line = lines[idx1];
		descriptor::line Line1 = lines[idx2];
		descriptor::line Line2 = lines[idx3];
		descriptor::line Line3 = lines[idx4];
		Result R = Hypothesis(Line, Line1, Line2, Line3);
		int inliers = 0;
		for (int j = 0;j < lines.size();j++) {
			matrix X(2, 1);
			X.V[0][0] = lines[j].ptB.x;
			X.V[1][0] = lines[j].ptB.y;
			matrix X1 = Apply(R, X);
			double d = 0;
			d += (X1.V[0][0] - lines[j].ptA.x) * (X1.V[0][0] - lines[j].ptA.x);
			d += (X1.V[1][0] - lines[j].ptA.y) * (X1.V[1][0] - lines[j].ptA.y);
				d = sqrt(d);
			if (d <= T) {
				inliers++;
			}
		}
		variants.push_back(variant(R, inliers));
	}

	std::sort(variants.begin(), variants.end(), [](const variant &a, const variant &b) {return a.inliers > b.inliers;});

	return variants[0].R;
}


ransac::~ransac()
{
}
