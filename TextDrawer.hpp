#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>
#include <hb-ft.h>

#include <cairo.h>
#include <cairo-ft.h>
struct TextDrawer {
    TextDrawer();
    ~TextDrawer();
    void draw_text(char *text, char *fontfile);
};
