import scipy as sp
import numpy as np
import os
import matplotlib.pyplot as plt

data = np.loadtxt(f"wyn/wyniki.txt").T
x = data[0]
y_a = data[1]
y_n = data[2]

fig, axes = plt.subplots(1, 3, figsize=(15, 5))

axes[1].plot(x, np.abs(y_a - y_n), 'o', label="blad bezwzgledny", markersize=0.2)

axes[0].plot(x, y_a, label="analityczne")
axes[0].plot(x, y_n, 'o',  label="numeryczne", markersize=0.2)
blad_wzgledny = np.zeros_like(y_a)
mask = y_a != 0
blad_wzgledny[mask] = np.abs((y_a[mask] - y_n[mask])/y_a[mask])
axes[2].plot(x[mask], blad_wzgledny[mask], 'o', label="blad wzgledny", markersize=0.2)
axes[2].set_yscale('log')
axes[0].grid()
axes[1].grid()
axes[2].grid()
fig.suptitle(f"y0 = {y_a[0]:.2}")

axes[0].legend(frameon = False)
axes[1].legend(frameon = False)
axes[2].legend(frameon = False)

plt.tight_layout()
plt.savefig("wyn/lab0.pdf", dpi=300)
plt.show()