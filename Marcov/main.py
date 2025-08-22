import numpy as np

A = np.array([[0, 0.5, 0.5], [0.5, 0, 0.5], [0.5, 0.5, 0]], dtype=np.float64)

Lambda = np.diag([-0.5, -0.5, 1])

U = np.array([[-1,-1,1], [1,0,1], [0,1,1]], dtype=np.float64)

P_0 = np.array([1,0,0], dtype=np.float64)

U_1 = np.linalg.inv(U)

if __name__ == "__main__" :
    p = P_0
    for i in range(1,11) :
        p = np.matmul(A, p)
        print("i: {}, p: {}, p_formula: {}".format(i, p[0], (1/3)+(2/3)*((-0.5)**i)))