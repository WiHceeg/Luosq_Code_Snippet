import numpy as np
import pandas as pd

def P(colID, attribute, C):  # 核心函数P(colName=attribute|C)，如P(色泽=青绿|是),P(密度=0.697|是)
    if (colID, attribute, C) in Pmap:
        return Pmap[(colID, attribute, C)]
    curJudgeList = []
    if C == '是':
        curJudgeList = goodList
    else:
        curJudgeList = badList
    ans = 0
    if colID >= 6:  # >=6的部分是连续值

        if (colID, C) in continuousPara:
            curPara = continuousPara[(colID, C)]
            mean = curPara[0]
            std = curPara[1]
        else:
            curData = X[curJudgeList, colID]
            mean = curData.mean()
            std = curData.std(ddof=1)  # ddof=1是使用无偏样本标准差，才和西瓜书上的结果一样
            # print(mean,std)
            continuousPara[(colID, C)] = (mean, std)
        ans = 1/(np.sqrt(np.pi*2)*std) * \
            np.exp((-(attribute-mean)**2)/(2*std**2))
    else:
        for i in curJudgeList:
            if X[i, colID] == attribute:
                ans += 1
        ans = (ans+1)/(len(curJudgeList)+kindsOfAttribute[colID])
    Pmap[(colID, attribute, C)] = ans
    print('P(', columnName[colID], '=', attribute, '|', C, ')=', ans, sep='')
    return ans


def predictOne(single):
    ansYes = np.log2((len(goodList)+1)/(len(y)+2))  # 计算类先验概率，取对数
    ansNo = np.log2((len(badList)+1)/(len(y)+2))
    for i in range(len(single)):
        ansYes += np.log2(P(i, single[i], '是'))  # 通过取对数将连乘转化为连加来避免数值下溢
        ansNo += np.log2(P(i, single[i], '否'))
    print('P(好瓜=是|样本)=', 2**ansYes, sep='')
    print('P(好瓜=否|样本)=', 2**ansNo, sep='')
    if ansYes > ansNo:
        print('判断该样例"好瓜=是"', end='\n\n')
        return '是'
    else:
        print('判断该样例"好瓜=否"', end='\n\n')
        return '否'


def predictAll(iX):
    predictY = []
    for i in range(m):
        print('样例', i, ' 编号', i+1, sep='')
        predictY.append(predictOne(iX[i]))
    return predictY


if __name__ == '__main__':
    
    dataset = pd.read_csv('./watermelon_3.txt', delimiter=',')
    del dataset['编号']  # 删掉编号方便后续处理
    print(dataset)
    X = dataset.values[:, :-1]
    m, n = X.shape
    y = dataset.values[:, -1]
    columnName = dataset.columns
    colIndex = dict(zip(columnName, range(len(columnName))))


    Pmap = {}  # Pmap用来保存离散属性条件概率以避免重复计算

    # 属性种类数，比如有三种色泽，所以kindsOfAttribute[0]=3。用来存第i个属性可能的取值数，后面用来当拉普拉斯修正要加在分母上的值
    kindsOfAttribute = {}
    for i in range(n):
        kindsOfAttribute[i] = len(set(X[:, i]))

    continuousPara = {}  # 西瓜数据集3.0中连续属性用到，可以用来保存连续值属性的均值mean和标准差std，避免重复计算

    goodList = []
    badList = []
    for i in range(len(y)):
        if y[i] == '是':
            goodList.append(i)
        else:
            badList.append(i)

    predictY = predictAll(X)
    print('实际值:', y)
    print('预测值:', np.array(predictY))
