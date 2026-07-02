let prefix = "skia_wrapped_stub"

let prologue = "
#include \"sk_types.h\"

/* Compatibility definitions for anonymous structs in newer Skia versions */
struct sk_imageinfo_t {
    sk_colorspace_t* colorspace;
    int32_t          width;
    int32_t          height;
    sk_colortype_t   colorType;
    sk_alphatype_t   alphaType;
};
struct sk_rect_t {
    float left;
    float top;
    float right;
    float bottom;
};
struct sk_irect_t {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};
struct sk_matrix_t {
    float mat[9];
};
struct sk_point_t {
    float x;
    float y;
};
struct sk_fontmetrics_t {
    uint32_t fFlags;
    float fTop;
    float fAscent;
    float fDescent;
    float fBottom;
    float fLeading;
    float fAvgCharWidth;
    float fMaxCharWidth;
    float fXMin;
    float fXMax;
    float fXHeight;
    float fCapHeight;
    float fUnderlineThickness;
    float fUnderlinePosition;
    float fStrikeoutThickness;
    float fStrikeoutPosition;
};
struct gr_gl_framebufferinfo_t {
    unsigned int fFBOID;
    unsigned int fFormat;
    bool fProtected;
};

/* Compatibility enums for missing/renamed enums in newer Skia versions */
typedef enum {
    DRAW_SHADOW_AND_FOREGROUND_SK_DROP_SHADOW_IMAGE_FILTER_SHADOW_MODE = 0,
    DRAW_SHADOW_ONLY_SK_DROP_SHADOW_IMAGE_FILTER_SHADOW_MODE = 1
} sk_drop_shadow_image_filter_shadow_mode_t;

typedef enum {
    NO_HINTING_SK_PAINT_HINTING = NONE_SK_FONT_HINTING,
    SLIGHT_HINTING_SK_PAINT_HINTING = SLIGHT_SK_FONT_HINTING,
    NORMAL_HINTING_SK_PAINT_HINTING = NORMAL_SK_FONT_HINTING,
    FULL_HINTING_SK_PAINT_HINTING = FULL_SK_FONT_HINTING
} sk_paint_hinting_t;

typedef enum {
    NONE_SK_FILTER_QUALITY = 0,
    LOW_SK_FILTER_QUALITY = 1,
    MEDIUM_SK_FILTER_QUALITY = 2,
    HIGH_SK_FILTER_QUALITY = 3
} sk_filter_quality_t;

struct sk_imagefilter_croprect_t { char dummy; };
"

let () =
  print_endline prologue;
  Cstubs.Types.write_c Format.std_formatter (module SkiaWrappedTypes.M)
