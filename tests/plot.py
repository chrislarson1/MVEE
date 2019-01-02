import numpy as np
import sys


def load_exp1(path):

    import glob
    """
    :param path: path to ellipses
    :return: dict {filename: list(np array)}
    """
    data = dict()
    for f in glob.glob(path + '*.csv'):
        datum = np.genfromtxt(f, skip_header=True, delimiter=',')
        key = f.split('/')[1].split('.')[0] + f.split('/')[1].split('.')[1]
        print(key)
        data[key] = list(datum)
    return data


def load_exp2(path):

    import glob
    """
    :param path: path to results
    :return: dict {filename: np array}
    """
    data = dict()
    for f in glob.glob(path + '*.csv'):
        datum = np.genfromtxt(f, skip_header=True, delimiter=',')
        key = f.split('.')[0].split('/')[1]
        print(key)
        data[key] = datum
    return data


def locus(e, n):
    """
    :param e: np array [a, b, x0, y0, phi]
    :param n: int num_points
    :return: np array of points (n x d)
    """
    points = np.zeros((n, 2))
    for i in range(n):
        theta = (i / float(n)) * 2 * np.pi
        points[i, 0] = e[2] + e[0] * np.cos(2 * np.pi * theta) * np.cos(e[4]) - e[1] * np.sin(2 * np.pi * theta) * np.sin(e[4])
        points[i, 1] = e[3] + e[0] * np.cos(2 * np.pi * theta) * np.sin(e[4]) + e[1] * np.sin(2 * np.pi * theta) * np.cos(e[4])
    return points


def plot_exp1(path, f, data):

    import matplotlib.pyplot as plt
    plt.style.use('ggplot')
    plt.plot(data[0][:, 0], data[0][:, 1], 'b',
             data[1][:, 0], data[1][:, 1], 'b',
             data[2][:, 0], data[2][:, 1], 'r')

    plt.xlabel('x')
    plt.ylabel('y')
    plt.title(f)
    plt.grid(False)
    plt.savefig(path + f + ".png")
    plt.close()


def plot_exp2(path, f, x, y, yax=None):

    import matplotlib.pyplot as plt
    plt.style.use('ggplot')
    xax = 'tolerance' if f == 'tol_sweep' else 'number of points'
    writepath = path + f + "_time" + ".png" if yax.__contains__('Time') else path + f + "_iter" + ".png"
    plt.plot(x, y, 'b')
    plt.xlabel(xax)
    plt.ylabel(yax)
    plt.title(f + ' (tol=0.01)' if f == 'n_sweep' else f + ' (n=10)')
    plt.grid(False)
    plt.savefig(writepath)
    plt.close()


def plot_shell(data):
    
    import matplotlib.pyplot as plt
    plt.style.use('ggplot')
    plt.plot(data[0][:, 0], data[0][:, 1], 'b',
             data[1][:, 0], data[1][:, 1], 'b',
             data[2][:, 0], data[2][:, 1], 'r')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid(False)
    plt.show()
    plt.close()


def exp1():
    # data = { file: list(ellipse, locus_e1, locus_e2, locus_be) }
    n = 1000
    path = 'output/'
    data = load_exp1(path)
    for key in data:
        for i in range(3):
            e = data[key][i]
            data[key].append(locus(e, n))
    for key in data:
        x = data[key][-3:]
        plot_exp1(path, key, x)


def exp2():
    # result = { file: np.array }
    path = 'analysis/'
    results = load_exp2(path)
    for key in results:
        x = results[key][:, 0]
        y = results[key][:, 1]
        plot_exp2(path, key, x, y, yax='Time (msec)')
        y = results[key][:, 2]
        plot_exp2(path, key, x, y, yax='Iterations to convergence')


def shell_session():
    # shell input: a1,b1,x01,y01phi1 a2,b2,x02,y02phi2
    n = 1000
    str1 = sys.argv[2]
    str2 = sys.argv[3]
    strbe = sys.argv[4]
    e1, e2, be = list(), list(), list()
    [e1.append(float(str1.split(',')[i])) for i in range(5)]
    [e2.append(float(str2.split(',')[i])) for i in range(5)]
    [be.append(float(strbe.split(',')[i])) for i in range(5)]
    data = list()
    for datum in [e1, e2, be]:
        data.append(locus(datum, n))
    plot_shell(data)


if sys.argv[1] == "shell=true":
    shell_session()

for arg in sys.argv:
    if arg == "exp1=true":
        exp1()
    if arg == "exp2=true":
        exp2()
