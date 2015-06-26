#include <iostream>
#include <ctime>

#include "SuffixTree.h"

int main(int argc, const char * argv[])
{
    std::string input("cdddc$");
//    int* root_end = new int;
//    *root_end = -1;
    
    SuffixTree st(input);
    
    std::clock_t start_time = std::clock();
    st.build_suffix_tree();
    st.print();
    std::cout << "Time taken = " << (std::clock() - start_time) / static_cast<double>(CLOCKS_PER_SEC/1000) << " ms\n\n";
    
    return 0;
}
