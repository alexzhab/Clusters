#ifndef K_MEANS_CORES_ALGORITHM_H_H_INCLUDED
#define K_MEANS_CORES_ALGORITHM_H_H_INCLUDED

#include "field.cpp"
#include "cluster.cpp"

class KMeansCore_Algorithm{
    private:
        int n, k, p;
        FILE *f;
        Field* p_field;
        vector <Vector> center;                 // Вектор центров класетров в алгоритме k-средних с ядрами
        vector <Cluster> clusters;
        vector <int> dots_membership;           // Вектор, размера N*k, где k-параметр алгоритма, N - количество точек в поле, в i-ой строке которой указано к какому из k-центров принадлежит i-ая точка из поля: 1- если принадлежит j-му центру, 0 - иначе.
        void KMeans(int k, FILE *f, int n, vector <Vector> vectdots, int spec, vector <Vector> *centvec, int q);
        void SaveKMCoreStep(int step, int k, int p, vector <Vector> centvec);
    public:
        KMeansCore_Algorithm(Field* p_fieldd, int kk, int pp, FILE* ff);
        ~KMeansCore_Algorithm() = default;
        vector <Cluster> MainAlgorithm();
};

#endif // K_MEANS_CORES_ALGORITHM_H_H_INCLUDED
