#include "BSTree.h"
#include <stdlib.h> /* Dynamic Memory */
#include <math.h>   /* fmax() */

#define LEFT_CHILD(_it) (((Node *)_it)->m_left)
#define RIGHT_CHILD(_it) (((Node *)_it)->m_right)
#define FATHER(_it) (((Node *)_it)->m_father)
#define DATA(_it) (((Node *)_it)->m_data)

#define FATHERS_LEFT_CHILD(_it) (((Node *)_it)->m_father->m_left)
#define FATHERS_RIGHT_CHILD(_it) (((Node *)_it)->m_father->m_right)
#define IS_FATHERS_RIGHT_CHILD(_it) (_it == ((Node *)_it)->m_father->m_right)
#define IS_FATHERS_LEFT_CHILD(_it) (_it == ((Node *)_it)->m_father->m_left)

#define HAS_ZERO_CHILDS(_it) (LEFT_CHILD(_it) == NULL && RIGHT_CHILD(_it) == NULL)
#define HAS_ONLY_RIGHT_CHILD(_it) (LEFT_CHILD(_it) == NULL && RIGHT_CHILD(_it) != NULL)
#define HAS_ONLY_LEFT_CHILD(_it) (RIGHT_CHILD(_it) == NULL && LEFT_CHILD(_it) != NULL)
#define HAS_ONLY_ONE_CHILD(_it) (HAS_ONLY_RIGHT_CHILD(_it) || HAS_ONLY_LEFT_CHILD(_it))
#define HAS_TWO_CHILDS(_it) (LEFT_CHILD(_it) != NULL && RIGHT_CHILD(_it) != NULL)

#define TRUE_TREE_ROOT(_tree) (_tree->m_root.m_left)
#define IS_EMPTY_TREE(_tree) (TRUE_TREE_ROOT(_tree) == NULL)
#define END_OF_TREE(_tree) ((void *)&_tree->m_root)
#define IS_END_OF_TREE(_it) ((Node *)_it == ((Node *)_it)->m_father)

#define TWO_CHILD_DIFF(_first, _second) (HAS_TWO_CHILDS(_first) && !HAS_TWO_CHILDS(_second)) || (!HAS_TWO_CHILDS(_first) && HAS_TWO_CHILDS(_second))
#define ONE_CHILD_DIFF(_first, _second) (HAS_ONLY_ONE_CHILD(_first) && !HAS_ONLY_ONE_CHILD(_second)) || (!HAS_ONLY_ONE_CHILD(_first) && HAS_ONLY_ONE_CHILD(_second))
#define ONE_CHILD_POS_DIFF(_first, _second) (HAS_ONLY_LEFT_CHILD(_first) && HAS_ONLY_RIGHT_CHILD(_second)) || (HAS_ONLY_RIGHT_CHILD(_first) && HAS_ONLY_LEFT_CHILD(_second))
#define ZERO_CHILD_DIFF(_first, _second) (HAS_ZERO_CHILDS(_first) && !HAS_ZERO_CHILDS(_second)) || (!HAS_ZERO_CHILDS(_first) && HAS_ZERO_CHILDS(_second))
#define ITSELF_DIFF(_first, _second) ((_first == NULL && _second != NULL) || (_first != NULL && _second == NULL))

typedef struct Node Node;

typedef enum Child
{
    LEFT,
    RIGHT
} Child;

struct Node
{
    void *m_data;
    Node *m_left;
    Node *m_right;
    Node *m_father;
};

struct BSTree
{
    Node m_root;
    LessComparator m_less;
};

/********************** Help Declerations **********************/

static void *RemoveZeroChilds(BSTreeItr _it);
static void SwapChildWithFatherChild(BSTreeItr _it, Child _child);
static void *RemoveOneChild(BSTreeItr _it);
static void InitNode(Node *_newNode, void *_item, BSTreeItr _it);
static BSTreeItr InsertItem(BSTreeItr _it, BSTreeItr _end, void *_item, LessComparator _less);
static void DestroyTree(BSTreeItr _node, void (*_destroyer)(void *));
static void InitTree(BSTree *_tree, LessComparator _less);
static BSTreeItr LeftMostChild(BSTreeItr _it);
static BSTreeItr RightMostChild(BSTreeItr _it);
static void SwapData(BSTreeItr _it, BSTreeItr _next);
static BSTreeItr InsertRoot(BSTree *_tree, void *_item);

/********************** API Functions **********************/

BSTree *BSTreeCreate(LessComparator _less)
{
    BSTree *newTree;
    if (_less == NULL)
    {
        return NULL;
    }
    newTree = malloc(sizeof(BSTree));
    if (newTree == NULL)
    {
        return NULL;
    }

    InitTree(newTree, _less);
    return newTree;
}

void BSTreeDestroy(BSTree **_tree, void (*_destroyer)(void *))
{
    if (_tree == NULL || *_tree == NULL)
    {
        return;
    }
    DestroyTree(TRUE_TREE_ROOT((*_tree)), _destroyer);
    free(*_tree);
    *_tree = NULL;
}

BSTreeItr BSTreeInsert(BSTree *_tree, void *_item)
{
    if (_tree == NULL)
    {
        return NULL;
    }
    if (_item == NULL)
    {
        return END_OF_TREE(_tree);
    }
    if (TRUE_TREE_ROOT(_tree) == NULL)
    {
        return InsertRoot(_tree, _item);
    }
    return InsertItem(TRUE_TREE_ROOT(_tree), END_OF_TREE(_tree), _item, _tree->m_less);
}

BSTreeItr BSTreeItrBegin(const BSTree *_tree)
{
    if (_tree == NULL)
    {
        return NULL;
    }
    else if (IS_EMPTY_TREE(_tree))
    {
        return END_OF_TREE(_tree);
    }
    return LeftMostChild(TRUE_TREE_ROOT(_tree));
}

BSTreeItr BSTreeItrEnd(const BSTree *_tree)
{
    if (_tree == NULL)
    {
        return NULL;
    }
    return END_OF_TREE(_tree);
}

int BSTreeItrEquals(BSTreeItr _a, BSTreeItr _b)
{
    return _a == _b;
}

BSTreeItr BSTreeItrNext(BSTreeItr _it)
{
    if (_it == NULL)
    {
        return NULL;
    }
    if (RIGHT_CHILD(_it) != NULL)
    {
        return LeftMostChild(RIGHT_CHILD(_it));
    }
    while (IS_FATHERS_RIGHT_CHILD(_it))
    {
        _it = FATHER(_it);
    }

    return FATHER(_it);
}

BSTreeItr BSTreeItrPrev(BSTreeItr _it)
{
    BSTreeItr temp = _it;
    if (_it == NULL)
    {
        return NULL;
    }
    if (LEFT_CHILD(_it) != NULL)
    {
        return RightMostChild(LEFT_CHILD(_it));
    }
    while (IS_FATHERS_LEFT_CHILD(_it))
    {
        _it = FATHER(_it);
    }
    _it = FATHER(_it);
    /* if we reached the end of the tree, _it was pointing at the beginning, return beginning */
    if (IS_END_OF_TREE(_it))
    {
        return temp;
    }
    return _it;
}

void *BSTreeItrRemove(BSTreeItr _it)
{
    BSTreeItr next;
    void *data;
    if (_it == NULL || IS_END_OF_TREE(_it))
    {
        return NULL;
    }

    data = DATA(_it);
    if (HAS_ZERO_CHILDS(_it))
    {
        RemoveZeroChilds(_it);
    }
    else if (HAS_ONLY_ONE_CHILD(_it))
    {
        RemoveOneChild(_it);
    }
    else
    {
        next = BSTreeItrNext(_it);
        SwapData(_it, next);
        return BSTreeItrRemove(next);
    }
    free(_it);
    _it = NULL;
    return data;
}

void *BSTreeItrGet(BSTreeItr _it)
{
    if (_it == NULL || IS_END_OF_TREE(_it))
    {
        return NULL;
    }
    return DATA(_it);
}

BSTreeItr BSTreeForEach(const BSTree *_tree, BSTreeTraversalMode _mode, ActionFunction _action, void *_context)
{
    return NULL;
}

/********************** Help Functions **********************/

static void *RemoveZeroChilds(BSTreeItr _it)
{
    if (IS_FATHERS_LEFT_CHILD(_it))
    {
        FATHERS_LEFT_CHILD(_it) = NULL;
    }
    else
    {
        FATHERS_RIGHT_CHILD(_it) = NULL;
    }
    return _it;
}

static void SwapChildWithFatherChild(BSTreeItr _it, Child _child)
{
    Node *childNode = (_child == LEFT) ? LEFT_CHILD(_it) : RIGHT_CHILD(_it);

    if (IS_FATHERS_LEFT_CHILD(_it))
    {
        FATHERS_LEFT_CHILD(_it) = childNode;
    }
    else
    {
        FATHERS_RIGHT_CHILD(_it) = childNode;
    }
    childNode->m_father = FATHER(_it);
}

static void *RemoveOneChild(BSTreeItr _it)
{
    if (HAS_ONLY_LEFT_CHILD(_it))
    {
        SwapChildWithFatherChild(_it, LEFT);
    }
    else
    {
        SwapChildWithFatherChild(_it, RIGHT);
    }
    return _it;
}

static void InitNode(Node *_newNode, void *_item, BSTreeItr _father)
{
    _newNode->m_data = _item;
    _newNode->m_left = NULL;
    _newNode->m_right = NULL;
    _newNode->m_father = _father;
}

static BSTreeItr InsertItem(BSTreeItr _it, BSTreeItr _end, void *_item, LessComparator _less)
{
    Node *newNode;
    /* Duplicates not allowed, return end */
    if (_less(_item, DATA(_it)) == 0)
    {
        return _end;
    }
    /* item > node's data, go right */
    else if (_less(_item, DATA(_it)) < 0)
    {
        if (RIGHT_CHILD(_it) == NULL)
        {
            newNode = malloc(sizeof(Node));
            if (newNode == NULL)
            {
                return _end;
            }
            InitNode(newNode, _item, _it);
            RIGHT_CHILD(_it) = newNode;
        }
        else
        {
            return InsertItem(RIGHT_CHILD(_it), _end, _item, _less);
        }
    }
    /* item < node's data, go left */
    else
    {
        if (LEFT_CHILD(_it) == NULL)
        {
            newNode = malloc(sizeof(Node));
            if (newNode == NULL)
            {
                return _end;
            }
            InitNode(newNode, _item, _it);
            LEFT_CHILD(_it) = newNode;
        }
        else
        {
            return InsertItem(LEFT_CHILD(_it), _end, _item, _less);
        }
    }
    return newNode;
}

static void DestroyTree(BSTreeItr _head, void (*_destroyer)(void *))
{
    if (_head == NULL)
    {
        return;
    }
    DestroyTree(LEFT_CHILD(_head), _destroyer);
    DestroyTree(RIGHT_CHILD(_head), _destroyer);
    if (_destroyer != NULL)
    {
        _destroyer(DATA(_head));
    }
    free(_head);
    _head = NULL;
}

static void InitTree(BSTree *_tree, LessComparator _less)
{
    _tree->m_root.m_father = END_OF_TREE(_tree);
    _tree->m_root.m_right = NULL;
    _tree->m_root.m_left = NULL;
    _tree->m_root.m_data = NULL;
    _tree->m_less = _less;
}

static BSTreeItr LeftMostChild(BSTreeItr _it)
{
    Node *curr = (Node *)_it;
    if (curr == NULL)
    {
        return NULL;
    }
    while (curr->m_left != NULL)
    {
        curr = curr->m_left;
    }
    return curr;
}

static BSTreeItr RightMostChild(BSTreeItr _it)
{
    Node *curr = (Node *)_it;
    if (curr == NULL)
    {
        return NULL;
    }
    while (curr->m_right != NULL)
    {
        curr = curr->m_right;
    }
    return curr;
}

static void SwapData(BSTreeItr _it, BSTreeItr _next)
{
    void *temp = DATA(_it);
    DATA(_it) = DATA(_next);
    DATA(_next) = temp;
}

static BSTreeItr InsertRoot(BSTree *_tree, void *_item)
{
    Node *newNode;
    newNode = malloc(sizeof(Node));
    if (newNode == NULL)
    {
        return END_OF_TREE(_tree);
    }
    InitNode(newNode, _item, END_OF_TREE(_tree));
    LEFT_CHILD(END_OF_TREE(_tree)) = newNode;
    return newNode;
}

/********************** Tree Getters **********************/

LessComparator GetLess(BSTree * _tree){
    return _tree->m_less;
}

Node *GetTreeRoot(BSTree *_tree)
{
    return &_tree->m_root;
}
void *GetDataBSTree(Node *_node)
{
    return _node->m_data;
}
Node *GetLeftChild(Node *_node)
{
    return _node->m_left;
}
Node *GetRightChild(Node *_node)
{
    return _node->m_right;
}
Node *GetFather(Node *_node)
{
    return _node->m_father;
}

static int IsFullTreeRec(Node *_curr)
{
    if (_curr == NULL)
    {
        return 1;
    }
    if (HAS_ONLY_ONE_CHILD(_curr))
    {
        return 0;
    }
    return IsFullTreeRec(_curr->m_left) & IsFullTreeRec(_curr->m_right);
}

int ISFullTree(BSTree *_tree)
{
    if (_tree == NULL)
    {
        return 1;
    }
    return IsFullTreeRec(TRUE_TREE_ROOT(_tree));
}

static int AreSimilarTreesRec(Node *_curr1, Node *_curr2)
{
    if (_curr1 == NULL && _curr2 == NULL)
    {
        return 1;
    }
    if (ITSELF_DIFF(_curr1, _curr2) || ZERO_CHILD_DIFF(_curr1, _curr2) || ONE_CHILD_DIFF(_curr1, _curr2) || ONE_CHILD_POS_DIFF(_curr1, _curr2) || TWO_CHILD_DIFF(_curr1, _curr2))
    {
        return 0;
    }
    return AreSimilarTreesRec(_curr1->m_left, _curr2->m_left) && AreSimilarTreesRec(_curr1->m_right, _curr2->m_right);
}

int AreSimilarTrees(BSTree *_tree1, BSTree *_tree2)
{
    if (_tree1 == NULL || _tree2 == NULL)
    {
        return 1;
    }
    return AreSimilarTreesRec(TRUE_TREE_ROOT(_tree1), TRUE_TREE_ROOT(_tree2));
}

static int CalcTreeLevelRec(Node *_curr)
{
    if (_curr == NULL)
    {
        return -1;
    }
    return fmax(CalcTreeLevelRec(LEFT_CHILD(_curr)), CalcTreeLevelRec(RIGHT_CHILD(_curr))) + 1;
}

int CalcTreeLevel(BSTree *_tree)
{
    if (_tree == NULL)
    {
        return -2;
    }

    return CalcTreeLevelRec(TRUE_TREE_ROOT(_tree));
}

static void SwapChilds(Node *_curr)
{
    Node *temp = LEFT_CHILD(_curr);
    LEFT_CHILD(_curr) = RIGHT_CHILD(_curr);
    RIGHT_CHILD(_curr) = temp;
}

static void MirrorTreeRec(Node *_curr)
{
    if (_curr == NULL)
    {
        return;
    }
    MirrorTreeRec(LEFT_CHILD(_curr));
    MirrorTreeRec(RIGHT_CHILD(_curr));
    SwapChilds(_curr);
}

void MirrorTree(BSTree *_tree)
{
    if (_tree == NULL)
    {
        return;
    }
    MirrorTreeRec(TRUE_TREE_ROOT(_tree));
}

int IsPerfectTreeRec(Node *_curr)
{
    if (_curr == NULL)
    {
        return 1;
    }
    if (CalcTreeLevelRec(LEFT_CHILD(_curr)) != CalcTreeLevelRec(RIGHT_CHILD(_curr)))
    {
        return 0;
    }
    return IsPerfectTreeRec(LEFT_CHILD(_curr)) & IsPerfectTreeRec(RIGHT_CHILD(_curr));
}

int IsPerfectTree(BSTree *_tree)
{
    if (_tree == NULL)
    {
        return 1;
    }
    return IsPerfectTreeRec(TRUE_TREE_ROOT(_tree));
}

int IsBSTRec(Node* _curr, LessComparator _less){
    if(_curr == NULL){
        return 1;
    }
    if(_less(_curr->m_right, _curr) > 0 || _less(_curr, _curr->m_left)){
        return 0;
    }
    return IsBSTRec(_curr->m_left,_less) & IsBSTRec(_curr->m_right,_less);
}

int IsBST(BSTree *_tree){
    if(_tree == NULL){
        return 1;
    }
    return IsBSTRec(TRUE_TREE_ROOT(_tree), _tree->m_less);
}