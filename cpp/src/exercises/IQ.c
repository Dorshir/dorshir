/* Single linked list has cycle? */
struct Node {
    int val;
    Node* next;
};

bool hasCycle(Node* head) {

    if (!head) {
        return false;
    }

    Node* slow = head;
    Node* fast = head->next;

    while (fast != NULL && fast->next != NULL) {

        if (slow == fast) {
            return true;
        }

        slow = slow->next;
        fast = fast->next->next;
    }

    return false;
}

/* Describe a cyclic single linked list in C  
Given a pointer to a node in such a list remove it.  In o(1)  */

typedef struct Node {
    int data;
    struct Node* next;
} Node;


void DeleteNode(Node* _node) {
    
    if (_node == NULL) {
        return;
    } else if (_node->next == _node) {
        free(_node);
        _node = NULL;
        return;
    }


    Node* temp = _node->next;
    _node->data = temp->data;
    _node->next = temp->next;

    free(temp);
    temp = NULL;
}

/* Detect cycle and return the starting point of the cycle */
Node* detectCycle(Node* head) {
    if (!head) {
        return NULL;
    }

    Node* slow = head;
    Node* fast = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;

        if (slow == fast) {
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow;
        }
    }

    return NULL;
}

/* Reverse single linked list */
Node* reverseList(Node* head) {

    if (!head || !head->next) {
        return head;
    }

    Node* prev = NULL;
    Node* next = head;

    while (head) {
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }

    return prev;
}


/* String to uppercase... */
char* toUpperCopy(const char* str) {
    if (!str) {
        return NULL;
    }

    size_t length = strlen(str);

    char* upper_str = malloc(sizeof(char) * (length + 1));
    if (!upper_str){
        return NULL;
    }

    for (size_t i = 0; i < length; i++) {
        upper_str[i] = toupper(str[i]);
    }

    upper_str[length] = '\0';

    return upper_str;
    
}


/* Write a recursive implementation of int strcmp(s1, s2) that will compare two strings. */
int strcmp_rec(const char* s1, const char* s2) {

    if (s1 == NULL && s2 == NULL) {
        return 0;
    }

    if (s1 == NULL) {
        return -1;
    }

    if (s2 == NULL) {
        return 1;
    }

    if (*s1 == '\0' && *s2 == '\0') {
        return 0;
    }

    return strcmp_rec(s1 + 1, s2 + 1);
}


/* Write a function to trim multiple consecutive spaces to one space in a string.  
char s[] = “he was    was a good     guy”;
strim(s);    
now s contains “he was a good guy” */

void trim(char* _str) {
    if (_str == NULL) {
        return;
    }

    int left = 0;
    int right = 0;

    while (_str[right] != '\0') {

        if (_str[right] == ' ') {
            if (!in_space) {
                _str[left++] = ' ';
                in_space = true;
            }
        } else {
            _str[left++] = _str[right];
            in_space = false;
        }
        ++right;
    }

    if (_str[left - 1] == ' ') {
        --left;
    }

    _str[left] = '\0';
}




typedef struct Node {

    void* data;
    struct Node* next;
} Node;

typedef struct List {

    Node* head;
} List;


typedef int (*KeyCompareFunc)(const void* k1, const void* k2);

void swap_nodes(List* list, const void* key1, const void* key2, KeyCompareFunc cmp) {

    if (list == NULL || key1 == NULL || key2 == NULL || cmp == NULL) {
        return;
    }

    Node* first = NULL;
    Node* second = NULL;

    Node* prev1 = NULL;
    Node* prev2 = NULL;

    Node* curr = list->head;

    while (curr != NULL) {

        if (cmp(curr->data, key1)) {
            first = curr;
            if (second) {
                break;
            }
        } else if (cmp(curr->data, key2)) {
            second = curr;
            if (first) {
                break;
            }
        }

        if (!first) {
            prev1 = curr;
        }
        if (!second) {
            prev2 = curr;
        }

        curr = curr->next;
    }

    if (curr != NULL) {
        prev1->next = second;
        prev2->next = first;
        
        Node * temp = first->next;
        first->next = second->next;
        second->next = temp;
    }
}


/* 36. Given a single linked list, write a function that will reverse the list. */

/* Recursive Approach */

void ReverseHelper(Node* node, Node** new_head) {

    if (node->next == NULL) {
        *new_head = node;
        return;
    }

    ReverseHelper(node->next, new_head);

    node->next->next = node;
    node->next = NULL;
}

void ReverseListRec(List* list) {

    if (!list || !list->head) {
        return;
    }

    Node* new_head = NULL;

    ReverseHelper(list->head, &new_head);

    list->head = new_head;
}


/* Iterative Approach */

void ReverseListIter(List* list) {

    if (!list || !list->head) {
        return;
    }

    Node* prev = NULL;
    Node* curr = list->head;
    Node* next = NULL;

    while (curr) {

        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }

    list->head = prev;
}


/* 33. Given a single linked list, write a function that will find in one traversal the middle 
of the linked list. */
void FindMid(List* list, Node** mid) {

    if (!list || !list->head || !mid) {
        return;
    }

    Node* slow = list->head;
    Node* fast = list->head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    *mid = slow;
}


/* 32. Write a function that given a binary tree returns true if the tree is a binary search 
tree. */

typedef struct TreeNode {
    int val;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

bool IsBSTHelper(TreeNode* node, int curr_min, int curr_max) {

    if (node == NULL) {
        return true;
    }

    if (node <= curr_min || node >= curr_max) {
        return false;
    }

    return IsBSTHelper(node->left, curr_min, node->val) && IsBSTHelper(node->right, node->val, curr_max);
}

bool IsBST(TreeNode* root) {
    
    if (root == NULL) {
        return false;
    }

    return IsBSTHelper(root, INT_MIN, INT_MAX);
}


bool AreIdenticalTrees(TreeNode* head1, TreeNode* head2) {

    if (!head1 && !head2) {
        return true;
    } else if (!head1 || !head2 || (head1->val != head2->val)) {
        return false;
    }

    return AreIdenticalTrees(head1->left, head2-> left) && AreIdenticalTrees(head1->right, head2->right);
}


void my_memcpy(const void* src, void* dest, size_t n) {

    if (n == 0 || src == dest) {
        return;
    }

    const size_t A = sizeof(size_t);
    const uint8_t* s8 = (const uint8_t*)src;
    uint8_t* d8 = (uint8_t*)dest;

    while (n > 0 && ((((uintptr_t)s8) % A != 0) || (((uintptr_t)d8) % A != 0))) {
        *d8++ = *s8++;
        --n;
    }

    const size_t* sw = (const size_t*)s8;
    size_t* dw = (size_t*)d8;

    while (n >= A) {
        *dw++ = *sw++;
        n -= A;
    }

    s8 = (const uint8_t*)sw;
    d8 = (uint8_t*)dw;

    while (n--) {
        *d8++ = *s8++;
    }
}

void my_memmove(void* dst, void const* src, size_t n) {

    if (n == 0 || dst == src) {
        return;
    }

    uint8_t const* ps = (uint8_t const*)src;
    uint8_t* pd = (uint8_t*)dst; 

    if (pd < ps || pd >= ps + n) {
        while (n--) {
            *pd++ = *ps++;
        }
    } else {
        ps += n;
        pd += n;

        while (n--) {
            *--pd = *--ps;
        }
    }
}



// myallocator

void* my_malloc(size_t size);
void my_free(void* ptr);

#define MAGIC_ALLOC 0xDEADBEEF
#define MAGIC_FREE  0xBADC0DEU


typedef struct alignas(max_align_t) BlockHeader {

    size_t size;
    unsigned int magic;
    struct BlockHeader* prev;
    struct BlockHeader* next;

} BlockHeader;

static BlockHeader* g_list_head = NULL;

static void list_insert(BlockHeader* header) {

    if (!header) {
        return;
    }

    header->prev = NULL;
    header->next = g_list_head;

    if (g_list_head) {
        g_list_head->prev = header;
    }

    g_list_head = header;
}

static void list_remove(BlockHeader* header) {

    if (!header) {
        return;
    }

    if (header->prev) {
        header->prev->next = header->next;
    } else {
        g_list_head = header->next;
    }

    if (header->next) {
        header->next->prev = header->prev;
    }
}

void* my_malloc(size_t size) {

    if (size == 0) {
        return NULL;
    } else if (size > SIZE_MAX - sizeof(BlockHeader)) {
        return NULL;
    }

    BlockHeader* header = malloc(sizeof(BlockHeader) + size);
    if (!header) {
        return NULL;
    }

    header->size = size;
    header->magic = MAGIC_ALLOC;

    list_insert(header);

    return (void*)(header + 1);
}


void  my_free(void* ptr) {

    if (!ptr) {
        return;
    }

    BlockHeader* ptr_header = ((BlockHeader*)ptr) - 1;

    if (ptr_header->magic != MAGIC_ALLOC) {
        return;
    }

    ptr_header->magic = MAGIC_FREE;

    list_remove(ptr_header);

    free(ptr_header);
}