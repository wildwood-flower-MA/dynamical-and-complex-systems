import numpy as np
import matplotlib.pyplot as plt
import math
from scipy.optimize import curve_fit
from numpy import random
from math import comb
from mpl_toolkits.axes_grid1 import make_axes_locatable

# do animacji
import os
from PIL import Image
from io import BytesIO
import shutil

# ----------------------------- ZADANIE 1.

N = 200
p = 0.5

fig, ax = plt.subplots(figsize = (8,4))

for it in range(5):

    x = 0
    pozycja = np.zeros(N)

    for n in range(N):

        pozycja[n] = x
        x += 1 if random.rand() < p else -1 # krok w lewo to -1, w prawo 1

    ax.plot(pozycja, color = 'k', alpha = 0.2 + 0.8*it/5)

ax.set_xlabel("krok")
ax.set_ylabel("pozycja")
fig.tight_layout()
plt.show()

# ----------------------------- ZADANIE 2.

N = 1000000
p = 0.5
po_ilu_krokach = 20

positions_Xth = np.zeros(N)
for it in range(N):

    x = 0
    for n in range(po_ilu_krokach):

        x += 1 if random.rand() < p else -1 # krok w lewo to -1, w prawo 1
    
    positions_Xth[it] = x

def P_teor(d):

    # comb to po prostu kombinacja bez powt. czyli nawias Newtona
    
    return (0.5**po_ilu_krokach)*comb(po_ilu_krokach, int((d + po_ilu_krokach)/2))

unique = np.unique(positions_Xth)
bins = np.zeros_like(unique)
for idx in range(N):

    idx_bin = np.where(unique == positions_Xth[idx])
    bins[idx_bin] += 1

fig, ax = plt.subplots(figsize = (4,4))
ax.plot(unique, bins, 'o', color = 'k', label = 'hist.')
ax.set_xlabel('pozycja po 20 krokach')
ax.set_ylabel('ile razy')

rozklad = np.asarray([P_teor(d) for d in unique])
ax.plot(unique, N*rozklad, 'x', alpha = 1, markersize = 4, color = 'r', label = r'P$_N$')
ax.legend(frameon = False)

plt.show()

# ----------------------------- ZADANIE 3.

def random_walk2D(M: int, N: int):

    area = np.zeros((M, M))
    x = 0
    y = 0

    for step in range(N):

        U = random.rand()
        if U < 0.25:
            x += 1 
        elif U < 0.5:
            x -= 1
        elif U < 0.75:
            y += 1
        elif U < 1:
            y -= 1  
        else:
            raise Exception("kurde balans")
        
        x = (M + x)%M
        y = (M + y)%M

        area[x, y] += 1

    return area

N = 1000000
M = 10
area = random_walk2D(M = M, N = N)

area_mean = np.sum(area)/M/M
area2_mean = np.sum(area*area)/M/M
stddev = np.sqrt(area2_mean - area_mean**2)

print(f"globalnie: {area_mean} ± {stddev:.3f}")

N = 1000000
M = 10
iters = 20

areas = []
for iteracja in range(iters):

    area = random_walk2D(M = M, N = N)
    areas.append(area/N)

area = sum(areas)
area2 = sum([area_n*area_n for area_n in areas])
stddev = np.sqrt((area2/iters - area*area/iters/iters))

# rysowanie
fig = plt.figure(figsize=(10, 4))
fig.suptitle(f"{iters} powtórzeń")

ax1 = fig.add_subplot(121)
ax1.set_title("średni procent odwiedzin")
ax1.set_xticks([])
ax1.set_yticks([])
im1 = ax1.imshow(area, interpolation='None', cmap='YlGn', origin='lower')
divider = make_axes_locatable(ax1)
cax = divider.append_axes('right', size='5%', pad=0.05)
fig.colorbar(im1, cax=cax, orientation='vertical')

ax2 = fig.add_subplot(122)
ax2.set_title("błąd")
ax2.set_xticks([])
ax2.set_yticks([])
im2 = ax2.imshow(stddev, interpolation='None')
divider = make_axes_locatable(ax2)
cax = divider.append_axes('right', size='5%', pad=0.05)
fig.colorbar(im2, cax=cax, orientation='vertical');

plt.show()

# wypisywanie tablicy
for i in range(M):
    row = [f"{area[i, j]:.6f}±{stddev[i, j]:.6f}" for j in range(M)] # type: ignore
    print(" | ".join(row))

# ----------------------------- ZADANIE 4.

def free_roam(N: int):

    x = 0
    y = 0

    for step in range(N):

        U = random.rand()
        if U < 0.25:
            x += 1 
        elif U < 0.5:
            x -= 1
        elif U < 0.75:
            y += 1
        elif U < 1:
            y -= 1

    return np.sqrt(x**2 + y**2)

powt = 10
l = 1
D = 2
eg = l*math.gamma((D + 1)/2)/math.gamma(D/2) # type: ignore

for n in [10, 100, 200]:

    dist_avg = 0

    for _ in range(powt):

        dist_avg += free_roam(N = n)

    dist_avg /= powt

    print(f"dla {n}: {dist_avg}; oczekiwana: {eg*np.sqrt(2*n/D)}")

# ----------------------------- ANIMACJA

def plot(M: int, N: int) -> None:

    # img = plt.imread("tlo.png")

    folder = 'symulacja'
    os.makedirs(folder, exist_ok=True)
    output_gif = 'chodziarz.gif'
    images = []

    area = np.zeros((M, M))
    x = M//2
    y = M//2

    for step in range(N):

        #area[x, y] = 0

        U = random.rand()
        if U < 0.25:
            x += 1 
        elif U < 0.5:
            x -= 1
        elif U < 0.75:
            y += 1
        elif U < 1:
            y -= 1  
        else:
            raise Exception("kurde balans")
        
        x = (M + x)%M
        y = (M + y)%M

        area[x, y] += 1

        fig, ax = plt.subplots(figsize = (6,6))
        ax.imshow(area, zorder=0, cmap='gray')
        ax.set_xticks([])
        ax.set_yticks([])

        buf = BytesIO()
        fig.savefig(buf, format='png', bbox_inches='tight', pad_inches=0)
        buf.seek(0)
        images.append(Image.open(buf))

        plt.close()

    if not images:
        pass
    else:
        try:
            images[0].save(output_gif, save_all=True, append_images=images[1:], duration=200)
            print(f"{output_gif} gotowy")
        except Exception as e:
            print(e)
            pass

plot(M = 40, N = 1000)