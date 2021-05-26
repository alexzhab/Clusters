// Copyright Zhabitskaya A.
// 2021

#include "Interpolation_alg.h"
#include "K_means_alg.h"
#define NN 10


int Interpolation::Start(Vector P, vector <Vector>& All_Point)
{
	Delaunay_triangulation A_T;
	for (int i = 0; i < All_Point.size(); i++)
	{
		double xx = All_Point[i].GetX() * All_Point[i].GetX();
		double yy = All_Point[i].GetY() * All_Point[i].GetY();
		double s = (xx + yy);
		All_Point[i].Set_F(sin(s));
	}
	Triang_Delon Triangle = A_T.Start(All_Point);// Дописать класс

	for (int i = 0; i < Triang.Get_N(); i++)
	{
		if (Triangle.Get_Triangle(i).Belongs_P(P) == 1)
		{
			double S, v;
			vector <double> p;
			S = v = 0;
			cout << "P=( " << P.GetX() << ", " << P.GetY() << ")" << " F(P)=" << sin(3 * (P.GetX() * P.GetX() + P.GetY() * P.GetY())) << endl;
			for (int j = 0; j < 3; j++)
			{
				Point PP;
				PP = Triangle.Get_Triangle(i).Get_P(j);
				cout << "PP=( " << PP.GetX() << ", " << PP.GetY() << ") F(PP)=" << PP.Get_F() << endl;
				p.push_back(exp((-1) * d(PP, P) * d(PP, P) * 0.5));
				cout << "p[" << j << "]=" << p[j] << " d(PP, P)=" << d(PP, P) << endl << endl;
				S += PP.Get_F() * p[j];
				v += p[j];
			}
			cout << "S=" << S << "v=" << v << endl;
			S = S / v;
			P.Set_F(S);
			double xx = P.GetX() * P.GetX();
			double yy = P.GetY() * P.GetY();
			double s = (xx + yy);
			cout << "Oshibka = " << S - sin(s) << endl;

			return 1;
		}
	}
	return -1;
}

double Interpolation::Evaluation(vector <Vector>& All_Point)
{
	vector <double> Eps;
	Delaunay_triangulation A_T;
	double F_s, S_e, S_y, R;

	for (int i = 0; i < All_Point.size(); i++)
	{
		double xx = All_Point[i].GetX() * All_Point[i].GetX();
		double yy = All_Point[i].GetY() * All_Point[i].GetY();
		double s = (xx + yy);
		All_Point[i].Set_F(sin(s));
	}
	for (int n = 0; n < NN; n++)
	{
		vector <Point> Points;
		for (int i = 0; i < All_Point.size(); i++)
		{
			if (i != n) Points.push_back(All_Point[i]);
		}

		Point P = All_Point[n];
		Interpolation A_Interpolation;
		int err = A_Interpolation.Start(P, Points);
		if (err == 1) Eps.push_back(P.Get_F() - All_Point[n].Get_F());
		if (err == -1) Eps.push_back(0);
	}
	F_s = S_e = S_y = 0;
	for (int i = 0; i < All_Point.size(); i++)
	{
		F_s += All_Point[i].Get_F();
	}
	F_s = F_s / All_Point.size();
	for (int i = 0; i < NN; i++)
	{
		S_e += Eps[i] * Eps[i];
		S_y += (All_Point[i].Get_F() - F_s) * (All_Point[i].Get_F() - F_s);
	}
	R = 1 - (S_e / S_y);
	cout << "R= " << R << endl;
	return R;
}

double Interpolation::d(Vector& A, Vector& B)
{
	double xx, yy;
	double x_1 = A.GetX();
	double x_2 = B.GetX();
	xx = (x_1 - x_2) * (x_1 - x_2);

	double y_1 = A.GetY();
	double y_2 = B.GetY();
	yy = (y_1 - y_2) * (y_1 - y_2);
	return sqrt(xx + yy);
}

