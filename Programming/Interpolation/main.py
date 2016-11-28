#!/usr/bin/python
from collections import defaultdict
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import sys
import math
import numpy as np
import bisect

'''/*
    * Read data from file
    */'''
def get_source_plot():
    raw_file = open("input_data.txt", "r") 
    N = int(raw_file.readline())
    x = []
    y = []
    for cur_line in raw_file:
        cur_list = cur_line.split(' ')
        x.append(float(cur_list[0]))
        y.append(float(cur_list[1]))
    return x, y

'''/*
    * Calculating optimal step for interpolated plot
    */'''
def interpolate_step(x, y):
    N = len(x)
    min_delta = sys.float_info.max
    for i in range(1,N):
        if (x[i] - x[i-1]) < min_delta:
            min_delta = x[i] - x[i-1]
    return min_delta/10

'''/*
    * Newton interpolation method
    * |
    * |-> calc_divided_difference:
    * |     Generate pyramid of divided differences
    * |-> newton_polynom
    * |     Interpolate input points via Newton polynom
    */'''
def calc_divided_difference(x, y):
    N = len(x);
    f_div = [y]
    for i in range(1, N):
        f_div.append([])
        for j in range(0, N - i):
            f_div[i].append((f_div[i - 1][j + 1] - f_div[i - 1][j]) / (x[j + i] - x[j]))
    return f_div

def newton_polynom(x, y):
    N = len(x);
    opt_step = interpolate_step(x, y)
    out_x = list(n for n in np.arange(x[0],(int(round(x[N-1]/opt_step)) + 1)* \
                                      opt_step, opt_step))
    f_div = calc_divided_difference(x, y)
    result = []
    for t in out_x:
        summand = 0
        delta = 1
        for i in range(N):
            if i - 1 >= 0:
                delta *= t - x[i - 1]
            summand += f_div[i][0] * delta
        result.append(summand)
    return out_x, result

'''/*
    * Lagrange interpolation method
    */'''
def lagrange_polynom(x, y):
    N = len(x);
    opt_step = interpolate_step(x, y)
    out_x = list(n for n in np.arange(x[0],(int(round(x[N-1]/opt_step)) + 1)* \
                                      opt_step, opt_step))
    result = []
    for t in out_x:
        calc_polynom = 0
        for i in range(N):
            l_i = y[i]
            for j in range(N):
                if i != j:
                    l_i *= (t - x[j]) / (x[i] - x[j])
            calc_polynom += l_i
        result.append(calc_polynom)
    return out_x, result

'''/*
    * Cubic spline interpolation method
    */'''
def tridiag_solve(a, c, b, f, N):
    c_new = [c[0]] + [0] * (N - 1)
    f_new = [f[0]] + [0] * (N - 1)
    for i in range(1, N):
        c_new[i] = c[i] - a[i-1] * b[i-1] / c_new[i-1]
        f_new[i] = f[i] - a[i-1] * f_new[i-1] / c_new[i-1]  

    x = [0] * (N - 1) + [f_new[N-1] / c_new[N-1]] 
    for i in reversed(range(0, N-1)):
        x[i] = (f_new[i] - b[i] * x[i+1]) / c_new[i]
    
    return x

class S3:
    def __init__(self, X, F, N):
        self.X = X.copy()
        a = F.copy()
        c = [0] + tridiag_solve([X[i+1] - X[i] for i in range(1, N-2)],
            [2 * (X[i+2] - X[i]) for i in range(0, N-2)],
            [X[i+2] - X[i+1] for i in range(0, N-3)],
            [6 * ((F[i+2] - F[i+1]) / (X[i+2] - X[i+1]) - (F[i+1] - F[i])/(X[i+1] - X[i])) for i in range(0, N-2)], N - 2) + [0]
        d = [0] + [(c[i] - c[i-1]) / (X[i] - X[i-1]) for i in range(1, N)]
        b = [0] + [(F[i+1] - F[i]) / (X[i+1] - X[i]) + (X[i+1] - X[i]) * (2 * c[i+1] + c[i]) / 6 for i in range(0, N-1)]
        self.splines = list(zip(a[1:], b[1:], c[1:], d[1:], X[1:]))
   
    def get_val_by_spline(self, s, t):
        return s[0] + s[1] * (t - s[4]) + s[2] / 2 * (t - s[4])**2 + s[3] / 6 * (t - s[4])**3 

    def get_val(self, t):
        return self.get_val_by_spline(self.splines[list(filter(lambda s: (s[0] <= s[3]) and (s[3] <= s[1]), 
                                                               zip(self.X, self.X[1:], range(0, N), [t] * N)))[0][2]], t)

'''/*
    * Main
    */'''
#Source plot
s_x,s_y = get_source_plot()
#Newton interpolation
plt.figure(1)
plt.subplot(311)
n_x,n_y = newton_polynom(s_x, s_y)
plt.plot(s_x, s_y, "go", linewidth=5.0, label="Source data")
plt.plot(n_x, n_y,"b", label="Newton interp.")
plt.grid(True)
plt.legend(loc=2, borderaxespad=0)
#Lagrange interpolation
plt.subplot(312)
l_x,l_y = lagrange_polynom(s_x, s_y)
plt.plot(s_x, s_y, "go", linewidth=5.0, label="Source data")
plt.plot(l_x, l_y, "m", label="Lagrange interp.")
plt.grid(True)
plt.legend(loc=2, borderaxespad=0)
#Cubic spline
N = len(s_x) - 1
cs = S3(s_x, s_y, len(s_x))
opt_step = interpolate_step(s_x, s_y)
out_x = list(n for n in np.arange(s_x[0],(int(round(s_x[N]/opt_step)))* \
                                      opt_step, opt_step))
plt.subplot(313)
plt.plot(s_x, s_y, "go", linewidth=5.0, label="Source data")
plt.plot(out_x, [cs.get_val(x) for x in out_x],"y", label="Cubic spline")
plt.grid(True)
plt.legend(loc=2, borderaxespad=0)
#Show
plt.show()
