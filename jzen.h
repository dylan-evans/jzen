#ifndef _JZEN_H
#define _JZEN_H supa dupa
#include <wchar.h>
#include <wctype.h>
#include <stdint.h>

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#define JZEN_BLOCK_SIZE     4096

#define JZEN_T_OBJECT       1
#define JZEN_T_LIST         2
#define JZEN_T_ARRAY        2
#define JZEN_T_NUMBER       3
#define JZEN_T_STRING       4
#define JZEN_T_BOOLEAN      5
#define JZEN_T_NULL         6

#define jzen_assert(CLAUSE) if (CLAUSE) { \
    return NULL; \
}

#define jzen_assert_free(CLAUSE, CLASS, VAR) if (CLAUSE) { \
    CLASS ## _destroy(VAR); \
    return NULL; \
}

typedef     struct _jzen_parser             jzenParser;
typedef     struct _jzen_document           jzenDocument;
typedef     struct _jzen_packed_document    jzenPackedDocument;
typedef     struct _jzen_iterator           jzenIterator;
typedef     struct _jzen_item_header        jzenItem;
typedef     struct _jzen_object             jzenObject;
typedef     struct _jzen_list               jzenList;
typedef     struct _jzen_string             jzenString;
typedef     struct _jzen_number             jzenNumber;
typedef     struct _jzen_boolean            jzenBoolean;
typedef     struct _jzen_null               jzenNull;


struct _jzen_document {
    jzenItem        *root;
};

struct _jzen_packed_document {
    uint32_t        header_size;
    uint32_t        packed_size;
    jzenDocument    *doc;
};

struct _jzen_item_header {
    uint8_t         type;
    jzenItem        *parent;
};

struct _jzen_object {
    jzenItem        item;
};

struct _jzen_list {
    jzenItem        item;
};

struct _jzen_string {
    jzenItem        item;
    uint32_t        length;
    uint32_t        capacity;
    wchar_t         *value;
};

struct _jzen_number {
    jzenItem        item;
    int             sub_type;
    union {
        uint32_t        i32;
        uint64_t        i64;
        double          f64;
        wchar_t         big_num;
    }               value;
};

struct _jzen_boolean {
    jzenItem        item;
    bool            value;
};

struct _jzen_null {
    jzenItem        item;
};

struct _jzen_parser {
    int             row;
    int             col;
    wchar_t         current;
    wchar_t         *error_message;
    wchar_t         (*next)(void *);
    void            *next_arg;
};


jzenItem        *jzenIterator_next(jzenIterator *iter);


void            jzenList_append(jzenList *lst, jzenItem *item);


jzenIterator    *jzenList_each(jzenList *lst);


jzenObject     *jzenObject_create();
jzenList       *jzenList_create();
jzenString     *jzenString_create();
jzenNumber     *jzenNumber_create();
jzenBoolean    *jzenBoolean_create();
jzenNull       *jzenNull_create();

void            jzenObject_destroy(jzenObject *);
void            jzenList_destroy(jzenList *);
void            jzenString_destroy(jzenString *);
void            jzenNumber_destroy(jzenNumber *);
void            jzenBoolean_destroy(jzenBoolean *);
void            jzenNull_destroy(jzenNull *);

jzenNumber     *jzenParser_parseNumber(jzenParser *);
jzenString     *jzenParser_parseString(jzenParser *);
jzenBoolean    *jzenParser_parseBoolean(jzenParser *);
jzenObject     *jzenParser_parseObject(jzenParser *);
jzenList       *jzenParser_parseList(jzenParser *);

void            jzenObject_destroy(jzenObject *obj);

void            jzenObject_add(jzenObject *obj, jzenItem *item);
jzenIterator   *jzenObject_each(jzenObject *obj);

jzenObject     *jzenItem_getObject(jzenItem *item);
jzenList       *jzenItem_getList(jzenItem *item);

bool            jzenParser_require(jzenParser *parser, wchar_t *);
jzenString     *jzenParser_parseString(jzenParser *parser);
jzenItem       *jzenParser_parseItem(jzenParser *parser);
jzenList       *jzenParser_parseList(jzenParser *parser);

void            jzenObject_addItem(jzenObject *obj, jzenString *key, jzenItem *val);
void            jzenString_appendOne(jzenString *str, wchar_t token);
void            jzenString_append(jzenString *str, wchar_t *tokens);

#endif /* _JZEN_H */

