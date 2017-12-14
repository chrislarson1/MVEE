# work_sample
Description: this program finds an affine hull of points sampled from the locuses of two ellipses. Ellipses are defined in main.cpp using the set: {a, b, x0, y0, phi}.

Dependencies are Eigen (v3) and Boost (v1.63), see Makefile for details. There is also binary in build/bin/ that was compiled on a macbook air (~2013). 

plot.py plots the outputs from functions experiment1 and experiment2 defined in main.cpp, the outputs can be found in outputs/ and analysis/ respectively.

To run program:
$ ./build/bin/MVEE 
