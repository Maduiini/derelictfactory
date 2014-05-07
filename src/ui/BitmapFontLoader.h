
#ifndef H_DER_BITMAP_FONT_LOADER_H
#define H_DER_BITMAP_FONT_LOADER_H

namespace der
{

    class BitmapFont;

    class BitmapFontLoader
    {
    public:
        static bool load(const char * const filepath, BitmapFont &font);

    };

} // der

#endif // H_DER_BITMAP_FONT_LOADER_H
