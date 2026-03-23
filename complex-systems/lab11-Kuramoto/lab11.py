import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit
from scipy.integrate import solve_ivp

def Kuramoto_simulation(n: int, k: float, t_range: tuple, t_N: int):

    # (1) definiowanie parametrow poczatkowych
    theta = [np.random.uniform(0, 2*np.pi) for _ in range(n)]
    omega = [np.random.normal(loc = 0, scale = 0.5) for _ in range(n)]
    
    # (2) definiowanie RHS ukladu rownan
    def ode(t, theta):

        sin_matrix = np.zeros((n, n))
        for i in range(n):
            for j in range(n):
                sin_matrix[i,j] = (k/n)*np.sin(theta[j] - theta[i])

        dtheta_dt = np.zeros(n)
        sum_RHS = np.sum(sin_matrix, axis = 1)
        for i in range(n):
            dtheta_dt[i] = omega[i] + sum_RHS[i]

        return dtheta_dt

    # (3) zakres czasu
    t = np.linspace(t_range[0], t_range[1], t_N)

    # (4) rozwiazanie ukladu rownan
    solution = solve_ivp(fun = ode,
                        t_span = t_range,
                        y0 = theta,
                        t_eval = t)

    theta_t = solution.y

    # (5) parametr porzadku
    re_ipsi = (1/n)*np.sum(np.exp(1j*theta_t), axis = 0)
    r = np.abs(re_ipsi)

    return t, theta_t, r

def Kuramoto_simulation_desync(n: int, k: float, t_range: tuple, t_N: int):

    # (1) definiowanie parametrow poczatkowych
    theta = [np.random.uniform(0, 2*np.pi) for _ in range(n//2)]\
            + [np.random.uniform(0, np.pi/12) for _ in range(n//2)]
    omega = [np.random.normal(loc = 0, scale = 0.5) for _ in range(n)]
    
    # (2) definiowanie RHS ukladu rownan
    def ode(t, theta):

        sin_matrix = np.zeros((n, n))
        for i in range(n):
            for j in range(n):
                sin_matrix[i,j] = (k/n)*np.sin(theta[j] - theta[i])

        dtheta_dt = np.zeros(n)
        sum_RHS = np.sum(sin_matrix, axis = 1)
        for i in range(n):
            dtheta_dt[i] = omega[i] + sum_RHS[i]

        return dtheta_dt

    # (3) zakres czasu
    t = np.linspace(t_range[0], t_range[1], t_N)

    # (4) rozwiazanie ukladu rownan
    solution = solve_ivp(fun = ode,
                        t_span = t_range,
                        y0 = theta,
                        t_eval = t)

    theta_t = solution.y

    # (5) parametr porzadku
    re_ipsi = (1/n)*np.sum(np.exp(1j*theta_t), axis = 0)
    r = np.abs(re_ipsi)

    return t, theta_t, r

N = [10, 20, 50]
K = np.arange(0.25, 5, 0.5).tolist()
t_range = (0, 100)
t_N = 40

for n in N:
    for k in K:
        
        t, theta_t, r = Kuramoto_simulation(n = n, k = k, t_range = t_range, t_N = t_N)

        # wykresy
        fig, ax = plt.subplots(1, 2, figsize = (8,4))
        for theta_n in theta_t:
            ax[0].plot(t, theta_n%(2*np.pi))
        ax[1].plot(t, r, color = 'k')
        ax[0].set_xlabel('t')
        ax[0].set_ylabel(r'$\vartheta$')
        ax[0].set_title(f"rozwiazanie")
        ax[1].set_xlabel('t')
        ax[1].set_ylabel('r')
        ax[1].set_ylim([0,1.1])
        ax[1].set_title(f"parametr porzadku")
        ax[0].grid()
        ax[1].grid()
        plt.suptitle(f"N = {n}, K = {k}")
        plt.show()

N = [10, 20, 50]
K = np.arange(0.25, 5, 0.5).tolist()
t_range = (0, 100)
t_N = 40

r_N = []
for n in N:
    r_average = []

    for k in K:
        r2average = []

        for iteration in range(10):
            t, theta_t, r = Kuramoto_simulation(n = n, k = k, t_range = t_range, t_N = t_N)
            r2average.append(r[-1])

        r_average.append(np.average(r2average))

    r_N.append(np.array(r_average))

fig, ax = plt.subplots(figsize = (4,4))
for idx, r_n in enumerate(r_N):
    ax.plot(K, r_n, label = f"N = {N[idx]}")
ax.legend(frameon = False)
ax.grid()
ax.set_xlabel('K')
ax.set_ylabel(r'r$_{max}$')
fig.tight_layout()
plt.show()

K = [0.01, 0.8, 2]

colors = ['k', 'r', 'b']
fig, ax = plt.subplots(figsize = (6,4))
for idx, k in enumerate(K):
    t, theta_t, r = Kuramoto_simulation_desync(n = 50, k = k, t_range = (0,100), t_N = 50)
    ax.plot(t, r, color = colors[idx], alpha = 1)
    t, theta_t, r = Kuramoto_simulation_desync(n = 50, k = k, t_range = (0,100), t_N = 50)
    ax.plot(t, r, color = colors[idx], alpha = 0.67)
    t, theta_t, r = Kuramoto_simulation_desync(n = 50, k = k, t_range = (0,100), t_N = 50)
    ax.plot(t, r, color = colors[idx], label = f"K = {k}", alpha = 0.33)
    
ax.set_xlabel('t')
ax.set_ylabel('r')
ax.set_ylim((0,1))
ax.legend(frameon = True, fancybox = False)
ax.set_title(f"parametr porzadku")
ax.grid()