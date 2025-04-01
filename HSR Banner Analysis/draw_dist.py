import numpy as np
import matplotlib.pyplot as plt
plt.rcParams["font.family"]="SimHei"

p = 0.016
n = 100
dist = np.zeros((n,),dtype=np.float64)

def drawTimesDist(dist: np.array, max_y: float, title: str) :
    length = dist.shape[0]
    fig = plt.figure(num=1, figsize=(1,1))
    ax = fig.add_subplot(111)
    ax.set_title(title,fontsize=40)
    ax.set_xlim([0,length])
    ax.set_ylim([0,max_y])
    # ax.scatter(np.arange(length), dist, color="b", linestyle="-")
    ax.plot(np.arange(length), dist, color="b", linestyle="-")

    xticks = np.linspace(0,n,6)
    yticks = np.linspace(0,max_y,20)
    ax.set_xticks(xticks)
    ax.set_yticks(yticks)
    ax.set_xticklabels(["{}".format(x) for x in xticks], fontsize=20,rotation=10)
    ax.set_yticklabels(["{:.4f}".format(float(x)) for x in yticks], fontsize=16,rotation=10)

    ax.annotate(r'$P(Y \leq 90) \approx 0.7658$',
         xy=(90, dist[90]), xycoords='data',
         xytext=(+10, +30), textcoords='offset points', fontsize=20,
         arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))
    plt.show()

def drawYDist() :
    dist[1] = p
    for i in range(2,n):
        dist[i] = dist[i-1]*(1-p)
    drawTimesDist(dist,1.1*p,"Distribution of random variant Y")
    # print(dist[90])

def drawYPreSumDist() :
    dist[1] = p
    for i in range(2,n):
        dist[i] = dist[i-1]*(1-p)
    for i in range(1,n):
        dist[i] += dist[i-1]
    print(dist[90])
    drawTimesDist(dist,1,"Distribution of Prefix-Summation of Y")