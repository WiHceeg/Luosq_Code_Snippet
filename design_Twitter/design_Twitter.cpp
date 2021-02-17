#include <iostream>
#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>

using namespace std;

class Tweet {
public:
    Tweet(int tweetId, int time) :
            tweetId(tweetId), time(time) {
        next = nullptr;
    }

    int tweetId;
    int time;
    Tweet *next;
};

class User {
public:
    int userId;
    Tweet *cur = nullptr;
    unordered_set<int> followees;
};

class Twitter {
public:
    /** Initialize your data structure here. */
    Twitter() {
        time_now_ = 0;
        max_recent_ = 10;
    }

    /** Compose a new tweet. */
    void postTweet(int userId, int tweetId) {
        time_now_++;
        if (users_.count(userId) == 0) {
            users_[userId].userId = userId;
            users_[userId].followees.insert(userId);
        }
        Tweet *newTweet = new Tweet(tweetId, time_now_);
        newTweet->next = users_[userId].cur;
        users_[userId].cur = newTweet;
    }

    /** Retrieve the 10 most recent tweet ids in the user's news feed. Each item in the news feed must be posted by users who the user followed or by the user herself. Tweets must be ordered from most recent to least recent. */
    vector<int> getNewsFeed(int userId) {
        if (users_.count(userId) == 0) {
            return {};
        }
        auto cmp = [](Tweet *x, Tweet *y) { return x->time < y->time; };
        priority_queue<Tweet *, vector<Tweet *>, decltype(cmp)> pq(cmp);
        for (int followee: users_.at(userId).followees) {
            if (users_.at(followee).cur != nullptr) {
                pq.push(users_.at(followee).cur);
            }
        }
        vector<int> recent_tweets;
        int left = max_recent_;
        while (!pq.empty() && left > 0) {
            Tweet *cur = pq.top();
            recent_tweets.push_back(cur->tweetId);
            pq.pop();
            if (cur->next != nullptr) {
                pq.push(cur->next);
            }
            left--;
        }
        return recent_tweets;
    }

    /** Follower follows a followee. If the operation is invalid, it should be a no-op. */
    void follow(int followerId, int followeeId) {
        if (users_.count(followerId) == 0) {        //如果没出现过此用户就创建
            users_[followerId].userId = followerId;
            users_[followerId].followees.insert(followerId);
        }
        if (users_.count(followeeId) == 0) {        //如果没出现过要关注的人就创建
            users_[followeeId].userId = followeeId;
            users_[followeeId].followees.insert(followeeId);
        }
        users_[followerId].followees.insert(followeeId);
    }

    /** Follower unfollows a followee. If the operation is invalid, it should be a no-op. */
    void unfollow(int followerId, int followeeId) {
        if (followerId == followeeId) { return; }     //一个人不能取关他自己
        if (users_.count(followerId) == 0) {        //如果没出现过此用户就创建
            users_[followerId].userId = followerId;
            users_[followerId].followees.insert(followerId);
        }
        if (users_.count(followeeId) == 0) {        //如果没出现过要关注的人就创建
            users_[followeeId].userId = followeeId;
            users_[followeeId].followees.insert(followeeId);
        }
        users_[followerId].followees.erase(followeeId);
    }

private:
    int time_now_;
    int max_recent_;
    unordered_map<int, User> users_;
};

/**
 * Your Twitter object will be instantiated and called as such:
 * Twitter* obj = new Twitter();
 * obj->postTweet(userId,tweetId);
 * vector<int> param_2 = obj->getNewsFeed(userId);
 * obj->follow(followerId,followeeId);
 * obj->unfollow(followerId,followeeId);
 */

void print_vector(const vector<int> &v) {
    for (int x: v) {
        cout << x << ' ';
    }
    cout << endl;
}

int main() {

    Twitter twitter;

// 用户1发送了一条新推文 (用户id = 1, 推文id = 5).
    twitter.postTweet(1, 5);

// 用户1的获取推文应当返回一个列表，其中包含一个id为5的推文.
    print_vector(twitter.getNewsFeed(1));

// 用户1关注了用户2.
    twitter.follow(1, 2);

// 用户2发送了一个新推文 (推文id = 6).
    twitter.postTweet(2, 6);

// 用户1的获取推文应当返回一个列表，其中包含两个推文，id分别为 -> [6, 5].
// 推文id6应当在推文id5之前，因为它是在5之后发送的.
    print_vector(twitter.getNewsFeed(1));

// 用户1取消关注了用户2.
    twitter.unfollow(1, 2);

// 用户1的获取推文应当返回一个列表，其中包含一个id为5的推文.
// 因为用户1已经不再关注用户2.
    print_vector(twitter.getNewsFeed(1));
    return 0;
}