#include "K_Means_Cores_Algorithm_h.h"

KMeansCore_Algorithm::KMeansCore_Algorithm(Field *p_fieldd, int kk, int pp, FILE* ff){
    p_field = p_fieldd;
    n = p_field->AllDots;
    k = kk;
    p = pp;
    f = ff;
    center.resize(k*p);
    dots_membership.resize(n*k, 0);
    clusters.resize(k*p);
    for(int i=0; i<k*p; ++i)
        clusters[i] = Cluster(p_field->Dots);
}

vector<Cluster>KMeansCore_Algorithm::MainAlgorithm(){

    int Position_Last_Cluster = 0;

    vector <Vector> tmpvect, centvec, prevcentvec;
    double mindist, localval;
    int tmp, iter;
    bool condition = false;
    mindist = localval = 1000000;
    tmp = iter = 0;
    // PreAlgorithm initialization

    centvec.resize(k*p);
    prevcentvec.resize(k*p);
    for (int i=0; i<k*p; ++i){
        centvec[i] = p_field->VectorID((n*i)/(k*p));
    }
    for(int i=0; i<n; ++i){
        mindist = 1000000;
        tmp = 0;
        for(int j=0; j<k*p; ++j){
            if (Distant(p_field->VectorID(i), centvec[j]) < mindist){
                mindist = Distant(p_field->VectorID(i), centvec[j]);
                tmp = j%p;
            }
        }
        for (int j=0; j<k; ++j){
            if (j == tmp)
                dots_membership[i*k + j] = 1;
            else
                dots_membership[i*k + j] = 0;
        }
    }
    // End
    // Core Algorithm
    while (!condition){
        iter++;
        double param;
        Vector CenterMass(0.0, 0.0);
        condition = true;
//        cout << "iter = " << iter << endl;
        // Loop to change membership dots to new loop
        for(int i=0; i<n; ++i){
            mindist = 1000000;
            tmp = 0;
            for(int j=0; j<k*p; ++j){
                if (Distant(p_field->VectorID(i), centvec[j]) < mindist){
                    mindist = Distant(p_field->VectorID(i), centvec[j]);
                    tmp = j%k;
                }
            }
            for (int j=0; j<k; ++j){
                if (j == tmp)
                    dots_membership[i*k + j] = 1;
                else
                    dots_membership[i*k + j] = 0;
            }
        }
        // End
        // Loop to change Centers of Clusters + Save Steps
        tmpvect.resize(n);
        for (int j = 0; j < k; ++j){
            for (int l=0; l<p; ++l)
                prevcentvec[j*p+l] = centvec[j*p+l];
            tmp =0;
            tmpvect.clear();
            for (int i=0; i< n; ++i){
                if (dots_membership[i*k + j] == 1)
                    tmp++;
            }
            tmpvect.resize(tmp);
            int w=0;
            for (int i=0; i<n; ++i){
                if (dots_membership[i*k + j] == 1){
                    tmpvect[w] = p_field->VectorID(i);
                    w++;
                }
            }

            KMeans(p, f, tmp, tmpvect, 1, &centvec, j*p);
            for (int l=0; l<p; ++l){
                if (Distant(prevcentvec[j*p + l], centvec[j*p + l]) >= EPS){
                    condition = false;
                }
            }
            if (iter >= 100)
                condition = true;
            SaveKMCoreStep(iter, k, p, centvec);
            for (int i=0; i<p; ++i)
                clusters[Position_Last_Cluster + i].DeleteCluster();
        }
        // End
    }

    for (int i=0; i< n; ++i){
        for (int j=0; j<k; ++j){
            if (dots_membership[i*k + j] == 1){
                clusters[Position_Last_Cluster + j].AddDot(i);
            }
        }
    }
    Position_Last_Cluster+=k;


    return clusters;
}



void KMeansCore_Algorithm::KMeans(int k, FILE *f, int n, vector <Vector> vectdots, int spec, vector <Vector> *centvec, int q){

    double mindist, localval;
    int PositionLastCluster=0;
    int tmp, iter;
    bool condition = false;
    mindist = localval = 1000000;
    tmp = iter = 0;
    // PreAlgorithm initialization


    for (int i=0; i<k; ++i)
        center[i] = vectdots[(n*i)/k];

    for (int i =0; i<n; ++i){
        mindist = 1000000;
        for (int j=0; j<k; ++j){
            localval = Distant(center[j], vectdots[i]);
            if (localval < mindist){
                mindist = localval;
                tmp = j;
            }
        }
        for (int j=0; j<k; ++j){
            if (j!= tmp){
                dots_membership[i*k + j] = 0;
            }
            else{
                dots_membership[i*k + j] = 1;
            }
        }
    }
    // End
    // Core Algorithm
    while (!condition){
        double param;
        Vector CenterMass(0.0, 0.0);
        condition = true;
        // Loop to change Centers of Clusters
        for(int i=0; i<k; ++i){
            param = 0;
            for (int j=0; j< n; ++j){
                if (dots_membership[j*k + i] == 1){
                    CenterMass = CenterMass + vectdots[j];
                    param += 1;
                }
            }
            if (param != 0){
                CenterMass = (1/param) * CenterMass;
                if (Distant(CenterMass, center[i]) > EPS)
                    condition = false;
                center[i] = CenterMass;
            }
        }
        // End
        // Loop to change membership dots to new loop
        for (int i =0; i<n; ++i){
            mindist = 1000000;
            tmp =0;
            for (int j=0; j<k; ++j){
                localval = Distant(center[j], vectdots[i]);
                if (localval < mindist){
                    mindist = localval;
                    tmp = j;
                }
            }

            for (int j=0; j<k; ++j){
                if (j!= tmp){
                    dots_membership[i*k + j] = 0;
                }
                else{
                    dots_membership[i*k + j] = 1;
                }
            }
        }
        iter++;
        // End
    }


    for (int i=0; i< n; ++i){
        for (int j=0; j<k; ++j){
            if (dots_membership[i*k + j] == 1){
                clusters[PositionLastCluster + j].AddDot(i);    // May be error, because before it was "AddDots(vectdots[i])
            }
        }
    }


    if (spec == 0){
        for (int j=0; j<k; ++j){
            for (int i=0; i<clusters[PositionLastCluster + j].Count_Dots(); ++i){
                if(clusters[PositionLastCluster + j].Indicator[i] == 1.0)
                    p_field->VectorID(i).PrintVecOutN(f);
                fprintf(f, "\n", j);
            }
            fprintf(f, "\n\n");
        }
    }
    if (spec == 1)
        for(int j=0; j<k; ++j)
            (*centvec)[q+j] = center[j];


    center.clear();
    dots_membership.clear();
}

void KMeansCore_Algorithm::SaveKMCoreStep(int step, int k, int p, vector <Vector> centvec){
    ofstream points_step("Data_3_sem/kmc/points_" + to_string(step) + ".plt");
    ofstream centers_step("Data_3_sem/kmc/centers_" + to_string(step) + ".plt");

    for(int i=0; i<n; ++i){
        for(int j=0; j<k; ++j){
            if (dots_membership[i*k + j] == 1){
                p_field->VectorID(i).PrintVec(points_step);
                points_step << j << endl;
            }
        }
    }
    for (int j=0; j<k; ++j){
        for (int l=0; l<p; ++l){
            centvec[j*p + l].PrintVec(centers_step);
            centers_step << j << endl;
        }
        centers_step << endl << endl;
    }
}
