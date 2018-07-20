import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import cm
from matplotlib.ticker import LinearLocator, FormatStrFormatter

data_ODE = np.genfromtxt('data_ODE.dat')
fig = plt.figure()
ax = fig.gca(projection='3d')

# Prepare arrays x, y, z
x = data_ODE[:,1]
y = data_ODE[:,2]
z = data_ODE[:,3]

fig.suptitle('Trayectoria de la partícula')
ax.plot(x, y, z)
plt.savefig('ODE.pdf')
plt.clf()
plt.close()

#PDE's

#condición inicial
data_PDE_init = np.genfromtxt('init.dat')
data_PDE_fixed = np.genfromtxt('data_PDE_fixed.dat')
fig = plt.figure()
ax = fig.gca(projection = '3d')
x = np.arange(0.0, 1.0, 0.01)
y = np.arange(0.0, 1.0, 0.01)
x, y = np.meshgrid(x, y)
z1 = data_PDE_init
z2 = data_PDE_fixed

ax.plot_surface(x, y, z2)
fig.savefig('PDE_initial.pdf')
