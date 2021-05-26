// Copyright Zhabitskaya A.
// 2021

#pragma once
#include "Vector.h"

class Distance_matrix
{
private:
    int N;
    vector <double> D;
public:
 
    Distance_matrix();
    Distance_matrix(const Distance_matrix& d);
    ~Distance_matrix() = default;

    const Distance_matrix& operator=(const Distance_matrix& d);

    int get_N();
    double get_d(int i, int j);

    void create_distance_matrix(vector <Vector> points);

    void print_distance_matrix();
};