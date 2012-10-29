#include <utils/map.h>
#include <stdlib.h>
#include <CuTest.h>

static int _compareInts(void* voida, void* voidb)
{
    int inta = *((int*)voida);
    int intb = *((int*)voidb);
    return inta == intb ? 0 : (inta < intb ? -1 : 1);
}

void Map_new_shouldReturnNonNull(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    CuAssert(tc, "Map_new returned NULL", m != NULL);
    Map_delete(m);
}

void Map_new_shouldReturnEmptyMap(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    CuAssertTrue(tc, Map_isEmpty(m));
    Map_delete(m);
}

void Map_set_shouldYieldNonEmptyMap(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a;
    Map_set(m, &a, &a);
    CuAssertTrue(tc, !Map_isEmpty(m));
    Map_delete(m);
}

void Map_get_shouldReturnNullOnEmptyMap(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a;
    CuAssertTrue(tc, Map_get(m, &a) == NULL);
    Map_delete(m);
}

void Map_get_shouldReturnNullOnMissingElement(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    int b = 1;
    Map_set(m, &a, &a);
    CuAssertTrue(tc, Map_get(m, &b) == NULL);
    Map_delete(m);
}

void Map_get_shouldFindAddedElement(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    int b = 0;
    int c = 0;
    Map_set(m, &a, &c);
    CuAssertTrue(tc, Map_get(m, &b) == &c);
    Map_delete(m);
}

void Map_removeAll_shouldEmptyMap(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    int b = 0;
    int c = 0;
    Map_set(m, &a, &a);
    Map_set(m, &b, &b);
    Map_set(m, &c, &c);
    Map_removeAll(m);
    CuAssertTrue(tc, Map_get(m, &a) == NULL);
    CuAssertTrue(tc, Map_get(m, &b) == NULL);
    CuAssertTrue(tc, Map_get(m, &c) == NULL);
    Map_delete(m);
}

void Map_getIterator_shouldReturnNonNull(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    MapIterator* iter = Map_getIterator(m);
    CuAssertTrue(tc, iter != NULL);
    MapIterator_delete(iter);
    Map_delete(m);
}

void MapIterator_getNextPair_shouldReturnNullOnEmptyMap(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    MapIterator* iter = Map_getIterator(m);
    CuAssertTrue(tc, MapIterator_getNextPair(iter) == NULL);
    MapIterator_delete(iter);
    Map_delete(m);
}

void MapIterator_getNextPair_shouldReturnSingleKey(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    Map_set(m, &a, &a);
    MapIterator* iter = Map_getIterator(m);

    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &a);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == NULL);
    MapIterator_delete(iter);
    Map_delete(m);
}

void MapIterator_getNextPair_shouldReturnKeysInOrder1(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    int b = 1;
    Map_set(m, &a, &a);
    Map_set(m, &b, &b);
    MapIterator* iter = Map_getIterator(m);

    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &a);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &b);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == NULL);
    MapIterator_delete(iter);
    Map_delete(m);
}

void MapIterator_getNextPair_shouldReturnKeysInOrder2(CuTest* tc)
{
    Map* m = Map_new(_compareInts);
    int a = 0;
    int b = 1;
    int c = 2;
    Map_set(m, &c, &c);
    Map_set(m, &a, &a);
    Map_set(m, &b, &b);
    MapIterator* iter = Map_getIterator(m);

    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &a);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &b);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == &c);
    CuAssertTrue(tc, MapIterator_getNextKey(iter) == NULL);
    MapIterator_delete(iter);
    Map_delete(m);
}

CuSuite* MapTest_GetSuite()
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, Map_new_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, Map_new_shouldReturnEmptyMap);
	SUITE_ADD_TEST(suite, Map_set_shouldYieldNonEmptyMap);
	SUITE_ADD_TEST(suite, Map_get_shouldReturnNullOnEmptyMap);
	SUITE_ADD_TEST(suite, Map_get_shouldReturnNullOnMissingElement);
	SUITE_ADD_TEST(suite, Map_get_shouldFindAddedElement);
	SUITE_ADD_TEST(suite, Map_removeAll_shouldEmptyMap);
	SUITE_ADD_TEST(suite, Map_getIterator_shouldReturnNonNull);
	SUITE_ADD_TEST(suite, MapIterator_getNextPair_shouldReturnNullOnEmptyMap);
	SUITE_ADD_TEST(suite, MapIterator_getNextPair_shouldReturnSingleKey);
	SUITE_ADD_TEST(suite, MapIterator_getNextPair_shouldReturnKeysInOrder1);
	SUITE_ADD_TEST(suite, MapIterator_getNextPair_shouldReturnKeysInOrder2);

	return suite;
}
