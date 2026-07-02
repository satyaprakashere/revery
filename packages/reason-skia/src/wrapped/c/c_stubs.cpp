/*
 * Use this file for building any C-layer functionality that we want to keep out
 * of Reason
 */

#include "c_stubs.h"

#include <stdio.h>
#include <stdlib.h>

void reason_skia_stub_sk_canvas_draw_rect_ltwh(sk_canvas_t *canvas, float left,
        float top, float width,
        float height,
        sk_paint_t *paint) {
    sk_rect_t rect;
    rect.left = left;
    rect.top = top;
    rect.right = left + width;
    rect.bottom = top + height;

    sk_canvas_draw_rect(canvas, &rect, paint);
}

void *reason_skia_sdl2_get(void *ctx, const char name[]) {
    return SDL_GL_GetProcAddress(name);
};

gr_glinterface_t *reason_skia_make_sdl2_gl_interface() {
    gr_glinterface_t *interface =
            (gr_glinterface_t *)gr_glinterface_assemble_gl_interface(0, (gr_gl_get_proc)reason_skia_sdl2_get);
    return interface;
}

gr_glinterface_t *reason_skia_make_sdl2_gles_interface() {
    gr_glinterface_t *interface =
            (gr_glinterface_t *)gr_glinterface_assemble_gles_interface(0, (gr_gl_get_proc)reason_skia_sdl2_get);
    return interface;
}

sk_shader_t* reason_skia_stub_linear_gradient2(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t startColor,
    sk_color_t stopColor,
    sk_shader_tilemode_t tileMode) {

    sk_point_t pts[2];
    pts[0] = *startPosition;
    pts[1] = *stopPosition;

    sk_color_t colors[2];
    colors[0] = startColor;
    colors[1] = stopColor;

    float stops[2];
    stops[0] = 0.0f;
    stops[1] = 1.0f;

    return sk_shader_new_linear_gradient(
               pts,
               colors,
               stops,
               2,
               tileMode,
               NULL
           );
}

sk_shader_t* reason_skia_stub_linear_gradient(
    sk_point_t* startPosition,
    sk_point_t* stopPosition,
    sk_color_t* colors,
    float* positions,
    int count,
    sk_shader_tilemode_t tileMode) {

    sk_point_t pts[2];
    pts[0] = *startPosition;
    pts[1] = *stopPosition;

    return sk_shader_new_linear_gradient(
               pts,
               colors,
               positions,
               count,
               tileMode,
               NULL
           );
}

#include "include/core/SkPath.h"
#include "include/core/SkPathBuilder.h"
#include "include/core/SkTypeface.h"
#include "include/core/SkFontMgr.h"
#include "include/core/SkFontStyle.h"
#include "include/core/SkM44.h"
#include "include/core/SkFont.h"
#include "include/core/SkFontTypes.h"
#include "include/core/SkFontMetrics.h"
#include "include/core/SkPaint.h"
#include "sk_pathbuilder.h"
#include "sk_path.h"
#include "sk_typeface.h"
#include "sk_canvas.h"
#include "sk_image.h"
#include "sk_imagefilter.h"
#include <unordered_map>
#include <mutex>

static std::unordered_map<sk_paint_t*, SkFont> g_paint_fonts;
static std::unordered_map<const sk_paint_t*, uint32_t> g_paint_filter_quality;
static std::unordered_map<const sk_paint_t*, sk_text_encoding_t> g_paint_text_encoding;
static std::mutex g_paint_fonts_mutex;

static SkFont& GetPaintFont(sk_paint_t* paint) {
    std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
    return g_paint_fonts[paint];
}

static SkM44 ToM44(const sk_matrix44_t* m) {
    SkM44 out(SkM44::kUninitialized_Constructor);
    out.setRC(0, 0, m->m00); out.setRC(0, 1, m->m01); out.setRC(0, 2, m->m02); out.setRC(0, 3, m->m03);
    out.setRC(1, 0, m->m10); out.setRC(1, 1, m->m11); out.setRC(1, 2, m->m12); out.setRC(1, 3, m->m13);
    out.setRC(2, 0, m->m20); out.setRC(2, 1, m->m21); out.setRC(2, 2, m->m22); out.setRC(2, 3, m->m23);
    out.setRC(3, 0, m->m30); out.setRC(3, 1, m->m31); out.setRC(3, 2, m->m32); out.setRC(3, 3, m->m33);
    return out;
}

static void FromM44(const SkM44& src, sk_matrix44_t* dst) {
    dst->m00 = src.rc(0, 0); dst->m01 = src.rc(0, 1); dst->m02 = src.rc(0, 2); dst->m03 = src.rc(0, 3);
    dst->m10 = src.rc(1, 0); dst->m11 = src.rc(1, 1); dst->m12 = src.rc(1, 2); dst->m13 = src.rc(1, 3);
    dst->m20 = src.rc(2, 0); dst->m21 = src.rc(2, 1); dst->m22 = src.rc(2, 2); dst->m23 = src.rc(2, 3);
    dst->m30 = src.rc(3, 0); dst->m31 = src.rc(3, 1); dst->m32 = src.rc(3, 2); dst->m33 = src.rc(3, 3);
}

extern "C" {

sk_typeface_t* sk_typeface_create_from_name_with_font_style(const char *name, const sk_fontstyle_t *style) {
    sk_fontmgr_t* fm = sk_fontmgr_create_default();
    if (!fm) return nullptr;
    sk_typeface_t* tf = sk_fontmgr_match_family_style(fm, name, const_cast<sk_fontstyle_t*>(style));
    sk_fontmgr_unref(fm);
    return tf;
}

sk_typeface_t* sk_typeface_create_from_file(const char *path, int index) {
    sk_fontmgr_t* fm = sk_fontmgr_create_default();
    if (!fm) return nullptr;
    sk_typeface_t* tf = sk_fontmgr_create_from_file(fm, path, index);
    sk_fontmgr_unref(fm);
    return tf;
}

void sk_path_move_to(sk_path_t *path, float x, float y) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_move_to(builder, x, y);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_rmove_to(sk_path_t *path, float dx, float dy) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_rmove_to(builder, dx, dy);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_line_to(sk_path_t *path, float x, float y) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_line_to(builder, x, y);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_rline_to(sk_path_t *path, float dx, float dy) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_rline_to(builder, dx, dy);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_cubic_to(sk_path_t *path, float x0, float y0, float x1, float y1, float x2, float y2) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_cubic_to(builder, x0, y0, x1, y1, x2, y2);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_rcubic_to(sk_path_t *path, float dx0, float dy0, float dx1, float dy1, float dx2, float dy2) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_rcubic_to(builder, dx0, dy0, dx1, dy1, dx2, dy2);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_quad_to(sk_path_t *path, float x0, float y0, float x1, float y1) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_quad_to(builder, x0, y0, x1, y1);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_rquad_to(sk_path_t *path, float dx0, float dy0, float dx1, float dy1) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_rquad_to(builder, dx0, dy0, dx1, dy1);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_arc_to(sk_path_t *path, float rx, float ry, float xAxisRotate, sk_path_arc_size_t largeArc, sk_path_direction_t sweep, float x, float y) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_arc_to(builder, rx, ry, xAxisRotate, largeArc, sweep, x, y);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_rarc_to(sk_path_t *path, float rx, float ry, float xAxisRotate, sk_path_arc_size_t largeArc, sk_path_direction_t sweep, float x, float y) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_rarc_to(builder, rx, ry, xAxisRotate, largeArc, sweep, x, y);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_close(sk_path_t *path) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_close(builder);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_add_rounded_rect(sk_path_t *path, const sk_rect_t *rect, float rx, float ry, sk_path_direction_t dir) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_add_rounded_rect(builder, rect, rx, ry, dir);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

void sk_path_add_circle(sk_path_t *path, float x, float y, float radius, sk_path_direction_t dir) {
    sk_pathbuilder_t* builder = sk_pathbuilder_new_from_path(path);
    sk_pathbuilder_add_circle(builder, x, y, radius, dir);
    sk_path_t* new_path = sk_pathbuilder_detach_path(builder);
    *reinterpret_cast<SkPath*>(path) = *reinterpret_cast<SkPath*>(new_path);
    sk_path_delete(new_path);
    sk_pathbuilder_delete(builder);
}

sk_text_encoding_t sk_paint_get_text_encoding(const sk_paint_t* paint);

void sk_canvas_draw_text(sk_canvas_t* canvas, const void* text, size_t byte_length, float x, float y, const sk_paint_t* paint) {
    SkFont& font = GetPaintFont(const_cast<sk_paint_t*>(paint));
    sk_text_encoding_t enc = sk_paint_get_text_encoding(paint);
    sk_canvas_draw_simple_text(canvas, text, byte_length, enc, x, y, reinterpret_cast<const sk_font_t*>(&font), paint);
}

sk_matrix44_t* sk_matrix44_new(void) {
    sk_matrix44_t* m = (sk_matrix44_t*)malloc(sizeof(sk_matrix44_t));
    if (m) {
        *m = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
    return m;
}

sk_matrix44_t* sk_matrix44_new_identity(void) {
    return sk_matrix44_new();
}

void sk_matrix44_destroy(sk_matrix44_t* m) {
    free(m);
}

float sk_matrix44_get(const sk_matrix44_t* m, int row, int col) {
    const float* f = reinterpret_cast<const float*>(m);
    return f[row * 4 + col];
}

void sk_matrix44_set(sk_matrix44_t* m, int row, int col, float val) {
    float* f = reinterpret_cast<float*>(m);
    f[row * 4 + col] = val;
}

void sk_matrix44_to_matrix(const sk_matrix44_t* m, sk_matrix_t* dst) {
    dst->scaleX = m->m00; dst->skewX  = m->m01; dst->transX = m->m03;
    dst->skewY  = m->m10; dst->scaleY = m->m11; dst->transY = m->m13;
    dst->persp0 = m->m30; dst->persp1 = m->m31; dst->persp2 = m->m33;
}

void sk_matrix44_set_translate(sk_matrix44_t* m, float dx, float dy, float dz) {
    SkM44 s = SkM44::Translate(dx, dy, dz);
    FromM44(s, m);
}

void sk_matrix44_pre_translate(sk_matrix44_t* m, float dx, float dy, float dz) {
    SkM44 s = ToM44(m);
    s.preTranslate(dx, dy, dz);
    FromM44(s, m);
}

void sk_matrix44_post_translate(sk_matrix44_t* m, float dx, float dy, float dz) {
    SkM44 s = ToM44(m);
    s.postTranslate(dx, dy, dz);
    FromM44(s, m);
}

void sk_matrix44_set_scale(sk_matrix44_t* m, float sx, float sy, float sz) {
    SkM44 s = SkM44::Scale(sx, sy, sz);
    FromM44(s, m);
}

void sk_matrix44_pre_scale(sk_matrix44_t* m, float sx, float sy, float sz) {
    SkM44 s = ToM44(m);
    s.preScale(sx, sy, sz);
    FromM44(s, m);
}

void sk_matrix44_post_scale(sk_matrix44_t* m, float sx, float sy, float sz) {
    SkM44 s = ToM44(m);
    SkM44 scale = SkM44::Scale(sx, sy, sz);
    SkM44 result(scale, s);
    FromM44(result, m);
}

void sk_matrix44_set_rotate_about_radians(sk_matrix44_t* m, float x, float y, float z, float radians) {
    SkM44 s = SkM44::Rotate({x, y, z}, radians);
    FromM44(s, m);
}

void sk_matrix44_set_rotate_about_degrees(sk_matrix44_t* m, float x, float y, float z, float degrees) {
    float radians = degrees * 3.14159265358979323846f / 180.0f;
    sk_matrix44_set_rotate_about_radians(m, x, y, z, radians);
}

void sk_matrix44_set_concat(sk_matrix44_t* m, const sk_matrix44_t* a, const sk_matrix44_t* b) {
    SkM44 sa = ToM44(a);
    SkM44 sb = ToM44(b);
    SkM44 s(sa, sb);
    FromM44(s, m);
}

void sk_matrix44_pre_concat(sk_matrix44_t* m, const sk_matrix44_t* other) {
    SkM44 s = ToM44(m);
    SkM44 o = ToM44(other);
    s.preConcat(o);
    FromM44(s, m);
}

void sk_matrix44_post_concat(sk_matrix44_t* m, const sk_matrix44_t* other) {
    SkM44 s = ToM44(m);
    SkM44 o = ToM44(other);
    s.postConcat(o);
    FromM44(s, m);
}

void sk_paint_set_typeface(sk_paint_t* paint, sk_typeface_t* typeface) {
    SkFont& font = GetPaintFont(paint);
    font.setTypeface(sk_ref_sp(reinterpret_cast<SkTypeface*>(typeface)));
}

void sk_paint_set_textsize(sk_paint_t* paint, float size) {
    SkFont& font = GetPaintFont(paint);
    font.setSize(size);
}

void sk_paint_set_lcd_render_text(sk_paint_t* paint, bool lcd) {
    SkFont& font = GetPaintFont(paint);
    font.setEdging(lcd ? SkFont::Edging::kSubpixelAntiAlias : SkFont::Edging::kAntiAlias);
}

void sk_paint_set_subpixel_text(sk_paint_t* paint, bool subpixel) {
    SkFont& font = GetPaintFont(paint);
    font.setSubpixel(subpixel);
}

void sk_paint_set_autohinted(sk_paint_t* paint, bool autohinted) {
    SkFont& font = GetPaintFont(paint);
    if (autohinted) {
        font.setHinting(SkFontHinting::kNormal);
    }
}

bool sk_paint_is_autohinted(const sk_paint_t* paint) {
    return false;
}

void sk_paint_set_hinting(sk_paint_t* paint, sk_font_hinting_t hinting) {
    SkFont& font = GetPaintFont(paint);
    font.setHinting(static_cast<SkFontHinting>(hinting));
}

sk_font_hinting_t sk_paint_get_hinting(const sk_paint_t* paint) {
    SkFont& font = GetPaintFont(const_cast<sk_paint_t*>(paint));
    return static_cast<sk_font_hinting_t>(font.getHinting());
}

void sk_paint_set_filter_quality(sk_paint_t* paint, uint32_t quality) {
    std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
    g_paint_filter_quality[paint] = quality;
}

uint32_t sk_paint_get_filter_quality(const sk_paint_t* paint) {
    std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
    auto it = g_paint_filter_quality.find(paint);
    return it != g_paint_filter_quality.end() ? it->second : 0;
}

void sk_paint_set_text_encoding(sk_paint_t* paint, sk_text_encoding_t encoding) {
    std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
    g_paint_text_encoding[paint] = encoding;
}

sk_text_encoding_t sk_paint_get_text_encoding(const sk_paint_t* paint) {
    std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
    auto it = g_paint_text_encoding.find(paint);
    return it != g_paint_text_encoding.end() ? it->second : (sk_text_encoding_t)0;
}

float sk_paint_get_fontmetrics(sk_paint_t* paint, sk_fontmetrics_t* metrics, float scale) {
    SkFont& font = GetPaintFont(paint);
    SkFontMetrics* m = reinterpret_cast<SkFontMetrics*>(metrics);
    float val = font.getMetrics(m);
    if (scale != 1.0f && m) {
        m->fTop *= scale;
        m->fAscent *= scale;
        m->fDescent *= scale;
        m->fBottom *= scale;
        m->fLeading *= scale;
        m->fAvgCharWidth *= scale;
        m->fMaxCharWidth *= scale;
        m->fXMin *= scale;
        m->fXMax *= scale;
        m->fXHeight *= scale;
        m->fCapHeight *= scale;
        m->fUnderlineThickness *= scale;
        m->fUnderlinePosition *= scale;
        m->fStrikeoutThickness *= scale;
        m->fStrikeoutPosition *= scale;
    }
    return val * scale;
}

float sk_paint_measure_text(sk_paint_t* paint, const void* text, size_t length, sk_rect_t* bounds) {
    SkFont& font = GetPaintFont(paint);
    sk_text_encoding_t enc = sk_paint_get_text_encoding(paint);
    SkTextEncoding encoding = static_cast<SkTextEncoding>(enc);
    SkRect* b = reinterpret_cast<SkRect*>(bounds);
    return font.measureText(text, length, encoding, b, reinterpret_cast<SkPaint*>(paint));
}

static sk_sampling_options_t GetDefaultSamplingOptions() {
    sk_sampling_options_t opt = {};
    opt.fMaxAniso = 0;
    opt.fUseCubic = false;
    opt.fCubic = {0.0f, 0.0f};
    opt.fFilter = LINEAR_SK_FILTER_MODE;
    opt.fMipmap = NONE_SK_MIPMAP_MODE;
    return opt;
}

void sk_paint_delete(sk_paint_t* paint) {
    {
        std::lock_guard<std::mutex> lock(g_paint_fonts_mutex);
        g_paint_fonts.erase(paint);
        g_paint_filter_quality.erase(paint);
        g_paint_text_encoding.erase(paint);
    }
    delete reinterpret_cast<SkPaint*>(paint);
}

void sk_canvas_draw_image_compat(sk_canvas_t* canvas, const sk_image_t* image, float x, float y, const sk_paint_t* paint) {
    sk_sampling_options_t opt = GetDefaultSamplingOptions();
    sk_canvas_draw_image(canvas, image, x, y, &opt, paint);
}

void sk_canvas_draw_image_rect_compat(sk_canvas_t* canvas, const sk_image_t* image, const sk_rect_t* src, const sk_rect_t* dst, const sk_paint_t* paint) {
    sk_sampling_options_t opt = GetDefaultSamplingOptions();
    sk_canvas_draw_image_rect(canvas, image, src, dst, &opt, paint);
}

sk_image_t* sk_image_new_from_encoded_compat(const sk_data_t* cdata, const sk_irect_t* subset) {
    return sk_image_new_from_encoded(cdata);
}

void sk_canvas_concat_compat(sk_canvas_t* canvas, const sk_matrix_t* matrix) {
    sk_matrix44_t m44 = {
        matrix->scaleX, matrix->skewX,  0.0f, matrix->transX,
        matrix->skewY,  matrix->scaleY, 0.0f, matrix->transY,
        0.0f,           0.0f,           1.0f, 0.0f,
        matrix->persp0, matrix->persp1, 0.0f, matrix->persp2
    };
    sk_canvas_concat(canvas, &m44);
}

void sk_canvas_set_matrix_compat(sk_canvas_t* canvas, const sk_matrix_t* matrix) {
    sk_matrix44_t m44 = {
        matrix->scaleX, matrix->skewX,  0.0f, matrix->transX,
        matrix->skewY,  matrix->scaleY, 0.0f, matrix->transY,
        0.0f,           0.0f,           1.0f, 0.0f,
        matrix->persp0, matrix->persp1, 0.0f, matrix->persp2
    };
    sk_canvas_set_matrix(canvas, &m44);
}

void sk_canvas_flush(sk_canvas_t* canvas) {
    gr_recording_context_t* rc = sk_get_recording_context(canvas);
    if (rc) {
        gr_direct_context_t* dc = gr_recording_context_get_direct_context(rc);
        if (dc) {
            gr_direct_context_flush(dc);
        }
    }
}

typedef struct gr_direct_context_t gr_context_t;
gr_context_t* gr_context_make_gl(const gr_glinterface_t* glInterface) {
    return reinterpret_cast<gr_context_t*>(gr_direct_context_make_gl(glInterface));
}

sk_data_t* sk_image_encode(const sk_image_t* cimage) {
    return sk_image_ref_encoded(cimage);
}

sk_imagefilter_t* sk_imagefilter_new_drop_shadow_compat(float dx, float dy, float sigmaX, float sigmaY, sk_color_t color, bool shadowOnly, const sk_imagefilter_t* input, const sk_rect_t* cropRect) {
    if (shadowOnly) {
        return sk_imagefilter_new_drop_shadow_only(dx, dy, sigmaX, sigmaY, color, input, cropRect);
    } else {
        return sk_imagefilter_new_drop_shadow(dx, dy, sigmaX, sigmaY, color, input, cropRect);
    }
}

}
