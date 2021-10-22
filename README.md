# Tries
 my C++ implementation of standard trie and compressed trie

## NOTICE

When the article we stores does not only contain chars from *a-z*. We cannot use a `children[max_size]` to indicate the position of the 26 letters.

Instead, a better way is to use a **map** or **hashmap**, where the key is a one-length string (we don't use char as its key since some more rare characters may have strange values that make debugging more confusing). And the corresponding value will be the next `TrieNode` or the `TrieNode*`. This version can be found in my another [repository](https://github.com/SongShaopu1998/Stanford-CS-106X/blob/main/Homework6C-MiniBrowswer2/Autocomplete.cpp).

