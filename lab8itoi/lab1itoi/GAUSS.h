#pragma once

#define MAXN 25

namespace GAUSS {

	class matr {
		bool is_nul(double d) {
		if (-0.0000001 < d && d < 0.0000001) return true;
		return false;
	}
	public:
		int n, m;
		double op;
		double a[MAXN][MAXN];
		int beg;

		int sign;

		matr() {
			sign = 1;
			n = m = beg = 0;
			for (int i = 0;i < MAXN;i++) {
				for (int j = 0;j < MAXN;j++) {
					a[i][j] = 0;
				}
			}
		}
		matr(int n0, int m0) {
			sign = 1;
			n = n0;
			m = m0;
			beg = 0;
			for (int i = 0;i < MAXN;i++) {
				for (int j = 0;j < MAXN;j++) {
					a[i][j] = 0;
				}
			}
		}
		matr(matr *x) {
			sign = x->sign;
			n = x->n;
			m = x->m;
			beg = x->beg;
			for (int i = 0;i < MAXN;i++) {
				for (int j = 0;j < MAXN;j++) {
					a[i][j] = x->a[i][j];
				}
			}
		}

		double det() {
			double d = sign;
			for (int i = 0;i < n;i++) {
				d *= a[i][i];
			}
			return d;
		}
		matr *minor(int k) {
			matr *M = new matr(this);
			for (int i = k;i < n - 1;i++) {
				M->movh(i, i + 1);
			}
			for (int i = 0;i < n - 1;i++) {
				M->movv(i, i + 1);
			}
			M->n--;
			M->m--;
			return M;
		}
		double det1() {
			if (n == 0) return 0;
			if (n == 1) return a[0][0];
			double d = 0;
			int s = 1;
			for (int i = 0;i < n;i++) {
				matr *M = this->minor(i);
				double b = M->det1();
				d += s*(a[0][i])*b;
				s *= -1;
			}
			return d;
		}

		double get(int i, int j) {
			return a[i + beg][j + beg];
		}
		void set(double x, int i, int j) {
			a[i + beg][j + beg] = x;
		}
		void movh(int i, int j) {
			for (int k = 0;k < n;k++) {
				double b = a[k][beg + i];
				a[k][beg + i] = a[k][beg + j];
				a[k][beg + j] = b;
			}
		}
		void movv(int i, int j) {
			for (int k = 0;k < n;k++) {
				double b = a[beg + i][k];
				a[beg + i][k] = a[beg + j][k];
				a[beg + j][k] = b;
			}
		}
		void subs(int k, matr *B) {
			if (a[k + beg][beg] == 0) return;
			for (int i = beg;i < n;i++) {
				if (i == k + beg) continue;
				if (is_nul(a[i][beg])) continue;
				double d = a[i][beg] / a[k + beg][beg];
				a[i][beg] = 0;
				for (int j = beg + 1;j < m;j++) {
					a[i][j] -= a[k + beg][j] * d;
				}
				if (B->n == 1) B->a[i][0] -= B->a[k + beg][0] * d;
				else {
					for (int j = 0;j < B->n;j++) {
						B->a[i][j] -= B->a[k + beg][j] * d;
					}
				}
			}
		}

		void gauss_step1(matr *B, matr *order) {
			sign = 1;
			while (beg < n) {
				if (order != nullptr) {
					double max = a[beg][beg];
					int j = 0;
					for (int i = beg;i < m;i++) {
						if (is_nul(a[i][beg])) continue;
						int sg = 1;
						if (a[i][beg] < 0) sg = -1;
						if (sg*a[i][beg] > max || is_nul(max)) {
							max = sg*a[i][beg];
							j = i - beg;
						}
					}

					if (j != 0) {
						movv(0, j);
						B->movv(beg, beg + j);
						sign *= -1;
						//	order->movv(beg,beg+j);
					}
				}

				subs(0, B);

				beg++;
			}
			beg = 0;
		}
		matr *gauss_step2(matr *B) {
			matr *X = new matr(1, n);
			X->a[n - 1][0] = (B->a[n - 1][0]) / (a[n - 1][n - 1]);
			for (int i = n - 2;i >= 0;i--) {
				double xi = B->a[i][0];
				for (int j = i + 1;j < n;j++) {
					xi -= (X->a[j][0])*(a[i][j]);
				}
				xi /= a[i][i];
				X->a[i][0] = xi;
			}
			return X;
		}

		matr *GAUSS(matr *B1) {
			matr *A = new matr(this);
			matr *B = new matr(B1);
			matr *X = new matr(n, 1);
			matr *order = new matr(n, 1);
			for (int i = 0;i < n;i++) {
				order->set(i, i, 0);
			}

			A->gauss_step1(B, order);

			double det = A->det();
			if (is_nul(det)) {
				op = 0;
				delete A;
				delete B;
				delete X;
				delete order;
				return nullptr;
			}
			else {

				op = det;
				matr *X1 = A->gauss_step2(B);
				matr *X = new matr(n, 1);
				for (int i = 0;i < n;i++) {
					X->set(X1->a[i][0], order->a[i][0], 0);
				}
				delete A;
				delete B;
				delete order;
				return X;
			}
		}

		matr *INVERT() {
			matr *A = new matr(this);
			matr *A1 = new matr(n, n);
			matr *E = new matr(n, n);

			for (int i = 0;i < n;i++) {
				E->set(1, i, i);
			}
			A->gauss_step1(E, nullptr);

			if (A->det() == 0) {
				delete A;
				delete A1;
				delete E;
				return nullptr;
			}

			for (int i = 0;i < n;i++) {
				matr *B = new matr(n, 1);
				for (int j = 0;j < n;j++) {
					B->set(E->a[j][i], j, 0);
				}
				matr *X1 = A->gauss_step2(B);
				for (int j = 0;j < n;j++) {
					A1->set(X1->a[j][0], j, i);
				}
				delete B;
				delete X1;
			}
			delete E;
			delete A;
			return A1;
		}
	};

}