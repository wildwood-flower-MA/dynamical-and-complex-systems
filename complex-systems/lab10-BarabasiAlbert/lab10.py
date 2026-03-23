import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit

def visualize_graph(graph: np.ndarray):

    N = np.shape(graph)[0]
    phis = np.linspace(0, 2*np.pi, N, endpoint=False)

    x = np.sin(phis)
    y = np.cos(phis)

    fig, ax = plt.subplots(figsize=(4, 4))
    for i in range(N):
        for j in range(i + 1, N):
            if graph[i, j] == 1:
                ax.plot([x[i], x[j]], [y[i], y[j]], 'k-', linewidth=0.5)

    for i in range(N):
        ax.plot(x[i], y[i], 'o', color='k', markersize=2)

    ax.axis('off')
    plt.show()

def generate_graph(N, m_0, m):

    def integral_for_probability(graph):
        stopnie_wierzcholka = np.sum(graph, axis = 0)

        integral_all = np.sum(stopnie_wierzcholka)
        integral = np.zeros(N)
        for k in range(N):
            integral[k] = np.sum(stopnie_wierzcholka[:(k + 1)])
        integral /= integral_all

        return integral

    graph = np.zeros((N,N), dtype = int)
    graph[:m_0,:m_0] = np.ones((m_0, m_0), dtype = int) - np.identity(m_0, dtype = int)

    for i in range(m_0, N):

        for _ in range(m):

            integral = integral_for_probability(graph)
            idx = i
            while idx == i:
                U = np.random.uniform(0, 1)
                idxs = np.where(integral > U)[0]
                idx = idxs[0]

            graph[idx,i] = 1
            graph[i, idx] = 1

    return graph

def analiza_grafu(graph: np.ndarray):
    
    N = np.shape(graph)[0]
    visited = np.zeros(N)
    def dfs_algorithm(u, visited):

        visited[u] = 1
        sasiedzi = np.where(graph[u] == 1)[0]

        for sasiad in sasiedzi:
            if visited[sasiad] == 0:
                dfs_algorithm(sasiad, visited)

        return visited

    stopnie_wierzcholka = np.sum(graph, axis = 0)
    linsp_stopnie_wierzcholka = np.arange(0, np.max(stopnie_wierzcholka) + 1)
    rozklad_stopni_wierzcholka = np.array([np.sum(stopnie_wierzcholka == value)
                                        for value in linsp_stopnie_wierzcholka])
    rozklad_stopni_wierzcholka = rozklad_stopni_wierzcholka.astype(float)/np.sum(rozklad_stopni_wierzcholka)

    random_idx = np.random.randint(0, N)
    dfs_algorithm(random_idx, visited)
    if np.any(visited == 0):
        spojny = 0
    else:
        spojny = 1

    sasiedzi = np.where(graph == 1)
    sasiedzi = zip(sasiedzi[0], sasiedzi[1])
    sasiedzi_list = list(sasiedzi)

    random_idx = np.random.randint(1, len(sasiedzi_list))
    wylosowany_sasiad = sasiedzi_list[random_idx]
    graph[wylosowany_sasiad[0], wylosowany_sasiad[1]] = 0
    graph[wylosowany_sasiad[1], wylosowany_sasiad[0]] = 0

    visited_2 = np.zeros(N)
    random_idx_2 = np.random.randint(0, N)
    dfs_algorithm(random_idx_2, visited_2)
    if np.any(visited_2 == 0):
        spojny_po_usunieciu = 0
    else:
        spojny_po_usunieciu = 1

    graph[wylosowany_sasiad[0], wylosowany_sasiad[1]] = 1
    graph[wylosowany_sasiad[1], wylosowany_sasiad[0]] = 1

    return (stopnie_wierzcholka,
            linsp_stopnie_wierzcholka,
            rozklad_stopni_wierzcholka,
            spojny,
            spojny_po_usunieciu)

graph = generate_graph(100, 5, 4)
visualize_graph(graph)

graph = generate_graph(100, 2, 1)
visualize_graph(graph)

(stopnie_wierzcholka,
linsp_stopnie_wierzcholka,
rozklad_stopni_wierzcholka,
spojny,
spojny_po_usunieciu) = analiza_grafu(graph)

print(f"Czy jest spojny? {'TAK' if spojny else 'NIE'}")
print(f"Czy jest spojny po usunieciu jednego wiazania? {'TAK' if spojny_po_usunieciu else 'NIE'}")

fig, ax = plt.subplots(figsize = (4,4))
ax.plot(linsp_stopnie_wierzcholka, rozklad_stopni_wierzcholka, '--', color = 'k')
ax.plot(linsp_stopnie_wierzcholka, rozklad_stopni_wierzcholka, 'o', color = 'k')
ax.set_xlabel("stopień wierzchołka")
ax.set_ylabel("rozkład stopni wierzchołka")
plt.show()

data = np.loadtxt("rozklad.dat")
data = np.array([data[i] for i in range(len(data)) if 20 < data[i,0] < 100])
data = data.T

data_plot = np.loadtxt("rozklad.dat").T

data_wykres = np.loadtxt("rozklad.dat")
data_wykres = np.array([data_wykres[i] for i in range(len(data_wykres)) if 3 < data_wykres[i,0] < 400]).T

def fun(k, alpha, c):
    return c*k**(-alpha)

ks = data[0][(data[0] > 0) & (data[1] > 0)]
ps = data[1][(data[0] > 0) & (data[1] > 0)]

popt, pcov = curve_fit(fun, ks, ps)
alpha = popt[0]

fig, ax = plt.subplots(figsize = (4,4))
ax.plot(data_plot[0], data_plot[1], 'o', label='rozkład stopni', color = 'k')
ax.plot(data_wykres[0], fun(data_wykres[0], *popt), '-',
        label = r"fit, $\alpha$" + f" = {alpha:.2f}", color = 'g')
ax.legend()
ax.set_xlabel('stopien')
ax.set_ylabel('rozklad')
ax.set_xscale('log')
ax.set_yscale('log')
ax.grid()
plt.show()
