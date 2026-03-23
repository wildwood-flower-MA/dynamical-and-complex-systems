import os
import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple
from copy import deepcopy
from types import SimpleNamespace
from scipy.optimize import curve_fit
from scipy.spatial.distance import pdist

plt.style.use('default')
path = os.getcwd()

fig, ax=plt.subplots(2,4,figsize = (16,5))
fig2, ax2=plt.subplots(2,4,figsize = (16,5))
a_vals = [0.5, 1.1, 1.25, 1.4]
for i in range(4):
    
    data = np.loadtxt(path + f"\\wyniki\\f{i}.txt").T
    ax[0, i].plot(data[0], 'o', color = 'k', markersize = 0.4)
    ax[1, i].plot(data[1], 'o', color = 'r', markersize = 0.4)

    ax[0, i].set_title(f"a = {a_vals[i]}")
    ax[0, i].set_ylabel(r"x$_n$")
    ax[1, i].set_ylabel(r"y$_n$")
    ax[0, i].set_xlabel(r"$n$")
    ax[1, i].set_xlabel(r"$n$")

    data = np.loadtxt(path + f"\\wyniki\\d{i}.txt").T
    ax2[0, i].plot(data[0], 'o', color = 'k', markersize = 0.4)
    ax2[1, i].plot(data[1], 'o', color = 'r', markersize = 0.4)

    ax2[0, i].set_title(f"a = {a_vals[i]}")
    ax2[0, i].set_ylabel(r"x$_n$")
    ax2[1, i].set_ylabel(r"y$_n$")
    ax2[0, i].set_xlabel(r"$n$")
    ax2[1, i].set_xlabel(r"$n$")

fig.suptitle("float")
fig2.suptitle("double")
fig.tight_layout()
fig2.tight_layout()
plt.show()

def autocorr(x):
    result = np.correlate(x, x, mode='full')
    return result[result.size//2:]

fig, ax=plt.subplots(1,4,figsize = (16,5))
fig2, ax2=plt.subplots(1,4,figsize = (16,5))

a_vals = [0.5, 1.1, 1.25, 1.4]
for i in range(4):
    
    data = np.loadtxt(path + f"\\wyniki\\f{i}.txt").T
    ax[i].plot(autocorr(data[0])[1:150]/autocorr(data[0])[0], 'o', color = 'k', markersize = 2, label = "X")
    ax[i].plot(autocorr(data[1])[1:150]/autocorr(data[0])[0], 'o', color = 'r', markersize = 2, label = "Y")
    ax[i].set_ylabel("autocorr")
    ax[i].set_xlabel("k")
    ax[i].legend(frameon  = True)

    data = np.loadtxt(path + f"\\wyniki\\d{i}.txt").T
    ax2[i].plot(autocorr(data[0])[1:150]/autocorr(data[0])[0], 'o', color = 'k', markersize = 2, label = "X")
    ax2[i].plot(autocorr(data[1])[1:150]/autocorr(data[0])[0], 'o', color = 'r', markersize = 2, label = "Y")
    ax2[i].set_ylabel("autocorr")
    ax2[i].set_xlabel("k")
    ax2[i].legend(frameon  = True)

fig.suptitle("float")
fig2.suptitle("double")
fig.tight_layout()
fig2.tight_layout()
plt.show()

fig, axb = plt.subplots(1,2, figsize=  (10,5))

for i in range(301):

        data = np.loadtxt(path + f"\\wyniki\\bif{i}.txt").T
        x = data[0]
        y = data[1]
        uniquex = np.unique(x[-50:])
        uniquey = np.unique(y[-50:])

        axb[0].plot(np.ones_like(uniquex)*1.5*(i/300), uniquex, 'o', color = 'k', markersize = 0.1)
        axb[1].plot(np.ones_like(uniquey)*1.5*(i/300), uniquey, 'o', color = 'r', markersize = 0.1)
        axb[0].set_ylabel("x*")
        axb[1].set_ylabel("y*")
        axb[0].set_xlabel("a")
        axb[1].set_xlabel("a")
        
fig.tight_layout()
plt.savefig("2.png", dpi = 500)

fig, ax=plt.subplots(1,4,figsize = (16,5))
fig2, ax2=plt.subplots(1,4,figsize = (16,5))

a_vals = [0.5, 1.1, 1.25, 1.4]
for i in range(4):
    
    data = np.loadtxt(path + f"\\wyniki\\f{i}.txt").T
    ax[i].plot(data[0], data[1], 'o', color = 'k', markersize = 0.5)

    ax[i].set_xlabel("x")
    ax[i].set_ylabel("y")
    data = np.loadtxt(path + f"\\wyniki\\d{i}.txt").T
    ax2[i].plot(data[0], data[1], 'o', color = 'k', markersize = 0.5)
    ax2[i].set_xlabel("x")
    ax2[i].set_ylabel("y")

fig.suptitle("float")
fig2.suptitle("double")
fig.tight_layout()
fig2.tight_layout()
plt.show()

data = np.loadtxt(path + f"\\wyniki\\f3.txt").T

fig, ax = plt.subplots(1,2, figsize = (8,4))
ax[0].plot(data[0], data[1], 'o', markersize = 0.4, color = 'k')
ax[1].plot(data[0], data[1], 'o', markersize = 0.4, color = 'k')
ax[1].set_xlim(0.0, 0.5)
ax[1].set_ylim(0.1, 0.3)
ax[0].set_xlabel("x")
ax[0].set_ylabel("y")
ax[1].set_xlabel("x")
ax[1].set_ylabel("y")
fig.tight_layout()
plt.plot()

data = np.loadtxt(path + f"\\wyniki\\f3.txt").T

def dimension_boxcounting(data, pow10min, pow10max, n_steps = 20):

    x = data[0]
    y = data[1]

    maxval = max(np.max(x), np.max(y))
    minval = min(np.min(x), np.min(y))
    zakres = [[minval, maxval], [minval, maxval]]
    
    box_sizes = []
    counts = []

    n_bins_list = np.logspace(pow10min, pow10max, n_steps).astype(int) 
    n_bins_list = np.unique(n_bins_list)

    for N in n_bins_list:

        H, xedges, _ = np.histogram2d(x, y, bins=N, range=zakres)
        eps = xedges[1] - xedges[0]
        non_empty = np.sum(H > 0)
        if non_empty > 0:
            box_sizes.append(eps)
            counts.append(non_empty)

    s = np.array(box_sizes)
    Nocc = np.array(counts)
    x = np.log10(1/s)
    y = np.log10(Nocc)

    popt, pcov = curve_fit(lambda vals, a, b: a*vals + b, x, y)
    a = popt[0]
    b = popt[1]
    error = np.sqrt(np.diag(pcov))[0]

    fig, ax = plt.subplots(1,1,figsize = (4,4))
    ax.plot(x, y, '-', color = 'k')
    ax.plot(x, a*x + b, '--', color = 'r')
    plt.show()

    return a, error

def dimension_correlation(data, pow10min, pow10max, n_steps=40):

    data_local = data.T
    data_local = data_local
    
    distances = pdist(data_local)
    epsilons = np.logspace(pow10min, pow10max, n_steps)
    distances.sort()
    N_pairs = len(distances)
    
    C_values = []
    for eps in epsilons:
        count = np.searchsorted(distances, eps)
        C_values.append(count)

    C_values = np.array(C_values)/N_pairs
    valid_mask = C_values > 0
    x = np.log10(epsilons[valid_mask])
    y = np.log10(C_values[valid_mask])
    
    popt, pcov = curve_fit(lambda vals, a, b: a*vals + b, x, y)
    a = popt[0]
    b = popt[1]
    error = np.sqrt(np.diag(pcov))[0]

    fig, ax = plt.subplots(1, 1, figsize=(4, 4))
    ax.plot(x, y, '-', color='k')
    ax.plot(x, a*x + b, '--', color = 'r')
    plt.show()

    return a, error

print(dimension_boxcounting(data, pow10min = 1.2, pow10max = 2.2))
print(dimension_correlation(data, pow10min = -1e-5, pow10max = -3.3))