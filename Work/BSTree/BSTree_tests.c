#include "BSTree.h"
#include <stdlib.h> /* Dynamic Memory */
#include <stdio.h>  /* printf */

typedef struct Node Node;

Node *GetTreeRoot(BSTree *_tree);
void *GetData(Node *_node);
Node *GetLeftChild(Node *_node);
Node *GetRightChild(Node *_node);
Node *GetFather(Node *_node);
static int IntComparator(void *_left, void *_right);
static int *CreateInt(int _value);


/* BSTreeCreate Tests */
static void TestBSTreeCreateValid();
static void TestBSTreeCreateInvalidLessFunc();

/* BSTreeDestroy Tests */
static void TestBSTreeDestroyValid();
static void TestBSTreeDestroyNullTree();
static void TestBSTreeDestroyNullElementDestroyFunc();
static void TestBSTreeDestroyDoubleDestroy();

/* BSTreeInsert Tests */
static void TestBSTreeInsertShouldBePlacedAsLeftChild();
static void TestBSTreeInsertShouldBePlacedAsRightChild();
static void TestBSTreeInsertNullTree();
static void TestBSTreeInsertNullItem();
static void TestBSTreeInsertDuplicateItem();
static void TestBSTreeInsertEmptyTree();

/* BSTreeItrBegin Tests */
static void TestBSTreeItrBeginValid();
static void TestBSTreeItrBeginNullItr();
static void TestBSTreeItrBeginNullTree();
static void TestBSTreeItrBeginEmptyTree();

/* BSTreeItrEnd Tests */
static void TestBSTreeItrEndValid();
static void TestBSTreeItrEndNullItr();
static void TestBSTreeItrEndNullTree();
static void TestBSTreeItrEndEmptyTree();

/* BSTreeItrEquals Tests */
static void TestBSTreeItrEqualsValid();
static void TestBSTreeItrEqualsNullInput();

/* BSTreeItrNext Tests */
static void TestBSTreeItrNextSuccessorIsRightChild();
static void TestBSTreeItrNextSuccessorIsRightChildLeftMost();
static void TestBSTreeItrNextSuccessorThroughFather();
static void TestBSTreeItrNextNoSuccessor();
static void TestBSTreeItrNextEmptyTree();
static void TestBSTreeItrNextNullInput();

/* BSTreeItrPrev Tests */
static void TestBSTreeItrPrevPredecessorIsLeftChild();
static void TestBSTreeItrPrevPredecessorIsLeftChildRightMost();
static void TestBSTreeItrPrevPredecessorThroughFather();
static void TestBSTreeItrPrevNoPredecessor();
static void TestBSTreeItrPrevEmptyTree();
static void TestBSTreeItrPrevNullInput();

/* BSTreeItrRemove Tests */
static void TestBSTreeItrRemoveHasZeroChilds();
static void TestBSTreeItrRemoveHasOneChild();
static void TestBSTreeItrRemoveHasTwoChilds();
static void TestBSTreeItrRemoveNullInput();
static void TestBSTreeItrRemoveTrueTreeRoot();
static void TestBSTreeItrRemoveRootSentinel();

/* BSTreeItrGet Tests */
static void TestBSTreeItrGetValid();
static void TestBSTreeItrGetNullInput();
static void TestBSTreeItrGetEnd();

int main()
{
    TestBSTreeCreateValid();
    TestBSTreeCreateInvalidLessFunc();
    TestBSTreeDestroyValid();
    TestBSTreeDestroyNullTree();
    TestBSTreeDestroyNullElementDestroyFunc();
    TestBSTreeDestroyDoubleDestroy();
    TestBSTreeInsertShouldBePlacedAsLeftChild();
    TestBSTreeInsertShouldBePlacedAsRightChild();
    TestBSTreeInsertNullTree();
    TestBSTreeInsertNullItem();
    TestBSTreeInsertDuplicateItem();
    TestBSTreeInsertEmptyTree();
    TestBSTreeItrBeginValid();
    TestBSTreeItrBeginNullItr();
    TestBSTreeItrBeginNullTree();
    TestBSTreeItrBeginEmptyTree();
    TestBSTreeItrEndValid();
    TestBSTreeItrEndNullItr();
    TestBSTreeItrEndNullTree();
    TestBSTreeItrEndEmptyTree();
    TestBSTreeItrEqualsValid();
    TestBSTreeItrEqualsNullInput();
    TestBSTreeItrNextSuccessorIsRightChild();
    TestBSTreeItrNextSuccessorIsRightChildLeftMost();
    TestBSTreeItrNextSuccessorThroughFather();
    TestBSTreeItrNextNoSuccessor();
    TestBSTreeItrNextEmptyTree();
    TestBSTreeItrNextNullInput();
    TestBSTreeItrPrevPredecessorIsLeftChild();
    TestBSTreeItrPrevPredecessorIsLeftChildRightMost();
    TestBSTreeItrPrevPredecessorThroughFather();
    TestBSTreeItrPrevNoPredecessor();
    TestBSTreeItrPrevEmptyTree();
    TestBSTreeItrPrevNullInput();
    TestBSTreeItrRemoveHasZeroChilds();
    TestBSTreeItrRemoveHasOneChild();
    TestBSTreeItrRemoveHasTwoChilds();
    TestBSTreeItrRemoveNullInput();
    TestBSTreeItrRemoveTrueTreeRoot();
    TestBSTreeItrRemoveRootSentinel();
    TestBSTreeItrGetValid();
    TestBSTreeItrGetNullInput();
    TestBSTreeItrGetEnd();

    return 0;
}

/* BSTreeCreate Tests */
static void TestBSTreeCreateValid()
{
    printf("TestBSTreeCreateValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    if (tree != NULL && GetTreeRoot(tree) != NULL && GetLeftChild(GetTreeRoot(tree)) == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, NULL);
}

static void TestBSTreeCreateInvalidLessFunc()
{
    printf("TestBSTreeCreateInvalidLessFunc: ");
    BSTree *tree = BSTreeCreate(NULL);
    if (tree == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
        BSTreeDestroy(&tree, NULL);
    }
}

/* BSTreeDestroy Tests */
static void TestBSTreeDestroyValid()
{
    printf("TestBSTreeDestroyValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val = CreateInt(10);
    BSTreeInsert(tree, val);

    BSTreeDestroy(&tree, free);
    if (tree == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeDestroyNullTree()
{
    printf("TestBSTreeDestroyNullTree: ");
    BSTreeDestroy(NULL, free);
    printf("PASS\n");
}

static void TestBSTreeDestroyNullElementDestroyFunc()
{
    printf("TestBSTreeDestroyNullElementDestroyFunc: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val = CreateInt(20);
    BSTreeInsert(tree, val);
    BSTreeDestroy(&tree, NULL);
    if (tree == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    free(val);
}

static void TestBSTreeDestroyDoubleDestroy()
{
    printf("TestBSTreeDestroyDoubleDestroy: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val = CreateInt(30);
    BSTreeInsert(tree, val);
    BSTreeDestroy(&tree, free);
    BSTreeDestroy(&tree, free);
    printf("PASS\n");
}

/* BSTreeInsert Tests */
static void TestBSTreeInsertShouldBePlacedAsLeftChild()
{
    printf("TestBSTreeInsertShouldBePlacedAsLeftChild: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(30);
    BSTreeInsert(tree, val1);
    Node *treeNode = BSTreeInsert(tree, val2);

    if (GetLeftChild(treeNode) == NULL && GetRightChild(treeNode) == NULL && GetData(treeNode) == val2 && GetData(GetFather(treeNode)) == val1 && GetLeftChild(GetFather(treeNode)) == treeNode)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeInsertShouldBePlacedAsRightChild()
{
    printf("TestBSTreeInsertShouldBePlacedAsRightChild: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(40);
    int *val2 = CreateInt(60);
    BSTreeInsert(tree, val1);
    Node *treeNode = BSTreeInsert(tree, val2);
    if (GetLeftChild(treeNode) == NULL && GetRightChild(treeNode) == NULL && GetData(treeNode) == val2 && GetData(GetFather(treeNode)) == val1 && GetRightChild(GetFather(treeNode)) == treeNode)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeInsertNullTree()
{
    printf("TestBSTreeInsertNullTree: ");
    int value = 100;
    BSTreeItr itr = BSTreeInsert(NULL, &value);
    if (itr == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeInsertNullItem()
{
    printf("TestBSTreeInsertNullItem: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeInsert(tree, NULL);
    if (itr == BSTreeItrEnd(tree))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeInsertDuplicateItem()
{
    printf("TestBSTreeInsertDuplicateItem: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(70);
    int *val2 = CreateInt(70);
    BSTreeInsert(tree, val1);
    BSTreeItr itr = BSTreeInsert(tree, val2);
    if (itr == BSTreeItrEnd(tree))
    {
        free(val2);
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeInsertEmptyTree()
{
    printf("TestBSTreeInsertEmptyTree: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val = CreateInt(80);
    BSTreeItr itr = BSTreeInsert(tree, val);
    if (itr != BSTreeItrEnd(tree) && BSTreeItrGet(itr) == val)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

/* BSTreeItrBegin Tests */
static void TestBSTreeItrBeginValid()
{
    printf("TestBSTreeItrBeginValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(25);
    int *val2 = CreateInt(15);
    int *val3 = CreateInt(35);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeItr itr = BSTreeItrBegin(tree);
    if (itr != NULL && *((int *)BSTreeItrGet(itr)) == 15)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrBeginNullItr()
{
    printf("TestBSTreeItrBeginNullItr: ");
    BSTreeItr itr = BSTreeItrBegin(NULL);
    if (itr == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrBeginNullTree()
{
    printf("TestBSTreeItrBeginNullTree: ");
    BSTreeItr itr = BSTreeItrBegin(NULL);
    if (itr == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrBeginEmptyTree()
{
    printf("TestBSTreeItrBeginEmptyTree: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeItrBegin(tree);
    if (itr == BSTreeItrEnd(tree))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

/* BSTreeItrEnd Tests */
static void TestBSTreeItrEndValid()
{
    printf("TestBSTreeItrEndValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeItrEnd(tree);
    if (itr != NULL && itr == GetTreeRoot(tree))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrEndNullItr()
{
    printf("TestBSTreeItrEndNullItr: ");
    BSTreeItr itr = BSTreeItrEnd(NULL);
    if (itr == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrEndNullTree()
{
    printf("TestBSTreeItrEndNullTree: ");
    BSTreeItr itr = BSTreeItrEnd(NULL);
    if (itr == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrEndEmptyTree()
{
    printf("TestBSTreeItrEndEmptyTree: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeItrEnd(tree);
    if (itr == GetTreeRoot(tree))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

/* BSTreeItrEquals Tests */
static void TestBSTreeItrEqualsValid()
{
    printf("TestBSTreeItrEqualsValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(45);
    BSTreeInsert(tree, val1);
    BSTreeItr itr1 = BSTreeItrBegin(tree);
    BSTreeItr itr2 = BSTreeItrBegin(tree);
    if (BSTreeItrEquals(itr1, itr2))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrEqualsNullInput()
{
    printf("TestBSTreeItrEqualsNullInput: ");
    if (BSTreeItrEquals(NULL, NULL))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* BSTreeItrNext Tests */
static void TestBSTreeItrNextSuccessorIsRightChild()
{
    printf("TestBSTreeItrNextSuccessorIsRightChild: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(30);
    int *val3 = CreateInt(70);
    BSTreeItr itr = BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeItr next = BSTreeItrNext(itr);
    if (next != BSTreeItrEnd(tree) && *(int *)BSTreeItrGet(next) == 70)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrNextSuccessorIsRightChildLeftMost()
{
    printf("TestBSTreeItrNextSuccessorIsRightChildLeftMost: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(20);
    int *val2 = CreateInt(10);
    int *val3 = CreateInt(30);
    int *val4 = CreateInt(25);
    BSTreeItr itr = BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeInsert(tree, val4);
    BSTreeItr next = BSTreeItrNext(itr);
    if (next != BSTreeItrEnd(tree) && *(int *)BSTreeItrGet(next) == 25)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrNextSuccessorThroughFather()
{
    printf("TestBSTreeItrNextSuccessorThroughFather: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(40);
    int *val2 = CreateInt(20);
    int *val3 = CreateInt(60);
    int *val4 = CreateInt(10);
    int *val5 = CreateInt(30);
    int *val6 = CreateInt(15);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeInsert(tree, val4);
    BSTreeInsert(tree, val5);
    BSTreeItr itr = BSTreeInsert(tree, val6);
    BSTreeItr next = BSTreeItrNext(itr);
    if (next != BSTreeItrEnd(tree) && *(int *)BSTreeItrGet(next) == 20)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrNextNoSuccessor()
{
    printf("TestBSTreeItrNextNoSuccessor: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(100);
    BSTreeInsert(tree, val1);
    BSTreeItr itr = BSTreeItrBegin(tree);
    BSTreeItr next = BSTreeItrNext(itr);
    if (BSTreeItrEquals(next, BSTreeItrEnd(tree)))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrNextEmptyTree()
{
    printf("TestBSTreeItrNextEmptyTree: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeItrNext(BSTreeItrBegin(tree));
    if (BSTreeItrEquals(itr, BSTreeItrEnd(tree)))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrNextNullInput()
{
    printf("TestBSTreeItrNextNullInput: ");
    BSTreeItr next = BSTreeItrNext(NULL);
    if (next == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* BSTreeItrPrev Tests */
static void TestBSTreeItrPrevPredecessorIsLeftChild()
{
    printf("TestBSTreeItrPrevPredecessorIsLeftChild: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(30);
    int *val3 = CreateInt(70);
    BSTreeItr itr = BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeItr prev = BSTreeItrPrev(itr);
    if (prev != BSTreeItrEnd(tree) && *((int *)BSTreeItrGet(prev)) == 30)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrPrevPredecessorIsLeftChildRightMost()
{
    printf("TestBSTreeItrPrevPredecessorIsLeftChildRightMost: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(40);
    int *val2 = CreateInt(20);
    int *val3 = CreateInt(60);
    int *val4 = CreateInt(10);
    int *val5 = CreateInt(30);
    int *val6 = CreateInt(15);
    int *val7 = CreateInt(16);
    BSTreeInsert(tree, val1);
    BSTreeItr itr = BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeInsert(tree, val4);
    BSTreeInsert(tree, val5);
    BSTreeInsert(tree, val6);
    BSTreeInsert(tree, val7);
    BSTreeItr prev = BSTreeItrPrev(itr);
    if (prev != NULL && *((int *)BSTreeItrGet(prev)) == 16)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrPrevPredecessorThroughFather()
{
    printf("TestBSTreeItrPrevPredecessorThroughFather: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(30);
    int *val3 = CreateInt(70);
    int *val4 = CreateInt(20);
    int *val5 = CreateInt(40);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeInsert(tree, val4);
    BSTreeItr itr = BSTreeInsert(tree, val5);
    BSTreeItr prev = BSTreeItrPrev(itr);
    if (prev != NULL && *((int *)BSTreeItrGet(prev)) == 30)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrPrevNoPredecessor()
{
    printf("TestBSTreeItrPrevNoPredecessor: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(80);
    int *val2 = CreateInt(70);
    int *val3 = CreateInt(50);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeItr itr = BSTreeInsert(tree, val3);
    BSTreeItr prev = BSTreeItrPrev(itr);
    if (BSTreeItrEquals(itr, prev))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrPrevEmptyTree()
{
    printf("TestBSTreeItrPrevEmptyTree: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr itr = BSTreeItrPrev(BSTreeItrBegin(tree));
    if (BSTreeItrEquals(itr, BSTreeItrEnd(tree)))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrPrevNullInput()
{
    printf("TestBSTreeItrPrevNullInput: ");
    BSTreeItr prev = BSTreeItrPrev(NULL);
    if (prev == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

/* BSTreeItrRemove Tests */
static void TestBSTreeItrRemoveHasZeroChilds()
{
    printf("TestBSTreeItrRemoveHasZeroChilds: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(100);
    BSTreeInsert(tree, val1);
    BSTreeItr itr = BSTreeItrBegin(tree);
    int *removed = BSTreeItrRemove(itr);
    if (removed == val1 && BSTreeItrBegin(tree) == BSTreeItrEnd(tree))
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, NULL);
}

static void TestBSTreeItrRemoveHasOneChild()
{
    printf("TestBSTreeItrRemoveHasOneChild: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(50);
    int *val2 = CreateInt(30);
    int *val3 = CreateInt(70);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeItr itr = BSTreeItrBegin(tree);
    int *removed = BSTreeItrRemove(itr);
    BSTreeItr newItr = BSTreeItrBegin(tree);
    if (removed == val2 && *(int *)BSTreeItrGet(newItr) == 50)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrRemoveHasTwoChilds()
{
    printf("TestBSTreeItrRemoveHasTwoChilds: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(40);
    int *val2 = CreateInt(20);
    int *val3 = CreateInt(60);
    int *val4 = CreateInt(10);
    int *val5 = CreateInt(30);
    int *val6 = CreateInt(50);
    int *val7 = CreateInt(70);
    BSTreeInsert(tree, val1);
    BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeInsert(tree, val4);
    BSTreeInsert(tree, val5);
    BSTreeInsert(tree, val6);
    BSTreeInsert(tree, val7);
    BSTreeItr itr = BSTreeItrBegin(tree);
    BSTreeItr toRemove = BSTreeItrNext(itr);
    int *removed = BSTreeItrRemove(toRemove);
    BSTreeItr newItr = BSTreeItrBegin(tree);
    BSTreeItr nextItr = BSTreeItrNext(newItr);
    if (*removed == *val2 && *(int *)BSTreeItrGet(nextItr) == 30)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrRemoveNullInput()
{
    printf("TestBSTreeItrRemoveNullInput: ");
    void *removed = BSTreeItrRemove(NULL);
    if (removed == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrRemoveTrueTreeRoot()
{
    printf("TestBSTreeItrRemoveTrueTreeRoot: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val1 = CreateInt(25);
    int *val2 = CreateInt(15);
    int *val3 = CreateInt(35);
    BSTreeInsert(tree, val1);
    BSTreeItr itr = BSTreeInsert(tree, val2);
    BSTreeInsert(tree, val3);
    BSTreeItr toRemove = BSTreeItrNext(itr);
    int *removed = BSTreeItrRemove(toRemove);
    BSTreeItr newRootItr = BSTreeItrBegin(tree);
    if (removed == val1 && *(int *)BSTreeItrGet(newRootItr) == 15)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrRemoveRootSentinel()
{
    printf("TestBSTreeItrRemoveRootSentinel: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *removed = BSTreeItrRemove(GetTreeRoot(tree));
    if (removed == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

/* BSTreeItrGet Tests */
static void TestBSTreeItrGetValid()
{
    printf("TestBSTreeItrGetValid: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    int *val = CreateInt(55);
    BSTreeInsert(tree, val);
    BSTreeItr itr = BSTreeItrBegin(tree);
    int *data = BSTreeItrGet(itr);
    if (data == val)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

static void TestBSTreeItrGetNullInput()
{
    printf("TestBSTreeItrGetNullInput: ");
    void *data = BSTreeItrGet(NULL);
    if (data == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
}

static void TestBSTreeItrGetEnd()
{
    printf("TestBSTreeItrGetEnd: ");
    BSTree *tree = BSTreeCreate(IntComparator);
    BSTreeItr endItr = BSTreeItrEnd(tree);
    void *data = BSTreeItrGet(endItr);
    if (data == NULL)
    {
        printf("PASS\n");
    }
    else
    {
        printf("FAIL\n");
    }
    BSTreeDestroy(&tree, free);
}

/* Help Functions */
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
