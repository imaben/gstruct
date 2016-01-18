#include "gstruct.h"

gstruct* gstruct_new()
{
    gstruct *gs = (gstruct *)malloc(sizeof(gstruct));
    gs->type = -1;
    gstruct_buffer_init(gs->buffer);
    gstruct_buffer_write(gs->buffer, gs, sizeof(gs));
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
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_array arr;
    arr.size = n;
    gstruct_buffer_write(gs->buffer, &arr, sizeof(gstruct_array));
    return 0;
}

int gstruct_add_map(gstruct* gs, size_t n)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_MAP;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_map m;
    m.size = n;
    gstruct_buffer_write(gs->buffer, &m, sizeof(gstruct_map));

    return 0;
}

int gstruct_add_str(gstruct* gs, size_t l)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_STR;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_str s;
    s.size = l;
    gstruct_buffer_write(gs->buffer, &s, sizeof(gstruct_str));

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
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));

    gstruct_bin b;
    b.size = l;
    gstruct_buffer_write(gs->buffer, &b, sizeof(gstruct_bin));

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

inline char *gstruct_parse_scalar(char *buffer)
{
    return buffer + sizeof(gstruct);
}

inline char *gstruct_parse_str(gstruct *gs, char *buffer)
{
    char *cursor;
    gstruct_str *s = (gstruct_str *)(buffer);
    gs->via.str.size = s->size;

    cursor += sizeof(gstruct_str);
    gs->via.str.ptr = cursor;
    return cursor + s->size;
}

inline char *gstruct_parse_array(char *buffer)
{
    int i = 0;
    gstruct_array *arr = (gstruct_array *)buffer;
    char *cursor = buffer;
    for (; i < arr->size; i++) {
        cursor += sizeof(gstruct_array);
        gstruct *g = (gstruct *)cursor;
        switch (arr->ptr[i].type) {
            case GSTRUCT_TYPE_NIL:
            case GSTRUCT_TYPE_BOOLEAN:
            case GSTRUCT_TYPE_INTEGER:
            case GSTRUCT_TYPE_CHAR:
            case GSTRUCT_TYPE_DOUBLE:
                arr->ptr = (gstruct *)gstruct_parse_scalar(cursor);
                break;
            case GSTRUCT_TYPE_STR:
                arr->ptr = (gstruct *)gstruct_parse_str(g, cursor);
                break;
            case GSTRUCT_TYPE_ARRAY:
                arr->ptr = (gstruct *)gstruct_parse_array(cursor);
                break;
            case GSTRUCT_TYPE_MAP:
                break;
            case GSTRUCT_TYPE_BIN:
                break;
            case GSTRUCT_TYPE_EXT:
                break;
            default:
                return buffer;

        }
        arr->ptr++;
    }
    return cursor;
}

gstruct_apply_return gstruct_apply_data(gstruct *gs)
{
    if (!gs->buffer) {
        return GSTRUCT_NOMEM_ERROR;
    }

    gstruct_buffer *buffer = gs->buffer;
    char *cursor = buffer->data;
    while (cursor < buffer->data + buffer->size) {
        gstruct *g = (gstruct *)cursor;
        switch (g->type) {
            case GSTRUCT_TYPE_NIL:
            case GSTRUCT_TYPE_BOOLEAN:
            case GSTRUCT_TYPE_INTEGER:
            case GSTRUCT_TYPE_CHAR:
            case GSTRUCT_TYPE_DOUBLE:
                cursor = gstruct_parse_scalar(cursor);
                break;
            case GSTRUCT_TYPE_STR:
                cursor = gstruct_parse_str(g, cursor);
                break;
            case GSTRUCT_TYPE_ARRAY:
                break;
            case GSTRUCT_TYPE_MAP:
                break;
            case GSTRUCT_TYPE_BIN:
                break;
            case GSTRUCT_TYPE_EXT:
                break;
            default:
                return GSTRUCT_PARSE_ERROR;
        }
    }
    return GSTRUCT_SUCCESS;
}
