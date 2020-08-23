#include "rectangle.h"
#include <topaz/topaz.h>


int main() {
    topaz_t * ctx = topaz_context_create();
    topazEntity_t * e = rectangle_create(ctx);
    topaz_context_set_root(ctx, e);
    topaz_context_run(ctx);
}