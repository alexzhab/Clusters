// Copyright Zhabitskaya A.
// 2021

#pragma once
#include "Triangle.h"
#include "Delaunay_alg.h" 

class Interpolation
{
public:
	int Start(Vector P, vector <Vector> All_Point);
	double Evaluation(vector <Vector> All_Point);
	double d(Vector A, Vector B);
};
