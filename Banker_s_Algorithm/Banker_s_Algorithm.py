import numpy as np

allocation = np.array([[0, 0, 1, 2],
                       [1, 0, 0, 0],
                       [1, 3, 5, 4],
                       [0, 6, 3, 2],
                       [0, 0, 1, 4]])
claim = np.array([[0, 0, 1, 2],
                  [1, 7, 5, 0],
                  [2, 3, 5, 6],
                  [0, 6, 5, 2],
                  [0, 6, 5, 6]])
available = np.array([1, 5, 2, 0])

if __name__ == "__main__":

    while np.any(allocation != 0):
        change = False
        for i in range(5):
            if np.any(allocation[i] != 0) and np.all(available >= claim[i] - allocation[i]):
                available += allocation[i]
                allocation[i] = np.array([0, 0, 0, 0])
                change = True
        if not change:
            print('不', end='')
            break
    print("安全")
