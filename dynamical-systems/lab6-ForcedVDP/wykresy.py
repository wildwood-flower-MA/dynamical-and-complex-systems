import scipy as sp
import numpy as np
import os
import matplotlib.pyplot as plt
import sympy as sp
from sympy.abc import x, y
from numpy.lib.scimath import sqrt as sqrt
from typing import Tuple

trace = lambda m, epsilon: -epsilon * (m**2 - 1)
det = lambda m, epsilon : np.ones_like(m)
delta = lambda m, epsilon: (epsilon * (m**2 - 1))**2 - 4

lambda1 = lambda m, epsilon: (trace(m, epsilon) + sqrt(delta(m, epsilon)))/2.0
lambda2 = lambda m, epsilon: (trace(m, epsilon) - sqrt(delta(m, epsilon)))/2.0

ms = np.linspace(0, 3, 10000)
epsilons = [0, 0.5, 3]

fig, ax = plt.subplots(1,3, figsize = (12, 4))
for i, ax_i in enumerate(ax):
    ax_i.set_title(r"$\varepsilon$ = " + f"{epsilons[i]}")
    ax_i.plot(ms, lambda1(ms, epsilons[i]).real, color = 'k', label = r'Re($\lambda$)')
    ax_i.plot(ms, lambda1(ms, epsilons[i]).imag, color = 'k', linestyle = '--', label = r'Im($\lambda$)')
    ax_i.plot(ms, lambda2(ms, epsilons[i]).real, color = 'k')
    ax_i.plot(ms, lambda2(ms, epsilons[i]).imag, color = 'k', linestyle = '--')
    ax_i.set_xlim(np.min(ms), np.max(ms))
    ax_i.legend(frameon = False)
    ax_i.set_xlabel("m")
fig.tight_layout()
plt.savefig("lambdas.pdf")
plt.show()

fig, ax = plt.subplots(1,3, figsize = (12, 4))
for i, ax_i in enumerate(ax):
    ax_i.set_title(r"$\varepsilon$ = " + f"{epsilons[i]}")
    ax_i.plot(ms, trace(ms, epsilons[i]), color = 'k', label = r'tr($\mathcal{J}|_{\boldsymbol{r}}$)')
    ax_i.plot(ms, det(ms, epsilons[i]), color = 'k', linestyle = '--', label = r'det($\mathcal{J}|_{\boldsymbol{r}}$)')
    ax_i.plot(ms, delta(ms, epsilons[i]), color = 'k', linestyle = 'dotted', label = r'$\Delta$($\mathcal{J}|_{\boldsymbol{r}}$)')
    
    ax_i.set_xlim(np.min(ms), np.max(ms))
    ax_i.legend(frameon = False)
    ax_i.set_xlabel("m")
    if i == 2:
        ax_i.set_ylim(-30, 50)
fig.tight_layout()
plt.savefig("trdetdelta.pdf")
plt.show()

def dotXV(x: np.ndarray, v: np.ndarray, m: float, epsilon: float) -> Tuple[np.ndarray, np.ndarray]:
    dotX = v
    dotV = m - epsilon*(x**2 - 1)*v - x
    return dotX, dotV

epsilons = np.array([0.0, 0.5, 3.0])

sqrt3_3 = np.sqrt(3)/3
sqrt15_3 = np.sqrt(15)/3

ms = np.array([
    np.array([1.0]),
    np.array([0.5, 1.0, (1.0 + np.sqrt(5))/2.0, np.sqrt(5), (np.sqrt(5) + 3.0)/2.0]),
    np.array([sqrt3_3/2.0, sqrt3_3, (sqrt3_3 + 1.0)/2.0, 1.0, (1.0 + sqrt15_3)/2.0, sqrt15_3, (sqrt15_3 + 3.0)/2.0])
    ], dtype=object)


for i, epsilon in enumerate(epsilons):
    ms_i = ms[i]

    for j, m in enumerate(ms_i):

        fig, ax = plt.subplots(1, 3, figsize=(16, 5))

        fig.suptitle(f"$\epsilon = {epsilon}$, $m = {m:.3f}$", fontsize=16, y=0.17, x = 0.12)

        x = np.linspace(-6, 8, 21)
        v = np.linspace(-8, 8, 21)
        X, V = np.meshgrid(x, v)
        dotX, dotV = dotXV(X, V, m=m, epsilon=epsilon)

        norm = np.sqrt(dotX**2 + dotV**2)
        with np.errstate(divide='ignore', invalid='ignore'):
            qn = 1/norm
        
        ax[0].quiver(X, V, dotX*qn, dotV*qn, color = "k", headwidth=3, scale=40)
        ax[0].set_title("$(x,v)$")
        ax[0].set_xlabel("$x$")
        ax[0].set_ylabel("$v$")
        ax[0].set_xlim(-6, 8)
        ax[0].set_ylim(-8, 8)
        ax[0].grid(True, linestyle=':', alpha=0.6)
        ax[0].plot(m, 0, 'ro', markersize=5, markeredgewidth=2, label='Punkt stały')
        for k in range(6): 
            fname = f"lab_{i}_{j}_{k}.dat"
        
            txv = np.loadtxt(fname).T

            ax[0].plot(txv[1], txv[2])
            ax[0].plot(txv[1, 0], txv[2, 0], 'o', markersize=4, color='black')
            ax[1].plot(txv[0], txv[1])
            ax[2].plot(txv[0], txv[2])

        t_min, t_max = np.min(txv[0]), np.max(txv[0])
        
        ax[1].set_xlim(t_min, t_max)
        ax[1].set_title("$x(t)$")
        ax[1].set_xlabel("$t$")
        ax[1].set_ylabel("$x$")
        ax[1].grid(True, linestyle=':', alpha=0.6)

        ax[2].set_xlim(t_min, t_max)
        ax[2].set_title("$v(t)$")
        ax[2].set_xlabel("$t$")
        ax[2].set_ylabel("$v$")
        ax[2].grid(True, linestyle=':', alpha=0.6)

        fig.tight_layout()
        plt.savefig(f"{epsilon}_{m:.2f}.pdf")
        
        plt.show()

for i, epsilon in enumerate(epsilons):
    ms_i = ms[i]

    for j, m in enumerate(ms_i):
        
        fig, ax = plt.subplots(figsize=(6, 5))
        
        ax.set_title(f"$\epsilon = {epsilon}$, $m = {m:.3f}$", fontsize=14)
        x = np.linspace(-4, 4, 41)
        v = np.linspace(-4, 4, 41)
        X, V = np.meshgrid(x, v)
        
        dotX, dotV = dotXV(X, V, m=m, epsilon=epsilon)
        norm = np.sqrt(dotX**2 + dotV**2)*1.5
        with np.errstate(divide='ignore', invalid='ignore'):
            qn = 1/norm
        
        ax.quiver(X, V, dotX*qn, dotV*qn, color = "k", headwidth=3, scale=35, width=0.003)
        ax.set_xlabel("$x$")
        ax.set_ylabel("$v$")
        ax.set_xlim(-4, 4)
        ax.set_ylim(-4, 4)
        ax.grid(True, linestyle=':', alpha=0.6)
        ax.set_aspect('equal')  # Ustawienie równej skali osi x i y
    
        ax.plot(m, 0, 'ro', markersize=6, markeredgewidth=1)

        fig.tight_layout()
        plt.savefig(f"field_only_{epsilon}_{m:.2f}.pdf")
        plt.show()

wartosci_m = np.linspace(0.9, 1, 5, endpoint=True)    

colors = [f"#{int(255 - i * 50):02x}{int(255 - i * 50):02x}{int(255 - i * 50):02x}" for i in range(5)]

fig, ax = plt.subplots(2,1, figsize = (10,6))
for i in range(5):
    data = np.loadtxt(f"amplituda_{i}.dat").T
    ax[0].plot(data[0], data[1], color = colors[i]); ax[0].set_xlabel("t"); ax[0].set_ylabel("x")
    ax[1].plot(data[0], data[2], color = colors[i]); ax[1].set_xlabel("t"); ax[1].set_ylabel("v")
    ax[0].plot(data[0], np.ones_like(data[0])*wartosci_m[i], linestyle = "--", color = colors[i], linewidth = 0.7)
    ax[0].set_xlim(np.min(data[0]), np.max(data[0]))
    ax[1].set_xlim(np.min(data[0]), np.max(data[0]))

fig.tight_layout()
plt.savefig("amplituda.pdf")

fig, ax = plt.subplots(1,1, figsize = (4,4))
for i in range(5):
    data = np.loadtxt(f"amplituda_{i}.dat").T
    ax.plot(wartosci_m[i], 0, 'ro', markersize = 0.6, color = colors[i])
    ax.plot(data[1], data[2], color = colors[i]); ax.set_xlabel("x"); ax.set_ylabel("v")
    ax.plot(data[1,0], data[2,0], 'o', markersize = 1.2, color = colors[i])

fig.tight_layout()
plt.savefig("amplituda_xy.pdf")