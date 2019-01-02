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
#include <string>
#include <map>
#include <cmath>
#include <thread>
#include <future>
#include <vector>
#include <Eigen/Dense>
#include "boost/chrono.hpp"
#include "boost/filesystem.hpp"
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
    Ellipsoid method
    @param X: matrix of data (n x d)
    @param eps: approximation error (default: 0.001)
    Ref: Khachiyan (1979)
*/
void Mvee::run(MatrixXd& data, double eps)
{
    // Khachiyan's algorithm
    auto t0 = chrono::system_clock::now();    
    iters = 0; 
    long double err = INFINITY; 
    double alpha;
    long int n = data.rows();
    long int d = data.cols();
    MatrixXd X = data.inverse();
    MatrixXd Q(X.rows() + 1, X.cols());
    Q.row(0) = X.row(0);
    Q.row(1) = X.row(1);
    Q.row(2).setOnes();
    VectorXd u = (1 / (double) n) * VectorXd::Ones(n);
    VectorXd uhat = u;
    MatrixXd G(d + 1, d + 1);
    VectorXd g(n);
    double m = -1; 
    double j = -1;
    while (err > eps)
    {
        G = Q * u.asDiagonal() * Q.transpose();
        g = (Q.transpose() * G.inverse() * Q).diagonal();
        findMaxIdx(g);
        alpha = (m - d - 1) / ((d + 1) * (m - 1));
        uhat = (1 - alpha) * u;
        uhat(j) = uhat(j) + alpha;
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
    _centroid = convertVec(x, [](double z) { return z; });
    _radii    = convertVec(s, [](double z) { return 1 / sqrt(z); });
    _pose     = convertMat(V, [](double z) { return z; });
}


/*
    Computes minimum volume ellipse enclosing data in file (path/to/file.csv)
    @param file:string path to .csv file containing data
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(string file, char delim, double eps)
{
    MatrixXd D = readCSV(file, delim);
    run(D, eps);
}


/*
    Computes minimum volume ellipse enclosing data
    @param data:vector<vector<double>> data (n x d)
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(vector<vector<double>>& data, double eps)
{
    int rows = floor(sizeof(data) / sizeof(data[0]));
    int cols = floor(sizeof(data[0]) / sizeof(data[0, 0]));
    MatrixXd D(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            D(i, j) = data[i][j];
        }
    }
    run(D, eps);
}


/*
    Computes minimum volume ellipse enclosing data in file
    @param file:Eigen::MatrixXd data (n x d)
    @param delim:char delimiter character
    @param eps:double approximation error (default: 0.001)
*/
void Mvee::compute(MatrixXd& data, double eps)
{
    run(data, eps);
}


/*
    Ellipse centroid
    @return centroid:vector<double> (d x 1)
    Note: Be sure to invoke mveeInstance.compute() prior to 
            retrieving the ellipse parameters.
*/
vector<double> Mvee::centroid()
{
    return _centroid;
}


/*
    Ellipse radii
    @return radii:vector<double> (d x 1)
    Note: Be sure to invoke mveeInstance.compute() prior to 
            retrieving the ellipse parameters.
*/
vector<double> Mvee::radii()
{
    return _radii;
}


/*
    Ellipse pose (rotation)
    @return pose:vector<vector<double>> (d x d)
    Note: Be sure to invoke mveeInstance.compute() prior to 
            retrieving the ellipse parameters.
*/
vector<vector<double>> Mvee::pose()
{
    return _pose;
}


Mvee::~Mvee() {}


int main()
{
    return 0;
}