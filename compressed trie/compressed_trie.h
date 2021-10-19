#ifndef _TEST_H
#define _TEST_H

#include<iostream>
#include<string>
#include<vector>

using namespace std;
const int SIZE = 26;

class CompressedTrie
{
private:
    /* data */
    struct TrieNode
    {
        /* data */
        TrieNode* children[SIZE];
        bool endOfWord;
        // the string currently stores in the node
        string key;

        // the struct constructor
        TrieNode() {
            // initialize the children node array
            for (int i = 0; i < SIZE; ++i) {
                children[i] = nullptr;
            }
            endOfWord = false;
            // initialize the key
            key = "";
        }
        // get the position of the pointer should point to
        // indicate which child it should pick
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
    void insertHelper(TrieNode*& node, const string word);
    bool searchHelper(TrieNode*& node, string& word, bool isPrefix);
    bool removeHelper(TrieNode*& node, const string& word);
    int matchHelper(vector<string>& remains, string nodeWord, string newWord);
    void traverseHelper(TrieNode*& node);
    void reConnectHelper(TrieNode*& newNode, TrieNode*& node, string& nodeWord, int curLength);
    // the root of the DST
    TrieNode* root;

public:
    CompressedTrie();
    ~CompressedTrie();
    void insert(const string& word);
    bool remove(const string& word);
    bool search(const string& word, bool isPrefix);
    void traverse();
    void clear(TrieNode* node);

};

#endif _TEST_H