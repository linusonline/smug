#include <utils/binarytree.h>
#include <stdlib.h>
#include <CuTest.h>

static int _compareAddrs(void* voida, void* voidb)
{
    return voida == voidb;
}

static int _compareInts(void* voida, void* voidb)
{
    int inta = *((int*)voida);
    int intb = *((int*)voidb);
    return inta == intb ? 0 : (inta < intb ? -1 : 1);
}

void BinaryTree_new_shouldReturnNonNull(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    CuAssert(tc, "BinaryTree_new returned NULL", bt != NULL);
    BinaryTree_delete(bt);
}

void BinaryTree_new_shouldReturnEmptyTree(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    CuAssertTrue(tc, BinaryTree_isEmpty(bt));
    BinaryTree_delete(bt);
}

void BinaryTree_insert_shouldYieldNonEmptyTree(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    BinaryTree_insert(bt, &a);
    CuAssertTrue(tc, !BinaryTree_isEmpty(bt));
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldReturnNullOnEmptyTree(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    CuAssertTrue(tc, BinaryTree_find(bt, &a) == NULL);
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldNotFindMissingElement(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    int b = 1;
    BinaryTree_insert(bt, &a);
    CuAssertTrue(tc, BinaryTree_find(bt, &b) == NULL);
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldFindInsertedElement1(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    BinaryTree_insert(bt, &a);
    CuAssertTrue(tc, BinaryTree_find(bt, &a) == &a);
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldFindInsertedElement2(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    int b = 0;
    BinaryTree_insert(bt, &a);
    CuAssertTrue(tc, BinaryTree_find(bt, &b) == &a);
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldFindInsertedElement3(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    int b = 1;
    BinaryTree_insert(bt, &a);
    BinaryTree_insert(bt, &b);
    CuAssertTrue(tc, BinaryTree_find(bt, &b) == &b);
    BinaryTree_delete(bt);
}

void BinaryTree_find_shouldFindInsertedElement4(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    int b = 1;
    int c = 2;
    BinaryTree_insert(bt, &b);
    BinaryTree_insert(bt, &a);
    BinaryTree_insert(bt, &c);
    CuAssertTrue(tc, BinaryTree_find(bt, &a) == &a);
    BinaryTree_delete(bt);
}

void BinaryTree_removeAll_shouldEmptyTree(CuTest* tc)
{
    BinaryTree* bt = BinaryTree_new(_compareInts);
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 3;
    BinaryTree_insert(bt, &a);
    BinaryTree_insert(bt, &b);
    BinaryTree_insert(bt, &c);
    BinaryTree_insert(bt, &d);
    BinaryTree_removeAll(bt);
    CuAssertTrue(tc, BinaryTree_isEmpty(bt));
    BinaryTree_delete(bt);
}

CuSuite* BinaryTreeTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, BinaryTree_new_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, BinaryTree_new_shouldReturnEmptyTree);
	SUITE_ADD_TEST(suite, BinaryTree_insert_shouldYieldNonEmptyTree);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldReturnNullOnEmptyTree);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldNotFindMissingElement);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldFindInsertedElement1);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldFindInsertedElement2);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldFindInsertedElement3);
	SUITE_ADD_TEST(suite, BinaryTree_find_shouldFindInsertedElement4);
	SUITE_ADD_TEST(suite, BinaryTree_removeAll_shouldEmptyTree);

	return suite;
}
