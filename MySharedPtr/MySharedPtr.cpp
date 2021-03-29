#include <bits/stdc++.h>

using namespace std;

struct Base {
    Base(int i) : i_(i) {
        cout << "construct " << i_ << endl;
    }

    ~Base() {
        cout << "deconstruct" << endl;
    }

    int i_;
};

template<typename T>
class MySharedPtr {
public:
    MySharedPtr(T *ptr = nullptr) : ptr_(ptr) {
        if (ptr_) {
            count_ = new int(1);
        } else {
            count_ = new int(0);
        }
    }

    MySharedPtr(const MySharedPtr &msp) {
        if (this != &msp) {
            ptr_ = msp.ptr_;
            count_ = msp.count_;
            (*count_)++;
        }
    }

    MySharedPtr &operator=(const MySharedPtr &msp) {
        if (ptr_ == msp.ptr_) {
            return *this;
        }
        if (ptr_ != nullptr) {
            (*count_)--;
            if (*count_ == 0) {
                delete ptr_;
                ptr_ = nullptr;
                delete count_;
                count_ = nullptr;
            }
        }
        ptr_ = msp.ptr_;
        count_ = msp.count_;
        (*count_)++;
        return *this;
    }

    T &operator*() {
        return *ptr_;
    }

    T *operator->() {
        return ptr_;
    }

    int use_count() {
        return *count_;
    }

    ~MySharedPtr() {
        (*count_)--;
        if (*count_ == 0) {
            delete ptr_;
            ptr_ = nullptr;
            delete count_;
            count_ = nullptr;
        }
    }

private:
    T *ptr_;
    int *count_;
};

int main() {
//
//    shared_ptr<Base> p1(new Base(10));
//    cout << p1.use_count() << endl;
//
//    shared_ptr<Base> p2(p1);
//
//    cout << p1.use_count() << endl;
//    cout << p2.use_count() << endl;


    MySharedPtr<Base> p1(new Base(10));
    cout << p1.use_count() << endl;

    MySharedPtr<Base> p2(p1);

    cout << p1.use_count() << endl;
    cout << p2.use_count() << endl;


    return 0;
}
