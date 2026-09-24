/*******************************************************************************
 * Size: 18 px
 * Bpp: 1
 * Opts: --bpp 1 --size 18 --font /home/jarda/Documents/My/eink_driver/managed_components/lvgl__lvgl/tests/src/test_files/fonts/Montserrat-Bold.ttf -r 0x20-0x7F --symbols ÁáČčĎďÉéĚěÍíŇňÓóŘřŠšŤťÚúŮůÝýŽž --font /home/jarda/Documents/My/eink_driver/managed_components/lvgl__lvgl/scripts/built_in_font/FontAwesome5-Solid+Brands+Regular.woff -r 61931,62016,62017,62018,62019,62020,61779,61780,61559,61560 --format lvgl -o main/fonts/font_salina_bold_18.c --force-fast-kern-format --lv-include lvgl.h
 ******************************************************************************/

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
#include "lvgl.h"
#else
#include "lvgl.h"
#endif

#ifndef FONT_SALINA_BOLD_18
#define FONT_SALINA_BOLD_18 1
#endif

#if FONT_SALINA_BOLD_18

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xff, 0x3, 0xfe,

    /* U+0022 "\"" */
    0xcf, 0x3c, 0xf3, 0xcc,

    /* U+0023 "#" */
    0x18, 0xc1, 0x8c, 0x18, 0xcf, 0xff, 0xff, 0xf3,
    0x8c, 0x39, 0xc3, 0x1c, 0xff, 0xff, 0xff, 0x31,
    0x83, 0x18, 0x31, 0x80,

    /* U+0024 "$" */
    0xc, 0x3, 0x3, 0xf1, 0xfe, 0xed, 0xbb, 0xf,
    0xc3, 0xf8, 0x7f, 0x87, 0xf0, 0xfc, 0x37, 0xcd,
    0xff, 0xe7, 0xf0, 0x30, 0xc, 0x0,

    /* U+0025 "%" */
    0x78, 0x31, 0xf8, 0xe3, 0x31, 0x86, 0x66, 0xc,
    0xdc, 0x1f, 0xb0, 0x1e, 0xce, 0x1, 0xbe, 0x6,
    0xc6, 0x19, 0x8c, 0x33, 0x18, 0xc3, 0xe3, 0x3,
    0x80,

    /* U+0026 "&" */
    0x1e, 0x7, 0xf8, 0x73, 0x87, 0x38, 0x7f, 0x83,
    0xf0, 0x3e, 0x47, 0xf7, 0xe7, 0xee, 0x1e, 0xf1,
    0xe7, 0xff, 0x3e, 0x20,

    /* U+0027 "'" */
    0xff, 0xc0,

    /* U+0028 "(" */
    0x39, 0x9c, 0xee, 0x73, 0x9c, 0xe7, 0x39, 0xce,
    0x39, 0xc6, 0x38,

    /* U+0029 ")" */
    0xe3, 0x1c, 0xe3, 0x9c, 0xe7, 0x39, 0xce, 0x73,
    0xb9, 0xcc, 0xe0,

    /* U+002A "*" */
    0x18, 0x5a, 0xff, 0x3c, 0xff, 0x5a, 0x18,

    /* U+002B "+" */
    0x1c, 0xe, 0x7, 0x3, 0x8f, 0xff, 0xfc, 0x70,
    0x38, 0x1c, 0x0,

    /* U+002C "," */
    0xff, 0xad, 0x80,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xff, 0x80,

    /* U+002F "/" */
    0x3, 0x81, 0xc0, 0xc0, 0xe0, 0x70, 0x30, 0x38,
    0x18, 0xc, 0xe, 0x6, 0x3, 0x3, 0x81, 0x80,
    0xc0, 0xe0, 0x60, 0x30, 0x0,

    /* U+0030 "0" */
    0x1f, 0x7, 0xf1, 0xc7, 0x78, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x3d, 0xc7, 0x1f,
    0xc1, 0xf0,

    /* U+0031 "1" */
    0xff, 0xf1, 0xc7, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c,

    /* U+0032 "2" */
    0x3f, 0xf, 0xf1, 0x87, 0x0, 0xe0, 0x1c, 0x7,
    0x80, 0xe0, 0x38, 0xe, 0x3, 0x80, 0xe0, 0x3f,
    0xf7, 0xfe,

    /* U+0033 "3" */
    0x7f, 0xdf, 0xf0, 0x38, 0x1c, 0xe, 0x3, 0xc0,
    0xf8, 0xf, 0x1, 0xc0, 0x76, 0x3d, 0xfe, 0x3f,
    0x0,

    /* U+0034 "4" */
    0x3, 0x80, 0x70, 0xf, 0x0, 0xe0, 0x1c, 0x3,
    0x9c, 0x79, 0xc7, 0x1c, 0xff, 0xff, 0xff, 0x1,
    0xc0, 0x1c, 0x1, 0xc0,

    /* U+0035 "5" */
    0x7f, 0x9f, 0xe7, 0x1, 0xc0, 0x70, 0x1f, 0xc7,
    0xf8, 0xf, 0x1, 0xc0, 0x74, 0x3f, 0xfe, 0x7e,
    0x0,

    /* U+0036 "6" */
    0xf, 0x8f, 0xe7, 0x9, 0xc0, 0xe0, 0x3b, 0xcf,
    0xfb, 0xcf, 0xe1, 0xf8, 0x77, 0x3c, 0xfe, 0x1f,
    0x0,

    /* U+0037 "7" */
    0xff, 0xff, 0xfe, 0x1f, 0x8e, 0x3, 0x80, 0xe0,
    0x70, 0x1c, 0xe, 0x3, 0x80, 0xe0, 0x70, 0x1c,
    0x0,

    /* U+0038 "8" */
    0x3f, 0xf, 0xf3, 0x87, 0x70, 0xee, 0x1c, 0xff,
    0x1f, 0xf7, 0xdf, 0xe0, 0xfc, 0x1f, 0xc7, 0xbf,
    0xe1, 0xf0,

    /* U+0039 "9" */
    0x3e, 0x1f, 0xcf, 0x1b, 0x87, 0xe1, 0xfc, 0x77,
    0xfc, 0xff, 0x1, 0xc0, 0x60, 0x39, 0xfc, 0x7e,
    0x0,

    /* U+003A ":" */
    0xff, 0x80, 0x7, 0xfc,

    /* U+003B ";" */
    0xff, 0x80, 0x7, 0xfd, 0x6c,

    /* U+003C "<" */
    0x1, 0x83, 0xcf, 0xdf, 0x8e, 0x7, 0xc0, 0xfc,
    0x1f, 0x1, 0x80,

    /* U+003D "=" */
    0xff, 0xff, 0xc0, 0x0, 0xf, 0xff, 0xfc,

    /* U+003E ">" */
    0x80, 0x78, 0x3f, 0x3, 0xf0, 0x38, 0xfd, 0xf9,
    0xe0, 0xc0, 0x0,

    /* U+003F "?" */
    0x1f, 0x1f, 0xe7, 0x1c, 0x7, 0x1, 0xc0, 0xe0,
    0x70, 0x38, 0xe, 0x0, 0x0, 0xe0, 0x38, 0xe,
    0x0,

    /* U+0040 "@" */
    0x3, 0xf0, 0xf, 0xfe, 0xf, 0x7, 0x8e, 0x0,
    0xe6, 0x3d, 0xf7, 0x3f, 0xff, 0x3c, 0xf7, 0x9c,
    0x3b, 0xce, 0x1d, 0xe7, 0xe, 0xf3, 0xcf, 0x7c,
    0xff, 0xe6, 0x3c, 0xe1, 0x80, 0x0, 0xf0, 0x20,
    0x1f, 0xf0, 0x3, 0xf0, 0x0,

    /* U+0041 "A" */
    0x7, 0x80, 0x1e, 0x0, 0xf8, 0x3, 0xf0, 0x1d,
    0xc0, 0x73, 0x81, 0x8e, 0xe, 0x1c, 0x3f, 0xf1,
    0xff, 0xc7, 0x3, 0xb8, 0xe, 0xe0, 0x1c,

    /* U+0042 "B" */
    0xff, 0x8f, 0xfc, 0xe1, 0xee, 0xe, 0xe0, 0xee,
    0x1c, 0xff, 0xcf, 0xfe, 0xe0, 0xfe, 0x7, 0xe0,
    0x7f, 0xfe, 0xff, 0xc0,

    /* U+0043 "C" */
    0xf, 0xc3, 0xff, 0x78, 0x67, 0x0, 0xe0, 0xe,
    0x0, 0xe0, 0xe, 0x0, 0xe0, 0x7, 0x0, 0x78,
    0x63, 0xff, 0xf, 0xc0,

    /* U+0044 "D" */
    0xff, 0x87, 0xff, 0x38, 0x3d, 0xc0, 0xee, 0x3,
    0xf0, 0x1f, 0x80, 0xfc, 0x7, 0xe0, 0x3f, 0x3,
    0xb8, 0x3d, 0xff, 0xcf, 0xf8, 0x0,

    /* U+0045 "E" */
    0xff, 0xff, 0xfe, 0x3, 0x80, 0xe0, 0x3f, 0xef,
    0xfb, 0x80, 0xe0, 0x38, 0xe, 0x3, 0xff, 0xff,
    0xc0,

    /* U+0046 "F" */
    0xff, 0xff, 0xfe, 0x3, 0x80, 0xe0, 0x38, 0xf,
    0xfb, 0xfe, 0xe0, 0x38, 0xe, 0x3, 0x80, 0xe0,
    0x0,

    /* U+0047 "G" */
    0xf, 0xc3, 0xff, 0x78, 0x67, 0x0, 0xe0, 0xe,
    0x0, 0xe0, 0x7e, 0x7, 0xe0, 0x77, 0x7, 0x78,
    0x73, 0xff, 0xf, 0xc0,

    /* U+0048 "H" */
    0xe0, 0x7e, 0x7, 0xe0, 0x7e, 0x7, 0xe0, 0x7f,
    0xff, 0xff, 0xfe, 0x7, 0xe0, 0x7e, 0x7, 0xe0,
    0x7e, 0x7, 0xe0, 0x70,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0xfe,

    /* U+004A "J" */
    0x7f, 0xbf, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0xe,
    0x7, 0x3, 0x81, 0xd9, 0xff, 0xe3, 0xe0,

    /* U+004B "K" */
    0xe0, 0xee, 0x1c, 0xe3, 0xce, 0x78, 0xef, 0xe,
    0xe0, 0xfe, 0xf, 0xf0, 0xf7, 0x8e, 0x38, 0xe1,
    0xce, 0xe, 0xe0, 0xf0,

    /* U+004C "L" */
    0xe0, 0x70, 0x38, 0x1c, 0xe, 0x7, 0x3, 0x81,
    0xc0, 0xe0, 0x70, 0x38, 0x1f, 0xff, 0xf8,

    /* U+004D "M" */
    0xe0, 0x1f, 0x80, 0x7f, 0x3, 0xfc, 0xf, 0xf8,
    0x7f, 0xf1, 0xff, 0xcf, 0xfb, 0xf7, 0xef, 0xdf,
    0x9e, 0x7e, 0x31, 0xf8, 0xc7, 0xe0, 0x1c,

    /* U+004E "N" */
    0xe0, 0x7f, 0x7, 0xf8, 0x7f, 0xc7, 0xfc, 0x7f,
    0xe7, 0xef, 0x7e, 0x7f, 0xe3, 0xfe, 0x1f, 0xe1,
    0xfe, 0xf, 0xe0, 0x70,

    /* U+004F "O" */
    0xf, 0xc0, 0xff, 0xc7, 0x87, 0x9c, 0xe, 0xe0,
    0x1f, 0x80, 0x7e, 0x1, 0xf8, 0x7, 0xe0, 0x1d,
    0xc0, 0xe7, 0x87, 0x8f, 0xfc, 0xf, 0xc0,

    /* U+0050 "P" */
    0xff, 0x1f, 0xfb, 0x87, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x7f, 0xfe, 0xff, 0x1c, 0x3, 0x80, 0x70,
    0xe, 0x0,

    /* U+0051 "Q" */
    0xf, 0xc0, 0x7f, 0xe1, 0xe1, 0xe3, 0x81, 0xce,
    0x1, 0xdc, 0x3, 0xb8, 0x7, 0x70, 0xe, 0xe0,
    0x1c, 0xe0, 0x71, 0xe1, 0xe1, 0xff, 0x80, 0xfc,
    0x0, 0x78, 0x80, 0x7f, 0x80, 0x3c,

    /* U+0052 "R" */
    0xff, 0x1f, 0xfb, 0x87, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x7f, 0xfe, 0xff, 0x1c, 0x73, 0x8f, 0x70,
    0xee, 0xe,

    /* U+0053 "S" */
    0x3f, 0x1f, 0xee, 0x1b, 0x80, 0xf0, 0x3f, 0x87,
    0xf8, 0x7f, 0x3, 0xc0, 0x7c, 0x1f, 0xfe, 0x7f,
    0x0,

    /* U+0054 "T" */
    0xff, 0xff, 0xfc, 0x38, 0x7, 0x0, 0xe0, 0x1c,
    0x3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0,

    /* U+0055 "U" */
    0xe0, 0xfc, 0x1f, 0x83, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1d, 0xc7, 0x3f,
    0xe1, 0xf0,

    /* U+0056 "V" */
    0xe0, 0x3b, 0xc0, 0xe7, 0x3, 0x9c, 0x1c, 0x38,
    0x70, 0xe3, 0x81, 0xce, 0x7, 0x30, 0x1d, 0xc0,
    0x3f, 0x0, 0xf8, 0x1, 0xe0, 0x7, 0x0,

    /* U+0057 "W" */
    0x70, 0x70, 0x33, 0x83, 0x83, 0x9c, 0x3e, 0x1c,
    0xf1, 0xf0, 0xc3, 0x8d, 0x8e, 0x1c, 0xee, 0x70,
    0xe7, 0x73, 0x3, 0xb1, 0xb8, 0x1f, 0x8f, 0xc0,
    0xfc, 0x7e, 0x3, 0xc3, 0xe0, 0x1e, 0xf, 0x0,
    0xf0, 0x78, 0x0,

    /* U+0058 "X" */
    0xe0, 0x77, 0xe, 0x39, 0xc3, 0xdc, 0x1f, 0x80,
    0xf0, 0xf, 0x1, 0xf8, 0x1f, 0x83, 0x9c, 0x79,
    0xe7, 0xe, 0xe0, 0x70,

    /* U+0059 "Y" */
    0xf0, 0x3b, 0x83, 0x8e, 0x18, 0x71, 0xc1, 0xdc,
    0xe, 0xe0, 0x3e, 0x0, 0xf0, 0x7, 0x0, 0x38,
    0x1, 0xc0, 0xe, 0x0, 0x70, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xfc, 0x7, 0x1, 0xe0, 0x78, 0xe,
    0x3, 0x80, 0xe0, 0x3c, 0xf, 0x1, 0xc0, 0x7f,
    0xff, 0xfe,

    /* U+005B "[" */
    0xff, 0xf9, 0xce, 0x73, 0x9c, 0xe7, 0x39, 0xce,
    0x73, 0x9f, 0xf8,

    /* U+005C "\\" */
    0xe0, 0x30, 0x18, 0xe, 0x3, 0x1, 0x80, 0xe0,
    0x30, 0x18, 0xe, 0x3, 0x1, 0x80, 0xe0, 0x30,
    0x18, 0xe, 0x3, 0x1, 0xc0,

    /* U+005D "]" */
    0xff, 0xce, 0x73, 0x9c, 0xe7, 0x39, 0xce, 0x73,
    0x9c, 0xff, 0xf8,

    /* U+005E "^" */
    0x1c, 0xe, 0xf, 0x6, 0xc3, 0x63, 0x19, 0x8d,
    0x87,

    /* U+005F "_" */
    0xff, 0xff, 0xc0,

    /* U+0060 "`" */
    0xe3, 0x8e,

    /* U+0061 "a" */
    0x3e, 0x3f, 0x91, 0xe0, 0x77, 0xff, 0xff, 0x8f,
    0xcf, 0xff, 0xbd, 0xc0,

    /* U+0062 "b" */
    0xe0, 0x1c, 0x3, 0x80, 0x77, 0x8f, 0xfd, 0xe3,
    0xb8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0xc7, 0x7f,
    0xee, 0xf0,

    /* U+0063 "c" */
    0x1f, 0x1f, 0xe7, 0x1b, 0x80, 0xe0, 0x38, 0xe,
    0x1, 0xc6, 0x7f, 0x87, 0xc0,

    /* U+0064 "d" */
    0x0, 0xe0, 0x1c, 0x3, 0x8f, 0x77, 0xfe, 0xe3,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1d, 0xc7, 0xbf,
    0xf1, 0xee,

    /* U+0065 "e" */
    0x1e, 0x1f, 0xe7, 0x3b, 0x87, 0xff, 0xff, 0xfe,
    0x1, 0xc4, 0x7f, 0x87, 0xc0,

    /* U+0066 "f" */
    0xf, 0x1e, 0x38, 0x38, 0xfe, 0xfe, 0x38, 0x38,
    0x38, 0x38, 0x38, 0x38, 0x38, 0x38,

    /* U+0067 "g" */
    0x1e, 0xef, 0xfd, 0xc7, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3b, 0x8f, 0x7f, 0xe3, 0xdc, 0x3, 0xb0,
    0xe7, 0xfc, 0x7e, 0x0,

    /* U+0068 "h" */
    0xe0, 0x38, 0xe, 0x3, 0xbc, 0xff, 0xbc, 0xfe,
    0x1f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0x87, 0xe1,
    0xc0,

    /* U+0069 "i" */
    0xff, 0x8f, 0xff, 0xff, 0xff, 0xc0,

    /* U+006A "j" */
    0x1c, 0x71, 0xc0, 0x1c, 0x71, 0xc7, 0x1c, 0x71,
    0xc7, 0x1c, 0x71, 0xc7, 0x7b, 0xc0,

    /* U+006B "k" */
    0xe0, 0x1c, 0x3, 0x80, 0x70, 0xee, 0x39, 0xde,
    0x3f, 0x87, 0xf0, 0xfe, 0x1e, 0xe3, 0x9e, 0x71,
    0xee, 0x1c,

    /* U+006C "l" */
    0xff, 0xff, 0xff, 0xff, 0xfe,

    /* U+006D "m" */
    0xef, 0x3e, 0x7f, 0xff, 0xbc, 0xf9, 0xfc, 0x38,
    0x7e, 0x1c, 0x3f, 0xe, 0x1f, 0x87, 0xf, 0xc3,
    0x87, 0xe1, 0xc3, 0xf0, 0xe1, 0xc0,

    /* U+006E "n" */
    0xef, 0x3f, 0xef, 0x3f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0x87, 0xe1, 0xf8, 0x70,

    /* U+006F "o" */
    0x1f, 0xf, 0xf9, 0xc7, 0x70, 0x7e, 0xf, 0xc1,
    0xf8, 0x3b, 0x8e, 0x7f, 0xc3, 0xe0,

    /* U+0070 "p" */
    0xef, 0x1f, 0xfb, 0xc7, 0x70, 0x7e, 0xf, 0xc1,
    0xf8, 0x3f, 0x8e, 0xff, 0xdd, 0xe3, 0x80, 0x70,
    0xe, 0x1, 0xc0, 0x0,

    /* U+0071 "q" */
    0x1e, 0xef, 0xfd, 0xc7, 0xf0, 0x7e, 0xf, 0xc1,
    0xf8, 0x3b, 0x8f, 0x7f, 0xe3, 0xdc, 0x3, 0x80,
    0x70, 0xe, 0x1, 0xc0,

    /* U+0072 "r" */
    0xef, 0xff, 0x38, 0xe3, 0x8e, 0x38, 0xe3, 0x80,

    /* U+0073 "s" */
    0x3f, 0x3f, 0xb8, 0x5c, 0xf, 0xe3, 0xfc, 0x1e,
    0x87, 0xff, 0x3f, 0x0,

    /* U+0074 "t" */
    0x38, 0x38, 0xfe, 0xfe, 0x38, 0x38, 0x38, 0x38,
    0x38, 0x38, 0x3e, 0x1f,

    /* U+0075 "u" */
    0xe1, 0xf8, 0x7e, 0x1f, 0x87, 0xe1, 0xf8, 0x7e,
    0x1f, 0xcf, 0x7f, 0xcf, 0x70,

    /* U+0076 "v" */
    0xe0, 0xfc, 0x19, 0xc7, 0x38, 0xc3, 0xb8, 0x77,
    0xe, 0xc0, 0xf8, 0x1e, 0x1, 0xc0,

    /* U+0077 "w" */
    0xe1, 0xc3, 0xb0, 0xe1, 0x9c, 0xf9, 0xce, 0x7c,
    0xe3, 0x36, 0x61, 0xfb, 0xf0, 0xf9, 0xf8, 0x3c,
    0x78, 0x1e, 0x3c, 0xe, 0x1e, 0x0,

    /* U+0078 "x" */
    0xe1, 0xdc, 0xe3, 0xf0, 0xfc, 0x1e, 0x7, 0x83,
    0xf0, 0xfc, 0x73, 0xb8, 0x70,

    /* U+0079 "y" */
    0xe0, 0xfc, 0x19, 0xc7, 0x38, 0xe3, 0x38, 0x77,
    0xe, 0xc0, 0xf8, 0x1e, 0x1, 0xc0, 0x38, 0x6,
    0xf, 0xc1, 0xf0, 0x0,

    /* U+007A "z" */
    0xff, 0xff, 0xe, 0xe, 0x1c, 0x38, 0x70, 0x70,
    0xff, 0xff,

    /* U+007B "{" */
    0x1e, 0x7c, 0xe1, 0xc3, 0x87, 0xe, 0x7c, 0xf0,
    0x70, 0xe1, 0xc3, 0x87, 0xe, 0x1f, 0x1e,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0,

    /* U+007D "}" */
    0xf3, 0xe3, 0x8e, 0x38, 0xe3, 0x8f, 0x1c, 0xe3,
    0x8e, 0x38, 0xe3, 0xbe, 0xf0,

    /* U+007E "~" */
    0x71, 0xfc, 0xf3, 0xf8, 0xe0,

    /* U+00C1 "Á" */
    0x0, 0xe0, 0x6, 0x0, 0x30, 0x0, 0x0, 0x7,
    0x80, 0x1e, 0x0, 0xf8, 0x3, 0xf0, 0x1d, 0xc0,
    0x73, 0x81, 0x8e, 0xe, 0x1c, 0x3f, 0xf1, 0xff,
    0xc7, 0x3, 0xb8, 0xe, 0xe0, 0x1c,

    /* U+00C9 "É" */
    0x7, 0x1, 0x80, 0xc0, 0x0, 0xff, 0xff, 0xfe,
    0x3, 0x80, 0xe0, 0x3f, 0xef, 0xfb, 0x80, 0xe0,
    0x38, 0xe, 0x3, 0xff, 0xff, 0xc0,

    /* U+00CD "Í" */
    0x3b, 0xb8, 0xe, 0x73, 0x9c, 0xe7, 0x39, 0xce,
    0x73, 0x9c, 0xe0,

    /* U+00D3 "Ó" */
    0x0, 0xe0, 0x7, 0x0, 0x30, 0x0, 0x0, 0xf,
    0xc0, 0xff, 0xc7, 0x87, 0x9c, 0xe, 0xe0, 0x1f,
    0x80, 0x7e, 0x1, 0xf8, 0x7, 0xe0, 0x1d, 0xc0,
    0xe7, 0x87, 0x8f, 0xfc, 0xf, 0xc0,

    /* U+00DA "Ú" */
    0x3, 0x80, 0xe0, 0x38, 0x0, 0xe, 0xf, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xdc, 0x73, 0xfe, 0x1f, 0x0,

    /* U+00DD "Ý" */
    0x1, 0xc0, 0x1c, 0x1, 0xc0, 0x0, 0xf, 0x3,
    0xb8, 0x38, 0xe3, 0x87, 0x1c, 0x1d, 0xc0, 0xfe,
    0x3, 0xe0, 0xe, 0x0, 0x70, 0x3, 0x80, 0x1c,
    0x0, 0xe0, 0x7, 0x0,

    /* U+00E1 "á" */
    0x7, 0x7, 0x3, 0x0, 0x3, 0xe3, 0xf9, 0x1e,
    0x7, 0x7f, 0xff, 0xf8, 0xfc, 0xff, 0xfb, 0xdc,

    /* U+00E9 "é" */
    0x7, 0x3, 0x80, 0x80, 0x0, 0x1e, 0x1f, 0xe7,
    0x3b, 0x87, 0xff, 0xff, 0xfe, 0x1, 0xc4, 0x7f,
    0x87, 0xc0,

    /* U+00ED "í" */
    0x3b, 0xb8, 0xe, 0x73, 0x9c, 0xe7, 0x39, 0xce,
    0x70,

    /* U+00F3 "ó" */
    0x3, 0x80, 0xc0, 0x30, 0x0, 0x1, 0xf0, 0xff,
    0x9c, 0x77, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xb8,
    0xe7, 0xfc, 0x3e, 0x0,

    /* U+00FA "ú" */
    0x7, 0x3, 0x81, 0xc0, 0x0, 0xe1, 0xf8, 0x7e,
    0x1f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0xcf, 0x7f,
    0xcf, 0x70,

    /* U+00FD "ý" */
    0x3, 0x80, 0xe0, 0x30, 0x0, 0xe, 0xf, 0xc1,
    0x9c, 0x73, 0x8e, 0x33, 0x87, 0x70, 0xec, 0xf,
    0x81, 0xe0, 0x1c, 0x3, 0x80, 0x60, 0xfc, 0x1f,
    0x0,

    /* U+010C "Č" */
    0x18, 0xc0, 0xd8, 0x7, 0x80, 0x0, 0xf, 0xc3,
    0xff, 0x78, 0x67, 0x0, 0xe0, 0xe, 0x0, 0xe0,
    0xe, 0x0, 0xe0, 0x7, 0x0, 0x78, 0x63, 0xff,
    0xf, 0xc0,

    /* U+010D "č" */
    0x33, 0x7, 0x80, 0xc0, 0x0, 0x1f, 0x1f, 0xe7,
    0x1b, 0x80, 0xe0, 0x38, 0xe, 0x1, 0xc6, 0x7f,
    0x87, 0xc0,

    /* U+010E "Ď" */
    0x31, 0x80, 0xd8, 0x3, 0x80, 0x0, 0xf, 0xf8,
    0x7f, 0xf3, 0x83, 0xdc, 0xe, 0xe0, 0x3f, 0x1,
    0xf8, 0xf, 0xc0, 0x7e, 0x3, 0xf0, 0x3b, 0x83,
    0xdf, 0xfc, 0xff, 0x80,

    /* U+010F "ď" */
    0x0, 0xc, 0x3, 0xb0, 0xe, 0xc0, 0x38, 0x1e,
    0xe1, 0xff, 0x87, 0x1e, 0x38, 0x38, 0xe0, 0xe3,
    0x83, 0x8e, 0xe, 0x1c, 0x78, 0x7f, 0xe0, 0x7b,
    0x80,

    /* U+011A "Ě" */
    0x63, 0xd, 0x81, 0xc0, 0x0, 0xff, 0xff, 0xfe,
    0x3, 0x80, 0xe0, 0x3f, 0xef, 0xfb, 0x80, 0xe0,
    0x38, 0xe, 0x3, 0xff, 0xff, 0xc0,

    /* U+011B "ě" */
    0x33, 0x7, 0x80, 0xc0, 0x0, 0x1e, 0x1f, 0xe7,
    0x3b, 0x87, 0xff, 0xff, 0xfe, 0x1, 0xc4, 0x7f,
    0x87, 0xc0,

    /* U+0147 "Ň" */
    0x30, 0xc1, 0xf8, 0xf, 0x0, 0x0, 0xe0, 0x7f,
    0x7, 0xf8, 0x7f, 0xc7, 0xfc, 0x7f, 0xe7, 0xef,
    0x7e, 0x7f, 0xe3, 0xfe, 0x1f, 0xe1, 0xfe, 0xf,
    0xe0, 0x70,

    /* U+0148 "ň" */
    0x33, 0x7, 0x80, 0xc0, 0x0, 0xef, 0x3f, 0xef,
    0x3f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0x87, 0xe1,
    0xf8, 0x70,

    /* U+0158 "Ř" */
    0x31, 0x83, 0x60, 0x38, 0x0, 0xf, 0xf1, 0xff,
    0xb8, 0x7f, 0x7, 0xe0, 0xfc, 0x1f, 0x87, 0xff,
    0xef, 0xf1, 0xc7, 0x38, 0xf7, 0xe, 0xe0, 0xe0,

    /* U+0159 "ř" */
    0x66, 0x3e, 0x1c, 0x0, 0x76, 0x7e, 0x78, 0x70,
    0x70, 0x70, 0x70, 0x70, 0x70, 0x70,

    /* U+0160 "Š" */
    0x33, 0xf, 0xc1, 0xe0, 0x0, 0x3f, 0x1f, 0xee,
    0x1b, 0x80, 0xf0, 0x3f, 0x87, 0xf8, 0x7f, 0x3,
    0xc0, 0x7c, 0x1f, 0xfe, 0x7f, 0x0,

    /* U+0161 "š" */
    0x67, 0x1f, 0x7, 0x0, 0x3, 0xf3, 0xfb, 0x85,
    0xc0, 0xfe, 0x3f, 0xc1, 0xe8, 0x7f, 0xf3, 0xf0,

    /* U+0164 "Ť" */
    0x31, 0x83, 0x60, 0x38, 0x0, 0xf, 0xff, 0xff,
    0xc3, 0x80, 0x70, 0xe, 0x1, 0xc0, 0x38, 0x7,
    0x0, 0xe0, 0x1c, 0x3, 0x80, 0x70, 0xe, 0x0,

    /* U+0165 "ť" */
    0x3, 0x3, 0x3b, 0x38, 0xfe, 0xfe, 0x38, 0x38,
    0x38, 0x38, 0x38, 0x38, 0x3e, 0x1f,

    /* U+016E "Ů" */
    0x6, 0x1, 0x20, 0x24, 0x3, 0x0, 0x1, 0xc1,
    0xf8, 0x3f, 0x7, 0xe0, 0xfc, 0x1f, 0x83, 0xf0,
    0x7e, 0xf, 0xc1, 0xf8, 0x3b, 0x8e, 0x7f, 0xc3,
    0xe0,

    /* U+016F "ů" */
    0xc, 0x4, 0x81, 0x20, 0x30, 0x0, 0x38, 0x7e,
    0x1f, 0x87, 0xe1, 0xf8, 0x7e, 0x1f, 0x87, 0xf3,
    0xdf, 0xf3, 0xdc,

    /* U+017D "Ž" */
    0x31, 0x83, 0x60, 0x38, 0x0, 0xf, 0xff, 0xff,
    0xc0, 0x70, 0x1e, 0x7, 0x80, 0xe0, 0x38, 0xe,
    0x3, 0xc0, 0xf0, 0x1c, 0x7, 0xff, 0xff, 0xe0,

    /* U+017E "ž" */
    0x66, 0x3c, 0x1c, 0x0, 0xff, 0xff, 0xe, 0xe,
    0x1c, 0x38, 0x70, 0x70, 0xff, 0xff,

    /* U+F077 "" */
    0x3, 0x80, 0xf, 0x80, 0x3f, 0x80, 0xf7, 0x83,
    0xc7, 0x8f, 0x7, 0xbc, 0x7, 0xf0, 0x7, 0x40,
    0x4,

    /* U+F078 "" */
    0xe0, 0xf, 0xe0, 0x3d, 0xe0, 0xf1, 0xe3, 0xc1,
    0xef, 0x1, 0xfc, 0x1, 0xf0, 0x1, 0xc0, 0x1,
    0x0,

    /* U+F153 "" */
    0x3, 0xe1, 0xfc, 0x70, 0x1c, 0xf, 0xfd, 0xff,
    0x9c, 0x3, 0x80, 0xff, 0x9f, 0xf0, 0xe0, 0x1c,
    0x1, 0xe0, 0x1f, 0xc0, 0xf8,

    /* U+F154 "" */
    0xf, 0x87, 0xf8, 0xe2, 0x3c, 0x7, 0x0, 0xe0,
    0x3f, 0xc7, 0xf8, 0x70, 0xe, 0x1, 0xc0, 0x38,
    0x37, 0x7, 0xff, 0xff, 0xf8,

    /* U+F1EB "" */
    0x0, 0x7c, 0x0, 0xf, 0xff, 0x80, 0x7f, 0xff,
    0xc1, 0xfc, 0x1f, 0xcf, 0x80, 0x7, 0xfe, 0x0,
    0x3, 0xd0, 0x3f, 0x81, 0x1, 0xff, 0xc0, 0x7,
    0xff, 0xc0, 0x1f, 0x7, 0xc0, 0x18, 0x3, 0x0,
    0x0, 0x0, 0x0, 0x3, 0x80, 0x0, 0xf, 0x80,
    0x0, 0x1f, 0x0, 0x0, 0x1c, 0x0,

    /* U+F240 "" */
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xec, 0x0, 0x1,
    0xf7, 0xff, 0xf7, 0xdf, 0xff, 0xcf, 0x7f, 0xff,
    0x3d, 0xff, 0xfc, 0xf0, 0x0, 0x7, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0xef, 0xff, 0xff, 0x80,

    /* U+F241 "" */
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xec, 0x0, 0x1,
    0xf7, 0xff, 0x7, 0xdf, 0xfc, 0xf, 0x7f, 0xf0,
    0x3d, 0xff, 0xc0, 0xf0, 0x0, 0x7, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0xef, 0xff, 0xff, 0x80,

    /* U+F242 "" */
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xec, 0x0, 0x1,
    0xf7, 0xf8, 0x7, 0xdf, 0xe0, 0xf, 0x7f, 0x80,
    0x3d, 0xfe, 0x0, 0xf0, 0x0, 0x7, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0xef, 0xff, 0xff, 0x80,

    /* U+F243 "" */
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xec, 0x0, 0x1,
    0xf7, 0xc0, 0x7, 0xdf, 0x0, 0xf, 0x7c, 0x0,
    0x3d, 0xf0, 0x0, 0xf0, 0x0, 0x7, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0xef, 0xff, 0xff, 0x80,

    /* U+F244 "" */
    0xff, 0xff, 0xfb, 0xff, 0xff, 0xec, 0x0, 0x1,
    0xf0, 0x0, 0x7, 0xc0, 0x0, 0xf, 0x0, 0x0,
    0x3c, 0x0, 0x0, 0xf0, 0x0, 0x7, 0xc0, 0x0,
    0x1f, 0xff, 0xff, 0xef, 0xff, 0xff, 0x80
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 82, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 83, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 126, .box_w = 6, .box_h = 5, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 10, .adv_w = 207, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 30, .adv_w = 184, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 52, .adv_w = 253, .box_w = 15, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 210, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 97, .adv_w = 66, .box_w = 2, .box_h = 5, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 99, .adv_w = 103, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 110, .adv_w = 103, .box_w = 5, .box_h = 17, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 121, .adv_w = 125, .box_w = 8, .box_h = 7, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 128, .adv_w = 173, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 139, .adv_w = 75, .box_w = 3, .box_h = 6, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 142, .adv_w = 111, .box_w = 5, .box_h = 2, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 144, .adv_w = 75, .box_w = 3, .box_h = 3, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 146, .adv_w = 113, .box_w = 9, .box_h = 18, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 167, .adv_w = 196, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 113, .box_w = 6, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 170, .box_w = 11, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 213, .adv_w = 170, .box_w = 10, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 230, .adv_w = 198, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 250, .adv_w = 171, .box_w = 10, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 267, .adv_w = 183, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 284, .adv_w = 179, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 301, .adv_w = 190, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 319, .adv_w = 183, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 336, .adv_w = 75, .box_w = 3, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 340, .adv_w = 75, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 345, .adv_w = 173, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 356, .adv_w = 173, .box_w = 9, .box_h = 6, .ofs_x = 1, .ofs_y = 4},
    {.bitmap_index = 363, .adv_w = 173, .box_w = 9, .box_h = 9, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 374, .adv_w = 170, .box_w = 10, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 391, .adv_w = 298, .box_w = 17, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 428, .adv_w = 221, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 451, .adv_w = 220, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 471, .adv_w = 211, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 491, .adv_w = 238, .box_w = 13, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 513, .adv_w = 193, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 530, .adv_w = 184, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 547, .adv_w = 222, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 567, .adv_w = 233, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 587, .adv_w = 94, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 592, .adv_w = 156, .box_w = 9, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 213, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 627, .adv_w = 174, .box_w = 9, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 275, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 665, .adv_w = 233, .box_w = 12, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 243, .box_w = 14, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 708, .adv_w = 211, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 726, .adv_w = 243, .box_w = 15, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 756, .adv_w = 212, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 774, .adv_w = 184, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 791, .adv_w = 178, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 809, .adv_w = 227, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 827, .adv_w = 215, .box_w = 14, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 850, .adv_w = 335, .box_w = 21, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 885, .adv_w = 206, .box_w = 12, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 905, .adv_w = 195, .box_w = 13, .box_h = 13, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 927, .adv_w = 193, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 945, .adv_w = 106, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 956, .adv_w = 113, .box_w = 9, .box_h = 18, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 977, .adv_w = 106, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 988, .adv_w = 173, .box_w = 9, .box_h = 8, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 997, .adv_w = 144, .box_w = 9, .box_h = 2, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 1000, .adv_w = 173, .box_w = 5, .box_h = 3, .ofs_x = 2, .ofs_y = 11},
    {.bitmap_index = 1002, .adv_w = 178, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1014, .adv_w = 199, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1032, .adv_w = 170, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1045, .adv_w = 199, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1063, .adv_w = 182, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1076, .adv_w = 111, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1090, .adv_w = 202, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1110, .adv_w = 199, .box_w = 10, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 87, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1133, .adv_w = 88, .box_w = 6, .box_h = 18, .ofs_x = -2, .ofs_y = -4},
    {.bitmap_index = 1147, .adv_w = 190, .box_w = 11, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1165, .adv_w = 87, .box_w = 3, .box_h = 13, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1170, .adv_w = 302, .box_w = 17, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1192, .adv_w = 199, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1205, .adv_w = 189, .box_w = 11, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1219, .adv_w = 199, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1239, .adv_w = 199, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1259, .adv_w = 124, .box_w = 6, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1267, .adv_w = 153, .box_w = 9, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1279, .adv_w = 125, .box_w = 8, .box_h = 12, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1291, .adv_w = 198, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1304, .adv_w = 172, .box_w = 11, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1318, .adv_w = 270, .box_w = 17, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1340, .adv_w = 171, .box_w = 10, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1353, .adv_w = 172, .box_w = 11, .box_h = 14, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 1373, .adv_w = 156, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1383, .adv_w = 113, .box_w = 7, .box_h = 17, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 1398, .adv_w = 89, .box_w = 3, .box_h = 17, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1405, .adv_w = 113, .box_w = 6, .box_h = 17, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 1418, .adv_w = 173, .box_w = 9, .box_h = 4, .ofs_x = 1, .ofs_y = 5},
    {.bitmap_index = 1423, .adv_w = 221, .box_w = 14, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1453, .adv_w = 193, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1475, .adv_w = 94, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1486, .adv_w = 243, .box_w = 14, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1516, .adv_w = 227, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1540, .adv_w = 195, .box_w = 13, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1568, .adv_w = 178, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1584, .adv_w = 182, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1602, .adv_w = 87, .box_w = 5, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1611, .adv_w = 189, .box_w = 11, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1631, .adv_w = 198, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1649, .adv_w = 172, .box_w = 11, .box_h = 18, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 1674, .adv_w = 211, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1700, .adv_w = 170, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1718, .adv_w = 238, .box_w = 13, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1746, .adv_w = 199, .box_w = 14, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1771, .adv_w = 193, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1793, .adv_w = 182, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1811, .adv_w = 233, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1837, .adv_w = 199, .box_w = 10, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1855, .adv_w = 212, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1879, .adv_w = 124, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1893, .adv_w = 184, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1915, .adv_w = 153, .box_w = 9, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1931, .adv_w = 178, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1955, .adv_w = 125, .box_w = 8, .box_h = 14, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1969, .adv_w = 227, .box_w = 11, .box_h = 18, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1994, .adv_w = 198, .box_w = 10, .box_h = 15, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2013, .adv_w = 193, .box_w = 11, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2037, .adv_w = 156, .box_w = 8, .box_h = 14, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 2051, .adv_w = 252, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 2068, .adv_w = 252, .box_w = 15, .box_h = 9, .ofs_x = 0, .ofs_y = 2},
    {.bitmap_index = 2085, .adv_w = 180, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 2106, .adv_w = 180, .box_w = 11, .box_h = 15, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 2127, .adv_w = 360, .box_w = 23, .box_h = 16, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 2173, .adv_w = 360, .box_w = 22, .box_h = 11, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 2204, .adv_w = 360, .box_w = 22, .box_h = 11, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 2235, .adv_w = 360, .box_w = 22, .box_h = 11, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 2266, .adv_w = 360, .box_w = 22, .box_h = 11, .ofs_x = 0, .ofs_y = 1},
    {.bitmap_index = 2297, .adv_w = 360, .box_w = 22, .box_h = 11, .ofs_x = 0, .ofs_y = 1}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_1[] = {
    0x0, 0x8, 0xc, 0x12, 0x19, 0x1c, 0x20, 0x28,
    0x2c, 0x32, 0x39, 0x3c, 0x4b, 0x4c, 0x4d, 0x4e,
    0x59, 0x5a, 0x86, 0x87, 0x97, 0x98, 0x9f, 0xa0,
    0xa3, 0xa4, 0xad, 0xae, 0xbc, 0xbd, 0xefb6, 0xefb7,
    0xf092, 0xf093, 0xf12a, 0xf17f, 0xf180, 0xf181, 0xf182, 0xf183
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 193, .range_length = 61828, .glyph_id_start = 96,
        .unicode_list = unicode_list_1, .glyph_id_ofs_list = NULL, .list_length = 40, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 0, 13, 14, 15, 16, 17,
    18, 19, 12, 20, 20, 0, 0, 0,
    21, 22, 23, 24, 25, 22, 26, 27,
    28, 29, 29, 30, 31, 32, 29, 29,
    22, 33, 34, 35, 3, 36, 30, 37,
    37, 38, 39, 40, 41, 42, 43, 0,
    44, 0, 45, 46, 47, 48, 49, 50,
    51, 45, 52, 52, 53, 48, 45, 45,
    46, 46, 54, 55, 56, 57, 51, 58,
    58, 59, 58, 60, 41, 0, 0, 9,
    23, 26, 29, 22, 30, 39, 45, 49,
    51, 46, 51, 58, 25, 47, 22, 61,
    26, 49, 29, 45, 35, 55, 3, 56,
    36, 62, 30, 51, 40, 60, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 1, 2, 0, 3, 4, 5,
    2, 6, 7, 8, 9, 10, 9, 10,
    11, 12, 13, 14, 15, 16, 17, 12,
    18, 19, 20, 21, 21, 0, 0, 0,
    22, 23, 24, 25, 23, 25, 25, 25,
    23, 25, 25, 26, 25, 25, 25, 25,
    23, 25, 23, 25, 3, 27, 28, 29,
    29, 30, 31, 32, 33, 34, 35, 0,
    36, 0, 37, 38, 39, 39, 39, 40,
    39, 38, 41, 42, 38, 38, 43, 43,
    39, 43, 39, 43, 44, 45, 46, 47,
    47, 48, 49, 50, 0, 0, 35, 9,
    24, 25, 25, 23, 28, 31, 37, 39,
    43, 39, 46, 47, 23, 39, 25, 39,
    25, 39, 25, 43, 25, 43, 3, 44,
    27, 45, 28, 46, 32, 50, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, 1, 0, 0, 0, 0, 0, 2,
    0, 1, 0, 0, 3, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 0,
    0, 6, 6, 0, 3, 0, 6, 0,
    0, 0, 1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 13, 0, 7, -5, 0,
    0, 6, 0, -16, -17, 1, 13, 5,
    4, -12, 1, 12, 0, 11, 3, 8,
    0, -12, 0, 0, 4, 0, 0, 0,
    0, 0, 0, 17, 4, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, 0, 0,
    0, 0, 0, -6, 4, 6, 0, 0,
    -3, 0, -1, 3, 0, -3, 0, -3,
    -1, -6, 0, 0, 0, 0, -3, 0,
    0, -3, -4, 0, 0, -3, 0, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -3, -3, -3, 0, 0, -7,
    0, -36, 0, 0, -6, -14, 6, 9,
    0, 0, -6, 3, 3, 9, 6, -4,
    6, 0, 0, -16, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -10,
    0, 0, 2, 0, 7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, -12, 0, -12, -1, 0, 0, -9,
    0, -1, 9, 0, -9, -4, -1, 1,
    0, -4, 0, 0, -2, -19, 0, 2,
    0, 8, -7, 0, -6, 0, -12, 2,
    0, -23, -4, 6, 3, 0, 0, 0,
    0, 0, 0, 0, 3, 0, -5, -1,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 10, 0, 3, 0,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 12, 4, 1, 0, 0,
    0, 0, 0, 30, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -10, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 2, 6,
    3, 9, -3, 0, 0, 6, -3, -10,
    -38, 1, 7, 6, -1, -3, 0, 7,
    0, 8, 0, 8, 0, -28, 0, -4,
    9, 0, 9, -3, 6, 3, 0, 0,
    1, -3, 0, 0, -4, 0, 0, 0,
    23, 0, 9, 0, 11, 5, 11, 4,
    0, 0, -4, -10, 0, 0, 0, -3,
    1, -2, 0, 1, -6, -5, -6, 1,
    0, -3, 0, 0, 0, -12, 1, -7,
    0, -6, -10, 0, -7, -6, -10, 0,
    0, -19, 0, 0, 0, 0, 3, 0,
    0, 0, 0, 0, 3, 0, -5, -7,
    -5, -3, 1, -16, 3, -19, 0, 0,
    0, -10, -2, 0, 26, -4, -5, 3,
    3, 0, 0, -5, 3, 0, 0, -16,
    -6, 9, 0, 16, -10, -2, -11, 0,
    -11, 4, 0, -27, 0, 3, 3, 0,
    -2, 0, 0, 3, 0, 0, -1, -2,
    -9, 0, -9, 0, 0, 17, -6, 0,
    -10, 0, 10, 0, -20, -27, -21, -6,
    9, 0, 0, -18, 0, 2, -7, 0,
    -4, 0, -6, -12, 0, -3, 9, 0,
    9, 0, 9, 0, 0, 4, 9, -34,
    -18, 0, -18, 0, 4, 1, -18, -18,
    -7, -18, -9, -14, -9, -18, 0, 1,
    0, 0, 0, 0, 0, 1, 1, -4,
    -6, 0, -1, -1, -3, 0, 0, -1,
    0, 0, 0, -6, 0, -3, 0, -7,
    -6, 0, -7, -10, -10, -4, 0, -6,
    0, -6, 0, 0, 0, 0, 0, -2,
    0, 0, 3, 0, 1, -3, 1, 0,
    0, 0, 0, 3, -1, 0, 0, 0,
    -1, 3, 3, 0, 0, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 5, -1, 0, -4, 0, -5, 0,
    0, -1, 0, 9, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    0, -1, 0, -3, 0, -3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    -1, 0, -3, -3, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, -3, -3,
    -3, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, 0, 0, 0, 0,
    -2, -5, -2, 0, 0, -9, -1, -9,
    6, 0, 0, -6, 3, 6, 7, 0,
    -7, 0, -2, 0, 0, -12, 3, -1,
    2, -16, 3, 0, 0, 1, -15, 0,
    -16, -3, -25, -1, 0, -14, 0, 6,
    8, 0, 5, 0, 0, 0, 0, 0,
    1, 0, -7, -5, -7, 0, 0, 0,
    0, -3, 0, 0, 0, -3, 0, 0,
    0, 0, 0, -1, -1, 0, -1, -3,
    0, 0, 0, 0, 0, 0, 0, -3,
    -3, 0, -2, -4, -2, 0, 0, -3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, -3, -3, 0,
    0, -1, 0, -6, 3, 0, 0, -2,
    1, 3, 3, 0, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, 0, 1,
    0, 0, -3, 0, -3, -1, -2, 0,
    0, 0, 0, 0, 0, 0, 2, 0,
    0, -2, 0, 0, 0, 0, -4, -4,
    -4, 0, 0, 9, -1, 1, -8, 0,
    0, 7, -14, -14, -11, -6, 3, 0,
    -2, -19, -5, 0, -5, 0, -6, 4,
    -5, -17, 0, -7, 0, 0, 1, 0,
    3, -1, 0, 3, 2, -9, -11, 0,
    -14, 0, 0, -6, -6, -9, -2, -7,
    1, -3, 1, -6, 0, 0, 0, -3,
    0, 0, 0, 1, 0, 3, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -3, 0, -1, 0, 0, -3, 0,
    -5, -7, -7, 0, 0, -9, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, -1, -3, -1, 0, 0, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
    15, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, -3, 0, 3, 0, 10,
    -3, 0, -7, -2, -11, 0, 0, -6,
    0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 1, -3, 1, 0,
    -1, 0, 0, 0, -3, 0, 0, 2,
    0, -14, -9, 0, 0, 0, -4, -14,
    0, 0, -3, 3, 0, -5, -1, -13,
    0, -9, 0, 0, -5, -6, -5, -3,
    -2, 0, 0, -4, 0, 0, -3, 0,
    0, 0, 0, 0, 0, 0, 3, 0,
    3, 0, 0, -5, 0, 0, 0, 0,
    2, 0, 1, -6, -6, 0, -3, -3,
    -4, 0, 0, 0, 0, 0, 0, -9,
    0, -3, 0, -4, -3, 0, -7, -7,
    -9, -2, 0, -6, 2, -9, 0, 0,
    0, 0, 0, 23, 0, 0, 1, 0,
    0, -5, 0, 0, 0, -12, 0, 0,
    0, 0, 0, -28, -7, 9, 9, -3,
    -12, 0, 3, -4, 0, -14, -1, -3,
    3, -20, -3, 7, 0, 4, -10, -4,
    -11, -10, -12, 0, 0, -17, 0, 16,
    0, 0, -1, 0, 0, 0, 0, -1,
    -1, -3, -8, -10, -8, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 1,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -3, 0, -1, -3, -4, 0, 0, -6,
    0, -3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, -6, 0, 0, 6,
    -1, 3, 0, -5, 3, -1, 0, -4,
    -3, 0, -3, -3, -2, 0, -4, -4,
    0, 0, -2, -1, -1, -4, -2, 0,
    0, -3, 0, 3, -1, 0, -5, 0,
    0, 0, 0, -6, 0, -4, 0, -4,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, -6, 3, 0, -3, 0, -1,
    -2, -5, -1, -1, -1, 0, -1, -2,
    0, 0, 0, 0, 0, 0, -3, -2,
    -2, 0, 0, 0, 0, 2, -1, 0,
    -1, 0, 0, 0, 0, -1, -2, -1,
    -1, -2, -1, -1, 4, 12, 0, 0,
    -6, 0, -1, 6, 0, -3, -11, -3,
    4, 1, 0, -12, -4, 3, -4, 2,
    0, 1, -2, -8, 0, -4, 1, 0,
    0, -4, 0, 0, 0, 3, 3, -6,
    -4, 0, -4, 0, 0, -4, -3, -3,
    0, -4, 1, -4, 1, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 3,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -2, -3, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, -3, 0, 0, -3,
    -3, 0, 0, 0, 0, -3, 0, 0,
    0, 0, 0, -1, 0, 0, 0, 0,
    0, -1, 0, 0, 0, 0, -4, 0,
    -6, 0, 0, 0, -10, 0, 1, -7,
    6, 1, -1, -13, 0, 0, -7, -3,
    0, -12, -7, -9, 0, 0, -14, -3,
    -12, -12, -15, 0, -4, 0, 4, 18,
    -3, 0, -6, 0, 0, 0, -3, -4,
    -7, -5, -11, -13, -11, -7, 0, 0,
    -1, 0, 1, 0, 0, -20, 0, 9,
    5, -5, -10, 0, 1, -5, 0, -14,
    -1, -3, 6, -25, -3, 1, 0, 0,
    -19, -3, -14, -3, -21, 0, 0, -20,
    0, 14, 1, 0, -1, 0, 0, 0,
    0, 0, -1, -1, -11, -1, -11, 0,
    0, 0, 0, 0, -8, 0, -1, 0,
    -1, -8, -13, 0, 0, -1, -4, -9,
    -3, 0, -2, 0, 0, 0, 0, -13,
    -3, -9, -8, -4, -5, -7, -3, -4,
    0, -6, -1, -10, -4, 0, -3, 0,
    0, -3, -3, 0, 1, 0, -1, -9,
    -1, 0, 0, -5, 0, 0, 0, 0,
    2, 0, 1, -6, 14, 0, -3, -3,
    -4, 0, 0, 0, 0, 0, 0, -9,
    0, -3, 0, -4, -3, 0, -7, -7,
    -9, -2, 0, -6, 4, 12, 0, 0,
    0, 0, 0, 23, 0, 0, 1, 0,
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 0, -1, -6, 0, 0, 0, 0,
    0, -1, 0, 0, 0, -3, -3, 0,
    0, -6, -3, 0, 0, -6, 0, 4,
    -1, 0, 0, 0, 0, 0, 0, 0,
    1, 0, 0, 0, 0, 0, 6, 4,
    -1, 0, -8, -2, 0, 9, -10, -10,
    -6, -6, 12, 7, 3, -24, -2, 6,
    -3, 0, -3, 6, -3, -10, 0, -3,
    3, -3, -4, -9, -4, 0, 0, 9,
    6, 0, -8, 0, -16, 0, 0, 12,
    -3, -11, 1, -3, -9, -9, -9, -3,
    3, 0, -4, 0, -7, 0, 4, 9,
    -7, -11, -12, -7, 9, 0, 1, -22,
    -3, 3, -5, -2, -7, 0, -7, -11,
    -4, -4, -4, 0, 0, -7, -7, -3,
    0, 9, 7, -3, -16, 0, -16, 0,
    -1, -6, -10, -18, -1, -10, -5, -9,
    -5, -9, 0, 0, -3, 0, -6, -1,
    0, -3, -6, 0, 4, -10, 3, 0,
    0, -16, 0, -3, -7, -5, -2, -9,
    -7, -10, -6, 0, -9, -3, -7, -3,
    -9, -3, 0, 0, 1, 13, -5, 0,
    -9, 0, 0, 0, -3, -6, -7, -8,
    -10, -12, -10, -5, 6, 0, -4, 0,
    -14, -2, 3, 6, -10, -11, -6, -10,
    10, -3, 1, -28, -6, 6, -7, -5,
    -11, 0, -9, -12, -2, -3, -4, -3,
    -7, -9, -1, 0, 0, 9, 10, -1,
    -19, 0, -17, 0, -3, 10, -11, -20,
    -6, -10, -12, -14, -12, -10, 0, 0,
    0, 0, -2, 0, 0, 3, -2, 6,
    1, -4, 6, 0, 0, -5, 0, 0,
    0, 0, 1, 1, -2, 0, 0, 0,
    0, 0, 0, -3, 0, 0, 0, 0,
    4, 9, 1, 0, -2, 0, 0, 0,
    0, 0, -1, -1, -2, 0, -2, 0,
    1, 4, 0, 0, 0, 0, 4, 1,
    -1, 0, 12, 0, 7, 1, 1, -5,
    0, 6, 0, 0, 0, 4, 0, 0,
    0, 0, 6, 0, 7, 1, 10, 0,
    0, 9, 0, 10, -1, 0, 0, 0,
    0, 29, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -17, 0, -3, 4, 0,
    9, -38, 0, 26, 2, -6, -6, 3,
    3, -1, 1, -14, 0, 0, 15, -17,
    -6, 9, 0, 9, -6, -3, -12, 6,
    -6, 0, 0, -21, 12, 40, 0, 0,
    0, 0, 0, 35, 0, 0, 0, 0,
    6, 0, 6, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, -3, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -1,
    6, -7, 0, 0, 1, -3, 0, 3,
    35, -6, -2, 8, 7, -7, 3, 0,
    0, 3, 3, -5, -9, 16, 9, 22,
    0, -3, -3, 13, -1, 6, 0, -37,
    10, 0, -3, 0, -7, 0, 0, 33,
    0, 3, -6, -7, -5, 10, 6, 4,
    0, 0, 0, -8, 0, 0, 0, -7,
    0, 0, 0, 0, -5, -1, 0, 0,
    0, -5, 0, -2, 0, -13, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -18, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, -3, 0,
    -3, 0, 0, -4, 0, -7, 0, 0,
    0, -4, 3, -2, 0, 0, -7, -3,
    -6, 0, 0, -7, 0, -3, 0, -13,
    0, -6, 0, 0, -19, -3, -12, -6,
    -12, 0, 0, -18, 0, -7, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -4, -5, -4, -2, 0, 0, 0, 0,
    -5, 0, -7, 5, -6, 6, 0, -1,
    -6, -1, -4, -3, 0, -2, -1, -1,
    2, -7, -1, 0, 0, 0, -20, -4,
    -7, 0, -11, 0, -1, -13, -1, 0,
    0, -1, -2, 0, -1, 0, 0, 0,
    2, 0, -2, -4, -2, -1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 11, 0, 0, 0, 0, 0,
    0, -5, 0, -1, 0, 0, 0, -6,
    3, 0, 0, 0, -7, -3, -6, 0,
    0, -8, 0, -3, 0, -13, 0, 0,
    0, 0, -27, 0, -6, -11, -14, 0,
    0, -18, 0, -2, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -3, -4,
    -3, -1, 1, 0, 0, 4, -5, 0,
    12, 12, -3, -3, -9, 2, 12, 4,
    5, -7, 2, 11, 2, 7, 5, 7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 18, 13, -7, -3, 3,
    -2, 2, 3, 13, 1, 0, 0, 0,
    3, 0, 3, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, 0, 0,
    0, 7, 0, 0, 0, 0, -18, -2,
    -4, -10, -12, 0, 0, -18, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, 0, 0, 0, 0, 0,
    0, 0, 0, 7, 0, 0, 0, 0,
    -18, -2, -4, -10, -12, 0, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, 0,
    -7, 3, 0, -3, 4, 7, 3, -9,
    0, 1, -3, 3, 0, 4, 0, 0,
    0, 0, -3, 0, -1, -1, -6, 0,
    -1, -12, 0, 16, -3, 0, -7, 0,
    0, 0, -1, -5, 0, -3, -9, -6,
    -9, -4, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 7,
    0, 0, 0, 0, -18, -2, -4, -10,
    -12, 0, 0, -18, 0, 0, 0, 0,
    0, 0, 0, 14, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -8, -2, -1, 9, -3, -3, -12, 1,
    2, 1, -1, -6, 1, 7, 1, 4,
    1, 4, -6, -13, -2, 0, -6, -3,
    -7, -11, -10, 0, -2, -6, -2, -5,
    -11, -1, -3, 0, -1, 0, -1, 0,
    4, 0, 4, -1, 4, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -1, -3, -3, 0, 0, -7,
    0, -1, 0, -4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -17,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -3, -3, -3, 0,
    0, 0, 0, 0, -2, 0, 0, -4,
    -3, 3, 0, -4, -4, -1, 0, -6,
    -1, -5, -2, -2, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -18, 0, 8, 0, 0, -5, 0,
    0, 0, 0, 0, -3, 0, -3, 0,
    -3, 0, 0, 0, -1, 0, -7, 0,
    0, 11, -5, -9, -10, 1, 6, 6,
    1, -10, 1, 4, 1, 9, 1, 10,
    -1, -8, 0, 0, -5, 0, 0, -9,
    -7, 0, 0, -6, 0, -5, -5, 0,
    -4, 0, 0, -4, 0, -2, 4, 0,
    -4, -9, -4, -3, 0, 0, -1, 0,
    -6, 0, 0, 5, -7, 0, 3, -3,
    4, 2, 0, -10, 0, -1, -1, 0,
    -3, 6, -4, 0, 0, 0, -6, -2,
    -7, 0, -9, 0, 0, -13, 0, 10,
    -3, 0, -5, 0, 0, 7, 0, -3,
    0, -3, -9, 0, -9, -3, 0, 0,
    0, 0, -1, 0, 0, 3, -3, 1,
    0, 0, -2, -1, 0, -2, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -16,
    0, 5, 0, 0, -2, 0, 0, 0,
    0, 0, 1, 0, -3, -3, -3, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 34, 0, 0,
    36, 34, 4, 0, 23, 4, 4, 4,
    4, 4, 0, 0, 0, 0, -2, 0,
    0, -4, -3, 3, 0, -4, -4, -1,
    0, -6, -1, -5, -2, -2, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -18, 0, 8, 0, 0,
    -5, 0, 0, 6, 7, 0, -3, 0,
    -3, 0, -3, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 62,
    .right_class_cnt     = 50,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 2,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_salina_bold_18 = {
#else
lv_font_t font_salina_bold_18 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 22,          /*The maximum line height required by the font*/
    .base_line = 4,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -2,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if FONT_SALINA_BOLD_18*/
