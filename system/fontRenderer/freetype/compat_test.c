#include <ft2build.h>
#include <freetype/ftbitmap.h>
#include <freetype/ftglyph.h>
#include FT_FREETYPE_H
int main() {
    FT_Library FTlib;
    if (FT_Init_FreeType(&FTlib)) {
        return 1;
    }
    return 0;
}
