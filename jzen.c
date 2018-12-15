#include <string.h>
#include <wchar.h>
#include <stdlib.h>

#include "jzen.h"

void jzenParser_error(jzenParser *parser, wchar_t *message)
{
    if (message) {
        parser->error_message = message;
    } else {
        parser->error_message = L"Unspecified error";
    }
}

jzenNumber *jzenParser_parseNumber(jzenParser *parser)
{
    return NULL;
}



bool jzenParser_running(jzenParser *parser)
{
    if (parser->error_message)
        return false;
    return true;
}

/**
 * Return the next non space token, also updates the row and col.
 */
wchar_t jzenParser_next(jzenParser *parser)
{
    wchar_t value;
    while (jzenParser_running(parser) && (value = parser->next(parser->next_arg))) {
        parser->col++;
        if (value == L'\n') {
            parser->col = 1;
            parser->row++;
        }
        if (!iswspace(value)) break;      
    }
    parser->current = value;
    return value;
}

jzenObject *jzenParser_parseObject(jzenParser *parser)
{
    wchar_t cur;
    jzenObject *obj = jzenObject_create();
    while ((cur = jzenParser_next(parser)) && cur != L'}') {
        if (cur == L',') {
            // Blah! no op
        }
        else {
            jzenString *key = jzenParser_parseString(parser);
            if (!key) {
                jzenParser_error(parser, L"String value required as key");
                break;
            }
            if (jzenParser_next(parser) != L':') {
                jzenParser_error(parser, L"Expected semi-colon");
            }
            jzenItem *val = jzenParser_parseItem(parser);
            if (!val) break;
            jzenObject_addItem(obj, key, val);
        }
    }
    if (!jzenParser_running(parser)) {
        jzenObject_destroy(obj);
        return NULL;
    }
    return obj;
}

jzenString *jzenParser_parseString(jzenParser *parser)
{
    bool slash = false;
    wchar_t cur;
    jzenString *str = jzenString_create();

    while ((cur = jzenParser_next(parser))) {
        if (slash) {
            slash = false;
            switch (cur) {
            case L'"':
            case L'\\':
            case L'/':
                break; // pass through
            case L'b':
                cur = L'\b';
                break;
            case L'f':
                cur = L'\f';
                break;
            case L'n':
                cur = L'\n';
                break;
            case L'r':
                cur = L'\r';
                break;
            case L't':
                cur = L'\t';
                break;
            // TODO: hex
            default:
                jzenParser_error(parser, L"Unsupported escape character");
            }
        }
        else {
            if (cur == L'\\') {
                slash = true;
                continue;
            } 
        }
        jzenString_appendOne(str, cur);
    }
    if (!jzenParser_running(parser)) {
        jzenString_destroy(str);
        return NULL;
    }
    return str;
}

jzenItem *jzenParser_parseItem(jzenParser *parser)
{
    jzenItem *item;
    switch (parser->current) {
    case L'{':
        item = (jzenItem*)jzenParser_parseObject(parser);
        break;
    case L'[':
        item = (jzenItem*)jzenParser_parseList(parser);
        break;
    case L'"':
        item = (jzenItem*)jzenParser_parseString(parser);
        break;
    case L't':
        if (jzenParser_require(parser, L"true")) {
            item = (jzenItem*)jzenBoolean_create(true);
        }
        break;
    case L'f':
        if (jzenParser_require(parser, L"false")) {
            item = (jzenItem*)jzenBoolean_create(false);
        }
        break;
    case L'n':
        if (jzenParser_require(parser, L"null")) {
            item = (jzenItem*)jzenNull_create();
        }
        break;
    default:
        item = (jzenItem*)jzenParser_parseNumber(parser);
    }

    return item;
}

bool jzenParser_require(jzenParser *parser, wchar_t *text)
{
    for (int i = 0; parser->current != text[i]; i++) {
        if (!text[i]) return true;
        jzenParser_next(parser);
    }
    
    jzenParser_error(parser, L"Unexpected keyword");
    return false;
}
        
        
jzenList *jzenParser_parseList(jzenParser *parser)
{
    wchar_t cur;
    jzenList *list = jzenList_create();
    jzen_assert(!list);
    while ((cur = jzenParser_next(parser))) {
        if (cur == L']') break;
        if (cur == L',') continue;
        jzenItem *item = jzenParser_parseItem(parser);
        jzen_assert_free(!item, jzenList, list);

    }
}

void jzenParser_pushToken(jzenParser *parser, wchar_t token)
{
    
}



