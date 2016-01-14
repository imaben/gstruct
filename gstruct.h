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


/**
 * gstruct type
 * @{
 */
typedef enum {
    GSTRUCT_TYPE_NIL                = 0x00,
    GSTRUCT_TYPE_BOOLEAN            = 0x01,
    GSTRUCT_TYPE_INTEGER            = 0x02,
    GSTRUCT_TYPE_CHAR               = 0x03,
    GSTRUCT_TYPE_FLOAT              = 0x04,
    GSTRUCT_TYPE_DOUBLE             = GSTRUCT_TYPE_FLOAT, /* obsolete */
    GSTRUCT_TYPE_STR                = 0x05,
    GSTRUCT_TYPE_ARRAY              = 0x06,
    GSTRUCT_TYPE_MAP                = 0x07,
    GSTRUCT_TYPE_BIN                = 0x08,
    GSTRUCT_TYPE_EXT                = 0x09
} gstruct_type;
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

static gstruct* gstruct_new();
static void gstruct_free(gstruct* gs);

static int gstruct_add_char(gstruct* gs, char d);
static int gstruct_add_int(gstruct* gs, int d);
static int gstruct_add_long(gstruct* gs, long d);

static int gstruct_add_float(gstruct* gs, float d);
static int gstruct_add_double(gstruct* gs, double d);

static int gstruct_add_nil(gstruct* gs);
static int gstruct_add_true(gstruct* gs);
static int gstruct_add_false(gstruct* gs);

static int gstruct_add_array(gstruct* gs, size_t n);

static int gstruct_add_map(gstruct* gs, size_t n);

static int gstruct_add_str(gstruct* gs, size_t l);
static int gstruct_add_str_body(gstruct* gs, const void* b, size_t l);

static int gstruct_add_bin(gstruct* gs, size_t l);
static int gstruct_add_bin_body(gstruct* gs, const void* b, size_t l);

static int gstruct_add_ext(gstruct* gs, size_t l, int8_t type);
static int gstruct_add_ext_body(gstruct* gs, const void* b, size_t l);

static int gstruct_apply_data(gstruct *gs);
/** @} */



/**
 * Buffer struct
 * @{
 */
typedef struct gstruct_buffer {
    size_t size;
    char* data;
    size_t alloc;
} gstruct_buffer;

static inline void gstruct_buffer_init(gstruct_buffer* buf)
{
    memset(buf, 0, sizeof(gstruct_buffer));
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
