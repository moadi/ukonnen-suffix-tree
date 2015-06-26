#include "SuffixTree.h"
#include <cassert>
#include <sstream>


Node* SuffixTree::split_edge(Node* ptr, int curr_pos)
{
    int* new_end = new int;
    *new_end = ptr->start + active_point.active_length - 1;
    Node* rc = nullptr;
    
    std::unique_ptr<Node> node1;
    
    // leaf nodes and internal nodes are split differently
    if (ptr->is_leaf == false)
    {
        std::unique_ptr<Node> node(new Node(*new_end+1, ptr->end, root.get()));
        node1 = std::move(node);
    }
    else
    {
        std::unique_ptr<Node> node(new Node(*new_end+1, &curr_end, root.get()));
        node1 = std::move(node);
    }
    
    std::unique_ptr<Node> node2(new Node(curr_pos, &curr_end, root.get()));
    ptr->end = new_end;
    
    char a = input_string[node1->start];
    char b = input_string[node2->start];
    
    // since we are splitting an internal node
    // the edges of ptr are shifted to node1
    // in this case it's like ptr is the new internal node being created
    if (ptr->is_leaf == false)
    {
        node1->is_leaf = false;
        for (auto it = ptr->edges.begin(); it != ptr->edges.end(); )
        {
            node1->edges.insert(std::make_pair(it->first, std::move(it->second)));
            it = ptr->edges.erase(it);
        }
        // node1 is a new internal node
        rc = node1.get();
        node1->suffix_link = ptr->suffix_link;
    }
    
    ptr->is_leaf = false;
    ptr->edges.insert(std::make_pair(a, std::move(node1)));
    ptr->edges.insert(std::make_pair(b, std::move(node2)));
    return rc;
}


void SuffixTree::print_vector(std::vector<std::pair<int, int>>& vec)
{
    for(auto& indices : vec)
    {
        print_string(indices.first, indices.second);
    }
    std::cout << std::endl;
}

void SuffixTree::add_to_tree(int pos) {
    
    ++curr_end; // automatically update all leaf nodes to new end
    
    ++remainder; // increment remainder as we are attempting to insert another character
    
    Node* last_new_node = nullptr;
    
    while (remainder > 0)
    {
        // if active length is 0, then
        // only the character at the current index
        // remains to be inserted
        if (active_point.active_length == 0)
        {
            active_point.active_edge = pos;
        }
        
        // check if the character is present at this node
        auto it = active_point.active_node->edges.find(input_string[active_point.active_edge]);
        
        if (it == active_point.active_node->edges.end())
        {
            // create a new leaf node
            std::unique_ptr<Node> node(new Node(pos, &curr_end, root.get()));
            
            // add it to active node's edges
            active_point.active_node->edges.insert(std::make_pair(input_string[active_point.active_edge], std::move(node)));
            active_point.active_node->is_leaf = false;
            
            if (last_new_node != nullptr)
            {
                last_new_node->suffix_link = active_point.active_node;
                last_new_node = nullptr;
            }
        }
        else
        {
            Node* ptr = it->second.get();
            
            // check if we are falling off
            if (ptr->start + active_point.active_length > *(ptr->end))
            {
                active_point.active_node = ptr;
                active_point.active_edge += (*(ptr->end) - ptr->start + 1);
                active_point.active_length -= (*(ptr->end) - ptr->start + 1);
               continue;
            }
            
            // check if the next character matches the current one
            if (input_string[ptr->start + active_point.active_length] == input_string[pos])
            {
                ++active_point.active_length;
                if (last_new_node != nullptr && active_point.active_node != root.get())
                {
                    last_new_node->suffix_link = active_point.active_node;
                    last_new_node = nullptr;
                }
                return;
            }
            
            // if we are here, the character does not match
            // need to create a new internal node
            Node* node = split_edge(ptr, pos);
            
            if (node && (last_new_node == ptr))
            {
                Node* temp = last_new_node;
                last_new_node = node;
                ptr = temp;
            }
            
            if (last_new_node != nullptr)
            {
                last_new_node->suffix_link = ptr;
            }
            last_new_node = ptr;
        }
        --remainder;
        if (active_point.active_node == root.get() && active_point.active_length > 0)
        {
            --active_point.active_length;
            active_point.active_edge = pos - remainder + 1;
        }
        else if(active_point.active_node != root.get())
        {
            active_point.active_node = active_point.active_node->suffix_link;
        }
    }
}


void SuffixTree::print_tree(Node* node)
{
    static std::vector<std::pair<int, int>> indices;
    
    if (node != root.get())
    {
        indices.push_back(std::make_pair(node->start, *(node->end)));
    }
    
    for (auto& it : node->edges)
    {
        print_tree(it.second.get());
    }
    
    if (node->is_leaf == true)
    {
        print_vector(indices);
    }
    if (indices.size() != 0)
    {
        indices.pop_back();
    }
}

void SuffixTree::build_suffix_tree() {
    
    for (int i = 0; i < input_string.size(); ++i)
    {
        add_to_tree(i);
    }
}

void SuffixTree::print() {
    
    print_tree(root.get());
    std::cout << "\n\n";
}


