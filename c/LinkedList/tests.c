#include <stdio.h>  /* printf */
#include <string.h> /* strncpy */
#include "person_linkedlist.h"

/********************** Help Functions **********************/

static void ResetPersons(Person persons[], size_t size);
static void PrintTestResult(const char *testName, int condition);

/********************** Test Functions **********************/

/* ListInsertHead Tests */
static void TestListInsertHeadEmptyList(Person persons[]);
static void TestListInsertHeadFullList(Person persons[]);

/* ListRemoveHead Tests */
static void TestListRemoveHeadEmptyList(Person persons[]);
static void TestListRemoveHeadFullList(Person persons[]);

/* ListInsertByKey Tests */
static void TestListInsertByKeyEmptyList(Person persons[]);
static void TestListInsertByKeyNullItem(Person persons[]);
static void TestListInsertByKeyDuplicateKey(Person persons[]);
static void TestListInsertByKeyInsertAtEnd(Person persons[]);
static void TestListInsertByKeyInsertAtMiddle(Person persons[]);
static void TestListInsertByKeyInsertAtHead(Person persons[]);

/* ListInsertByKeyRec Tests */
static void TestListInsertByKeyRecEmptyList(Person persons[]);
static void TestListInsertByKeyRecNullItem(Person persons[]);
static void TestListInsertByKeyRecDuplicateKey(Person persons[]);
static void TestListInsertByKeyRecInsertAtEnd(Person persons[]);
static void TestListInsertByKeyRecInsertAtMiddle(Person persons[]);
static void TestListInsertByKeyRecInsertAtHead(Person persons[]);

/* ListRemoveByKey Tests */
static void TestListRemoveByKeyEmptyList(Person persons[]);
static void TestListRemoveByKeyNullItem(Person persons[]);
static void TestListRemoveByKeyKeyNotFound(Person persons[]);
static void TestListRemoveByKeyRemoveMiddle(Person persons[]);
static void TestListRemoveByKeyRemoveHead(Person persons[]);

/* ListRemoveByKeyRec Tests */
static void TestListRemoveByKeyRecEmptyList(Person persons[]);
static void TestListRemoveByKeyRecNullItem(Person persons[]);
static void TestListRemoveByKeyRecKeyNotFound(Person persons[]);
static void TestListRemoveByKeyRecRemoveMiddle(Person persons[]);
static void TestListRemoveByKeyRecRemoveHead(Person persons[]);

/* PrintList Tests */
static void TestPrintListEmptyList(Person persons[]);
static void TestPrintListFullList(Person persons[]);

int main()
{
    Person persons[3] = {{1, "Bob", 20, NULL}, {2, "Alice", 25, NULL}, {3, "Robert", 30, NULL}};

    /* ListInsertHead Tests */
    TestListInsertHeadEmptyList(persons);
    TestListInsertHeadFullList(persons);

    /* ListRemoveHead Tests */
    TestListRemoveHeadEmptyList(persons);
    TestListRemoveHeadFullList(persons);

    /* ListInsertByKey Tests */
    TestListInsertByKeyEmptyList(persons);
    TestListInsertByKeyNullItem(persons);
    TestListInsertByKeyDuplicateKey(persons);
    TestListInsertByKeyInsertAtEnd(persons);
    TestListInsertByKeyInsertAtMiddle(persons);
    TestListInsertByKeyInsertAtHead(persons);

    /* ListInsertByKeyRec Tests */
    TestListInsertByKeyRecEmptyList(persons);
    TestListInsertByKeyRecNullItem(persons);
    TestListInsertByKeyRecDuplicateKey(persons);
    TestListInsertByKeyRecInsertAtEnd(persons);
    TestListInsertByKeyRecInsertAtMiddle(persons);
    TestListInsertByKeyRecInsertAtHead(persons);

    /* ListRemoveByKey Tests */
    TestListRemoveByKeyEmptyList(persons);
    TestListRemoveByKeyNullItem(persons);
    TestListRemoveByKeyKeyNotFound(persons);
    TestListRemoveByKeyRemoveMiddle(persons);
    TestListRemoveByKeyRemoveHead(persons);

    /* ListRemoveByKeyRec Tests */
    TestListRemoveByKeyRecEmptyList(persons);
    TestListRemoveByKeyRecNullItem(persons);
    TestListRemoveByKeyRecKeyNotFound(persons);
    TestListRemoveByKeyRecRemoveMiddle(persons);
    TestListRemoveByKeyRecRemoveHead(persons);

    /* PrintList Tests */
    TestPrintListEmptyList(persons);
    TestPrintListFullList(persons);

    return 0;
}

/********************** Test Functions **********************/

/* ListInsertHead Tests */

static void TestListInsertHeadEmptyList(Person persons[])
{
    const char *testName = "TestListInsertHeadEmptyList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertHeadFullList(Person persons[])
{
    const char *testName = "TestListInsertHeadFullList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[1]);
    head = ListInsertHead(head, &persons[0]);

    int condition = (head == &persons[0]) && (head->m_next == &persons[1]);
    PrintTestResult(testName, condition);
}

/* ListRemoveHead Tests */

static void TestListRemoveHeadEmptyList(Person persons[])
{
    const char *testName = "TestListRemoveHeadEmptyList";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListRemoveHead(head, &removedPerson);

    int condition = (head == NULL) && (removedPerson == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveHeadFullList(Person persons[])
{
    const char *testName = "TestListRemoveHeadFullList";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);
    head = ListInsertHead(head, &persons[1]);

    head = ListRemoveHead(head, &removedPerson);

    int condition = (removedPerson == &persons[1]) && (head == &persons[0]);
    PrintTestResult(testName, condition);
}

/* ListInsertByKey Tests */

static void TestListInsertByKeyEmptyList(Person persons[])
{
    const char *testName = "TestListInsertByKeyEmptyList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyNullItem(Person persons[])
{
    const char *testName = "TestListInsertByKeyNullItem";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, 1, NULL);

    int condition = (head == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyDuplicateKey(Person persons[])
{
    const char *testName = "TestListInsertByKeyDuplicateKey";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[0].m_id, &persons[0]); /* Duplicate */

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyInsertAtEnd(Person persons[])
{
    const char *testName = "TestListInsertByKeyInsertAtEnd";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKey(head, persons[2].m_id, &persons[2]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]) &&
                    (head->m_next->m_next->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyInsertAtMiddle(Person persons[])
{
    const char *testName = "TestListInsertByKeyInsertAtMiddle";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[2].m_id, &persons[2]);
    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyInsertAtHead(Person persons[])
{
    const char *testName = "TestListInsertByKeyInsertAtHead";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKey(head, persons[2].m_id, &persons[2]);
    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]);
    PrintTestResult(testName, condition);
}

/* ListInsertByKeyRec Tests */

static void TestListInsertByKeyRecEmptyList(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecEmptyList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyRecNullItem(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecNullItem";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, 1, NULL);

    int condition = (head == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyRecDuplicateKey(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecDuplicateKey";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]); /* Duplicate */

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyRecInsertAtEnd(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecInsertAtEnd";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKeyRec(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKeyRec(head, persons[2].m_id, &persons[2]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]) &&
                    (head->m_next->m_next->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyRecInsertAtMiddle(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecInsertAtMiddle";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKeyRec(head, persons[2].m_id, &persons[2]);
    head = ListInsertByKeyRec(head, persons[1].m_id, &persons[1]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]);
    PrintTestResult(testName, condition);
}

static void TestListInsertByKeyRecInsertAtHead(Person persons[])
{
    const char *testName = "TestListInsertByKeyRecInsertAtHead";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKeyRec(head, persons[2].m_id, &persons[2]);
    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);

    int condition = (head == &persons[0]) &&
                    (head->m_next == &persons[1]) &&
                    (head->m_next->m_next == &persons[2]);
    PrintTestResult(testName, condition);
}

/* ListRemoveByKey Tests */

static void TestListRemoveByKeyEmptyList(Person persons[])
{
    const char *testName = "TestListRemoveByKeyEmptyList";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListRemoveByKey(head, 1, &removedPerson);

    int condition = (head == NULL) && (removedPerson == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyNullItem(Person persons[])
{
    const char *testName = "TestListRemoveByKeyNullItem";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);

    head = ListRemoveByKey(head, persons[0].m_id, NULL);

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyKeyNotFound(Person persons[])
{
    const char *testName = "TestListRemoveByKeyKeyNotFound";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);

    head = ListRemoveByKey(head, 10, &removedPerson);

    int condition = (removedPerson == NULL) && (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRemoveMiddle(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRemoveMiddle";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKey(head, persons[2].m_id, &persons[2]);

    head = ListRemoveByKey(head, persons[1].m_id, &removedPerson);

    int condition = (removedPerson == &persons[1]) &&
                    (head == &persons[0]) &&
                    (head->m_next == &persons[2]) &&
                    (head->m_next->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRemoveHead(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRemoveHead";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);

    head = ListRemoveByKey(head, persons[0].m_id, &removedPerson);

    int condition = (removedPerson == &persons[0]) &&
                    (head == &persons[1]) &&
                    (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

/* ListRemoveByKeyRec Tests */

static void TestListRemoveByKeyRecEmptyList(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRecEmptyList";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListRemoveByKeyRec(head, 1, &removedPerson);

    int condition = (head == NULL) && (removedPerson == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRecNullItem(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRecNullItem";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);

    head = ListRemoveByKeyRec(head, persons[0].m_id, NULL);

    int condition = (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRecKeyNotFound(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRecKeyNotFound";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertHead(head, &persons[0]);

    head = ListRemoveByKeyRec(head, 10, &removedPerson);
    int condition = (removedPerson == NULL) && (head == &persons[0]) && (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRecRemoveMiddle(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRecRemoveMiddle";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKeyRec(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKeyRec(head, persons[2].m_id, &persons[2]);

    head = ListRemoveByKeyRec(head, persons[1].m_id, &removedPerson);

    int condition = (removedPerson == &persons[1]) &&
                    (head == &persons[0]) &&
                    (head->m_next == &persons[2]) &&
                    (head->m_next->m_next == NULL);
    PrintTestResult(testName, condition);
}

static void TestListRemoveByKeyRecRemoveHead(Person persons[])
{
    const char *testName = "TestListRemoveByKeyRecRemoveHead";
    Person *head = NULL;
    Person *removedPerson = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKeyRec(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKeyRec(head, persons[1].m_id, &persons[1]);

    head = ListRemoveByKeyRec(head, persons[0].m_id, &removedPerson);

    int condition = (removedPerson == &persons[0]) &&
                    (head == &persons[1]) &&
                    (head->m_next == NULL);
    PrintTestResult(testName, condition);
}

/* PrintList Tests */

static void TestPrintListEmptyList(Person persons[])
{
    const char *testName = "TestPrintListEmptyList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    PrintList(head);              /* Should print nothing */
    PrintTestResult(testName, 1); /* Always pass */
}

static void TestPrintListFullList(Person persons[])
{
    const char *testName = "TestPrintListFullList";
    Person *head = NULL;

    ResetPersons(persons, 3);

    head = ListInsertByKey(head, persons[0].m_id, &persons[0]);
    head = ListInsertByKey(head, persons[1].m_id, &persons[1]);
    head = ListInsertByKey(head, persons[2].m_id, &persons[2]);

    printf("%s:\n", testName);
    PrintList(head);              /* Should print all persons */
    PrintTestResult(testName, 1); /* Always pass */
}

/********************** Help Functions **********************/

static void ResetPersons(Person persons[], size_t size)
{
    for (size_t i = 0; i < size; ++i)
    {
        persons[i].m_next = NULL;
    }
}

static void PrintTestResult(const char *testName, int condition)
{
    printf("%s: %s\n", testName, condition ? "PASSED" : "FAILED");
}
