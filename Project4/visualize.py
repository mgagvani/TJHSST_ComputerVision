import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import cv2

SCALEFACTOR = 0.1

# generate some sample data
import scipy.misc
img = cv2.imread("./l04.jpg", 0)

# downscaling has a "smoothing" effect
newshape = (151, 202)
img = cv2.resize(img, newshape)

# create the x and y coordinate arrays (here we just use pixel indices)
xx, yy = np.mgrid[0:img.shape[0], 0:img.shape[1]]

# create the figure
fig = plt.figure()
ax = fig.gca(projection='3d')
ax.plot_surface(xx, yy, img ,rstride=1, cstride=1, cmap=plt.cm.jet,
                linewidth=0)

# show it
plt.show()
'''
import numpy as np
from matplotlib import cm
import matplotlib.pyplot as plt

cols = 2016
rows = 1512

G = np . loadtxt('g.csv' , delimiter=',' , skiprows=0)

C = np.arange(0,cols,1)
R = np.arange(0,rows,1)
C, R = np.meshgrid(C, R)



fig = plt.figure()
ax = plt.axes(projection = '3d')
ax . plot_surface(C , R , G , cmap=cm.coolwarm)

plt.show()
'''