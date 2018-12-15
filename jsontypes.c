/*
 * Functions related to the structs used to represent JSON values 
 */

#include <stdlib.h>
#include <string.h>

#include "jzen.h"

#define STD_CREATE(NAME) jzen ## NAME  *jzen ## NAME  ## _create() \
    { \
        jzen ## NAME *item = (jzen ## NAME  *)malloc(sizeof(jzen ## NAME  )); \
        if (item) bzero(item, sizeof(jzen ## NAME )); \
        return item; \
    }




/*    ********** jzenItem    **********    */
// The jzenItem represents an abstract base type for each of the real JSON
// types. The item is primarily used to provide generic functionality.

jzenObject *jzenItem_getObject(jzenItem *item)
{
    if (item->type == JZEN_T_OBJECT) {
        return (jzenObject*)item;
    }
    return NULL;
}

jzenList   *jzenItem_getList(jzenItem *item)
{
    if (item->type == JZEN_T_LIST) {
        return (jzenList*)item;
    }
    return NULL;
}

jzenNumber *jzenItem_getNumber(jzenItem *item)
{
    if (item->type == JZEN_T_NUMBER) {
        return (jzenNumber*)item;
    }
    return NULL;
}

jzenString *jzenItem_getString(jzenItem *item)
{
    if (item->type == JZEN_T_STRING) {
        return (jzenString*)item;
    }
    return NULL;
}


/*    ********** jzenString  **********    */

jzenString *jzenString_create()
{
    jzenString *str = (jzenString*)malloc(sizeof(jzenString));
    str->value = malloc(sizeof(wchar_t) * JZEN_BLOCK_SIZE);
    str->length = 0;
    str->capacity = JZEN_BLOCK_SIZE;
    return str;
}

void jzenString_destroy(jzenString *str)
{
    free(str);
}

void jzenString_appendOne(jzenString *str, wchar_t token)
{
    if (str->capacity <= (str->length + 1)) {
        str->capacity += JZEN_BLOCK_SIZE;
        str->value = realloc(str->value, str->capacity);
    }
    str->value[str->length] = token;
    str->length++;
}

void jzenString_append(jzenString *str, wchar_t *tokens)
{
    wchar_t *cur;
    for (cur=tokens; *cur; cur++) {
        jzenString_appendOne(str, *cur);
    }
}


/*    ********** jzenObject  **********    */
STD_CREATE(Object);
void jzenObject_destroy(jzenObject *obj)
{
    free(obj);
}
void jzenObject_addItem(jzenObject *obj, jzenString *key, jzenItem *val)
{

}

/*    ********** jzenList    **********    */
STD_CREATE(List);
void jzenList_destroy(jzenList *lst)
{
    free(lst);
}


/*    ********** jzenNumber  **********    */
STD_CREATE(Number);
void jzenNumber_destroy(jzenNumber *num)
{
    free(num);
}


/*    ********** jzenBoolean **********    */
STD_CREATE(Boolean);
void jzenBoolean_destroy(jzenBoolean *bill)
{
    free(bill);
}


/*    ********** jzenNull    **********    */
STD_CREATE(Null);
void jzenNull_destroy(jzenNull *nelly)
{
    free(nelly);
}



