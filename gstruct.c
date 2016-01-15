#include "gstruct.h"

static gstruct* gstruct_new()
{
    gstruct *gs = (gstruct *)malloc(sizeof(gstruct));
    gs->type = -1;
    gstruct_buffer_init(gs->buffer);
    gstruct_buffer_write(gs->buffer, gs, sizeof(gs));
    return gs;
}

static void gstruct_free(gstruct* gs)
{
    if (gs->buffer) {
        free(gs->buffer);
    }
    free(gs);
}

static int gstruct_add_char(gstruct* gs, char d)
{
    gstruct g;
    g.type = GSTRUCT_TYPE_CHAR;
    g.via.chr = d;
    gstruct_buffer_write(gs->buffer, &g, sizeof(gstruct));
    return 0;
}

static int gstruct_add_int(gstruct* gs, int d)
{
    return 0;
}

static int gstruct_add_long(gstruct* gs, long d)
{
    return 0;
}

static int gstruct_add_float(gstruct* gs, float d)
{
    return 0;
}

static int gstruct_add_double(gstruct* gs, double d)
{
    return 0;
}

static int gstruct_add_nil(gstruct* gs)
{
    return 0;
}

static int gstruct_add_true(gstruct* gs)
{
    return 0;
}

static int gstruct_add_false(gstruct* gs)
{
    return 0;
}


static int gstruct_add_array(gstruct* gs, size_t n)
{
    return 0;
}

static int gstruct_add_map(gstruct* gs, size_t n)
{
    return 0;
}

static int gstruct_add_str(gstruct* gs, size_t l)
{
    return 0;
}

static int gstruct_add_str_body(gstruct* gs, const void* b, size_t l)
{
    return 0;
}

static int gstruct_add_bin(gstruct* gs, size_t l)
{
    return 0;
}

static int gstruct_add_bin_body(gstruct* gs, const void* b, size_t l)
{
    return 0;
}

static int gstruct_add_ext(gstruct* gs, size_t l, int8_t type)
{
    return 0;
}

static int gstruct_add_ext_body(gstruct* gs, const void* b, size_t l)
{
    return 0;
}

static int gstruct_apply_data(gstruct *gs)
{
    return 0;
}

