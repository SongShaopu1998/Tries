#include <iostream>
#include"compressed_trie.h"

// constructor
CompressedTrie::CompressedTrie() {
    // initialize the root
    // call the constructor of the TrieNode struct
    // use a different method comparing with the standard trie tree
    // since we need to save strings in it
    root = new TrieNode();
}
/**
 * use recursion to delete the entire tree
 * notice we must delete from the leaf
 */
void CompressedTrie::clear(TrieNode* node) {
    if (node) {
        for (int i = 0; i < SIZE; ++i) {
            clear(node->children[i]);
        }
        delete node;
        node = nullptr;
    }
}

void CompressedTrie::traverse() {
    traverseHelper(root);
}

void CompressedTrie::traverseHelper(TrieNode*& node) {
    if (node) {
        for (int i = 0; i < SIZE; ++i) {
            traverseHelper(node->children[i]);
        }
        cout << node->key << endl;
    }
}

// deconstructor
CompressedTrie::~CompressedTrie() {
    clear(root);
}

void CompressedTrie::insert(const string& word) {
    int nextChild = root->get(word[0]);
    insertHelper(root->children[nextChild], word);
}

// we should not add words to thr root directly
void CompressedTrie::insertHelper(TrieNode*& node, const string word) {
    // the node does not exist, we insert the word (or the remaining part of the word)
    if (node == nullptr) {
        // store the remaining part of our string
        node = new TrieNode();
        node->key = word;
        node->endOfWord = true;
        return;
    }

    // if the node exists
    // store the results of string comparing
    vector<string> remains;
    // find the word contained in the node
    string nodeWord = node->key;
    // curLength: the common prefix length
    int curLength = matchHelper(remains, nodeWord, word);
    // four cases:
    // case 1:
    if (remains[0].empty() && remains[1].empty()) {
        // lol, do nothing
        return;
    }
    // case 2:
    else if (remains[0].empty() && !remains[1].empty()) {
        // recursivly perform the insert operation (to the next Child node)
        // the first char of the newWord
        char first = remains[1][0];
        int nextChild = node->get(first);
        insertHelper(node->children[nextChild], remains[1]);
    }
    // case 3:
    else if (!remains[0].empty() && remains[1].empty()) {
        // build a new node, containing the suffix the original word
        TrieNode* newNode = new TrieNode();
        reConnectHelper(newNode, node, nodeWord, curLength);
        // set the newNode as the end of a word
        newNode->endOfWord = true;
    }
    // case 4:
    else {
        string newWordSuffix = remains[1];
        char first = newWordSuffix[0];
        // perform case 3 operation
        // build a new node, containing the suffix the original word
        TrieNode* newNode = new TrieNode();
        reConnectHelper(newNode, node, nodeWord, curLength);
        // set the newNode as the end of a word
        newNode->endOfWord = true;
        int nextChild = newNode->get(first);
        // notice: DEBUG: we cannot set another pointer, when definition,
        // the meaning is not to let the two pointer pointing to the same place
        // set a second new word, and set all its attributes
        node->children[nextChild] = new TrieNode();
        node->children[nextChild]->key = newWordSuffix;
        node->children[nextChild]->endOfWord = true;
    }

}

/*
* insert a new node containing the suffix of some word, reconnecting the original node 
* with the new inserting node and all their children
*/
void CompressedTrie::reConnectHelper(TrieNode*& newNode, TrieNode*& node, string& nodeWord, int curLength) {
    // get the prefix of the nodeWord
    string nodeWordPrefix = nodeWord.substr(0, curLength);
    string nodeWordSuffix = nodeWord.substr(curLength);
    newNode->key = nodeWordSuffix;
    // reconnect the children and reset all the children of the original node
    for (int i = 0; i < SIZE; ++i) {
        newNode->children[i] = node->children[i];
        // child i now point to null
        node->children[i] = nullptr;
    }
    // connect the oroginal node corresponding child to the newNode
    int nextChild = node->get(nodeWordSuffix[0]);
    node->children[nextChild] = newNode;
    // set the key of the original node to the prefix of the word
    node->key = nodeWordPrefix;
}

// we need a function to calculate the last match position of the given word and the word already
// contained in the node
// input: node, a given word and ta vector using to store remaining strings
// output: the two remaining string stroed in a vector and the length that ndoeWord might be cut
/*
* there are several cases of the strings returned in the vector:
* 1. both empty: perfectly match, don't need to do anymore
* 2. nodeWord empty: we should go to the nextChild position and continually run this function, using the
*                    new nodeWord(the word stored in the nextChild) and the new newWord(the remain word from this time),
*                    until we meet (1), (3) or (4).
* 3. newWord empty: we should split the nodeWord into a suffix and a prefix using this return result. In other words,
*                   a new node will be inserted (like inserting a node into a linked-list)
* 4. no one is empty: we need to perform (3), also add a new child containing the remain part of newWord. 
*/
int CompressedTrie::matchHelper(vector<string>& remain, string nodeWord, string newWord) {
    // base case
    if (nodeWord.empty() || newWord.empty()
        || (nodeWord[0] != newWord[0])) {
        // push nodeWord first
        remain.push_back(nodeWord);
        // push newWord second
        remain.push_back(newWord);
        return 0;
    }
    // recursive case
    if (nodeWord[0] == newWord[0]) {
        return 1 + matchHelper(remain, nodeWord.substr(1), newWord.substr(1));
    }
}

bool CompressedTrie::search(const string& word, bool isPrefix) {
    string searchWord = word;
    return searchHelper(root, searchWord, isPrefix);
}

bool CompressedTrie::searchHelper(TrieNode*& node, string& word, bool isPrefix) {
    if (node == nullptr) {
        return false;
    }
    string nodeWord = node->key;
    // we can find the word in the nodeWord
    // base case
    if (nodeWord.find(word) != string::npos) {
        if (isPrefix) {
            return true;
        }
        return node->endOfWord;
    }
    // recursive case
    else if (word.find(nodeWord) != string::npos) {
        int length = nodeWord.length();
        word = word.substr(length);
        int nextChild = node->get(word[0]);
        TrieNode* curNode = node->children[nextChild];
        return searchHelper(curNode, word, isPrefix);
    }
    else {
        return false;
    }
}

bool CompressedTrie::remove(const string& word) {
    return removeHelper(root, word);
}

/*
* this method is much like the delete implementation in the standard trie
*/
bool CompressedTrie::removeHelper(TrieNode*& node, const string& word) {
    if (node == nullptr) {
        return false;
    }
    string nodeWord = node->key;
    // if prefectly match
    // base case
    if (nodeWord == word) {
        // not a leaf node and not word end, it's a prefix
        if (!node->endOfWord) {
            return false;
        }
        // word end, set it
        node->endOfWord = false;
        // leaf node
        if (node->isLeaf()) {
            delete node;
            node = nullptr;
        }
        
        return true;
    }
    // the given word is a prefix (not equal)
    if (nodeWord.find(word) != string::npos) {
        return false;
    }
    // the word contains the nodeWord part
    else if (word.find(nodeWord) != string::npos) {
        // recursion
        vector<string> remains;
        int length = matchHelper(remains, nodeWord, word);
        string newWord = word.substr(length);
        int nextChild = node->get(newWord[0]);
        bool result = removeHelper(node->children[nextChild], newWord);
        if (node->isLeaf() && result && !node->endOfWord) {
            delete node;
            node = nullptr;
            return true;
        }
        return false;
    }
}

int main()
{
    // test
    string keys[] = { "the", "a", "there",
                    "answer", "any", "by",
                    "bye", "their", "hero", 
                    "heroplane", "shaopu", "combine", 
                    "computer", "berkeley"};
    // the number of words
    int n = sizeof(keys) / sizeof(keys[0]);
    // an instance
    CompressedTrie test;

    // test insert function
    for (int i = 0; i < n; ++i) {
        test.insert(keys[i]);
    }
    //// test insert
    //test.traverse();
    // test if the words already exist? also test search function
    cout << "------------insert & search------------" << endl;
    for (auto& key : keys) {
        cout << "search result of " << key << ": " << test.search(key, false) << endl;
    }
    cout << "search result of another word " << "shaopu" << ": " << test.search("shaopu", false) << endl;
    cout << "search result of another word " << "herow" << ": " << test.search("herow", false) << endl;
    cout << "search result of prefix " << "her" << ": " << test.search("her", true) << endl;
    cout << "search result of prefix " << "herow" << ": " << test.search("herow", true) << endl;

    cout << "------------remove------------" << endl;
    // test the remove function

    // after removing there and their, "the" should still be in the tree
    test.remove("there");
    test.remove("their");
    test.remove("answer");
    test.remove("computer");
    test.remove("a");
    for (auto& key : keys) {
        cout << "search result of " << key << ": " << test.search(key, false) << endl;
    }

    test.remove("shaopu");
    cout << "search result of another word " << "shaopu" << ": " << test.search("shaopu", false) << endl;

    return 0;
}

