#include "new_gen_dlist.h"
#include <stdio.h> /* printf */

/* Helper functions */
static int SumAction(void *element, void *context);

/* ListCreate Tests */
static void TestListCreateValid();

/* ListDestroy Tests */
static void TestListDestroyValid();
static void TestListDestroyNullList();
static void TestListDestroyDoubleDestroy();

/* ListPushHead Tests */
static void TestListPushHeadValid();
static void TestListPushHeadNullList();
static void TestListPushHeadNullItem();

/* ListPushTail Tests */
static void TestListPushTailValid();
static void TestListPushTailNullList();
static void TestListPushTailNullItem();

/* ListPopHead Tests */
static void TestListPopHeadValid();
static void TestListPopHeadEmptyList();
static void TestListPopHeadNullList();

/* ListPopTail Tests */
static void TestListPopTailValid();
static void TestListPopTailEmptyList();
static void TestListPopTailNullList();

/* ListSize Tests */
static void TestListSizeEmptyList();
static void TestListSizeNonEmptyList();
static void TestListSizeNullList();

/* ListIsEmpty Tests */
static void TestListIsEmptyEmptyList();
static void TestListIsEmptyNonEmptyList();
static void TestListIsEmptyNullList();

/* ListItrBegin Tests */
static void TestListItrBeginValid();
static void TestListItrBeginNullList();

/* ListItrEnd Tests */
static void TestListItrEndValid();
static void TestListItrEndNullList();

/* ListItrNext Tests */
static void TestListItrNextValid();
static void TestListItrNextEnd();

/* ListItrPrev Tests */
static void TestListItrPrevValid();
static void TestListItrPrevBegin();

/* ListItrGet Tests */
static void TestListItrGetValid();
static void TestListItrGetEnd();

/* ListItrSet Tests */
static void TestListItrSetValid();
static void TestListItrSetNullElement();
static void TestListItrSetEnd();

/* ListItrInsertBefore Tests */
static void TestListItrInsertBeforeValid();
static void TestListItrInsertBeforeNullElement();
static void TestListItrInsertBeforeNullItr();

/* ListItrRemove Tests */
static void TestListItrRemoveValid();
static void TestListItrRemoveEnd();

/* ListItrEquals Tests */
static void TestListItrEqualsSameItr();
static void TestListItrEqualsDifferentItr();
static void TestListItrEqualsNullItr();

/* ListItrForEach Tests */
static void TestListItrForEachValid();
static void TestListItrForEachNullAction();
static void TestListItrForEachNullBeginOrEnd();

int main()
{
    /* ListCreate Tests */
    TestListCreateValid();

    /* ListDestroy Tests */
    TestListDestroyValid();
    TestListDestroyNullList();
    TestListDestroyDoubleDestroy();

    /* ListPushHead Tests */
    TestListPushHeadValid();
    TestListPushHeadNullList();
    TestListPushHeadNullItem();

    /* ListPushTail Tests */
    TestListPushTailValid();
    TestListPushTailNullList();
    TestListPushTailNullItem();

    /* ListPopHead Tests */
    TestListPopHeadValid();
    TestListPopHeadEmptyList();
    TestListPopHeadNullList();

    /* ListPopTail Tests */
    TestListPopTailValid();
    TestListPopTailEmptyList();
    TestListPopTailNullList();

    /* ListSize Tests */
    TestListSizeEmptyList();
    TestListSizeNonEmptyList();
    TestListSizeNullList();

    /* ListIsEmpty Tests */
    TestListIsEmptyEmptyList();
    TestListIsEmptyNonEmptyList();
    TestListIsEmptyNullList();

    /* ListItrBegin Tests */
    TestListItrBeginValid();
    TestListItrBeginNullList();

    /* ListItrEnd Tests */
    TestListItrEndValid();
    TestListItrEndNullList();

    /* ListItrNext Tests */
    TestListItrNextValid();
    TestListItrNextEnd();

    /* ListItrPrev Tests */
    TestListItrPrevValid();
    TestListItrPrevBegin();

    /* ListItrGet Tests */
    TestListItrGetValid();
    TestListItrGetEnd();

    /* ListItrSet Tests */
    TestListItrSetValid();
    TestListItrSetNullElement();
    TestListItrSetEnd();

    /* ListItrInsertBefore Tests */
    TestListItrInsertBeforeValid();
    TestListItrInsertBeforeNullElement();
    TestListItrInsertBeforeNullItr();

    /* ListItrRemove Tests */
    TestListItrRemoveValid();
    TestListItrRemoveEnd();

    /* ListItrEquals Tests */
    TestListItrEqualsSameItr();
    TestListItrEqualsDifferentItr();
    TestListItrEqualsNullItr();

    /* ListItrForEach Tests */
    TestListItrForEachValid();
    TestListItrForEachNullAction();
    TestListItrForEachNullBeginOrEnd();

    return 0;
}

/* Helper function */
static int SumAction(void *element, void *context)
{
    int *sum = (int *)context;
    int *value = (int *)element;
    *sum += *value;
    return 1;
}

/* ListCreate Tests */
static void TestListCreateValid()
{
    List *list = ListCreate();
    if (list != NULL)
    {
        printf("TestListCreateValid: PASS\n");
        ListDestroy(&list, NULL);
    }
    else
    {
        printf("TestListCreateValid: FAIL\n");
    }
}

/* ListDestroy Tests */
static void TestListDestroyValid()
{
    List *list = ListCreate();
    ListDestroy(&list, NULL);
    if (list == NULL)
    {
        printf("TestListDestroyValid: PASS\n");
    }
    else
    {
        printf("TestListDestroyValid: FAIL\n");
    }
}

static void TestListDestroyNullList()
{
    List *list = NULL;
    ListDestroy(&list, NULL);
    if (list == NULL)
    {
        printf("TestListDestroyNullList: PASS\n");
    }
    else
    {
        printf("TestListDestroyNullList: FAIL\n");
    }
}

static void TestListDestroyDoubleDestroy()
{
    List *list = ListCreate();
    ListDestroy(&list, NULL);
    ListDestroy(&list, NULL);
    if (list == NULL)
    {
        printf("TestListDestroyDoubleDestroy: PASS\n");
    }
    else
    {
        printf("TestListDestroyDoubleDestroy: FAIL\n");
    }
}

/* ListPushHead Tests */
static void TestListPushHeadValid()
{
    int item = 0;
    List *list = ListCreate();
    ListItr itr = ListPushHead(list, &item);
    if (itr != NULL && ListItrGet(itr) == &item)
    {
        printf("TestListPushHeadValid: PASS\n");
    }
    else
    {
        printf("TestListPushHeadValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPushHeadNullList()
{
    int item = 30;
    ListItr itr = ListPushHead(NULL, &item);
    if (itr == NULL)
    {
        printf("TestListPushHeadNullList: PASS\n");
    }
    else
    {
        printf("TestListPushHeadNullList: FAIL\n");
    }
}

static void TestListPushHeadNullItem()
{
    List *list = ListCreate();
    ListItr itr = ListPushHead(list, NULL);
    if (itr == NULL)
    {
        printf("TestListPushHeadNullItem: PASS\n");
    }
    else
    {
        printf("TestListPushHeadNullItem: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListPushTail Tests */
static void TestListPushTailValid()
{
    int item = 42;
    List *list = ListCreate();
    ListItr itr = ListPushTail(list, &item);
    if (itr != NULL && ListItrGet(itr) == &item)
    {
        printf("TestListPushTailValid: PASS\n");
    }
    else
    {
        printf("TestListPushTailValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPushTailNullList()
{
    int item = 42;
    ListItr itr = ListPushTail(NULL, &item);
    if (itr == NULL)
    {
        printf("TestListPushTailNullList: PASS\n");
    }
    else
    {
        printf("TestListPushTailNullList: FAIL\n");
    }
}

static void TestListPushTailNullItem()
{
    List *list = ListCreate();
    ListItr itr = ListPushTail(list, NULL);
    if (itr == NULL)
    {
        printf("TestListPushTailNullItem: PASS\n");
    }
    else
    {
        printf("TestListPushTailNullItem: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListPopHead Tests */
static void TestListPopHeadValid()
{
    int item = 42;
    List *list = ListCreate();
    ListPushHead(list, &item);
    void *popped = ListPopHead(list);
    if (popped == &item)
    {
        printf("TestListPopHeadValid: PASS\n");
    }
    else
    {
        printf("TestListPopHeadValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPopHeadEmptyList()
{
    List *list = ListCreate();
    void *popped = ListPopHead(list);
    if (popped == NULL)
    {
        printf("TestListPopHeadEmptyList: PASS\n");
    }
    else
    {
        printf("TestListPopHeadEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPopHeadNullList()
{
    void *popped = ListPopHead(NULL);
    if (popped == NULL)
    {
        printf("TestListPopHeadNullList: PASS\n");
    }
    else
    {
        printf("TestListPopHeadNullList: FAIL\n");
    }
}

/* ListPopTail Tests */
static void TestListPopTailValid()
{
    int item = 42;
    List *list = ListCreate();
    ListPushTail(list, &item);
    void *popped = ListPopTail(list);
    if (popped == &item)
    {
        printf("TestListPopTailValid: PASS\n");
    }
    else
    {
        printf("TestListPopTailValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPopTailEmptyList()
{
    List *list = ListCreate();
    void *popped = ListPopTail(list);
    if (popped == NULL)
    {
        printf("TestListPopTailEmptyList: PASS\n");
    }
    else
    {
        printf("TestListPopTailEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListPopTailNullList()
{
    void *popped = ListPopTail(NULL);
    if (popped == NULL)
    {
        printf("TestListPopTailNullList: PASS\n");
    }
    else
    {
        printf("TestListPopTailNullList: FAIL\n");
    }
}

/* ListSize Tests */
static void TestListSizeEmptyList()
{
    List *list = ListCreate();
    size_t size = ListSize(list);
    if (size == 0)
    {
        printf("TestListSizeEmptyList: PASS\n");
    }
    else
    {
        printf("TestListSizeEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListSizeNonEmptyList()
{
    List *list = ListCreate();
    int items[] = {1, 2, 3};
    ListPushHead(list, &items[0]);
    ListPushHead(list, &items[1]);
    ListPushHead(list, &items[2]);
    size_t size = ListSize(list);
    if (size == 3)
    {
        printf("TestListSizeNonEmptyList: PASS\n");
    }
    else
    {
        printf("TestListSizeNonEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListSizeNullList()
{
    size_t size = ListSize(NULL);
    if (size == 0)
    {
        printf("TestListSizeNullList: PASS\n");
    }
    else
    {
        printf("TestListSizeNullList: FAIL\n");
    }
}

/* ListIsEmpty Tests */
static void TestListIsEmptyEmptyList()
{
    List *list = ListCreate();
    size_t result = ListIsEmpty(list);
    if (result != 0)
    {
        printf("TestListIsEmptyEmptyList: PASS\n");
    }
    else
    {
        printf("TestListIsEmptyEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListIsEmptyNonEmptyList()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    size_t result = ListIsEmpty(list);
    if (result == 0)
    {
        printf("TestListIsEmptyNonEmptyList: PASS\n");
    }
    else
    {
        printf("TestListIsEmptyNonEmptyList: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListIsEmptyNullList()
{
    size_t result = ListIsEmpty(NULL);
    if (result != 0)
    {
        printf("TestListIsEmptyNullList: PASS\n");
    }
    else
    {
        printf("TestListIsEmptyNullList: FAIL\n");
    }
}

/* ListItrBegin Tests */
static void TestListItrBeginValid()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrBegin(list);
    if (itr != NULL && ListItrGet(itr) == &item)
    {
        printf("TestListItrBeginValid: PASS\n");
    }
    else
    {
        printf("TestListItrBeginValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrBeginNullList()
{
    ListItr itr = ListItrBegin(NULL);
    if (itr == NULL)
    {
        printf("TestListItrBeginNullList: PASS\n");
    }
    else
    {
        printf("TestListItrBeginNullList: FAIL\n");
    }
}

/* ListItrEnd Tests */
static void TestListItrEndValid()
{
    List *list = ListCreate();
    ListItr itr = ListItrEnd(list);
    if (itr != NULL)
    {
        printf("TestListItrEndValid: PASS\n");
    }
    else
    {
        printf("TestListItrEndValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrEndNullList()
{
    ListItr itr = ListItrEnd(NULL);
    if (itr == NULL)
    {
        printf("TestListItrEndNullList: PASS\n");
    }
    else
    {
        printf("TestListItrEndNullList: FAIL\n");
    }
}

/* ListItrNext Tests */
static void TestListItrNextValid()
{
    List *list = ListCreate();
    int items[] = {1, 2};
    ListPushTail(list, &items[0]);
    ListPushTail(list, &items[1]);

    ListItr itr = ListItrBegin(list);
    itr = ListItrNext(itr);
    if (ListItrGet(itr) == &items[1])
    {
        printf("TestListItrNextValid: PASS\n");
    }
    else
    {
        printf("TestListItrNextValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrNextEnd()
{
    List *list = ListCreate();
    ListItr itr = ListItrEnd(list);
    ListItr nextItr = ListItrNext(itr);
    if (nextItr == itr)
    {
        printf("TestListItrNextEnd: PASS\n");
    }
    else
    {
        printf("TestListItrNextEnd: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListItrPrev Tests */
static void TestListItrPrevValid()
{
    List *list = ListCreate();
    int items[] = {1, 2};
    ListPushTail(list, &items[0]);
    ListPushTail(list, &items[1]);

    ListItr itr = ListItrEnd(list);
    itr = ListItrPrev(itr);
    if (ListItrGet(itr) == &items[1])
    {
        printf("TestListItrPrevValid: PASS\n");
    }
    else
    {
        printf("TestListItrPrevValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrPrevBegin()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);

    ListItr itr = ListItrBegin(list);
    ListItr prevItr = ListItrPrev(itr);
    if (prevItr == itr)
    {
        printf("TestListItrPrevBegin: PASS\n");
    }
    else
    {
        printf("TestListItrPrevBegin: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListItrGet Tests */
static void TestListItrGetValid()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrBegin(list);
    void *data = ListItrGet(itr);
    if (data == &item)
    {
        printf("TestListItrGetValid: PASS\n");
    }
    else
    {
        printf("TestListItrGetValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrGetEnd()
{
    List *list = ListCreate();
    ListItr itr = ListItrEnd(list);
    void *data = ListItrGet(itr);
    if (data == NULL)
    {
        printf("TestListItrGetEnd: PASS\n");
    }
    else
    {
        printf("TestListItrGetEnd: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListItrSet Tests */
static void TestListItrSetValid()
{
    List *list = ListCreate();
    int item1 = 42;
    int item2 = 24;
    ListPushHead(list, &item1);
    ListItr itr = ListItrBegin(list);
    void *oldData = ListItrSet(itr, &item2);
    if (oldData == &item1 && ListItrGet(itr) == &item2)
    {
        printf("TestListItrSetValid: PASS\n");
    }
    else
    {
        printf("TestListItrSetValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrSetNullElement()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrBegin(list);
    void *oldData = ListItrSet(itr, NULL);
    if (oldData == NULL)
    {
        printf("TestListItrSetNullElement: PASS\n");
    }
    else
    {
        printf("TestListItrSetNullElement: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrSetEnd()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrEnd(list);
    void *oldData = ListItrSet(itr, &item);
    if (oldData == NULL)
    {
        printf("TestListItrSetEnd: PASS\n");
    }
    else
    {
        printf("TestListItrSetEnd: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListItrInsertBefore Tests */
static void TestListItrInsertBeforeValid()
{
    List *list = ListCreate();
    int items[] = {1, 3};
    ListPushTail(list, &items[1]); // List: 3
    ListItr itr = ListItrBegin(list);
    int item2 = 2;
    ListItr newItr = ListItrInsertBefore(itr, &item2); // Insert 2 before 3
    if (newItr != NULL && ListItrGet(newItr) == &item2)
    {
        printf("TestListItrInsertBeforeValid: PASS\n");
    }
    else
    {
        printf("TestListItrInsertBeforeValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrInsertBeforeNullElement()
{
    List *list = ListCreate();
    int item = 1;
    ListPushTail(list, &item);
    ListItr itr = ListItrBegin(list);
    ListItr newItr = ListItrInsertBefore(itr, NULL);
    if (newItr == NULL)
    {
        printf("TestListItrInsertBeforeNullElement: PASS\n");
    }
    else
    {
        printf("TestListItrInsertBeforeNullElement: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrInsertBeforeNullItr()
{
    int item = 1;
    ListItr newItr = ListItrInsertBefore(NULL, &item);
    if (newItr == NULL)
    {
        printf("TestListItrInsertBeforeNullItr: PASS\n");
    }
    else
    {
        printf("TestListItrInsertBeforeNullItr: FAIL\n");
    }
}

/* ListItrRemove Tests */
static void TestListItrRemoveValid()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrBegin(list);
    void *removedData = ListItrRemove(itr);
    if (removedData == &item && ListSize(list) == 0)
    {
        printf("TestListItrRemoveValid: PASS\n");
    }
    else
    {
        printf("TestListItrRemoveValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrRemoveEnd()
{
    List *list = ListCreate();
    ListItr itr = ListItrEnd(list);
    void *removedData = ListItrRemove(itr);
    if (removedData == NULL)
    {
        printf("TestListItrRemoveEnd: PASS\n");
    }
    else
    {
        printf("TestListItrRemoveEnd: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

/* ListItrEquals Tests */
static void TestListItrEqualsSameItr()
{
    List *list = ListCreate();
    int item = 42;
    ListPushHead(list, &item);
    ListItr itr = ListItrBegin(list);
    int result = ListItrEquals(itr, itr);
    if (result)
    {
        printf("TestListItrEqualsSameItr: PASS\n");
    }
    else
    {
        printf("TestListItrEqualsSameItr: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrEqualsDifferentItr()
{
    List *list = ListCreate();
    int items[] = {1, 2};
    ListPushTail(list, &items[0]);
    ListPushTail(list, &items[1]);
    ListItr itr1 = ListItrBegin(list);
    ListItr itr2 = ListItrNext(itr1);
    int result = ListItrEquals(itr1, itr2);
    if (!result)
    {
        printf("TestListItrEqualsDifferentItr: PASS\n");
    }
    else
    {
        printf("TestListItrEqualsDifferentItr: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrEqualsNullItr()
{
    int result = ListItrEquals(NULL, NULL);
    if (result == 0)
    {
        printf("TestListItrEqualsNullItr: PASS\n");
    }
    else
    {
        printf("TestListItrEqualsNullItr: FAIL\n");
    }
}

/* ListItrForEach Tests */
static void TestListItrForEachValid()
{
    List *list = ListCreate();
    int items[] = {1, 2, 3};
    ListPushTail(list, &items[0]);
    ListPushTail(list, &items[1]);
    ListPushTail(list, &items[2]);
    int sum = 0;
    ListItr begin = ListItrBegin(list);
    ListItr end = ListItrEnd(list);
    ListItr resultItr = ListItrForEach(begin, end, SumAction, &sum);
    if (sum == 6 && resultItr == end)
    {
        printf("TestListItrForEachValid: PASS\n");
    }
    else
    {
        printf("TestListItrForEachValid: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrForEachNullAction()
{
    List *list = ListCreate();
    int items[] = {1, 2};
    ListPushTail(list, &items[0]);
    ListPushTail(list, &items[1]);
    ListItr begin = ListItrBegin(list);
    ListItr end = ListItrEnd(list);
    ListItr resultItr = ListItrForEach(begin, end, NULL, NULL);
    if (resultItr == NULL)
    {
        printf("TestListItrForEachNullAction: PASS\n");
    }
    else
    {
        printf("TestListItrForEachNullAction: FAIL\n");
    }
    ListDestroy(&list, NULL);
}

static void TestListItrForEachNullBeginOrEnd()
{
    List *list = ListCreate();
    int sum = 0;
    ListItr resultItr = ListItrForEach(NULL, NULL, SumAction, &sum);
    if (resultItr == NULL)
    {
        printf("TestListItrForEachNullBeginOrEnd: PASS\n");
    }
    else
    {
        printf("TestListItrForEachNullBeginOrEnd: FAIL\n");
    }
    ListDestroy(&list, NULL);
}
