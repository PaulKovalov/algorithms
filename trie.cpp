#include "trie.h"
trie::trie() {
    root = nullptr;
    s = 0;
}
trie::trie(std::string t) {
    trie();  
} 
void trie::add_word(std::string word) {
    if (root == nullptr) {
        root = new node(' ');
    } 
    node* current = root;
    if (current->nodes[word[0]] == 0) {
        for (int i = 0; i < word.length(); ++i) {
            current->nodes[word[i]] = new node(word[i]);
            current = current->nodes[word[i]];
        }
    } else {
        int i = 0;
        while (current->nodes[word[i]] != 0) {
            current = current->nodes[word[i]];
            ++i;
        }
        for (int j = i; j < word.length(); ++j) {
            current->nodes[word[j]] = new node(word[j]);
            current = current->nodes[word[j]];
        }
    }
    
}
bool trie::has_word(std::string word) {
    node *current = root;
    int i = 0;
    while(i < word.length() && current->nodes[word[i]] != 0) {
        current = current->nodes[word[i]];
        ++i;
    }
    if (i == word.length()) {
        return true;
    } 
    return false;
}
void trie::dfs(node* n, std::string p, std::vector<std::string> &w) {
    if (n->nodes.size() == 0) {
        w.push_back(p);
        return;
    }
    std::unordered_map<char, node*>::iterator it;
    for (it = n->nodes.begin(); it != n->nodes.end(); ++it) {
        // std::cout << p+it->first << std::endl;
        dfs(it->second, p+it->first, w);
    }
    
}
std::vector<std::string> trie::get_all_words_with_prefix(std::string prefix) {
    node *current = root;
    int i = 0;
    while(i < prefix.length() && current->nodes[prefix[i]] != 0) {
        current = current->nodes[prefix[i]];
        ++i;
    }
    std::vector<std::string> all_words;
    dfs(current, prefix, all_words);
    return all_words;
}