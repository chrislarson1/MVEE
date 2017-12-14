//
//  main.cpp
//  Minimum Volume Enclosing Ellipsoid Problem
//  Chris larson (2017)
//

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <map>
#include <cmath>
#include <thread>
#include <future>
#include <vector>
#include <Eigen/Dense>
#include "boost/chrono.hpp"
#include "boost/filesystem.hpp"

#define PI 3.14159265
#define EPS 1e-2
#define N 10
#define RUNS 10

using namespace std;
using namespace chrono;
using namespace Eigen;


// Set of random ellipses
vector<vector<double>> E {
    {1, 1, 0, 0, 0},
    {1.2, 0.95, -1., -0.4, 0.25 * PI},
    {1.1, 0.9, -1., -1., PI / 2},
    {0.85, 1.15, 0.2, -0.5, 1.5 * PI},
    {0.9, 1, 0.4, 1, 0.5 * PI},
    {1.2, 0.95, -1., -0.4, 0.25 * PI},
    {1, 0.8, 0.5, -1., 0.75 * PI},
    {1.1, 0.93, -0.2, 0.6, 0.33 * PI},
    {0.85, 1., -0.3, 0.2, 1.66 * PI},
    {1.1, 0.82, 1., 1.5, 0 * PI}
};

vector<vector<int>> inds {
    {0, 1},
    {7, 4},
    {4, 6},
    {2, 5},
    {3, 5},
    {8, 9},
    {1, 4},
    {3, 5},
    {0, 6},
    {9, 7}
};


MatrixXd locus(double e[5], int num_points)
{
    // Container to store points on ellipse
    MatrixXd points(num_points, 2);
    
    // Generate points along ellipses using parametric form
    double theta;
    for (int i = 0; i < num_points; i++)
    {
        // e: {a, b, x0, y0, phi}
        theta = i * 2 * PI / num_points;
        points(i, 0) = e[2] + e[0] * cos(2 * PI * theta) * cos(e[4]) - e[1] * sin(2 * PI * theta) * sin(e[4]);
        points(i, 1) = e[3] + e[0] * cos(2 * PI * theta) * sin(e[4]) + e[1] * sin(2 * PI * theta) * cos(e[4]);
    }

    return points;
}


void find_max(VectorXd &vec, double &max_elem, int &max_ind)
{
    max_elem = vec.maxCoeff();
    for (int i = 0; i < vec.rows(); i++)
    {
        if (vec(i) == max_elem) { max_ind = i; break; }
    }
}


void decomp(MatrixXd &A, VectorXd &s, MatrixXd &U, MatrixXd &V, string type)
{
    if (type == "SVD")
    {
        JacobiSVD<MatrixXd> SVD(A, ComputeThinU | ComputeThinV);
        s = SVD.singularValues();
        V = SVD.matrixV();
        U = SVD.matrixU();
    }
    else if (type == "EIGD")
    {
        assert(A.cols() == A.rows());
        // TODO
    }
}


void bounding_ellipse(MatrixXd &V, VectorXd &svals, VectorXd &xhat, double (&be)[5])
{
    be[2] = xhat(0);
    be[3] = xhat(1);
    be[0] = 1 / sqrt(svals(0));
    be[1] = 1 / sqrt(svals(1));
    be[4] = acos(V(0, 0));
}


void MVEE(double (&e1)[5], double (&e2)[5], double (&be)[5], double &t, int &it, int num_points, double eps)
{
    auto init_time = chrono::system_clock::now();
    
    // Get discrete points along locus, params: (ellipse, num_points)
    auto future1 = std::async(locus, e1, num_points);
    auto future2 = std::async(locus, e2, num_points);
    MatrixXd ellipse_1 = future1.get();
    MatrixXd ellipse_2 = future2.get();
    
    //////////// MVEE approximation via Khachiyan's algorithm ////////////
    int count = 1; long double err = INFINITY; double alpha;
    // long double EPSS = 1e-3;
    long int n = ellipse_1.rows() + ellipse_2.rows();
    long int d = ellipse_1.cols();
    MatrixXd X(d, n);
    //P << ellipse_1.transpose(), ellipse_2.transpose();
    int j = 0;
    for (int i = 0; i < (n / (double) 2); i++)
    {
        X.col(j) = ellipse_1.transpose().col(i);
        X.col(j + 1) = ellipse_2.transpose().col(i);
        j = j + 2;
    }
    MatrixXd Q(X.rows() + 1, X.cols());
    Q.row(0) = X.row(0);
    Q.row(1) = X.row(1);
    Q.row(2).setOnes();
    VectorXd u = (1 / (double) n) * VectorXd::Ones(n);
    VectorXd uhat = u;
    MatrixXd G(d + 1, d + 1);
    VectorXd g(n);
    double m = -1; j = -1;
    while (err > eps)
    {
        G = Q * u.asDiagonal() * Q.transpose();
        g = (Q.transpose() * G.inverse() * Q).diagonal();
        find_max(g, m, j);
        alpha = (m - d - 1) / ((d + 1) * (m - 1));
        uhat = (1 - alpha) * u;
        uhat(j) = uhat(j) + alpha;
        count++;
        err = (uhat - u).norm();
        u = uhat;
    }
    ////////////////////// End Khachiyan's algorithm //////////////////////
    
    // Timeit
    double execution_time = chrono::duration<double>(chrono::system_clock::now() - init_time).count();
    
    // Decompose US^2U^T
    MatrixXd E = (1 / (double) d) * (X * u.asDiagonal() * X.transpose() - (X * u) * (X * u).transpose()).inverse();
    VectorXd xhat = X * u;
    VectorXd svals(d);
    MatrixXd V(d, d);
    MatrixXd U(d, d);
    decomp(E, svals, U, V, "SVD");
    
    // Store {a, b, x0, y0, phi} in bounding ellipse be
    bounding_ellipse(V, svals, xhat, be);
    it = count;
    t = execution_time;
}


void write_exp1(ofstream &f, double (&e)[5], double (&t), int (&it))
{
    for (auto entry : e)
    {
        f << entry << ',';
    }
    f << t << "," << it;
    f << '\n';
}


void experiment1()
{
    double t = 0; int it = 0;
    VectorXd time((int) RUNS); VectorXd iters((int) RUNS);
    cout << "Tolerance: " << EPS << endl;
    for (int i = 0; i < 10; i++)
    {
        double e1[5], e2[5], be[5];
        copy(E[inds[i][0]].begin(), E[inds[i][0]].end(), e1);
        copy(E[inds[i][1]].begin(), E[inds[i][1]].end(), e2);
        for (int j = 0; j < RUNS; j++)
        {
            MVEE(e1, e2, be, t, it, N, EPS);
            time(j) = t;
            iters(j) = it;
        }
        const string path = "output/" + to_string(inds[i][0]) + "_" +
                            to_string(inds[i][1]) + "_" + to_string(N) +
                            "_" + to_string(EPS) + ".csv";
        ofstream f(path);
        f << "a,b,x0,y0,phi,msec,iters\n";
        t = 1000 * time.mean();
        it = (int) iters.mean();
        write_exp1(f, e1, t, it);
        write_exp1(f, e2, t, it);
        write_exp1(f, be, t, it);
        f.close();
        if (i == 9) { cout << "Compute time: " << t << " milliseconds" << endl; }
    }
}


void experiment2()
{
    double EPS_sweep[5] = {.1, .05, .01, .005, .001};
    int N_sweep[5] = {5, 50, 100, 1000, 10000};
    double teps = 0; int iteps = 0; double tns = 0; int itns = 0;
    string path_N_sweep = "analysis/n_sweep.csv";
    string path_EPS_sweep = "analysis/tol_sweep.csv";
    double e1[5], e2[5], be[5];
    copy(E[7].begin(), E[7].end(), e1);
    copy(E[4].begin(), E[4].end(), e2);
    VectorXd time_eps((int) RUNS); VectorXd iters_eps((int) RUNS);
    VectorXd time_ns((int) RUNS); VectorXd iters_ns((int) RUNS);
    
    ofstream feps(path_EPS_sweep);
    ofstream fns(path_N_sweep);
    feps << "eps,msec,iterations\n";
    fns << "N,msec,iterations\n";
    
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < RUNS; j++)
        {
            MVEE(e1, e2, be, teps, iteps, N, EPS_sweep[i]);
            MVEE(e1, e2, be, tns, itns, N_sweep[i], EPS);
            time_eps(j) = teps;
            iters_eps(j) = iteps;
            time_ns(j) = tns;
            iters_ns(j) = itns;
        }
        
        teps = 1000 * time_eps.mean();
        iteps = (int) iters_eps.mean();
        tns = 1000 * time_ns.mean();
        itns = (int) iters_ns.mean();
        
        feps << to_string(EPS_sweep[i]) << "," << teps << "," << iteps << "\n";
        fns << to_string(N_sweep[i]) << "," << tns << "," << itns << "\n";

        cout << "eps_sweep: " << teps << " milliseconds, " << iteps << " iterations." << endl;
        cout << "n_sweep: " << tns << " milliseconds, " << itns << " iterations." << endl;
    }
    
    feps.close();
    fns.close();
}


void shell_session()
{
    // Query user
    double e1[5], e2[5], be[5], eps, t;
    int n, it;
    cout << "Enter number of points to sample from each ellipse:" << endl;
    cin >> n; n = 2 * n;
    cout << "Enter tolerance (default=0.01):" << endl;
    cin >> eps;
    cout << "Enter ellipse 1, format=a b x0 y0 phi" << endl;
    cin >> e1[0] >> e1[1] >> e1[2] >> e1[3] >> e1[4];
    cout << "Enter ellipse 2, format=a b x0 y0 phi" << endl;
    cin >> e2[0] >> e2[1] >> e2[2] >> e2[3] >> e2[4];
    
    // MVEE
    MVEE(e1, e2, be, t, it, n, eps);
    
    // Ghost shell script that calls plot.py
    // ... format of call: a1,b1,x01,y01phi1 a2,b2,x02,y02phi2
    // boost::filesystem::path pwd(boost::filesystem::current_path());
    string script_name = "gen_plot.sh";
    ofstream gen_plot(script_name);
    gen_plot <<
    "python plot.py shell=true" <<
    " " << to_string(e1[0]) << "," << to_string(e1[1]) << "," << to_string(e1[2]) << "," << to_string(e1[3]) << "," << to_string(e1[4]) <<
    " " << to_string(e2[0]) << "," << to_string(e2[1]) << "," << to_string(e2[2]) << "," << to_string(e2[3]) << "," << to_string(e2[4]) <<
    " " << to_string(be[0]) << "," << to_string(be[1]) << "," << to_string(be[2]) << "," << to_string(be[3]) << "," << to_string(be[4]);
    gen_plot.close();
    string script_call = "sh " + script_name;
    system(script_call.c_str());
    remove(script_name.c_str());
}


int main(int argc, const char * argv[]) {
    
    // Seed random number generator
    srand((unsigned int) time(0));
    
    // // Runtime analyses
    // experiment1();
    // experiment2();
    
    // Run test from shell
    shell_session();
    
    return 0;
}
