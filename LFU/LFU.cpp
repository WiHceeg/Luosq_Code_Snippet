//
// Created by luosq on 2020/12/23.
//
#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <iterator>

using namespace std;

struct Node {
    Node(int key, int val, int freq) : key(key), val(val), freq(freq) {}

    int key;
    int val;
    int freq;
};

class LFUCache {
public:
    LFUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        if (capacity_ == 0) { return -1; }
        if (key_table_.find(key) == key_table_.end()) {
            return -1;
        } else {
            Upgrade(key);
        }
        return key_table_[key]->val;
    }

    void put(int key, int value) {
        if (capacity_ == 0) { return; }
        if (key_table_.find(key) == key_table_.end()) {
            Insert(key, value, 1);  //之前没有，就插入，令freq = 1
            min_freq_ = 1;
        } else {
            Upgrade(key);
            key_table_.find(key)->second->val = value;
        }
    }

private:
    int capacity_;
    int min_freq_ = 0;
    unordered_map<int, list<Node>::iterator> key_table_;   //键 - Node迭代器
    unordered_map<int, list<Node>> freq_table_;             //频率 - 双向链表
    void Upgrade(int key) {
        int val = key_table_.at(key)->val;
        int freq = key_table_.at(key)->freq;
        freq_table_.at(freq).erase(key_table_.at(key));     //将原node从list上删除
        key_table_.erase(key);                                      //将原key从key_table_删除
        if (freq_table_.at(min_freq_).empty()) {
            min_freq_++;
        }
        freq++;
        Insert(key, val, freq);
    }

    void Insert(int key, int val, int freq) {       //之前肯定没有，插入，满了就先去掉最小的，不管min_freq
        if (key_table_.size() == capacity_) {
            list<Node> &lflist = freq_table_.at(min_freq_);     //注意这里要用引用
            key_table_.erase(lflist.front().key);       //将最少使用的键从key_table_里删掉
            lflist.pop_front();                         //将最少使用的node从双向链表里删掉
        }
        list<Node> &curlist = freq_table_[freq];        //注意这里要用引用
        curlist.push_back(Node(key, val, freq));
        key_table_[key] = prev(curlist.end());          //更新key_table_
    }
};

//lFUCache.put(1, 1);
//lFUCache.put(2, 2);
//lFUCache.get(1);      // 返回 1
//lFUCache.put(3, 3);   // 去除键 2
//lFUCache.get(2);      // 返回 -1（未找到）
//lFUCache.get(3);      // 返回 3
//lFUCache.put(4, 4);   // 去除键 1
//lFUCache.get(1);      // 返回 -1（未找到）
//lFUCache.get(3);      // 返回 3
//lFUCache.get(4);      // 返回 4

int main() {
    LFUCache lFUCache(2);
    lFUCache.put(1, 1);
    lFUCache.put(2, 2);
    cout << lFUCache.get(1) << endl;      // 返回 1
    lFUCache.put(3, 3);   // 去除键 2
    cout << lFUCache.get(2) << endl;      // 返回 -1（未找到）
    cout << lFUCache.get(3) << endl;      // 返回 3
    lFUCache.put(4, 4);   // 去除键 1
    cout << lFUCache.get(1) << endl;      // 返回 -1（未找到）
    cout << lFUCache.get(3) << endl;      // 返回 3
    cout << lFUCache.get(4) << endl;      // 返回 4
    return 0;
}