/*
    mvee.hpp
    Desc: Computes minimum volume ellipsoid
    @author Chris Larson
    @date (2017)
    @version 1.0
*/

#ifndef MVEE_HPP_
#define MVEE_HPP_

#include <string>
#include <vector>
#include <Eigen/Dense>


namespace mvee {

class Mvee {

private:

	std::vector<double> _centroid;

	std::vector<double> _radii;

	std::vector<std::vector<double>> _pose;
	
	void decompose(Eigen::MatrixXd&,
			 	   Eigen::VectorXd&, 
			 	   Eigen::MatrixXd&, 
			 	   Eigen::MatrixXd&);

	void run(Eigen::MatrixXd&, double);

public:
	
	Mvee();

	double time;

	long int iters;

	std::vector<double> centroid();

	std::vector<double> radii();

	std::vector<std::vector<double>> pose();

	void compute(std::vector<std::vector<double>>&, double);

	void compute(Eigen::MatrixXd&, double);

	void compute(std::string, char, double);

	~Mvee();

};

}


#endif /* MVEE_HPP_ */