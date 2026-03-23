import numpy as np
import matplotlib.pyplot as plt
import math
#from scipy.optimize import curve_fit
from typing import Callable
from itertools import product, permutations, combinations

def calculate_energy(spins: tuple, J: np.ndarray) -> float:
    
    E = 0
    size = len(spins)
    for idx in range(size):
        idx_plus = (idx + 1) % size
        
        E += -spins[idx] * (J[idx] * spins[idx_plus])
    return E

def ile_k_spinowo_stabilnych(spins_list, k):
    N = len(spins_list[0])
    
    J = np.random.normal(loc = 0, scale = 1, size = N)

    spins_to_flip = []
    for m in range(1, k+1):
        spins_to_flip += list(combinations(range(N), m))

    number = 0
    for spins in spins_list:
        
        E_0 = calculate_energy(spins, J)
        
        in_the_strict_minimum = True
        for spins_to_flip_now in spins_to_flip:
            spins_changed = list(spins)
            for idx in spins_to_flip_now:
                spins_changed[idx] *= -1

            E_now = calculate_energy(tuple(spins_changed), J)
            
            if E_now < E_0:
                in_the_strict_minimum = False
                break
        
        if in_the_strict_minimum:
            number += 1
    
    return number

# -------- DODATKOWO - REDUKCJA SPINOW DO ROWNOWAZNYCH POSTACI
# -------- TYLKO DLA SPINOW +-1

def reduction(spins: tuple | list, N: int):

    # redukcja wszystkich spinow do formy nazwijmy to podstawowej
    # - z internetu pomysl

    return min([spins[i:] + spins[:i] for i in range(N)]) # type: ignore

def unique_spins(N: int):

    # tutaj sa wszystkie mozliwosci 
    # wybrania UNIKATOWEGO lancucha spinow
    # tj. z dokladnoscia do "fazy" i znaku

    already_seen = set()
    unique_cycles = []
    possible_spins = product([1, -1], repeat = N)
    
    for spins in possible_spins:
        
        reducted = reduction(spins, N)

        if reducted not in already_seen:
            already_seen.add(reducted)
            unique_cycles.append(spins)

    return [unique for unique in unique_cycles if unique[0] == 1]

# ------------------------- WYNIKI ---------------------------

# ZADANIE 1

Ns = [5, 6]
Js = [-1, 1]
for N in Ns:
    for j_val in Js:
        print(f"\nJ = {j_val}, N = {N}")
        spins_list = list(product([1, -1], repeat = N))
        J_arr = np.full(N, j_val)

        for spins in spins_list:
            E = calculate_energy(spins, J_arr)
            print(f"{spins} -> E = {E}")

# ZADANIE 2

Ns = [9, 12, 15]
ks = [1, 2]
for k in ks:
    for N in Ns:
        spins_list = list(product([1, -1], repeat = N))
        ile = []
        for _ in range(100):
            ile_stabilnych = ile_k_spinowo_stabilnych(spins_list, k)
            ile.append(ile_stabilnych)

        theory = 2**(N/(2*k + 1))
        print(f"N = {N}, k = {k}, teor: {theory:.3f}, jest: {np.mean(ile)}")