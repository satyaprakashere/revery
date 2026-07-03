#include <stdio.h>

#include <ft2build.h>
#include FT_FREETYPE_H

int main() {
    FT_Library ft;
    if (!FT_Init_FreeType(&ft)) {
        printf("Freetype was initialized successfully!\n");
        return 0;
    } else {
        printf("Freetype could not be initialized.\n");
        return 1;
    }
}
