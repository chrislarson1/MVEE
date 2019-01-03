import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from data import locus


plt.style.use('ggplot')


PATH = os.path.join(
    os.path.dirname(os.path.realpath(__file__)),
    'results'
)
if not os.path.isdir(PATH):
    os.mkdir(PATH)


def plot(data, ellipse, fname):
    ellipse = locus(ellipse, len(data) // 2)
    plt.scatter(data[:, 0], data[:, 1], c='b')
    plt.plot(ellipse[:, 0], ellipse[:, 1], c='r')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(False)
    plt.savefig(fname=fname, dpi=300)
    plt.close()


if __name__ == '__main__':
    test = sys.argv[1]
    data_f = sys.argv[2]
    res_f = os.path.join(PATH, "{}.png".format(test))
    ellipse = [float(x) for x in sys.argv[3].split(",")]
    if test.__contains__("test_irregular_"):
        data = np.loadtxt(data_f, delimiter=",")
        plot(data, ellipse, res_f)
    elif test == "test_multimodal":
        data = np.loadtxt(data_f, delimiter=",")
        plot(data, ellipse, res_f)
    elif test == "test_multivariate":
        data = np.loadtxt(data_f, delimiter=",")
        plot(data, ellipse, res_f)
    elif test == "test_outlier":
        data = np.loadtxt(data_f, delimiter=",")
        plot(data, ellipse, res_f)