import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt

data_ODE = np.genfromtxt('data_ODE.dat')
fig = plt.figure()
ax = fig.gca(projection='3d')

# Prepare arrays x, y, z
x = data_ODE[:,1]
y = data_ODE[:,2]
z = data_ODE[:,3]

ax.plot(x, y, z, label='parametric curve')
ax.legend()
plt.savefig('ODE.pdf')
