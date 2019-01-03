/*
    mvee.cpp
    Desc: minimum volume ellipsoid method
    @author Chris Larson 
    @date (2017)
    @version 1.0
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <exception>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <Eigen/Dense>
#include "boost/chrono.hpp"
#include "mvee.hpp"
#include "utils.hpp"

using namespace std;
using namespace Eigen;
using namespace mvee;


Mvee::Mvee() {}


void Mvee::decompose(MatrixXd& A, VectorXd& s, MatrixXd& U, MatrixXd& V)
{
    JacobiSVD<MatrixXd> SVD(A, ComputeThinU | ComputeThinV);
    s = SVD.singularValues();
    V = SVD.matrixV();
    U = SVD.matrixU();
}


/*
    Khachiyan's ellipsoid method
    @param X: matrix of data (n x d)
    @param eps: approximation error (default: 0.001)
    @param lmcoeff: Levenberg-Marquardt coefficient 
                    used for inverting ill-condditioned
                    matrices. (default: 1e-8)
    Ref: Khachiyan (1979)
*/
void Mvee::khachiyan(MatrixXd& data, double eps, double lmcoeff)
{
    // Khachiyan's algorithm
    auto t0 = chrono::system_clock::now();    
    iters = 0; 
    long double err = INFINITY; 
    double alpha;
    int n = data.rows();
    int d = data.cols();
    MatrixXd X = data.transpose();
    MatrixXd Q(X.rows() + 1, X.cols());
    Q.row(0) = X.row(0);
    Q.row(1) = X.row(1);
    Q.row(2).setOnes();
    VectorXd u = (1 / (double) n) * VectorXd::Ones(n);
    VectorXd uhat = u;
    MatrixXd G(d + 1, d + 1);
    MatrixXd noiseye = MatrixXd::Identity(G.rows(), G.cols()) * lmcoeff;
    VectorXd g(n);
    double m; int i;
    while (err > eps)
    {
        G = Q * u.asDiagonal() * Q.transpose() + noiseye;
        g = (Q.transpose() * G.inverse() * Q).diagonal();
        m = g.maxCoeff();
        i = findIdx(g, m);
        alpha = (m - d - 1) / ((d + 1) * (m - 1));
        uhat = (1 - alpha) * u;
        uhat(i) += alpha;
        err = (uhat - u).norm();
        u = uhat;
        iters++;
    }
    time = chrono::duration<double>(
        chrono::system_clock::now() - t0
    ).count();
    
    // Decompose US^2U^T
    MatrixXd E = (1 / (double) d) * (
        X * u.asDiagonal() * X.transpose()
        - (X * u) * (X * u).transpose()
    ).inverse();
    VectorXd x = X * u;
    VectorXd s(d);
    MatrixXd V(d, d);
    MatrixXd U(d, d);
    decompose(E, s, U, V);

    // Cache result
    _centroid = toStdVec(x, [](double z) { return z; });
    _radii    = toStdVec(s, [](double z) { return 1 / sqrt(z); });
    _pose     = toStdMat(V, [](double z) { return z; });
}


/*
    Computes minimum volume ellipse enclosing data in file (path/to/file.csv)
    @param file:string path to .csv file containing data
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(string file, char delim, double eps, double lmcoeff)
{
    MatrixXd D = readCSV(file, delim);
    khachiyan(D, eps, lmcoeff);
}


/*
    Computes minimum volume ellipse enclosing data
    @param data:vector<vector<double>> data (n x d)
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(vector<vector<double>>& data, double eps, double lmcoeff)
{
    int rows = data.size();
    int cols = data[0].size();
    MatrixXd D(rows, cols);
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<cols; j++)
        {
            D(i, j) = data[i][j];
        }
    }
    khachiyan(D, eps, lmcoeff);
}


/*
    Computes minimum volume ellipse enclosing data in file
    @param file:Eigen::MatrixXd data (n x d)
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(MatrixXd& data, double eps, double lmcoeff)
{
    khachiyan(data, eps, lmcoeff);
}


/*
    Ellipse centroid
    @return centroid:vector<double> (d x 1)
    Note: Invoke mveeInstance.compute() prior to retrieving 
            the ellipse parameters.
*/
vector<double> Mvee::centroid()
{
    return _centroid;
}


/*
    Ellipse radii
    @return radii:vector<double> (d x 1)
    Note: Invoke mveeInstance.compute() prior to retrieving 
            the ellipse parameters.
*/
vector<double> Mvee::radii()
{
    return _radii;
}


/*
    Ellipse pose (rotation)
    @return pose:vector<vector<double>> (d x d)
    Note: Invoke mveeInstance.compute() prior to retrieving 
            the ellipse parameters.
*/
vector<vector<double>> Mvee::pose()
{
    return _pose;
}


Mvee::~Mvee() {}


int main(int argc, char **argv)
{
    return 0;
}