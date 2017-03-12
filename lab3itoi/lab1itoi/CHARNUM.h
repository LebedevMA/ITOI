#pragma once
#define MAXN 25
#define MAXK 5

namespace CHARNUM {
	bool is_nul(double d) {
		if (-0.0000001 < d && d < 0.0000001) return true;
		return false;
	}
	class matr {
	public:
		double v[MAXN][MAXN];
		int n, m;
		matr() {
			n = m = 0;
		}
		matr(int n, int m) {
			this->n = n;
			this->m = m;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					v[i][j] = 0;
				}
			}
		}
		matr(matr *A) {
			n = A->n;
			m = A->m;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					v[i][j] = A->v[i][j];
				}
			}
		}
		matr(matr &A) {
			n = A.n;
			m = A.m;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					v[i][j] = A.v[i][j];
				}
			}
		}
		matr *operator+ (matr &A) {
			matr *B = new matr(this);
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					B->v[i][j] += A.v[i][j];
				}
			}
			return B;
		}
		matr *operator- (matr &A) {
			matr *B = new matr(this);
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					B->v[i][j] -= A.v[i][j];
				}
			}
			return B;
		}
		matr *operator* (matr &A) {
			matr *B = new matr();
			B->n = n;
			B->m = A.m;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<A.m;j++) {
					double b = 0;
					for (int k = 0;k<m;k++) {
						b += (v[i][k])*(A.v[k][j]);
					}
					B->v[i][j] = b;
				}
			}
			return B;
		}
		double operator^ (matr &A) {
			double sum = 0;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					sum += (v[i][j])*(A.v[i][j]);
				}
			}
			return sum;
		}

		matr *operator* (double a) {
			matr *B = new matr(this);
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					B->v[i][j] *= a;
				}
			}
			return B;
		}
		matr *operator/ (double a) {
			matr *B = new matr(this);
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					B->v[i][j] /= a;
				}
			}
			return B;
		}

		matr *Minor(int i0, int j0) {
			matr *Min = new matr(n - 1, m - 1);
			for (int i = 0;i<n;i++) {
				int i1 = i;
				if (i1 == i0) continue;
				if (i1 > i0) i1--;
				for (int j = 0;j<m;j++) {
					int j1 = j;
					if (j1 == j0) continue;
					if (j1 > j0) j1--;
					Min->v[i1][j1] = v[i][j];
				}
			}
			return Min;
		}
		double det() {
			if (n == 0) return 0;
			if (n == 1) return v[0][0];
			double sum = 0;
			int sg = 1;
			for (int i = 0;i<n;i++) {
				matr *M = this->Minor(i, 0);
				double M1 = M->det();
				delete M;
				sum += sg*(v[i][0])*M1;
				sg *= -1;
			}
			return sum;
		}

		matr *Invert() {
			double detA = 0;
			matr *A1 = new matr(n, m);
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					int sg = 1 - 2 * ((i + j) % 2);
					matr *M = this->Minor(i, j);
					double M1 = sg*M->det();
					delete M;
					if (i == 0) detA -= M1;
					A1->v[j][i] = M1;
				}
			}
			if (is_nul(detA)) detA = 0.0001;
			matr *A1old = A1;
			A1 = (*A1)*(1.0 / detA);
			delete A1old;

			matr *KM = (*A1)*(*this);

			double kostyl = KM->v[0][0];
			if (is_nul(kostyl)) kostyl = 0.0001;
			*A1old = A1;
			A1 = (*A1)*(1.0 / kostyl);
			//delete A1old;

			return A1;
		}

		matr *T() {
			matr *A = new matr();
			A->n = m;
			A->m = n;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					A->v[j][i] = v[i][j];
				}
			}
			return A;
		}
		double norm() {
			double res = 0;
			for (int i = 0;i<n;i++) {
				for (int j = 0;j<m;j++) {
					if (v[i][j] > res) res = v[i][j];
					else if (-v[i][j] > res) res = -v[i][j];
				}
			}
			return res;
		}
		void movh(int i, int j) {
			for (int k = 0;k<m;k++) {
				double u = v[i][k];
				v[i][k] = v[j][k];
				v[j][k] = u;
			}
		}
		void movv(int i, int j) {
			for (int k = 0;k<n;k++) {
				double u = v[k][i];
				v[k][i] = v[k][j];
				v[k][j] = u;
			}
		}
	};

	double charnumA(matr *A, matr *X0, double e, double lam0) {
		if (is_nul(lam0)) lam0 = 0.00000001;
		int n = A->n;
		matr *E = new matr(n, n);
		for (int i = 0;i<n;i++) {
			E->v[i][i] = lam0;
		}
		matr *B1 = ((*A) - (*E));
		matr *B = B1->Invert();


		double lam1 = 0;
		double lam2 = 0;

		matr *X = new matr(X0);


		for (int i = 0;i<MAXK;i++) {
			matr *Xold = X;
			X = ((*B)*(*X));
			double d1 = ((*Xold) ^ (*Xold));
			double d2 = ((*X) ^ (*Xold));
			if (is_nul(d2)) d2 = 0.0001;
			lam2 = lam0 + d1 / d2;
			delete Xold;
			Xold = X;
			double Xnorm = X->norm();
			if (is_nul(Xnorm)) Xnorm = 0.0001;
			X = (*X) / Xnorm;
			delete Xold;

			if (lam1 - lam2 < e && lam1 - lam2 > -e && i > 0) break;
			lam1 = lam2;
		}

		delete B1;
		delete B;
		delete E;
		delete X;

		return lam2;
	}
}