import os
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from typing import Tuple
from copy import deepcopy
from types import SimpleNamespace
from scipy.optimize import curve_fit
from scipy.spatial.distance import pdist
import decimal
decimal.setcontext(decimal.Context(prec=34))
plt.style.use('default')
path = os.getcwd()

def logistic_map(r):
    x_n = 0.0
    r = decimal.Decimal(r)
    return lambda x_n: r*decimal.Decimal(x_n)*(decimal.Decimal(1) - decimal.Decimal(x_n))

N = 1000000
array = np.zeros(N)
array[0] = 1e-5
r_infty = 3.569945672
lm = logistic_map(r_infty)

for n in range(N - 1):
    array[n+1] = lm(array[n])
x = array[5000:]

def dimension_boxcounting(x, power_of_10_minmax = (1, 2),
                        nsteps = 20, plot = False) -> Tuple[float, float]:

    n_bins_list = np.unique(np.logspace(power_of_10_minmax[0],
                                        power_of_10_minmax[1],
                                        nsteps, base = 10, dtype=int))

    histograms = [np.histogram(x, bins=N, range=(0, 1)) for N in n_bins_list]
    s = np.array([edges[1] - edges[0] for _, edges in histograms])
    Nocc = np.array([np.sum(H > 0) for H, _ in histograms])

    x = np.log10(1/s) # logarytm szerokosci_pudelek^-1
    y = np.log10(Nocc) # logarytm liczby wpadajacych punktow

    popt, pcov = curve_fit(lambda vals, a, const: a*vals + const, x, y)
    d = popt[0]
    const = popt[1]
    error = np.sqrt(np.diag(pcov))[0]

    if plot:
        fig, ax = plt.subplots(1,1,figsize = (2,1))
        ax.set_axis_off()
        ax.plot(x, y, '-', color = 'k')
        ax.plot(x, d*x + const, '--', color = 'r')
        plt.show()

    return d, error

d, error = dimension_boxcounting(x, power_of_10_minmax=(3, 5), nsteps=50, plot=True)
print(f"d = {d:.6} ± {error:.6}")

def ilustracja_sampopodobienstwa_histogramu(x, zooms):

    fig, ax =plt.subplots(len(zooms), 1, figsize = (6, 1.2*len(zooms)))

    H, xedges = np.histogram(x, bins=int(1e5), range=(0, 1))
    for n, zoom in enumerate(zooms):
        if n<len(zooms)-1:
            ax[n].axvline(zooms[n+1][0], color='r', linestyle='--')
            ax[n].axvline(zooms[n+1][1], color='r', linestyle='--')
        ax[n].stairs(H, xedges, color = 'k')
        ax[n].set_xlim(zoom)
        ax[n].set_ylabel(r'N$_i$')
        ax[n].set_xticks(np.linspace(zoom[0], zoom[1], 5))

    ax[-1].set_xlabel('i')
    fig.tight_layout()
    plt.show()

ilustracja_sampopodobienstwa_histogramu(x, zooms=[(0, 1), (0.3, 0.4), (0.34, 0.35), (0.342, 0.344)])

############################
######### OGÓLNIE ##########
############################

def multifractal_dimension(x, q, power_of_2point3_minmax = (4, 10),
                        nsteps = 20, plot = False, histrange = (0, 1)) -> Tuple[float, float]:

    epsilon = np.array([1.5**(-n) for n in np.linspace(power_of_2point3_minmax[0],
                                                        power_of_2point3_minmax[1],
                                                        nsteps)])
    n_bins_list = np.unique([int((histrange[1] - histrange[0])/e) for e in epsilon])

    N = np.sum(x)
    histograms = [np.histogram(x, bins=n_bins, range=histrange) for n_bins in n_bins_list]
    #epsilon = np.array([edges[1] - edges[0] for _, edges in histograms])
    mu = [H[H > 0]/N for H, _ in histograms]
    Z = np.array([np.sum(np.array(mu_i)**q) for mu_i in mu])

    x = np.log10(epsilon)
    y = np.log10(Z)
    popt, pcov = curve_fit(lambda vals, a, const: a*vals + const, x, y)

    tau = popt[0]
    const = popt[1]
    error = np.sqrt(np.diag(pcov))[0]

    if plot:
        fig, ax = plt.subplots(1,1,figsize = (2,1))
        ax.set_axis_off()
        ax.plot(x, y, '-', color = 'k')
        ax.plot(x, tau*x + const, '--', color = 'r')
        plt.show()

    return tau, error

############################
#### DLA ZBIORU CANTORA ####
############################

def multifractal_dimension_Cantor(q, p=0.5):

    def mu(k, p):
        if k == 0:
            return np.array([1.0])
        base = np.array([p, 1 - p])
        
        res = base
        for _ in range(k - 1):
            res = np.kron(res, base)
        return res

    Zs = []
    box_sizes = []
    for k in range(0, 12):
        edge = 3**(-k)
        mu_vals = mu(k, p)
        
        box_sizes.append(edge) # appendowanie szerokosci przedzialow
        Zs.append(sum(mu_vals**q)) # appendowanie Z
    s = np.array(box_sizes)
    Nocc = np.array(Zs)

    x = np.log10(s) # logarytm szerokosci_pudelek^-1
    y = np.log10(Nocc) # logarytm liczby wpadajacych punktow

    popt, pcov = curve_fit(lambda vals, a, b: a*vals + b, x, y)
    a = popt[0]
    error = np.sqrt(np.diag(pcov))[0]

    return a, error

def tau_exact(p, q):
    return -np.log(p**q + (1 - p)**q)/np.log(3)

def D(tau, q):
    return tau/(q-1)

data = []
for p in [0.5, 0.25]:

    for q in [-1, 0, 2]:

        tau_cantor, error_cantor = multifractal_dimension_Cantor(q=q, p=p)
        D_cantor = D(tau_cantor, q)
        tau_exact_val = tau_exact(p, q)
        D_exact_val = D(tau_exact_val, q)

        data.append({
            "p": p,
            "q": q,
            "tau": f"{tau_cantor:.6} ± {error_cantor:.6}",
            "tau_exact": f"{tau_exact_val:.6}",
            "D": f"{D_cantor:.6}",
            "D_exact": f"{D_exact_val:.6}"
        })

    # przesadnie pokomplikowane
    q = 1
    tau_cantor, error_cantor, D_cantor, tau_exact_val, D_exact_val = (np.zeros(2) for _ in range(5))
    for i, dq in enumerate([-0.001, 0.001]):
        tau_cantor[i], error_cantor[i] = multifractal_dimension_Cantor(q=q + dq, p=p)
        D_cantor[i] = D(tau_cantor[i], q + dq)
        tau_exact_val[i] = tau_exact(p, q + dq)
        D_exact_val[i] = D(tau_exact_val[i], q + dq)
    tau_cantor, error_cantor, D_cantor, tau_exact_val, D_exact_val = (np.average(x) for x in [tau_cantor, error_cantor, D_cantor, tau_exact_val, D_exact_val])
    data.append({
        "p": p,
        "q": q,
        "tau": f"{tau_cantor:.6} ± {error_cantor:.6}",
        "tau_exact": f"{tau_exact_val:.6}",
        "D": f"{D_cantor:.6}",
        "D_exact": f"{D_exact_val:.6}"
    })

print(pd.DataFrame(data))
print()
print(pd.DataFrame(data).to_latex(index=False))

data = []

q = 0
tau, error = multifractal_dimension(x, q, power_of_2point3_minmax =(10, 20), nsteps=40, plot=False)
data.append({
    "q": q,
    "D": f"{D(tau, q):.6} pm {error/(1-q):.6}",
    })

taup, errorp = multifractal_dimension(x, 1 + 0.001, power_of_2point3_minmax =(10, 20), nsteps=40, plot=False)
taum, errorm = multifractal_dimension(x, 1 - 0.001, power_of_2point3_minmax =(10, 20), nsteps=40, plot=False)
D_1 = (D(taup, 1 + 0.001) + D(taum, 1 - 0.001))/2
Derror = (errorp/(1 + 0.001 - 1) + errorm/(1 - 0.001 - 1))/2
data.append({
    "q": 1,
    "D": f"{D_1:.6} pm {abs(Derror):.6}",
    })

q = 2
tau, error = multifractal_dimension(x, q, power_of_2point3_minmax =(10, 20), nsteps=40, plot=False)
data.append({
    "q": q,
    "D": f"{D(tau, q):.5} pm {abs(error/(1-q)):.4}",
    })

print(pd.DataFrame(data))
print()
print(pd.DataFrame(data).to_latex(index=False))

q_linspace = np.arange(-10, 10.1, 0.5)
tau_vals = np.zeros_like(q_linspace)
tau_errs = np.zeros_like(q_linspace)
D_vals = np.zeros_like(q_linspace)

for idxq, q in enumerate(q_linspace):
    tau, error = multifractal_dimension(x, q, power_of_2point3_minmax =(10, 20), nsteps=40, plot=False)
    tau_vals[idxq] = tau
    tau_errs[idxq] = error
    D_vals[idxq] = D(tau, q)
D_errs = np.abs(tau_errs/(q_linspace - 1))

where = np.array(np.where(D_vals == np.inf))
D_vals[where] = (D_vals[where - 1] + D_vals[where + 1])/2

fig, ax = plt.subplots(1,2, figsize = (6, 2))
ax[0].plot(q_linspace, tau_vals, 'o', color = 'k', markersize = 0.5)
ax[0].errorbar(q_linspace, tau_vals, yerr = tau_errs, fmt='o', color = 'r', markersize = 0.5, elinewidth=0.5)
ax[0].set_xlabel(r'$q$')
ax[0].set_ylabel(r'$\tau$')
ax[1].plot(q_linspace, D_vals, 'o', color = 'g', markersize = 0.5)
ax[1].errorbar(q_linspace, D_vals, yerr = D_errs, fmt='o', color = 'r', markersize = 0.5, elinewidth=0.5)
ax[1].set_xlabel(r'$q$')
ax[1].set_ylabel(r'$D$')
plt.tight_layout()
plt.savefig("tauD.pdf")
plt.show()