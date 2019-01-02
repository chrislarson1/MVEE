/**
    test.cpp
    Desc: tests for mvee class
    @author Chris Larson 
    @date (2017)
    @version 1.0
**/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <Eigen/Dense>
#include "boost/chrono.hpp"
#include "boost/filesystem.hpp"
#include "mvee/mvee.hpp"
#include "mvee/utils.hpp"

#define PI 3.14159265
#define EPS 0.001

using namespace std;
using namespace Eigen;
using namespace mvee;


/*
	@param e:double ellipse {a, b, x0, y0, phi}
	@param npoints:int number of points to draw from ellipse
	@return points:MatrixXd (npoints x 2)
*/
MatrixXd locus2D(vector<double> e, int npoints)
{
    // Container to store points on ellipse
    MatrixXd points(npoints, 2);
    
    // Generate points along ellipses via parametric form
    double theta;
    for (int i = 0; i < npoints; i++)
    {
        theta = i * 2 * PI / npoints;
        points(i, 0) = e[2] + e[0] * cos(2 * PI * theta) * cos(e[4]) - e[1] * sin(2 * PI * theta) * sin(e[4]);
        points(i, 1) = e[3] + e[0] * cos(2 * PI * theta) * sin(e[4]) + e[1] * sin(2 * PI * theta) * cos(e[4]);
    }

    return points;
}



int test_irregular_data()
{

	mvee::Mvee Bound;

	// Set of random ellipses
	vector<vector<double>> E {
	    {1.00, 1.00,  0.0,  0.0, 0.00 * PI	},
	    {1.20, 0.95, -1.0, -0.4, 0.25 * PI 	},
	    {1.10, 0.90, -1.0, -1.0, 0.50 * PI	},
	    {0.85, 1.15,  0.2, -0.5, 1.50 * PI 	},
	    {0.90, 1.00,  0.4,  1.0, 0.50 * PI 	},
	    {1.20, 0.95, -1.0, -0.4, 0.25 * PI 	},
	    {1.00, 0.80,  0.5, -1.0, 0.75 * PI 	},
	    {1.10, 0.93, -0.2,  0.6, 0.33 * PI 	},
	    {0.85, 1.00, -0.3,  0.2, 1.66 * PI 	},
	    {1.10, 0.82,  1.0,  1.5, 0.00 * PI 	}
	};
	vector<vector<int>> idxs {
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

	for (auto idx : idxs)
	{
		// Get discrete points along locus, params: (ellipse, num_points)
	    MatrixXd e1 = locus2D(E[idx[0]], 100);
	    MatrixXd e2 = locus2D(E[idx[1]], 100);
    	MatrixXd X(e1.rows() + e2.rows(), e1.cols());
    	for (int i = 0; i < e1.rows(); i++)
    	{
    		X.row(i * 2) = e1.row(i);
    		X.row(i * 2 + 1) = e2.row(i); 
    	}
    	Bound.compute(X, EPS);
	}

    return 0;
}



int test_multivariate_guassian()
{
    return 0;
}



int test_outliers()
{
    return 0;
}



int main()
{
	int status;

	// Irregular data
	status = test_irregular_data();
	if (status == 0)
	{
		cout << "test_irregular_data passed!" << endl;
	}
	else
	{
		throw runtime_error("test_irregular_data failed!");
	}

	// Multivariate normal data
	status = test_multivariate_guassian();
	if (status == 0)
	{
		cout << "test_multivariate_guassian passed!" << endl;
	}
	else
	{
		throw runtime_error("test_multivariate_guassian failed!");
	}

	// Outlier data
	status = test_outliers();
	if (status == 0)
	{
		cout << "test_outliers passed!" << endl;
	}
	else
	{
		throw runtime_error("test_outliers failed!");
	}

	return 0;
}
