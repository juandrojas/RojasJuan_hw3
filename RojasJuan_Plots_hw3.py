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
t = data_ODE[:,0]
x = data_ODE[:,1]
y = data_ODE[:,2]
z = data_ODE[:,3]

fig.suptitle('Trayectoria de la partícula', fontsize=18)
ax.plot(x, y, z)
plt.savefig('ODE.pdf')
plt.clf()
plt.close()

#Gráficas Adicionales
plt.figure(figsize=(8, 12))
plt.subplot(311)
plt.plot(y, x)
plt.title('Gráfica de X vs Y', fontsize=18)
plt.subplot(312)
plt.plot(z, x)
plt.title('Gráfica de X vs Z', fontsize=18)
plt.subplot(313)
plt.title('Gráfica de Y vs T', fontsize=18)
plt.plot(y, t)
plt.subplots_adjust(hspace=0.5)
plt.savefig('ODE_additional.pdf')
plt.clf()
plt.close()
#PDE's

#condición inicial
data_PDE_init = np.genfromtxt('init.dat')
fig = plt.figure()
ax = fig.gca(projection = '3d')
x = np.arange(0.0, 1.0, 0.01)
y = np.arange(0.0, 1.0, 0.01)
x, y = np.meshgrid(x, y)
z = data_PDE_init
ax.plot_surface(x, y, z)
fig.suptitle('Condición inicial', fontsize=18)
fig.savefig('PDE_initial.pdf')
plt.clf()
plt.close()

#fixed
data_PDE_fixed = np.genfromtxt('data_PDE_fixed.dat')
fig = plt.figure()
ax = fig.gca(projection = '3d')
x = np.arange(0.0, 1.0, 0.01)
y = np.arange(0.0, 1.0, 0.01)
x, y = np.meshgrid(x, y)
z = data_PDE_fixed
ax.plot_surface(x, y, z)
fig.suptitle('Estado en t=60ms', fontsize=18)
fig.savefig('PDE_fixed.pdf')
plt.clf()
plt.close()
