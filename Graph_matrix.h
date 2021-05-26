// Copyright Zhabitskaya A.
// 2021

#pragma once
#include "Distance_matrix.h"

class Graph_matrix
{
private:
    int N;
    vector <int> B;
public:

    Graph_matrix();
    Graph_matrix(const Graph_matrix& b);
    ~Graph_matrix() = default;

    const Graph_matrix& operator=(const Graph_matrix& b);

    int get_N();
    int get_b(int i, int j);

    void create_graph_matrix(Distance_matrix& d, double delta);

    void print_graph_matrix();
};
