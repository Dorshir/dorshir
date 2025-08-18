#include "BSTree.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct Node Node;
static int IntComparator(void *_left, void *_right);
static int *CreateInt(int _value);

int ISFullTree(BSTree *_tree);
int AreSimilarTrees(BSTree *_tree1, BSTree *_tree2);
int CalcTreeLevel(BSTree *_tree);
void MirrorTree(BSTree *_tree);
int IsPerfectTree(BSTree *_tree);

int main()
{
    int *val1 = CreateInt(20);
    int *val2 = CreateInt(10);
    int *val3 = CreateInt(30);
    int *val4 = CreateInt(45);
    int *val5 = CreateInt(25);
    int *val6 = CreateInt(8);
    int *val7 = CreateInt(12);
    BSTree *tree1 = BSTreeCreate(IntComparator);
    BSTreeInsert(tree1, val1);
    BSTreeInsert(tree1, val2);
    BSTreeInsert(tree1, val3);

    BSTree *tree2 = BSTreeCreate(IntComparator);
    BSTreeInsert(tree2, val1);
    BSTreeInsert(tree2, val2);
    BSTreeInsert(tree2, val3);
    BSTreeInsert(tree2, val4);
    BSTreeInsert(tree2, val5);
    BSTreeInsert(tree2, val6);
    BSTreeInsert(tree2, val7);


    printf("Is full tree: %s\n",ISFullTree(tree1) == 1 ? "TRUE" : "FALSE");
    printf("Are similar trees: %s\n",AreSimilarTrees(tree1, tree2) == 1 ? "TRUE" : "FALSE");
    printf("Level of tree: %d\n", CalcTreeLevel(tree2));
    printf("Is perfect tree: %s\n", IsPerfectTree(tree2) == 1 ? "TRUE" : "FALSE");

    return 0;
}

static int IntComparator(void *_left, void *_right)
{
    if (_left == NULL || _right == NULL)
    {
        return 0;
    }
    if (*(int *)_left < *(int *)_right)
    {
        return 1;
    }
    else if (*(int *)_left > *(int *)_right)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

static int *CreateInt(int _value)
{
    int *ptr = malloc(sizeof(int));
    if (ptr == NULL)
    {
        return NULL;
    }
    *ptr = _value;
    return ptr;
}
