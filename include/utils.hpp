/*
    utils.cpp
    Desc: Utility functions for mvee library
    @author Chris Larson 
    @date (2017)
    @version 1.0
*/

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <thread>
#include <future>
#include <exception>
#include <Eigen/Dense>
#include <boost/algorithm/string.hpp>



int findMaxIdx(Eigen::VectorXd& vec)
{
    int idx = 0;
    double max_elem = vec.maxCoeff();
    for (int i=0; i<vec.rows(); i++)
    {
        if (vec(i) == max_elem) 
        { 
            idx = i; 
            break;
        }
    }
    return idx;
}


void fillvec(std::vector<double> toX,
			 Eigen::VectorXd& fromX,
			 unsigned int begin, 
			 unsigned int end, 
			 double (*fn)(double x))
{
	for (int i=begin; i=end; i++)
	{
		toX[i] = (*fn)(fromX(i));
	}
}


std::vector<double> convertVec(Eigen::VectorXd& fromX, double (*fn)(double))
{
	unsigned int dim = fromX.rows();
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::vector<double> toX(dim, 0.);

	// if (dim < 100)
	// {
	// 	fillvec(0, fromX.rows());
	// }
	// else
	// {
	// 	std::vector<std::future<int>> jobs;
	// 	for (unsigned int i=0; i<floor(dim / nthreads); i++)
	// 	{
	// 		unsigned int n = floor(dim / nthreads);
	// 		unsigned int N;
	// 		if (i == nthreads - 1) 
	// 		{ 
	// 			N = (unsigned int)n + dim % nthreads; 
	// 		}
	// 		else 
	// 		{ 
	// 			N = n; 
	// 		}
	// 		jobs.push_back(std::async(
	// 			fillvec((unsigned int)i * n, (unsigned int)i * n + N)
	// 		));
	// 	}
	// 	for (auto job : jobs) 
	// 	{ 
	// 		int status = job.get(); 
	// 	}
	// }

	fillvec(toX, fromX, 0, fromX.rows(), *fn);

	return toX;
}

void fillmat(std::vector<std::vector<double>> toX, 
			 Eigen::MatrixXd& fromX,
			 unsigned int row, 
			 unsigned int begin, 
			 unsigned int end, 
			 double (*fn)(double))
{
	for (int i=begin; i=end; i++)
	{
		toX[row][i] = (*fn)(fromX(row, i));
	}
}

std::vector<std::vector<double>> convertMat(Eigen::MatrixXd& fromX, double (*fn)(double))
{
	std::pair<int, int> dim = std::make_pair(fromX.rows(), fromX.cols());
	unsigned int nthreads = std::thread::hardware_concurrency();
	std::vector<std::vector<double>> toX(dim.first, std::vector<double>(dim.second, 0.));
	
	// if (dim < 100)
	// {
	// 	fillvec(0, fromX.rows());
	// }
	// else
	// {
	// 	std::vector<std::future<int>> jobs;
	// 	for (unsigned int i=0; i<floor(dim / nthreads); i++)
	// 	{
	// 		unsigned int n = floor(dim / nthreads);
	// 		unsigned int N;
	// 		if (i == nthreads - 1) 
	// 		{ 
	// 			N = (unsigned int)n + dim % nthreads; 
	// 		}
	// 		else 
	// 		{ 
	// 			N = n; 
	// 		}
	// 		jobs.push_back(std::async(
	// 			fillvec((unsigned int)i * n, (unsigned int)i * n + N)
	// 		));
	// 	}
	// 	for (auto job : jobs) 
	// 	{ 
	// 		int status = job.get(); 
	// 	}
	// }

	for (int i=0; i < dim.first; i++)
	{
		fillmat(toX, fromX, i, 0, fromX.cols(), *fn);
	}
	
	return toX;
}


Eigen::MatrixXd readCSV(std::string file, char delim) 
{
	// Get dimensions for Eigen::MatrixXd constructor
	int rows = 0;
	int cols = 0;
	std::ifstream in;
	in.open(file);
	if (in.is_open())
	{
		std::string s;
		while(!in.eof()) 
		{
			getline(in, s);
			rows++;
		}
		cols = count(s.begin(), s.end(), delim);
	}
	else
	{
		throw std::runtime_error("Error: Cannot open " + file + ".");
	}
	in.close();
	
	// Stream file into Eigen::MatrixXd container
	std::string line;
	Eigen::MatrixXd X = Eigen::MatrixXd(rows, cols);
	int row = 0;
	int col = 0;
	in.open(file);
	while (getline(in, line)) 
	{
		char* ptr = (char*) line.c_str();
		int len = line.length();
		col = 0;
		char* start = ptr;
		for (int i = 0; i < len; i++) 
		{
			if (ptr[i] == delim) 
			{
			  X(row, col++) = atof(start);
			  start = ptr + i + 1;
			}
		}
		X(row, col) = atof(start);
		row++;
	}
	in.close();
	
	return X;
}
