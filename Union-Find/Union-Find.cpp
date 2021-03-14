#include <iostream>
#include <vector>

using namespace std;

class UF {
public:
    explicit UF(int n) :
            count(n), parent(n, 0), size(n, 0) {
        for (int i = 0; i < n; i++) {
            parent[i] = i;
            size[i] = 1;
        }
    }

    int getcount() {
        return count;
    }

    void union2(int p, int q) {
        int rootp = find(p);
        int rootq = find(q);
        if (rootp == rootq) {
            return;
        }
        //小树接到大树下面，较平衡
        if (size[rootp] < size[rootq]) {
            parent[rootp] = rootq;
            size[rootq] += size[rootp];
        } else {
            parent[rootq] = rootp;
            size[rootp] += size[rootq];
        }
        count--;
    }

    bool isconnect(int p, int q) {
        int rootp = find(p);
        int rootq = find(q);
        return rootp == rootq;
    }

private:
    int count;
    vector<int> parent; //记录每个节点的父节点，相当于指向父节点的指针，所以 parent 数组内实际存储着一个森林（若干棵多叉树）
    vector<int> size;   //用 size 数组记录着每棵树的重量，目的是让 union 后树依然拥有平衡性，而不会退化成链表，影响操作效率

    //返回节点x的根节点
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];  //让x和它的父节点平级
            x = parent[x];
        }
        return x;
    }
};

int main() {
    UF ans(10);
    ans.union2(1, 6);
    ans.union2(2, 4);
    ans.union2(9, 5);
    ans.union2(4, 9);
    cout << ans.isconnect(2, 5) << endl;
    cout << ans.getcount() << endl;
    return 0;
}