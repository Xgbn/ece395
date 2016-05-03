import matplotlib as mpl
from mpl_toolkits.mplot3d import Axes3D
import numpy as np
import matplotlib.pyplot as plt


mpl.rcParams['legend.fontsize'] = 10

# fig = plt.figure()
# ax = fig.gca(projection='3d')
data = np.genfromtxt('log.txt', delimiter=',', names=True)
# ax.plot(data['X'], data['Y'], label='trace')
# ax.legend()
# ax.set_xlabel('X')
# ax.set_ylabel('Y')
# plt.plot(data['time'], data['X'], label='X')
# plt.plot(data['time'], data['Y'], label='Y')
# plt.plot(data['time'], data['Z'], label='Z')
# ax.set_zlabel('Z')
plt.plot(data['time'], data['aX'], marker='o', label='aX')
plt.plot(data['time'], data['aY'], marker='o', label='aY')
plt.plot(data['time'], data['aZ'], marker='o', label='aZ')



plt.legend()
plt.show()
