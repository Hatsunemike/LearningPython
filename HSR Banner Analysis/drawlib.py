import matplotlib.pyplot as plt
import numpy as np

def drawDist(dist: np.array, fig, pos) :
    length = dist.shape[0]
    ax = fig.add_subplot(pos)
    ax.set_xlim([0,length])
    ax.set_ylim([0,1.5])
    ax.plot(np.arange(length), dist, color="b", linestyle="-")
