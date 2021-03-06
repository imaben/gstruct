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

#pragma once

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int bool;

/** Initialization buffer size */
#ifndef GSTRUCT_BUFFER_INIT_SIZE
#define GSTRUCT_BUFFER_INIT_SIZE 8192
#endif

#define   GS_TYPE_P(g)         g->type
#define   GS_CHR_P(g)          g->via.chr
#define   GS_INT_P(g)          g->via.lval
#define   GS_LONG_P(g)         g->via.lval
#define   GS_DOUBLE_P(g)       g->via.dval
#define   GS_BOOL_P(g)         g->via.boolean
#define   GS_STR_SIZE_P(g)     g->via.str.size
#define   GS_STR_PTR_P(g)      g->via.str.ptr
#define   GS_ARRSIZE_P(g)      g->via.array.size
#define   GS_ARRVAL_P(g,o)     g->via.array.ptr[o]
#define   GS_MAPSIZE_P(g)      g->via.map.size
#define   GS_MAPVAL_P(g,o)     g->via.map.ptr[o]
#define   GS_BIN_SIZE_P(g)     g->via.bin.size
#define   GS_BIN_PTR_P(g)      g->via.bin.ptr

#define   GS_TYPE(g)         g.type
#define   GS_CHR(g)          g.via.chr
#define   GS_INT(g)          g.via.lval
#define   GS_LONG(g)         g.via.lval
#define   GS_DOUBLE(g)       g.via.dval
#define   GS_BOOL(g)         g.via.boolean
#define   GS_STR_SIZE(g)     g.via.str.size
#define   GS_STR_PTR(g)      g.via.str.ptr
#define   GS_ARRSIZE(g)      g.via.array.size
#define   GS_ARRVAL(g,o)     g.via.array.ptr[o]
#define   GS_MAPSIZE(g)      g.via.map.size
#define   GS_MAPVAL(g,o)     g.via.map.ptr[o]
#define   GS_BIN_SIZE(g)     g.via.bin.size
#define   GS_BIN_PTR(g)      g.via.bin.ptr

#define GS_FOREACH(gs, gg) \
    for (int gs##gg##_i = 0; gs##gg##_i < GS_ARRSIZE_P(gs) && (gg = &GS_ARRVAL_P(gs, gs##gg##_i)); gs##gg##_i++)

/**
 * gstruct type
 * @{
 */
typedef enum {
    GSTRUCT_TYPE_NIL     = 0x00,
    GSTRUCT_TYPE_BOOLEAN = 0x01,
    GSTRUCT_TYPE_INTEGER = 0x02,
    GSTRUCT_TYPE_CHAR    = 0x03,
    GSTRUCT_TYPE_FLOAT   = 0x04,
    GSTRUCT_TYPE_DOUBLE  = GSTRUCT_TYPE_FLOAT, /* obsolete */
    GSTRUCT_TYPE_STR     = 0x05,
    GSTRUCT_TYPE_ARRAY   = 0x06,
    GSTRUCT_TYPE_MAP     = 0x07,
    GSTRUCT_TYPE_BIN     = 0x08,
    GSTRUCT_TYPE_EXT     = 0x09
} gstruct_type;
/** @} */

/**
 * gstruct apply data error
 * @{
 */
typedef enum {
    GSTRUCT_SUCCESS     = 2,
    GSTRUCT_EXTRA_BYTES = 1,
    GSTRUCT_CONTINUE    = 0,
    GSTRUCT_PARSE_ERROR = -1,
    GSTRUCT_NOMEM_ERROR = -2
} gstruct_apply_return;
/** @} */

/**
 * Base struct and operation api
 * @{
 */
struct gstruct;
struct gstruct_kv;
struct gstruct_buffer;

typedef struct {
    uint32_t size;
    struct gstruct* ptr;
} gstruct_array;

typedef struct {
    uint32_t size;
    struct gstruct_kv* ptr;
} gstruct_map;

typedef struct {
    uint32_t size;
    const char* ptr;
} gstruct_str;

typedef struct {
    uint32_t size;
    const char* ptr;
} gstruct_bin;

typedef struct {
    int8_t type;
    uint32_t size;
    const char* ptr;
} gstruct_ext;

typedef union {
    bool            boolean;
    char            chr;
    long            lval;
    double          dval;
    gstruct_str     str;
    gstruct_array   array;
    gstruct_map     map;
    gstruct_bin     bin;
    gstruct_ext     ext;
} gstruct_union;

typedef struct gstruct {
    gstruct_type type;
    gstruct_union via;
    struct gstruct_buffer *buffer;
} gstruct;

typedef struct gstruct_kv {
    gstruct key;
    gstruct val;
} gstruct_kv;

gstruct* gstruct_new();
void gstruct_free(gstruct* gs);

int gstruct_add_char(gstruct* gs, char d);
int gstruct_add_int(gstruct* gs, int d);
int gstruct_add_long(gstruct* gs, long d);

int gstruct_add_float(gstruct* gs, float d);
int gstruct_add_double(gstruct* gs, double d);

int gstruct_add_nil(gstruct* gs);
int gstruct_add_true(gstruct* gs);
int gstruct_add_false(gstruct* gs);

int gstruct_add_array(gstruct* gs, size_t n);

int gstruct_add_map(gstruct* gs, size_t n);

int gstruct_add_str(gstruct* gs, const void *b, size_t l);

int gstruct_add_bin(gstruct* gs, const void *b, size_t l);

int gstruct_add_ext(gstruct* gs, size_t l, int8_t type);

gstruct_apply_return gstruct_apply_data(gstruct *gs);
/** @} */

int gstruct_map_find_str(gstruct *gs, char *find, gstruct **g);

/**
 * Buffer struct
 * @{
 */
typedef struct gstruct_buffer {
    size_t size;
    char* data;
    size_t alloc;
} gstruct_buffer;

static inline void gstruct_buffer_init(gstruct_buffer** buf)
{
    *buf = (gstruct_buffer *)malloc(sizeof(gstruct_buffer));
    (*buf)->alloc = 0;
    (*buf)->size = 0;
}

static inline void gstruct_buffer_destroy(gstruct_buffer* buf)
{
    free(buf->data);
}

static inline gstruct_buffer* gstruct_buffer_new(void)
{
    return (gstruct_buffer*)calloc(1, sizeof(gstruct_buffer));
}

static inline void gstruct_buffer_free(gstruct_buffer* buf)
{
    if (buf == NULL) {
        return;
    }
    gstruct_buffer_destroy(buf);
    free(buf);
}

static inline int gstruct_buffer_write(void* data, const void* buf, size_t len)
{
    gstruct_buffer* gbuf = (gstruct_buffer*)data;
    if (0 == gbuf->alloc) {
        gbuf->data = calloc(0, GSTRUCT_BUFFER_INIT_SIZE);
    }
    if (gbuf->alloc - gbuf->size < len) {
        void* tmp;
        size_t nsize = (gbuf->alloc) ?
            gbuf->alloc * 2 : GSTRUCT_BUFFER_INIT_SIZE;

        while (nsize < gbuf->size + len) {
            size_t tmp_nsize = nsize * 2;
            if (tmp_nsize <= nsize) {
                nsize = gbuf->size + len;
                break;
            }
            nsize = tmp_nsize;
        }

        tmp = realloc(gbuf->data, nsize);
        if (!tmp) {
            return -1;
        }

        gbuf->data = (char*)tmp;
        gbuf->alloc = nsize;
    }

    memcpy(gbuf->data + gbuf->size, buf, len);
    gbuf->size += len;
    return 0;
}

static inline char* gstruct_buffer_release(gstruct_buffer* buf)
{
    char* tmp = buf->data;
    buf->size = 0;
    buf->data = NULL;
    buf->alloc = 0;
    return tmp;
}

static inline void gstruct_buffer_clear(gstruct_buffer* buf)
{
    buf->size = 0;
}
/** @} */
