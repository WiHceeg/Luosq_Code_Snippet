#include <iostream>
#include <vector>

using namespace std;

class MySort {
public:
    void show(vector<int> &a) {
        for (int &e: a) {
            cout << e << ' ';
        }
        cout << endl;
    }

    void selection_sort(vector<int> &a) {   //选择排序，每次选最小的和第一个交换，从剩下的选最小的和第二个交换...
        int N = a.size();
        for (int i = 0; i < N; i++) {
            int minIndex = i;
            for (int j = i + 1; j < N; j++) {
                if (a[j] < a[minIndex]) {
                    minIndex = j;
                }
            }
            exch(a, i, minIndex);
        }
    }

    void insertion_sort(vector<int> &a) {   //插入排序，前面是排好序的，把当前值插到前面
        int N = a.size();
        for (int i = 1; i < N; i++) {
            for (int j = i; j > 0 && a[j] < a[j - 1]; j--) {
                exch(a, j - 1, j);
            }
//            int temp = a[i];      //这样写代替里面的for循环效率更高
//            int j = i - 1;
//            while(j >= 0 && temp < a[j]){
//                a[j + 1] = a[j];
//                j--;
//            }
//            a[j + 1] = temp;
        }
    }

    void bubble_sort(vector<int> &a) {  //冒泡排序，每次把最大的冒泡到后面
        int N = a.size();
        int flag = 0;
        for (int i = 0; i < N; i++) {
            flag = 0;
            for (int j = 0; j < N - i - 1; j++) {
                if (a[j] > a[j+1]){
                    exch(a, j, j+1);
                    flag = 1;
                }
            }
            if(flag == 0){
                break;
            }
        }
    }

    void shell_sort(vector<int> &a) {   //希尔排序，在插入排序加入外循环控制增量缩小
        int N = a.size();
        int h = 1;
        while (h < N / 3) {
            h = h * 3 + 1;
        }
        while (h >= 1) {
            for (int i = h; i < N; i++) {
                for (int j = i; j >= h && a[j] < a[j - h]; j -= h) {
                    exch(a, j, j - h);
                }
            }
            h /= 3;
        }
    }

    void merge_sort_TD(vector<int> &a) {     //归并排序，自顶向下，递归
        b = a;
        merge_sort_TD(a, 0, a.size() - 1);
    }

    void merge_sort_BU(vector<int> &a) {     //归并排序，自底向上，迭代
        int N = a.size();
        b = a;
        for (int seg = 1; seg < N; seg *= 2) {
            for (int lo = 0; lo < N - seg; lo += seg * 2) {
                merge(a, lo, lo + seg - 1, min(lo + seg * 2 - 1, N - 1));
            }
        }
    }

    void quick_sort(vector<int> &a) {       //快速排序
        quick_sort(a, 0, a.size() - 1);
    }

    void heap_sort(vector<int> &a) {
        int N = a.size();
        for (int i = (N - 2) / 2; i >= 0; i--) {    //调整成大根堆
            sink(a, i, N - 1);
        }
        for (int i = N - 1; i > 0; i--) {
            exch(a, 0, i);  //把目前最大的换到后面
            sink(a, 0, i - 1);   //把刚刚换上来的下沉，下沉的过程
        }
    }

private:
    void exch(vector<int> &a, int i, int j) {
        int t = a[i];
        a[i] = a[j];
        a[j] = t;
    }

    vector<int> b;  //用来做归并排序的辅助数组
    void merge(vector<int> &a, int lo, int mid, int hi) {    //将a[low...mid]和a[mid+1...hi]归并到a[low...hi]中
        int i = lo, j = mid + 1;
        for (int k = lo; k <= hi; k++) {
            b[k] = a[k];
        }
        int k = lo;
        while (i <= mid && j <= hi) {
            a[k++] = b[i] <= b[j] ? b[i++] : b[j++];
        }
        while (i <= mid) {
            a[k++] = b[i++];
        }
        while (j <= hi) {
            a[k++] = b[j++];
        }
    }

    void merge_sort_TD(vector<int> &a, int lo, int hi) {
        if (lo >= hi) { return; }
        int mid = lo + (hi - lo) / 2;
        merge_sort_TD(a, lo, mid);
        merge_sort_TD(a, mid + 1, hi);
        merge(a, lo, mid, hi);
    }

    void quick_sort(vector<int> &a, int lo, int hi) {
        if (lo >= hi) { return; }

        int i = partition(a, lo, hi);
        quick_sort(a, lo, i - 1);
        quick_sort(a, i + 1, hi);

    }

    int partition(vector<int> &a, int lo, int hi) {
        int temp = a[lo];
        while (lo < hi) {
            while (lo < hi && a[hi] >= temp) {
                hi--;
            }
            a[lo] = a[hi];
            while (lo < hi && a[lo] <= temp) {
                lo++;
            }
            a[hi] = a[lo];
        }
        a[lo] = temp;
        return lo;
    }

    void sink(vector<int> &a, int k, int end) {  //将下标为k的元素下沉
        int temp = a[k];
        int son = k * 2 + 1;
        while (son <= end) {
            if (son < end && a[son] < a[son + 1]) {
                son++;
            }
            if (temp < a[son]) {
                a[k] = a[son];
                k = son;
                son = k * 2 + 1;
            } else {
                break;
            }
        }
        a[k] = temp;
    }
};

int main() {
    MySort A;
    vector<int> a = {4, 28, 11, 9, 15, 24, 35, 23, 17, 29, 26, 31, 33, 22, 19, 2, 9, 0, 36, 23, 25, 18, 32, 8, 5, 21, 7,
                     12, 14, 20, 27, 1, 6, 10, 1, 30, 9, 13, 34, 3, 16};
    A.show(a);

//    A.selection_sort(a);
//    A.insertion_sort(a);
//    A.bubble_sort(a);
    A.shell_sort(a);
//    A.merge_sort_TD(a);
//    A.merge_sort_BU(a);
//    A.quick_sort(a);
//    A.heap_sort(a);
    A.show(a);

    return 0;
}
