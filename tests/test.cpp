/**
    test.cpp
    Desc: tests for mvee class
    @author Chris Larson, Cornell University
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

#define EPS 1e-4
#define LMC 1e-8

using namespace std;
using namespace Eigen;
using namespace mvee;



/* 
	Ghost shell script that calls plot.py
	@param dfpath: path/to/fname.csv
	@param e: [a, b, x0, y0, phi]
*/
void ghostShellCall(string dfpath, string desc, vector<double> e)
{
	boost::filesystem::path BASEPATH(boost::filesystem::current_path());

    // string script_name = to_string(BASEPATH) + "/ghost.sh";
    string script_name = BASEPATH.string() + "/ghost.sh";
    ofstream gen_result(script_name);
    gen_result << "python3 test.py" << " " << desc << " " << dfpath << " " 
    		   << to_string(e[0]) << "," << to_string(e[1]) << "," 
    		   << to_string(e[2]) << "," << to_string(e[3]) << "," 
    		   << to_string(e[4]) << "," << to_string(e[5]);
    gen_result.close();
    string shell_cmd = "sh " + script_name;
    system(shell_cmd.c_str());
    remove(shell_cmd.c_str());
    shell_cmd = "rm -f " + script_name;
	system(shell_cmd.c_str());
}


vector<double> getEllipse(Mvee& B)
{
	vector<double> ellipse {
		B.radii()[0],
		B.radii()[1],
		B.centroid()[0],
		B.centroid()[1],
		acos(B.pose()[0][0]),
		acos(B.pose()[0][0])		
	};
	return ellipse;
}


int test_irregular()
{
	Mvee B;
	string dfbase = "data/irregular";
	for (int i=0; i<10; i++)
	{
		string dfpath = dfbase + "_" + to_string(i) + ".csv";
    	B.compute(dfpath, ',', EPS, LMC);
		vector<double> ellipse = getEllipse(B);
    	ghostShellCall(dfpath, "test_irregular_" + to_string(i), ellipse);
	}
    return 0;
}


int test_multimodal()
{
	Mvee B;
	string dfpath = "data/multimodal.csv";
	B.compute(dfpath, ',', EPS, LMC);
	vector<double> ellipse = getEllipse(B);
	ghostShellCall(dfpath, "test_multimodal", ellipse);
    return 0;
}


int test_multivariate()
{
	Mvee B;
	string dfpath = "data/multivariate.csv";
	MatrixXd X = readCSV(dfpath, ',');
	B.compute(X, 1e-8, LMC);
    vector<double> radii = B.radii();
	vector<double> centroid = B.centroid();
	vector<vector<double>> pose = B.pose();
	double t = B.time;
	long int iters = B.iters;
	string fname = boost::filesystem::current_path().string() + "/results/multivariate.csv";
	ofstream result(fname);
	result << "data_rows" << "," << X.rows() << "\n" <<
			  "data_cols" << "," << X.cols() << "\n" <<
			  "compute_time" << "," << t << "\n" <<
			  "compute_iters" << "," << iters;
	result.close();
    return 0;
}


int test_outliers()
{
	Mvee B;
	string dfpath = "data/outlier.csv";
	MatrixXd X = readCSV(dfpath, ',');
	vector<vector<double>> X_STD = toStdMat(X, [](double z) { return z; });
	B.compute(X_STD, EPS, LMC);
	vector<double> ellipse = getEllipse(B);
	ghostShellCall(dfpath, "test_outlier", ellipse);
    return 0;
}



int main(int argc, char **argv)
{
	boost::filesystem::path BASEPATH(boost::filesystem::current_path());
    
    /******* TEST DATA *******/ 
	string script_name = BASEPATH.string() + "/ghost.sh";
	ofstream gen_data(script_name);
	gen_data << "python3 data.py";
    gen_data.close();
    string shell_cmd = "sh " + script_name;
    system(shell_cmd.c_str());
    remove(shell_cmd.c_str());
    shell_cmd = "rm -f " + script_name;
	system(shell_cmd.c_str());

    /******* UNIT TESTS *******/ 
	int status;

	// Irregular data
	status = test_irregular();
	if (status == 0)
	{
		cout << "test_irregular passed!" << endl;
	}
	else
	{
		throw runtime_error("test_irregular failed!");
	}

	// Multimodal data
	status = test_multimodal();
	if (status == 0)
	{
		cout << "test_multimodal passed!" << endl;
	}
	else
	{
		throw runtime_error("test_multimodal failed!");
	}

	// Multivariate data
	status = test_multivariate();
	if (status == 0)
	{
		cout << "test_multivariate passed!" << endl;
	}
	else
	{
		throw runtime_error("test_multivariate failed!");
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