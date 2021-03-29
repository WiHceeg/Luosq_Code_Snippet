//
// Created by luosq on 2021/3/18.
//
#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
    int totalNQueens(int n) {
        n_ = n;
        BackTrace(0);
        return res_;
    }

private:
    //row是行，columns是列
    unordered_set<int> columns_, pies_, nas_;
    int res_ = 0;
    int n_;

    void BackTrace(int i) {

        if (i == n_) {
            res_++;
            return;
        }
        for (int j = 0; j < n_; j++) {
            //j是选择放在第几列
            int pie = i + j - (n_ - 1);
            int na = j - i;
            if (columns_.count(j) || pies_.count(pie) || nas_.count(j - i)) {
                continue;
            } else {
                columns_.insert(j);
                pies_.insert(pie);
                nas_.insert(na);

                BackTrace(i + 1);

                columns_.erase(j);
                pies_.erase(pie);
                nas_.erase(na);
            }

        }
    }
};

int main() {

    Solution n_queens;
    for (int i = 1; i <= 13; i++) {
        cout << "i: " << n_queens.totalNQueens(i) << endl;
    }

    return 0;
}