//
// shoes/ruby.c
// Just little bits of Ruby I've become accustomed to.
//
#include "shoes/app.h"
#include "shoes/canvas.h"
#include "shoes/ruby.h"
#include "shoes/internal.h"
#include "shoes/world.h"
#include "shoes/native.h"
#include "shoes/version.h"
#include <math.h>

VALUE cShoes, cApp, cDialog, cShoesWindow, cMouse, cCanvas, cFlow, cStack, cMask, cWidget, cShape, cImage, cImageBlock, cEffect, cBlur, cShadow, cGlow, cVideo, cTimerBase, cTimer, cEvery, cAnim, cPattern, cBorder, cBackground, cTextBlock, cPara, cBanner, cTitle, cSubtitle, cTagline, cCaption, cInscription, cTextClass, cSpan, cDel, cStrong, cSub, cSup, cCode, cEm, cIns, cLinkUrl, cNative, cButton, cCheck, cRadio, cEditLine, cEditBox, cListBox, cProgress, cColor, cColors, cLink, cLinkHover, ssNestSlot;
VALUE eVlcError, eImageError, eNotImpl;
VALUE reHEX_SOURCE, reHEX3_SOURCE, reRGB_SOURCE, reRGBA_SOURCE, reGRAY_SOURCE, reGRAYA_SOURCE, reLF;
VALUE symAltQuest, symAltSlash, symAltDot;
ID s_aref, s_mult, s_perc, s_bind, s_gsub, s_keys, s_update, s_new, s_run, s_to_pattern, s_to_i, s_to_s, s_angle, s_arrow, s_autoplay, s_begin, s_call, s_center, s_change, s_choose, s_click, s_corner, s_curve, s_distance, s_displace_left, s_displace_top, s_downcase, s_draw, s_end, s_fill, s_finish, s_font, s_hand, s_hidden, s_hover, s_href, s_inner, s_insert, s_items, s_keypress, s_motion, s_release, s_wheel, s_scroll, s_start, s_attach, s_leading, s_leave, s_match, s_text, s_title, s_top, s_right, s_bottom, s_left, s_up, s_down, s_height, s_resizable, s_remove, s_strokewidth, s_width, s_margin, s_margin_left, s_margin_right, s_margin_top, s_margin_bottom, s_radius, s_secret, s_now, s_debug, s_error, s_warn, s_info;

//
// Mauricio's instance_eval hack (he bested my cloaker back in 06 Jun 2006)
//
VALUE instance_eval_proc;

VALUE
mfp_instance_eval(VALUE obj, VALUE block)
{
  return rb_funcall(instance_eval_proc, s_call, 2, obj, block);
}

//
// From Guy Decoux [ruby-talk:144098]
//
static VALUE
ts_each(VALUE *tmp)
{
  return rb_funcall2(tmp[0], (ID)tmp[1], (int)tmp[2], (VALUE *)tmp[3]);
}

VALUE
ts_funcall2(VALUE obj, ID meth, int argc, VALUE *argv)
{
  VALUE tmp[4];
  tmp[0] = obj;
  tmp[1] = (VALUE)meth; 
  tmp[2] = (VALUE)argc;
  tmp[3] = (VALUE)argv;
  return rb_iterate((VALUE(*)(VALUE))ts_each, (VALUE)tmp, CASTHOOK(rb_yield), 0);
}

long
rb_ary_index_of(VALUE ary, VALUE val)
{
  long i;
 
  for (i=0; i<RARRAY(ary)->len; i++) {
    if (rb_equal(RARRAY(ary)->ptr[i], val))
      return i;
  }

  return -1;
}

VALUE
rb_ary_insert_at(VALUE ary, long index, int len, VALUE ary2)
{
  rb_funcall(ary, s_aref, 3, LONG2NUM(index), INT2NUM(len), ary2);
  return ary;
}

//
// from ruby's eval.c
// 
static inline VALUE
call_cfunc(HOOK func, VALUE recv, int len, int argc, VALUE *argv)
{
  if (len >= 0 && argc != len) {
    rb_raise(rb_eArgError, "wrong number of arguments (%d for %d)",
      argc, len);
  }

  switch (len) {
    case -2:
    return (*func)(recv, rb_ary_new4(argc, argv));
    case -1:
    return (*func)(argc, argv, recv);
    case 0:
    return (*func)(recv);
    case 1:
    return (*func)(recv, argv[0]);
    case 2:
    return (*func)(recv, argv[0], argv[1]);
    case 3:
    return (*func)(recv, argv[0], argv[1], argv[2]);
    case 4:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3]);
    case 5:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4]);
    case 6:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5]);
    case 7:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6]);
    case 8:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7]);
    case 9:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8]);
    case 10:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9]);
    case 11:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9], argv[10]);
    case 12:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9],
               argv[10], argv[11]);
    case 13:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9], argv[10],
               argv[11], argv[12]);
    case 14:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9], argv[10],
               argv[11], argv[12], argv[13]);
    case 15:
    return (*func)(recv, argv[0], argv[1], argv[2], argv[3], argv[4],
               argv[5], argv[6], argv[7], argv[8], argv[9], argv[10],
               argv[11], argv[12], argv[13], argv[14]);
    default:
    rb_raise(rb_eArgError, "too many arguments (%d)", len);
    break;
  }
  return Qnil;        /* not reached */
}

typedef struct
{
  VALUE canvas;
  VALUE block;
  VALUE args;
} safe_block;

static VALUE
shoes_safe_block_call(VALUE rb_sb)
{
  int i;
  VALUE vargs[10];
  safe_block *sb = (safe_block *)rb_sb;
  for (i = 0; i < RARRAY_LEN(sb->args); i++)
    vargs[i] = rb_ary_entry(sb->args, i);
  return rb_funcall2(sb->block, s_call, RARRAY_LEN(sb->args), vargs);
}

static VALUE
shoes_safe_block_exception(VALUE rb_sb, VALUE e)
{
  safe_block *sb = (safe_block *)rb_sb;
  shoes_canvas_error(sb->canvas, e);
  return Qnil;
}

VALUE
shoes_safe_block(VALUE canvas, VALUE block, VALUE args)
{
  safe_block sb;
  VALUE v;
  sb.canvas = canvas;
  sb.block = block;
  sb.args = args;
  rb_gc_register_address(&args);
  v = rb_rescue2(CASTHOOK(shoes_safe_block_call), (VALUE)&sb, 
    CASTHOOK(shoes_safe_block_exception), (VALUE)&sb, rb_cObject, 0);
  rb_gc_unregister_address(&args);
  return v;
}

int
shoes_px(VALUE attr, ID k, int dv, int pv, int nv)
{
  int px;
  VALUE obj = shoes_hash_get(attr, k);
  if (TYPE(obj) == T_STRING) {
    char *ptr = RSTRING_PTR(obj);
    int len = RSTRING_LEN(obj);
    obj = rb_funcall(obj, s_to_i, 0);
    if (len > 1 && ptr[len - 1] == '%')
    {
      obj = rb_funcall(obj, s_mult, 1, rb_float_new(0.01)); 
    }
  }
  if (rb_obj_is_kind_of(obj, rb_cFloat)) {
    px = (int)((double)pv * NUM2DBL(obj));
  } else {
    if (NIL_P(obj))
      px = dv;
    else
      px = NUM2INT(obj);
    if (px < 0 && nv == 1)
      px += pv;
  }
  return px;
}

int
shoes_px2(VALUE attr, ID k1, ID k2, int dv, int dr, int pv)
{
  int px;
  VALUE obj = shoes_hash_get(attr, k2);
  if (!NIL_P(obj))
  {
    px = shoes_px(attr, k2, 0, pv, 0);
    px = (pv - dr) - px;
  }
  else
  {
    px = shoes_px(attr, k1, dv, pv, 0);
  }
  return px;
}

VALUE
shoes_hash_set(VALUE hsh, ID key, VALUE val)
{
  if (NIL_P(hsh))
    hsh = rb_hash_new();
  rb_hash_aset(hsh, ID2SYM(key), val);
  return hsh;
}

VALUE
shoes_hash_get(VALUE hsh, ID key)
{
  VALUE v;

  if (!NIL_P(hsh))
  {
    v = rb_hash_aref(hsh, ID2SYM(key));
    if (!NIL_P(v)) return v;
  }

  return Qnil;
}

int
shoes_hash_int(VALUE hsh, ID key, int dn)
{
  VALUE v = shoes_hash_get(hsh, key);
  if (!NIL_P(v)) return NUM2INT(v);
  return dn;
}

double
shoes_hash_dbl(VALUE hsh, ID key, double dn)
{
  VALUE v = shoes_hash_get(hsh, key);
  if (!NIL_P(v)) return NUM2DBL(v);
  return dn;
}

char *
shoes_hash_cstr(VALUE hsh, ID key, char *dn)
{
  VALUE v = shoes_hash_get(hsh, key);
  if (!NIL_P(v)) return RSTRING_PTR(v);
  return dn;
}

VALUE
rb_str_to_pas(VALUE str)
{
  VALUE str2;
  char strlen[2];
  strlen[0] = RSTRING_LEN(str);
  strlen[1] = '\0';
  str2 = rb_str_new2(strlen);
  rb_str_append(str2, str);
  return str2;
}

void
shoes_place_decide(shoes_place *place, VALUE c, VALUE attr, int dw, int dh, unsigned char rel, int padded)
{
  shoes_canvas *canvas = NULL;
  VALUE ck = rb_obj_class(c);
  VALUE stuck = ATTR(attr, attach);
  if (!NIL_P(c))
    Data_Get_Struct(c, shoes_canvas, canvas);

  ATTR_MARGINS(attr, 0);
  if (padded) {
    dh += tmargin + bmargin;
    dw += lmargin + rmargin;
  }

  int testw = dw;
  if (testw == 0) testw = lmargin + 1 + rmargin;

  if (!NIL_P(stuck))
  {
    if (stuck == cShoesWindow)
      rel = REL_WINDOW;
    else if (stuck == cMouse)
      rel = REL_CURSOR;
    else
      rel = REL_STICKY;
  }

  place->flags = rel;
  place->dx = place->dy = 0;
  if (canvas == NULL)
  {
    place->ix = place->x = 0;
    place->iy = place->y = 0;
    place->iw = place->w = dw;
    place->ih = place->h = dh;
  }
  else
  {
    int cx, cy, ox, oy, tw, th;

    if (rel == REL_WINDOW)
    {
      cx = 0; cy = 0;
      ox = 0; oy = canvas->slot.scrolly;
    }
    else if (rel == REL_CANVAS)
    {
      cx = canvas->cx - CPX(canvas);
      cy = canvas->cy - CPY(canvas);
      ox = CPX(canvas);
      oy = CPY(canvas);
    }
    else if (rel == REL_CURSOR)
    {
      cx = 0; cy = 0;
      ox = canvas->app->mousex; oy = canvas->app->mousey;
    }
    else if (rel == REL_TILE)
    {
      cx = 0; cy = 0;
      ox = CPX(canvas);
      oy = CPY(canvas);
      tw = dw; th = dh;
      testw = dw = CPW(canvas);
      dh = max(canvas->height, CPH(canvas));
    }
    else
    {
      cx = 0; cy = 0;
      ox = canvas->cx; oy = canvas->cy;
      if (rel == REL_STICKY)
      {
        if (rb_respond_to(stuck, s_top) && rb_respond_to(stuck, s_left))
        {
          ox = NUM2INT(rb_funcall(stuck, s_left, 0));
          oy = NUM2INT(rb_funcall(stuck, s_top, 0));
        }
      }
    }

    place->w = PX(attr, width, testw, CPW(canvas));
    if (dw == 0 && place->w + (int)canvas->cx > canvas->place.iw) {
      canvas->cx = canvas->endx = CPX(canvas);
      canvas->cy = canvas->endy;
      place->w = canvas->place.iw;
    }
    place->h = PX(attr, height, dh, CPH(canvas));

    if (rel != REL_TILE)
    {
      tw = place->w;
      th = place->h;
    }

    place->x = PX2(attr, left, right, cx, tw, canvas->place.iw) + ox;
    place->y = PX2(attr, top, bottom, cy, th, canvas->fully) + oy;
    if (!ORIGIN(canvas->place))
    {
      place->dx = canvas->place.dx; 
      place->dy = canvas->place.dy;
    }
    place->dx += PXN(attr, displace_left, 0, CPW(canvas)); 
    place->dy += PXN(attr, displace_top, 0, CPH(canvas));

    place->flags |= NIL_P(ATTR(attr, left)) && NIL_P(ATTR(attr, right)) ? 0 : FLAG_ABSX;
    place->flags |= NIL_P(ATTR(attr, top)) && NIL_P(ATTR(attr, bottom)) ? 0 : FLAG_ABSY;
    if (rel != REL_TILE && ABSY(*place) == 0 && (ck == cStack || place->x + place->w > CPX(canvas) + canvas->place.iw))
    {
      canvas->cx = place->x = CPX(canvas);
      canvas->cy = place->y = canvas->endy;
    }
  }

  place->ix = place->x + lmargin;
  place->iy = place->y + tmargin;
  place->iw = place->w - (lmargin + rmargin);
  place->ih = place->h - (tmargin + bmargin);
  INFO("PLACE: (%d, %d), (%d: %d, %d: %d) [%d, %d] %x\n", place->x, place->y, place->w, place->iw, place->h, place->ih, ABSX(*place), ABSY(*place), place->flags);
}

void
shoes_ele_remove_all(VALUE contents)
{
  if (!NIL_P(contents))
  {
    long i;
    VALUE ary;
    ary = rb_ary_dup(contents);
    for (i = 0; i < RARRAY_LEN(ary); i++) 
      rb_funcall(rb_ary_entry(ary, i), s_remove, 0);
    rb_ary_clear(contents);
  }
}

void
shoes_cairo_rect(cairo_t *cr, double x, double y, double w, double h, double r)
{
  double rc = r * BEZIER;
  cairo_new_path(cr);
  cairo_move_to(cr, x + r, y);
  cairo_rel_line_to(cr, w - 2 * r, 0.0);
  cairo_rel_curve_to(cr, rc, 0.0, r, rc, r, r);
  cairo_rel_line_to(cr, 0, h - 2 * r);
  cairo_rel_curve_to(cr, 0.0, rc, rc - r, r, -r, r);
  cairo_rel_line_to(cr, -w + 2 * r, 0);
  cairo_rel_curve_to(cr, -rc, 0, -r, -rc, -r, -r);
  cairo_rel_line_to(cr, 0, -h + 2 * r);
  cairo_rel_curve_to(cr, 0.0, -rc, r - rc, -r, r, -r);
  cairo_close_path(cr);
}

void
shoes_control_hide_ref(SHOES_CONTROL_REF ref)
{
  if (ref != NULL) shoes_native_control_hide(ref);
}

void
shoes_control_show_ref(SHOES_CONTROL_REF ref)
{
  if (ref != NULL) shoes_native_control_show(ref);
}

//
// Macros for setting up drawing
//
#define SETUP(self_type, rel, dw, dh) \
  self_type *self_t; \
  shoes_place place; \
  shoes_canvas *canvas; \
  VALUE ck = rb_obj_class(c); \
  Data_Get_Struct(self, self_type, self_t); \
  Data_Get_Struct(c, shoes_canvas, canvas); \
  if (ATTR(self_t->attr, hidden) == Qtrue) return self; \
  shoes_place_decide(&place, c, self_t->attr, dw, dh, rel, rel == REL_CANVAS)

#define SETUP_CONTROL(dh, dw) \
  char *msg = ""; \
  int len = dw ? dw : 200; \
  shoes_control *self_t; \
  shoes_canvas *canvas; \
  shoes_place place; \
  VALUE text = Qnil, ck = rb_obj_class(c); \
  Data_Get_Struct(self, shoes_control, self_t); \
  Data_Get_Struct(c, shoes_canvas, canvas); \
  text = ATTR(self_t->attr, text); \
  if (!NIL_P(text)) { \
    text = shoes_native_to_s(text); \
    msg = RSTRING_PTR(text); \
    len = (RSTRING_LEN(text) * 8) + 32; \
  } \
  shoes_place_decide(&place, c, self_t->attr, len, 28 + dh, REL_CANVAS, TRUE)

#define FINISH() \
  if (!ABSY(place)) { \
    canvas->cx += place.w; \
    canvas->cy = place.y; \
    canvas->endx = canvas->cx; \
    canvas->endy = max(canvas->endy, place.y + place.h); \
  } \
  if (ck == cStack) { \
    canvas->cx = CPX(canvas); \
    canvas->cy = canvas->endy; \
  }

#define PATTERN_SCALE(self_t) \
  if (self_t->width == 1.0 && self_t->height == 1.0) \
  { \
    cairo_pattern_get_matrix(self_t->pattern, &matrix1); \
    cairo_pattern_get_matrix(self_t->pattern, &matrix2); \
    cairo_matrix_scale(&matrix2, 1. / (place.iw + (sw * 2.)), 1. / (place.ih + (sw * 2.))); \
    if (sw != 0.0) cairo_matrix_translate(&matrix2, sw, sw); \
    cairo_pattern_set_matrix(self_t->pattern, &matrix2); \
  }

#define PATTERN_RESET(self_t) \
  if (self_t->width == 1.0 && self_t->height == 1.0) \
  { \
    cairo_pattern_set_matrix(self_t->pattern, &matrix1); \
  }

//
// Shoes::Shape
//
static void
shoes_shape_mark(shoes_shape *path)
{
  rb_gc_mark_maybe(path->fg);
  rb_gc_mark_maybe(path->bg);
  rb_gc_mark_maybe(path->parent);
  rb_gc_mark_maybe(path->attr);
}

static void
shoes_shape_free(shoes_shape *path)
{
  if (path->line != NULL)
    cairo_path_destroy(path->line);
  RUBY_CRITICAL(free(path));
}

VALUE
shoes_shape_new(cairo_path_t *line, VALUE parent, VALUE x, VALUE y, int w, int h)
{
  shoes_shape *path;
  shoes_canvas *canvas;
  VALUE obj = shoes_shape_alloc(cShape);
  Data_Get_Struct(obj, shoes_shape, path);
  Data_Get_Struct(parent, shoes_canvas, canvas);
  path->line = line;
  path->parent = parent;
  path->fg = canvas->fg;
  path->bg = canvas->bg;
  path->sw = canvas->sw;
  path->width = w;
  path->height = h;
  ATTRSET(path->attr, left, x);
  ATTRSET(path->attr, top, y);
  return obj;
}

VALUE
shoes_shape_alloc(VALUE klass)
{
  VALUE obj;
  shoes_shape *shape = SHOE_ALLOC(shoes_shape);
  SHOE_MEMZERO(shape, shoes_shape, 1);
  obj = Data_Wrap_Struct(klass, shoes_shape_mark, shoes_shape_free, shape);
  shape->line = NULL;
  shape->attr = Qnil;
  shape->parent = Qnil;
  shape->fg = Qnil;
  shape->bg = Qnil;
  shape->width = 0;
  shape->height = 0;
  shape->hover = 0;
  return obj;
}

VALUE
shoes_shape_remove(VALUE self)
{
  GET_STRUCT(shape, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 0, 0);
  return self;
}

#define PATH_OUT(pen, cfunc) \
  if (!NIL_P(self_t->pen)) \
  { \
    double r = 0., sw = self_t->sw; \
    cairo_matrix_t matrix1, matrix2; \
    shoes_pattern *pattern; \
    Data_Get_Struct(self_t->pen, shoes_pattern, pattern); \
    PATTERN_SCALE(pattern); \
    cairo_set_line_width(canvas->cr, sw); \
    cairo_set_source(canvas->cr, pattern->pattern); \
    cfunc(canvas->cr); \
    PATTERN_RESET(pattern); \
  }

VALUE
shoes_shape_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP(shoes_shape, REL_CANVAS, self_t->width, self_t->height);

  if (RTEST(actual))
  {
    cairo_save(canvas->cr);
    cairo_translate(canvas->cr, place.ix + place.dx, place.iy + place.dy);
    cairo_new_path(canvas->cr);
    cairo_append_path(canvas->cr, self_t->line);

    PATH_OUT(bg, cairo_fill_preserve);
    PATH_OUT(fg, cairo_stroke);

    cairo_restore(canvas->cr);
  }

  self_t->place = place;
  return self;
}

VALUE
shoes_shape_motion(VALUE self, int x, int y, char *touch)
{
  char h = 0;
  VALUE click;
  GET_STRUCT(shape, self_t);

  click = ATTR(self_t->attr, click);
  if (self_t->line == NULL) return Qnil;

  if (IS_INSIDE(self_t, x, y))
  {
    cairo_bool_t in_shape;
    cairo_t *cr = cairo_create(cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 1, 1));
    cairo_new_path(cr);
    cairo_append_path(cr, self_t->line);
    in_shape = cairo_in_fill(cr, x - (self_t->place.ix + self_t->place.dx),
      y - (self_t->place.iy + self_t->place.dy));
    cairo_destroy(cr);

    if (in_shape)
    {
      if (!NIL_P(click))
      {
        shoes_canvas *canvas;
        Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
        shoes_app_cursor(canvas->app, s_hand);
      }
      h = 1;
    }
  }

  CHECK_HOVER(self_t, h, touch);

  return h ? click : Qnil;
}

VALUE
shoes_shape_send_click(VALUE self, int button, int x, int y)
{
  VALUE v = Qnil;

  if (button == 1)
  {
    GET_STRUCT(shape, self_t);
    v = shoes_shape_motion(self, x, y, NULL);
    if (self_t->hover & HOVER_MOTION)
      self_t->hover = HOVER_MOTION | HOVER_CLICK;
  }

  return v;
}

void
shoes_shape_send_release(VALUE self, int button, int x, int y)
{
  GET_STRUCT(shape, self_t);
  if (button == 1 && (self_t->hover & HOVER_CLICK))
  {
    VALUE proc = rb_hash_aref(self_t->attr, ID2SYM(s_release));
    self_t->hover ^= HOVER_CLICK;
    if (!NIL_P(proc))
      shoes_safe_block(self, proc, rb_ary_new3(1, self));
  }
}

//
// Shoes::Image
//
static void
shoes_image_mark(shoes_image *image)
{
  rb_gc_mark_maybe(image->path);
  rb_gc_mark_maybe(image->parent);
  rb_gc_mark_maybe(image->attr);
  rb_gc_mark_maybe(image->mode);
}

static void
shoes_image_free(shoes_image *image)
{
  SHOE_FREE(image->tf);
  RUBY_CRITICAL(SHOE_FREE(image));
}

VALUE
shoes_image_new(VALUE klass, VALUE path, VALUE realpath, VALUE attr, VALUE parent, cairo_matrix_t *tf, VALUE mode)
{
  GError *error = NULL;
  VALUE obj = Qnil;
  shoes_image *image;
  cairo_surface_t *surf;

  obj = shoes_image_alloc(klass);
  Data_Get_Struct(obj, shoes_image, image);

  image->path = Qnil;
  if (rb_obj_is_kind_of(path, cImage))
  {
    shoes_image *image2;
    Data_Get_Struct(path, shoes_image, image2);
    cairo_surface_reference(image->surface = image2->surface);
    attr = realpath;
  }
  else if (rb_obj_is_kind_of(path, cImageBlock))
  {
    shoes_canvas *c;
    Data_Get_Struct(path, shoes_canvas, c);
    cairo_surface_reference(image->surface = cairo_get_target(c->cr));
    attr = realpath;
  }
  else
  {
    image->path = path;
    image->surface = shoes_load_image(realpath);
  }
  cairo_matrix_init_identity(image->tf);
  cairo_matrix_multiply(image->tf, image->tf, tf);
  image->mode = mode;
  image->attr = attr;
  image->parent = parent;

  return obj;
}

VALUE
shoes_image_alloc(VALUE klass)
{
  VALUE obj;
  shoes_image *image = SHOE_ALLOC(shoes_image);
  SHOE_MEMZERO(image, shoes_image, 1);
  obj = Data_Wrap_Struct(klass, shoes_image_mark, shoes_image_free, image);
  image->path = Qnil;
  image->surface = NULL;
  image->tf = SHOE_ALLOC(cairo_matrix_t);
  cairo_matrix_init_identity(image->tf);
  image->mode = Qnil;
  image->attr = Qnil;
  image->parent = Qnil;
  image->hover = 0;
  return obj;
}

VALUE
shoes_image_get_path(VALUE self)
{
  GET_STRUCT(image, image);
  return image->path;
}

VALUE
shoes_image_set_path(VALUE self, VALUE path)
{
  cairo_surface_t *surf = shoes_load_image(path);
  GET_STRUCT(image, image);
  image->path = path;
  image->surface = surf;
  shoes_canvas_repaint_all(image->parent);
  return path;
}

VALUE
shoes_image_remove(VALUE self)
{
  GET_STRUCT(image, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 0, 0);
  return self;
}

#define SHOES_IMAGE_PLACE(type, surf) \
  int imw, imh; \
  SETUP(shoes_##type, REL_CANVAS, \
    (imw = cairo_image_surface_get_width(surf)), \
    (imh = cairo_image_surface_get_height(surf))); \
  if (RTEST(actual)) \
  { \
    cairo_save(canvas->cr); \
    cairo_translate(canvas->cr, place.ix + place.dx, place.iy + place.dy); \
    shoes_apply_transformation(canvas, self_t->tf, place.ix + place.dx, place.iy + place.dy, \
      place.iw, place.ih, self_t->mode); \
    if (place.iw != imw || place.ih != imh) \
    { \
      cairo_scale(canvas->cr, (place.iw * 1.) / imw, (place.ih * 1.) / imh); \
    } \
    cairo_set_source_surface(canvas->cr, surf, -imw / 2., -imh / 2.); \
    cairo_paint(canvas->cr); \
    cairo_restore(canvas->cr); \
    self_t->place = place; \
  } \
  FINISH(); \
  return self;

VALUE
shoes_image_draw(VALUE self, VALUE c, VALUE actual)
{
  SHOES_IMAGE_PLACE(image, self_t->surface);
}

VALUE
shoes_imageblock_draw(VALUE self, VALUE c, VALUE actual)
{
  SHOES_IMAGE_PLACE(canvas, cairo_get_target(self_t->cr));
}

VALUE
shoes_imageblock_paint(VALUE self, int init)
{
  GET_STRUCT(canvas, self_t);
  VALUE hidden = ATTR(self_t->attr, hidden);
  ATTRSET(self_t->attr, hidden, Qfalse);

  if (!init)
  {
    cairo_set_operator(self_t->cr, CAIRO_OPERATOR_CLEAR);
    cairo_paint(self_t->cr);
  }

  cairo_set_operator(self_t->cr, CAIRO_OPERATOR_OVER);
  shoes_canvas_draw(self, self_t->parent, Qfalse);
  shoes_canvas_draw(self, self_t->parent, Qtrue);
  ATTRSET(self_t->attr, hidden, hidden);

  if (!init)
    shoes_canvas_repaint_all(self_t->parent);
  return self;
}

VALUE
shoes_imageblock_refresh(VALUE self)
{
  return shoes_imageblock_paint(self, 0);
}

VALUE
shoes_image_size(VALUE self)
{
  GET_STRUCT(image, self_t);
  return rb_ary_new3(2,
    INT2NUM(cairo_image_surface_get_width(self_t->surface)),
    INT2NUM(cairo_image_surface_get_height(self_t->surface)));
}

static unsigned char *
box_run(unsigned int size)
{
  int i;
  unsigned char *tmp = SHOE_ALLOC_N(unsigned char, size * 256);
  for (i = 0; i < 256; i++)
    memset(tmp + i * size, i, size);
  return tmp;
}

#define BOX_H 1
#define BOX_V 2

static void
box_blur(unsigned char *in, unsigned char *out,
  int stride, shoes_place place,
  unsigned int edge1, unsigned int edge2,
  const unsigned char *run, int dir)
{
  int i, j1, j2, l, l2, l3, l4, lx, c1, c2, c3, c4, start;
  int boxSize = edge1 + edge2 + 1;
  if (dir == BOX_H)
  {
    c1 = place.y;
    c2 = place.y + place.h;
    c3 = place.x;
    c4 = place.x + place.w;
  }
  else
  {
    c1 = place.x;
    c2 = place.x + place.w;
    c3 = place.y;
    c4 = place.y + place.h;
  }

  start = c3 - edge1;
  for (j1 = c1; j1 < c2; j1++) {
    unsigned int sums[4] = {0, 0, 0, 0};
    if (dir == BOX_H)
      l = stride * j1;
    else
      lx = j1 << 2;
    for (i = 0; i < boxSize; i++) {
      int pos = start + i;
      pos = max(pos, c3);
      pos = min(pos, c4 - 1);
      if (dir == BOX_V)
        l = stride * pos + lx;
      sums[0] += in[l];
      sums[1] += in[l + 1];
      sums[2] += in[l + 2];
      sums[3] += in[l + 3];
    }
    for (j2 = c3; j2 < c4; j2++) {
      if (dir == BOX_H)
        l2 = l + (j2 << 2);
      else
        l2 = stride * j2 + lx;
      out[l2] = run[sums[0]];
      out[l2 + 1] = run[sums[1]];
      out[l2 + 2] = run[sums[2]];
      out[l2 + 3] = run[sums[3]];

      int tmp = j2 - edge1;
      int last = max(tmp, c3);
      int next = min(tmp + boxSize, c4 - 1);
      if (dir == BOX_H)
      {
        l3 = l + (next << 2);
        l4 = l + (last << 2);
      }
      else
      {
        l3 = stride * next + lx;
        l4 = stride * last + lx;
      }

      sums[0] += in[l3] - in[l4];
      sums[1] += in[l3 + 1] - in[l4 + 1];
      sums[2] += in[l3 + 2] - in[l4 + 2];
      sums[3] += in[l3 + 3] - in[l4 + 3];
    }
  }
}

#define RAW_FILTER_START(self_t) \
  int width, height, stride; \
  guchar *out; \
  static const cairo_user_data_key_t key; \
  cairo_surface_t *source = cairo_get_target(cr); \
  cairo_surface_t *target; \
  unsigned char *in = cairo_image_surface_get_data(source); \
  \
  self_t->place.x = self_t->place.y = 0; \
  self_t->place.w = width  = cairo_image_surface_get_width(source); \
  self_t->place.h = height = cairo_image_surface_get_height(source); \
  stride = cairo_image_surface_get_stride(source); \
  \
  out = (guchar *)g_malloc(4 * width * height); \
  target = cairo_image_surface_create_for_data((unsigned char *)out, \
    CAIRO_FORMAT_ARGB32, \
    width, height, 4 * width); \
  cairo_surface_set_user_data(target, &key, out, (cairo_destroy_func_t)g_free); \
  unsigned int len = 4 * width * height
   
#define RAW_FILTER_END(self_t) \
  cairo_set_operator(cr, CAIRO_OPERATOR_CLEAR); \
  cairo_paint(cr); \
  cairo_set_operator(cr, CAIRO_OPERATOR_OVER); \
  cairo_set_source_surface(cr, target, 0, 0); \
  cairo_paint(cr); \
  cairo_surface_destroy(target);

void
shoes_gaussian_blur_filter(cairo_t *cr, void *data)
{
  shoes_effect *fx = (shoes_effect *)data;
  RAW_FILTER_START(fx);
  float blur_d = ATTR2(dbl, fx->attr, radius, 2.);
  float blur_x = ATTR2(dbl, fx->attr, width, blur_d);
  float blur_y = ATTR2(dbl, fx->attr, height, blur_d);

  if (blur_x < 0 || blur_y < 0)
    return;

  if (blur_x == 0 || blur_y == 0)
    memset(out, 0, len);

  unsigned int dX, dY;
  dX = (unsigned int) floor(blur_x * 3*sqrt(2*SHOES_PI)/4 + 0.5);
  dY = (unsigned int) floor(blur_y * 3*sqrt(2*SHOES_PI)/4 + 0.5);

  unsigned char *tmp = SHOE_ALLOC_N(unsigned char, len);

  if (dX & 1) {
    unsigned char *run = box_run(2 * (dX / 2) + 1);
    box_blur(in, tmp,  stride, fx->place, dX/2, dX/2, run, BOX_H);
    box_blur(tmp, out, stride, fx->place, dX/2, dX/2, run, BOX_H);
    box_blur(out, tmp, stride, fx->place, dX/2, dX/2, run, BOX_H);
    SHOE_FREE(run);
  } else {
    if (dX == 0) {
      memcpy(tmp, in, len);
    } else {
      unsigned char *run1 = box_run(2 * (dX / 2) + 1);
      unsigned char *run2 = box_run(2 * (dX / 2));
      box_blur(in, tmp,  stride, fx->place, dX/2,     dX/2 - 1, run2, BOX_H);
      box_blur(tmp, out, stride, fx->place, dX/2 - 1, dX/2,     run2, BOX_H);
      box_blur(out, tmp, stride, fx->place, dX/2,     dX/2,     run1, BOX_H);
      SHOE_FREE(run1);
      SHOE_FREE(run2);
    }
  }

  if (dY & 1) {
    unsigned char *run = box_run(2 * (dY / 2) + 1);
    box_blur(tmp, out, stride, fx->place, dY/2, dY/2, run, BOX_V);
    box_blur(out, tmp, stride, fx->place, dY/2, dY/2, run, BOX_V);
    box_blur(tmp, out, stride, fx->place, dY/2, dY/2, run, BOX_V);
    SHOE_FREE(run);
  } else {
    if (dY == 0) {
      memcpy(out, tmp, len);
    } else {
      unsigned char *run1 = box_run(2 * (dY / 2) + 1);
      unsigned char *run2 = box_run(2 * (dY / 2));
      box_blur(tmp, out, stride, fx->place, dY/2,     dY/2 - 1, run2, BOX_V);
      box_blur(out, tmp, stride, fx->place, dY/2 - 1, dY/2,     run2, BOX_V);
      box_blur(tmp, out, stride, fx->place, dY/2,     dY/2,     run1, BOX_V);
      SHOE_FREE(run1);
      SHOE_FREE(run2);
    }
  }

  SHOE_FREE(tmp);
  RAW_FILTER_END(fx);
}

static void
shoes_layer_blur_filter(cairo_t *cr, void *data, cairo_operator_t blur_op,
  cairo_operator_t merge_op, int distance)
{
  shoes_canvas *canvas;
  shoes_effect *fx = (shoes_effect *)data;
  cairo_surface_t *source = cairo_get_target(cr);
  int width  = cairo_image_surface_get_width(source);
  int height = cairo_image_surface_get_height(source);
  VALUE fill = ATTR(fx->attr, fill);
  Data_Get_Struct(fx->parent, shoes_canvas, canvas);

  cairo_surface_t *target = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
  cairo_t *cr2 = cairo_create(target);
  cairo_set_source_surface(cr2, source, distance, distance);
  cairo_paint(cr2);
  cairo_set_operator(cr2, blur_op);
  if (NIL_P(canvas->bg))
    cairo_set_source_rgb(cr2, 0., 0., 0.);
  else
  {
    shoes_pattern *pattern;
    Data_Get_Struct(canvas->bg, shoes_pattern, pattern);
    cairo_set_source(cr2, pattern->pattern);
  }
  cairo_rectangle(cr2, 0, 0, width, height);
  cairo_paint(cr2);
  shoes_gaussian_blur_filter(cr2, data);
  cairo_set_operator(cr, merge_op);
  cairo_set_source_surface(cr, target, 0, 0);
  cairo_paint(cr);
  cairo_destroy(cr2);
}

void
shoes_shadow_filter(cairo_t *cr, void *data)
{
  shoes_effect *fx = (shoes_effect *)data;
  int distance = ATTR2(int, fx->attr, distance, 4);
  shoes_layer_blur_filter(cr, data, CAIRO_OPERATOR_IN, CAIRO_OPERATOR_DEST_OVER, distance);
}

void
shoes_glow_filter(cairo_t *cr, void *data)
{
  shoes_effect *fx = (shoes_effect *)data;
  cairo_operator_t blur_op = CAIRO_OPERATOR_IN;
  cairo_operator_t merge_op = CAIRO_OPERATOR_DEST_OVER;
  if (RTEST(ATTR(fx->attr, inner)))
  {
    blur_op = CAIRO_OPERATOR_OUT;
    merge_op = CAIRO_OPERATOR_ATOP;
  }
  shoes_layer_blur_filter(cr, data, blur_op, merge_op, 0);
}

VALUE
shoes_image_motion(VALUE self, int x, int y, char *touch)
{
  char h = 0;
  VALUE click;
  GET_STRUCT(image, self_t);

  click = ATTR(self_t->attr, click);
  if (self_t->surface == NULL) return Qnil;

  if (IS_INSIDE(self_t, x, y))
  {
    if (!NIL_P(click))
    {
      shoes_canvas *canvas;
      Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
      shoes_app_cursor(canvas->app, s_hand);
    }
    h = 1;
  }

  CHECK_HOVER(self_t, h, touch);

  return h ? click : Qnil;
}

VALUE
shoes_image_send_click(VALUE self, int button, int x, int y)
{
  VALUE v = Qnil;

  if (button == 1)
  {
    GET_STRUCT(image, self_t);
    v = shoes_image_motion(self, x, y, NULL);
    if (self_t->hover & HOVER_MOTION)
      self_t->hover = HOVER_MOTION | HOVER_CLICK;
  }

  return v;
}

void
shoes_image_send_release(VALUE self, int button, int x, int y)
{
  GET_STRUCT(image, self_t);
  if (button == 1 && (self_t->hover & HOVER_CLICK))
  {
    VALUE proc = rb_hash_aref(self_t->attr, ID2SYM(s_release));
    self_t->hover ^= HOVER_CLICK;
    if (!NIL_P(proc))
      shoes_safe_block(self, proc, rb_ary_new3(1, self));
  }
}

//
// Shoes::Effect
//
static void
shoes_effect_mark(shoes_effect *fx)
{
  rb_gc_mark_maybe(fx->parent);
  rb_gc_mark_maybe(fx->attr);
}

static void
shoes_effect_free(shoes_effect *fx)
{
  RUBY_CRITICAL(free(fx));
}

VALUE
shoes_effect_new(VALUE klass, VALUE attr, VALUE parent)
{
  shoes_effect *fx;
  shoes_canvas *canvas;
  VALUE obj = shoes_effect_alloc(klass);
  Data_Get_Struct(obj, shoes_effect, fx);
  Data_Get_Struct(parent, shoes_canvas, canvas);
  fx->parent = parent;
  fx->attr = attr;
  if (klass == cBlur)
    fx->filter = &shoes_gaussian_blur_filter;
  else if (klass == cShadow)
    fx->filter = &shoes_shadow_filter;
  else if (klass == cGlow)
    fx->filter = &shoes_glow_filter;
  return obj;
}

VALUE
shoes_effect_alloc(VALUE klass)
{
  VALUE obj;
  shoes_effect *fx = SHOE_ALLOC(shoes_effect);
  SHOE_MEMZERO(fx, shoes_effect, 1);
  obj = Data_Wrap_Struct(klass, shoes_effect_mark, shoes_effect_free, fx);
  fx->attr = Qnil;
  fx->parent = Qnil;
  return obj;
}

VALUE
shoes_effect_remove(VALUE self)
{
  GET_STRUCT(effect, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 0, 0);
  return self;
}

VALUE
shoes_effect_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP(shoes_effect, REL_TILE, canvas->width, canvas->height);

  if (RTEST(actual))
  {
    self_t->filter(canvas->cr, (void *)self_t);
  }

  self_t->place = place;
  return self;
}

//
// Shoes::Video
//
#ifdef VIDEO
static void shoes_vlc_exception(libvlc_exception_t *excp)
{
  if (libvlc_exception_raised(excp))
  {
    shoes_error("from VLC: %s", libvlc_exception_get_message(excp)); 
  }
}

static void
shoes_video_mark(shoes_video *video)
{
  rb_gc_mark_maybe(video->path);
  rb_gc_mark_maybe(video->parent);
  rb_gc_mark_maybe(video->attr);
}

static void
shoes_video_free(shoes_video *video)
{
  if (video->vlc != NULL)
    libvlc_destroy(video->vlc);
  RUBY_CRITICAL(SHOE_FREE(video));
}

VALUE
shoes_video_new(VALUE klass, VALUE path, VALUE attr, VALUE parent)
{
  GError *error = NULL;
  shoes_video *video;
  VALUE obj = shoes_video_alloc(klass);
  Data_Get_Struct(obj, shoes_video, video);

  video->path = path;
  video->attr = attr;
  video->parent = parent;
  return obj;
}

VALUE
shoes_video_alloc(VALUE klass)
{
  VALUE obj;
  shoes_video *video = SHOE_ALLOC(shoes_video);
  SHOE_MEMZERO(video, shoes_video, 1);
#ifndef SHOES_GTK
  char *ppsz_argv[2] = {"vlc", NULL};
  char pathsw[SHOES_BUFSIZE];
  int ppsz_argc = 2;
  sprintf(pathsw, "--plugin-path=%s/plugins", shoes_world->path);
  ppsz_argv[1] = pathsw;
#else
  char *ppsz_argv[1] = {"vlc"};
  int ppsz_argc = 1;
#endif
  obj = Data_Wrap_Struct(klass, shoes_video_mark, shoes_video_free, video);
  libvlc_exception_init(&video->excp);
  video->ref = NULL;
  video->vlc = libvlc_new(ppsz_argc, ppsz_argv, NULL);
  video->path = Qnil;
  video->attr = Qnil;
  video->parent = Qnil;
  video->init = 0;
  return obj;
}

VALUE
shoes_video_hide(VALUE self)
{
  GET_STRUCT(video, self_t);
  ATTRSET(self_t->attr, hidden, Qtrue);
  shoes_native_surface_hide(self_t->ref);
  shoes_canvas_repaint_all(self_t->parent);
  return self;
}

VALUE
shoes_video_show(VALUE self)
{
  GET_STRUCT(video, self_t);
  ATTRSET(self_t->attr, hidden, Qfalse);
  shoes_native_surface_show(self_t->ref);
  shoes_canvas_repaint_all(self_t->parent);
  return self;
}

VALUE
shoes_video_remove(VALUE self)
{
  shoes_canvas *canvas;
  GET_STRUCT(video, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 1, 0);

  Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
  shoes_native_surface_remove(canvas, self_t->ref);
  return self;
}

VALUE
shoes_video_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP(shoes_video, REL_CANVAS, 400, 300);

  if (RTEST(actual))
  {
    if (HAS_DRAWABLE(canvas->slot))
    {
      if (self_t->init == 0)
      {
        self_t->init = 1;

        int play_id = libvlc_playlist_add(self_t->vlc, 
          RSTRING_PTR(self_t->path), NULL, &self_t->excp);
        shoes_vlc_exception(&self_t->excp);

        self_t->ref = shoes_native_surface_new(canvas, self, &self_t->place);
        shoes_native_surface_position(self_t->ref, &self_t->place, self, canvas, &place);

        libvlc_video_set_parent(self_t->vlc, DRAWABLE(self_t->ref), &self_t->excp);
        shoes_vlc_exception(&self_t->excp);

#ifdef SHOES_QUARTZ
        libvlc_rectangle_t view, clip;
        view.top = -(self_t->place.iy + self_t->place.dy);
        view.left = -(self_t->place.ix + self_t->place.dx);
        view.right = view.left + self_t->place.iw;
        view.bottom = view.top + self_t->place.ih;
        clip.top = self_t->place.iy + self_t->place.dy;
        clip.left = self_t->place.ix + self_t->place.dx;
        clip.bottom = clip.top + self_t->place.ih; 
        clip.right = clip.left + self_t->place.iw; 
        libvlc_video_set_viewport(self_t->vlc, &view, &clip, NULL);
#endif

        if (RTEST(ATTR(self_t->attr, autoplay)))
        {
          INFO("Starting playlist.\n");
          libvlc_playlist_play(self_t->vlc, 0, 0, NULL, &self_t->excp);
          shoes_vlc_exception(&self_t->excp);
        }
      }
      else
      {
#ifndef SHOES_QUARTZ
        shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
#endif
      }
    }
  }

  FINISH();
  return self;
}

VALUE
shoes_video_is_playing(VALUE self)
{
  GET_STRUCT(video, self_t);
  if (self_t->init == 1)
  {
    int isp = libvlc_playlist_isplaying(self_t->vlc, &self_t->excp);
    shoes_vlc_exception(&self_t->excp);
    return isp == 0 ? Qfalse : Qtrue;
  }
  return Qfalse;
}

VALUE
shoes_video_play(VALUE self)
{
  GET_STRUCT(video, self_t);
  if (self_t->init == 1)
  {
    libvlc_playlist_play(self_t->vlc, 0, 0, NULL, &self_t->excp);
    shoes_vlc_exception(&self_t->excp);
  }
  return self;
}

#define VIDEO_METHOD(x) \
  VALUE \
  shoes_video_##x(VALUE self) \
  { \
    GET_STRUCT(video, self_t); \
    if (self_t->init == 1) \
    { \
      libvlc_playlist_##x(self_t->vlc, &self_t->excp); \
      shoes_vlc_exception(&self_t->excp); \
    } \
    return self; \
  }

#define VIDEO_GET_METHOD(x, ctype, rbtype) \
  VALUE shoes_video_get_##x(VALUE self) \
  { \
    GET_STRUCT(video, self_t); \
    if (self_t->init == 1) \
    { \
      libvlc_input_t *input = libvlc_playlist_get_input(self_t->vlc, NULL); \
      if (input != NULL) { \
        ctype len = libvlc_input_get_##x(input, &self_t->excp); \
        shoes_vlc_exception(&self_t->excp); \
        return rbtype(len); \
      } \
    } \
    return Qnil; \
  }

#define VIDEO_SET_METHOD(x, rbconv) \
  VALUE shoes_video_set_##x(VALUE self, VALUE val) \
  { \
    GET_STRUCT(video, self_t); \
    if (self_t->init == 1) \
    { \
      libvlc_input_t *input = libvlc_playlist_get_input(self_t->vlc, NULL); \
      if (input != NULL) { \
        libvlc_input_set_##x(input, rbconv(val), &self_t->excp); \
        shoes_vlc_exception(&self_t->excp); \
      } \
    } \
    return val; \
  }

VIDEO_METHOD(clear);
VIDEO_METHOD(prev);
VIDEO_METHOD(next);
VIDEO_METHOD(pause);
VIDEO_METHOD(stop);
VIDEO_GET_METHOD(length, vlc_int64_t, INT2NUM);
VIDEO_GET_METHOD(time, vlc_int64_t, INT2NUM);
VIDEO_SET_METHOD(time, NUM2INT);
VIDEO_GET_METHOD(position, float, rb_float_new);
VIDEO_SET_METHOD(position, NUM2DBL);
#endif

//
// Shoes::Pattern
//
static void
shoes_pattern_mark(shoes_pattern *pattern)
{
  rb_gc_mark_maybe(pattern->source);
  rb_gc_mark_maybe(pattern->parent);
  rb_gc_mark_maybe(pattern->attr);
}

static void
shoes_pattern_free(shoes_pattern *pattern)
{
  cairo_pattern_destroy(pattern->pattern);
  RUBY_CRITICAL(free(pattern));
}

static void
shoes_pattern_gradient(shoes_pattern *pattern, VALUE r1, VALUE r2, VALUE attr)
{
  double angle = ATTR2(dbl, attr, angle, 0.);
  double rads = angle * SHOES_RAD2PI;
  double edge = sin(rads) + cos(rads);

  if (rb_obj_is_kind_of(r1, rb_cString))
    r1 = shoes_color_parse(cColor, r1);
  if (rb_obj_is_kind_of(r2, rb_cString))
    r2 = shoes_color_parse(cColor, r2);

  VALUE radius = ATTR(attr, radius);
  if (!NIL_P(radius))
  {
    double r = 0.001;
    if (rb_obj_is_kind_of(r, rb_cFloat)) r = NUM2DBL(radius);
    pattern->pattern = cairo_pattern_create_radial(0.5, 0.5, r, edge / 2., edge / 2., edge / 2.);
  }
  else
  {
    pattern->pattern = cairo_pattern_create_linear(0.0, 0.0, 0.0, edge);
    if (angle != 0.)
    {
      cairo_matrix_t matrix;
      cairo_matrix_init_rotate(&matrix, rads);
      cairo_pattern_set_matrix(pattern->pattern, &matrix);
    }
  }
  shoes_color_grad_stop(pattern->pattern, 0.0, r1);
  shoes_color_grad_stop(pattern->pattern, 1.0, r2);
  pattern->width = pattern->height = 1.;
}

VALUE
shoes_pattern_self(VALUE self)
{
  return self;
}

VALUE
shoes_pattern_args(int argc, VALUE *argv, VALUE self)
{
  VALUE source, attr;
  rb_scan_args(argc, argv, "11", &source, &attr);
  return shoes_pattern_new(cPattern, source, attr, Qnil);
}

VALUE
shoes_pattern_new(VALUE klass, VALUE source, VALUE attr, VALUE parent)
{
  shoes_pattern *pattern;
  VALUE obj = shoes_pattern_alloc(klass);
  VALUE rgb;
  Data_Get_Struct(obj, shoes_pattern, pattern);
  pattern->source = Qnil;

  if (rb_obj_is_kind_of(source, rb_cRange))
  {
    VALUE r1 = rb_funcall(source, s_begin, 0);
    VALUE r2 = rb_funcall(source, s_end, 0);
    shoes_pattern_gradient(pattern, r1, r2, attr);
  }
  else
  {
    if (!rb_obj_is_kind_of(source, cColor))
    {
      rgb = shoes_color_parse(cColor, source);
      if (!NIL_P(rgb)) source = rgb;
    }

    if (rb_obj_is_kind_of(source, cColor))
    {
      pattern->pattern = shoes_color_pattern(source);
      pattern->width = pattern->height = 1.;
    }
    else
    {
      cairo_surface_t *surface = shoes_load_image(source);
      pattern->source = source;
      pattern->width = cairo_image_surface_get_width(surface);
      pattern->height = cairo_image_surface_get_height(surface);
      pattern->pattern = cairo_pattern_create_for_surface(surface);
    }
    cairo_pattern_set_extend(pattern->pattern, CAIRO_EXTEND_REPEAT);
  }

  pattern->attr = attr;
  pattern->parent = parent;
  return obj;
}

VALUE
shoes_pattern_method(VALUE klass, VALUE source)
{
  return shoes_pattern_new(cPattern, source, Qnil, Qnil);
}

VALUE
shoes_pattern_alloc(VALUE klass)
{
  VALUE obj;
  shoes_pattern *pattern = SHOE_ALLOC(shoes_pattern);
  SHOE_MEMZERO(pattern, shoes_pattern, 1);
  obj = Data_Wrap_Struct(klass, shoes_pattern_mark, shoes_pattern_free, pattern);
  pattern->source = Qnil;
  pattern->pattern = NULL;
  pattern->attr = Qnil;
  pattern->parent = Qnil;
  pattern->hover = 0;
  return obj;
}

VALUE
shoes_background_draw(VALUE self, VALUE c, VALUE actual)
{
  cairo_matrix_t matrix1, matrix2;
  double r = 0., sw = 1.;
  SETUP(shoes_pattern, REL_TILE, self_t->width, self_t->height);
  r = ATTR2(dbl, self_t->attr, curve, 0.);

  if (RTEST(actual))
  {
    cairo_save(canvas->cr);
    cairo_translate(canvas->cr, place.ix + place.dx, place.iy + place.dy);
    PATTERN_SCALE(self_t);
    shoes_cairo_rect(canvas->cr, 0, 0, place.iw, place.ih, r);
    cairo_set_source(canvas->cr, self_t->pattern);
    cairo_fill(canvas->cr);
    cairo_restore(canvas->cr);
    PATTERN_RESET(self_t);
  }

  INFO("BACKGROUND: (%d, %d), (%d, %d)\n", place.x, place.y, place.w, place.h);
  return self;
}

VALUE
shoes_border_draw(VALUE self, VALUE c, VALUE actual)
{
  cairo_matrix_t matrix1, matrix2;
  double r = 0., sw = 1.;
  SETUP(shoes_pattern, REL_TILE, self_t->width, self_t->height);
  r = ATTR2(dbl, self_t->attr, curve, 0.);
  sw = ATTR2(dbl, self_t->attr, strokewidth, 1.);

  place.iw -= sw;
  place.ih -= sw;
  place.ix += sw / 2.;
  place.iy += sw / 2.;

  if (RTEST(actual))
  {
    cairo_save(canvas->cr);
    cairo_translate(canvas->cr, place.ix + place.dx, place.iy + place.dy);
    PATTERN_SCALE(self_t);
    cairo_set_source(canvas->cr, self_t->pattern);
    shoes_cairo_rect(canvas->cr, 0, 0, place.iw, place.ih, r);
    cairo_set_antialias(canvas->cr, CAIRO_ANTIALIAS_NONE);
    cairo_set_line_width(canvas->cr, sw);
    cairo_stroke(canvas->cr);
    cairo_restore(canvas->cr);
    PATTERN_RESET(self_t);
  }

  INFO("BORDER: (%d, %d), (%d, %d)\n", place.x, place.y, place.w, place.h);
  return self;
}

VALUE
shoes_pattern_remove(VALUE self)
{
  GET_STRUCT(pattern, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 0, 0);
  return self;
}

VALUE
shoes_subpattern_new(VALUE klass, VALUE pat, VALUE parent)
{
  shoes_pattern *back, *pattern;
  VALUE obj = shoes_pattern_alloc(klass);
  Data_Get_Struct(obj, shoes_pattern, back);
  Data_Get_Struct(pat, shoes_pattern, pattern);
  back->source = pattern->source;
  back->pattern = pattern->pattern;
  cairo_pattern_reference(back->pattern);
  back->width = pattern->width;
  back->height = pattern->height;
  back->attr = pattern->attr;
  back->parent = parent;
  return obj;
}

//
// Shoes::Color
//
static void
shoes_color_mark(shoes_color *color)
{
}

static void
shoes_color_free(shoes_color *color)
{
  RUBY_CRITICAL(free(color));
}

VALUE
shoes_color_new(int r, int g, int b, int a)
{
  shoes_color *color;
  VALUE obj = shoes_color_alloc(cColor);
  Data_Get_Struct(obj, shoes_color, color);
  color->r = r;
  color->g = g;
  color->b = b;
  color->a = a;
  return obj;
}

VALUE
shoes_color_alloc(VALUE klass)
{
  VALUE obj;
  shoes_color *color = SHOE_ALLOC(shoes_color);
  SHOE_MEMZERO(color, shoes_color, 1);
  obj = Data_Wrap_Struct(klass, shoes_color_mark, shoes_color_free, color);
  color->r = 0x00;
  color->g = 0x00;
  color->b = 0x00;
  color->a = SHOES_COLOR_OPAQUE;
  color->on = TRUE;
  return obj;
}

VALUE
shoes_color_rgb(int argc, VALUE *argv, VALUE self)
{
  int a;
  VALUE _r, _g, _b, _a;
  rb_scan_args(argc, argv, "31", &_r, &_g, &_b, &_a);

  a = SHOES_COLOR_OPAQUE;
  if (!NIL_P(_a)) a = NUM2RGBINT(_a);
  return shoes_color_new(NUM2RGBINT(_r), NUM2RGBINT(_g), NUM2RGBINT(_b), a);
}

VALUE
shoes_color_gradient(int argc, VALUE *argv, VALUE self)
{
  shoes_pattern *pattern;
  VALUE obj, r1, r2;
  VALUE attr = Qnil;
  rb_scan_args(argc, argv, "21", &r1, &r2, &attr);

  obj = shoes_pattern_alloc(cPattern);
  Data_Get_Struct(obj, shoes_pattern, pattern);
  pattern->source = Qnil;
  shoes_pattern_gradient(pattern, r1, r2, attr);
  return obj;
}

VALUE
shoes_color_gray(int argc, VALUE *argv, VALUE self)
{
  shoes_color *color;
  VALUE _g, _a;
  int g, a;
  rb_scan_args(argc, argv, "02", &_g, &_a);

  a = SHOES_COLOR_OPAQUE;
  g = 128;
  if (!NIL_P(_g)) g = NUM2RGBINT(_g);
  if (!NIL_P(_a)) a = NUM2RGBINT(_a);
  return shoes_color_new(g, g, g, a);
}

cairo_pattern_t *
shoes_color_pattern(VALUE self)
{
  GET_STRUCT(color, color);
  if (color->a == 255)
    return cairo_pattern_create_rgb(color->r / 255., color->g / 255., color->b / 255.);
  else
    return cairo_pattern_create_rgba(color->r / 255., color->g / 255., color->b / 255., color->a / 255.);
}

void
shoes_color_grad_stop(cairo_pattern_t *pattern, double stop, VALUE self)
{
  GET_STRUCT(color, color);
  if (color->a == 255)
    return cairo_pattern_add_color_stop_rgb(pattern, stop, color->r / 255., color->g / 255., color->b / 255.);
  else
    return cairo_pattern_add_color_stop_rgba(pattern, stop, color->r / 255., color->g / 255., color->b / 255., color->a / 255.);
}

VALUE
shoes_color_args(int argc, VALUE *argv, VALUE self)
{
  VALUE _r, _g, _b, _a, _color;
  argc = rb_scan_args(argc, argv, "13", &_r, &_g, &_b, &_a);

  if (argc == 1 && rb_obj_is_kind_of(_r, cColor))
    _color = _r;
  else if (argc == 1 && rb_obj_is_kind_of(_r, rb_cString))
    _color = shoes_color_parse(cColor, _r);
  else if (argc == 1 || argc == 2)
    _color = shoes_color_gray(argc, argv, cColor);
  else
    _color = shoes_color_rgb(argc, argv, cColor);

  return _color;
}

#define NEW_COLOR(v, o) \
  shoes_color *v; \
  VALUE o = shoes_color_alloc(cColor); \
  Data_Get_Struct(o, shoes_color, v)

VALUE
shoes_color_parse(VALUE self, VALUE source)
{
  VALUE reg;

  reg = rb_funcall(source, s_match, 1, reHEX3_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = NUM2INT(rb_str2inum(rb_reg_nth_match(1, reg), 16)) * 17;
    color->g = NUM2INT(rb_str2inum(rb_reg_nth_match(2, reg), 16)) * 17;
    color->b = NUM2INT(rb_str2inum(rb_reg_nth_match(3, reg), 16)) * 17;
    return obj;
  }

  reg = rb_funcall(source, s_match, 1, reHEX_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = NUM2INT(rb_str2inum(rb_reg_nth_match(1, reg), 16));
    color->g = NUM2INT(rb_str2inum(rb_reg_nth_match(2, reg), 16));
    color->b = NUM2INT(rb_str2inum(rb_reg_nth_match(3, reg), 16));
    return obj;
  }

  reg = rb_funcall(source, s_match, 1, reRGB_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = NUM2INT(rb_Integer(rb_reg_nth_match(1, reg)));
    color->g = NUM2INT(rb_Integer(rb_reg_nth_match(2, reg)));
    color->b = NUM2INT(rb_Integer(rb_reg_nth_match(3, reg)));
    return obj;
  }

  reg = rb_funcall(source, s_match, 1, reRGBA_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = NUM2INT(rb_Integer(rb_reg_nth_match(1, reg)));
    color->g = NUM2INT(rb_Integer(rb_reg_nth_match(2, reg)));
    color->b = NUM2INT(rb_Integer(rb_reg_nth_match(3, reg)));
    color->a = NUM2INT(rb_Integer(rb_reg_nth_match(4, reg)));
    return obj;
  }

  reg = rb_funcall(source, s_match, 1, reGRAY_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = color->g = color->b = NUM2INT(rb_Integer(rb_reg_nth_match(1, reg)));
    return obj;
  }

  reg = rb_funcall(source, s_match, 1, reGRAYA_SOURCE);
  if (!NIL_P(reg))
  {
    NEW_COLOR(color, obj);
    color->r = color->g = color->b = NUM2INT(rb_Integer(rb_reg_nth_match(1, reg)));
    color->a = NUM2INT(rb_Integer(rb_reg_nth_match(2, reg)));
    return obj;
  }

  return Qnil;
}

VALUE
shoes_color_is_black(VALUE self)
{
  GET_STRUCT(color, color);
  return (color->r + color->g + color->b == 0) ? Qtrue : Qfalse;
}

VALUE
shoes_color_is_dark(VALUE self)
{
  GET_STRUCT(color, color);
  return ((int)color->r + (int)color->g + (int)color->b < SHOES_COLOR_DARK) ? Qtrue : Qfalse;
}

VALUE
shoes_color_is_light(VALUE self)
{
  GET_STRUCT(color, color);
  return ((int)color->r + (int)color->g + (int)color->b > SHOES_COLOR_LIGHT) ? Qtrue : Qfalse;
}

VALUE
shoes_color_is_opaque(VALUE self)
{
  GET_STRUCT(color, color);
  return (color->a == SHOES_COLOR_OPAQUE) ? Qtrue : Qfalse;
}

VALUE
shoes_color_is_transparent(VALUE self)
{
  GET_STRUCT(color, color);
  return (color->a == SHOES_COLOR_TRANSPARENT) ? Qtrue : Qfalse;
}

VALUE
shoes_color_is_white(VALUE self)
{
  GET_STRUCT(color, color);
  return (color->r + color->g + color->b == 765) ? Qtrue : Qfalse;
}

VALUE
shoes_color_invert(VALUE self)
{
  GET_STRUCT(color, color);
  NEW_COLOR(color2, obj);
  color2->r = 255 - color->r; color2->g = 255 - color->g; color2->b = 255 - color->b;
  return obj;
}

VALUE
shoes_color_to_s(VALUE self)
{
  GET_STRUCT(color, color);

  VALUE ary = rb_ary_new3(4, 
    INT2NUM(color->r), INT2NUM(color->g), INT2NUM(color->b), 
    rb_float_new((color->a * 1.) / 255.));

  if (color->a == 255)
    return rb_funcall(rb_str_new2("rgb(%d, %d, %d)"), s_perc, 1, ary);
  else
    return rb_funcall(rb_str_new2("rgb(%d, %d, %d, %0.1f)"), s_perc, 1, ary);
}

VALUE
shoes_color_to_pattern(VALUE self)
{
  return shoes_pattern_method(cPattern, self);
}

VALUE
shoes_color_method_missing(int argc, VALUE *argv, VALUE self)
{
  VALUE alpha;
  VALUE cname = argv[0];
  VALUE c = Qnil;
  if (rb_obj_is_kind_of(cColors, rb_cHash))
    c = rb_hash_aref(cColors, cname);
  if (NIL_P(c))
  {
    self = rb_inspect(self);
    rb_raise(rb_eNoMethodError, "undefined method `%s' for %s", 
      rb_id2name(SYM2ID(cname)), RSTRING_PTR(self));
  }

  rb_scan_args(argc, argv, "11", &cname, &alpha);
  if (!NIL_P(alpha))
  {
    shoes_color *color;
    Data_Get_Struct(c, shoes_color, color);
    c = shoes_color_new(color->r, color->g, color->b, NUM2RGBINT(alpha));
  }

  return c;
}

VALUE
shoes_app_method_missing(int argc, VALUE *argv, VALUE self)
{
  VALUE cname, canvas;
  GET_STRUCT(app, app);

  cname = argv[0];
  canvas = rb_ary_entry(app->nesting, RARRAY_LEN(app->nesting) - 1);
  if (!NIL_P(canvas) && rb_respond_to(canvas, SYM2ID(cname)))
  {
    if (rb_block_given_p())
      return ts_funcall2(canvas, SYM2ID(cname), argc - 1, argv + 1);
    else
      return rb_funcall2(canvas, SYM2ID(cname), argc - 1, argv + 1);
  }

  return shoes_color_method_missing(argc, argv, self);
}

//
// Shoes::LinkUrl
//
static void
shoes_link_mark(shoes_link *link)
{
}

static void
shoes_link_free(shoes_link *link)
{
  RUBY_CRITICAL(free(link));
}

VALUE
shoes_link_new(VALUE ele, int start, int end)
{
  shoes_link *link;
  VALUE obj = shoes_link_alloc(cLinkUrl);
  Data_Get_Struct(obj, shoes_link, link);
  link->ele = ele;
  link->start = start;
  link->end = end;
  return obj;
}

VALUE
shoes_link_alloc(VALUE klass)
{
  VALUE obj;
  shoes_link *link = SHOE_ALLOC(shoes_link);
  SHOE_MEMZERO(link, shoes_link, 1);
  obj = Data_Wrap_Struct(klass, shoes_link_mark, shoes_link_free, link);
  link->ele = Qnil;
  link->start = 0;
  link->end = 0;
  return obj;
}

static VALUE
shoes_link_at(VALUE self, int index, int blockhover, VALUE *clicked, char *touch)
{
  char h = 0;
  VALUE url = Qnil;
  shoes_text *self_t;

  GET_STRUCT(link, link);
  Data_Get_Struct(link->ele, shoes_text, self_t);
  if (blockhover && link->start <= index && link->end >= index)
  {
    h = 1;
    if (clicked != NULL) *clicked = link->ele;
    if (!NIL_P(self_t->attr)) url = rb_hash_aref(self_t->attr, ID2SYM(s_click));
  }

  CHECK_HOVER(self_t, h, touch);

  return url;
}

//
// Shoes::Text
//
static void
shoes_text_mark(shoes_text *text)
{
  rb_gc_mark_maybe(text->texts);
  rb_gc_mark_maybe(text->attr);
  rb_gc_mark_maybe(text->parent);
}

static void
shoes_text_free(shoes_text *text)
{
  RUBY_CRITICAL(free(text));
}

static VALUE
shoes_text_check(VALUE texts, VALUE parent)
{
  long i;
  for (i = 0; i < RARRAY_LEN(texts); i++)
  {
    VALUE ele = rb_ary_entry(texts, i);
    if (rb_obj_is_kind_of(ele, cTextClass))
    {
      shoes_text *text;
      Data_Get_Struct(ele, shoes_text, text);
      text->parent = parent;
    }
  }
  return texts;
}

VALUE
shoes_text_new(VALUE klass, VALUE texts, VALUE attr)
{
  shoes_text *text;
  VALUE obj = shoes_text_alloc(klass);
  Data_Get_Struct(obj, shoes_text, text);
  text->hover = 0;
  text->texts = shoes_text_check(texts, obj);
  text->attr = attr;
  return obj;
}

VALUE
shoes_text_alloc(VALUE klass)
{
  VALUE obj;
  shoes_text *text = SHOE_ALLOC(shoes_text);
  SHOE_MEMZERO(text, shoes_text, 1);
  obj = Data_Wrap_Struct(klass, shoes_text_mark, shoes_text_free, text);
  text->texts = Qnil;
  text->attr = Qnil;
  text->parent = Qnil;
  text->hover = 0;
  return obj;
}

VALUE
shoes_text_parent(VALUE self)
{
  GET_STRUCT(text, text);
  return text->parent;
}

VALUE
shoes_text_children(VALUE self)
{
  GET_STRUCT(text, text);
  return text->texts;
}

//
// Shoes::TextBlock
//
static void
shoes_textblock_mark(shoes_textblock *text)
{
  rb_gc_mark_maybe(text->string);
  rb_gc_mark_maybe(text->texts);
  rb_gc_mark_maybe(text->links);
  rb_gc_mark_maybe(text->attr);
  rb_gc_mark_maybe(text->parent);
  rb_gc_mark_maybe(text->cursor);
}

static void
shoes_textblock_free(shoes_textblock *text)
{
  if (text->layout != NULL)
    g_object_unref(text->layout);
  RUBY_CRITICAL(free(text));
}

VALUE
shoes_textblock_new(VALUE klass, VALUE texts, VALUE attr, VALUE parent)
{
  shoes_canvas *canvas;
  shoes_textblock *text;
  VALUE obj = shoes_textblock_alloc(klass);
  Data_Get_Struct(obj, shoes_textblock, text);
  Data_Get_Struct(parent, shoes_canvas, canvas);
  text->texts = shoes_text_check(texts, obj);
  text->attr = attr;
  text->parent = parent;
  return obj;
}

VALUE
shoes_textblock_alloc(VALUE klass)
{
  VALUE obj;
  shoes_textblock *text = SHOE_ALLOC(shoes_textblock);
  SHOE_MEMZERO(text, shoes_textblock, 1);
  obj = Data_Wrap_Struct(klass, shoes_textblock_mark, shoes_textblock_free, text);
  text->string = Qnil;
  text->texts = Qnil;
  text->links = Qnil;
  text->attr = Qnil;
  text->parent = Qnil;
  text->layout = NULL;
  text->cursor = Qnil;
  text->hover = 0;
  return obj;
}

VALUE
shoes_textblock_children(VALUE self)
{
  GET_STRUCT(textblock, text);
  return text->texts;
}

VALUE
shoes_textblock_set_cursor(VALUE self, VALUE pos)
{
  GET_STRUCT(textblock, self_t);
  self_t->cursor = pos;
  return pos;
}

VALUE
shoes_textblock_get_cursor(VALUE self)
{
  GET_STRUCT(textblock, self_t);
  return self_t->cursor;
}

VALUE
shoes_textblock_remove(VALUE self)
{
  GET_STRUCT(textblock, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 0, 0);
  return self;
}

VALUE
shoes_textblock_string(VALUE self)
{
  GET_STRUCT(textblock, self_t);
  return self_t->string;
}

VALUE
shoes_textblock_replace(int argc, VALUE *argv, VALUE self)
{
  long i;
  VALUE texts, attr;
  GET_STRUCT(textblock, self_t);

  attr = Qnil;
  texts = rb_ary_new();
  for (i = 0; i < argc; i++)
  {
    if (rb_obj_is_kind_of(argv[i], rb_cHash))
      attr = argv[i];
    else
      rb_ary_push(texts, argv[i]);
  }

  self_t->texts = texts;
  if (!NIL_P(attr)) self_t->attr = attr;

  shoes_canvas_repaint_all(self_t->parent);
  return self;
}

static VALUE
shoes_textblock_send_hover(VALUE self, int x, int y, VALUE *clicked, char *t)
{
  VALUE url = Qnil;
  int index, trailing, i, hover;
  GET_STRUCT(textblock, self_t);
  if (self_t->layout == NULL || NIL_P(self_t->links)) return Qnil;

  x -= self_t->place.ix + self_t->place.dx;
  y -= self_t->place.iy + self_t->place.dy;
  hover = pango_layout_xy_to_index(self_t->layout, x * PANGO_SCALE, y * PANGO_SCALE, &index, &trailing);
  if (hover)
  {
    INFO("HOVER (%d, %d) OVER (%d, %d)\n", x, y, self_t->place.ix + self_t->place.dx, self_t->place.iy + self_t->place.dy);
  }
  for (i = 0; i < RARRAY_LEN(self_t->links); i++)
  {
    VALUE urll = shoes_link_at(rb_ary_entry(self_t->links, i), index, hover, clicked, t);
    if (NIL_P(url)) url = urll;
  }

  return url;
}

VALUE
shoes_textblock_motion(VALUE self, int x, int y, char *t)
{
  VALUE url = shoes_textblock_send_hover(self, x, y, NULL, t);
  if (!NIL_P(url))
  {
    shoes_canvas *canvas;
    GET_STRUCT(textblock, self_t);
    Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
    shoes_app_cursor(canvas->app, s_hand);
  }
  return url;
}

VALUE
shoes_textblock_send_click(VALUE self, int button, int x, int y, VALUE *clicked)
{
  VALUE v = Qnil;

  if (button == 1)
  {
    GET_STRUCT(textblock, self_t);
    v = shoes_textblock_send_hover(self, x, y, clicked, NULL);
    if (self_t->hover & HOVER_MOTION)
      self_t->hover = HOVER_MOTION | HOVER_CLICK;
  }

  return v;
}

void
shoes_textblock_send_release(VALUE self, int button, int x, int y)
{
  GET_STRUCT(textblock, self_t);
  if (button == 1 && (self_t->hover & HOVER_CLICK))
  {
    VALUE proc = rb_hash_aref(self_t->attr, ID2SYM(s_release));
    self_t->hover ^= HOVER_CLICK;
    if (!NIL_P(proc))
      shoes_safe_block(self, proc, rb_ary_new3(1, self));
  }
}

//
// this is a secret structure that i got to name all by myself
//
typedef struct {
  shoes_app *app;
  PangoLayout *layout;
  PangoAttrList *attr;
  GString *text;
  gsize len;
  VALUE links;
} shoes_kxxxx;

#define APPLY_ATTR() \
  if (attr != NULL) { \
    attr->start_index = start_index; \
    attr->end_index = end_index; \
    pango_attr_list_insert_before(k->attr, attr); \
    attr = NULL; \
  }

#define GET_STYLE(name) \
  attr = NULL; \
  str = Qnil; \
  if (!NIL_P(oattr)) str = rb_hash_aref(oattr, ID2SYM(rb_intern("" # name))); \
  if (!NIL_P(hsh) && NIL_P(str)) str = rb_hash_aref(hsh, ID2SYM(rb_intern("" # name)))

#define APPLY_STYLE_COLOR(name, func) \
  GET_STYLE(name); \
  if (!NIL_P(str)) \
  { \
    if (TYPE(str) == T_STRING) \
      str = shoes_color_parse(cColor, str); \
    if (rb_obj_is_kind_of(str, cColor)) \
    { \
      shoes_color *color; \
      Data_Get_Struct(str, shoes_color, color); \
      attr = pango_attr_##func##_new(color->r * 255, color->g * 255, color-> b * 255); \
    } \
    APPLY_ATTR(); \
  }

static void
shoes_app_style_for(shoes_kxxxx *k, VALUE klass, VALUE oattr, gsize start_index, gsize end_index)
{
  VALUE str;
  VALUE hsh = rb_hash_aref(k->app->styles, klass);
  if (NIL_P(hsh) && NIL_P(oattr)) return;

  PangoAttrList *list = pango_attr_list_new();
  PangoAttribute *attr = NULL;

  if (rb_class_inherited_p(klass, cTextBlock))
  {
    GET_STYLE(justify);
    if (!NIL_P(str))
    {
      pango_layout_set_justify(k->layout, RTEST(str));
    }

    GET_STYLE(align);
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "left", 4) == 0)
        pango_layout_set_alignment(k->layout, PANGO_ALIGN_LEFT);
      else if (strncmp(RSTRING_PTR(str), "center", 6) == 0)
        pango_layout_set_alignment(k->layout, PANGO_ALIGN_CENTER);
      else if (strncmp(RSTRING_PTR(str), "right", 5) == 0)
        pango_layout_set_alignment(k->layout, PANGO_ALIGN_RIGHT);
    }
  }

  APPLY_STYLE_COLOR(stroke, foreground);
  APPLY_STYLE_COLOR(fill, background);
  APPLY_STYLE_COLOR(strikecolor, strikethrough_color);
  APPLY_STYLE_COLOR(undercolor, underline_color);

  GET_STYLE(font);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      attr = pango_attr_font_desc_new(pango_font_description_from_string(RSTRING_PTR(str)));
    }
    APPLY_ATTR();
  }

  GET_STYLE(size);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "xx-small", 8) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_XX_SMALL);
      else if (strncmp(RSTRING_PTR(str), "x-small", 7) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_X_SMALL);
      else if (strncmp(RSTRING_PTR(str), "small", 5) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_SMALL);
      else if (strncmp(RSTRING_PTR(str), "medium", 6) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_MEDIUM);
      else if (strncmp(RSTRING_PTR(str), "large", 5) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_LARGE);
      else if (strncmp(RSTRING_PTR(str), "x-large", 7) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_X_LARGE);
      else if (strncmp(RSTRING_PTR(str), "xx-large", 8) == 0)
        attr = pango_attr_scale_new(PANGO_SCALE_XX_LARGE);
      else
        str = rb_funcall(str, s_to_i, 0);
    }
    if (TYPE(str) == T_FIXNUM)
    {
      int i = NUM2INT(str);
      if (i > 0)
        attr = pango_attr_size_new_absolute(i * PANGO_SCALE * (96./72.));
    }
    APPLY_ATTR();
  }

  GET_STYLE(family);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      attr = pango_attr_family_new(RSTRING_PTR(str));
    }
    APPLY_ATTR();
  }

  GET_STYLE(weight);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "ultralight", 10) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_ULTRALIGHT);
      else if (strncmp(RSTRING_PTR(str), "light", 5) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_LIGHT);
      else if (strncmp(RSTRING_PTR(str), "normal", 6) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_NORMAL);
      else if (strncmp(RSTRING_PTR(str), "semibold", 8) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_SEMIBOLD);
      else if (strncmp(RSTRING_PTR(str), "bold", 4) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_BOLD);
      else if (strncmp(RSTRING_PTR(str), "ultrabold", 9) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_ULTRABOLD);
      else if (strncmp(RSTRING_PTR(str), "heavy", 5) == 0)
        attr = pango_attr_weight_new(PANGO_WEIGHT_HEAVY);
    }
    else if (TYPE(str) == T_FIXNUM)
    {
      int i = NUM2INT(str);
      if (i >= 100 && i <= 900)
        attr = pango_attr_weight_new((PangoWeight)i);
    }
    APPLY_ATTR();
  }

  GET_STYLE(rise);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
      str = rb_funcall(str, s_to_i, 0);
    if (TYPE(str) == T_FIXNUM)
    {
      int i = NUM2INT(str);
      attr = pango_attr_rise_new(i * PANGO_SCALE);
    }
    APPLY_ATTR();
  }

  GET_STYLE(kerning);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
      str = rb_funcall(str, s_to_i, 0);
    if (TYPE(str) == T_FIXNUM)
    {
      int i = NUM2INT(str);
      attr = pango_attr_letter_spacing_new(i * PANGO_SCALE);
    }
    APPLY_ATTR();
  }

  GET_STYLE(emphasis);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "normal", 6) == 0)
        attr = pango_attr_style_new(PANGO_STYLE_NORMAL);
      else if (strncmp(RSTRING_PTR(str), "oblique", 7) == 0)
        attr = pango_attr_style_new(PANGO_STYLE_OBLIQUE);
      else if (strncmp(RSTRING_PTR(str), "italic", 6) == 0)
        attr = pango_attr_style_new(PANGO_STYLE_ITALIC);
    }
    APPLY_ATTR();
  }

  GET_STYLE(strikethrough);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "none", 4) == 0)
        attr = pango_attr_strikethrough_new(FALSE);
      else if (strncmp(RSTRING_PTR(str), "single", 6) == 0)
        attr = pango_attr_strikethrough_new(TRUE);
    }
    APPLY_ATTR();
  }

  GET_STYLE(stretch);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "condensed", 9) == 0)
        attr = pango_attr_stretch_new(PANGO_STRETCH_CONDENSED);
      else if (strncmp(RSTRING_PTR(str), "normal", 6) == 0)
        attr = pango_attr_stretch_new(PANGO_STRETCH_NORMAL);
      else if (strncmp(RSTRING_PTR(str), "expanded", 8) == 0)
        attr = pango_attr_stretch_new(PANGO_STRETCH_EXPANDED);
    }
    APPLY_ATTR();
  }

  GET_STYLE(underline);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "none", 4) == 0)
        attr = pango_attr_underline_new(PANGO_UNDERLINE_NONE);
      else if (strncmp(RSTRING_PTR(str), "single", 6) == 0)
        attr = pango_attr_underline_new(PANGO_UNDERLINE_SINGLE);
      else if (strncmp(RSTRING_PTR(str), "double", 6) == 0)
        attr = pango_attr_underline_new(PANGO_UNDERLINE_DOUBLE);
      else if (strncmp(RSTRING_PTR(str), "low", 3) == 0)
        attr = pango_attr_underline_new(PANGO_UNDERLINE_LOW);
      else if (strncmp(RSTRING_PTR(str), "error", 5) == 0)
        attr = pango_attr_underline_new(PANGO_UNDERLINE_ERROR);
    }
    APPLY_ATTR();
  }

  GET_STYLE(variant);
  if (!NIL_P(str))
  {
    if (TYPE(str) == T_STRING)
    {
      if (strncmp(RSTRING_PTR(str), "normal", 6) == 0)
        attr = pango_attr_variant_new(PANGO_VARIANT_NORMAL);
      else if (strncmp(RSTRING_PTR(str), "smallcaps", 9) == 0)
        attr = pango_attr_variant_new(PANGO_VARIANT_SMALL_CAPS);
    }
    APPLY_ATTR();
  }
}

static void
shoes_textblock_iter_pango(VALUE texts, shoes_kxxxx *k)
{
  VALUE v;
  long i;

  if (NIL_P(texts))
    return;

  for (i = 0; i < RARRAY_LEN(texts); i++)
  {
    v = rb_ary_entry(texts, i);
    if (rb_obj_is_kind_of(v, cTextClass))
    {
      VALUE tklass = rb_obj_class(v);
      gsize start;
      shoes_text *text;
      Data_Get_Struct(v, shoes_text, text);

      start = k->len;
      shoes_textblock_iter_pango(text->texts, k);
      if ((text->hover & HOVER_MOTION) && tklass == cLink)
        tklass = cLinkHover;
      shoes_app_style_for(k, tklass, text->attr, start, k->len);
      if (rb_obj_is_kind_of(v, cLink) && !NIL_P(text->attr))
      {
        rb_ary_push(k->links, shoes_link_new(v, start, k->len));
      }
    }
    else if (rb_obj_is_kind_of(v, rb_cArray))
    {
      shoes_textblock_iter_pango(v, k);
    }
    else
    {
      v = rb_funcall(v, s_to_s, 0);
      k->len += RSTRING_LEN(v); 
      g_string_append_len(k->text, RSTRING_PTR(v), RSTRING_LEN(v));
    }
  }
}

static void
shoes_textblock_make_pango(shoes_app *app, VALUE klass, shoes_textblock *block, PangoAttrList **attr_list, char **text)
{
  shoes_kxxxx *k = SHOE_ALLOC(shoes_kxxxx);
  k->attr = pango_attr_list_new();
  k->text = g_string_new(NULL);
  k->len = 0;
  k->app = app;
  k->layout = block->layout;
  block->links = k->links = rb_ary_new();

  shoes_textblock_iter_pango(block->texts, k);
  shoes_app_style_for(k, klass, block->attr, 0, k->len);

  *attr_list = k->attr;
  *text = g_string_free(k->text, FALSE);
  SHOE_FREE(k);
}

static void
shoes_textblock_on_layout(shoes_app *app, VALUE klass, shoes_textblock *block)
{
  PangoAttrList *list = NULL;
  char *text = NULL;
  
  g_return_if_fail(PANGO_IS_LAYOUT(block->layout));
  g_return_if_fail(block != NULL);
  
  shoes_textblock_make_pango(app, klass, block, &list, &text);
  block->string = rb_str_new2(text);
  pango_layout_set_text(block->layout, text, -1);
  pango_layout_set_attributes(block->layout, list);
  pango_attr_list_unref(list);
  g_free(text);
}

VALUE
shoes_textblock_draw(VALUE self, VALUE c, VALUE actual)
{
  int px, py, pd, li, m, ld;
  double cx, cy;
  char *font;
  shoes_canvas *canvas;
  PangoLayoutLine *last;
  PangoRectangle lrect;
  PangoFontDescription *desc;

  VALUE ck = rb_obj_class(c);
  GET_STRUCT(textblock, self_t);
  Data_Get_Struct(c, shoes_canvas, canvas);

  if (!NIL_P(self_t->attr) && ATTR(self_t->attr, hidden) == Qtrue)
  {
    rb_ary_clear(self_t->links);
    return self;
  }

  ATTR_MARGINS(self_t->attr, 4);
  if (NIL_P(ATTR(self_t->attr, margin)) && NIL_P(ATTR(self_t->attr, margin_bottom)))
    bmargin = 12;
  self_t->place.flags = REL_CANVAS;
  self_t->place.flags |= NIL_P(ATTR(self_t->attr, left)) && NIL_P(ATTR(self_t->attr, right)) ? 0 : FLAG_ABSX;
  self_t->place.flags |= NIL_P(ATTR(self_t->attr, top)) && NIL_P(ATTR(self_t->attr, bottom)) ? 0 : FLAG_ABSY;
  self_t->place.x = ATTR2(int, self_t->attr, left, canvas->cx);
  self_t->place.y = ATTR2(int, self_t->attr, top, canvas->cy);
  if (!ORIGIN(canvas->place))
  {
    self_t->place.dx = canvas->place.dx; 
    self_t->place.dy = canvas->place.dy;
  }
  else
  {
    self_t->place.dx = 0;
    self_t->place.dy = 0;
  }
  self_t->place.dx += PXN(self_t->attr, displace_left, 0, CPW(canvas)); 
  self_t->place.dy += PXN(self_t->attr, displace_top, 0, CPH(canvas));
  self_t->place.w = ATTR2(int, self_t->attr, width, canvas->place.iw - (canvas->cx - self_t->place.x));
  self_t->place.iw = self_t->place.w - (lmargin + rmargin);
  ld = ATTR2(int, self_t->attr, leading, 4);

  if (self_t->layout != NULL)
    g_object_unref(self_t->layout);

  self_t->layout = pango_cairo_create_layout(canvas->cr);
  pd = 0;
  if (!ABSX(self_t->place) && self_t->place.x == canvas->cx)
  {
    if (self_t->place.x - CPX(canvas) > self_t->place.w)
    {
      self_t->place.x = CPX(canvas);
      canvas->cy = self_t->place.y = canvas->endy;
    }
    else
    {
      if (self_t->place.x > CPX(canvas)) {
        pd = self_t->place.x - CPX(canvas);
        pango_layout_set_indent(self_t->layout, pd * PANGO_SCALE);
        self_t->place.x = CPX(canvas);
      }
    }
  }

  pango_layout_set_width(self_t->layout, self_t->place.iw * PANGO_SCALE);
  pango_layout_set_spacing(self_t->layout, ld * PANGO_SCALE);
  shoes_textblock_on_layout(canvas->app, rb_obj_class(self), self_t);
  desc = pango_font_description_new();
  pango_font_description_set_family(desc, "Arial");
  pango_font_description_set_absolute_size(desc, 14. * PANGO_SCALE * (96./72.));
  pango_layout_set_font_description(self_t->layout, desc);
  pango_font_description_free(desc);

  //
  // Line up the first line with the y-cursor
  //
  if (!ABSX(self_t->place) && !ABSY(self_t->place) && pd)
  {
    last = pango_layout_get_line(self_t->layout, 0);
    pango_layout_line_get_pixel_extents(last, NULL, &lrect);
    if (lrect.width > self_t->place.iw - pd)
    {
      pango_layout_set_indent(self_t->layout, 0);
      self_t->place.x = CPX(canvas);
      canvas->cy = self_t->place.y = canvas->endy;
      pd = 0;
    }
  }
  self_t->place.ix = self_t->place.x + lmargin;
  self_t->place.iy = self_t->place.y + tmargin;

  li = pango_layout_get_line_count(self_t->layout) - 1;
  last = pango_layout_get_line(self_t->layout, li);
  pango_layout_line_get_pixel_extents(last, NULL, &lrect);
  pango_layout_get_pixel_size(self_t->layout, &px, &py);

  if (RTEST(actual))
  {
    cairo_move_to(canvas->cr, self_t->place.ix + self_t->place.dx, self_t->place.iy + self_t->place.dy);
    cairo_set_source_rgb(canvas->cr, 0., 0., 0.);
    pango_cairo_update_layout(canvas->cr, self_t->layout);
    pango_cairo_show_layout(canvas->cr, self_t->layout);

    // draw the cursor
    if (!NIL_P(self_t->cursor))
    {
      int cursor = NUM2INT(self_t->cursor);
      PangoRectangle crect;
      double crx, cry;
      if (cursor < 0) cursor += RSTRING_LEN(self_t->string) + 1;
      pango_layout_index_to_pos(self_t->layout, cursor, &crect);
      crx = (self_t->place.ix + self_t->place.dx) + (crect.x / PANGO_SCALE);
      cry = (self_t->place.iy + self_t->place.dy) + (crect.y / PANGO_SCALE);

      cairo_save(canvas->cr);
      cairo_new_path(canvas->cr);
      cairo_move_to(canvas->cr, crx, cry);
      cairo_line_to(canvas->cr, crx, cry + (crect.height / PANGO_SCALE));
      cairo_set_antialias(canvas->cr, CAIRO_ANTIALIAS_NONE);
      cairo_set_source_rgb(canvas->cr, 0., 0., 0.);
      cairo_set_line_width(canvas->cr, 1.);
      cairo_stroke(canvas->cr);
      cairo_restore(canvas->cr);
    }
  }

  self_t->place.ih = py;
  self_t->place.h = py + tmargin + bmargin;
  INFO("TEXT: %d, %d (%d, %d) / (%d: %d, %d: %d) %d, %d [%d]\n", canvas->cx, canvas->cy,
    canvas->place.w, canvas->height, self_t->place.x, self_t->place.ix,
    self_t->place.y, self_t->place.iy, self_t->place.w, self_t->place.h, pd);

  if (!ABSY(self_t->place)) {
    // newlines have an empty size
    if (ck != cStack) {
      if (li == 0) {
        canvas->cx = self_t->place.x + lrect.x + lrect.width + rmargin + pd;
      } else {
        canvas->cy = self_t->place.y + py - lrect.height;
        if (lrect.width == 0) {
          canvas->cx = self_t->place.x + lrect.x;
        } else {
          canvas->cx = self_t->place.x + lrect.width + rmargin;
        }
      }
    }

    canvas->endy = max(self_t->place.y + self_t->place.h, canvas->endy);
    canvas->endx = canvas->cx;

    if (ck == cStack || canvas->cx - CPX(canvas) > canvas->width) {
      canvas->cx = CPX(canvas);
      canvas->cy = canvas->endy;
    }
    if (NIL_P(ATTR(self_t->attr, margin)) && NIL_P(ATTR(self_t->attr, margin_top)))
      bmargin = lrect.height;

    INFO("CX: (%d, %d) / LRECT: (%d, %d) / END: (%d, %d)\n", 
      canvas->cx, canvas->cy,
      lrect.x, lrect.width,
      canvas->endx, canvas->endy);
  }
  return self;
}

//
// Shoes::Button
//
static void
shoes_control_mark(shoes_control *control)
{
  rb_gc_mark_maybe(control->parent);
  rb_gc_mark_maybe(control->attr);
}

static void
shoes_control_free(shoes_control *control)
{
  if (control->ref != NULL) shoes_native_control_free(control->ref);
  RUBY_CRITICAL(free(control));
}

VALUE
shoes_control_new(VALUE klass, VALUE attr, VALUE parent)
{
  shoes_control *control;
  VALUE obj = shoes_control_alloc(klass);
  Data_Get_Struct(obj, shoes_control, control);
  control->attr = attr;
  control->parent = parent;
  return obj;
}

VALUE
shoes_control_alloc(VALUE klass)
{
  VALUE obj;
  shoes_control *control = SHOE_ALLOC(shoes_control);
  SHOE_MEMZERO(control, shoes_control, 1);
  obj = Data_Wrap_Struct(klass, shoes_control_mark, shoes_control_free, control);
  control->place.ix = control->place.iy = control->place.iw = control->place.ih = 0;
  control->ref = NULL;
  control->attr = Qnil;
  control->parent = Qnil;
  return obj;
}

VALUE
shoes_control_focus(VALUE self)
{
  GET_STRUCT(control, self_t);
  ATTRSET(self_t->attr, hidden, Qtrue);
  if (self_t->ref != NULL) shoes_native_control_focus(self_t->ref);
  return self;
}

VALUE
shoes_control_hide(VALUE self)
{
  GET_STRUCT(control, self_t);
  ATTRSET(self_t->attr, hidden, Qtrue);
  if (self_t->ref != NULL) shoes_control_hide_ref(self_t->ref);
  return self;
}

VALUE
shoes_control_show(VALUE self)
{
  GET_STRUCT(control, self_t);
  ATTRSET(self_t->attr, hidden, Qfalse);
  if (self_t->ref != NULL) shoes_control_show_ref(self_t->ref);
  return self;
}

VALUE
shoes_control_remove(VALUE self)
{
  shoes_canvas *canvas;
  GET_STRUCT(control, self_t);
  shoes_canvas_remove_item(self_t->parent, self, 1, 0);

  Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
  if (self_t->ref != NULL) shoes_native_control_remove(self_t->ref, canvas);
  return self;
}

void
shoes_control_send(VALUE self, ID event)
{
  VALUE click;
  GET_STRUCT(control, self_t);

  if (!NIL_P(self_t->attr))
  {
    click = rb_hash_aref(self_t->attr, ID2SYM(event));
    if (!NIL_P(click))
      shoes_safe_block(self_t->parent, click, rb_ary_new3(1, self));
  }
}

VALUE
shoes_button_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(2, 0);

#ifdef SHOES_QUARTZ
  place.h += 8;
  place.ih += 8;
#endif
  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_button(self, canvas, &place, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_edit_line_get_text(VALUE self)
{
  GET_STRUCT(control, self_t);
  if (self_t->ref == NULL) return Qnil;
  return shoes_native_edit_line_get_text(self_t->ref);
}

VALUE
shoes_edit_line_set_text(VALUE self, VALUE text)
{
  char *msg = "";
  GET_STRUCT(control, self_t);
  if (!NIL_P(text))
  {
    text = shoes_native_to_s(text);
    msg = RSTRING_PTR(text);
  }
  shoes_native_edit_line_set_text(self_t->ref, msg);
  return text;
}

VALUE
shoes_edit_line_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(0, 0);

#ifdef SHOES_QUARTZ
  place.x += 4;
  place.ix += 4;
#endif
  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_edit_line(self, canvas, &place, self_t->attr, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_edit_box_get_text(VALUE self)
{
  GET_STRUCT(control, self_t);
  if (self_t->ref == NULL) return Qnil;
  return shoes_native_edit_box_get_text(self_t->ref);
}

VALUE
shoes_edit_box_set_text(VALUE self, VALUE text)
{
  char *msg = "";
  GET_STRUCT(control, self_t);
  if (!NIL_P(text))
  {
    text = shoes_native_to_s(text);
    msg = RSTRING_PTR(text);
  }
  shoes_native_edit_box_set_text(self_t->ref, msg);
  return text;
}


VALUE
shoes_edit_box_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(80, 0);

  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_edit_box(self, canvas, &place, self_t->attr, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_list_box_choose(VALUE self, VALUE item)
{
  VALUE text = Qnil, items = Qnil;
  int idx = -1;
  GET_STRUCT(control, self_t);
  if (self_t->ref == NULL) text = Qnil;

  items = ATTR(self_t->attr, items);
  shoes_native_list_box_set_active(self_t->ref, items, item);
  return self;
}

VALUE
shoes_list_box_text(VALUE self)
{
  GET_STRUCT(control, self_t);
  if (self_t->ref == NULL) return Qnil;
  return shoes_native_list_box_get_active(self_t->ref, ATTR(self_t->attr, items));
}

VALUE
shoes_list_box_items_get(VALUE self)
{
  GET_STRUCT(control, self_t);
  return ATTR(self_t->attr, items);
}

VALUE
shoes_list_box_items_set(VALUE self, VALUE items)
{
  VALUE opt = shoes_list_box_text(self);
  GET_STRUCT(control, self_t);
  ATTRSET(self_t->attr, items, items);
  shoes_native_list_box_update(self_t->ref, items);
  shoes_list_box_choose(self, opt);
  return items;
}

VALUE
shoes_list_box_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(0, 0);

#ifdef SHOES_QUARTZ
  place.h += 4;
  place.ih += 4;
#endif
  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      VALUE items = ATTR(self_t->attr, items);
      self_t->ref = shoes_native_list_box(self, canvas, &place, self_t->attr, msg);

      if (!NIL_P(items))
      {
        shoes_native_list_box_update(self_t->ref, items);
        if (!NIL_P(ATTR(self_t->attr, choose)))
          shoes_native_list_box_set_active(self_t->ref, items, ATTR(self_t->attr, choose));
      }

      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_progress_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(0, 0);

  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_progress(self, canvas, &place, self_t->attr, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_progress_get_fraction(VALUE self)
{
  double perc = 0.;
  GET_STRUCT(control, self_t);
  if (self_t->ref != NULL)
    perc = shoes_native_progress_get_fraction(self_t->ref);
  return rb_float_new(perc);
}

VALUE
shoes_progress_set_fraction(VALUE self, VALUE _perc)
{
  double perc = NUM2DBL(_perc);
  GET_STRUCT(control, self_t);
  if (self_t->ref != NULL)
    shoes_native_progress_set_fraction(self_t->ref, perc);
  return self;
}

VALUE
shoes_check_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(0, 20);

  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_check(self, canvas, &place, self_t->attr, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

  FINISH();

  return self;
}

VALUE
shoes_check_is_checked(VALUE self)
{
  GET_STRUCT(control, self_t);
  return shoes_native_check_get(self_t->ref);
}

VALUE
shoes_check_set_checked(VALUE self, VALUE on)
{
  GET_STRUCT(control, self_t);
  shoes_native_check_set(self_t->ref, RTEST(on));
  return on;
}

VALUE
shoes_radio_draw(VALUE self, VALUE c, VALUE actual)
{
  SETUP_CONTROL(0, 20);

  if (RTEST(actual))
  {
    if (self_t->ref == NULL)
    {
      self_t->ref = shoes_native_radio(self, canvas, &place, self_t->attr, msg);
      shoes_native_control_position(self_t->ref, &self_t->place, self, canvas, &place);
    }
    else
      shoes_native_control_repaint(self_t->ref, &self_t->place, canvas, &place);
  }

#ifdef SHOES_GTK
    if (canvas->group.radios == NULL && self_t->ref != NULL)
      canvas->group.radios = self_t->ref;
#endif

  FINISH();

  return self;
}

//
// Transformations
//
#define TRANS_COMMON(ele, repaint) \
  VALUE \
  shoes_##ele##_transform(VALUE self, VALUE _m) \
  { \
    GET_STRUCT(ele, self_t); \
    ID m = SYM2ID(_m); \
    if (m == s_center || m == s_corner) \
    { \
      self_t->mode = m; \
    } \
    else \
    { \
      rb_raise(rb_eArgError, "transform must be called with either :center or :corner."); \
    } \
    return self; \
  } \
  VALUE \
  shoes_##ele##_translate(VALUE self, VALUE _x, VALUE _y) \
  { \
    GET_STRUCT(ele, self_t); \
    double x, y; \
    x = NUM2DBL(_x); \
    y = NUM2DBL(_y); \
    cairo_matrix_translate(self_t->tf, x, y); \
    return self; \
  } \
  VALUE \
  shoes_##ele##_rotate(VALUE self, VALUE _deg) \
  { \
    GET_STRUCT(ele, self_t); \
    double rad; \
    rad = NUM2DBL(_deg) * SHOES_RAD2PI; \
    cairo_matrix_rotate(self_t->tf, -rad); \
    if (repaint) shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  VALUE \
  shoes_##ele##_scale(int argc, VALUE *argv, VALUE self) \
  { \
    VALUE _sx, _sy; \
    double sx, sy; \
    GET_STRUCT(ele, self_t); \
    rb_scan_args(argc, argv, "11", &_sx, &_sy); \
    sx = NUM2DBL(_sx); \
    if (NIL_P(_sy)) sy = sx; \
    else            sy = NUM2DBL(_sy); \
    cairo_matrix_scale(self_t->tf, sx, sy); \
    if (repaint) shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  VALUE \
  shoes_##ele##_skew(int argc, VALUE *argv, VALUE self) \
  { \
    cairo_matrix_t matrix; \
    VALUE _sx, _sy; \
    double sx, sy; \
    GET_STRUCT(ele, self_t); \
    rb_scan_args(argc, argv, "11", &_sx, &_sy); \
    sx = NUM2DBL(_sx) * SHOES_RAD2PI; \
    sy = 0.0; \
    if (!NIL_P(_sy)) sy = NUM2DBL(_sy) * SHOES_RAD2PI; \
    cairo_matrix_init(&matrix, 1.0, sy, sx, 1.0, 0.0, 0.0); \
    cairo_matrix_multiply(self_t->tf, self_t->tf, &matrix); \
    if (repaint) shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  }

TRANS_COMMON(canvas, 0);
TRANS_COMMON(image, 1);

//
// Common methods
//
#define EVENT_COMMON(ele, est, sym) \
  VALUE \
  shoes_##ele##_##sym(int argc, VALUE *argv, VALUE self) \
  { \
    VALUE str = Qnil, blk = Qnil; \
    GET_STRUCT(est, self_t); \
  \
    rb_scan_args(argc, argv, "01&", &str, &blk); \
    if (NIL_P(self_t->attr)) self_t->attr = rb_hash_new(); \
    rb_hash_aset(self_t->attr, ID2SYM(s_##sym), NIL_P(blk) ? str : blk ); \
    return self; \
  }

EVENT_COMMON(linktext, text, click);
EVENT_COMMON(linktext, text, release);
EVENT_COMMON(linktext, text, hover);
EVENT_COMMON(linktext, text, leave);

#define CLASS_COMMON(ele) \
  VALUE \
  shoes_##ele##_style(int argc, VALUE *argv, VALUE self) \
  { \
    VALUE attr; \
    GET_STRUCT(ele, self_t); \
    rb_scan_args(argc, argv, "01", &attr); \
    if (!NIL_P(attr)) \
    { \
      if (NIL_P(self_t->attr)) self_t->attr = rb_hash_new(); \
      rb_funcall(self_t->attr, s_update, 1, attr); \
      shoes_canvas_repaint_all(self_t->parent); \
    } \
    return self_t->attr; \
  } \
  \
  VALUE \
  shoes_##ele##_displace(VALUE self, VALUE x, VALUE y) \
  { \
    GET_STRUCT(ele, self_t); \
    ATTRSET(self_t->attr, displace_left, x); \
    ATTRSET(self_t->attr, displace_top, y); \
    shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  \
  VALUE \
  shoes_##ele##_move(VALUE self, VALUE x, VALUE y) \
  { \
    GET_STRUCT(ele, self_t); \
    ATTRSET(self_t->attr, left, x); \
    ATTRSET(self_t->attr, top, y); \
    shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  }

#define CLASS_COMMON2(ele) \
  VALUE \
  shoes_##ele##_hide(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    ATTRSET(self_t->attr, hidden, Qtrue); \
    shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  \
  VALUE \
  shoes_##ele##_show(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    ATTRSET(self_t->attr, hidden, Qfalse); \
    shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  \
  VALUE \
  shoes_##ele##_toggle(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    ATTRSET(self_t->attr, hidden, ATTR(self_t->attr, hidden) == Qtrue ? Qfalse : Qtrue); \
    shoes_canvas_repaint_all(self_t->parent); \
    return self; \
  } \
  CLASS_COMMON(ele); \
  EVENT_COMMON(ele, ele, change); \
  EVENT_COMMON(ele, ele, click); \
  EVENT_COMMON(ele, ele, release); \
  EVENT_COMMON(ele, ele, hover); \
  EVENT_COMMON(ele, ele, leave);

#define PLACE_COMMON(ele) \
  VALUE \
  shoes_##ele##_get_parent(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    return self_t->parent; \
  } \
  \
  VALUE \
  shoes_##ele##_get_left(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    return INT2NUM(self_t->place.x); \
  } \
  \
  VALUE \
  shoes_##ele##_get_top(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    return INT2NUM(self_t->place.y); \
  } \
  \
  VALUE \
  shoes_##ele##_get_height(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    return INT2NUM(self_t->place.h); \
  } \
  \
  VALUE \
  shoes_##ele##_get_width(VALUE self) \
  { \
    GET_STRUCT(ele, self_t); \
    return INT2NUM(self_t->place.w); \
  }

PLACE_COMMON(canvas)
PLACE_COMMON(control)
CLASS_COMMON(control)
EVENT_COMMON(control, control, click)
EVENT_COMMON(control, control, change)
CLASS_COMMON(text)
#ifdef VIDEO
PLACE_COMMON(video)
CLASS_COMMON(video)
#endif
PLACE_COMMON(image)
CLASS_COMMON2(image)
PLACE_COMMON(shape)
CLASS_COMMON2(shape)
CLASS_COMMON2(pattern)
PLACE_COMMON(textblock)
CLASS_COMMON2(textblock)

//
// Shoes::Timer
//
//
void
shoes_timer_call(VALUE self)
{
  GET_STRUCT(timer, timer);
  shoes_safe_block(timer->parent, timer->block, rb_ary_new3(1, INT2NUM(timer->frame)));
  timer->frame++;

  if (rb_obj_is_kind_of(self, cTimer))
  {
    timer->ref = 0;
    timer->started = ANIM_STOPPED;
  }
}

static void
shoes_timer_mark(shoes_timer *timer)
{
  rb_gc_mark_maybe(timer->block);
  rb_gc_mark_maybe(timer->parent);
}

static void
shoes_timer_free(shoes_timer *timer)
{
  RUBY_CRITICAL(free(timer));
}

VALUE
shoes_timer_new(VALUE klass, VALUE rate, VALUE block, VALUE parent)
{
  shoes_timer *timer;
  VALUE obj = shoes_timer_alloc(klass);
  Data_Get_Struct(obj, shoes_timer, timer);
  timer->block = block;
  if (!NIL_P(rate))
  {
    if (klass == cAnim)
      timer->rate = 1000 / NUM2INT(rate);
    else
      timer->rate = (int)(1000. * NUM2DBL(rate));
  }
  timer->parent = parent;
  return obj;
}

VALUE
shoes_timer_alloc(VALUE klass)
{
  VALUE obj;
  shoes_timer *timer = SHOE_ALLOC(shoes_timer);
  SHOE_MEMZERO(timer, shoes_timer, 1);
  obj = Data_Wrap_Struct(klass, shoes_timer_mark, shoes_timer_free, timer);
  timer->ref = 0;
  timer->block = Qnil;
  timer->rate = 1000 / 12;  // 12 frames per second
  timer->frame = 0;
  timer->parent = Qnil;
  timer->started = ANIM_NADA;
  return obj;
}

VALUE
shoes_timer_remove(VALUE self)
{
  GET_STRUCT(timer, self_t);
  shoes_timer_stop(self);
  shoes_canvas_remove_item(self_t->parent, self, 0, 1);
  return self;
}

VALUE
shoes_timer_stop(VALUE self)
{
  GET_STRUCT(timer, self_t);
  if (self_t->started == ANIM_STARTED)
  {
    shoes_canvas *canvas;
    Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
    shoes_native_timer_remove(canvas, self_t->ref);
    self_t->started = ANIM_PAUSED;
  }
  return self;
}

VALUE
shoes_timer_start(VALUE self)
{
  GET_STRUCT(timer, self_t);
  unsigned int interval = self_t->rate;
  if (self_t->started != ANIM_STARTED)
  {
    shoes_canvas *canvas;
    Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
    self_t->ref = shoes_native_timer_start(self, canvas, interval);
    self_t->started = ANIM_STARTED;
  }
  return self;
}

VALUE
shoes_timer_toggle(VALUE self)
{
  GET_STRUCT(timer, self_t);
  return self_t->started == ANIM_STARTED ? shoes_timer_stop(self) : shoes_timer_start(self);
}

VALUE
shoes_timer_draw(VALUE self, VALUE c, VALUE actual)
{
  shoes_canvas *canvas;
  GET_STRUCT(timer, self_t);
  Data_Get_Struct(self_t->parent, shoes_canvas, canvas);
  if (RTEST(actual) && self_t->started == ANIM_NADA)
  {
    self_t->frame = 0;
    shoes_timer_start(self);
  }
  return self;
}

void
shoes_msg(ID typ, VALUE str)
{
  ID func = rb_frame_last_func();
  rb_ary_push(shoes_world->msgs, rb_ary_new3(6, 
    ID2SYM(typ), str, rb_funcall(rb_cTime, s_now, 0),
    func ? ID2SYM(func) : Qnil, 
    rb_str_new2(ruby_sourcefile), INT2NUM(ruby_sourceline)));
}

#define DEBUG_TYPE(t) \
  VALUE \
  shoes_canvas_##t(VALUE self, VALUE str) \
  { \
    shoes_msg(s_##t, str); \
    return Qnil; \
  } \
  \
  void \
  shoes_##t(const char *fmt, ...) \
  { \
    va_list args; \
    char buf[BUFSIZ]; \
  \
    va_start(args,fmt); \
    vsnprintf(buf, BUFSIZ, fmt, args); \
    va_end(args); \
    shoes_msg(s_##t, rb_str_new2(buf)); \
  }

DEBUG_TYPE(info);
DEBUG_TYPE(debug);
DEBUG_TYPE(warn);
DEBUG_TYPE(error);

VALUE
shoes_p(VALUE self, VALUE obj)
{
  return shoes_canvas_debug(self, rb_inspect(obj));
}

VALUE
shoes_log(VALUE self)
{
  return shoes_world->msgs;
}

//
// Defines a redirecting function which applies the element or transformation
// to the currently active canvas.  This is used in place of the old instance_eval
// and ensures that you have access to the App's instance variables while
// assembling elements in a layout.
//
#define FUNC_M(name, func, argn) \
  VALUE \
  shoes_canvas_c_##func(int argc, VALUE *argv, VALUE self) \
  { \
    VALUE canvas, obj; \
    GET_STRUCT(canvas, self_t); \
    char *n = name; cairo_t *cr = self_t->cr; \
    if (rb_ary_entry(self_t->app->nesting, 0) == self) \
      canvas = rb_ary_entry(self_t->app->nesting, RARRAY_LEN(self_t->app->nesting) - 1); \
    else \
      canvas = self; \
    if (cr == NULL && n[0] == '+') shoes_canvas_memdraw_begin(canvas); \
    obj = call_cfunc(CASTHOOK(shoes_canvas_##func), canvas, argn, argc, argv); \
    if (cr == NULL && n[0] == '+') shoes_canvas_memdraw_end(self); \
    return obj; \
  } \
  VALUE \
  shoes_app_c_##func(int argc, VALUE *argv, VALUE self) \
  { \
    VALUE canvas; \
    GET_STRUCT(app, app); \
    if (RARRAY_LEN(app->nesting) > 0) \
      canvas = rb_ary_entry(app->nesting, RARRAY_LEN(app->nesting) - 1); \
    else \
      canvas = app->canvas; \
    return shoes_canvas_c_##func(argc, argv, canvas); \
  }

//
// See ruby.h for the complete list of App methods which redirect to Canvas.
//
CANVAS_DEFS(FUNC_M);

#define C(n, s) \
  re##n = rb_eval_string(s); \
  rb_const_set(cShoes, rb_intern("" # n), re##n);

VALUE progname;

//
// Everything exposed to Ruby is exposed here.
//
void
shoes_ruby_init()
{
  char proc[SHOES_BUFSIZE];
  progname = rb_str_new2("(eval)");
  rb_define_variable("$0", &progname);
  rb_define_variable("$PROGRAM_NAME", &progname);

  instance_eval_proc = rb_eval_string("lambda{|o,b| o.instance_eval(&b)}");
  rb_gc_register_address(&instance_eval_proc);
  ssNestSlot = rb_eval_string("{:height => 1.0}");
  rb_gc_register_address(&ssNestSlot);

  s_aref = rb_intern("[]=");
  s_perc = rb_intern("%");
  s_mult = rb_intern("*");
  s_bind = rb_intern("bind");
  s_gsub = rb_intern("gsub");
  s_keys = rb_intern("keys");
  s_update = rb_intern("update");
  s_new = rb_intern("new");

  s_now   = rb_intern("now");
  s_debug = rb_intern("DEBUG");
  s_info  = rb_intern("INFO");
  s_warn  = rb_intern("WARN");
  s_error = rb_intern("ERROR");

  s_run = rb_intern("run");
  s_to_i = rb_intern("to_i");
  s_to_s = rb_intern("to_s");
  s_to_pattern = rb_intern("to_pattern");
  s_angle = rb_intern("angle");
  s_arrow = rb_intern("arrow");
  s_autoplay = rb_intern("autoplay");
  s_begin = rb_intern("begin");
  s_call = rb_intern("call");
  s_center = rb_intern("center");
  s_change = rb_intern("change");
  s_choose = rb_intern("choose");
  s_click = rb_intern("click");
  s_corner = rb_intern("corner");
  s_curve = rb_intern("curve");
  s_distance = rb_intern("distance");
  s_displace_left = rb_intern("displace_left");
  s_displace_top = rb_intern("displace_top");
  s_downcase = rb_intern("downcase");
  s_draw = rb_intern("draw");
  s_end = rb_intern("end");
  s_fill = rb_intern("fill");
  s_finish = rb_intern("finish");
  s_font = rb_intern("font");
  s_hand = rb_intern("hand");
  s_hidden = rb_intern("hidden");
  s_hover = rb_intern("hover");
  s_href = rb_intern("href");
  s_insert = rb_intern("insert");
  s_inner = rb_intern("inner");
  s_items = rb_intern("items");
  s_keypress = rb_intern("keypress");
  s_match = rb_intern("match");
  s_motion = rb_intern("motion");
  s_leading = rb_intern("leading");
  s_leave = rb_intern("leave");
  s_release = rb_intern("release");
  s_wheel = rb_intern("wheel");
  s_scroll = rb_intern("scroll");
  s_start = rb_intern("start");
  s_attach = rb_intern("attach");
  s_text = rb_intern("text");
  s_title = rb_intern("title");
  s_top = rb_intern("top");
  s_right = rb_intern("right");
  s_bottom = rb_intern("bottom");
  s_left = rb_intern("left");
  s_up = rb_intern("up");
  s_down = rb_intern("down");
  s_height = rb_intern("height");
  s_remove = rb_intern("remove");
  s_resizable = rb_intern("resizable");
  s_strokewidth = rb_intern("strokewidth");
  s_width = rb_intern("width");
  s_margin = rb_intern("margin");
  s_margin_left = rb_intern("margin_left");
  s_margin_right = rb_intern("margin_right");
  s_margin_top = rb_intern("margin_top");
  s_margin_bottom = rb_intern("margin_bottom");
  s_radius = rb_intern("radius");
  s_secret = rb_intern("secret");

  symAltQuest = ID2SYM(rb_intern("alt_?"));
  symAltSlash = ID2SYM(rb_intern("alt_/"));
  symAltDot = ID2SYM(rb_intern("alt_."));

  cShoesWindow = rb_define_class("Window", rb_cObject);
  cMouse = rb_define_class("Mouse", rb_cObject);

  cCanvas = rb_define_class("Canvas", rb_cObject);
  rb_define_alloc_func(cCanvas, shoes_canvas_alloc);
  rb_define_method(cCanvas, "children", CASTHOOK(shoes_canvas_contents), 0);

  cShoes = rb_define_class("Shoes", cCanvas);
  rb_const_set(cShoes, rb_intern("RELEASE_NAME"), rb_str_new2(SHOES_RELEASE_NAME));
  rb_const_set(cShoes, rb_intern("RELEASE_ID"), INT2NUM(SHOES_RELEASE_ID));
  rb_const_set(cShoes, rb_intern("REVISION"), INT2NUM(SHOES_REVISION));

  cApp = rb_define_class_under(cShoes, "App", rb_cObject);
  rb_define_alloc_func(cApp, shoes_app_alloc);
  rb_define_method(cApp, "name", CASTHOOK(shoes_app_get_title), 0);
  rb_define_method(cApp, "name=", CASTHOOK(shoes_app_set_title), 1);
  rb_define_method(cApp, "location", CASTHOOK(shoes_app_location), 0);
  rb_define_method(cApp, "started?", CASTHOOK(shoes_app_is_started), 0);
  rb_define_method(cApp, "children", CASTHOOK(shoes_app_contents), 0);
  cDialog = rb_define_class_under(cShoes, "Dialog", cApp);

  eNotImpl = rb_define_class_under(cShoes, "NotImplementedError", rb_eStandardError);
  eVlcError = rb_define_class_under(cShoes, "VideoError", rb_eStandardError);
  eImageError = rb_define_class_under(cShoes, "ImageError", rb_eStandardError);
  C(HEX_SOURCE, "/^(?:0x|#)?([0-9A-F]{2})([0-9A-F]{2})([0-9A-F]{2})$/i");
  C(HEX3_SOURCE, "/^(?:0x|#)?([0-9A-F])([0-9A-F])([0-9A-F])$/i");
  C(RGB_SOURCE, "/^rgb\\((\\d+), *(\\d+), *(\\d+)\\)$/i");
  C(RGBA_SOURCE, "/^rgb\\((\\d+), *(\\d+), *(\\d+), *(\\d+)\\)$/i");
  C(GRAY_SOURCE, "/^gray\\((\\d+)\\)$/i");
  C(GRAYA_SOURCE, "/^gray\\((\\d+), *(\\d+)\\)$/i");
  C(LF, "/\\r?\\n/");
  rb_eval_string(
    "def Shoes.escape(string);"
       "string.gsub(/&/n, '&amp;').gsub(/\\\"/n, '&quot;').gsub(/>/n, '&gt;').gsub(/</n, '&lt;');"
    "end"
  );

  rb_define_singleton_method(cShoes, "APPS", CASTHOOK(shoes_apps_get), 0);
  rb_define_singleton_method(cShoes, "app", CASTHOOK(shoes_app_main), -1);
  rb_define_singleton_method(cShoes, "p", CASTHOOK(shoes_p), 1);
  rb_define_singleton_method(cShoes, "log", CASTHOOK(shoes_log), 0);

  //
  // Canvas methods
  // See ruby.h for the complete list of Canvas method signatures.
  // Macros are used to build App redirection methods, which should be
  // speedier than method_missing.
  //
#define RUBY_M(name, func, argc) \
  rb_define_method(cCanvas, name + 1, CASTHOOK(shoes_canvas_c_##func), -1); \
  rb_define_method(cApp, name + 1, CASTHOOK(shoes_app_c_##func), -1)

  CANVAS_DEFS(RUBY_M);

  //
  // Shoes Kernel methods
  //
  rb_define_method(rb_mKernel, "rgb", CASTHOOK(shoes_color_rgb), -1);
  rb_define_method(rb_mKernel, "gray", CASTHOOK(shoes_color_gray), -1);
  rb_define_method(rb_mKernel, "gradient", CASTHOOK(shoes_color_gradient), -1);
  rb_define_method(rb_mKernel, "pattern", CASTHOOK(shoes_pattern_method), 1);
  rb_define_method(rb_mKernel, "quit", CASTHOOK(shoes_app_quit), 0);
  rb_define_method(rb_mKernel, "exit", CASTHOOK(shoes_app_quit), 0);

  rb_define_method(rb_mKernel, "debug", CASTHOOK(shoes_canvas_debug), 1);
  rb_define_method(rb_mKernel, "info", CASTHOOK(shoes_canvas_info), 1);
  rb_define_method(rb_mKernel, "warn", CASTHOOK(shoes_canvas_warn), 1);
  rb_define_method(rb_mKernel, "error", CASTHOOK(shoes_canvas_error), 1);

  cFlow       = rb_define_class_under(cShoes, "Flow", cShoes);
  cStack      = rb_define_class_under(cShoes, "Stack", cShoes);
  cMask       = rb_define_class_under(cShoes, "Mask", cShoes);
  cWidget     = rb_define_class_under(cShoes, "Widget", cShoes);

  cImageBlock = rb_define_class_under(cShoes, "ImageBlock", cShoes);
  rb_define_method(cImageBlock, "draw", CASTHOOK(shoes_imageblock_draw), 2);
  rb_define_method(cImageBlock, "refresh", CASTHOOK(shoes_imageblock_refresh), 0);

  cShape    = rb_define_class_under(cShoes, "Shape", rb_cObject);
  rb_define_alloc_func(cShape, shoes_shape_alloc);
  rb_define_method(cShape, "displace", CASTHOOK(shoes_shape_displace), 2);
  rb_define_method(cShape, "draw", CASTHOOK(shoes_shape_draw), 2);
  rb_define_method(cShape, "move", CASTHOOK(shoes_shape_move), 2);
  rb_define_method(cShape, "parent", CASTHOOK(shoes_shape_get_parent), 0);
  rb_define_method(cShape, "top", CASTHOOK(shoes_shape_get_top), 0);
  rb_define_method(cShape, "left", CASTHOOK(shoes_shape_get_left), 0);
  rb_define_method(cShape, "width", CASTHOOK(shoes_shape_get_width), 0);
  rb_define_method(cShape, "height", CASTHOOK(shoes_shape_get_height), 0);
  rb_define_method(cShape, "remove", CASTHOOK(shoes_shape_remove), 0);
  rb_define_method(cShape, "style", CASTHOOK(shoes_shape_style), -1);
  rb_define_method(cShape, "hide", CASTHOOK(shoes_shape_hide), 0);
  rb_define_method(cShape, "show", CASTHOOK(shoes_shape_show), 0);
  rb_define_method(cShape, "toggle", CASTHOOK(shoes_shape_toggle), 0);
  rb_define_method(cShape, "click", CASTHOOK(shoes_shape_click), -1);
  rb_define_method(cShape, "release", CASTHOOK(shoes_shape_release), -1);
  rb_define_method(cShape, "hover", CASTHOOK(shoes_shape_hover), -1);
  rb_define_method(cShape, "leave", CASTHOOK(shoes_shape_leave), -1);

  cImage    = rb_define_class_under(cShoes, "Image", rb_cObject);
  rb_define_alloc_func(cImage, shoes_image_alloc);
  rb_define_method(cImage, "path", CASTHOOK(shoes_image_get_path), 0);
  rb_define_method(cImage, "path=", CASTHOOK(shoes_image_set_path), 1);
  rb_define_method(cImage, "displace", CASTHOOK(shoes_image_displace), 2);
  rb_define_method(cImage, "draw", CASTHOOK(shoes_image_draw), 2);
  rb_define_method(cImage, "size", CASTHOOK(shoes_image_size), 0);
  rb_define_method(cImage, "move", CASTHOOK(shoes_image_move), 2);
  rb_define_method(cImage, "parent", CASTHOOK(shoes_image_get_parent), 0);
  rb_define_method(cImage, "rotate", CASTHOOK(shoes_image_rotate), 1);
  rb_define_method(cImage, "scale", CASTHOOK(shoes_image_scale), -1);
  rb_define_method(cImage, "skew", CASTHOOK(shoes_image_skew), -1);
  rb_define_method(cImage, "transform", CASTHOOK(shoes_image_transform), 1);
  rb_define_method(cImage, "translate", CASTHOOK(shoes_image_translate), 2);
  rb_define_method(cImage, "top", CASTHOOK(shoes_image_get_top), 0);
  rb_define_method(cImage, "left", CASTHOOK(shoes_image_get_left), 0);
  rb_define_method(cImage, "width", CASTHOOK(shoes_image_get_width), 0);
  rb_define_method(cImage, "height", CASTHOOK(shoes_image_get_height), 0);
  rb_define_method(cImage, "remove", CASTHOOK(shoes_image_remove), 0);
  rb_define_method(cImage, "style", CASTHOOK(shoes_image_style), -1);
  rb_define_method(cImage, "hide", CASTHOOK(shoes_image_hide), 0);
  rb_define_method(cImage, "show", CASTHOOK(shoes_image_show), 0);
  rb_define_method(cImage, "toggle", CASTHOOK(shoes_image_toggle), 0);
  rb_define_method(cImage, "click", CASTHOOK(shoes_image_click), -1);
  rb_define_method(cImage, "release", CASTHOOK(shoes_image_release), -1);
  rb_define_method(cImage, "hover", CASTHOOK(shoes_image_hover), -1);
  rb_define_method(cImage, "leave", CASTHOOK(shoes_image_leave), -1);

  cEffect   = rb_define_class_under(cShoes, "Effect", rb_cObject);
  rb_define_alloc_func(cEffect, shoes_effect_alloc);
  rb_define_method(cEffect, "draw", CASTHOOK(shoes_effect_draw), 2);
  rb_define_method(cEffect, "remove", CASTHOOK(shoes_effect_remove), 0);
  cBlur     = rb_define_class_under(cShoes, "Blur", cEffect);
  cShadow   = rb_define_class_under(cShoes, "Shadow", cEffect);
  cGlow     = rb_define_class_under(cShoes, "Glow", cEffect);

#ifdef VIDEO
  cVideo    = rb_define_class_under(cShoes, "Video", rb_cObject);
  rb_define_alloc_func(cVideo, shoes_video_alloc);
  rb_define_method(cVideo, "displace", CASTHOOK(shoes_video_displace), 2);
  rb_define_method(cVideo, "draw", CASTHOOK(shoes_video_draw), 2);
  rb_define_method(cVideo, "style", CASTHOOK(shoes_video_style), -1);
  rb_define_method(cVideo, "hide", CASTHOOK(shoes_video_hide), 0);
  rb_define_method(cVideo, "show", CASTHOOK(shoes_video_show), 0);
  rb_define_method(cVideo, "move", CASTHOOK(shoes_video_move), 2);
  rb_define_method(cVideo, "parent", CASTHOOK(shoes_video_get_parent), 0);
  rb_define_method(cVideo, "top", CASTHOOK(shoes_video_get_top), 0);
  rb_define_method(cVideo, "left", CASTHOOK(shoes_video_get_left), 0);
  rb_define_method(cVideo, "width", CASTHOOK(shoes_video_get_width), 0);
  rb_define_method(cVideo, "height", CASTHOOK(shoes_video_get_height), 0);
  rb_define_method(cVideo, "remove", CASTHOOK(shoes_video_remove), 0);
  rb_define_method(cVideo, "playing?", CASTHOOK(shoes_video_is_playing), 0);
  rb_define_method(cVideo, "play", CASTHOOK(shoes_video_play), 0);
  rb_define_method(cVideo, "clear", CASTHOOK(shoes_video_clear), 0);
  rb_define_method(cVideo, "previous", CASTHOOK(shoes_video_prev), 0);
  rb_define_method(cVideo, "next", CASTHOOK(shoes_video_next), 0);
  rb_define_method(cVideo, "pause", CASTHOOK(shoes_video_pause), 0);
  rb_define_method(cVideo, "stop", CASTHOOK(shoes_video_stop), 0);
  rb_define_method(cVideo, "length", CASTHOOK(shoes_video_get_length), 0);
  rb_define_method(cVideo, "position", CASTHOOK(shoes_video_get_position), 0);
  rb_define_method(cVideo, "position=", CASTHOOK(shoes_video_set_position), 1);
  rb_define_method(cVideo, "time", CASTHOOK(shoes_video_get_time), 0);
  rb_define_method(cVideo, "time=", CASTHOOK(shoes_video_set_time), 1);
#endif

  cPattern = rb_define_class_under(cShoes, "Pattern", rb_cObject);
  rb_define_alloc_func(cPattern, shoes_pattern_alloc);
  rb_define_method(cPattern, "displace", CASTHOOK(shoes_pattern_displace), 2);
  rb_define_method(cPattern, "move", CASTHOOK(shoes_pattern_move), 2);
  rb_define_method(cPattern, "remove", CASTHOOK(shoes_pattern_remove), 0);
  rb_define_method(cPattern, "to_pattern", CASTHOOK(shoes_pattern_self), 0);
  rb_define_method(cPattern, "style", CASTHOOK(shoes_pattern_style), -1);
  rb_define_method(cPattern, "hide", CASTHOOK(shoes_pattern_hide), 0);
  rb_define_method(cPattern, "show", CASTHOOK(shoes_pattern_show), 0);
  rb_define_method(cPattern, "toggle", CASTHOOK(shoes_pattern_toggle), 0);
  cBackground = rb_define_class_under(cShoes, "Background", cPattern);
  rb_define_method(cBackground, "draw", CASTHOOK(shoes_background_draw), 2);
  cBorder = rb_define_class_under(cShoes, "Border", cPattern);
  rb_define_method(cBorder, "draw", CASTHOOK(shoes_border_draw), 2);

  cTextBlock = rb_define_class_under(cShoes, "TextBlock", rb_cObject);
  rb_define_alloc_func(cTextBlock, shoes_textblock_alloc);
  rb_define_method(cTextBlock, "contents", CASTHOOK(shoes_textblock_children), 0);
  rb_define_method(cTextBlock, "parent", CASTHOOK(shoes_textblock_get_parent), 0);
  rb_define_method(cTextBlock, "displace", CASTHOOK(shoes_textblock_displace), 2);
  rb_define_method(cTextBlock, "draw", CASTHOOK(shoes_textblock_draw), 2);
  rb_define_method(cTextBlock, "cursor=", CASTHOOK(shoes_textblock_set_cursor), 1);
  rb_define_method(cTextBlock, "cursor", CASTHOOK(shoes_textblock_get_cursor), 0);
  rb_define_method(cTextBlock, "move", CASTHOOK(shoes_textblock_move), 2);
  rb_define_method(cTextBlock, "top", CASTHOOK(shoes_textblock_get_top), 0);
  rb_define_method(cTextBlock, "left", CASTHOOK(shoes_textblock_get_left), 0);
  rb_define_method(cTextBlock, "width", CASTHOOK(shoes_textblock_get_width), 0);
  rb_define_method(cTextBlock, "height", CASTHOOK(shoes_textblock_get_height), 0);
  rb_define_method(cTextBlock, "remove", CASTHOOK(shoes_textblock_remove), 0);
  rb_define_method(cTextBlock, "to_s", CASTHOOK(shoes_textblock_string), 0);
  rb_define_method(cTextBlock, "replace", CASTHOOK(shoes_textblock_replace), -1);
  rb_define_method(cTextBlock, "style", CASTHOOK(shoes_textblock_style), -1);
  rb_define_method(cTextBlock, "hide", CASTHOOK(shoes_textblock_hide), 0);
  rb_define_method(cTextBlock, "show", CASTHOOK(shoes_textblock_show), 0);
  rb_define_method(cTextBlock, "toggle", CASTHOOK(shoes_textblock_toggle), 0);
  rb_define_method(cTextBlock, "click", CASTHOOK(shoes_textblock_click), -1);
  rb_define_method(cTextBlock, "release", CASTHOOK(shoes_textblock_release), -1);
  rb_define_method(cTextBlock, "hover", CASTHOOK(shoes_textblock_hover), -1);
  rb_define_method(cTextBlock, "leave", CASTHOOK(shoes_textblock_leave), -1);
  cPara = rb_define_class_under(cShoes, "Para", cTextBlock);
  cBanner = rb_define_class_under(cShoes, "Banner", cTextBlock);
  cTitle = rb_define_class_under(cShoes, "Title", cTextBlock);
  cSubtitle = rb_define_class_under(cShoes, "Subtitle", cTextBlock);
  cTagline = rb_define_class_under(cShoes, "Tagline", cTextBlock);
  cCaption = rb_define_class_under(cShoes, "Caption", cTextBlock);
  cInscription = rb_define_class_under(cShoes, "Inscription", cTextBlock);

  cTextClass = rb_define_class_under(cShoes, "Text", rb_cObject);
  rb_define_alloc_func(cTextClass, shoes_text_alloc);
  rb_define_method(cTextClass, "contents", CASTHOOK(shoes_text_children), 0);
  rb_define_method(cTextClass, "parent", CASTHOOK(shoes_text_parent), 0);
  rb_define_method(cTextClass, "style", CASTHOOK(shoes_text_style), -1);
  cCode      = rb_define_class_under(cShoes, "Code", cTextClass);
  cDel       = rb_define_class_under(cShoes, "Del", cTextClass);
  cEm        = rb_define_class_under(cShoes, "Em", cTextClass);
  cIns       = rb_define_class_under(cShoes, "Ins", cTextClass);
  cSpan      = rb_define_class_under(cShoes, "Span", cTextClass);
  cStrong    = rb_define_class_under(cShoes, "Strong", cTextClass);
  cSub       = rb_define_class_under(cShoes, "Sub", cTextClass);
  cSup       = rb_define_class_under(cShoes, "Sup", cTextClass);

  cLink      = rb_define_class_under(cShoes, "Link", cTextClass);
  rb_define_method(cTextClass, "click", CASTHOOK(shoes_linktext_click), -1);
  rb_define_method(cTextClass, "release", CASTHOOK(shoes_linktext_release), -1);
  rb_define_method(cTextClass, "hover", CASTHOOK(shoes_linktext_hover), -1);
  rb_define_method(cTextClass, "leave", CASTHOOK(shoes_linktext_leave), -1);
  cLinkHover = rb_define_class_under(cShoes, "LinkHover", cTextClass);

  cNative  = rb_define_class_under(cShoes, "Native", rb_cObject);
  rb_define_alloc_func(cNative, shoes_control_alloc);
  rb_define_method(cNative, "parent", CASTHOOK(shoes_control_get_parent), 0);
  rb_define_method(cNative, "style", CASTHOOK(shoes_control_style), -1);
  rb_define_method(cNative, "displace", CASTHOOK(shoes_control_displace), 2);
  rb_define_method(cNative, "hide", CASTHOOK(shoes_control_hide), 0);
  rb_define_method(cNative, "show", CASTHOOK(shoes_control_show), 0);
  rb_define_method(cNative, "move", CASTHOOK(shoes_control_move), 2);
  rb_define_method(cNative, "top", CASTHOOK(shoes_control_get_top), 0);
  rb_define_method(cNative, "left", CASTHOOK(shoes_control_get_left), 0);
  rb_define_method(cNative, "width", CASTHOOK(shoes_control_get_width), 0);
  rb_define_method(cNative, "height", CASTHOOK(shoes_control_get_height), 0);
  rb_define_method(cNative, "remove", CASTHOOK(shoes_control_remove), 0);
  cButton  = rb_define_class_under(cShoes, "Button", cNative);
  rb_define_method(cButton, "draw", CASTHOOK(shoes_button_draw), 2);
  rb_define_method(cButton, "click", CASTHOOK(shoes_control_click), -1);
  cEditLine  = rb_define_class_under(cShoes, "EditLine", cNative);
  rb_define_method(cEditLine, "text", CASTHOOK(shoes_edit_line_get_text), 0);
  rb_define_method(cEditLine, "text=", CASTHOOK(shoes_edit_line_set_text), 1);
  rb_define_method(cEditLine, "draw", CASTHOOK(shoes_edit_line_draw), 2);
  rb_define_method(cEditLine, "change", CASTHOOK(shoes_control_change), -1);
  cEditBox  = rb_define_class_under(cShoes, "EditBox", cNative);
  rb_define_method(cEditBox, "text", CASTHOOK(shoes_edit_box_get_text), 0);
  rb_define_method(cEditBox, "text=", CASTHOOK(shoes_edit_box_set_text), 1);
  rb_define_method(cEditBox, "draw", CASTHOOK(shoes_edit_box_draw), 2);
  rb_define_method(cEditBox, "change", CASTHOOK(shoes_control_change), -1);
  cListBox  = rb_define_class_under(cShoes, "ListBox", cNative);
  rb_define_method(cListBox, "text", CASTHOOK(shoes_list_box_text), 0);
  rb_define_method(cListBox, "draw", CASTHOOK(shoes_list_box_draw), 2);
  rb_define_method(cListBox, "choose", CASTHOOK(shoes_list_box_choose), 1);
  rb_define_method(cListBox, "change", CASTHOOK(shoes_control_change), -1);
  rb_define_method(cListBox, "items", CASTHOOK(shoes_list_box_items_get), 0);
  rb_define_method(cListBox, "items=", CASTHOOK(shoes_list_box_items_set), 1);
  cProgress  = rb_define_class_under(cShoes, "Progress", cNative);
  rb_define_method(cProgress, "draw", CASTHOOK(shoes_progress_draw), 2);
  rb_define_method(cProgress, "fraction", CASTHOOK(shoes_progress_get_fraction), 0);
  rb_define_method(cProgress, "fraction=", CASTHOOK(shoes_progress_set_fraction), 1);
  cCheck  = rb_define_class_under(cShoes, "Check", cNative);
  rb_define_method(cCheck, "draw", CASTHOOK(shoes_check_draw), 2);
  rb_define_method(cCheck, "checked?", CASTHOOK(shoes_check_is_checked), 0);
  rb_define_method(cCheck, "checked=", CASTHOOK(shoes_check_set_checked), 1);
  rb_define_method(cCheck, "click", CASTHOOK(shoes_control_click), -1);
  cRadio  = rb_define_class_under(cShoes, "Radio", cNative);
  rb_define_method(cRadio, "draw", CASTHOOK(shoes_radio_draw), 2);
  rb_define_method(cRadio, "checked?", CASTHOOK(shoes_check_is_checked), 0);
  rb_define_method(cRadio, "checked=", CASTHOOK(shoes_check_set_checked), 1);
  rb_define_method(cRadio, "click", CASTHOOK(shoes_control_click), -1);

  cTimerBase   = rb_define_class_under(cShoes, "TimerBase", rb_cObject);
  rb_define_alloc_func(cTimerBase, shoes_timer_alloc);
  rb_define_method(cTimerBase, "draw", CASTHOOK(shoes_timer_draw), 2);
  rb_define_method(cTimerBase, "remove", CASTHOOK(shoes_timer_remove), 0);
  rb_define_method(cTimerBase, "start", CASTHOOK(shoes_timer_start), 0);
  rb_define_method(cTimerBase, "stop", CASTHOOK(shoes_timer_stop), 0);
  rb_define_method(cTimerBase, "toggle", CASTHOOK(shoes_timer_toggle), 0);
  cAnim    = rb_define_class_under(cShoes, "Animation", cTimerBase);
  cEvery   = rb_define_class_under(cShoes, "Every", cTimerBase);
  cTimer   = rb_define_class_under(cShoes, "Timer", cTimerBase);

  cColor   = rb_define_class_under(cShoes, "Color", rb_cObject);
  rb_define_alloc_func(cColor, shoes_color_alloc);
  rb_define_method(rb_mKernel, "rgb", CASTHOOK(shoes_color_rgb), -1);
  rb_define_method(rb_mKernel, "gray", CASTHOOK(shoes_color_gray), -1);
  rb_define_singleton_method(cColor, "rgb", CASTHOOK(shoes_color_rgb), -1);
  rb_define_singleton_method(cColor, "gray", CASTHOOK(shoes_color_gray), -1);
  rb_define_singleton_method(cColor, "parse", CASTHOOK(shoes_color_parse), 1);
  rb_define_method(cColor, "black?", CASTHOOK(shoes_color_is_black), 0);
  rb_define_method(cColor, "dark?", CASTHOOK(shoes_color_is_dark), 0);
  rb_define_method(cColor, "inspect", CASTHOOK(shoes_color_to_s), 0);
  rb_define_method(cColor, "invert", CASTHOOK(shoes_color_invert), 0);
  rb_define_method(cColor, "light?", CASTHOOK(shoes_color_is_light), 0);
  rb_define_method(cColor, "opaque?", CASTHOOK(shoes_color_is_opaque), 0);
  rb_define_method(cColor, "to_s", CASTHOOK(shoes_color_to_s), 0);
  rb_define_method(cColor, "to_pattern", CASTHOOK(shoes_color_to_pattern), 0);
  rb_define_method(cColor, "transparent?", CASTHOOK(shoes_color_is_transparent), 0);
  rb_define_method(cColor, "white?", CASTHOOK(shoes_color_is_white), 0);

  rb_define_method(cCanvas, "method_missing", CASTHOOK(shoes_color_method_missing), -1);
  rb_define_method(cApp, "method_missing", CASTHOOK(shoes_app_method_missing), -1);

  rb_const_set(cShoes, rb_intern("ALL_NAMED_COLORS"), rb_hash_new());
  cColors = rb_const_get(cShoes, rb_intern("ALL_NAMED_COLORS"));
  rb_const_set(cShoes, rb_intern("COLORS"), cColors);
  DEF_COLOR(aliceblue, 240, 248, 255);
  DEF_COLOR(antiquewhite, 250, 235, 215);
  DEF_COLOR(aqua, 0, 255, 255);
  DEF_COLOR(aquamarine, 127, 255, 212);
  DEF_COLOR(azure, 240, 255, 255);
  DEF_COLOR(beige, 245, 245, 220);
  DEF_COLOR(bisque, 255, 228, 196);
  DEF_COLOR(black, 0, 0, 0);
  DEF_COLOR(blanchedalmond, 255, 235, 205);
  DEF_COLOR(blue, 0, 0, 255);
  DEF_COLOR(blueviolet, 138, 43, 226);
  DEF_COLOR(brown, 165, 42, 42);
  DEF_COLOR(burlywood, 222, 184, 135);
  DEF_COLOR(cadetblue, 95, 158, 160);
  DEF_COLOR(chartreuse, 127, 255, 0);
  DEF_COLOR(chocolate, 210, 105, 30);
  DEF_COLOR(coral, 255, 127, 80);
  DEF_COLOR(cornflowerblue, 100, 149, 237);
  DEF_COLOR(cornsilk, 255, 248, 220);
  DEF_COLOR(crimson, 220, 20, 60);
  DEF_COLOR(cyan, 0, 255, 255);
  DEF_COLOR(darkblue, 0, 0, 139);
  DEF_COLOR(darkcyan, 0, 139, 139);
  DEF_COLOR(darkgoldenrod, 184, 134, 11);
  DEF_COLOR(darkgray, 169, 169, 169);
  DEF_COLOR(darkgreen, 0, 100, 0);
  DEF_COLOR(darkkhaki, 189, 183, 107);
  DEF_COLOR(darkmagenta, 139, 0, 139);
  DEF_COLOR(darkolivegreen, 85, 107, 47);
  DEF_COLOR(darkorange, 255, 140, 0);
  DEF_COLOR(darkorchid, 153, 50, 204);
  DEF_COLOR(darkred, 139, 0, 0);
  DEF_COLOR(darksalmon, 233, 150, 122);
  DEF_COLOR(darkseagreen, 143, 188, 143);
  DEF_COLOR(darkslateblue, 72, 61, 139);
  DEF_COLOR(darkslategray, 47, 79, 79);
  DEF_COLOR(darkturquoise, 0, 206, 209);
  DEF_COLOR(darkviolet, 148, 0, 211);
  DEF_COLOR(deeppink, 255, 20, 147);
  DEF_COLOR(deepskyblue, 0, 191, 255);
  DEF_COLOR(dimgray, 105, 105, 105);
  DEF_COLOR(dodgerblue, 30, 144, 255);
  DEF_COLOR(firebrick, 178, 34, 34);
  DEF_COLOR(floralwhite, 255, 250, 240);
  DEF_COLOR(forestgreen, 34, 139, 34);
  DEF_COLOR(fuchsia, 255, 0, 255);
  DEF_COLOR(gainsboro, 220, 220, 220);
  DEF_COLOR(ghostwhite, 248, 248, 255);
  DEF_COLOR(gold, 255, 215, 0);
  DEF_COLOR(goldenrod, 218, 165, 32);
  DEF_COLOR(gray, 128, 128, 128);
  DEF_COLOR(green, 0, 128, 0);
  DEF_COLOR(greenyellow, 173, 255, 47);
  DEF_COLOR(honeydew, 240, 255, 240);
  DEF_COLOR(hotpink, 255, 105, 180);
  DEF_COLOR(indianred, 205, 92, 92);
  DEF_COLOR(indigo, 75, 0, 130);
  DEF_COLOR(ivory, 255, 255, 240);
  DEF_COLOR(khaki, 240, 230, 140);
  DEF_COLOR(lavender, 230, 230, 250);
  DEF_COLOR(lavenderblush, 255, 240, 245);
  DEF_COLOR(lawngreen, 124, 252, 0);
  DEF_COLOR(lemonchiffon, 255, 250, 205);
  DEF_COLOR(lightblue, 173, 216, 230);
  DEF_COLOR(lightcoral, 240, 128, 128);
  DEF_COLOR(lightcyan, 224, 255, 255);
  DEF_COLOR(lightgoldenrodyellow, 250, 250, 210);
  DEF_COLOR(lightgreen, 144, 238, 144);
  DEF_COLOR(lightgrey, 211, 211, 211);
  DEF_COLOR(lightpink, 255, 182, 193);
  DEF_COLOR(lightsalmon, 255, 160, 122);
  DEF_COLOR(lightseagreen, 32, 178, 170);
  DEF_COLOR(lightskyblue, 135, 206, 250);
  DEF_COLOR(lightslategray, 119, 136, 153);
  DEF_COLOR(lightsteelblue, 176, 196, 222);
  DEF_COLOR(lightyellow, 255, 255, 224);
  DEF_COLOR(lime, 0, 255, 0);
  DEF_COLOR(limegreen, 50, 205, 50);
  DEF_COLOR(linen, 250, 240, 230);
  DEF_COLOR(magenta, 255, 0, 255);
  DEF_COLOR(maroon, 128, 0, 0);
  DEF_COLOR(mediumaquamarine, 102, 205, 170);
  DEF_COLOR(mediumblue, 0, 0, 205);
  DEF_COLOR(mediumorchid, 186, 85, 211);
  DEF_COLOR(mediumpurple, 147, 112, 219);
  DEF_COLOR(mediumseagreen, 60, 179, 113);
  DEF_COLOR(mediumslateblue, 123, 104, 238);
  DEF_COLOR(mediumspringgreen, 0, 250, 154);
  DEF_COLOR(mediumturquoise, 72, 209, 204);
  DEF_COLOR(mediumvioletred, 199, 21, 133);
  DEF_COLOR(midnightblue, 25, 25, 112);
  DEF_COLOR(mintcream, 245, 255, 250);
  DEF_COLOR(mistyrose, 255, 228, 225);
  DEF_COLOR(moccasin, 255, 228, 181);
  DEF_COLOR(navajowhite, 255, 222, 173);
  DEF_COLOR(navy, 0, 0, 128);
  DEF_COLOR(oldlace, 253, 245, 230);
  DEF_COLOR(olive, 128, 128, 0);
  DEF_COLOR(olivedrab, 107, 142, 35);
  DEF_COLOR(orange, 255, 165, 0);
  DEF_COLOR(orangered, 255, 69, 0);
  DEF_COLOR(orchid, 218, 112, 214);
  DEF_COLOR(palegoldenrod, 238, 232, 170);
  DEF_COLOR(palegreen, 152, 251, 152);
  DEF_COLOR(paleturquoise, 175, 238, 238);
  DEF_COLOR(palevioletred, 219, 112, 147);
  DEF_COLOR(papayawhip, 255, 239, 213);
  DEF_COLOR(peachpuff, 255, 218, 185);
  DEF_COLOR(peru, 205, 133, 63);
  DEF_COLOR(pink, 255, 192, 203);
  DEF_COLOR(plum, 221, 160, 221);
  DEF_COLOR(powderblue, 176, 224, 230);
  DEF_COLOR(purple, 128, 0, 128);
  DEF_COLOR(red, 255, 0, 0);
  DEF_COLOR(rosybrown, 188, 143, 143);
  DEF_COLOR(royalblue, 65, 105, 225);
  DEF_COLOR(saddlebrown, 139, 69, 19);
  DEF_COLOR(salmon, 250, 128, 114);
  DEF_COLOR(sandybrown, 244, 164, 96);
  DEF_COLOR(seagreen, 46, 139, 87);
  DEF_COLOR(seashell, 255, 245, 238);
  DEF_COLOR(sienna, 160, 82, 45);
  DEF_COLOR(silver, 192, 192, 192);
  DEF_COLOR(skyblue, 135, 206, 235);
  DEF_COLOR(slateblue, 106, 90, 205);
  DEF_COLOR(slategray, 112, 128, 144);
  DEF_COLOR(snow, 255, 250, 250);
  DEF_COLOR(springgreen, 0, 255, 127);
  DEF_COLOR(steelblue, 70, 130, 180);
  DEF_COLOR(tan, 210, 180, 140);
  DEF_COLOR(teal, 0, 128, 128);
  DEF_COLOR(thistle, 216, 191, 216);
  DEF_COLOR(tomato, 255, 99, 71);
  DEF_COLOR(turquoise, 64, 224, 208);
  DEF_COLOR(violet, 238, 130, 238);
  DEF_COLOR(wheat, 245, 222, 179);
  DEF_COLOR(white, 255, 255, 255);
  DEF_COLOR(whitesmoke, 245, 245, 245);
  DEF_COLOR(yellow, 255, 255, 0);
  DEF_COLOR(yellowgreen, 154, 205, 50);

  cLinkUrl = rb_define_class_under(cShoes, "LinkUrl", rb_cObject);

  rb_define_method(rb_mKernel, "alert", CASTHOOK(shoes_dialog_alert), 1);
  rb_define_method(rb_mKernel, "ask", CASTHOOK(shoes_dialog_ask), 1);
  rb_define_method(rb_mKernel, "confirm", CASTHOOK(shoes_dialog_confirm), 1);
  rb_define_method(rb_mKernel, "ask_color", CASTHOOK(shoes_dialog_color), 1);
  rb_define_method(rb_mKernel, "ask_open_file", CASTHOOK(shoes_dialog_open), 0);
  rb_define_method(rb_mKernel, "ask_save_file", CASTHOOK(shoes_dialog_save), 0);
}
