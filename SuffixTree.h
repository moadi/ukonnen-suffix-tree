#include <unordered_map>
#include <memory>
#include <string>
#include <cstddef>
#include <iostream>
#include <vector>

#ifndef SuffixTree_SuffixTree_h
#define SuffixTree_SuffixTree_h


class Node {
   
  public:
    
    int start;
    int* end;
    Node* suffix_link;
    bool is_leaf;
    std::unordered_map<char, std::unique_ptr<Node>> edges;
    
    // we assume by default that a newly created node is a leaf node
    // ONLY exception is root
    Node(int start_idx, int* end_idx, Node* ptr) :
    start(start_idx), end(end_idx), suffix_link(ptr), is_leaf(true) { }
    
    Node()
    {
        int* root_end = new int;
        *root_end = -1;
        end = root_end;
    }
    
    ~Node()
    {
        // if the node is not a leaf node,
        // it's end is allocated on heap.
        // leaf nodes point to curr_end
        if(!is_leaf)
            delete end;
        suffix_link = nullptr;
    }
    
};


struct ActivePoint {
    
    Node* active_node;
    int active_edge;
    std::size_t active_length;
    
};

class SuffixTree {
    
  public:
    SuffixTree(const std::string& str) :
    input_string(str), curr_end(-1), root(new Node()),
    remainder(0)
    {
        root->is_leaf = false;
        active_point.active_node = root.get();
        active_point.active_edge = -1;
        active_point.active_length = 0;
    }
    
    void build_suffix_tree();
    void print();
    
  private:
    const std::string& input_string;
    int curr_end;
    std::unique_ptr<Node> root;
    ActivePoint active_point;
    std::size_t remainder;
    
    void add_to_tree(int);
    void print_tree(Node*);
    Node* split_edge(Node*, int);
    void print_vector(std::vector<std::pair<int, int>>&);
    
    void print_string(int s, int e)
    {
        std::cout << input_string.substr(s, e-s+1);
        std::cout.flush();
    }
};

#endif
