import scipy as sp
import numpy as np
import os
import matplotlib.pyplot as plt

def eq1(theta, omega):

    U = omega
    V = -theta

    return U, V

def eq2(theta, omega):

    U = omega
    V = (theta**3)/6 - theta

    return U, V

def eq3(theta, omega):

    U = omega
    V = -np.sin(theta)

    return U, V

def calculate_E(theta, omega):

    m = 1 # kg
    l = 9.8 # m
    g = 9.8 # m/s^2

    for i, om in enumerate(omega):
        omega[i] = omega[i] if omega[i] < 100 else 0

    for i, om in enumerate(theta):
        theta[i] = theta[i] if theta[i] < 100 else 0

    Ep = m*l*g*(1.0-np.cos(theta))
    Ek = 0.5*m*l*l*omega**2

    return Ep, Ek

if __name__ == "__main__":

    funcs = [eq1, eq2, eq3]
    theta = np.linspace(-10, 10, 30)
    omega = np.linspace(-5, 5, 15)

    Theta, Omega = np.meshgrid(theta, omega)

    fig, ax = plt.subplots(1,3, figsize = (12, 4))
    for i, fun in enumerate(funcs):

        U, V = fun(Theta, Omega)
        norm = 1/np.sqrt(U**2 + V**2)
        ax[i].quiver(Theta, Omega, norm*U, norm*V)

    ax[0].set_ylabel(r"$\dot{\theta}$")
    ax[1].set_xlabel(r"$\dot{\omega}$")
    ax[1].set_yticks([])
    ax[2].set_yticks([])
    fig.tight_layout()
    plt.savefig("0.pdf")
    plt.show()

    fig, ax = plt.subplots(1,3, figsize = (12, 4))

    omegas = [10.0*(0.5 + i*0.5) for i in range(6)]

    for i in range(6):
        data = np.loadtxt(f"lab_{i}.dat").T

        ax[0].plot(data[0], data[1])
        ax[0].set_xlabel("t")
        ax[0].set_ylabel(r"$\theta$")
        ax[0].grid(True)

        ax[1].set_xlabel("t")
        ax[1].plot(data[0], data[2]) 
        ax[1].set_ylabel(r"$\omega$")
        ax[1].grid(True)

        ax[2].plot(data[1], data[2])
        ax[2].set_xlabel(r"$\omega$")
        ax[2].set_ylabel(r"$\theta$")
        ax[2].grid(True)

    omega_lim = 4
    theta_lim = 4

    ax[0].set_ylim(-theta_lim, theta_lim)
    ax[1].set_ylim(-omega_lim, omega_lim)
    ax[2].set_xlim(-omega_lim, omega_lim)
    ax[2].set_ylim(-theta_lim, theta_lim)

        #ax[i, 2].axis('equal')

    fig.tight_layout()
    plt.savefig("3.pdf")
    plt.show()

    omegas = [(0.5 + i*0.5) for i in range(6)]

    fig, ax = plt.subplots(1,2, figsize = (8, 4))
    for i in range(6):
        
        data = np.loadtxt(f"lab_{i}.dat").T
        ep, ek = calculate_E(data[1], data[2])
        energy = ep + ek

        ax[0].plot(data[0], energy, alpha = 1, label = r"$\omega_0=$" + f"{omegas[i]}")
    ax[0].set_xlabel("t (s)")
    ax[0].set_ylabel(r"$E$ $(kg\ m^2/s^2)$")
    ax[0].grid(True)
    ax[0].set_ylim(0, 500)
    ax[0].legend(frameon = False)

    data = np.loadtxt(f"lab_0.dat").T
    ep, ek = calculate_E(data[1], data[2])
    energy = ep + ek

    ax[1].plot(data[0], energy - energy[0], alpha = 1)
    ax[1].set_xlabel("t (s)")
    ax[1].set_ylabel(r"$\Delta E$ $(kg\ m^2/s^2)$")
    ax[1].grid(True)

    fig.tight_layout()
    plt.savefig("E3_final.pdf")
    plt.show()