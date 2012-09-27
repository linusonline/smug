#include <linkedlist.h>
#include <stdlib.h>
#include <CuTest.h>

static int accumulatedItems = 0;
static int calledTimes = 0;

static void accumulateItems(void* number)
{
    calledTimes++;
    accumulatedItems += *(int*)number;
}

static void freeInt(void* number)
{
    free((int*)number);
}

static void* doubleNumber(void* number)
{
    calledTimes++;
    int* intptr = (int*)malloc(sizeof(int));
    *intptr = (*(int*)number) * 2;
    return intptr;
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
    LinkedList_delete(ll);
}

void LinkedList_new_returnsEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssert(tc, "LinkedList_new returned non-empty list", LinkedList_isEmpty(ll));
    LinkedList_delete(ll);
}

void LinkedList_new_returnsZeroLengthList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssert(tc, "LinkedList_new returned non-empty list", LinkedList_length(ll) == 0);
    LinkedList_delete(ll);
}

void LinkedList_addLast_shouldMakeNewListNonEmpty(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, !LinkedList_isEmpty(ll));
    LinkedList_delete(ll);
}

void LinkedList_addLast_shouldIncreaseLength(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 1);
    LinkedList_addLast(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 2);
    LinkedList_delete(ll);
}

void LinkedList_addLast_itemShouldBeAddedLast(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addLast(ll, &one);
    LinkedList_addLast(ll, &two);
    CuAssertTrue(tc, (int*)LinkedList_getLast(ll) == &two);
    LinkedList_delete(ll);
}

void LinkedList_addFirst_shouldMakeNewListNonEmpty(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, !LinkedList_isEmpty(ll));
    LinkedList_delete(ll);
}

void LinkedList_addFirst_shouldIncreaseLength(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int n = 4;
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 1);
    LinkedList_addFirst(ll, &n);
    CuAssertTrue(tc, LinkedList_length(ll) == 2);
    LinkedList_delete(ll);
}

void LinkedList_addFirst_itemShouldBeAddedFirst(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, (int*)LinkedList_getFirst(ll) == &two);
    LinkedList_delete(ll);
}

void LinkedList_getFirst_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_getFirst(ll) == NULL);
    LinkedList_delete(ll);
}

void LinkedList_getLast_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_getLast(ll) == NULL);
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
}

void LinkedList_removeAll_shouldWorkOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    LinkedList_removeAll(ll);
    CuAssertTrue(tc, LinkedList_isEmpty(ll));
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
}

void LinkedList_popLast_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_popLast(ll) == NULL);
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
}

void LinkedList_popLast_shouldReturnLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, (int*)LinkedList_popLast(ll) == &two);
    LinkedList_delete(ll);
}

void LinkedList_popFirst_shouldReturnNullOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_popFirst(ll) == NULL);
    LinkedList_delete(ll);
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
    LinkedList_delete(ll);
}

void LinkedList_popFirst_shouldReturnLastItem(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, (int*)LinkedList_popFirst(ll) == &one);
    LinkedList_delete(ll);
}

void LinkedList_forAll_shouldReturnTrueForEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, LinkedList_forAll(ll, isEven));
    CuAssertTrue(tc, LinkedList_forAll(ll, isOdd));
    LinkedList_delete(ll);
}

void LinkedList_forAll_shouldReturnTrueWhenAllTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int two = 2;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_forAll(ll, isEven));
    LinkedList_delete(ll);
}

void LinkedList_forAll_shouldReturnFalseWhenSomeFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int one = 1;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &one);
    CuAssertTrue(tc, !LinkedList_forAll(ll, isEven));
    LinkedList_delete(ll);
}

void LinkedList_forAll_shouldReturnFalseWhenAllFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &three);
    CuAssertTrue(tc, !LinkedList_forAll(ll, isEven));
    LinkedList_delete(ll);
}

void LinkedList_exists_shouldReturnFalseForEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    CuAssertTrue(tc, !LinkedList_exists(ll, isEven));
    CuAssertTrue(tc, !LinkedList_exists(ll, isOdd));
    LinkedList_delete(ll);
}

void LinkedList_exists_shouldReturnTrueWhenAllTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int three = 3;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &three);
    CuAssertTrue(tc, LinkedList_exists(ll, isOdd));
    LinkedList_delete(ll);
}

void LinkedList_exists_shouldReturnTrueWhenSomeTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, LinkedList_exists(ll, isOdd));
    LinkedList_delete(ll);
}

void LinkedList_exists_shouldReturnFalseWhenAllFalse(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int two = 2;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &two);
    CuAssertTrue(tc, !LinkedList_exists(ll, isOdd));
    LinkedList_delete(ll);
}

void LinkedList_doList_shouldWorkOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    accumulatedItems = 0;
    calledTimes = 0;
    LinkedList_doList(ll, accumulateItems);
    CuAssertTrue(tc, accumulatedItems == 0);
    CuAssertTrue(tc, calledTimes == 0);
    LinkedList_delete(ll);
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
    accumulatedItems = 0;
    calledTimes = 0;
    LinkedList_doList(ll, accumulateItems);
    CuAssertTrue(tc, accumulatedItems == 15);
    CuAssertTrue(tc, calledTimes == 4);
    LinkedList_delete(ll);
}

void LinkedList_doListIf_shouldWorkOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    accumulatedItems = 0;
    calledTimes = 0;
    LinkedList_doListIf(ll, accumulateItems, isEven);
    CuAssertTrue(tc, accumulatedItems == 0);
    CuAssertTrue(tc, calledTimes == 0);
    LinkedList_delete(ll);
}

void LinkedList_doListIf_shouldDoWhenPredicateIsTrue(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int two = 2;
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    accumulatedItems = 0;
    calledTimes = 0;
    LinkedList_doListIf(ll, accumulateItems, isEven);
    CuAssertTrue(tc, accumulatedItems == 2);
    CuAssertTrue(tc, calledTimes == 1);
    LinkedList_delete(ll);
}

void LinkedList_getThose_shouldReturnEmptyListOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    LinkedList* l2 = LinkedList_getThose(ll, isEven);
    CuAssertTrue(tc, LinkedList_isEmpty(l2));
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

void LinkedList_getThose_shouldReturnEmptyListOnNoMatches(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int one = 1;
    int three = 3;
    LinkedList_addLast(ll, &one);
    LinkedList_addLast(ll, &three);
    LinkedList* l2 = LinkedList_getThose(ll, isEven);
    CuAssertTrue(tc, LinkedList_isEmpty(l2));
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

void LinkedList_getThose_shouldReturnAllPositives(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    LinkedList_addLast(ll, &zero);
    LinkedList_addLast(ll, &one);
    LinkedList_addLast(ll, &two);
    LinkedList_addLast(ll, &three);
    LinkedList_addLast(ll, &four);
    LinkedList_addLast(ll, &five);
    LinkedList* l2 = LinkedList_getThose(ll, isOdd);
    CuAssertTrue(tc, LinkedList_length(l2) == 3);
    CuAssertTrue(tc, LinkedList_elementExists(l2, &one));
    CuAssertTrue(tc, LinkedList_elementExists(l2, &three));
    CuAssertTrue(tc, LinkedList_elementExists(l2, &five));
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

void LinkedList_getThose_shouldReturnResultsInOriginalOrder(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int zero = 0;
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    LinkedList_addFirst(ll, &zero);
    LinkedList_addFirst(ll, &one);
    LinkedList_addFirst(ll, &two);
    LinkedList_addFirst(ll, &three);
    LinkedList_addFirst(ll, &four);
    LinkedList_addFirst(ll, &five);
    LinkedList* l2 = LinkedList_getThose(ll, isOdd);
    CuAssertTrue(tc, LinkedList_length(l2) == 3);
    CuAssertTrue(tc, LinkedList_popFirst(l2) == &five);
    CuAssertTrue(tc, LinkedList_popFirst(l2) == &three);
    CuAssertTrue(tc, LinkedList_popFirst(l2) == &one);
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

void LinkedList_map_shouldReturnEmptyListOnEmptyList(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    LinkedList* l2 = LinkedList_map(ll, doubleNumber);
    CuAssertTrue(tc, LinkedList_isEmpty(l2));
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

void LinkedList_map_shouldReturnCorrectResult(CuTest* tc)
{
    LinkedList* ll = LinkedList_new();
    int two = 2;
    int three = 3;
    LinkedList_addLast(ll, &two);
    LinkedList_addLast(ll, &three);
    LinkedList* l2 = LinkedList_map(ll, doubleNumber);
    CuAssertTrue(tc, LinkedList_length(l2) == 2);
    CuAssertTrue(tc, *(int*)LinkedList_getFirst(l2) == 4);
    CuAssertTrue(tc, *(int*)LinkedList_getLast(l2) == 6);
    LinkedList_deleteContents(l2, freeInt);
    LinkedList_delete(ll);
    LinkedList_delete(l2);
}

// LinkedList_concat

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
	SUITE_ADD_TEST(suite, LinkedList_doListIf_shouldWorkOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_doListIf_shouldDoWhenPredicateIsTrue);
	SUITE_ADD_TEST(suite, LinkedList_getThose_shouldReturnEmptyListOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_getThose_shouldReturnEmptyListOnNoMatches);
	SUITE_ADD_TEST(suite, LinkedList_getThose_shouldReturnAllPositives);
	SUITE_ADD_TEST(suite, LinkedList_getThose_shouldReturnResultsInOriginalOrder);
	SUITE_ADD_TEST(suite, LinkedList_map_shouldReturnEmptyListOnEmptyList);
	SUITE_ADD_TEST(suite, LinkedList_map_shouldReturnCorrectResult);

	return suite;
}
