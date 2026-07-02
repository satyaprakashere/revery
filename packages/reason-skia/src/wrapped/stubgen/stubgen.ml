let prefix = "skia_wrapped_stub"

let prologue = {|
#include "include/c/sk_types.h"

/* Compatibility declarations for missing functions in newer Skia C API */
typedef struct sk_imagefilter_croprect_t sk_imagefilter_croprect_t;

SK_C_API sk_typeface_t* sk_typeface_create_from_name_with_font_style(const char *name, const sk_fontstyle_t *style);
SK_C_API sk_typeface_t* sk_typeface_create_from_file(const char *path, int index);

SK_C_API void sk_path_add_rounded_rect(sk_path_t *path, const sk_rect_t *rect, float rx, float ry, sk_path_direction_t dir);
SK_C_API void sk_path_add_circle(sk_path_t *path, float x, float y, float radius, sk_path_direction_t dir);
SK_C_API void sk_path_move_to(sk_path_t *path, float x, float y);
SK_C_API void sk_path_rmove_to(sk_path_t *path, float dx, float dy);
SK_C_API void sk_path_line_to(sk_path_t *path, float x, float y);
SK_C_API void sk_path_rline_to(sk_path_t *path, float dx, float dy);
SK_C_API void sk_path_cubic_to(sk_path_t *path, float x0, float y0, float x1, float y1, float x2, float y2);
SK_C_API void sk_path_rcubic_to(sk_path_t *path, float dx0, float dy0, float dx1, float dy1, float dx2, float dy2);
SK_C_API void sk_path_quad_to(sk_path_t *path, float x0, float y0, float x1, float y1);
SK_C_API void sk_path_rquad_to(sk_path_t *path, float dx0, float dy0, float dx1, float dy1);
SK_C_API void sk_path_arc_to(sk_path_t *path, float rx, float ry, float xAxisRotate, sk_path_arc_size_t largeArc, sk_path_direction_t sweep, float x, float y);
SK_C_API void sk_path_rarc_to(sk_path_t *path, float rx, float ry, float xAxisRotate, sk_path_arc_size_t largeArc, sk_path_direction_t sweep, float x, float y);
SK_C_API void sk_path_close(sk_path_t* path);

SK_C_API void sk_canvas_draw_text(sk_canvas_t* canvas, const void* text, size_t byte_length, float x, float y, const sk_paint_t* paint);

SK_C_API sk_matrix44_t* sk_matrix44_new(void);
SK_C_API sk_matrix44_t* sk_matrix44_new_identity(void);
SK_C_API void sk_matrix44_destroy(sk_matrix44_t* m);
SK_C_API float sk_matrix44_get(const sk_matrix44_t* m, int row, int col);
SK_C_API void sk_matrix44_set(sk_matrix44_t* m, int row, int col, float val);
SK_C_API void sk_matrix44_to_matrix(const sk_matrix44_t* m, sk_matrix_t* dst);
SK_C_API void sk_matrix44_set_translate(sk_matrix44_t* m, float dx, float dy, float dz);
SK_C_API void sk_matrix44_pre_translate(sk_matrix44_t* m, float dx, float dy, float dz);
SK_C_API void sk_matrix44_post_translate(sk_matrix44_t* m, float dx, float dy, float dz);
SK_C_API void sk_matrix44_set_scale(sk_matrix44_t* m, float sx, float sy, float sz);
SK_C_API void sk_matrix44_pre_scale(sk_matrix44_t* m, float sx, float sy, float sz);
SK_C_API void sk_matrix44_post_scale(sk_matrix44_t* m, float sx, float sy, float sz);
SK_C_API void sk_matrix44_set_rotate_about_degrees(sk_matrix44_t* m, float x, float y, float z, float degrees);
SK_C_API void sk_matrix44_set_rotate_about_radians(sk_matrix44_t* m, float x, float y, float z, float radians);
SK_C_API void sk_matrix44_set_concat(sk_matrix44_t* m, const sk_matrix44_t* a, const sk_matrix44_t* b);
SK_C_API void sk_matrix44_pre_concat(sk_matrix44_t* m, const sk_matrix44_t* other);
SK_C_API void sk_matrix44_post_concat(sk_matrix44_t* m, const sk_matrix44_t* other);

SK_C_API void sk_paint_set_typeface(sk_paint_t* paint, sk_typeface_t* typeface);
SK_C_API void sk_paint_set_lcd_render_text(sk_paint_t* paint, bool lcd);
SK_C_API void sk_paint_set_subpixel_text(sk_paint_t* paint, bool subpixel);
SK_C_API void sk_paint_set_textsize(sk_paint_t* paint, float size);
SK_C_API float sk_paint_get_fontmetrics(sk_paint_t* paint, sk_fontmetrics_t* metrics, float scale);
SK_C_API bool sk_paint_is_autohinted(const sk_paint_t* paint);
SK_C_API void sk_paint_set_autohinted(sk_paint_t* paint, bool autohinted);
SK_C_API sk_font_hinting_t sk_paint_get_hinting(const sk_paint_t* paint);
SK_C_API void sk_paint_set_hinting(sk_paint_t* paint, sk_font_hinting_t hinting);
SK_C_API void sk_paint_set_filter_quality(sk_paint_t* paint, uint32_t quality);
SK_C_API uint32_t sk_paint_get_filter_quality(const sk_paint_t* paint);
SK_C_API float sk_paint_measure_text(sk_paint_t* paint, const void* text, size_t length, sk_rect_t* bounds);
SK_C_API sk_text_encoding_t sk_paint_get_text_encoding(const sk_paint_t* paint);
SK_C_API void sk_paint_set_text_encoding(sk_paint_t* paint, sk_text_encoding_t encoding);

#include "c_stubs.h"
#include "gr_context.h"
#include "sk_canvas.h"
#include "sk_data.h"
#include "sk_image.h"
#include "sk_imagefilter.h"
#include "sk_paint.h"
#include "sk_path.h"
#include "sk_surface.h"
#include "sk_rrect.h"
#include "sk_matrix.h"
#include "sk_typeface.h"
#include "sk_stream.h"
#include "sk_string.h"

// Enable the SVG functions
#define ESY_SKIA_SVG
#include "sk_svg.h"

typedef struct gr_direct_context_t gr_context_t;
typedef sk_font_hinting_t sk_paint_hinting_t;

typedef enum {
    NONE_SK_FILTER_QUALITY = 0,
    LOW_SK_FILTER_QUALITY = 1,
    MEDIUM_SK_FILTER_QUALITY = 2,
    HIGH_SK_FILTER_QUALITY = 3
} sk_filter_quality_t;

#define sk_canvas_draw_image sk_canvas_draw_image_compat
#define sk_canvas_draw_image_rect sk_canvas_draw_image_rect_compat
#define sk_image_new_from_encoded sk_image_new_from_encoded_compat
#define sk_canvas_concat sk_canvas_concat_compat
#define sk_canvas_set_matrix sk_canvas_set_matrix_compat
#define sk_imagefilter_new_drop_shadow sk_imagefilter_new_drop_shadow_compat

SK_C_API void sk_canvas_draw_image_compat(sk_canvas_t* canvas, const sk_image_t* image, float x, float y, const sk_paint_t* paint);
SK_C_API void sk_canvas_draw_image_rect_compat(sk_canvas_t* canvas, const sk_image_t* image, const sk_rect_t* src, const sk_rect_t* dst, const sk_paint_t* paint);
SK_C_API sk_image_t* sk_image_new_from_encoded_compat(const sk_data_t* cdata, const sk_irect_t* subset);
SK_C_API void sk_canvas_concat_compat(sk_canvas_t* canvas, const sk_matrix_t* matrix);
SK_C_API void sk_canvas_set_matrix_compat(sk_canvas_t* canvas, const sk_matrix_t* matrix);
SK_C_API void sk_canvas_flush(sk_canvas_t* canvas);
SK_C_API gr_context_t* gr_context_make_gl(const gr_glinterface_t* glInterface);
SK_C_API sk_data_t* sk_image_encode(const sk_image_t* cimage);
SK_C_API sk_imagefilter_t* sk_imagefilter_new_drop_shadow_compat(float dx, float dy, float sigmaX, float sigmaY, sk_color_t color, bool shadowOnly, const sk_imagefilter_t* input, const sk_rect_t* cropRect);
|}

let () =
  let generate_ml, generate_c = ref false, ref false in
  let () =
    Arg.(parse [ ("-ml", Set generate_ml, "Generate ML");
                 ("-c", Set generate_c, "Generate C") ])
      (fun _ -> failwith "unexpected anonymous argument")
      "stubgen [-ml|-c]"
  in
  match !generate_ml, !generate_c with
  | false, false
  | true, true ->
    failwith "Exactly one of -ml and -c must be specified"
  | true, false ->
    Cstubs.write_ml Format.std_formatter ~prefix (module SkiaWrappedBindings.M)
  | false, true ->
    print_endline prologue;
    Cstubs.write_c Format.std_formatter ~prefix (module SkiaWrappedBindings.M)
