import numpy as np
import matplotlib.pyplot as plt
import math

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

def generate_graph(N, alpha):

    p = alpha/N
    graph = np.triu((np.random.uniform(0, 1, (N, N)) < p).astype(int), 1)
    graph += graph.T

    return p, graph

def p_k_1(k: int, N: int, p: float):
    
    binomial_coeffs = math.factorial(N - 1)/math.factorial(k)/math.factorial(N - 1 - k)

    return binomial_coeffs*(p**k)*((1 - p)**(N - 1 - k))

def p_k_2(k: int, k_av: float):
    
    return np.exp(-k_av)*(k_av**k)/math.factorial(k)

def N_distances(d: float, k_av: float):

    return (k_av**(d + 1) - 1)/(k_av - 1)

def d_matrix(graph: np.ndarray):

    N = np.shape(graph)[0]
    d = np.zeros((N,N))

    d = [[0 if i == j else (graph[i][j] if graph[i][j] else math.inf) for j in range(N)] for i in range(N)]

    for k in range(N):
        for i in range(N):
            for j in range(N):
                if d[i][j] > d[i][k] + d[k][j]:
                    d[i][j] = d[i][k] + d[k][j]
                    
    return d

def liczba_n_katow(graph: np.ndarray, n: int):

    A2nth_power = np.linalg.matrix_power(graph, n)
    return np.trace(A2nth_power)/2/n

def analiza_grafu(graph: np.ndarray):
    
    N = np.shape(graph)[0]
    liczba_wiazan = np.sum(graph)//2
    sredni_stopien_wierzcholka = 2*liczba_wiazan/N

    stopnie_wierzcholka = np.sum(graph, axis = 0)
    linsp_stopnie_wierzcholka = np.arange(0, np.max(stopnie_wierzcholka) + 1)
    rozklad_stopni_wierzcholka = np.array([np.sum(stopnie_wierzcholka == value)
                                        for value in linsp_stopnie_wierzcholka])
    rozklad_stopni_wierzcholka = rozklad_stopni_wierzcholka.astype(float)/np.sum(rozklad_stopni_wierzcholka)

    macierz_odleglosci = d_matrix(graph)

    srednia_odleglosc_miedzy_wierzcholkami = 0; n_srednia = 0
    for i in range(N):
        for j in range(N):
            if macierz_odleglosci[i][j] != math.inf:
                n_srednia += 1
                srednia_odleglosc_miedzy_wierzcholkami += macierz_odleglosci[i][j]
    srednia_odleglosc_miedzy_wierzcholkami /= n_srednia

    ilo_kat = 3
    liczba_trojkatow = liczba_n_katow(graph, n = ilo_kat)

    return (liczba_wiazan,
            sredni_stopien_wierzcholka,
            stopnie_wierzcholka,
            linsp_stopnie_wierzcholka,
            rozklad_stopni_wierzcholka,
            srednia_odleglosc_miedzy_wierzcholkami,
            liczba_trojkatow)

def analiza_grafu_oczekiwana(N, alpha):

    p = alpha/N
    ilo_kat = 3
    oczekiwana_liczba_wiazan = p*N*(N - 1)/2
    oczekiwany_sredni_stopien_wierzcholka = p*(N - 1)
    oczekiwana_srednia_odleglosc_miedzy_wierzcholkami = np.log(N)/np.log(oczekiwany_sredni_stopien_wierzcholka)
    oczekiwana_liczba_trojkatow = math.factorial(N)/math.factorial(ilo_kat)/math.factorial(N - ilo_kat)*p**3

    return (oczekiwana_liczba_wiazan,
            oczekiwany_sredni_stopien_wierzcholka,
            oczekiwana_srednia_odleglosc_miedzy_wierzcholkami,
            oczekiwana_liczba_trojkatow)

def analiza_wielu_grafow(N, alpha, N_powt):
    
    n_liczba_wiazan = 0
    n_sredni_stopien_wierzcholka = 0
    n_stopnie_wierzcholka = np.zeros(0)
    n_srednia_odleglosc_miedzy_wierzcholkami = 0
    n_liczba_trojkatow = 0

    pominiete = 0
    for i in range(N_powt):

        p, graf = generate_graph(N, alpha)

        (liczba_wiazan,
        sredni_stopien_wierzcholka,
        stopnie_wierzcholka,
        linsp_stopnie_wierzcholka,
        rozklad_stopni_wierzcholka,
        srednia_odleglosc_miedzy_wierzcholkami,
        liczba_trojkatow) = analiza_grafu(graf)

        # Pani Profesor! Zgodnie z tym, co padło na zajęciach
        # - tutaj po prostu nie są brane do średniej odległości
        # wartości nieskończone; zakomentowany fragment - 
        # tutaj i niżej powinien realizować "uzupełnienie"
        '''
        if np.any(stopnie_wierzcholka == 0):
            pominiete += 1
        else:
            n_srednia_odleglosc_miedzy_wierzcholkami += srednia_odleglosc_miedzy_wierzcholkami
        '''
        n_srednia_odleglosc_miedzy_wierzcholkami += srednia_odleglosc_miedzy_wierzcholkami

        n_liczba_wiazan += liczba_wiazan
        n_sredni_stopien_wierzcholka += sredni_stopien_wierzcholka

        n_liczba_trojkatow += liczba_trojkatow
        n_stopnie_wierzcholka = np.append(n_stopnie_wierzcholka, stopnie_wierzcholka)

    '''
    uzupelnione_pominite = 0
    while(uzupelnione_pominite != pominiete):

        p, graf = generate_graph(N, alpha)

        (liczba_wiazan,
        sredni_stopien_wierzcholka,
        stopnie_wierzcholka,
        linsp_stopnie_wierzcholka,
        rozklad_stopni_wierzcholka,
        srednia_odleglosc_miedzy_wierzcholkami,
        liczba_trojkatow) = analiza_grafu(graf)

        if np.any(stopnie_wierzcholka == 0):
            continue

        n_srednia_odleglosc_miedzy_wierzcholkami += srednia_odleglosc_miedzy_wierzcholkami
        uzupelnione_pominite += 1
    '''

    n_liczba_wiazan /= N_powt
    n_sredni_stopien_wierzcholka /= N_powt
    n_srednia_odleglosc_miedzy_wierzcholkami /= N_powt
    n_liczba_trojkatow /= N_powt

    n_linsp_stopnie_wierzcholka = np.arange(0, np.max(n_stopnie_wierzcholka) + 1)
    n_rozklad_stopni_wierzcholka = np.array([np.sum(n_stopnie_wierzcholka == value)
                                        for value in n_linsp_stopnie_wierzcholka])
    n_rozklad_stopni_wierzcholka = n_rozklad_stopni_wierzcholka.astype(float)/np.sum(n_rozklad_stopni_wierzcholka)

    return (n_liczba_wiazan,
            n_sredni_stopien_wierzcholka,
            n_stopnie_wierzcholka,
            n_linsp_stopnie_wierzcholka,
            n_rozklad_stopni_wierzcholka,
            n_srednia_odleglosc_miedzy_wierzcholkami,
            n_liczba_trojkatow)

def generuj_wyniki(N: int, alpha: int) -> None:
    
    p, graf = generate_graph(N, alpha)
    visualize_graph(graf)

    (liczba_wiazan,
    sredni_stopien_wierzcholka,
    stopnie_wierzcholka,
    linsp_stopnie_wierzcholka,
    rozklad_stopni_wierzcholka,
    srednia_odleglosc_miedzy_wierzcholkami,
    liczba_trojkatow) = analiza_grafu(graf)

    (oczekiwana_liczba_wiazan,
    oczekiwany_sredni_stopien_wierzcholka,
    oczekiwana_srednia_odleglosc_miedzy_wierzcholkami,
    oczekiwana_liczba_trojkatow) = analiza_grafu_oczekiwana(N, alpha)

    print(f"oczekiwana liczba wiązań: {oczekiwana_liczba_wiazan}")
    print(f"liczba wiązań: {liczba_wiazan}")
    print(f"oczekiwany średni stopień wierzchołka: {oczekiwany_sredni_stopien_wierzcholka}")
    print(f"średni stopień wierzchołka: {sredni_stopien_wierzcholka}")
    print(f"oczekiwana średnia odległość między wierzchołkami: {oczekiwana_srednia_odleglosc_miedzy_wierzcholkami}")
    print(f"średnia odległość między wierzchołkami: {srednia_odleglosc_miedzy_wierzcholkami}")
    print(f"oczekiwana liczba trójkątów: {oczekiwana_liczba_trojkatow}")
    print(f"liczba trójkątów: {liczba_trojkatow}")

    fig, ax = plt.subplots(figsize = (4,4))

    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_1(k, N, p) for k in linsp_stopnie_wierzcholka],
            '--', color='b', alpha = 0.5)
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_1(k, N, p) for k in linsp_stopnie_wierzcholka],
            'o', color='b', alpha = 0.5, label = r'p$_k$ (r. dwumianowy)')
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_2(int(k), sredni_stopien_wierzcholka) for k in linsp_stopnie_wierzcholka],
            '--', color='r', alpha = 0.5)
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_2(int(k), sredni_stopien_wierzcholka) for k in linsp_stopnie_wierzcholka],
            'o', color='r', alpha = 0.5, label = r'p$_k$ (r. Poissona)')
    ax.plot(linsp_stopnie_wierzcholka,
            rozklad_stopni_wierzcholka, 'o', color = 'k')
    ax.set_xlabel("stopień wierzchołka")
    ax.set_ylabel("rozkład stopni wierzchołka")
    ax.legend(fontsize='small')
    plt.show()

def generuj_wyniki_srednie(N: int, alpha: int, N_powt: int) -> None:

    p = alpha/N

    (liczba_wiazan,
    sredni_stopien_wierzcholka,
    stopnie_wierzcholka,
    linsp_stopnie_wierzcholka,
    rozklad_stopni_wierzcholka,
    srednia_odleglosc_miedzy_wierzcholkami,
    liczba_trojkatow) = analiza_wielu_grafow(N, alpha, N_powt)

    (oczekiwana_liczba_wiazan,
    oczekiwany_sredni_stopien_wierzcholka,
    oczekiwana_srednia_odleglosc_miedzy_wierzcholkami,
    oczekiwana_liczba_trojkatow) = analiza_grafu_oczekiwana(N, alpha)

    print(f"oczekiwana liczba wiązań: {oczekiwana_liczba_wiazan}")
    print(f"średnia liczba wiązań: {liczba_wiazan}")
    print(f"oczekiwany średni stopień wierzchołka: {oczekiwany_sredni_stopien_wierzcholka}")
    print(f"średni stopień wierzchołka: {sredni_stopien_wierzcholka}")
    print(f"oczekiwana średnia odległość między wierzchołkami: {oczekiwana_srednia_odleglosc_miedzy_wierzcholkami}")
    print(f"średnia odległość między wierzchołkami: {srednia_odleglosc_miedzy_wierzcholkami}")
    print(f"oczekiwana liczba trójkątów: {oczekiwana_liczba_trojkatow}")
    print(f"średnia liczba trójkątów: {liczba_trojkatow}")

    fig, ax = plt.subplots(figsize = (4,4))

    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_1(int(k), N, p) for k in linsp_stopnie_wierzcholka],
            '--', color='b', alpha = 0.5)
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_1(int(k), N, p) for k in linsp_stopnie_wierzcholka],
            'o', color='b', alpha = 0.5, label = r'p$_k$ (r. dwumianowy)')
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_2(int(k), sredni_stopien_wierzcholka) for k in linsp_stopnie_wierzcholka],
            '--', color='r', alpha = 0.5)
    ax.plot(linsp_stopnie_wierzcholka,
            [p_k_2(int(k), sredni_stopien_wierzcholka) for k in linsp_stopnie_wierzcholka],
            'o', color='r', alpha = 0.5, label = r'p$_k$ (r. Poissona)')
    ax.plot(linsp_stopnie_wierzcholka,
            rozklad_stopni_wierzcholka, 'o', color = 'k')
    ax.set_xlabel("stopień wierzchołka")
    ax.set_ylabel("uśredniony rozkład stopni wierzchołka")
    ax.set_title(f"N = {N}, alpha = {alpha}, N_powt = {N_powt}")
    ax.legend(fontsize='small')
    plt.show()

print(f"alpha = 4")
generuj_wyniki_srednie(N = 100, alpha = 4, N_powt = 100)
print()
print(f"alpha = 10")
generuj_wyniki_srednie(N = 100, alpha = 10, N_powt = 100)