#include <linkedlist.h>
#include <CuTest.h>

void LinkedList_new_returnsNonNull(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssert(tc, "LinkedList_new returned NULL", ll != NULL);
}

void LinkedList_new_returnsEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssert(tc, "LinkedList_new returned non-empty list", LinkedList_isEmpty(ll));
}

CuSuite* LinkedListTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, LinkedList_new_returnsNonNull);
	SUITE_ADD_TEST(suite, LinkedList_new_returnsEmptyList);

	return suite;
}
