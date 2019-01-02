import numpy as np


def plot(xax, x, y, xtitle, ytitle, rng=None):
    import matplotlib.pyplot as plt
    plt.style.use('ggplot')
    plt.plot(xax, x, 'b-',
             xax, y, 'r-')
    plt.xlabel(xtitle)
    plt.ylabel(ytitle)
    if rng:
        plt.ylim([rng[0], rng[1]])
    plt.show()


def scatter(x, y):
    import matplotlib.pyplot as plt
    plt.style.use('ggplot')
    xax = np.arange(len(x))
    plt.scatter(xax, x, c='b', s=100)
    plt.scatter(xax, y, c='r', s=30)
    plt.xlabel('n')
    plt.ylabel('amplitude')
    plt.ylim([-2., 2.])
    plt.show()


def uw_fir(x, N):
    n = len(x)
    y = np.zeros_like(x)
    x = np.hstack((np.zeros((N - 1,)), x))
    for i in range(n):
        y[i] = np.sum(x[i: i + N])
    y /= N
    scatter(x[N - 1:], y)
    return y


def hw_fir(x, M):
    n = len(x)
    y = np.zeros_like(x)
    x = np.hstack((np.zeros((M - 1,)), x))
    for i in range(n):
        for j in range(M):
            y[i] += (j + 1) * x[i + j]
    y = y / float(0.5 * (M ** 2 + M))
    scatter(x[M - 1:], y)
    return y


def delay_v_depth():
    xax = np.array(range(1, 51))
    uw_del = (xax - 1) / float(2)
    hw_del = (xax - 1) / float(3)
    plot(xax, uw_del, hw_del, 'Number of taps (N,M)', 'delay (samples)')


def boost_v_N():
    N = np.array(range(1, 51))
    yboost = np.sqrt(N)
    zboost = np.sqrt(1.5) * np.sqrt(N * (N + 1) / (2 * N + 1).astype(float))
    plot(N, yboost, zboost, 'Number of taps (N,M)', 'SNR Boost')


if __name__ == '__main__':

    # x = np.array([4, 3, 5, 6, 7, 4, 3, 2, 3, 4, 1, -1, 0, 3, 2, 6, 5, 7, 7, 6, 5, 4, 3, 2, 1])
    # x = np.array([0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1])
    # x = np.array([0, 0, 0, 0, 1, 1, 1, 1, 1.5, 1.5, 1.5, 1.5, 0, 0, 0, 0])

    # cycles = 2
    # xax = np.linspace(0, cycles * 2 * np.pi, 1000)
    # x = np.sin(xax)
    #
    # yuw = uw_fir(x, 10)
    # yhw = hw_fir(x, 10)

    delay_v_depth()

    # boost_v_N()





