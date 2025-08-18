#include "part3.hpp"


namespace iq {


Node* merge_k_lists_helper(std::vector<Node*>& lists, size_t left, size_t right);
Node* merge_two_lists(Node* first_list, Node* second_list);


Node* merge_k_lists(std::vector<Node*>& lists)
{
    size_t size = lists.size();
    if (size == 0) {
        return nullptr;
    }
    else if (size == 1) {
        return lists[0];
    }

    return merge_k_lists_helper(lists, 0, lists.size() - 1);
}



Node* merge_k_lists_helper(std::vector<Node*>& lists, size_t left, size_t right)
{
    if (left == right) {
        return lists[left];
    }
    else if (left > right) {
        return nullptr;
    }

    size_t mid = left + (right - left) / 2;

    return merge_two_lists(merge_k_lists_helper(lists, left, mid), merge_k_lists_helper(lists, mid + 1, right));
}


Node* merge_two_lists(Node* first_list, Node* second_list)
{
    if (first_list == nullptr) {
        return second_list;
    }
    else if (second_list == nullptr) {
        return first_list;
    }


    Node temp;
    Node* curr = &temp;


    while (first_list != nullptr && second_list != nullptr) {
        if (first_list->value < second_list->value) {
            curr->next = first_list;
            first_list = first_list->next;
        }
        else {
            curr->next = second_list;
            second_list = second_list->next;
        }
        curr = curr->next;
    }

    if (first_list != nullptr) {
        curr->next = first_list;
    }
    else if (second_list != nullptr) {
        curr->next = second_list;
    }

    return temp.next;
}

}