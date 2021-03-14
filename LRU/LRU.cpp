#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <iterator>

using namespace std;

//自己定义双向链表的方法
//class Node {
//public:
//    int key, val;
//    Node *pre, *next;
//
//    Node() :
//            key(0), val(0), pre(nullptr), next(nullptr) {}
//
//    Node(int k, int v) :
//            key(k), val(v), pre(nullptr), next(nullptr) {}
//};
//
//class DoubleList {
//public:
//    DoubleList() {
//        dummyhead = new Node();
//        dummytail = new Node();
//        dummyhead->next = dummytail;
//        dummytail->pre = dummyhead;
//    }
//
//    void move_to_tail(Node *node) {
//        node->pre->next = node->next;
//        node->next->pre = node->pre;
//        add_to_tail(node);
//    }
//
//    void add_to_tail(Node *node) {
//        node->pre = dummytail->pre;
//        dummytail->pre->next = node;
//        node->next = dummytail;
//        dummytail->pre = node;
//    }
//
//    int remove_head() {
//        Node *temp = dummyhead->next;
//        int headKey = temp->key;
//        dummyhead->next = temp->next;
//        temp->next->pre = dummyhead;
//        delete temp;
//        temp = nullptr;
//        return headKey;
//    }
//
//private:
//    Node *dummyhead, *dummytail;
//};
//
//class LRUCache {
//public:
//    LRUCache(int capacity)
//            : cap(capacity) {}
//
//    int get(int key) {
//        if (hash.count(key) == 0) {
//            return -1;
//        }
//        cacheDL.move_to_tail(hash[key]);
//        return hash[key]->val;
//    }
//
//    void put(int key, int value) {
//        if (hash.count(key)) {
//            hash[key]->val = value;
//            cacheDL.move_to_tail(hash[key]);
//        } else {
//            Node *node = new Node(key, value);
//            hash[key] = node;
//            cacheDL.add_to_tail(node);
//            size++;
//            if (size > cap) {
//                int headKey = cacheDL.remove_head();
//                hash.erase(headKey);
//                size--;
//            }
//        }
//    }
//
//private:
//    int cap;
//    int size = 0;
//    DoubleList cacheDL;
//    unordered_map<int, Node *> hash;
//};

//用list的方法，很简短，学到stl里的函数splice, advance, next, prev
class LRUCache {
public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if(hash.count(key) == 0){
            return -1;
        }
        cacheDL.splice(cacheDL.end(), cacheDL, hash[key]);
        return hash[key]->second;
    }

    void put(int key, int value) {
        if(hash.count(key) == 0){
            if(cacheDL.size() == cap){
                hash.erase(cacheDL.front().first);
                cacheDL.pop_front();
            }
            cacheDL.push_back({key, value});
            hash[key] = prev(cacheDL.end());
        }else{
            cacheDL.splice(cacheDL.end(), cacheDL, hash[key]);
            hash[key]->second = value;
        }
    }

private:
    int cap;
    list<pair<int, int>> cacheDL;
    unordered_map<int, list<pair<int, int>>::iterator> hash;

};


/*测试用例
LRUCache lRUCache = new LRUCache(2);
lRUCache.put(1, 1); // 缓存是 {1=1}
lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
lRUCache.get(1);    // 返回 1
lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
lRUCache.get(2);    // 返回 -1 (未找到)
lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
lRUCache.get(1);    // 返回 -1 (未找到)
lRUCache.get(3);    // 返回 3
lRUCache.get(4);    // 返回 4
*/
int main() {

    LRUCache lRUCache(2);
    lRUCache.put(1, 1); // 缓存是 {1=1}
    lRUCache.put(2, 2); // 缓存是 {1=1, 2=2}
    cout << lRUCache.get(1) << endl;    // 返回 1
    lRUCache.put(3, 3); // 该操作会使得关键字 2 作废，缓存是 {1=1, 3=3}
    cout << lRUCache.get(2) << endl;    // 返回 -1 (未找到)
    lRUCache.put(4, 4); // 该操作会使得关键字 1 作废，缓存是 {4=4, 3=3}
    cout << lRUCache.get(1) << endl;    // 返回 -1 (未找到)
    cout << lRUCache.get(3) << endl;    // 返回 3
    cout << lRUCache.get(4) << endl;    // 返回 4

    return 0;
}