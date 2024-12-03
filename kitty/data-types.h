/*
 * data-types.h
 * Copyright (C) 2016 Kovid Goyal <kovid at kovidgoyal.net>
 *
 * Distributed under terms of the GPL3 license.
 */

#pragma once

#ifdef _POSIX_C_SOURCE
#error "Must include \"data-types.h\" before any system headers"
#endif
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <poll.h>
#include <pthread.h>
#include "glfw-wrapper.h"
#include "banned.h"
// Required minimum OpenGL version
#define OPENGL_REQUIRED_VERSION_MAJOR 3
#ifdef __APPLE__
#define OPENGL_REQUIRED_VERSION_MINOR 3
#else
#define OPENGL_REQUIRED_VERSION_MINOR 1
#endif
#define GLSL_VERSION 140
#define GLFW_MOD_KITTY (GLFW_MOD_LAST * 2)
#define UNUSED __attribute__ ((unused))
#define PYNOARG PyObject *__a1 UNUSED, PyObject *__a2 UNUSED
#define EXPORTED __attribute__ ((visibility ("default")))
#define LIKELY(x)    __builtin_expect (!!(x), 1)
#define UNLIKELY(x)  __builtin_expect (!!(x), 0)
#define MAX(x, y) __extension__ ({ \
    const __typeof__ (x) __a__ = (x); const __typeof__ (y) __b__ = (y); \
        __a__ > __b__ ? __a__ : __b__;})
#define MIN(x, y) __extension__ ({ \
    const __typeof__ (x) __a__ = (x); const __typeof__ (y) __b__ = (y); \
        __a__ < __b__ ? __a__ : __b__;})
#define SWAP(x, y) do { __typeof__(x) _sw_ = y; y = x; x = _sw_; } while(0)
#define xstr(s) str(s)
#define str(s) #s
#define arraysz(x) (sizeof(x)/sizeof(x[0]))
#define zero_at_i(array, idx) memset((array) + (idx), 0, sizeof((array)[0]))
#define zero_at_ptr(p) memset((p), 0, sizeof((p)[0]))
#define literal_strlen(x) (sizeof(x)-1)
#define zero_at_ptr_count(p, count) memset((p), 0, (count) * sizeof((p)[0]))
#define C0_EXCEPT_NL_SPACE_TAB 0x0 ... 0x8: case 0xb ... 0x1f: case 0x7f
void log_error(const char *fmt, ...) __attribute__ ((format (printf, 1, 2)));
#define fatal(...) { log_error(__VA_ARGS__); exit(EXIT_FAILURE); }
static inline void cleanup_free(void *p) { free(*(void**)p); }
#define RAII_ALLOC(type, name, initializer) __attribute__((cleanup(cleanup_free))) type *name = initializer
static inline void cleanup_decref(PyObject **p) { Py_CLEAR(*p); }
#define RAII_PyObject(name, initializer) __attribute__((cleanup(cleanup_decref))) PyObject *name = initializer
#define RAII_PY_BUFFER(name) __attribute__((cleanup(PyBuffer_Release))) Py_buffer name = {0}
#if PY_VERSION_HEX < 0x030a0000
static inline PyObject* Py_NewRef(PyObject *o) { Py_INCREF(o); return o; }
static inline PyObject* Py_XNewRef(PyObject *o) { Py_XINCREF(o); return o; }
#endif

typedef unsigned long long id_type;
typedef uint32_t char_type;
static_assert(sizeof(Py_UCS4) == sizeof(char_type), "PyUCS4 and char_type dont match");
#define MAX_CHAR_TYPE_VALUE UINT32_MAX
typedef uint32_t color_type;
typedef uint16_t hyperlink_id_type;
typedef int key_type;
#define HYPERLINK_MAX_NUMBER UINT16_MAX
typedef uint16_t combining_type;
typedef uint16_t glyph_index;
typedef uint32_t pixel;
typedef unsigned int index_type;
typedef uint16_t sprite_index;
typedef enum CursorShapes { NO_CURSOR_SHAPE, CURSOR_BLOCK, CURSOR_BEAM, CURSOR_UNDERLINE, CURSOR_HOLLOW, NUM_OF_CURSOR_SHAPES } CursorShape;
typedef enum { DISABLE_LIGATURES_NEVER, DISABLE_LIGATURES_CURSOR, DISABLE_LIGATURES_ALWAYS } DisableLigature;

#define ERROR_PREFIX "[PARSE ERROR]"
typedef enum MouseTrackingModes { NO_TRACKING, BUTTON_MODE, MOTION_MODE, ANY_MODE } MouseTrackingMode;
typedef enum MouseTrackingProtocols { NORMAL_PROTOCOL, UTF8_PROTOCOL, SGR_PROTOCOL, URXVT_PROTOCOL, SGR_PIXEL_PROTOCOL} MouseTrackingProtocol;
typedef enum MouseShapes {
    INVALID_POINTER,
    /* start mouse shapes (auto generated by gen-key-constants.py do not edit) */
    DEFAULT_POINTER,
    TEXT_POINTER,
    POINTER_POINTER,
    HELP_POINTER,
    WAIT_POINTER,
    PROGRESS_POINTER,
    CROSSHAIR_POINTER,
    CELL_POINTER,
    VERTICAL_TEXT_POINTER,
    MOVE_POINTER,
    E_RESIZE_POINTER,
    NE_RESIZE_POINTER,
    NW_RESIZE_POINTER,
    N_RESIZE_POINTER,
    SE_RESIZE_POINTER,
    SW_RESIZE_POINTER,
    S_RESIZE_POINTER,
    W_RESIZE_POINTER,
    EW_RESIZE_POINTER,
    NS_RESIZE_POINTER,
    NESW_RESIZE_POINTER,
    NWSE_RESIZE_POINTER,
    ZOOM_IN_POINTER,
    ZOOM_OUT_POINTER,
    ALIAS_POINTER,
    COPY_POINTER,
    NOT_ALLOWED_POINTER,
    NO_DROP_POINTER,
    GRAB_POINTER,
    GRABBING_POINTER,
/* end mouse shapes */
} MouseShape;
typedef enum { NONE, MENUBAR, WINDOW, ALL } WindowTitleIn;
typedef enum { TILING, SCALED, MIRRORED, CLAMPED, CENTER_CLAMPED, CENTER_SCALED } BackgroundImageLayout;
typedef struct ImageAnchorPosition {
    float canvas_x, canvas_y, image_x, image_y;
} ImageAnchorPosition;

#define MAX_CHILDREN 512
#define BLANK_CHAR 0
#define COL_MASK 0xFFFFFFFF
#define DECORATION_FG_CODE 58

// PUA character used as an image placeholder.
#define IMAGE_PLACEHOLDER_CHAR 0x10EEEE

#define FG 1
#define BG 2

#define COPY_CELL(src, s, dest, d) \
    (dest)->cpu_cells[d] = (src)->cpu_cells[s]; (dest)->gpu_cells[d] = (src)->gpu_cells[s];

#define COPY_SELF_CELL(s, d) COPY_CELL(self, s, self, d)

#define METHOD(name, arg_type) {#name, (PyCFunction)name, arg_type, name##_doc},
#define METHODB(name, arg_type) {#name, (PyCFunction)name, arg_type, ""}

#define BOOL_GETSET(type, x) \
    static PyObject* x##_get(type *self, void UNUSED *closure) { PyObject *ans = self->x ? Py_True : Py_False; Py_INCREF(ans); return ans; } \
    static int x##_set(type *self, PyObject *value, void UNUSED *closure) { if (value == NULL) { PyErr_SetString(PyExc_TypeError, "Cannot delete attribute"); return -1; } self->x = PyObject_IsTrue(value) ? true : false; return 0; }

#define GETSET(x) \
    {#x, (getter) x##_get, (setter) x##_set, #x, NULL},

#ifndef EXTRA_INIT
#define EXTRA_INIT
#endif
#define INIT_TYPE(type) \
    int init_##type(PyObject *module) {\
        if (PyType_Ready(&type##_Type) < 0) return 0; \
        if (PyModule_AddObject(module, #type, (PyObject *)&type##_Type) != 0) return 0; \
        Py_INCREF(&type##_Type); \
        EXTRA_INIT; \
        return 1; \
    }

#define RICHCMP(type) \
    static PyObject * richcmp(PyObject *obj1, PyObject *obj2, int op) { \
        PyObject *result = NULL; \
        int eq; \
        if (op != Py_EQ && op != Py_NE) { Py_RETURN_NOTIMPLEMENTED; } \
        if (!PyObject_TypeCheck(obj1, &type##_Type)) { Py_RETURN_FALSE; } \
        if (!PyObject_TypeCheck(obj2, &type##_Type)) { Py_RETURN_FALSE; } \
        eq = __eq__((type*)obj1, (type*)obj2); \
        if (op == Py_NE) result = eq ? Py_False : Py_True; \
        else result = eq ? Py_True : Py_False; \
        Py_INCREF(result); \
        return result; \
    }

#ifdef __clang__
#define START_IGNORE_DIAGNOSTIC(diag) _Pragma(xstr(clang diagnostic push))  _Pragma(xstr(clang diagnostic ignored diag))
#define END_IGNORE_DIAGNOSTIC _Pragma("clang diagnostic pop")
#else
#define START_IGNORE_DIAGNOSTIC(diag) _Pragma(xstr(GCC diagnostic push))  _Pragma(xstr(GCC diagnostic ignored diag))
#define END_IGNORE_DIAGNOSTIC _Pragma("GCC diagnostic pop")
#endif

#define IGNORE_PEDANTIC_WARNINGS START_IGNORE_DIAGNOSTIC("-Wpedantic")
#define END_IGNORE_PEDANTIC_WARNINGS END_IGNORE_DIAGNOSTIC
#define ALLOW_UNUSED_RESULT IGNORE_DIAGNOSTIC("-Wunused-result")
#define END_ALLOW_UNUSED_RESULT END_IGNORE_DIAGNOSTIC
#define START_ALLOW_CASE_RANGE IGNORE_PEDANTIC_WARNINGS
#define END_ALLOW_CASE_RANGE END_IGNORE_PEDANTIC_WARNINGS
#define BIT_MASK(__TYPE__, __ONE_COUNT__) \
    (((__TYPE__) (-((__ONE_COUNT__) != 0))) \
    & (((__TYPE__) -1) >> ((sizeof(__TYPE__) * CHAR_BIT) - (__ONE_COUNT__))))
#define ADD_TYPE(which) \
    if (PyType_Ready(&which##_Type) < 0) return false; \
    if (PyModule_AddObject(module, #which, (PyObject *)&which##_Type) != 0) return false; \
    Py_INCREF(&which##_Type);


typedef enum UTF8State { UTF8_ACCEPT = 0, UTF8_REJECT = 1} UTF8State;

typedef struct {
    uint32_t left, top, right, bottom;
} Region;

typedef enum { UNKNOWN_PROMPT_KIND = 0, PROMPT_START = 1, SECONDARY_PROMPT = 2, OUTPUT_START = 3 } PromptKind;
typedef struct {int x;} *HYPERLINK_POOL_HANDLE;
typedef struct {
    Py_UCS4 *buf;
    size_t len, capacity;
    HYPERLINK_POOL_HANDLE hyperlink_pool;
    hyperlink_id_type active_hyperlink_id;
} ANSIBuf;

typedef struct {
    PyObject_HEAD

    bool bold, italic, reverse, strikethrough, dim, non_blinking;
    monotonic_t position_changed_by_client_at;
    unsigned int x, y;
    uint8_t decoration;
    CursorShape shape;
    color_type fg, bg, decoration_fg;
} Cursor;

typedef struct {
    bool is_focused, render_even_when_unfocused;
    CursorShape shape;
    unsigned int x, y;
    float opacity;
} CursorRenderInfo;

typedef enum DynamicColorType {
    COLOR_NOT_SET, COLOR_IS_SPECIAL, COLOR_IS_INDEX, COLOR_IS_RGB
} DynamicColorType;

typedef union DynamicColor {
    struct {
        color_type rgb: 24;
        DynamicColorType type: 8;
    };
    color_type val;
} DynamicColor;

typedef struct {
    DynamicColor default_fg, default_bg, cursor_color, cursor_text_color, highlight_fg, highlight_bg, visual_bell_color;
} DynamicColors;

typedef struct TransparentDynamicColor {
    color_type color; float opacity; bool is_set;
} TransparentDynamicColor;


#define MARK_MASK (3u)

typedef struct {
    PyObject_HEAD

    bool dirty;
    uint32_t color_table[256], orig_color_table[256];
    TransparentDynamicColor configured_transparent_colors[8], overriden_transparent_colors[8];
    struct { DynamicColors dynamic_colors; uint32_t color_table[256]; TransparentDynamicColor transparent_colors[8]; } *color_stack;
    unsigned int color_stack_idx, color_stack_sz;
    DynamicColors configured, overridden;
    color_type mark_foregrounds[MARK_MASK+1], mark_backgrounds[MARK_MASK+1];
} ColorProfile;

typedef struct {
    unsigned int width, height;
} CellPixelSize;

typedef struct {int x;} *SPRITE_MAP_HANDLE;

typedef struct FontCellMetrics {
    unsigned int cell_width, cell_height, baseline, underline_position, underline_thickness, strikethrough_position, strikethrough_thickness;
} FontCellMetrics;
#define FONTS_DATA_HEAD SPRITE_MAP_HANDLE sprite_map; double logical_dpi_x, logical_dpi_y, font_sz_in_pts; FontCellMetrics fcm;
typedef struct {FONTS_DATA_HEAD} *FONTS_DATA_HANDLE;

#define clear_sprite_position(cell) (cell).sprite_x = 0; (cell).sprite_y = 0; (cell).sprite_z = 0;

#define ensure_space_for(base, array, type, num, capacity, initial_cap, zero_mem) \
    if ((base)->capacity < num) { \
        size_t _newcap = MAX((size_t)initial_cap, MAX(2 * (base)->capacity, (size_t)num)); \
        (base)->array = realloc((base)->array, sizeof(type) * _newcap); \
        if ((base)->array == NULL) fatal("Out of memory while ensuring space for %zu elements in array of %s", (size_t)num, #type); \
        if (zero_mem) memset((base)->array + (base)->capacity, 0, sizeof(type) * (_newcap - (base)->capacity)); \
        (base)->capacity = _newcap; \
    }

#define remove_i_from_array(array, i, count) { \
    (count)--; \
    if ((i) < (count)) { \
        memmove((array) + (i), (array) + (i) + 1, sizeof((array)[0]) * ((count) - (i))); \
    }}

// Global functions
Cursor* alloc_cursor(void);
ColorProfile* alloc_color_profile(void);
void copy_color_profile(ColorProfile*, ColorProfile*);
PyObject* parse_bytes_dump(PyObject UNUSED *, PyObject *);
PyObject* parse_bytes(PyObject UNUSED *, PyObject *);
void cursor_reset(Cursor*);
Cursor* cursor_copy(Cursor*);
void cursor_copy_to(Cursor *src, Cursor *dest);
void cursor_reset_display_attrs(Cursor*);
void cursor_from_sgr(Cursor *self, int *params, unsigned int count, bool is_group);
const char* cursor_as_sgr(const Cursor *);

PyObject* cm_thread_write(PyObject *self, PyObject *args);
bool schedule_write_to_child(unsigned long id, unsigned int num, ...);
bool schedule_write_to_child_python(unsigned long id, const char *prefix, PyObject* tuple_of_str_or_bytes, const char *suffix);
bool set_iutf8(int, bool);

DynamicColor colorprofile_to_color(const ColorProfile *self, DynamicColor entry, DynamicColor defval);
bool colorprofile_to_transparent_color(const ColorProfile *self, unsigned index, color_type *color, float *opacity);
color_type
colorprofile_to_color_with_fallback(ColorProfile *self, DynamicColor entry, DynamicColor defval, DynamicColor fallback, DynamicColor falback_defval);
void copy_color_table_to_buffer(ColorProfile *self, color_type *address, int offset, size_t stride);
bool colorprofile_push_colors(ColorProfile*, unsigned int);
bool colorprofile_pop_colors(ColorProfile*, unsigned int);
void colorprofile_report_stack(ColorProfile*, unsigned int*, unsigned int*);

void set_mouse_cursor(MouseShape);
void enter_event(void);
void mouse_event(const int, int, int);
void focus_in_event(void);
void scroll_event(double, double, int, int);
void on_key_input(const GLFWkeyevent *ev);
void request_window_attention(id_type, bool);
#ifndef __APPLE__
void play_canberra_sound(const char *which_sound, const char *event_id, bool is_path, const char *role, const char *theme_name);
#endif
SPRITE_MAP_HANDLE alloc_sprite_map(unsigned int, unsigned int);
SPRITE_MAP_HANDLE free_sprite_map(SPRITE_MAP_HANDLE);
const char* get_hyperlink_for_id(const HYPERLINK_POOL_HANDLE, hyperlink_id_type id, bool only_url);

#define memset_array(array, val, count) if ((count) > 0) { \
    (array)[0] = (val); \
    size_t __copied__ = 1; \
    while (__copied__ < (count)) { \
        const size_t __num__ = MIN(__copied__, (count) - __copied__); \
        memcpy((array) + __copied__, (array), __num__ * sizeof((val))); \
        __copied__ += __num__; \
    } \
}
