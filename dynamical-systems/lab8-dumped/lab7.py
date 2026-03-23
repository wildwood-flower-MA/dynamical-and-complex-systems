import numpy as np
import matplotlib.pyplot as plt
from typing import Tuple
from types import SimpleNamespace
plt.style.use('default')
import os
from PIL import Image
from io import BytesIO
import shutil


def next_values(L: float, Theta: float, pm: SimpleNamespace) -> Tuple[float, float]:

    if pm.b == 0:
        Lew = (L + pm.K*np.sin(Theta))
        Thetaew = Theta + (L + pm.K*np.sin(Theta))
    else:
        Lew = (L + pm.K*np.sin(Theta))*np.exp(-pm.b)
        Thetaew = Theta + (L + pm.K*np.sin(Theta))*(1 - np.exp(-pm.b))/pm.b

    return (Lew, Thetaew)

steps = 1001
folder = 'figures'
os.makedirs(folder, exist_ok=True)


for b in [0.0, 0.01, 0.001]:
    for k in [0.5, 0.971635, 5.0]:
        
        fig = plt.figure(figsize=(12, 12))
        gs = fig.add_gridspec(3, 3)
        fig.suptitle(f'K = {k}, b = {b}', fontsize=16)

        axes_L = [fig.add_subplot(gs[0, i]) for i in range(3)]
        axes_th = [fig.add_subplot(gs[1, i]) for i in range(3)]
        
        start_points = list(zip([0.0, 0.2*2.0*np.pi, 0.2*2.0*np.pi], 
                                [0.1*2.0*np.pi, 0.0, 0.65*2.0*np.pi]))

        for i, r0 in enumerate(start_points):
            pm = SimpleNamespace(K=k, b=b)
            r = np.zeros((2, steps))
            r[:, 0] = np.array(r0[::-1])
            
            for n in range(1, steps):
                r[:, n] = next_values(r[0, n-1], r[1, n-1], pm)

            axes_L[i].scatter(np.arange(steps), r[0, :], s=0.5, color = 'k', lw=0.8)
            axes_L[i].set_title(r"($\theta_0, L_0$) = " + f"({r0[0]/np.pi:.1f}$\pi$, {r0[1]/np.pi:.1f}$\pi$)")
            axes_L[i].set_xlim(0, 1000)
            axes_L[i].grid(True, alpha=0.3)
            axes_th[i].scatter(np.arange(steps), r[1, :]%(2*np.pi)/np.pi, s=0.5, c='k')
            axes_th[i].set_xlim(0, 1000)
            axes_th[i].set_ylim(0, 2)
            axes_th[i].set_xlabel('n')

        axes_L[0].set_ylabel(r'$L$')
        axes_th[0].set_ylabel(r'$\theta$ [$\pi$]')

        ax_poincare = fig.add_subplot(gs[2, :])
        a_values = np.arange(0, 1.0, 0.02)
        
        cmap = plt.get_cmap('viridis')
        cmap_colors = cmap(np.linspace(0, 1, len(a_values)))

        for idx, a in enumerate(a_values):
            r0 = (0.5*2.0*np.pi, a*2.0*np.pi)
            
            pm = SimpleNamespace(K=k, b=b)
            r = np.zeros((2, steps))
            r[:, 0] = np.array(r0[::-1])
            
            for n in range(1, steps):
                r[:, n] = next_values(r[0, n-1], r[1, n-1], pm)

            theta_mod = r[1, :]%(2*np.pi)
            L_vals = r[0, :]

            ax_poincare.scatter(theta_mod/np.pi, L_vals, s=0.1, c=cmap_colors[idx], marker='.')
            
        cbar = fig.colorbar(plt.cm.ScalarMappable(cmap=cmap), ax=ax_poincare, orientation='vertical', pad = 0.01)
        cbar.set_label('$a$')

        ax_poincare.set_xlabel(r'$\theta$ [$\pi$]')
        ax_poincare.set_ylabel(r'$L$')
        ax_poincare.set_xlim(0, 2)

        plt.tight_layout()
        plt.savefig(f'figures/poincare_K_{k}_b_{b}.png', dpi=300)
        plt.show()


steps = 1000
b_values = [0.0, 0.001, 0.01] 
K_values = np.linspace(0.0, 2.2, 50)
a_values = np.linspace(0, 0.98, 40)

folder = 'anim_poincare'
os.makedirs(folder, exist_ok=True)

for b in b_values:
    frames = []
    for k_idx, k in enumerate(K_values):

        fig, ax = plt.subplots(figsize=(5, 5)) 
        fig.suptitle(f'K = {k:.2f}, b = {b}', fontsize=16)
        pm = SimpleNamespace(K=k, b=b)
        l_traj = np.zeros((len(a_values), steps))
        th_traj = np.zeros((len(a_values), steps))
        l_traj[:, 0] = a_values*(2.0 * np.pi)
        th_traj[:, 0] = 0.5*2.0*np.pi
        
        for n in range(1, steps):
            nxt = next_values(l_traj[:, n-1], th_traj[:, n-1], pm) #type:ignore
            l_traj[:, n] = nxt[0]
            th_traj[:, n] = nxt[1]
            
        plot_th = th_traj.flatten() % (2 * np.pi)
        plot_L = l_traj.flatten()
        ax.scatter(plot_th/np.pi, plot_L, s=0.1, color='k', marker='.')
        ax.set_xlim(0, 2)
        ax.set_xlabel(r'$\theta$ [$\pi$]', fontsize=14)
        ax.set_ylabel(r'$L$', fontsize=14)
        ax.grid(True, alpha=0.3)

        plt.tight_layout()
        buf = BytesIO()
        plt.savefig(buf, format='png', dpi=100)
        buf.seek(0)
        frames.append(Image.open(buf))
        plt.close(fig)

    output_filename = os.path.join(folder, f'poincare_b_{b}.gif')
    frames[0].save(output_filename,
                save_all=True,
                append_images=frames[1:],
                duration=100, loop=0)