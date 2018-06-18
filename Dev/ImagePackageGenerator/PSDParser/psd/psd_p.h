#ifndef PSD_C_PRIVATE_H_
#define PSD_C_PRIVATE_H_

/* THIS IS A PRIVATE DECLARATIONS OF PSD_C DO NOT INCLUDE DIRECTLY */

#include "./psd.h"
#include <string.h>
#include <float.h>

#ifdef UINT8_MAX
#define PSD_UINT8_MAX UINT8_MAX
#else
#define PSD_UINT8_MAX 255
#endif /* UINT8_MAX */

#ifdef PSD_UINT16_MAX
#define PSD_UINT16_MAX UINT16_MAX
#else
#define PSD_UINT16_MAX 65535
#endif /* PSD_UINT16_MAX */

#ifdef SIZE_MAX
#define PSD_SIZE_MAX SIZE_MAX
#else
#define PSD_SIZE_MAX 4294967295
#endif /* SIZE_MAX */

/* branch predication macros */
#ifdef PSD_ENABLE_BRANCH_PREDICTION
#define psd_likely(expr) __builtin_expected(!!(expr), 1)
#define psd_unlikely(expr) __builtin_expected(!!(expr), 0)
#else
#define psd_likely(expr) (expr)
#define psd_unlikely(expr) (expr)
#endif

/* utility macros */
#define psd_has_error(cond) (psd_unlikely((cond) != PSD_STATUS_SUCCESS))
#define psd_is_null(cond) (psd_unlikely((cond) == PSD_NULL))
#define psd_is_not_null(cond) (psd_likely((cond) != PSD_NULL))
#define psd_mark_unused(cond) ((void)(cond))
#define psd_string_equals(s1, s2) (psd_is_not_null((s1)) && psd_is_not_null((s2)) && strcmp((s1), (s2)) == 0)

/* allocation macros */
#define psd_malloc(size) (__psd_global_allocator->malloc((size)))
#define psd_calloc(length, size) (__psd_global_allocator->calloc((length), (size)))
#define psd_realloc(ptr, size) (__psd_global_allocator->realloc((ptr), (size)))
#define psd_free(ptr) (__psd_global_allocator->free((ptr)))

#define LIST_MALLOC psd_malloc
#define LIST_FREE   psd_free
#include "list/list.h"

typedef void (*psd_list_node_free_t)(void *);
typedef struct psd_descriptor_class_t psd_descriptor_class_t;
typedef struct psd_descriptor_enum_t psd_descriptor_enum_t;
typedef struct psd_descriptor_path_t psd_descriptor_path_t;
typedef struct psd_descriptor_enum_reference_t psd_descriptor_enum_reference_t;

enum psd_layer_channel_id_t {
    PSD_LAYER_CHANNEL_ID_ALPHA   = -1,
    PSD_LAYER_CHANNEL_ID_GRAY    = 0,
    PSD_LAYER_CHANNEL_ID_RED     = 0,
    PSD_LAYER_CHANNEL_ID_GREEN   = 1,
    PSD_LAYER_CHANNEL_ID_BLUE    = 2,
    PSD_LAYER_CHANNEL_ID_CYAN    = 0,
    PSD_LAYER_CHANNEL_ID_MAGENTA = 1,
    PSD_LAYER_CHANNEL_ID_YELLOW  = 2,
    PSD_LAYER_CHANNEL_ID_KEY     = 3
};
typedef enum psd_layer_channel_id_t psd_layer_channel_id_t;

struct psd_buffer_t {
    const psd_uint8_t *data;
    psd_rsize_t length;
    psd_rsize_t offset;
};
struct psd_descriptor_class_t {
    char *name;
    char *id;
};
struct psd_descriptor_enum_t {
    char *type;
    char *value;
};
struct psd_descriptor_path_t {
    char *signature;
    char *value;
};
struct psd_descriptor_enum_reference_t {
    psd_descriptor_class_t *_class;
    psd_descriptor_enum_t *_enum;
};
struct psd_descriptor_value_t {
    char *id;
    psd_descriptor_key_t key;
    union {
        psd_bool_t bval;
        int ival;
        psd_uint64_t lval;
        psd_float64_t dval;
        psd_descriptor_enum_t *eval;
        psd_descriptor_class_t *cval;
        struct {
            int nitems;
            psd_descriptor_value_t **items;
        } a;
        int index;
        int offset;
        char *alias;
        psd_descriptor_path_t *path;
        char *raw;
        char *text;
        psd_descriptor_unit_t uval;
    } u;
};
struct psd_descriptor_t {
    psd_descriptor_class_t *_class;
    list_t *values;
};
struct psd_header_t {
    psd_uint32_t signature;
    psd_uint32_t version;
    psd_uint32_t num_channels;
    psd_size_t size;
    psd_uint32_t depths;
    psd_color_mode_t color_mode;
    psd_uint8_t *color_mode_data;
};
struct psd_resource_slice_t {
    int id;
    int group_id;
    int origin;
    int associated_layer_id;
    char *name;
    int type;
    psd_rect_t bounds;
    char *url;
    char *target;
    char *message;
    char *alt;
    psd_bool_t is_cell_text_html;
    char *cell_text;
    int horizontal_alignment;
    int vertical_alignment;
	psd_uint32_t color;
};
struct psd_resource_thumbnail_t {
    int format;
    psd_size_t size;
    int width_bytes_length;
    int total_bytes_length;
    int compressed_length;
    int bits_per_pixel;
    int num_planes;
    psd_uint8_t *compressed_pixel_data;
};
struct psd_resource_color_sampler_t {
    union {
        int ival;
        float fval;
    } horizontal_position;
    union {
        int ival;
        float fval;
    } vertical_position;
	psd_uint16_t color_space;
	psd_uint16_t depth;
};
struct psd_resource_color_sampler_container_t {
    int version;
    int num_color_samplers;
    psd_resource_color_sampler_t **color_samplers;
};
struct psd_resource_slice_container_t {
    psd_rect_t bounds;
    char *name;
    list_t *items;
};
struct psd_resource_t {
    char *name;
    psd_resource_id_t id;
    psd_rsize_t length;
    const psd_uint8_t *data;
    union {
        struct {
            int num_guides;
            psd_resource_guide_t **values;
        } guide;
        psd_descriptor_t *descriptor;
        struct {
            psd_resource_slice_container_t *data;
            psd_descriptor_t *descriptor;
        } slices;
        psd_resource_thumbnail_t *thumbnail;
        psd_resource_color_sampler_container_t *color_samplers;
    } u;
};
struct psd_resource_container_t {
    const psd_header_t *header;
    list_t *resources;
};
struct psd_path_record_t {
    psd_uint16_t type;
    union {
        int num_points;
        struct {
            psd_float64_t predecent_vert;
            psd_float64_t predecent_horiz;
            psd_float64_t anchor_vert;
            psd_float64_t anchor_horiz;
            psd_float64_t leaving_vert;
            psd_float64_t leaving_horiz;
            psd_bool_t is_closed;
        } bezier;
        struct {
            psd_float64_t top;
            psd_float64_t left;
            psd_float64_t bottom;
            psd_float64_t right;
            psd_float64_t resolution;
        } clipboard;
        int initial_fill;
    } u;
};
struct psd_mask_t {
    psd_rect_t rect;
    int size;
    int default_color;
    int flags;
};
struct psd_blend_mode_t {
    psd_uint32_t key;
    int opacity;
    int clipping;
    int flags;
};
struct psd_layer_channel_t {
    const psd_header_t *header;
    const psd_layer_t *layer;
    psd_layer_channel_id_t id;
    psd_rsize_t data_ptr_length;
    psd_rsize_t raw_data_length;
    const psd_uint8_t *data_ptr;
    psd_uint8_t *raw_data;
};
struct psd_layer_info_t {
    psd_layer_info_key_t key;
    psd_rsize_t length;
    const psd_uint8_t *data;
    struct {
        psd_float64_t xx;
        psd_float64_t xy;
        psd_float64_t yx;
        psd_float64_t yy;
        psd_float64_t tx;
        psd_float64_t ty;
    } transform_matrix;
    union {
        psd_bool_t blend_clipping_element_enabled;
        psd_bool_t blend_interior_element_enabled;
        struct {
            int version;
            int monochrome;
            int values[10];
        } channel_mixer;
        psd_uint32_t fill_opacity_value;
        psd_descriptor_t *gradient_fill;
        psd_bool_t knockout_enabled;
        struct {
            psd_bool_t is_folder;
            psd_bool_t is_hidden;
        } layer_group;
        psd_uint32_t layer_id;
        psd_uint32_t layer_name_source_id;
        struct {
            psd_bool_t is_folder;
            psd_bool_t is_hidden;
            psd_blend_mode_key_t blend_mode;
            int sub_type;
        } layer_section_divider;
        psd_uint32_t layer_version;
        struct {
            psd_bool_t transparency;
            psd_bool_t composite;
            psd_bool_t position;
        } locked;
        psd_descriptor_t *metadata_layer_composition;
        psd_descriptor_t *object_effect;
        /* pattern */
        psd_descriptor_t *placed_layer;
        struct {
            psd_float64_t x;
            psd_float64_t y;
        } reference_point;
        struct {
            psd_uint16_t _1;
            psd_uint16_t _2;
            psd_uint16_t _3;
            psd_uint16_t _4;
        } sheet_color;
        psd_descriptor_t *solid_color;
        psd_descriptor_t *type_tool;
        char *unicode_name;
        struct {
            psd_uint32_t tags;
            psd_uint32_t nrecords;
            list_t *path_records;
        } vector_mask;
        psd_descriptor_t *vector_origination;
        psd_descriptor_t *vector_stroke;
        struct {
            psd_uint32_t key;
            psd_descriptor_t *value;
        } vector_stroke_content;
        psd_bool_t using_aligned_rendering;
    } u;
};
struct psd_layer_t {
    /* position and channels */
    const psd_header_t *header;
    psd_rect_t rect;
    int num_channels;
    psd_layer_channel_t **channels;
    /* blend modes */
    psd_blend_mode_t *blend_mode;
    /* mask */
    psd_mask_t *mask;
    /* blending range */
    psd_blending_range_t gray_blending_ranges;
    int num_channel_blending_ranges;
    psd_blending_range_t *channel_blending_ranges;
    /* legacy pascal name */
    char *name;
    /* layer info */
    list_t *info;
    psd_uint8_t *pixel_data;
};
struct psd_layer_container_t {
    const psd_header_t *header;
    list_t *layers;
    float opacity;
    int color_components[4];
    int overlay_color_space;
    int kind;
    int merged_alpha;
};
struct psd_image_t {
    const psd_header_t *header;
    psd_layer_channel_t *channel;
    psd_buffer_t *buffer;
    psd_uint8_t *pixel_data;
    psd_image_compression_t compression;
    psd_bool_t has_mask;
};
struct psd_document_t {
    psd_header_t *header;
    psd_resource_container_t *resources;
    psd_image_t *image;
    psd_layer_container_t *layers;
};

static const psd_global_allocator_t __psd_global_default_allocator = {
    malloc,
    calloc,
    realloc,
    free
};
static const psd_global_allocator_t *__psd_global_allocator = &__psd_global_default_allocator;

void
psdDescriptorDestroy(psd_descriptor_t *descriptor);
void
psdResourceDestroy(psd_resource_t *resource);
void
psdLayerDestroy(psd_layer_t *layer);

/* private functions */

PSD_DECL_INLINE static int
psdUtilPadInt32Align2(int value)
{
    return ((value + 1) & ~0x01);
}

PSD_DECL_INLINE static psd_rsize_t
psdUtilPadSizeAlign2(psd_rsize_t value)
{
    return ((value + 1) & ~0x01);
}

PSD_DECL_INLINE static int
psdUtilPadAlign4(int value)
{
    return ((value + 4) & ~0x03) - 1;
}

PSD_DECL_INLINE static int
psdUtilAlignDiv8(int value)
{
    return (value + 7) / 8;
}

PSD_DECL_INLINE static unsigned int
psdUtilMinUnsigned(unsigned int value, unsigned int min)
{
    return value < min ? min : value;
}

PSD_DECL_INLINE static unsigned int
psdUtilMaxUnsigned(unsigned int value, unsigned int max)
{
    return value > max ? max : value;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilHasPlusWrapAround(psd_rsize_t left, psd_rsize_t right)
{
    return PSD_SIZE_MAX - left < right;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilHasMinusWrapAround(psd_rsize_t left, psd_rsize_t right)
{
    return left < right;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilIsNumChannelsInRange(int num_channels)
{
    return num_channels >= 1 && num_channels <= 56;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilIsDepthsInRange(int depths)
{
    return depths == 1 || depths == 8 || depths == 16 || depths == 32;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilIsColorModeInRange(int mode)
{
    return mode >= PSD_COLOR_MODE_BITMAP && mode < PSD_COLOR_MODE_MAX_ENUM;
}

PSD_DECL_INLINE static psd_bool_t
psdUtilIsValidLayerInfoSignature(int value)
{
    return value == PSD_MAKE_TAG('8', 'B', 'I', 'M') || value == PSD_MAKE_TAG('8', 'B', '6', '4');
}

PSD_DECL_INLINE static psd_bool_t
psdUtilIsInRangeUnsigned(unsigned int value, unsigned int min, unsigned int max)
{
    return value >= min && value <= max;
}

PSD_DECL_INLINE static void
psdStringDestroy(char **string)
{
    if (psd_is_not_null(*string)) {
        psd_free(*string);
        *string = PSD_NULL;
    }
}

PSD_DECL_INLINE static void
psdBufferDataDestroy(psd_uint8_t **buffer)
{
    if (psd_is_not_null(*buffer)) {
        psd_free(*buffer);
        *buffer = PSD_NULL;
    }
}

PSD_DECL_INLINE static void
psdColorConvertCmyk2Rgb(psd_uint32_t c, psd_uint32_t m, psd_uint32_t y, psd_uint32_t k,
                        psd_uint32_t *r, psd_uint32_t *g, psd_uint32_t *b)
{
    *r = (PSD_UINT16_MAX - (c * (PSD_UINT8_MAX - k) + (k << 8))) >> 8;
    *g = (PSD_UINT16_MAX - (m * (PSD_UINT8_MAX - k) + (k << 8))) >> 8;
    *b = (PSD_UINT16_MAX - (y * (PSD_UINT8_MAX - k) + (k << 8))) >> 8;
}

static psd_rsize_t
psdBufferGetRestLength(const psd_buffer_t *buffer)
{
    return (psd_is_not_null(buffer) && !psdUtilHasMinusWrapAround(buffer->length, buffer->offset)) ? buffer->length - buffer->offset : 0;
}

psd_float64_t
psdBufferReadPathNumber(psd_buffer_t *buffer, psd_bool_t *ok)
{
    /* TODO: implement this */
    return psdBufferReadInt32BigEndian(buffer, ok);
}

PSD_DECL_INLINE static void
psdSizeParse(psd_size_t *size, psd_buffer_t *buffer, psd_bool_t *ok)
{
    if (psd_is_not_null(size)) {
        size->rows = psdBufferReadInt32BigEndianUnsigned(buffer, ok);
        size->columns = psdBufferReadInt32BigEndianUnsigned(buffer, ok);
    }
}

PSD_DECL_INLINE static psd_bool_t
psdSizeIsInRange(const psd_size_t *size, unsigned int min, unsigned int max)
{
    return psdUtilIsInRangeUnsigned(size->columns, min, max) && psdUtilIsInRangeUnsigned(size->rows, min, max);
}

PSD_DECL_INLINE static void
psdRectParse(psd_rect_t *rect, psd_buffer_t *buffer, psd_bool_t *ok)
{
    if (psd_is_not_null(rect)) {
        rect->top = psdBufferReadInt32BigEndian(buffer, ok);
        rect->left = psdBufferReadInt32BigEndian(buffer, ok);
        rect->bottom = psdBufferReadInt32BigEndian(buffer, ok);
        rect->right = psdBufferReadInt32BigEndian(buffer, ok);
    }
}

PSD_DECL_INLINE static void
psdLayerBlendingRangeParse(psd_blending_range_t *range, psd_buffer_t *buffer, psd_bool_t *ok)
{
    if (psd_is_not_null(range)) {
        range->source.black.from      = psdBufferReadByte(buffer, ok);
        range->source.black.to        = psdBufferReadByte(buffer, ok);
        range->source.white.from      = psdBufferReadByte(buffer, ok);
        range->source.white.to        = psdBufferReadByte(buffer, ok);
        range->destination.black.from = psdBufferReadByte(buffer, ok);
        range->destination.black.to   = psdBufferReadByte(buffer, ok);
        range->destination.white.from = psdBufferReadByte(buffer, ok);
        range->destination.white.to   = psdBufferReadByte(buffer, ok);
    }
}

static psd_resource_slice_t *
psdResourceSliceItemCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_slice_t));
}

static psd_status_t
psdResourceSliceItemParse(psd_resource_slice_t *item, psd_buffer_t *buffer)
{
    psd_uint8_t a, r, g, b;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(item)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE_SLICE;
    }
    item->id = psdBufferReadInt32BigEndian(buffer, &ok);
    item->group_id = psdBufferReadInt32BigEndian(buffer, &ok);
    item->origin = psdBufferReadInt32BigEndian(buffer, &ok);
    if (item->origin == 1) {
        item->associated_layer_id = psdBufferReadInt32BigEndian(buffer, &ok);
    }
    item->name = psdBufferReadUnicodeString(buffer, &ok);
    item->type = psdBufferReadInt32BigEndian(buffer, &ok);
    psdRectParse(&item->bounds, buffer, &ok);
    item->url = psdBufferReadUnicodeString(buffer, &ok);
    item->target = psdBufferReadUnicodeString(buffer, &ok);
    item->message = psdBufferReadUnicodeString(buffer, &ok);
    item->alt = psdBufferReadUnicodeString(buffer, &ok);
    item->is_cell_text_html = psdBufferReadByte(buffer, &ok) != 0;
    item->cell_text = psdBufferReadUnicodeString(buffer, &ok);
    item->horizontal_alignment = psdBufferReadInt32BigEndian(buffer, &ok);
    item->vertical_alignment = psdBufferReadInt32BigEndian(buffer, &ok);
    a = psdBufferReadByte(buffer, &ok);
    r = psdBufferReadByte(buffer, &ok);
    g = psdBufferReadByte(buffer, &ok);
    b = psdBufferReadByte(buffer, &ok);
    item->color = PSD_MAKE_TAG(a, b, g, r);
    return PSD_STATUS_SUCCESS;
}

static void
psdResourceSliceItemDestroy(psd_resource_slice_t *item)
{
    if (psd_is_not_null(item)) {
        psdStringDestroy(&item->name);
        psdStringDestroy(&item->url);
        psdStringDestroy(&item->target);
        psdStringDestroy(&item->message);
        psdStringDestroy(&item->alt);
        psdStringDestroy(&item->cell_text);
        psd_free(item);
    }
}

static psd_descriptor_value_t *
psdDescriptorValueCreate(void)
{
    return psd_calloc(1, sizeof(psd_descriptor_value_t));
}

PSD_DECL_INLINE static char *
psdDescriptorParseId(psd_buffer_t *buffer)
{
    psd_bool_t ok;
    psd_rsize_t length = psdBufferReadLength(buffer, &ok);
    return psdBufferReadBuffer(buffer, length == 0 ? 4 : length, &ok);
}

PSD_DECL_INLINE static psd_descriptor_class_t *
psdDescriptorCreateClass(void)
{
    return psd_calloc(1, sizeof(psd_descriptor_class_t));
}

PSD_DECL_INLINE static void
psdDescriptorParseClass(psd_descriptor_class_t *_class, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_not_null(_class)) {
        _class->id = psdBufferReadUnicodeString(buffer, &ok);
        _class->name = psdDescriptorParseId(buffer);
    }
}

static void
psdDescriptorDestroyClass(psd_descriptor_class_t *_class)
{
    if (psd_is_not_null(_class)) {
        psdStringDestroy(&_class->id);
        psdStringDestroy(&_class->name);
        psd_free(_class);
    }
}

PSD_DECL_INLINE static psd_descriptor_enum_t *
psdDescriptorCreateEnum(void)
{
    return psd_calloc(1, sizeof(psd_descriptor_enum_t));
}

PSD_DECL_INLINE static void
psdDescriptorParseEnum(psd_descriptor_enum_t *_enum, psd_buffer_t *buffer)
{
    if (psd_is_not_null(_enum)) {
        _enum->type = psdDescriptorParseId(buffer);
        _enum->value = psdDescriptorParseId(buffer);
    }
}

static void
psdDescriptorDestroyEnum(psd_descriptor_enum_t *_enum)
{
    if (psd_is_not_null(_enum)) {
        psdStringDestroy(&_enum->type);
        psdStringDestroy(&_enum->value);
        psd_free(_enum);
    }
}

static psd_status_t
psdDescriptorParseValue(psd_descriptor_value_t *value, psd_buffer_t *buffer)
{
    psd_descriptor_key_t key;
    psd_descriptor_value_t **items, *item;
    psd_bool_t ok;
    psd_rsize_t length, nitems, i;
    value->key = key = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
    switch (key) {
    case PSD_DESCRIPTOR_BOOL:
        value->u.bval = psdBufferReadByte(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_CLASS:
    case PSD_DESCRIPTOR_CLASS_REF:
    case PSD_DESCRIPTOR_GLOBAL_CLASS:
        value->u.cval = psdDescriptorCreateClass();
        psdDescriptorParseClass(value->u.cval, buffer);
        break;
    case PSD_DESCRIPTOR_OBJECT:
    case PSD_DESCRIPTOR_GLOBAL_OBJECT:
        break;
    case PSD_DESCRIPTOR_DOUBLE:
        value->u.dval = psdBufferReadFloat64BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_ENUM:
        value->u.eval = psdDescriptorCreateEnum();
        psdDescriptorParseEnum(value->u.eval, buffer);
        break;
    case PSD_DESCRIPTOR_ALIAS:
        length = psdBufferReadLength(buffer, &ok);
        value->u.alias = psdBufferReadBuffer(buffer, length, &ok);
        break;
    case PSD_DESCRIPTOR_FILEPATH:
        break;
    case PSD_DESCRIPTOR_INT:
        value->u.ival = psdBufferReadFloat32BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_LARGE_INT:
        value->u.lval = psdBufferReadFloat64BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_LIST:
        value->u.a.nitems = nitems = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        if (nitems > 0) {
            value->u.a.items = items = psd_calloc(nitems, sizeof(*items));
            if (psd_is_not_null(items)) {
                for (i = 0; i < nitems; i++) {
                    items[i] = item = psdDescriptorValueCreate();
                    psdDescriptorParseValue(item, buffer);
                }
            }
            else {
                value->u.a.nitems = 0;
            }
        }
        break;
    case PSD_DESCRIPTOR_OBJECT_ARRAY:
        /* not implemented yet */
        break;
    case PSD_DESCRIPTOR_RAW:
        length = psdBufferReadLength(buffer, &ok);
        value->u.raw = psdBufferReadBuffer(buffer, length, &ok);
        break;
    case PSD_DESCRIPTOR_REFERENCE:
        value->u.a.nitems = nitems = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        if (nitems > 0) {
            value->u.a.items = items  = psd_calloc(nitems, sizeof(*items));
            if (psd_is_not_null(items)) {
                for (i = 0; i < nitems; i++) {
                    items[i] = item = psdDescriptorValueCreate();
                    psdDescriptorParseValue(item, buffer);
                }
            }
            else {
                value->u.a.nitems = 0;
            }
        }
        break;
    case PSD_DESCRIPTOR_UNICODE_TEXT:
    case PSD_DESCRIPTOR_NAME:
        value->u.text = psdBufferReadUnicodeString(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_UNIT_DOUBLE:
        value->u.uval.key = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        value->u.uval.value = psdBufferReadFloat64BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_UNIT_FLOAT:
        value->u.uval.key = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        value->u.uval.value = psdBufferReadFloat32BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_PROPERTY:
        break;
    case PSD_DESCRIPTOR_ENUM_REF:
        break;
    case PSD_DESCRIPTOR_IDENTIFIER:
    case PSD_DESCRIPTOR_INDEX:
    case PSD_DESCRIPTOR_OFFSET:
        value->u.ival = psdBufferReadInt32BigEndian(buffer, &ok);
        break;
    case PSD_DESCRIPTOR_INVALID:
    default:
        break;
    }
    return PSD_STATUS_SUCCESS;
}

static void
psdDescriptorValueDestroy(psd_descriptor_value_t *value)
{
    int nitems, i;
    psdStringDestroy(&value->id);
    switch (value->key) {
    case PSD_DESCRIPTOR_ENUM:
        psdDescriptorDestroyEnum(value->u.eval);
        value->u.eval = PSD_NULL;
        break;
    case PSD_DESCRIPTOR_ALIAS:
        psdStringDestroy(&value->u.alias);
        break;
    case PSD_DESCRIPTOR_CLASS:
    case PSD_DESCRIPTOR_CLASS_REF:
    case PSD_DESCRIPTOR_GLOBAL_CLASS:
        psdDescriptorDestroyClass(value->u.cval);
        value->u.cval = 0;
        break;
    case PSD_DESCRIPTOR_OBJECT:
    case PSD_DESCRIPTOR_GLOBAL_OBJECT:
        break;
    case PSD_DESCRIPTOR_LIST:
        if (psd_is_not_null(value->u.a.items)) {
            nitems = value->u.a.nitems;
            for (i = 0; i < nitems; i++) {
                psdDescriptorValueDestroy(value->u.a.items[i]);
            }
            psd_free(value->u.a.items);
            value->u.a.items = 0;
        }
        break;
    case PSD_DESCRIPTOR_OBJECT_ARRAY:
        /* not implemented yet */
        break;
    case PSD_DESCRIPTOR_FILEPATH:
        break;
    case PSD_DESCRIPTOR_RAW:
        psdStringDestroy(&value->u.raw);
        break;
    case PSD_DESCRIPTOR_UNICODE_TEXT:
    case PSD_DESCRIPTOR_NAME:
        psdStringDestroy(&value->u.text);
        break;
    case PSD_DESCRIPTOR_REFERENCE:
        if (psd_is_not_null(value->u.a.items)) {
            nitems = value->u.a.nitems;
            for (i = 0; i < nitems; i++) {
                psdDescriptorValueDestroy(value->u.a.items[i]);
            }
            psd_free(value->u.a.items);
            value->u.a.items = 0;
        }
        break;
    case PSD_DESCRIPTOR_PROPERTY:
        break;
    case PSD_DESCRIPTOR_ENUM_REF:
        break;
    case PSD_DESCRIPTOR_BOOL:
    case PSD_DESCRIPTOR_DOUBLE:
    case PSD_DESCRIPTOR_INT:
    case PSD_DESCRIPTOR_LARGE_INT:
    case PSD_DESCRIPTOR_UNIT_DOUBLE:
    case PSD_DESCRIPTOR_UNIT_FLOAT:
    case PSD_DESCRIPTOR_IDENTIFIER:
    case PSD_DESCRIPTOR_INDEX:
    case PSD_DESCRIPTOR_OFFSET:
    case PSD_DESCRIPTOR_INVALID:
    default:
        break;
    }
    psd_free(value);
}

psd_layer_channel_t *
psdLayerChannelCreate(const psd_header_t *header, const psd_layer_t *parent)
{
    psd_layer_channel_t *channel = psd_calloc(1, sizeof(psd_layer_channel_t));
    if (psd_is_not_null(channel)) {
        channel->layer = parent;
        channel->header = header;
    }
    return channel;
}

static psd_status_t
psdLayerParsePositionAndChannels(psd_layer_t *layer, psd_buffer_t *buffer)
{
    const psd_header_t *header;
    psd_layer_channel_t **channels, *channel;
    psd_bool_t ok;
    int i, num_channels;
    psdRectParse(&layer->rect, buffer, &ok);
    layer->num_channels = num_channels = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (num_channels > 0) {
        if (!psdUtilIsNumChannelsInRange(num_channels)) {
            return PSD_STATUS_ERROR_INVALID_CHANNEL_RANGE;
        }
        layer->channels = channels = psd_calloc(num_channels, sizeof(*channels));
        if (psd_is_not_null(channels)) {
            header = layer->header;
            for (i = 0; i < num_channels; i++) {
                channels[i] = channel = psdLayerChannelCreate(header, layer);
                if (psd_is_not_null(channel)) {
                    channel->id = psdBufferReadInt16BigEndian(buffer, &ok);
                    channel->data_ptr_length = psdBufferReadLengthBig(header, buffer, &ok);
                    channel->layer = layer;
                }
            }
        }
        else {
            layer->num_channels = 0;
        }
    }
    return PSD_STATUS_SUCCESS;
}

static const psd_layer_info_t *
psdLayerInfoFind(const psd_layer_t *layer, psd_layer_info_key_t key)
{
    psd_layer_info_t *layer_info = 0, *found = 0;
    if (psd_is_not_null(layer) && psd_is_not_null(layer->info)) {
        list_node_t *node = layer->info->head;
        while (psd_is_not_null(node)) {
            layer_info = (psd_layer_info_t *) node->val;
            if (layer_info->key == key) {
                found = layer_info;
                break;
            }
            node = node->next;
        }
    }
    return found;
}

PSD_DECL_INLINE static list_t *
psdResourceContainerCreateList(void)
{
    list_t *resources = list_new();
    if (psd_is_not_null(resources)) {
        resources->free = (psd_list_node_free_t) psdResourceDestroy;
    }
    return resources;
}

PSD_DECL_INLINE static list_t *
psdLayerContainerCreateList(void)
{
    list_t *layers = list_new();
    if (psd_is_not_null(layers)) {
        layers->free = (psd_list_node_free_t) psdLayerDestroy;
    }
    return layers;
}

psd_descriptor_t *
psdDescriptorCreate(void)
{
    psd_descriptor_t *descriptor = psd_calloc(1, sizeof(*descriptor));
    if (psd_is_not_null(descriptor)) {
        descriptor->_class = psdDescriptorCreateClass();
        descriptor->values = list_new();
        if (psd_is_not_null(descriptor->values)) {
            descriptor->values->free = (psd_list_node_free_t) psdDescriptorValueDestroy;
        }
    }
    return descriptor;
}

psd_status_t
psdDescriptorParse(psd_descriptor_t *descriptor, psd_buffer_t *buffer)
{
    psd_descriptor_value_t *value;
    psd_bool_t ok;
    int nitems, i;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(descriptor)) {
        return PSD_STATUS_ERROR_NULL_DESCRIPTOR;
    }
    psdDescriptorParseClass(descriptor->_class, buffer);
    nitems = psdBufferReadInt32BigEndian(buffer, &ok);
    for (i = 0; i < nitems; i++) {
        value = psdDescriptorValueCreate();
        if (value) {
            value->id = psdDescriptorParseId(buffer);
            psdDescriptorParseValue(value, buffer);
            list_rpush(descriptor->values, list_node_new(value));
        }
    }
    return PSD_STATUS_SUCCESS;
}

void
psdDescriptorDestroy(psd_descriptor_t *descriptor)
{
    if (psd_is_not_null(descriptor)) {
        psdDescriptorDestroyClass(descriptor->_class);
        list_destroy(descriptor->values);
        descriptor->values = PSD_NULL;
        psd_free(descriptor);
    }
}

psd_status_t
psdHeaderParse(psd_header_t *header, psd_buffer_t *buffer)
{
    psd_rsize_t color_data_len;
    psd_bool_t ok;
    unsigned int max;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(header)) {
        return PSD_STATUS_ERROR_NULL_HEADER;
    }
    header->signature = psdBufferReadInt32BigEndian(buffer, &ok);
    if (psd_unlikely(header->signature != PSD_MAKE_TAG('8', 'B', 'P', 'S'))) {
        return PSD_STATUS_ERROR_INVALID_HEADER_SIGNATURE;
    }
    header->version = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (psd_unlikely(header->version != 1 && header->version != 2)) {
        return PSD_STATUS_ERROR_INVALID_VERSION;
    }
    psdBufferSkip(buffer, 6, &ok);
    header->num_channels = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (psd_unlikely(!psdUtilIsNumChannelsInRange(header->num_channels))) {
        return PSD_STATUS_ERROR_INVALID_CHANNEL_RANGE;
    }
    psdSizeParse(&header->size, buffer, &ok);
    max = psdHeaderIsBigDocument(header) ? 300000 : 30000;
    if (psd_unlikely(!psdSizeIsInRange(&header->size, 1, max))) {
        return PSD_STATUS_ERROR_INVALID_SIZE_RANGE;
    }
    header->depths = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (psd_unlikely(!psdUtilIsDepthsInRange(header->depths))) {
        return PSD_STATUS_ERROR_INVALID_DEPTH_RANGE;
    }
    header->color_mode = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (psd_unlikely(!psdUtilIsColorModeInRange(header->color_mode))) {
        return PSD_STATUS_ERROR_INVALID_COLOR_MODE;
    }
    color_data_len = psdBufferReadInt32BigEndian(buffer, &ok);
    psdBufferDataDestroy(&header->color_mode_data);
    if (color_data_len > 0) {
        header->color_mode_data = (psd_uint8_t *) psdBufferReadBuffer(buffer, color_data_len, &ok);
    }
    return PSD_STATUS_SUCCESS;
}

psd_resource_slice_container_t *
psdResourceSliceCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_slice_container_t));
}

static psd_status_t
psdResourceSliceParse(psd_resource_slice_container_t *slice, psd_buffer_t *buffer)
{
    psd_resource_slice_t *item;
    psd_bool_t ok;
    psd_uint32_t length, i;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(slice)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE_SLICE;
    }
    psdRectParse(&slice->bounds, buffer, &ok);
    slice->name = psdBufferReadUnicodeString(buffer, &ok);
    length = psdBufferReadLength(buffer, &ok);
    slice->items = list_new();
    if (psd_is_not_null(slice->items)) {
        slice->items->free = (psd_list_node_free_t) psdResourceSliceItemDestroy;
    }
    for (i = 0; i < length; i++) {
        item = psdResourceSliceItemCreate();
        psdResourceSliceItemParse(item, buffer);
        list_rpush(slice->items, list_node_new(item));
    }
    return PSD_STATUS_SUCCESS;
}

void
psdResourceSliceDestroy(psd_resource_slice_container_t *slice)
{
    if (psd_is_not_null(slice)) {
        psdStringDestroy(&slice->name);
        if (psd_is_not_null(slice->items)) {
            list_destroy(slice->items);
        }
        psd_free(slice);
    }
}

static psd_resource_thumbnail_t *
psdResourceThumbnailCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_thumbnail_t));
}

static psd_status_t
psdResourceThumbnailParse(psd_resource_thumbnail_t *thumbnail, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    thumbnail->format = psdBufferReadInt32BigEndian(buffer, &ok);
    psdSizeParse(&thumbnail->size, buffer, &ok);
    thumbnail->width_bytes_length = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
    thumbnail->total_bytes_length = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
    thumbnail->compressed_length = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
    thumbnail->bits_per_pixel = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    thumbnail->num_planes = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    thumbnail->compressed_pixel_data = (psd_uint8_t *) psdBufferReadBuffer(buffer, thumbnail->compressed_length, &ok);
    return PSD_STATUS_SUCCESS;
}

static void
psdResourceThumbnailDestroy(psd_resource_thumbnail_t *thumbnail)
{
    if (psd_is_not_null(thumbnail)) {
        psdBufferDataDestroy(&thumbnail->compressed_pixel_data);
        psd_free(thumbnail);
    }
}

static psd_resource_color_sampler_t *
psdResourceColorSamplerItemCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_color_sampler_t));
}

static psd_status_t
psdResourceColorSamplerItemParse(psd_resource_color_sampler_t *item, psd_buffer_t *buffer, int version)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (version == 3) {
        psdBufferSkip(buffer, 4, &ok); /* version */
    }
    item->horizontal_position.ival = psdBufferReadInt32BigEndian(buffer, &ok);
    item->vertical_position.ival = psdBufferReadInt32BigEndian(buffer, &ok);
    item->color_space = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    if (version == 2) {
        item->depth = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    }
    return PSD_STATUS_SUCCESS;
}

static void
psdResourceColorSamplerItemDestroy(psd_resource_color_sampler_t *item)
{
    if (psd_is_not_null(item)) {
        psd_free(item);
    }
}

static psd_resource_color_sampler_container_t *
psdResourceColorSamplerCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_color_sampler_container_t));
}

static psd_status_t
psdResourceColorSamplerParse(psd_resource_color_sampler_container_t *color_sampler, psd_buffer_t *buffer)
{
    psd_resource_color_sampler_t *item;
    psd_bool_t ok;
    psd_rsize_t length, i;
    int version;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    color_sampler->version = version = psdBufferReadInt32BigEndian(buffer, &ok);
    color_sampler->num_color_samplers = length = psdBufferReadLength(buffer, &ok);
    if (color_sampler->num_color_samplers > 0) {
        color_sampler->color_samplers = psd_calloc(length, sizeof(*color_sampler->color_samplers));
        if (psd_is_not_null(color_sampler->color_samplers)) {
            for (i = 0; i < length; i++) {
                color_sampler->color_samplers[i] = item = psdResourceColorSamplerItemCreate();
                psdResourceColorSamplerItemParse(item, buffer, version);
            }
        }
        else {
            color_sampler->color_samplers = PSD_NULL;
        }
    }
    return PSD_STATUS_SUCCESS;
}

static void
psdResourceColorSamplerDestroy(psd_resource_color_sampler_container_t *sampler)
{
    psd_rsize_t length, i;
    if (psd_is_not_null(sampler)) {
        length = sampler->num_color_samplers;
        for (i = 0; i < length; i++) {
            psdResourceColorSamplerItemDestroy(sampler->color_samplers[i]);
        }
        psd_free(sampler);
    }
}

static psd_status_t
psdResourceParseGuides(psd_resource_t *resource, psd_buffer_t *buffer)
{
    psd_resource_guide_t **guides, *guide;
    psd_bool_t ok;
    int nguides, i;
    if (psd_is_null(resource)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE;
    }
    psdBufferReadInt32BigEndian(buffer, &ok); /* version */
    psdBufferSkip(buffer, 8, &ok); /* reserved */
    resource->u.guide.num_guides = nguides = psdBufferReadInt32BigEndian(buffer, &ok);
    if (resource->u.guide.num_guides > 0) {
        resource->u.guide.values = guides = psd_calloc(nguides, sizeof(*guides));
        if (psd_is_not_null(guides)) {
            for (i = 0; i < nguides; i++) {
                guides[i] = guide = psd_calloc(1, sizeof(*guide));
                guide->location = psdBufferReadInt32BigEndian(buffer, &ok) / 32;
                guide->direction = psdBufferReadByte(buffer, &ok) == 0 ? PSD_DIRECTION_VERTICAL : PSD_DIRECTION_HORIZONTAL;
            }
        }
        else {
            resource->u.guide.num_guides = 0;
        }
    }
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdResourceParseDescriptor(psd_resource_t *resource, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_null(resource)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE;
    }
    psd_uint32_t version = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
    if (version != 16) {
        return PSD_STATUS_ERROR_UNSUPPORTED_RESOURCE_DESCRIPTOR_VERSION;
    }
    resource->u.descriptor = psdDescriptorCreate();
    return psdDescriptorParse(resource->u.descriptor, buffer);
}

static psd_status_t
psdResourceParseSlices(psd_resource_t *resource, psd_buffer_t *buffer)
{
    psd_uint32_t version, descriptor_version;
    psd_bool_t ok;
    if (psd_is_null(resource)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE_SLICE;
    }
    resource->u.slices.data = psdResourceSliceCreate();
    resource->u.slices.descriptor = PSD_NULL;
    if (psd_is_not_null(resource->u.slices.data)) {
        version = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        switch (version) {
        case 6:
            return psdResourceSliceParse(resource->u.slices.data, buffer);
        case 7:
        case 8:
            descriptor_version = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
            if (descriptor_version != 16) {
                return PSD_STATUS_ERROR_UNSUPPORTED_RESOURCE_DESCRIPTOR_VERSION;
            }
            resource->u.slices.descriptor = psdDescriptorCreate();
            psdDescriptorParse(resource->u.slices.descriptor, buffer);
            break;
        default:
            return PSD_STATUS_ERROR_UNSUPPORTED_RESOURCE_SLICE_VERSION;
        }
    }
    return PSD_STATUS_SUCCESS;
}

psd_resource_t *
psdResourceCreate(void)
{
    return psd_calloc(1, sizeof(psd_resource_t));
}

psd_status_t
psdResourceParse(psd_resource_t *resource, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    int signature, len;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(resource)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE;
    }
    signature = psdBufferReadInt32BigEndian(buffer, &ok);
    if (signature != PSD_MAKE_TAG('8', 'B', 'I', 'M')) {
        return PSD_STATUS_ERROR_INVALID_RESOURCE_SIGNATURE;
    }
    resource->id = psdBufferReadInt16BigEndian(buffer, &ok);
    len = psdUtilPadInt32Align2(psdBufferReadByte(buffer, &ok) + 1) - 1;
    resource->name = psdBufferReadBuffer(buffer, len, &ok);
    resource->length = psdUtilPadInt32Align2(psdBufferReadInt32BigEndian(buffer, &ok));
    resource->data = psdBufferGetDataPtr(buffer);
    return PSD_STATUS_SUCCESS;
}

psd_status_t
psdResourceContainerParse(psd_resource_container_t *container, psd_buffer_t *buffer)
{
    psd_resource_t *resource;
    psd_status_t rc;
    psd_rsize_t length, resource_end, container_end;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(container)) {
        return PSD_STATUS_ERROR_NULL_RESOURCE;
    }
    length = psdBufferReadLength(buffer, &ok);
    if (length == 0) {
        return PSD_STATUS_SUCCESS;
    }
    else if (ok) {
        container_end = buffer->offset + length;
        if (psd_is_not_null(container->resources) && container->resources->len > 0) {
            list_destroy(container->resources);
            container->resources = psdResourceContainerCreateList();
        }
        while (buffer->offset < container_end) {
            resource = psdResourceCreate();
            rc = psdResourceParse(resource, buffer);
            if (psd_has_error(rc)) {
                goto error;
            }
            if (psdBufferCanReadLength(buffer, resource->length)) {
                resource_end = buffer->offset + resource->length;
                switch (resource->id) {
                case PSD_RESOURCE_ID_GUIDE:
                    rc = psdResourceParseGuides(resource, buffer);
                    break;
                case PSD_RESOURCE_ID_SLICES:
                    rc = psdResourceParseSlices(resource, buffer);
                    break;
                case PSD_RESOURCE_ID_LAYER_COMP_DESC:
                case PSD_RESOURCE_ID_MEASUREMENT_SCALE_DESC:
                case PSD_RESOURCE_ID_TIMELINE_DESC:
                case PSD_RESOURCE_ID_SHEET_DISCLOSURE_DESC:
                case PSD_RESOURCE_ID_ONION_SKIN_DESC:
                case PSD_RESOURCE_ID_COUNT_DESC:
                case PSD_RESOURCE_ID_PRINT_DESC:
                case PSD_RESOURCE_ID_PRINT_STYLE_DESC:
                case PSD_RESOURCE_ID_PATH_SELECTION_STATE_DESC:
                case PSD_RESOURCE_ID_ORIGIN_PATH_DESC:
                    rc = psdResourceParseDescriptor(resource, buffer);
                    break;
                case PSD_RESOURCE_ID_COLOR_SAMPLER_PS5:
                case PSD_RESOURCE_ID_COLOR_SAMPLER_CS3:
                    psdResourceColorSamplerDestroy(resource->u.color_samplers);
                    resource->u.color_samplers = psdResourceColorSamplerCreate();
                    rc = psdResourceColorSamplerParse(resource->u.color_samplers, buffer);
                    break;
                case PSD_RESOURCE_ID_THUMBNAIL_PS4:
                case PSD_RESOURCE_ID_THUMBNAIL_PS5:
                    psdResourceThumbnailDestroy(resource->u.thumbnail);
                    resource->u.thumbnail = psdResourceThumbnailCreate();
                    rc = psdResourceThumbnailParse(resource->u.thumbnail, buffer);
                    break;
                case PSD_RESOURCE_ID_ICC_PROFILE:
                case PSD_RESOURCE_ID_ICC_UNTAGGED_PROFILE:
                case PSD_RESOURCE_ID_NULL:
                default: /* ignore */
                    break;
                }
                if (psd_has_error(rc)) {
                    goto error;
                }
                if (psd_is_not_null(container->resources)) {
                    list_rpush(container->resources, list_node_new(resource));
                    psdBufferSeek(buffer, resource_end, &ok);
                }
            }
            else {
                rc = PSD_STATUS_ERROR_SIZE_OVERFLOW;
                goto error;
            }
        }
        psdBufferSeek(buffer, container_end, &ok);
        return PSD_STATUS_SUCCESS;
    }
    else {
        return PSD_STATUS_ERROR_SIZE_OVERFLOW;
    }
error:
    psdResourceDestroy(resource);
    return rc;
}

static void
psdPathRecordParsePoints(psd_path_record_t *path_record, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    path_record->u.num_points = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    psdBufferSkip(buffer, 22, &ok);
}

static void
psdPathRecordParseBezierPoints(psd_path_record_t *path_record, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    path_record->u.bezier.predecent_vert = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.predecent_horiz = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.anchor_vert = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.anchor_horiz = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.leaving_vert = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.leaving_horiz = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.bezier.is_closed = (path_record->type == 1 || path_record->type == 4);
}

static void
psdPathRecordParseClipboard(psd_path_record_t *path_record, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    path_record->u.clipboard.top = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.clipboard.left = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.clipboard.bottom = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.clipboard.right = psdBufferReadPathNumber(buffer, &ok);
    path_record->u.clipboard.resolution = psdBufferReadPathNumber(buffer, &ok);
    psdBufferSkip(buffer, 2, &ok);
}

static void
psdPathRecordPathInitialFill(psd_path_record_t *path_record, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    path_record->u.initial_fill = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    psdBufferSkip(buffer, 22, &ok);
}

psd_status_t
psdPathRecordParse(psd_path_record_t *path_record, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(path_record)) {
        return PSD_STATUS_ERROR_NULL_PATH_INFO;
    }
    path_record->type = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
    switch (path_record->type) {
    case 0:
    case 3:
        psdPathRecordParsePoints(path_record, buffer);
        break;
    case 1:
    case 2:
    case 4:
    case 5:
        psdPathRecordParseBezierPoints(path_record, buffer);
        break;
    case 7:
        psdPathRecordParseClipboard(path_record, buffer);
        break;
    case 8:
        psdPathRecordPathInitialFill(path_record, buffer);
        break;
    default:
        psdBufferSkip(buffer, 24, &ok);
        break;
    }
    return PSD_STATUS_SUCCESS;
}

psd_header_t *
psdHeaderCreate(void)
{
    return psd_calloc(1, sizeof(psd_header_t));
}

void
psdHeaderDestroy(psd_header_t *header)
{
    if (psd_is_not_null(header)) {
        psdBufferDataDestroy(&header->color_mode_data);
        psd_free(header);
    }
}

void
psdResourceDestroy(psd_resource_t *resource)
{
    int length, i;
    if (psd_is_not_null(resource)) {
        psdStringDestroy(&resource->name);
        switch (resource->id) {
        case PSD_RESOURCE_ID_GUIDE:
            if (psd_is_not_null(resource->u.guide.values)) {
                length = resource->u.guide.num_guides;
                for (i = 0; i < length; i++) {
                    psd_free(resource->u.guide.values[i]);
                }
                psd_free(resource->u.guide.values);
                resource->u.guide.values = PSD_NULL;
            }
            break;
        case PSD_RESOURCE_ID_SLICES:
            if (psd_is_not_null(resource->u.slices.data)) {
                psdResourceSliceDestroy(resource->u.slices.data);
                resource->u.slices.data = PSD_NULL;
            }
            psdDescriptorDestroy(resource->u.slices.descriptor);
            resource->u.slices.descriptor = PSD_NULL;
            break;
        case PSD_RESOURCE_ID_LAYER_COMP_DESC:
        case PSD_RESOURCE_ID_MEASUREMENT_SCALE_DESC:
        case PSD_RESOURCE_ID_TIMELINE_DESC:
        case PSD_RESOURCE_ID_SHEET_DISCLOSURE_DESC:
        case PSD_RESOURCE_ID_ONION_SKIN_DESC:
        case PSD_RESOURCE_ID_COUNT_DESC:
        case PSD_RESOURCE_ID_PRINT_DESC:
        case PSD_RESOURCE_ID_PRINT_STYLE_DESC:
        case PSD_RESOURCE_ID_PATH_SELECTION_STATE_DESC:
        case PSD_RESOURCE_ID_ORIGIN_PATH_DESC:
            psdDescriptorDestroy(resource->u.descriptor);
            resource->u.descriptor = PSD_NULL;
            break;
        case PSD_RESOURCE_ID_COLOR_SAMPLER_PS5:
        case PSD_RESOURCE_ID_COLOR_SAMPLER_CS3:
            psdResourceColorSamplerDestroy(resource->u.color_samplers);
            resource->u.color_samplers = PSD_NULL;
            break;
        case PSD_RESOURCE_ID_THUMBNAIL_PS4:
        case PSD_RESOURCE_ID_THUMBNAIL_PS5:
            psdResourceThumbnailDestroy(resource->u.thumbnail);
            resource->u.thumbnail = PSD_NULL;
            break;
        case PSD_RESOURCE_ID_ICC_PROFILE:
        case PSD_RESOURCE_ID_ICC_UNTAGGED_PROFILE:
        case PSD_RESOURCE_ID_NULL:
        default:
            break;
        }
        resource->id = resource->length = 0;
        resource->data = PSD_NULL;
        psd_free(resource);
    }
}

psd_resource_container_t *
psdResourceContainerCreate(const psd_header_t *header)
{
    psd_resource_container_t *container = psd_calloc(1, sizeof(*container));
    if (psd_is_not_null(container)) {
        container->header = header;
        container->resources = psdResourceContainerCreateList();
    }
    return container;
}

void
psdResourceContainerDestroy(psd_resource_container_t *container)
{
    if (psd_is_not_null(container)) {
        if (psd_is_not_null(container->resources)) {
            list_destroy(container->resources);
            container->resources = PSD_NULL;
        }
        psd_free(container);
    }
}

psd_path_record_t *
psdPathRecordCreate(void)
{
    return psd_calloc(1, sizeof(psd_path_record_t));
}

void
psdPathRecordDestroy(psd_path_record_t *path_record)
{
    if (psd_is_not_null(path_record)) {
        psd_free(path_record);
    }
}

psd_blend_mode_t *
psdBlendModeCreate(void)
{
    return psd_calloc(1, sizeof(psd_blend_mode_t));
}

psd_status_t
psdBlendModeParse(psd_blend_mode_t *blend_mode, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(blend_mode)) {
        return PSD_STATUS_ERROR_NULL_BLEND_MODE;
    }
    psdBufferSkip(buffer, 4, &ok);
    blend_mode->key = psdBufferReadInt32BigEndian(buffer, &ok);
    blend_mode->opacity = psdBufferReadByte(buffer, &ok);
    blend_mode->clipping = psdBufferReadByte(buffer, &ok);
    blend_mode->flags = psdBufferReadByte(buffer, &ok);
    psdBufferSkip(buffer, 1, &ok);
    return PSD_STATUS_SUCCESS;
}

void
psdBlendModeDestroy(psd_blend_mode_t *blend_mode)
{
    if (psd_is_not_null(blend_mode)) {
        psd_free(blend_mode);
    }
}

psd_mask_t *
psdMaskCreate(void)
{
    return psd_calloc(1, sizeof(psd_mask_t));
}

psd_status_t
psdMaskParse(psd_mask_t *mask, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    psd_rsize_t length;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(mask)) {
        return PSD_STATUS_ERROR_NULL_MASK;
    }
    length = psdBufferReadLength(buffer, &ok);
    if (length == 0) {
        return PSD_STATUS_SUCCESS;
    }
    else if (ok) {
        psd_rsize_t mask_end = buffer->offset + length;
        psdRectParse(&mask->rect, buffer, &ok);
        mask->default_color = psdBufferReadByte(buffer, &ok);
        mask->flags = psdBufferReadByte(buffer, &ok);
        mask->size = length;
        psdBufferSeek(buffer, mask_end, &ok);
        return PSD_STATUS_SUCCESS;
    }
    return PSD_STATUS_ERROR_SIZE_OVERFLOW;
}

void
psdMaskDestroy(psd_mask_t *mask)
{
    if (psd_is_not_null(mask)) {
        psd_free(mask);
    }
}

void
psdLayerChannelDestroy(psd_layer_channel_t *channel)
{
    if (psd_is_not_null(channel)) {
        psdBufferDataDestroy(&channel->raw_data);
        psd_free(channel);
    }
}

psd_layer_info_t *
psdLayerInfoCreate(void)
{
    return psd_calloc(1, sizeof(psd_layer_info_t));
}

void
psdLayerInfoDestroy(psd_layer_info_t *layer_info)
{
    if (psd_is_not_null(layer_info)) {
        switch (layer_info->key) {
        case PSD_LAYER_INFO_ANNOTATIONS:
        case PSD_LAYER_INFO_ANIMATION_EFFECTS:
        case PSD_LAYER_INFO_BLACK_AND_WHITE:
        case PSD_LAYER_INFO_BRIGHTNESS_AND_CONTRAST:
        case PSD_LAYER_INFO_CHANNEL_BLEND_RESTRICTION:
        case PSD_LAYER_INFO_COLOR_LOOKUP:
        case PSD_LAYER_INFO_COTENT_GENERATOR_EXTRA_DATA:
        case PSD_LAYER_INFO_EFFECT_LAYER:
        case PSD_LAYER_INFO_EXPOSURE:
        case PSD_LAYER_INFO_GRADIENT:
        case PSD_LAYER_INFO_FILTER_EFFECTS:
        case PSD_LAYER_INFO_FILTER_EFFECTS2:
        case PSD_LAYER_INFO_FILTER_MASK:
        case PSD_LAYER_INFO_FOREIGN_EFFECT_ID:
        case PSD_LAYER_INFO_LAYER_MASK_AS_GLOBAL_MASK:
        case PSD_LAYER_INFO_LINKED_LAYER:
        case PSD_LAYER_INFO_LINKED_LAYER2:
        case PSD_LAYER_INFO_LINKED_LAYER3:
        case PSD_LAYER_INFO_PATTERN_DATA:
        case PSD_LAYER_INFO_PATTERN_FILL:
        case PSD_LAYER_INFO_PHOTO_FILTER:
        case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY:
        case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY2:
        case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY3:
        case PSD_LAYER_INFO_TEXT_ENGINE_DATA:
        case PSD_LAYER_INFO_TRANSPARENCY_SHAPE_LAYER:
        case PSD_LAYER_INFO_UNICODE_PATH_NAME:
        case PSD_LAYER_INFO_USER_MASK:
        case PSD_LAYER_INFO_VECTOR_MASK_AS_GLOBAL_MASK:
        case PSD_LAYER_INFO_VIBRANCE:
            /* TODO: implemented this */
            break;
        case PSD_LAYER_INFO_GRADIENT_FILL:
            psdDescriptorDestroy(layer_info->u.gradient_fill);
            layer_info->u.gradient_fill = PSD_NULL;
            break;
        case PSD_LAYER_INFO_METADATA_SETTING:
            psdDescriptorDestroy(layer_info->u.metadata_layer_composition);
            layer_info->u.metadata_layer_composition = PSD_NULL;
            break;
        case PSD_LAYER_INFO_OBJECT_EFFECTS:
            psdDescriptorDestroy(layer_info->u.object_effect);
            layer_info->u.object_effect = PSD_NULL;
            break;
        case PSD_LAYER_INFO_PATTERN:
        case PSD_LAYER_INFO_PATTERN2:
        case PSD_LAYER_INFO_PATTERN3:
            /* TODO: implemented this */
            break;
        case PSD_LAYER_INFO_PLACED_LAYER:
        case PSD_LAYER_INFO_PLACED_LAYER2:
            psdDescriptorDestroy(layer_info->u.placed_layer);
            layer_info->u.placed_layer = PSD_NULL;
            break;
        case PSD_LAYER_INFO_SOLID_COLOR:
            psdDescriptorDestroy(layer_info->u.solid_color);
            layer_info->u.solid_color = PSD_NULL;
            break;
        case PSD_LAYER_INFO_TYPE_TOOL:
            psdDescriptorDestroy(layer_info->u.type_tool);
            break;
        case PSD_LAYER_INFO_UNICODE_NAME:
            psdStringDestroy(&layer_info->u.unicode_name);
            break;
        case PSD_LAYER_INFO_VECTOR_MASK:
        case PSD_LAYER_INFO_VECTOR_MASK2:
            if (psd_is_not_null(layer_info->u.vector_mask.path_records)) {
                list_destroy(layer_info->u.vector_mask.path_records);
                layer_info->u.vector_mask.path_records = PSD_NULL;
            }
            break;
        case PSD_LAYER_INFO_VECTOR_ORIGINATION:
            psdDescriptorDestroy(layer_info->u.vector_origination);
            layer_info->u.vector_origination = PSD_NULL;
            break;
        case PSD_LAYER_INFO_VECTOR_STROKE:
            psdDescriptorDestroy(layer_info->u.vector_stroke);
            layer_info->u.vector_stroke = PSD_NULL;
            break;
        case PSD_LAYER_INFO_VECTOR_STROKE_CONTENT:
            psdDescriptorDestroy(layer_info->u.vector_stroke_content.value);
            layer_info->u.vector_stroke_content.value = PSD_NULL;
            break;
        case PSD_LAYER_INFO_BLEND_CLIPPING_ELEMENTS:
        case PSD_LAYER_INFO_BLEND_INTERIOR_ELEMENTS:
        case PSD_LAYER_INFO_CHANNEL_MIXER:
        case PSD_LAYER_INFO_FILL_OPACITY:
        case PSD_LAYER_INFO_KNOCKOUT:
        case PSD_LAYER_INFO_LAYER_ID:
        case PSD_LAYER_INFO_LAYER_NAME_SOURCE:
        case PSD_LAYER_INFO_LAYER_SECTION_DIVIDER:
        case PSD_LAYER_INFO_LAYER_VERSION:
        case PSD_LAYER_INFO_LEGACY_TYPE_TOOL:
        case PSD_LAYER_INFO_LOCKED:
        case PSD_LAYER_INFO_NESTED_LAYER_DIVIDER:
        case PSD_LAYER_INFO_REFERENCE_POINT:
        case PSD_LAYER_INFO_SHEET_COLOR:
        case PSD_LAYER_INFO_USING_ALIGNED_RENDERING:
        case PSD_LAYER_INFO_INVALID:
        default:
            break;
        }
        layer_info->key = PSD_LAYER_INFO_INVALID;
        layer_info->length = 0;
        layer_info->data = PSD_NULL;
        psd_free(layer_info);
    }
}

psd_layer_t *
psdLayerCreate(const psd_header_t *header)
{
    psd_layer_t *layer = psd_calloc(1, sizeof(*layer));
    if (psd_is_not_null(layer)) {
        layer->header = header;
        layer->blend_mode = psdBlendModeCreate();
        layer->mask = psdMaskCreate();
        layer->info = list_new();
        if (psd_is_not_null(layer->info)) {
            layer->info->free = (psd_list_node_free_t) psdLayerInfoDestroy;
        }
    }
    return layer;
}

static psd_status_t
psdLayerParseBlendModes(psd_layer_t *layer, psd_buffer_t *buffer)
{
    return psdBlendModeParse(layer->blend_mode, buffer);
}

static psd_status_t
psdLayerParseMaskData(psd_layer_t *layer, psd_buffer_t *buffer)
{
    return psdMaskParse(layer->mask, buffer);
}

static psd_status_t
psdLayerParseBlendingRange(psd_layer_t *layer, psd_buffer_t *buffer)
{
    psd_blending_range_t *channel_blending_ranges, *channel_blending_range;
    psd_rsize_t length;
    psd_bool_t ok;
    int num_channel_blending_range, i;
	length = psdBufferReadInt32BigEndian(buffer, &ok);
	if (length == 0)
	{
		layer->num_channel_blending_ranges = 0;
		return PSD_STATUS_SUCCESS;
	}

    length = psdUtilMinUnsigned(length, 8);
    psdLayerBlendingRangeParse(&layer->gray_blending_ranges, buffer, &ok);
    layer->num_channel_blending_ranges = num_channel_blending_range = (length - 8) / 8;
    if (num_channel_blending_range > 0) {
        if (psd_is_not_null(layer->channel_blending_ranges)) {
            psd_free(layer->channel_blending_ranges);
        }
        channel_blending_ranges = psd_calloc(num_channel_blending_range, sizeof(*channel_blending_range));
        layer->channel_blending_ranges = channel_blending_ranges;
        if (psd_is_not_null(channel_blending_ranges)) {
            for (i = 0; i < num_channel_blending_range; i++) {
                channel_blending_range = &channel_blending_ranges[i];
                if (channel_blending_range) {
                    psdLayerBlendingRangeParse(channel_blending_range, buffer, &ok);
                }
            }
        }
        else {
            layer->num_channel_blending_ranges = 0;
        }
    }
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdLayerParsePascalName(psd_layer_t *layer, psd_buffer_t *buffer)
{
    psd_rsize_t length;
    psd_bool_t ok;
    length = psdBufferReadByte(buffer, &ok);
    length = psdUtilPadAlign4(length);
    psdStringDestroy(&layer->name);
    layer->name = psdBufferReadBuffer(buffer, length, &ok);
    return PSD_STATUS_SUCCESS;
}

static void
psdLayerInfoParseTransformMatrix(psd_layer_info_t *layer_info, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    layer_info->transform_matrix.xx = psdBufferReadFloat64BigEndian(buffer, &ok);
    layer_info->transform_matrix.xy = psdBufferReadFloat64BigEndian(buffer, &ok);
    layer_info->transform_matrix.yx = psdBufferReadFloat64BigEndian(buffer, &ok);
    layer_info->transform_matrix.yy = psdBufferReadFloat64BigEndian(buffer, &ok);
    layer_info->transform_matrix.tx = psdBufferReadFloat64BigEndian(buffer, &ok);
    layer_info->transform_matrix.ty = psdBufferReadFloat64BigEndian(buffer, &ok);
}

static psd_status_t
psdLayerInfoParse(psd_layer_info_t *layer_info, psd_buffer_t *buffer)
{
    psd_path_record_t *path_record;
    psd_bool_t ok;
    psd_uint32_t length, data_length, i, key;
    psd_int32_t code;
    psd_status_t rc = PSD_STATUS_SUCCESS;
    switch (layer_info->key) {
    case PSD_LAYER_INFO_ANIMATION_EFFECTS:
    case PSD_LAYER_INFO_ANNOTATIONS:
    case PSD_LAYER_INFO_BLACK_AND_WHITE:
    case PSD_LAYER_INFO_BRIGHTNESS_AND_CONTRAST:
    case PSD_LAYER_INFO_COLOR_LOOKUP:
    case PSD_LAYER_INFO_COTENT_GENERATOR_EXTRA_DATA:
    case PSD_LAYER_INFO_EXPOSURE:
    case PSD_LAYER_INFO_FILTER_EFFECTS:
    case PSD_LAYER_INFO_FILTER_EFFECTS2:
    case PSD_LAYER_INFO_FILTER_MASK:
    case PSD_LAYER_INFO_FOREIGN_EFFECT_ID:
    case PSD_LAYER_INFO_LAYER_MASK_AS_GLOBAL_MASK:
    case PSD_LAYER_INFO_LEGACY_TYPE_TOOL:
    case PSD_LAYER_INFO_LINKED_LAYER:
    case PSD_LAYER_INFO_LINKED_LAYER2:
    case PSD_LAYER_INFO_LINKED_LAYER3:
    case PSD_LAYER_INFO_PATTERN_DATA:
    case PSD_LAYER_INFO_PATTERN_FILL:
    case PSD_LAYER_INFO_PHOTO_FILTER:
    case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY:
    case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY2:
    case PSD_LAYER_INFO_SAVING_MERGED_TRANSPARENCY3:
    case PSD_LAYER_INFO_TEXT_ENGINE_DATA:
    case PSD_LAYER_INFO_TRANSPARENCY_SHAPE_LAYER:
    case PSD_LAYER_INFO_UNICODE_PATH_NAME:
    case PSD_LAYER_INFO_USER_MASK:
    case PSD_LAYER_INFO_VECTOR_MASK_AS_GLOBAL_MASK:
    case PSD_LAYER_INFO_VIBRANCE:
        /* TODO: implemented this */
        break;
    case PSD_LAYER_INFO_BLEND_CLIPPING_ELEMENTS:
        layer_info->u.blend_clipping_element_enabled = psdBufferReadInt32BigEndianUnsigned(buffer, &ok) != 0;
        break;
    case PSD_LAYER_INFO_BLEND_INTERIOR_ELEMENTS:
        layer_info->u.blend_interior_element_enabled = psdBufferReadInt32BigEndianUnsigned(buffer, &ok) != 0;
        break;
    case PSD_LAYER_INFO_CHANNEL_BLEND_RESTRICTION:
        /* TODO: implemented this */
        break;
    case PSD_LAYER_INFO_CHANNEL_MIXER:
        layer_info->u.channel_mixer.version = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
        layer_info->u.channel_mixer.monochrome = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
        for (i = 0; i < 10; i++) {
            layer_info->u.channel_mixer.version = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
        }
        break;
    case PSD_LAYER_INFO_EFFECT_LAYER:
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* version */
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* effect count */
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* effect version */
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* signature */
        /* TODO: implement this */
        break;
    case PSD_LAYER_INFO_FILL_OPACITY:
        layer_info->u.fill_opacity_value = psdBufferReadByte(buffer, &ok);
        break;
    case PSD_LAYER_INFO_GRADIENT:
        /* TODO: implement this */
        break;
    case PSD_LAYER_INFO_GRADIENT_FILL:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        layer_info->u.gradient_fill = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.gradient_fill, buffer);
        break;
    case PSD_LAYER_INFO_KNOCKOUT:
        layer_info->u.knockout_enabled = psdBufferReadInt32BigEndianUnsigned(buffer, &ok) != 0;
        break;
    case PSD_LAYER_INFO_LAYER_ID:
        layer_info->u.layer_id = psdBufferReadInt32BigEndian(buffer, &ok);
        break;
    case PSD_LAYER_INFO_LAYER_NAME_SOURCE:
        layer_info->u.layer_name_source_id = psdBufferReadInt32BigEndian(buffer, &ok);
        break;
    case PSD_LAYER_INFO_LAYER_SECTION_DIVIDER:
        code = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        layer_info->u.layer_section_divider.is_folder = (code == 1 || code == 2);
        layer_info->u.layer_section_divider.is_hidden = (code == 3);
        if (layer_info->length >= 12) {
            psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* signature */
            layer_info->u.layer_section_divider.blend_mode = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
            if (layer_info->length >= 16) {
                layer_info->u.layer_section_divider.sub_type = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
            }
        }
        break;
    case PSD_LAYER_INFO_LAYER_VERSION:
        layer_info->u.layer_version = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        break;
    case PSD_LAYER_INFO_LOCKED:
        code = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        layer_info->u.locked.transparency = ((code & 0x1) != 0) || (code == INT_MIN);
        layer_info->u.locked.composite    = ((code & 0x2) != 0) || (code == INT_MIN);
        layer_info->u.locked.position     = ((code & 0x4) != 0) || (code == INT_MIN);
        break;
    case PSD_LAYER_INFO_METADATA_SETTING:
        length = psdBufferReadLength(buffer, &ok);
        for (i = 0; i < length; i++) {
            key = psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* signature */
            psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
            data_length = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
            if (key == PSD_MAKE_TAG('c', 'm', 'l', 's')) { /* layer composition setting */
                psdBufferReadInt32BigEndianUnsigned(buffer, &ok);  /* version */
                psdDescriptorDestroy(layer_info->u.metadata_layer_composition);
                layer_info->u.metadata_layer_composition = psdDescriptorCreate();
                rc = psdDescriptorParse(layer_info->u.metadata_layer_composition, buffer);
                if (psd_has_error(rc)) {
                    break;
                }
            }
            psdBufferSkip(buffer, data_length, &ok);
        }
        break;
    case PSD_LAYER_INFO_NESTED_LAYER_DIVIDER:
        code = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        layer_info->u.layer_group.is_folder = (code == 1 || code == 2);
        layer_info->u.layer_group.is_hidden = (code == 3);
        break;
    case PSD_LAYER_INFO_OBJECT_EFFECTS:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        layer_info->u.object_effect = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.object_effect, buffer);
        break;
    case PSD_LAYER_INFO_PATTERN:
    case PSD_LAYER_INFO_PATTERN2:
    case PSD_LAYER_INFO_PATTERN3:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* length */
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* color mode */
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* vertical point */
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* horizontal point */
        psdBufferReadUnicodeString(buffer, &ok); /* name */
        psdBufferReadBuffer(buffer, psdBufferReadByte(buffer, &ok), &ok); /* pascal string */
        /* TODO: not implemented */
        break;
    case PSD_LAYER_INFO_PLACED_LAYER:
    case PSD_LAYER_INFO_PLACED_LAYER2:
        psdBufferSkip(buffer, 12, &ok);
        layer_info->u.placed_layer = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.placed_layer, buffer);
        break;
    case PSD_LAYER_INFO_REFERENCE_POINT:
        layer_info->u.reference_point.x = psdBufferReadFloat64BigEndian(buffer, &ok);
        layer_info->u.reference_point.y = psdBufferReadFloat64BigEndian(buffer, &ok);
        break;
    case PSD_LAYER_INFO_SHEET_COLOR:
        layer_info->u.sheet_color._1 = psdBufferReadInt16BigEndian(buffer, &ok);
        layer_info->u.sheet_color._2 = psdBufferReadInt16BigEndian(buffer, &ok);
        layer_info->u.sheet_color._3 = psdBufferReadInt16BigEndian(buffer, &ok);
        layer_info->u.sheet_color._4 = psdBufferReadInt16BigEndian(buffer, &ok);
        break;
    case PSD_LAYER_INFO_SOLID_COLOR:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        layer_info->u.solid_color = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.solid_color, buffer);
        break;
    case PSD_LAYER_INFO_TYPE_TOOL:
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* version */
        psdLayerInfoParseTransformMatrix(layer_info, buffer);
        psdBufferReadInt16BigEndianUnsigned(buffer, &ok); /* text version */
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* descriptor version */
        layer_info->u.type_tool = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.type_tool, buffer);
        break;
    case PSD_LAYER_INFO_UNICODE_NAME:
        layer_info->u.unicode_name = psdBufferReadUnicodeString(buffer, &ok);
        break;
    case PSD_LAYER_INFO_USING_ALIGNED_RENDERING:
        layer_info->u.using_aligned_rendering = psdBufferReadInt32BigEndianUnsigned(buffer, &ok) != 0;
        break;
    case PSD_LAYER_INFO_VECTOR_MASK:
    case PSD_LAYER_INFO_VECTOR_MASK2:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        code = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        length = (layer_info->length - 8) / 26;
        layer_info->u.vector_mask.tags = code;
        layer_info->u.vector_mask.nrecords = length;
        layer_info->u.vector_mask.path_records = list_new();
        if (psd_is_not_null(layer_info->u.vector_mask.path_records)) {
            layer_info->u.vector_mask.path_records->free = (psd_list_node_free_t) psdPathRecordDestroy;
            for (i = 0; i < length; i++) {
                path_record = psdPathRecordCreate();
                rc = psdPathRecordParse(path_record, buffer);
                if (psd_has_error(rc)) {
                    break;
                }
                list_rpush(layer_info->u.vector_mask.path_records, list_node_new(path_record));
            }
        }
        break;
    case PSD_LAYER_INFO_VECTOR_ORIGINATION:
        psdBufferSkip(buffer, 8, &ok);
        layer_info->u.vector_origination = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.vector_origination, buffer);
        break;
    case PSD_LAYER_INFO_VECTOR_STROKE:
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        layer_info->u.vector_stroke = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.vector_stroke, buffer);
        break;
    case PSD_LAYER_INFO_VECTOR_STROKE_CONTENT:
        layer_info->u.vector_stroke_content.key = psdBufferReadInt32BigEndianUnsigned(buffer, &ok);
        psdBufferReadInt32BigEndianUnsigned(buffer, &ok); /* version */
        layer_info->u.vector_stroke_content.value = psdDescriptorCreate();
        rc = psdDescriptorParse(layer_info->u.vector_stroke_content.value, buffer);
    case PSD_LAYER_INFO_INVALID:
    default:
        break;
    }
    return rc;
}

static psd_status_t
psdLayerParseLayerInfo(psd_layer_t *layer, psd_buffer_t *buffer, psd_rsize_t layer_end)
{
    psd_layer_info_t *layer_info;
    psd_layer_info_key_t key;
    psd_rsize_t length, layer_info_end;
    psd_status_t rc;
    psd_bool_t ok;
    int signature;
    while (buffer->offset < layer_end) {
        signature = psdBufferReadInt32BigEndian(buffer, &ok);
        if (!psdUtilIsValidLayerInfoSignature(signature)) {
            return PSD_STATUS_ERROR_INVALID_LAYER_INFO_SIGNATURE;
        }
        key = psdBufferReadInt32BigEndian(buffer, &ok);
        switch ((unsigned int)key) {
        case PSD_MAKE_TAG('L', 'M', 's', 'k'): /* user mask */
        case PSD_MAKE_TAG('L', 'r', '1', '6'):
        case PSD_MAKE_TAG('L', 'r', '3', '2'):
        case PSD_MAKE_TAG('L', 'a', 'y', 'r'):
        case PSD_MAKE_TAG('M', 't', '1', '6'): /* saving merged transparency */
        case PSD_MAKE_TAG('M', 't', '3', '2'):
        case PSD_MAKE_TAG('A', 'l', 'p', 'h'): /* unknown */
        case PSD_MAKE_TAG('F', 'M', 's', 'k'): /* filter mask */
        case PSD_MAKE_TAG('I', 'n', 'k', '2'): /* unknown */
        case PSD_MAKE_TAG('F', 'E', 'i', 'd'): /* filter effect */
        case PSD_MAKE_TAG('F', 'X', 'i', 'd'):
        case PSD_MAKE_TAG('P', 'x', 'S', 'D'): /* unknown */
            length = psdBufferReadLengthBig(layer->header, buffer, &ok);
            break;
        default:
            length = psdBufferReadLength(buffer, &ok);
            break;
        }
        layer_info_end = buffer->offset + length;
        if (psd_is_not_null(layer->info)) {
            layer_info = psdLayerInfoCreate();
            if (psd_is_not_null(layer_info)) {
                layer_info->key = key;
                layer_info->length = length;
                layer_info->data = psdBufferGetDataPtr(buffer);
                rc = psdLayerInfoParse(layer_info, buffer);
                if (psd_has_error(rc)) {
                    return rc;
                }
                list_rpush(layer->info, list_node_new(layer_info));
            }
        }
        psdBufferSeek(buffer, layer_info_end, &ok);
    }
    return PSD_STATUS_SUCCESS;
}

psd_status_t
psdLayerParse(psd_layer_t *layer, psd_buffer_t *buffer)
{
    psd_status_t rc;
    psd_rsize_t layer_end, extra_len;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(layer)) {
        return PSD_STATUS_ERROR_NULL_LAYER;
    }
    rc = psdLayerParsePositionAndChannels(layer, buffer);
    if (psd_has_error(rc)) {
        return rc;
    }
    rc = psdLayerParseBlendModes(layer, buffer);
    if (psd_has_error(rc)) {
        return rc;
    }
    extra_len = psdBufferReadInt32BigEndian(buffer, &ok);
    if (extra_len > 0 && !psdBufferCanReadLength(buffer, extra_len)) {
        return PSD_STATUS_ERROR_SIZE_OVERFLOW;
    }
    layer_end = buffer->offset + extra_len;
    rc = psdLayerParseMaskData(layer, buffer);
    if (psd_has_error(rc)) {
        return rc;
    }
    rc = psdLayerParseBlendingRange(layer, buffer);
    if (psd_has_error(rc)) {
        return rc;
    }
    rc = psdLayerParsePascalName(layer, buffer);
    if (psd_has_error(rc)) {
        return rc;
    }
    rc = psdLayerParseLayerInfo(layer, buffer, layer_end);
    if (psd_has_error(rc)) {
        return rc;
    }
    psdBufferSeek(buffer, layer_end, &ok);
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdLayerContainerParseLayers(psd_layer_container_t *container, psd_buffer_t *buffer)
{
    list_node_t *node;
    psd_layer_t *layer;
    psd_layer_channel_t *channel;
    psd_rsize_t layer_info_size;
    psd_status_t rc;
    psd_bool_t ok;
    int i, num_channels;
    layer_info_size = psdUtilPadSizeAlign2(psdBufferReadLengthBig(container->header, buffer, &ok));
    if (layer_info_size > 0) {
        int layer_count = psdBufferReadInt16BigEndian(buffer, &ok);
        if (layer_count < 0) {
            layer_count = abs(layer_count);
            container->merged_alpha = 1;
        }
        if (psd_is_not_null(container->layers) && container->layers->len > 0) {
            list_destroy(container->layers);
            container->layers = psdLayerContainerCreateList();
        }
        for (i = 0; i < layer_count; i++) {
            layer = psdLayerCreate(container->header);
            rc = psdLayerParse(layer, buffer);
            if (psd_has_error(rc)) {
                psdLayerDestroy(layer);
                return rc;
            }
            if (psd_is_not_null(container->layers)) {
                list_rpush(container->layers, list_node_new(layer));
            }
        }
        if (psd_is_not_null(container->layers)) {
            node = container->layers->head;
            while (psd_is_not_null(node)) {
                layer = (psd_layer_t *) node->val;
                num_channels = layer->num_channels;
                for (i = 0; i < num_channels; i++) {
                    channel = layer->channels[i];
                    channel->data_ptr = psdBufferGetDataPtr(buffer);
                    psdBufferSkip(buffer, channel->data_ptr_length, &ok);
                }
                node = node->next;
            }
        }
    }
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdLayerContainerParseGlobalMask(psd_layer_container_t *container, psd_buffer_t *buffer)
{
    psd_rsize_t length, mask_end;
    psd_bool_t ok;
    int i;
    length = psdBufferReadLength(buffer, &ok);
    if (length == 0) {
        return PSD_STATUS_SUCCESS;
    }
    else if (ok) {
        mask_end = buffer->offset + length;
        container->overlay_color_space = psdBufferReadInt16BigEndian(buffer, &ok);
        for (i = 0; i < 4; i++) {
            container->color_components[i] = psdBufferReadInt16BigEndian(buffer, &ok) >> 8;
        }
        container->opacity = psdBufferReadInt16BigEndian(buffer, &ok) / 100.0f;
        container->kind = psdBufferReadByte(buffer, &ok);
        psdBufferSeek(buffer, mask_end, &ok);
        return PSD_STATUS_SUCCESS;
    }
    return PSD_STATUS_ERROR_SIZE_OVERFLOW;
}

static psd_status_t
psdLayerContainerParseLayerTaggedBlocks(psd_layer_container_t *container, psd_buffer_t *buffer, psd_rsize_t rest_size)
{
    psd_status_t rc;
    psd_rsize_t end_offset;
    psd_bool_t ok;
    int key;
    end_offset = buffer->offset + rest_size;
    while (buffer->offset < end_offset) {
        int signature = psdBufferReadInt32BigEndian(buffer, &ok);
        if (!psdUtilIsValidLayerInfoSignature(signature)) {
            return PSD_STATUS_ERROR_INVALID_LAYER_INFO_SIGNATURE;
        }
        key = psdBufferReadInt32BigEndian(buffer, &ok);
        if (key == PSD_MAKE_TAG('L', 'r', '1', '6') || key == PSD_MAKE_TAG('L', 'r', '3', '2') || key == PSD_MAKE_TAG('M', 't', '1', '6')) {
            rc = psdLayerContainerParseLayers(container, buffer);
            if (psd_has_error(rc)) {
                return rc;
            }
        }
    }
    return PSD_STATUS_SUCCESS;
}

void
psdLayerDestroy(psd_layer_t *layer)
{
    int i;
    if (psd_is_not_null(layer)) {
        psdBufferDataDestroy(&layer->pixel_data);
        if (psd_is_not_null(layer->channel_blending_ranges)) {
            psd_free(layer->channel_blending_ranges);
            layer->channel_blending_ranges = PSD_NULL;
        }
        if (psd_is_not_null(layer->channels)) {
            for (i = 0; i < layer->num_channels; i++) {
                psdLayerChannelDestroy(layer->channels[i]);
            }
            psd_free(layer->channels);
            layer->channels = PSD_NULL;
        }
        psdStringDestroy(&layer->name);
        if (psd_is_not_null(layer->info)) {
            list_destroy(layer->info);
            layer->info = PSD_NULL;
        }
        psdBlendModeDestroy(layer->blend_mode);
        layer->blend_mode = PSD_NULL;
        psdMaskDestroy(layer->mask);
        layer->mask = PSD_NULL;
        psd_free(layer);
    }
}

psd_layer_container_t *
psdLayerContainerCreate(const psd_header_t *header)
{
    psd_layer_container_t *container = psd_calloc(1, sizeof(*container));
    if (psd_is_not_null(container)) {
        container->header = header;
        container->layers = psdLayerContainerCreateList();
    }
    return container;
}

psd_status_t
psdLayerContainerParse(psd_layer_container_t *container, psd_buffer_t *buffer)
{
    psd_rsize_t container_size, container_start, container_end, rest_size;
    psd_status_t rc;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(container)) {
        return PSD_STATUS_ERROR_NULL_LAYER;
    }
    container_size = psdBufferReadLengthBig(container->header, buffer, &ok);
    if (container_size == 0) {
        return PSD_STATUS_SUCCESS;
    }
    else if (ok) {
        container_start = buffer->offset;
        container_end = container_start + container_size;
        if (container_size > 0) {
            rc = psdLayerContainerParseLayers(container, buffer);
            if (psd_has_error(rc)) {
                return rc;
            }
            rc = psdLayerContainerParseGlobalMask(container, buffer);
            if (psd_has_error(rc)) {
                return rc;
            }
            rest_size = container_size - (buffer->offset - container_start);
            psdLayerContainerParseLayerTaggedBlocks(container, buffer, rest_size);
        }
        psdBufferSeek(buffer, container_end, &ok);
        return PSD_STATUS_SUCCESS;
    }
    else {
        return PSD_STATUS_ERROR_SIZE_OVERFLOW;
    }
}

void
psdLayerContainerDestroy(psd_layer_container_t *container)
{
    if (psd_is_not_null(container)) {
        if (psd_is_not_null(container->layers)) {
            list_destroy(container->layers);
            container->layers = PSD_NULL;
        }
        psd_free(container);
    }
}

psd_image_t *
psdImageCreate(const psd_header_t *header)
{
    psd_image_t *image = 0;
    if (psd_is_not_null(header)) {
        image = psd_calloc(1, sizeof(*image));
        if (psd_is_not_null(image)) {
            image->header = header;
            image->channel = psdLayerChannelCreate(header, PSD_NULL);
        }
    }
    return image;
}

psd_status_t
psdImageParse(psd_image_t *image, psd_buffer_t *buffer)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(image)) {
        return PSD_STATUS_ERROR_NULL_IMAGE;
    }
    psdBufferDestroy(image->buffer);
    image->compression = psdBufferReadInt16BigEndian(buffer, &ok);
    image->buffer = psdBufferCreate(psdBufferGetDataPtr(buffer), psdBufferGetRestLength(buffer));
    return PSD_STATUS_SUCCESS;
}

void
psdImageDestroy(psd_image_t *image)
{
    if (psd_is_not_null(image)) {
        psdBufferDataDestroy(&image->pixel_data);
        psdBufferDestroy(image->buffer);
        psdLayerChannelDestroy(image->channel);
        psd_free(image);
    }
}

#endif /* PSD_C_PRIVATE_H_ */
