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
    if (-1 == gs->type) {
        gs->type = GSTRUCT_TYPE_CHAR;
    }
}
