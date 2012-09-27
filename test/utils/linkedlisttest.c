#include <linkedlist.h>
#include <CuTest.h>

static int testCounter = 0;

static void incrementCounter(void* number)
{
    testCounter += *(int*)number;
}

static BOOL isEven(void* i)
{
    return (*(int*)i % 2) == 0;
}

static BOOL isOdd(void* i)
{
    return (*(int*)i % 2) == 1;
}

void failingTest(CuTest* tc)
{
    CuAssertTrue(tc, FALSE);
}

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

void LinkedList_new_returnsZeroLengthList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssert(tc, "LinkedList_new returned non-empty list", LinkedList_length(ll) == 0);
}

void LinkedList_addLast_shouldMakeNewListNonEmpty(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, !LinkedList_isEmpty(ll));
}

void LinkedList_addLast_shouldIncreaseLength(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 1);
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 2);
}

void LinkedList_addLast_itemShouldBeAddedLast(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addLast(ll, &one);
    LinkedList_addLast(ll, &two);
    CuAssertTrue(tc, (int*)LinkedList_getLast(ll) == &two);
}

void LinkedList_addFirst_shouldMakeNewListNonEmpty(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, !LinkedList_isEmpty(ll));
}

void LinkedList_addFirst_shouldIncreaseLength(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 1);
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 2);
}

void LinkedList_addFirst_itemShouldBeAddedFirst(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, (int*)LinkedList_getFirst(ll) == &two);
}

void LinkedList_getFirst_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_getFirst(ll) == NULL);
}

void LinkedList_getLast_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_getLast(ll) == NULL);
}

void LinkedList_elementExists_shouldFindAddedElement(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_elementExists(ll, &one));
    CuAssertTrue(tc, LinkedList_elementExists(ll, &two));
}

void LinkedList_elementExists_shouldNotFindMissingElement(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, !LinkedList_elementExists(ll, &three));
}

void LinkedList_removeAll_shouldWorkOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    LinkedList_removeAll(ll);
    CuAssertTrue(tc, LinkedList_isEmpty(ll));
}

void LinkedList_removeAll_shouldEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    LinkedList_removeAll(ll);
    CuAssertTrue(tc, LinkedList_length(ll) == 0);
    CuAssertTrue(tc, LinkedList_isEmpty(ll));
}

void LinkedList_removeItem_shouldDecreaseLengthWhenItemFound(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_removeItem(ll, &one));
    CuAssertTrue(tc, LinkedList_length(ll) == 1);
}

void LinkedList_removeItem_shouldNotDecreaseLengthWhenItemNotFound(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, !LinkedList_removeItem(ll, &three));
    CuAssertTrue(tc, LinkedList_length(ll) == 2);
}

void LinkedList_removeItem_shouldRemoveCorrectItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    LinkedList_removeItem(ll, &one);
    CuAssertTrue(tc, (int*)LinkedList_getFirst(ll) == &two);
    CuAssertTrue(tc, !LinkedList_elementExists(ll, &one));
}

void LinkedList_popLast_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_popLast(ll) == NULL);
}

void LinkedList_popLast_shouldRemoveLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    LinkedList_popLast(ll);
    CuAssertTrue(tc, (int*)LinkedList_getFirst(ll) == &one);
    CuAssertTrue(tc, !LinkedList_elementExists(ll, &two));
}

void LinkedList_popLast_shouldReturnLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, (int*)LinkedList_popLast(ll) == &two);
}

void LinkedList_popFirst_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_popFirst(ll) == NULL);
}

void LinkedList_popFirst_shouldRemoveLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    LinkedList_popFirst(ll);
    CuAssertTrue(tc, (int*)LinkedList_getFirst(ll) == &two);
    CuAssertTrue(tc, !LinkedList_elementExists(ll, &one));
}

void LinkedList_popFirst_shouldReturnLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, (int*)LinkedList_popFirst(ll) == &one);
}

void LinkedList_forAll_shouldReturnTrueForEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_forAll(ll, isEven));
    CuAssertTrue(tc, LinkedList_forAll(ll, isOdd));
}

void LinkedList_forAll_shouldReturnTrueWhenAllTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int two = 2;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_forAll(ll, isEven));
}

void LinkedList_forAll_shouldReturnFalseWhenSomeFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int one = 1;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, !LinkedList_forAll(ll, isEven));
}

void LinkedList_forAll_shouldReturnFalseWhenAllFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &three);
    CuAssertTrue(tc, !LinkedList_forAll(ll, isEven));
}

void LinkedList_exists_shouldReturnFalseForEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, !LinkedList_exists(ll, isEven));
    CuAssertTrue(tc, !LinkedList_exists(ll, isOdd));
}

void LinkedList_exists_shouldReturnTrueWhenAllTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &three);
    CuAssertTrue(tc, LinkedList_exists(ll, isOdd));
}

void LinkedList_exists_shouldReturnTrueWhenSomeTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_exists(ll, isOdd));
}

void LinkedList_exists_shouldReturnFalseWhenAllFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int two = 2;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, !LinkedList_exists(ll, isOdd));
}

void LinkedList_doList_shouldWorkOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    testCounter = 0;
    LinkedList_doList(ll, incrementCounter);
    CuAssertTrue(tc, testCounter == 0);
}

void LinkedList_doList_shouldDoForEachItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    int four = 4;
    int eight = 8;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &four);
    LinkedList_addFirst(ll, &eight);
    testCounter = 0;
    LinkedList_doList(ll, incrementCounter);
    CuAssertTrue(tc, testCounter == 15);
}

CuSuite* LinkedListTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	// SUITE_ADD_TEST(suite, failingTest);
	SUITE_ADD_TEST(suite, LinkedList_new_returnsNonNull);
	SUITE_ADD_TEST(suite, LinkedList_new_returnsEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_new_returnsZeroLengthList);
	SUITE_ADD_TEST(suite, LinkedList_addLast_shouldMakeNewListNonEmpty);
	SUITE_ADD_TEST(suite, LinkedList_addLast_shouldIncreaseLength);
	SUITE_ADD_TEST(suite, LinkedList_addLast_itemShouldBeAddedLast);
	SUITE_ADD_TEST(suite, LinkedList_addFirst_shouldMakeNewListNonEmpty);
	SUITE_ADD_TEST(suite, LinkedList_addFirst_shouldIncreaseLength);
	SUITE_ADD_TEST(suite, LinkedList_addFirst_itemShouldBeAddedFirst);
	SUITE_ADD_TEST(suite, LinkedList_getFirst_shouldReturnNullOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_getLast_shouldReturnNullOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_elementExists_shouldFindAddedElement);
	SUITE_ADD_TEST(suite, LinkedList_elementExists_shouldNotFindMissingElement);
	SUITE_ADD_TEST(suite, LinkedList_removeAll_shouldWorkOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_removeAll_shouldEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_removeItem_shouldDecreaseLengthWhenItemFound);
	SUITE_ADD_TEST(suite, LinkedList_removeItem_shouldNotDecreaseLengthWhenItemNotFound);
	SUITE_ADD_TEST(suite, LinkedList_removeItem_shouldRemoveCorrectItem);
	SUITE_ADD_TEST(suite, LinkedList_popLast_shouldReturnNullOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_popLast_shouldRemoveLastItem);
	SUITE_ADD_TEST(suite, LinkedList_popLast_shouldReturnLastItem);
	SUITE_ADD_TEST(suite, LinkedList_popFirst_shouldReturnNullOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_popFirst_shouldRemoveLastItem);
	SUITE_ADD_TEST(suite, LinkedList_popFirst_shouldReturnLastItem);
	SUITE_ADD_TEST(suite, LinkedList_forAll_shouldReturnTrueForEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_forAll_shouldReturnTrueWhenAllTrue);
	SUITE_ADD_TEST(suite, LinkedList_forAll_shouldReturnFalseWhenSomeFalse);
	SUITE_ADD_TEST(suite, LinkedList_forAll_shouldReturnFalseWhenAllFalse);
	SUITE_ADD_TEST(suite, LinkedList_exists_shouldReturnFalseForEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_exists_shouldReturnTrueWhenAllTrue);
	SUITE_ADD_TEST(suite, LinkedList_exists_shouldReturnTrueWhenSomeTrue);
	SUITE_ADD_TEST(suite, LinkedList_exists_shouldReturnFalseWhenAllFalse);
	SUITE_ADD_TEST(suite, LinkedList_doList_shouldWorkOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_doList_shouldDoForEachItem);

	return suite;
}
