#pragma once
#include "descriptor.h"
#include <vector>
#include <stdio.h>
class ransac
{
public:
	struct matrix {
		double V[10][10];
		int N, M;
		matrix() { N = 0;M = 0; }
		matrix(int N, int M) { 
			this->N = N;this->M = M;
			for (int i = 0;i < N;i++) {
				for (int j = 0;j < M;j++) {
					V[i][j] = 0;
				}
			}
		}
		matrix(const matrix &a) {
			this->N = a.N;
			this->M = a.M;
			for (int i = 0;i < a.N;i++) {
				for (int j = 0;j < a.M;j++) {
					V[i][j] = a.V[i][j];
				}
			}
		}
	};
	struct Result {
		double h00, h01, h02, h10, h11, h12, h20, h21, h22;
		Result() {
			this->h00 = 0;
			this->h01 = 0;
			this->h02 = 0;
			this->h10 = 0;
			this->h11 = 0;
			this->h12 = 0;
			this->h20 = 0;
			this->h21 = 0;
			this->h22 = 0;
		}
		Result(const Result &R) {
			this->h00 = R.h00;
			this->h01 = R.h01;
			this->h02 = R.h02;
			this->h10 = R.h10;
			this->h11 = R.h11;
			this->h12 = R.h12;
			this->h20 = R.h20;
			this->h21 = R.h21;
			this->h22 = R.h22;
		}
	};
	struct variant {
		Result R;
		int inliers;
		variant(Result R, int inliers) {
			this->R = R;
			this->inliers = inliers;
		}
		variant(const variant &a) {
			this->R = a.R;
			this->inliers = a.inliers;
		}
	};
	ransac();


	static matrix add(const matrix &A, const matrix &B) {
		matrix res(A.M, A.N);
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < A.M;j++) {
				res.V[j][i] = A.V[i][j] + B.V[i][j];
			}
		}
		return res;
	}
	static matrix transp(const matrix &A) {
		matrix res(A.M, A.N);
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < A.M;j++) {
				res.V[j][i] = A.V[i][j];
			}
		}
		return res;
	}
	static matrix multiply(const matrix &A, double k) {
		matrix res(A.N, A.M);
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < A.M;j++) {
				res.V[i][j] = A.V[i][j] * k;
			}
		}
		return res;
	}
	static matrix multiply(const matrix &A, const matrix &B) {
		matrix C(A.N, B.M);
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < B.M;j++) {
				C.V[i][j] = 0;
				for (int k = 0;k < A.M;k++) {
					C.V[i][j] += A.V[i][k] * B.V[k][j];
				}
			}
		}
		return C;
	}
	static matrix minor(const matrix &A, int i0, int j0) {
		matrix res(A.N-1, A.M-1);
		for (int i = 0;i < A.N;i++) {
			if (i == i0) continue;
			int i1 = i; if (i > i0) i1 = i - 1;
			for (int j = 0;j < A.M;j++) {
				if (j == j0) continue;
				int j1 = j; if (j > j0) j1 = j - 1;
				res.V[i1][j1] = A.V[i][j];
			}
		}
		return res;
	}
	static double det(const matrix &A) {
		if (A.N == 1) return A.V[0][0];
		double sum = 0;
		int sign = 1;
		for (int j = 0;j < A.M;j++) {
			sum += sign * A.V[0][j] * det(minor(A, 0, j));
			sign *= -1;
		}
		return sum;
	}
	static matrix inverse(const matrix &A) {
		matrix res(A.N, A.M);

		double detA = det(A);

		int sign1 = 1;
		int sign2 = 1;
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < A.M;j++) {
				res.V[i][j] = sign1 * sign2 * det(minor(A, i, j));
				sign2 *= -1;
			}
			sign1 *= -1;
		}

		res = transp(res);
		res = multiply(res, 1.0 / detA);

		return res;
	}
	static matrix SVD(const matrix &A) {
		DotNetMatrix::GeneralMatrix ^AA = gcnew DotNetMatrix::GeneralMatrix(A.N, A.M);
		for (int i = 0;i < A.N;i++) {
			for (int j = 0;j < A.M;j++) {
				AA->SetElement(i, j, A.V[i][j]);
			}
		}

		DotNetMatrix::GeneralMatrix ^AT = AA->Transpose();

		DotNetMatrix::GeneralMatrix ^ATA = AT->Multiply(AA);

		DotNetMatrix::SingularValueDecomposition ^svd = gcnew DotNetMatrix::SingularValueDecomposition(ATA);
		double minSV = svd->SingularValues[0];
		int idx = 0;
		for (int i = 1;i < svd->SingularValues->Length;i++) {
			if (svd->SingularValues[i] < minSV) {
				minSV = svd->SingularValues[i];
				idx = i;
			}
		}

		matrix X(A.M, 1);

		for (int i = 0;i < A.M;i++) {
			X.V[i][0] = svd->GetU()->GetElement(i, idx);
		}

		return X;
	}


	static matrix Apply(const Result &R, const matrix &X1);
	static matrix Apply(const Result &R, double x, double y);
	

	static Result Hypothesis(const descriptor::line &Line, const descriptor::line & Line1, const descriptor::line & Line2, const descriptor::line & Line3);
	static Result Run(const std::vector<descriptor::line> &lines, double T);
	~ransac();
};

