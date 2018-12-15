
#include <stdio.h>

#include "jzen.h"


#define TEST_NULL(PTR) \
    if (!PTR) { \
        printf("Bad pointer : %s", #PTR); \
        return; \
    }

void test_obj();
void test_string();

int main()
{
    test_string();
    test_obj();   
    return 0;
}

void test_string()
{
    jzenString *str = jzenString_create();
    TEST_NULL(str);
    wprintf(L"%d: %ls\n", str->length, str->value);

    jzenString_appendOne(str, L'F');
    jzenString_appendOne(str, L'O');
    jzenString_appendOne(str, L'o');

    wprintf(L"%d: %ls\n", str->length, str->value);
    jzenString_append(str, L" bar");
    wprintf(L"%d: %ls\n", str->length, str->value);
    jzenString_destroy(str);
}

void test_obj()
{
    jzenObject *obj = jzenObject_create();
    TEST_NULL(obj);
    jzenObject_destroy(obj);
}

