#include <iostream>
#include <string>
#include "trie.h"

// constructor
Trie::Trie() {
    // initialize the root
    // call the constructor of the TrieNode struct
    root = nullptr;
    cur_size = 0;
}

// deconstructor
Trie::~Trie() {
    clear(root);
}

/**
 * use recursion to delete the entire tree
 * notice we must delete from the leaf
 */
void Trie::clear(TrieNode* node) {
    if (node) {
        for (int i = 0; i < SIZE; ++i) {
            clear(node->children[i]);
        }
        delete node;
        node = nullptr;
    }
}

// insert function
void Trie::insert(const string& word) {
    // we need a helper function to keep track of
    // the current node
    insertHelper(root, word, 0);
}

/**
 * pass the node by reference
 */
void Trie::insertHelper(TrieNode*& node, const string& word, int index) {
    if (node == nullptr) {
        // build a new node indicating that "this" prefix exists
        // however, we do not need to set the children to a specific alpha
        node = new TrieNode();
    }
    // base case: reach the end of the given word
    // notice: it should be index, not index + 1
    if (word.length() == index) {
        // indicate this is a new word
        node->endOfWord = true;
        // the size++
        cur_size++;
        return;
    }
    int nextChild = node->get(word[index]);
    insertHelper(node->children[nextChild], word, index + 1);
}

bool Trie::search(const string& word, bool isPrefix) {
    return searchHelper(root, word, 0, isPrefix);
}

bool Trie::searchHelper(TrieNode*& node, const string& word, int index, bool isPrefix) {
    if (node == nullptr) {
        // if we meet a null node, then the word/prefix doesn't exist
        return false;
    }
    // base case: reach the end of the given word
    if (word.length() == index) {
        // if judging prefix
        if (isPrefix) {
            return true;
        }
        // if judging word
        else {
            return node->endOfWord;
        }
    }
    // if not null
    int nextIndex = node->get(word[index]);
    return searchHelper(node->children[nextIndex], word, index + 1, isPrefix);
}

bool Trie::remove(const string& word) {
    return removeHelper(root, word, 0);
}

// use the return value indicating the node already be removed?
bool Trie::removeHelper(TrieNode*& node, const string& word, int index) {
    // if the node is already empty, means the word is not contained
    if (node == nullptr) {
        return false;
    }
    // base case
    if (word.length() == index) {
        // try to remove the word
        // under any case, we must first perform this operation
            
        // the given string is not a word (might be a prefix)
        if (!node->endOfWord) {
            return false;
        }
        node->endOfWord = false;

        // is a leaf node, remove it
        // if not a leaf, we don't remove
        if (node->isLeaf()) {
            delete node;
            node = nullptr;
        }
        // change the size
        cur_size--;
        // if not a leaf, don't remove since there might be other words forming after this node
        return true;
    }
    int nextChild = node->get(word[index]);
    // if we can go into this function, it means node != nullptr
    bool result = removeHelper(node->children[nextChild], word, index + 1);
    // if this becomes a leaf node, remove it
    // node becomes a leaf after processing its children
    // if one node do not need to be removed, then all the anscestor nodes don't to be removed
    // the node itself is not indicating the end of some other word
    if (node->isLeaf() && result && !node->endOfWord) {
        // free the memory
        delete node;
        // set the node to nullptr
        node = nullptr;
        return true;
    }
    // otherwise
    return false;
}

string Trie::longestPrefix(const string& word) {
    int length = longestPrefixHelper(root, word, 0, 0);
    return word.substr(0, length);
}

/**
* the requirement is to find the longest prefix in the given string which is
* also a word in the dictionary
* input: a string, might not be a word, but might contain a word as its prefix
* output: the longest word contained in the given string
*/
int Trie::longestPrefixHelper(TrieNode*& node, const string& word, int index, int length) {
    // only parts of the string match the words in the dictionary
    if (node == nullptr) {
        return length;
    }
    // reach the end of the given word
    if (word.length() == index) {
        // if this is a word too
        if (node->endOfWord) {
            length = index;
            return length;
        }
        // otherwise, return the previous result
        return length;
    }
    // update the length when me meet a word in the dict
    if (node->endOfWord) {
        length = index;
    }
    int nextChild = node->get(word[index]);
    return longestPrefixHelper(node->children[nextChild], word, index + 1, length);
}

vector<string> Trie::keysWithPrefix(const string& word) {
    vector<string> chosen;
    string newWord = word;
    int length = word.length();
    keysWithPrefixHelper(root, chosen, newWord, 0, length);
    return chosen;
}

// keys = words here
void Trie::keysWithPrefixHelper(TrieNode*& node, vector<string>& chosen, string word, int index, int length) {
    // there is not matches in this road when we meet a null pointer
    // base case 1
    if (node == nullptr) {      
        // no one matches me in the dictionary!
        return;
    }
    // meet a word with the given prefix (or the word(prefix) itself), then add it to the chosen vector
    // base case 2
    if (node->endOfWord && index >= length) {
        chosen.push_back(word);
    }
    // the prefix has not been finished traversing yet, then must go along the prefix itself's road
    // when index = length, we should not gointo this branch again
    if (index < length) {
        int nextChild = node->get(word[index]);
        // recursion traversing
        keysWithPrefixHelper(node->children[nextChild], chosen, word, index + 1, length);
    }
    // if the prefix is finished traversing, then find all possbilities
    else {
        // need to look at every possible combinations
        for (int i = 0; i < SIZE; ++i) {
            char ch = 'a' + i;
            keysWithPrefixHelper(node->children[i], chosen, word + ch, index + 1, length);
        }
    }

}

int main() {
    // test
    string keys[] = { "the", "a", "there",
                    "answer", "any", "by",
                    "bye", "their", "hero", "heroplane" };
    // the number of words
    int n = sizeof(keys) / sizeof(keys[0]);
    // an instance
    Trie test;

    // test insert function
    for (int i = 0; i < n; ++i) {
        test.insert(keys[i]);
    }
    // test if the words already exist? also test search function
    cout << "------------insert & search------------" << endl;
    for (auto& key : keys) {
        cout << "search result of " << key << ": " << test.search(key, false) << endl;
    }
    cout << "search result of another word " << "shaopu" << ": " << test.search("shaopu", false) << endl;
    cout << "search result of prefix " << "her" << ": " << test.search("her", true) << endl;

    cout << "------------remove------------" << endl;
    // test the remove function

    // after removing there and their, "the" should still be in the tree
    test.remove("there");
    test.remove("their");
    test.remove("answer");
    for (auto& key : keys) {
        cout << "search result of " << key << ": " << test.search(key, false) << endl;
    }

    test.remove("shaopu");
    cout << "search result of another word " << "shaopu" << ": " << test.search("shaopu", false) << endl;


    cout << "------------longestPrefix------------" << endl;
    test.insert("there");
    test.insert("their");
    test.insert("answer");
    cout << test.longestPrefix("thewe") << endl;
    cout << test.longestPrefix("their") << endl;
    cout << test.longestPrefix("theirwe") << endl;
    cout << test.longestPrefix("ans") << endl;
    cout << test.longestPrefix("wefwe") << endl;
    cout << test.longestPrefix("answerasd") << endl;
    cout << test.longestPrefix("therwer") << endl;

    cout << "------------keysWithPrefix------------" << endl;
    vector<string> chosen;
    chosen = test.keysWithPrefix("a");
    for (auto& word : chosen) {
        cout << word << endl;
    }
    ////test.keysWithPrefix("wer");
    //test.keysWithPrefix("an");

    return 0;
}