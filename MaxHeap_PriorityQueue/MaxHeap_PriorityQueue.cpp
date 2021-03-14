#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class MaxPriorityQueue {
public:
    MaxPriorityQueue() = default;

    explicit MaxPriorityQueue(const vector<T> &vec) {
        for (const T &e: vec) {
            Insert(e);
        }
    }

    T Max() {
        return priority_queue_[1];
    }

    int Size(){
        return n_;
    }

    bool Empty() {
        return n_ == 0;
    }

    void Insert(const T &e) {
        n_++;
        if(Size() > priority_queue_.size() - 1){
            priority_queue_.resize(priority_queue_.size() * 2);
        }
        priority_queue_[n_] = e;
        Swim(n_);
    }


    void DelMax() {
        if (Empty()) {
            cout << "DelMax失败" << endl;
            return;
        }
        swap(priority_queue_[1], priority_queue_[n_]);
        n_--;
        Sink(1);
    }

private:
    int n_ = 0;
    vector<T> priority_queue_ = {0};


    void Swim(int k) {
        while (k > 1 && priority_queue_[k] > priority_queue_[k / 2]) {
            Exch(k, k / 2);
            k = k / 2;
        }
    }

    void Sink(int k) {
        while (k * 2 <= n_) {
            int son = k * 2;
            if (k * 2 + 1 <= n_) {
                if (priority_queue_[son] < priority_queue_[son + 1]) {
                    son++;
                }
            }
            if (priority_queue_[k] >= priority_queue_[son]) {
                break;
            }
            Exch(k, son);
            k = son;
        }
    }

    void Exch(const int i, const int j) {
        swap(priority_queue_[i], priority_queue_[j]);
    }

};


int main() {
    MaxPriorityQueue<int> ans({1, 8, 5, 6, 3, 4, 0, 9, 7, 2});
    while (!ans.Empty()) {
        cout << ans.Max() << endl;
        ans.DelMax();
    }

    return 0;
}