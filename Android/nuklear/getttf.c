#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

int main() {
    FT_Library library;
    FT_Face face;

    if (FT_Init_FreeType(&library)) {
        fprintf(stderr, "Failed to initialize FreeType library\n");
        return 1;
    }

    if (FT_New_Face(library, "/system/fonts/DroidSansFallback.ttf", 0, &face)) {
        fprintf(stderr, "Failed to load font face\n");
        return 1;
    }

    FT_UInt char_index;
    FT_ULong charcode = FT_Get_First_Char(face, &char_index);

    while (char_index != 0) {
        // 使用UTF-8编码的格式化字符串打印中文字符
        printf("Character: U+%04X, Glyph Index: %d\n", (unsigned int)charcode, char_index);
        charcode = FT_Get_Next_Char(face, charcode, &char_index);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(library);

    return 0;
}
