#include "minunit.h"
#include <linkedlist.h>
#include <stdio.h>

static char* LinkedList_new_returnsNonNull()
{
    LinkedList* ll = LinkedList_new();
    mu_assert("LinkedList_new returned NULL", ll != NULL);
    return 0;
}

static char* test_bar()
{
    LinkedList* ll = LinkedList_new();
    mu_assert("LinkedList_new returned non-empty list", LinkedList_isEmpty(ll));
    return 0;
}

static char* all_tests()
{
    mu_run_test(LinkedList_new_returnsNonNull);
    mu_run_test(test_bar);
    return 0;
}

int tests_run = 0;

int main(int argc, char **argv)
{
    char *result = all_tests();
    if (result != 0)
    {
        printf("%s\n", result);
    }
    else {
        printf("ALL TESTS PASSED\n");
    }
    printf("Tests run: %d\n", tests_run);

    return result != 0;
}
