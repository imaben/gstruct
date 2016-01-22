/*
 * The MIT License (MIT)
 * Copyright (c) 2016 maben <www@maben.com.cn>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "gstruct.h"

gstruct* gstruct_new()
{
    gstruct *gs = (gstruct *)malloc(sizeof(gstruct));
    gs->type = -1;
    gstruct_buffer_init(&gs->buffer);
    return gs;
}

void gstruct_free(gstruct* gs)
{
    if (gs->buffer) {
        free(gs->buffer);
    }
    free(gs);
}

int gstruct_add_char(gstruct* gs, char d)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_CHAR;
    g.via.chr = d;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_int(gstruct* gs, int d)
{
    gstruct_add_long(gs, d);
    return 0;
}

int gstruct_add_long(gstruct* gs, long d)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_INTEGER;
    g.via.lval = d;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_float(gstruct* gs, float d)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_FLOAT;
    g.via.dval = d;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_double(gstruct* gs, double d)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_DOUBLE;
    g.via.dval = d;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_nil(gstruct* gs)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_NIL;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_true(gstruct* gs)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_BOOLEAN;
    g.via.boolean = 1;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

int gstruct_add_false(gstruct* gs)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_BOOLEAN;
    g.via.boolean = 0;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}


int gstruct_add_array(gstruct* gs, size_t n)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_ARRAY;
    g.via.array.size = n;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    for (int i = 0; i < n; i++) {
        gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    }
    return 0;
}

int gstruct_add_map(gstruct* gs, size_t n)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_MAP;
    g.via.map.size = n;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_kv kv;

    for (int i = 0; i < n; i++) {
        // kv
        gstruct_buffer_write(gs->buffer, &kv, sizeof(gstruct_kv));
    }

    return 0;
}

int gstruct_add_str(gstruct* gs, size_t l)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_STR;
    g.via.str.size = l;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    return 0;
}

int gstruct_add_str_body(gstruct* gs, const void* b, size_t l)
{
    gstruct_buffer_write(gs->buffer, b, l);
    return 0;
}

int gstruct_add_bin(gstruct* gs, size_t l)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_BIN;
    g.via.bin.size = l;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    return 0;
}

int gstruct_add_bin_body(gstruct* gs, const void* b, size_t l)
{
    gstruct_buffer_write(gs->buffer, b, l);
    return 0;
}

int gstruct_add_ext(gstruct* gs, size_t l, int8_t type)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_EXT;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_ext e;
    e.type = type;
    e.size = l;
    gstruct_buffer_write(gs->buffer, &e, sizeof(gstruct_ext));
    return 0;
}

int gstruct_add_ext_body(gstruct* gs, const void* b, size_t l)
{
    gstruct_buffer_write(gs->buffer, b, l);
    return 0;
}

static inline int gstruct_parse(gstruct **gspp, char *buffer, char **offset)
{
    gstruct *gs = (gstruct *)buffer;
    (*gspp)->type = gs->type;
    char *cursor = buffer + sizeof(gstruct);
    int i = 0;
    gstruct_array *a    = NULL;
    gstruct_map *m      = NULL;
    switch (gs->type) {
        case GSTRUCT_TYPE_NIL:
        case GSTRUCT_TYPE_BOOLEAN:
        case GSTRUCT_TYPE_INTEGER:
        case GSTRUCT_TYPE_CHAR:
        case GSTRUCT_TYPE_DOUBLE:
            (*gspp)->via = gs->via;
            *offset = cursor;
            return GSTRUCT_SUCCESS;
        case GSTRUCT_TYPE_STR:
            (*gspp)->via.str.size = gs->via.str.size;
            (*gspp)->via.str.ptr = cursor;
            *offset = cursor + gs->via.str.size;
            return GSTRUCT_SUCCESS;
        case GSTRUCT_TYPE_ARRAY:
            a = &gs->via.array;
            (*gspp)->via.array.size = a->size;
            (*gspp)->via.array.ptr = (gstruct *)cursor;

            cursor += sizeof(gstruct) * a->size;
            for (i = 0; i < a->size; i++) {
                gstruct *p = (*gspp)->via.array.ptr + i;
                gstruct_parse(&p, cursor, &cursor);
            }
            *offset = cursor;
            return GSTRUCT_SUCCESS;
        case GSTRUCT_TYPE_MAP:
            m = &gs->via.map;
            (*gspp)->via.map.size = m->size;
            (*gspp)->via.map.ptr = (gstruct_kv *)cursor;

            cursor += sizeof(gstruct_kv) * m->size;
            for (i = 0; i < m->size; i++) {
                gstruct_kv *kv = (*gspp)->via.map.ptr + i;
                gstruct *key = &kv->key;
                gstruct *val = &kv->val;
                gstruct_parse(&key, cursor, &cursor);
                gstruct_parse(&val, cursor, &cursor);
            }
            *offset = cursor;
            return GSTRUCT_SUCCESS;
        case GSTRUCT_TYPE_BIN:
            (*gspp)->via.bin.size = gs->via.bin.size;
            (*gspp)->via.bin.ptr = cursor;
            *offset = cursor + gs->via.bin.size;
            return GSTRUCT_SUCCESS;
        case GSTRUCT_TYPE_EXT:
            // todo
            break;
        default:
            return GSTRUCT_PARSE_ERROR;
    }
    return GSTRUCT_PARSE_ERROR;
}

gstruct_apply_return gstruct_apply_data(gstruct *gs)
{
    if (!gs->buffer) {
        return GSTRUCT_NOMEM_ERROR;
    }

    char *offset;
    return gstruct_parse(&gs, gs->buffer->data, &offset);
}
