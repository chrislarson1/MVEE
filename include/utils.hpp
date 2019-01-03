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



int findIdx(Eigen::VectorXd& vec, double val)
{
    for (int i=0; i<vec.rows(); i++)
    {
        if (vec(i) == val) 
        { 
			return i;
        }
    }
    return -1;
}


std::vector<double> toStdVec(Eigen::VectorXd& fromX, double (*f)(double))
{
	std::vector<double> toX(fromX.size(), 0.);
	for (int i=0; i<fromX.size(); i++)
	{
		toX[i] = (*f)(fromX(i));
	}
	return toX;
}


std::vector<std::vector<double>> toStdMat(Eigen::MatrixXd& fromX, double (*f)(double))
{
	std::vector<std::vector<double>> 
	toX(fromX.rows(), std::vector<double>(fromX.cols(), 0.));
	for (int i=0; i<fromX.rows(); i++)
	{
		for (int j=0; j<fromX.cols(); j++)
		{
			toX[i][j] = (*f)(fromX(i, j));
		}
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
			if (s != "")
			{
				rows++;
				if (!cols)
				{
					cols = count(s.begin(), s.end(), delim) + 1;
				} 
			}
		}
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
		for (int i=0; i<len; i++) 
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
