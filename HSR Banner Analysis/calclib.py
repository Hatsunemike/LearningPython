import numpy as np

FloatType = np.float64 # 采用的浮点精度

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

def checkDist(dist: np.array) :
    sum = np.sum(dist)
    eps = 10 ** -8
    if(-eps <= 1-sum and 1-sum <= eps) : return True
    return False

def calcDist(prob: np.array):
    result = np.zeros_like(prob, dtype=FloatType)
    mul = 1
    for i in range(1, prob.shape[0]) :
        result[i] = prob[i]*mul
        mul *= (1-prob[i])
    return result

def calcConv(dist1: np.array, dist2: np.array) :
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
        result = calcConv(result, dist)
    return result

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
    # Return 0 if the Quantile doesn't exist.
    for i in range(preSum.shape[0]) :
        if(preSum[i] >= p) :
            return i
    return 0

def getDistQuantile(dist: np.array, p: float) -> int:
    return getQuantile(calcPreSum(dist), p)

# high-practical functions

def calcDistBigGrt(dist: np.array, missProb: float = 0.5) -> np.ndarray:
    bigDist = calcConv(dist, dist)
    dist = np.pad(dist, (0, bigDist.shape[0]-dist.shape[0]), mode="constant", constant_values=0)
    result = bigDist*missProb+dist*(1-missProb)
    return result

def calcEachNumDist(dist: np.array, num: int) ->  list:
    """
    Return Value: [dist_n=0, dist_n=1, dist_n=2, ...]
    """
    if(num == 1) :
        return [dist]
    result = [dist]
    now = dist
    for i in range(1, num) :
        now = calcConv(now, dist)
        result.append(now)
    return result