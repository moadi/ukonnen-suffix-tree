An implementation of Ukonnen's linear time suffix tree algorithm. Based on the two descriptions provided here:

http://stackoverflow.com/questions/9452701/ukkonens-suffix-tree-algorithm-in-plain-english

In main.cpp, set the variable input to the string you want to test. Usually for the tree to be complete, the last character in the string should be unique. Otherwise each suffix will not end at a leaf node. This could make it seem like the output is incorrect due to a not all suffixes being shown.

To compile: g++ -std=c++0x -o <output-file-name> SuffixTree.cpp main.cpp

Just run the executable and see the output. It should be a list of all the suffixes in the tree (hopefully).
