#!/usr/bin/env python

"""
    data.py
    Desc: Computes minimum volume ellipsoid
    @author Chris Larson, Cornell University
    @date (2017)
    @version 1.0
"""

import os
import numpy as np

PATH = os.path.join(
    os.path.dirname(os.path.realpath(__file__)),
    'data'
)
if not os.path.isdir(PATH):
    os.mkdir(PATH)


def locus(e, n):
    """
    :param e: np array [a, b, x0, y0, phi1, phi2]
    :param n: int num_points
    :return: np array of points (n x d)
    """
    assert len(e) == 6, len(e)
    points = np.zeros((n, 2))
    for i in range(n):
        theta = (i / float(n)) * 2 * np.pi
        points[i, 0] = e[2] + e[0] * np.cos(2 * np.pi * theta) * np.cos(e[4]) - \
                       e[1] * np.sin(2 * np.pi * theta) * np.sin(e[5])
        points[i, 1] = e[3] + e[0] * np.cos(2 * np.pi * theta) * np.sin(e[5]) + \
                       e[1] * np.sin(2 * np.pi * theta) * np.cos(e[4])
    return points


def gen_irregular_data():

    ellipses = [
        [1.00, 1.00,  0.0,  0.0, 0.00 * np.pi, 0.00 * np.pi],
        [1.20, 0.95, -1.0, -0.4, 0.25 * np.pi, 0.25 * np.pi],
        [1.10, 0.90, -1.0, -1.0, 0.50 * np.pi, 0.50 * np.pi],
        [0.85, 1.15,  0.2, -0.5, 1.50 * np.pi, 1.50 * np.pi],
        [0.90, 1.00,  0.4,  1.0, 0.50 * np.pi, 0.50 * np.pi],
        [1.20, 0.95, -1.0, -0.4, 0.25 * np.pi, 0.25 * np.pi],
        [1.00, 0.80,  0.5, -1.0, 0.75 * np.pi, 0.75 * np.pi],
        [1.10, 0.93, -0.2,  0.6, 0.33 * np.pi, 0.33 * np.pi],
        [0.85, 1.00, -0.3,  0.2, 1.66 * np.pi, 1.66 * np.pi],
        [1.10, 0.82,  1.0,  1.5, 0.00 * np.pi, 0.00 * np.pi]
    ]

    idxs = [
        [0, 1], [7, 4], [4, 6], [2, 5], [3, 5],
        [8, 9], [1, 4], [7, 2], [0, 6], [9, 7]
    ]

    for idx, (i, j) in enumerate(idxs):
        x1 = locus(ellipses[i], 500)
        x2 = locus(ellipses[j], 500)
        data = np.concatenate((x1, x2), axis=0)
        np.random.shuffle(data)
        fname = os.path.join(PATH, "irregular_{}.csv".format(idx))
        np.savetxt(fname, data, delimiter=",", newline="\n")


def gen_multimodal_data():
    mu1  =  [0.0, 0.0]
    cov1 = [[1.0, 0.0],
            [0.0, 1.0]]
    x1 = np.random.multivariate_normal(mu1, cov1, 250)
    mu2  =  [3.0, 2.0]
    cov2 = [[0.5, 0.3],
            [0.3, 2.0]]
    x2 = np.random.multivariate_normal(mu2, cov2, 250)
    mu3  =  [-.9, -.9]
    cov3 = [[1.0, 0.0],
            [0.0, 1.0]]
    x3 = np.random.multivariate_normal(mu3, cov3, 250)
    data = np.concatenate((x1, x2, x3), axis=0)
    np.random.shuffle(data)
    fname = os.path.join(PATH, "multimodal.csv")
    np.savetxt(fname, data, delimiter=",", newline="\n")



def gen_multivariate_data():
    mu  = np.random.uniform(0, 5, size=500)
    cov = np.diag(np.array([np.random.randint(1, 4) for _ in range(500)]))
    data = np.random.multivariate_normal(mu, cov, 5000)
    np.random.shuffle(data)
    fname = os.path.join(PATH, "multivariate.csv")
    np.savetxt(fname, data, delimiter=",", newline="\n")


def gen_outlier_data():
    mu1  =  [0.0, 0.0]
    cov1 = [[0.1, 0.0],
            [0.0, 0.1]]
    x1 = np.random.multivariate_normal(mu1, cov1, 500)
    mu2  =  [0.50, -1.0]
    cov2 = [[0.01, 0.00],
            [0.00, 0.50]]
    x2 = np.random.multivariate_normal(mu2, cov2, 50)
    data = np.concatenate((x1, x2), axis=0)
    np.random.shuffle(data)
    fname = os.path.join(PATH, "outlier.csv")
    np.savetxt(fname, data, delimiter=",", newline="\n")


if __name__ == '__main__':
    gen_irregular_data()
    gen_multimodal_data()
    gen_multivariate_data()
    gen_outlier_data()