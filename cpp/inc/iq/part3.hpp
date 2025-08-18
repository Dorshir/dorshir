#ifndef IQ_PART3_HPP
#define IQ_PART3_HPP

#include <memory>
#include <vector>

namespace iq {

struct Node {
    int value;
    Node* next;

    Node(int val = 0) 
    : value(val)
    , next(nullptr) 
    {}
};


/*  1.  You are given a vector of k linked-lists lists, each linked-list is sorted in ascending order.
        Merge all the linked-lists into one sorted linked-list and return it. 
        
        Pseudo code:

        std::unique_pointer<Node> merge_k_lists(std::vector<std::unique_pointer<Node>>& lists, size_t left, size_t right)
        {
            if lists.size == 0 {
                return {}
            }
            else if lists.size == 1 {
                return lists[0]
            }

            Naive : k pointers (like merge with 2 lists..)

            More efficient : split and merge each 2 halves

            if left == right : return lists[left]
            if left > right : return null

            mid = left + (right-left)/2

            return merge_two_lists(merge_k_lists(lists, left, mid), merge_k_lists(lists, mid, right))
        }
*/


/*  2.  Given an text file group all words in the file into groups of anagrams. 
        return a vector of lists where each list contain words that are anagrams to each other. 
        the vector should be sorted by length of lists (descending).

        Pseudo code:

        vector<lists> get_anagrams(file)

            map<key=sorted_word, value=list_of_anagrams>

            while file:
                if sort(next word) in map:
                    add as anagram
                else:
                    add as new key with new list for the anagrams
            
            
            return a vector contains the sorted map
*/


/*  3.  Given an m x n 2D grid(vector<vector<int>>) which represents a map of '1's (land) and '0's (water), return the number of islands.
        An island is surrounded by water and is formed by connecting adjacent lands horizontally or vertically. 
        You may assume all four edges of the grid are all surrounded by water.

            
*/


Node* merge_k_lists(std::vector<Node*>& lists);




}

#endif // IQ_PART3_HPP