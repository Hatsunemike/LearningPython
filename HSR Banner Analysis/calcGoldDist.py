import numpy as np
import matplotlib.pyplot as plt

FloatType = np.float64 # 采用的浮点精度
plt.rcParams["font.family"]="SimHei"

def genBaseProbArray(
        n: int,
        base_prob: FloatType,
        start_pos: int,
        add: FloatType) -> np.array :
    prob = np.full(n+1, base_prob,dtype=FloatType)
    prob[0] = 0
    for i in range(n+1) :
        if(i >= start_pos) :
            prob[i] = prob[i-1]+add
            if(prob[i]>1): prob[i] = 1
    return prob

def showTwoProbArray(cha: np.array, light: np.array) :
    length = cha.shape[0]
    fig = plt.figure(num=2, figsize=(2,1))
    ax = fig.add_subplot(211)
    ax.set_title("角色池出金基础概率变化")
    ax.set_xlim([0,length])
    ax.set_ylim([0,1])
    ax.plot(np.arange(cha.shape[0]), cha, color="b", linestyle="-")

    ax = fig.add_subplot(212)
    ax.set_title("光锥池出金基础概率变化")
    ax.set_xlim([0,length])
    ax.set_ylim([0,1])
    ax.plot(np.arange(light.shape[0]), light, color="y", linestyle="-")
    plt.show()

def checkDist(dist: np.array) :
    sum = np.sum(dist)
    eps = 10 ** -8
    if(-eps <= 1-sum and 1-sum <= eps) : return True
    return False

def calcTimesDist(prob: np.array):
    result = np.zeros_like(prob, dtype=FloatType)
    mul = 1
    for i in range(1, prob.shape[0]) :
        result[i] = prob[i]*mul
        mul *= (1-prob[i])
    return result

def drawTimesDist(dist: np.array) :
    length = dist.shape[0]
    fig = plt.figure(num=1, figsize=(1,1))
    ax = fig.add_subplot(111)
    ax.set_title("出金所需的抽卡次数分布")
    ax.set_xlim([0,length])
    ax.set_ylim([0,1.5])
    ax.plot(np.arange(length), dist, color="b", linestyle="-")
    plt.show()

def calcTwoSumDist(dist1: np.array, dist2: np.array) :
    length = dist1.shape[0]+dist2.shape[0]-1
    result = np.zeros((length, ), dtype=FloatType)
    for i in range(dist1.shape[0]) :
        for j in range(dist2.shape[0]) :
            result[i+j] += dist1[i]*dist2[j]
    return result

def calcSumDist(dist: np.array, num: int) :
    if(num == 1): return dist
    result = dist
    for i in range(2, num+1) :
        result = calcTwoSumDist(result, dist)
    return result

def drawTwoTotalTimesDist(cha_dist: np.array, lig_dist: np.array, total_gold_num: int, preSum: bool = False) :
    fig = plt.figure(num=2, figsize=(2,1))
    max_x = max(cha_dist.shape[0], lig_dist.shape[0])
    if not preSum: max_y = 0.0025
    else : max_y = 1

    ax = fig.add_subplot(211)
    if not preSum: ax.set_title("角色池{}金抽数分布".format(total_gold_num))
    else : ax.set_title("角色池xx抽出{}金概率分布".format(total_gold_num))
    ax.set_xlim([0,max_x])
    ax.set_ylim([0,max_y])
    ax.plot(np.arange(cha_dist.shape[0]), cha_dist, color="b", linestyle="-")

    ax = fig.add_subplot(212)
    if not preSum: ax.set_title("光锥池{}金抽数分布".format(total_gold_num))
    else : ax.set_title("光锥池xx抽出{}金概率分布".format(total_gold_num))
    ax.set_xlim([0,max_x])
    ax.set_ylim([0,max_y])
    ax.plot(np.arange(lig_dist.shape[0]), lig_dist, color="y", linestyle="-")

    plt.show()

def storeArray(arr: np.array, filename: str) :
    with open(filename, encoding="utf-8", mode="w") as f:
        for i in range(arr.shape[0]):
            f.write("{} ".format(arr[i]))

def readArray(filename: str) -> np.array:
    with open(filename, encoding="utf-8", mode="r") as f:
        a = f.readline()
        result = []
        for x in a.split(" "):
            try:
                result.append(float(x))
            except ValueError:
                continue
        return np.array(result,dtype=np.float64)

def calcPreSum(arr: np.array) :
    result = np.zeros_like(arr,dtype=FloatType)
    result[0] = arr[0]
    for i in range(1, arr.shape[0]) :
        result[i] = result[i-1] + arr[i]
    return result

def calcExpect(dist: np.array) -> float:
    result = 0.0
    for i in range(dist.shape[0]): result += i * dist[i]
    return result

def getQuantile(preSum: np.array, p: float) -> int:
    for i in range(preSum.shape[0]) :
        if(preSum[i] >= p) :
            return i

def drawAllDist() :
    charac_base_prob = genBaseProbArray(90, 0.006, 74, 0.06)
    lightc_base_prob = genBaseProbArray(80, 0.008, 66, 0.07)

    charac_times_dist = calcTimesDist(charac_base_prob)
    lightc_times_dist = calcTimesDist(lightc_base_prob)

    charac_times_presum = calcPreSum(charac_times_dist)
    lightc_times_presum = calcPreSum(lightc_times_dist)

    fig = plt.figure(num=6, figsize=(2,3))
    ax =fig.add_subplot(231)
    ax.set_title("角色池基础概率变化")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,91,1),charac_base_prob,color="b")

    ax = fig.add_subplot(232)
    ax.set_title("角色池出金抽数分布")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,91,1),charac_times_dist,color="b")

    ax = fig.add_subplot(233)
    ax.set_title("角色池抽N抽能出货的概率分布")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,91,1),charac_times_presum,color="b")

    ax =fig.add_subplot(234)
    ax.set_title("光锥池基础概率变化")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,81,1),lightc_base_prob,color="y")

    ax =fig.add_subplot(235)
    ax.set_title("光锥池出金抽数分布")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,81,1),lightc_times_dist,color="y")

    ax =fig.add_subplot(236)
    ax.set_title("光锥池抽N抽能出货的概率分布")
    ax.set_xlim([0,90])
    ax.set_ylim([0,1])
    ax.plot(np.arange(0,81,1),lightc_times_presum,color="y")
    plt.show()

def showTotalGoldDistPreSum() :
    charac_base_prob = genBaseProbArray(90, 0.006, 74, 0.06)
    lightc_base_prob = genBaseProbArray(80, 0.008, 66, 0.07)

    charac_times_dist = calcTimesDist(charac_base_prob)
    lightc_times_dist = calcTimesDist(lightc_base_prob)

    total_gold_num = 69
    charac_times_total_dist = calcSumDist(charac_times_dist, total_gold_num)
    lightc_times_total_dist = calcSumDist(lightc_times_dist, total_gold_num)
    # storeArray(charac_times_total_dist, "./tmpdata/角色池69金抽数概率分布.txt")
    # storeArray(lightc_times_total_dist, "./tmpdata/光锥池69金抽数概率分布.txt")
    # exit(0)

    # charac_times_total_dist = readArray("./tmpdata/角色池{}金抽数概率分布.txt".format(total_gold_num))
    # lightc_times_total_dist = readArray("./tmpdata/光锥池{}金抽数概率分布.txt".format(total_gold_num))
    charac_expect = calcExpect(charac_times_total_dist)
    lightc_expect = calcExpect(lightc_times_total_dist)
    print("两个期望：", charac_expect, lightc_expect)

    charac_presum = calcPreSum(charac_times_total_dist)
    lightc_presum = calcPreSum(lightc_times_total_dist)
    # storeArray(charac_presum, "./tmpdata/角色池69金前缀和.txt")
    # storeArray(lightc_presum, "./tmpdata/光锥池69金前缀和.txt")
    # exit(0)
    # charac_presum = readArray("./tmpdata/角色池{}金前缀和.txt".format(total_gold_num))
    # lightc_presum = readArray("./tmpdata/光锥池{}金前缀和.txt".format(total_gold_num))

    length = max(charac_presum.shape[0], lightc_presum.shape[0])

    scatter_size = 50
    fig = plt.figure(num=4, figsize=(2,1))
    ax =fig.add_subplot(211)
    ax.set_title("角色池（常驻池）前N抽出{}金的概率分布".format(total_gold_num),fontsize=40)
    ax.set_xlim([0,length])
    ax.set_ylim([0,1])
    pos_50 = getQuantile(charac_presum,0.5)
    pos_90 = getQuantile(charac_presum,0.9)
    pos_99 = getQuantile(charac_presum,0.99)
    ax.scatter(pos_50,charac_presum[pos_50],c="r",s=scatter_size)
    ax.scatter(pos_90,charac_presum[pos_90],c="r",s=scatter_size)
    ax.scatter(pos_99,charac_presum[pos_99],c="r",s=scatter_size)
    ax.annotate('({},{})'.format(pos_50,0.5),
         xy=(pos_50, charac_presum[pos_50]), xycoords='data',
         xytext=(+20, -30), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.annotate('({},{})'.format(pos_90,0.9),
         xy=(pos_90, charac_presum[pos_90]), xycoords='data',
         xytext=(-40, -40), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.annotate('({},{})'.format(pos_99,0.99),
         xy=(pos_99, charac_presum[pos_99]), xycoords='data',
         xytext=(+20, -30), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.plot(np.arange(0,charac_presum.shape[0]), charac_presum, color='b')

    ax =fig.add_subplot(212)
    ax.set_title("光锥池前N抽出{}金的概率分布".format(total_gold_num),fontsize=40)
    ax.set_xlim([0,length])
    ax.set_ylim([0,1])
    pos_50 = getQuantile(lightc_presum,0.5)
    pos_90 = getQuantile(lightc_presum,0.9)
    pos_99 = getQuantile(lightc_presum,0.99)
    ax.scatter(pos_50,lightc_presum[pos_50],c="g",s=scatter_size)
    ax.scatter(pos_90,lightc_presum[pos_90],c="g",s=scatter_size)
    ax.scatter(pos_99,lightc_presum[pos_99],c="g",s=scatter_size)
    ax.annotate('({},{})'.format(pos_50,0.5),
         xy=(pos_50, lightc_presum[pos_50]), xycoords='data',
         xytext=(-50, -50), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.annotate('({},{})'.format(pos_90,0.9),
         xy=(pos_90, lightc_presum[pos_90]), xycoords='data',
         xytext=(-50, -50), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.annotate('({},{})'.format(pos_99,0.99),
         xy=(pos_99, lightc_presum[pos_99]), xycoords='data',
         xytext=(+20, -30), textcoords='offset points', fontsize=30,
         arrowprops=dict(arrowstyle="-", connectionstyle="arc3,rad=0"))
    ax.plot(np.arange(0,lightc_presum.shape[0]), lightc_presum, color='y')
    plt.show()

if __name__ == "__main__" :
    showTotalGoldDistPreSum()