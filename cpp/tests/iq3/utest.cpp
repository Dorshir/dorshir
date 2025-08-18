#include "mu_test.h"
#include "part3.hpp"
#include <vector>
#include <iostream>

iq::Node* createList(const std::vector<int>& values) {
    if (values.empty()) return nullptr;

    iq::Node* head = new iq::Node(values[0]);
    iq::Node* tail = head;

    for (size_t i = 1; i < values.size(); i++) {
        tail->next = new iq::Node(values[i]);
        tail = tail->next;
    }
    return head;
}

bool compareList(iq::Node* head, const std::vector<int>& expected) {
    size_t index = 0;
    while (head) {
        if (index >= expected.size() || head->value != expected[index]) {
            return false; 
        }
        head = head->next;
        index++;
    }
    return index == expected.size(); 
}

void deleteList(iq::Node* head) {
    while (head) {
        iq::Node* temp = head;
        head = head->next;
        delete temp;
    }
}

BEGIN_TEST(merge_k_test)
    std::vector<iq::Node*> lists = {
        createList({1, 5, 9}),
        createList({2, 6, 10}),
        createList({3, 7, 11}),
        createList({4, 8, 12}),
        createList({13, 17, 21}),
        createList({14, 18, 22}),
        createList({15, 19, 23}),
        createList({16, 20, 24}),
        createList({25, 29, 33}),
        createList({26, 30, 34}),
        createList({27, 31, 35}),
        createList({28, 32, 36}),
        createList({37, 41, 45}),
        createList({38, 42, 46}),
        createList({39, 43, 47}),
    };

    iq::Node* merged = merge_k_lists(lists);

    std::vector<int> expected = {
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36,
        37, 38, 39, 41, 42, 43, 45, 46, 47
    };

    ASSERT_THAT(compareList(merged, expected));

    deleteList(merged);
END_TEST

TEST_SUITE(part3 tests)
    TEST(merge_k_test)
END_SUITE
