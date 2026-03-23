import numpy as np
import types
import matplotlib.pyplot as plt

N = 10
zad1_1 = np.zeros((N,N))
for n in range(N):
    for m in range(N):
        if m!=n:
            zad1_1[n,m] = 1

print(zad1_1)

# źle!!!
for n in range(N):
    plt.plot(n, np.sum(zad1_1[n]), 'o')
plt.show()

N = 10
def wymysl_siasiadow(N: int, n: int) -> list:
    sasiedzi = []
    for _n in range(N-1):
        if 3*np.random.rand() <= 1:
            if N != _n:
                sasiedzi.append(_n+1)

    return sasiedzi

lista_sasiedztwa = np.empty(N, dtype = list)
for n, sasiad in enumerate(lista_sasiedztwa):
    lista_sasiedztwa[n] = wymysl_siasiadow(N, n)

print(lista_sasiedztwa)

for n in range(N):
    for m in range(len(lista_sasiedztwa[n])):
        plt.plot(n, lista_sasiedztwa[n][m], 'o')
plt.show()

N = 10

# kwadrat
tablica_zera = [None]*N
for n in range(N):
    tablica_zera[n] = [0]*N
for n in range(1,N-1):
    for m in range(1,N-1):
        tablica_zera[n][m] = 1
print(tablica_zera)

# przekatna
tablica_zera = [None]*N
for n in range(N):
    tablica_zera[n] = [0]*N
for n in range(N):
        tablica_zera[n][n] = 1
print(tablica_zera)

# przekatna 2
tablica_zera = [None]*N
for n in range(N):
    tablica_zera[n] = [0]*N
for n in range(N):
        tablica_zera[n][N-1-n] = 1
print(tablica_zera)

# przekatne
tablica_zera = [None]*N
for n in range(N):
    tablica_zera[n] = [0]*N
for n in range(N):
        tablica_zera[n][N-1-n] = 1
        tablica_zera[n][n] = 1
print(tablica_zera)

# szachownica
tablica_zera = [None]*N
for n in range(N):
    tablica_zera[n] = [0]*N
for n in range(N):
    for m in range(N):
        if (m%2 and n%2) or (not m%2 and not n%2):
            tablica_zera[n][m] = 1
print(tablica_zera)

