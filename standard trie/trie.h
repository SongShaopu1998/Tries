#ifndef _TEST_H
#define _TEST_H

#include<iostream>
#include<string>
#include<vector>

using namespace std;
const int SIZE = 26;

class Trie
{
private:
    /* data */
    struct TrieNode
    {
        /* data */
        TrieNode* children[SIZE];
        bool endOfWord;

        // the struct constructor
        TrieNode() {
            for (int i = 0; i < SIZE; ++i) {
                children[i] = nullptr;
            }
            endOfWord = false;
        }
        // get the position of the pointer should go to
        int get(char ch) {
            int index = ch - 'a';
            return index;
        }
        // is this node a leaf already? (all children point to nullptr)
        bool isLeaf() {
            for (auto& child : children) {
                if (child != nullptr) {
                    return false;
                }
            }
            return true;
        }

    };
    // helper function
    void insertHelper(TrieNode*& node, const string& word, int index);
    bool searchHelper(TrieNode*& node, const string& word, int index, bool isPrefix);
    bool removeHelper(TrieNode*& node, const string& word, int index);
    int longestPrefixHelper(TrieNode*& node, const string& word, int index, int length);
    void keysWithPrefixHelper(TrieNode*& node, vector<string>& chosen, string word, int index, int length);
    // the root of the DST
    TrieNode* root;
    int cur_size;

public:
    Trie();
    ~Trie();
    void insert(const string& word);
    bool remove(const string& word);
    bool search(const string& word, bool isPrefix);
    string longestPrefix(const string& word);
    vector<string> keysWithPrefix(const string& word);
    void clear(TrieNode* node);

};

#endif _TEST_H
