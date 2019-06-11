#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <vector>

class trie {
    struct node {
        char letter;
        std::unordered_map<char, node*> nodes;
        node(char c):letter(c){}
    };
    int s;
    node* root;
    void dfs(node*, std::string, std::vector<std::string>&);

public:
    trie(std::string);
    trie();
    void add_word(std::string);
    bool has_word(std::string);
    int get_size();
    std::vector<std::string> get_all_words_with_prefix(std::string);
};