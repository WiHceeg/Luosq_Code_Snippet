# -*- coding: utf-8 -*-
"""
Created on Wed May 05 16:42:59 2020

@author: Luo_S
"""
import numpy as np
import matplotlib.pyplot as plt
# import sklearn

def kmeans(p_datas, k):
    max_iter = 10000
    tolerance = 0.00001
    result = -np.ones(p_datas.shape[0])
    centers_idx = np.random.randint(0, p_datas.shape[0], size=2)
    centers = p_datas[centers_idx]
    
    for i in range(max_iter):
        for j in range(p_datas.shape[0]):
            j_diffs = p_datas[j] - centers
            j_l2_squares = np.sum(j_diffs**2, axis=1)
            result[j] = np.argmin(j_l2_squares)
        last_centers = centers.copy()
        for k in range(k):
            arg_k = np.where(result == k)
            centers[k] = np.mean(p_datas[arg_k], axis = 0)
        if np.linalg.norm(centers - last_centers) < tolerance:
            break
    return result

def spectral_clustering(data, k):
    n = data.shape[0]
    W = np.zeros((n,n))
    for i in range(n):
        for j in range(i+1):
            W[i][j] = np.exp(-np.sum((data[i]-data[j])**2)/2)
            W[j][i] = W[i][j]
    d = np.zeros(n)
    for i in range(n):
        d[i] = np.sum(W[i])
    D =np.diag(d)
    L = D - W
    Dminus_0dot5 = np.diag(pow(d, -0.5))
    Lsym = Dminus_0dot5 @ L @ Dminus_0dot5
    # Lrw = np.linalg.inv(D)@L
    eigenValues, eigenVectors = np.linalg.eig(Lsym)
    idx = eigenValues.argsort()
    eigenValues = eigenValues[idx]
    eigenVectors = eigenVectors[:,idx]
    V = eigenVectors[:,:2].real
    normalizedV = V / np.std(V, axis=0)
    
    # res = sklearn.cluster.KMeans(n_clusters=2).fit_predict(normalizedV)
    res = kmeans(normalizedV, k)
    
    plt.figure()
    plt.scatter(normalizedV[:, 0], normalizedV[:, 1], c=res)
    plt.show()
    
    return res
    

if __name__ == "__main__":
    circles = np.loadtxt("./circles.txt")
    moons = np.loadtxt("./moons.txt")

    # circles_pred = spectral_clustering(circles, 2)
    # plt.figure()
    # plt.scatter(circles[:, 0], circles[:, 1], c=circles_pred)
    # plt.show()
    
    
    moons_pred = spectral_clustering(moons, 2)
    plt.figure()
    plt.scatter(moons[:, 0], moons[:, 1], c=moons_pred)
    plt.show()