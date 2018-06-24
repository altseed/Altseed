#include "./psd.h"
#include "./psd_p.h"

#include <stdio.h>

void
psdGlobalSetCustomAllocator(const psd_global_allocator_t *allocator)
{
    if (psd_is_not_null(allocator)) {
        __psd_global_allocator = allocator;
    }
    else {
        __psd_global_allocator = &__psd_global_default_allocator;
    }
}

psd_rsize_t
psdBufferGetLength(const psd_buffer_t *buffer)
{
    return psd_is_not_null(buffer) ? buffer->length : 0;
}

psd_rsize_t
psdBufferGetOffset(const psd_buffer_t *buffer)
{
    return psd_is_not_null(buffer) ? buffer->offset : 0;
}

const psd_uint8_t *
psdBufferGetDataPtr(const psd_buffer_t *buffer)
{
    return (psd_is_not_null(buffer) && buffer->offset < buffer->length) ? (buffer->data + buffer->offset) : PSD_NULL;
}

psd_bool_t
psdBufferCanReadLength(const psd_buffer_t *buffer, psd_rsize_t size)
{
    return size > 0 && psd_is_not_null(buffer) && buffer->length >= buffer->offset && buffer->length - buffer->offset >= size;
}

void
psdBufferSeek(psd_buffer_t *buffer, psd_rsize_t position, psd_bool_t *ok)
{
    *ok = psd_is_not_null(buffer) && position < buffer->length;
    if (*ok) {
        buffer->offset = position;
    }
}

psd_uint8_t
psdBufferReadByte(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint8_t result = 0;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        result = *(buffer->data + buffer->offset++);
    }
    return result;
}

psd_int16_t
psdBufferReadInt16BigEndian(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_int16_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_uint16_t
psdBufferReadInt16BigEndianUnsigned(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint16_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_int32_t
psdBufferReadInt32BigEndian(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_int32_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= *(data + buffer->offset++) << 24;
        result |= *(data + buffer->offset++) << 16;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_uint32_t
psdBufferReadInt32BigEndianUnsigned(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint32_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= *(data + buffer->offset++) << 24;
        result |= *(data + buffer->offset++) << 16;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_int64_t
psdBufferReadInt64BigEndian(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_int64_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= (psd_int64_t) (*(data + buffer->offset++)) << 56;
        result |= (psd_int64_t) (*(data + buffer->offset++)) << 48;
        result |= (psd_int64_t) (*(data + buffer->offset++)) << 40;
        result |= (psd_int64_t) (*(data + buffer->offset++)) << 32;
        result |= *(data + buffer->offset++) << 24;
        result |= *(data + buffer->offset++) << 16;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_uint64_t
psdBufferReadInt64BigEndianUnsigned(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint64_t result = 0;
    const psd_uint8_t *data;
    *ok = psdBufferCanReadLength(buffer, sizeof(result));
    if (*ok) {
        data = buffer->data;
        result |= (psd_uint64_t) (*(data + buffer->offset++)) << 56;
        result |= (psd_uint64_t) (*(data + buffer->offset++)) << 48;
        result |= (psd_uint64_t) (*(data + buffer->offset++)) << 40;
        result |= (psd_uint64_t) (*(data + buffer->offset++)) << 32;
        result |= *(data + buffer->offset++) << 24;
        result |= *(data + buffer->offset++) << 16;
        result |= *(data + buffer->offset++) << 8;
        result |= *(data + buffer->offset++);
    }
    return result;
}

psd_float32_t
psdBufferReadFloat32BigEndian(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint32_t result = psdBufferReadInt32BigEndianUnsigned(buffer, ok);
    return (psd_float32_t) result;
}

psd_float64_t
psdBufferReadFloat64BigEndian(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint64_t result = psdBufferReadInt64BigEndianUnsigned(buffer, ok);
    return (psd_float64_t) result;
}

psd_rsize_t
psdBufferReadLength(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_uint32_t length = psdBufferReadInt32BigEndianUnsigned(buffer, ok);
    return ((length == 0 && *ok) || psdBufferCanReadLength(buffer, length)) ? length : 0;
}

psd_rsize_t
psdBufferReadLengthBig(const psd_header_t *header, psd_buffer_t *buffer, psd_bool_t *ok)
{
    if (psdHeaderIsBigDocument(header)) {
        psd_uint64_t length = psdBufferReadInt64BigEndianUnsigned(buffer, ok);
        return ((length == 0 && *ok) || psdBufferCanReadLength(buffer, length)) ? length : 0;
    }
    else {
        return psdBufferReadLength(buffer, ok);
    }
}

char *
psdBufferReadBuffer(psd_buffer_t *buffer, psd_rsize_t length, psd_bool_t *ok)
{
    char *bytes;
    *ok = psdBufferCanReadLength(buffer, length);
    if (*ok) {
        bytes = psd_malloc(length + 2);
        if (psd_is_not_null(bytes)) {
            bytes[length+0] = 0;
			bytes[length+1] = 0;
            memcpy(bytes, buffer->data + buffer->offset, length);
            buffer->offset += length;
        }
    }
    else {
        bytes = psd_calloc(1, sizeof(*bytes));
    }
    return bytes;
}

char *
psdBufferReadUnicodeString(psd_buffer_t *buffer, psd_bool_t *ok)
{
    psd_rsize_t length;
    length = psdBufferReadInt32BigEndianUnsigned(buffer, ok);
    return psdBufferReadBuffer(buffer, length * 2, ok);
}

void
psdBufferSkip(psd_buffer_t *buffer, psd_rsize_t skip, psd_bool_t *ok)
{
    *ok = psdBufferCanReadLength(buffer, skip);
    if (*ok) {
        buffer->offset += skip;
    }
}

psd_rsize_t
psdSizeGetStrideLength(const psd_size_t *size, int depths)
{
    psd_rsize_t width, stride = 1;
    if (psd_is_not_null(size)) {
        width = psdUtilMinUnsigned(size->columns, 1);
        depths = psdUtilAlignDiv8(psdUtilMinUnsigned(depths, 1));
        stride = width * depths;
    }
    return stride;
}

psd_rsize_t
psdSizeGetPixelCount(const psd_size_t *size, int depths)
{
    psd_rsize_t height, num_pixels = 1;
    if (psd_is_not_null(size)) {
        height = psdUtilMinUnsigned(size->rows, 1);
        num_pixels = psdSizeGetStrideLength(size, depths) * height;
    }
    return num_pixels;
}

psd_rsize_t
psdSizeGetImageLength(const psd_size_t *size, int depths)
{
    psd_rsize_t width, height;
    if (psd_is_not_null(size) && depths == 1) {
        width = psdUtilMinUnsigned(size->columns, 1);
        height = psdUtilMinUnsigned(size->rows, 1);
        return ((width + 7) / 8) * height;
    }
    else {
        return psdSizeGetPixelCount(size, depths);
    }
}

psd_bool_t
psdRectIsEmpty(const psd_rect_t *rect)
{
    return psd_is_null(rect) || (psdRectGetWidth(rect) == 0 && psdRectGetHeight(rect) == 0);
}

int
psdRectGetWidth(const psd_rect_t *rect)
{
    return psd_is_not_null(rect) ? (rect->right - rect->left) : 0;
}

int
psdRectGetHeight(const psd_rect_t *rect)
{
    return psd_is_not_null(rect) ? (rect->bottom - rect->top) : 0;
}

psd_buffer_t *
psdBufferCreate(const psd_uint8_t *bytes, psd_rsize_t size)
{
    psd_buffer_t *buffer = psd_calloc(1, sizeof(*buffer));
    if (psd_is_not_null(buffer)) {
        buffer->data = bytes;
        buffer->length = size;
        buffer->offset = 0;
    }
    return buffer;
}

void
psdBufferDestroy(psd_buffer_t *buffer)
{
    if (psd_is_not_null(buffer)) {
        psd_free(buffer);
    }
}

const char *
psdDescriptorGetId(const psd_descriptor_t *descriptor)
{
    return psd_is_not_null(descriptor) ? descriptor->_class->id : PSD_NULL;
}

const char *
psdDescriptorGetName(const psd_descriptor_t *descriptor)
{
    return psd_is_not_null(descriptor) ? descriptor->_class->name : PSD_NULL;
}

psd_descriptor_value_t *const *
psdDescriptorGetValues(const psd_descriptor_t *descriptor, psd_rsize_t *length)
{
    list_node_t *node;
    psd_descriptor_value_t **values = 0;
    int i = 0;
    *length = 0;
    if (psd_is_not_null(descriptor) && psd_is_not_null(descriptor->values)) {
        values = psd_calloc(descriptor->values->len, sizeof(*values));
        if (psd_is_not_null(values)) {
            node = descriptor->values->head;
            while (psd_is_not_null(node)) {
                values[i++] = (psd_descriptor_value_t *) node->val;
                node = node->next;
            }
            *length = i;
        }
    }
    return values;
}

psd_descriptor_key_t
psdDescriptorValueGetKey(const psd_descriptor_value_t *value)
{
    return psd_is_not_null(value) ? value->key : PSD_DESCRIPTOR_INVALID;
}

psd_bool_t
psdDescriptorValueGetBooelan(const psd_descriptor_value_t *value)
{
    psd_descriptor_key_t key;
    if (psd_is_not_null(value)) {
        key = value->key;
        if (psd_likely(key == PSD_DESCRIPTOR_BOOL)) {
            return value->u.bval;
        }
        else if (key == PSD_DESCRIPTOR_INT) {
            return value->u.ival != 0;
        }
        else if (key == PSD_DESCRIPTOR_LARGE_INT) {
            return value->u.lval != 0;
        }
        else if (key == PSD_DESCRIPTOR_DOUBLE) {
            return ((value->u.dval < -DBL_EPSILON) || (value->u.dval > DBL_EPSILON));
        }
        else if (key == PSD_DESCRIPTOR_UNIT_DOUBLE || key == PSD_DESCRIPTOR_UNIT_FLOAT) {
            return ((value->u.uval.value < -DBL_EPSILON) || (value->u.uval.value > DBL_EPSILON));
        }
    }
    return psd_false;
}

int
psdDescriptorValueGetInt32(const psd_descriptor_value_t *value)
{
    psd_descriptor_key_t key;
    if (psd_is_not_null(value)) {
        key = value->key;
        if (psd_likely(key == PSD_DESCRIPTOR_INT)) {
            return value->u.ival;
        }
        else if (key == PSD_DESCRIPTOR_LARGE_INT) {
            return (psd_int32_t) value->u.lval;
        }
        else if (key == PSD_DESCRIPTOR_BOOL) {
            return value->u.bval ? 1 : 0;
        }
        else if (key == PSD_DESCRIPTOR_DOUBLE) {
            return (psd_int32_t) value->u.dval;
        }
        else if (key == PSD_DESCRIPTOR_UNIT_DOUBLE || key == PSD_DESCRIPTOR_UNIT_FLOAT) {
            return (psd_int32_t) value->u.uval.value;
        }
        else if (key == PSD_DESCRIPTOR_UNICODE_TEXT) {
            return (psd_int32_t) strtol(value->u.text, 0, 10);
        }
    }
    return 0;
}

psd_int64_t
psdDescriptorValueGetInt64(const psd_descriptor_value_t *value)
{
    psd_descriptor_key_t key;
    if (psd_is_not_null(value)) {
        key = value->key;
        if (psd_likely(key == PSD_DESCRIPTOR_LARGE_INT)) {
            return value->u.lval;
        }
        else if (key == PSD_DESCRIPTOR_INT) {
            return value->u.ival;
        }
        else if (key == PSD_DESCRIPTOR_BOOL) {
            return value->u.bval ? 1 : 0;
        }
        else if (key == PSD_DESCRIPTOR_DOUBLE) {
            return (psd_int64_t) value->u.dval;
        }
        else if (key == PSD_DESCRIPTOR_UNIT_DOUBLE || key == PSD_DESCRIPTOR_UNIT_FLOAT) {
            return (psd_int64_t) value->u.uval.value;
        }
        else if (key == PSD_DESCRIPTOR_UNICODE_TEXT) {
            return (psd_int64_t) strtoll(value->u.text, 0, 10);
        }
    }
    return 0;
}

psd_float64_t
psdDescriptorValueGetDouble(const psd_descriptor_value_t *value)
{
    psd_descriptor_key_t key;
    if (psd_is_not_null(value)) {
        key = value->key;
        if (psd_likely(key == PSD_DESCRIPTOR_DOUBLE)) {
            return value->u.dval;
        }
        else if (key == PSD_DESCRIPTOR_UNIT_DOUBLE || key == PSD_DESCRIPTOR_UNIT_FLOAT) {
            return value->u.uval.value;
        }
        else if (key == PSD_DESCRIPTOR_INT) {
            return (psd_float64_t) value->u.ival;
        }
        else if (key == PSD_DESCRIPTOR_LARGE_INT) {
            return (psd_float64_t) value->u.lval;
        }
        else if (key == PSD_DESCRIPTOR_BOOL) {
            return value->u.ival ? 1 : 0;
        }
        else if (key == PSD_DESCRIPTOR_UNICODE_TEXT) {
            return strtod(value->u.text, 0);
        }
    }
    return 0;
}

const char *
psdDescriptorValueGetAlias(const psd_descriptor_value_t *value)
{
    return (psd_is_not_null(value) && value->key == PSD_DESCRIPTOR_ALIAS) ? value->u.alias : PSD_NULL;
}

const char *
psdDescriptorValueGetRawData(const psd_descriptor_value_t *value)
{
    return (psd_is_not_null(value) && value->key == PSD_DESCRIPTOR_RAW) ? value->u.raw : PSD_NULL;
}

const char *
psdDescriptorValueGetUnicodeText(const psd_descriptor_value_t *value)
{
    return (psd_is_not_null(value) && value->key == PSD_DESCRIPTOR_UNICODE_TEXT) ? value->u.text : PSD_NULL;
}

const psd_descriptor_unit_t *
psdDescriptorValueGetUnit(const psd_descriptor_value_t *value)
{
    return (psd_is_not_null(value) && (value->key == PSD_DESCRIPTOR_UNIT_DOUBLE || value->key == PSD_DESCRIPTOR_UNIT_FLOAT)) ? &value->u.uval : PSD_NULL;
}

unsigned int
psdHeaderGetVersion(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->version : 0;
}

psd_bool_t
psdHeaderIsBigDocument(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->version >= 2 : psd_false;
}

unsigned int
psdHeaderGetChannelCount(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->num_channels : 0;
}

psd_size_t
psdHeaderGetSize(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->size : psd_size_null;
}

unsigned int
psdHeaderGetDepth(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->depths : 0;
}

psd_color_mode_t
psdHeaderGetColorMode(const psd_header_t *header)
{
    return psd_is_not_null(header) ? header->color_mode : PSD_COLOR_MODE_BITMAP;
}

psd_rsize_t
psdHeaderGetStrideLength(const psd_header_t *header)
{
    return psd_is_not_null(header) ? psdSizeGetStrideLength(&header->size, header->depths) : 1;
}

psd_rsize_t
psdHeaderGetPixelCount(const psd_header_t *header)
{
    return psd_is_not_null(header) ? psdSizeGetPixelCount(&header->size, header->depths) : 1;
}

psd_rsize_t
psdHeaderGetImageLength(const psd_header_t *header)
{
    return psd_is_not_null(header) ? psdSizeGetImageLength(&header->size, header->depths) : 1;
}

psd_resource_t *const *
psdResourceContainerGetAllResources(const psd_resource_container_t *container, psd_rsize_t *length)
{
    list_node_t *node;
    psd_resource_t **resources = 0;
    int i = 0;
    *length = 0;
    if (psd_is_not_null(container) && psd_is_not_null(container->resources)) {
        resources = psd_calloc(container->resources->len, sizeof(psd_resource_t*));
        if (psd_is_not_null(resources)) {
            node = container->resources->head;
            while (psd_is_not_null(node)) {
                resources[i++] = (psd_resource_t *) node->val;
                node = node->next;
            }
            *length = i;
        }
    }
    return resources;
}

const psd_resource_t *
psdResourceContainerGetReosurceById(const psd_resource_container_t *container, psd_resource_id_t value)
{
    list_node_t *node;
    psd_resource_t *resource;
    if (psd_is_not_null(container)) {
        node = container->resources->head;
        while (psd_is_not_null(node)) {
            resource = (psd_resource_t *) node->val;
            if (resource->id == value) {
                return resource;
            }
            node = node->next;
        }
    }
    return PSD_NULL;
}

psd_blend_mode_key_t
psdBlendModeGetKey(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? blend_mode->key : PSD_BLEND_MODE_NULL;
}

int
psdBlendModeGetOpacity(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? blend_mode->opacity : 0;
}

psd_bool_t
psdBlendModeIsClipped(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? blend_mode->clipping == 1 : psd_false;
}

psd_bool_t
psdBlendModeIsTransparencyProtected(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? ((blend_mode->flags & 0x01) != 0) : psd_false;
}

psd_bool_t
psdBlendModeIsVisible(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? !((blend_mode->flags & 0x02) != 0) : psd_false;
}

psd_bool_t
psdBlendModeIsObsolete(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? ((blend_mode->flags & 0x04) != 0) : psd_false;
}

psd_bool_t
psdBlendModeIsPixelDataIrrelevant(const psd_blend_mode_t *blend_mode)
{
    return psd_is_not_null(blend_mode) ? ((blend_mode->flags & 0x08) != 0) && ((blend_mode->flags & 0x16) != 0) : psd_false;
}

psd_rect_t
psdMaskGetRect(const psd_mask_t *mask)
{
    return psd_is_not_null(mask) ? mask->rect : psd_rect_null;
}

psd_bool_t
psdMaskIsRelative(const psd_mask_t *mask)
{
    return psd_is_not_null(mask) ? ((mask->flags & 0x01) != 0) : psd_false;
}

psd_bool_t
psdMaskIsDisabled(const psd_mask_t *mask)
{
    return psd_is_not_null(mask) ? ((mask->flags & 0x02) != 0) : psd_false;
}

psd_bool_t
psdMaskIsInverted(const psd_mask_t *mask)
{
    return psd_is_not_null(mask) ? ((mask->flags & 0x04) != 0) : psd_false;
}

int
psdResourceSliceGetId(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->id : 0;
}

int
psdResourceSliceGetGroupId(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->group_id : 0;
}

int
psdResourceSliceGetOrigin(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->origin : 0;
}

int
psdResourceSliceGetAssociatedLayerId(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->associated_layer_id : 0;
}

const char *
psdResourceSliceGetName(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->name : PSD_NULL;
}

int
psdResourceSliceGetType(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->type : 0;
}

psd_rect_t
psdResourceSliceGetBoundRect(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->bounds : psd_rect_null;
}

const char *
psdResourceSliceGetUrl(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->url : PSD_NULL;
}

const char *
psdResourceSliceGetTarget(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->target : PSD_NULL;
}

const char *
psdResourceSliceGetMessage(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->message : PSD_NULL;
}

const char *
psdResourceSliceGetAlt(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->alt : PSD_NULL;
}

psd_bool_t
psdResourceSliceIsCellTextHtml(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->is_cell_text_html : psd_false;
}

const char *
psdResourceSliceGetCellText(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->cell_text : PSD_NULL;
}

int
psdResourceSliceGetVerticalAlignment(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->vertical_alignment : 0;
}

int
psdResourceSliceGetHorizontalAlignment(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->horizontal_alignment : 0;
}

psd_uint32_t
psdResourceSliceGetColor(const psd_resource_slice_t *slice)
{
    return psd_is_not_null(slice) ? slice->color : 0;
}

psd_rect_t
psdResourceSliceContainerGetBoundRect(const psd_resource_slice_container_t *container)
{
    return psd_is_not_null(container) ? container->bounds : psd_rect_null;
}

const char *
psdResourceSliceContainerGetName(const psd_resource_slice_container_t *container)
{
    return psd_is_not_null(container) ? container->name : PSD_NULL;
}

psd_resource_slice_t *const *
psdResourceSliceContainerGetSlices(const psd_resource_slice_container_t *container, psd_rsize_t *length)
{
    list_node_t *node;
    psd_resource_slice_t **slices = 0;
    int i = 0;
    *length = 0;
    if (psd_is_not_null(container) && psd_is_not_null(container->items) && container->items->len > 0) {
        slices = psd_calloc(container->items->len, sizeof(*slices));
        if (psd_is_not_null(slices)) {
            node = container->items->head;
            while (psd_is_not_null(node)) {
                slices[i++] = (psd_resource_slice_t *) node->val;
                node = node->next;
            }
            *length = i;
        }
    }
    return slices;
}

int
psdResourceThumbnailGetFormat(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->format : 0;
}

psd_size_t
psdResourceThumbnailGetSize(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->size : psd_size_null;
}

psd_rsize_t
psdResourceThumbnailGetWidthByteLength(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->width_bytes_length : 0;
}

psd_rsize_t
psdResourceThumbnailGetTotalByteLength(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->total_bytes_length : 0;
}

psd_rsize_t
psdResourceThumbnailGetCompressedLength(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->compressed_length : 0;
}

int
psdResourceThumbnailGetBitsPerPixel(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->bits_per_pixel : 0;
}

int
psdResourceThumbnailGetPlaneCount(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->num_planes : 0;
}

const psd_uint8_t *
psdResourceThumbnailGetCompressedPixelData(const psd_resource_thumbnail_t *thumbnail)
{
    return psd_is_not_null(thumbnail) ? thumbnail->compressed_pixel_data : PSD_NULL;
}

int
psdResourceColorSamplerGetColorSpace(const psd_resource_color_sampler_t *sampler)
{
    return psd_is_not_null(sampler) ? sampler->color_space : 0;
}

int
psdResourceColorSamplerGetDepth(const psd_resource_color_sampler_t *sampler)
{
    return psd_is_not_null(sampler) ? sampler->depth : 0;
}

int
psdResourceColorSamplerGetVerticalPosition(const psd_resource_color_sampler_t *sampler)
{
    return psd_is_not_null(sampler) ? sampler->horizontal_position.ival : 0;
}

int
psdResourceColorSamplerGetHorizontalPosition(const psd_resource_color_sampler_t *sampler)
{
    return psd_is_not_null(sampler) ? sampler->horizontal_position.ival : 0;
}

int
psdResourceColorSamplerContainerGetVersion(const psd_resource_color_sampler_container_t *container)
{
    return psd_is_not_null(container) ? container->version : 0;
}

psd_resource_color_sampler_t *const *
psdResourceColorSamplerContainerGetColorSamplers(const psd_resource_color_sampler_container_t *container, psd_rsize_t *length)
{
    if (psd_is_not_null(container)) {
        *length = container->num_color_samplers;
        return container->color_samplers;
    }
    else {
        *length = 0;
        return PSD_NULL;
    }
}

psd_resource_id_t
psdResourceGetId(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) ? resource->id : 0;
}

psd_rsize_t
psdResourceGetLength(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) ? resource->length : 0;
}

const char *
psdResourceGetName(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) ? resource->name : PSD_NULL;
}

const psd_uint8_t *
psdResourceGetData(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) ? resource->data : PSD_NULL;
}

const psd_descriptor_t *
psdResourceGetDescriptor(const psd_resource_t *resource)
{
    const psd_descriptor_t *descriptor = PSD_NULL;
    if (psd_is_not_null(resource)) {
        switch (resource->id) {
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
            descriptor = resource->u.descriptor;
            break;
        case PSD_RESOURCE_ID_GUIDE:
        case PSD_RESOURCE_ID_SLICES:
        case PSD_RESOURCE_ID_COLOR_SAMPLER_PS5:
        case PSD_RESOURCE_ID_COLOR_SAMPLER_CS3:
        case PSD_RESOURCE_ID_THUMBNAIL_PS4:
        case PSD_RESOURCE_ID_THUMBNAIL_PS5:
        case PSD_RESOURCE_ID_ICC_PROFILE:
        case PSD_RESOURCE_ID_ICC_UNTAGGED_PROFILE:
        case PSD_RESOURCE_ID_NULL:
        default: /* ignore */
            break;
        }
    }
    return descriptor;
}

psd_resource_guide_t *const *
psdResourceGetGuides(const psd_resource_t *resource, psd_rsize_t *length)
{
    if (psd_is_not_null(resource) && resource->id == PSD_RESOURCE_ID_GUIDE) {
        *length = resource->u.guide.num_guides;
        return resource->u.guide.values;
    }
    else {
        *length = 0;
        return PSD_NULL;
    }
}

const psd_resource_slice_container_t *
psdResourceGetSlice(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) && resource->id == PSD_RESOURCE_ID_SLICES ? resource->u.slices.data : PSD_NULL;
}

const psd_resource_color_sampler_container_t *
psdResourceGetColorSampler(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) && (resource->id == PSD_RESOURCE_ID_COLOR_SAMPLER_CS3 || resource->id == PSD_RESOURCE_ID_COLOR_SAMPLER_PS5) ? resource->u.color_samplers : PSD_NULL;
}

const psd_resource_thumbnail_t *
psdResourceGetThumbnail(const psd_resource_t *resource)
{
    return psd_is_not_null(resource) && (resource->id == PSD_RESOURCE_ID_THUMBNAIL_PS4 || resource->id == PSD_RESOURCE_ID_THUMBNAIL_PS5) ? resource->u.thumbnail : PSD_NULL;
}

psd_rsize_t
psdLayerChannelGetLength(const psd_layer_channel_t *channel)
{
    return psd_is_not_null(channel) ? channel->data_ptr_length : 0;
}

const psd_uint8_t *
psdLayerChannelGetDataPtr(const psd_layer_channel_t *channel)
{
    return psd_is_not_null(channel) ? channel->data_ptr : PSD_NULL;
}

const psd_uint8_t *
psdLayerChannelGetRawData(const psd_layer_channel_t *channel)
{
    return psd_is_not_null(channel) ? channel->raw_data : PSD_NULL;
}

psd_status_t
psdLayerChannelParseRaw(psd_layer_channel_t *channel, psd_buffer_t *buffer, psd_rsize_t channel_data_length)
{
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(channel)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    psdBufferDataDestroy(&channel->raw_data);
    channel->raw_data = psd_calloc(channel_data_length, sizeof(*channel->raw_data));
    if (psd_is_not_null(channel->raw_data) && psdBufferCanReadLength(buffer, channel_data_length)) {
        channel->raw_data_length = channel_data_length;
        memcpy(channel->raw_data, psdBufferGetDataPtr(buffer), channel_data_length);
        psdBufferSkip(buffer, channel_data_length, &ok);
        return PSD_STATUS_SUCCESS;
    }
    return PSD_STATUS_ERROR_SIZE_OVERFLOW;
}

psd_status_t
psdLayerChannelParseRle(psd_layer_channel_t *channel,
                        psd_buffer_t *buffer,
                        psd_rsize_t channel_data_length,
                        psd_rsize_t height,
                        psd_rsize_t num_channels,
                        psd_bool_t is_big_document)
{
    psd_uint32_t *bytes_offsets, data_byte_count;
    psd_rsize_t byte_offsets_index, data_end,
            channel_data_offset, channel_and_height, data_byte_length, i,  j;
    psd_status_t rc = PSD_STATUS_SUCCESS;
    psd_uint8_t pixel_color;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(channel)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    channel_and_height = num_channels * height;
    bytes_offsets = psd_calloc(channel_and_height, sizeof(*bytes_offsets));
    if (psd_is_not_null(bytes_offsets)) {
        channel_data_offset = 0;
        if (is_big_document) {
            for (i = 0; i < channel_and_height; i++) {
                bytes_offsets[i] = psdBufferReadInt32BigEndian(buffer, &ok);
            }
        }
        else {
            for (i = 0; i < channel_and_height; i++) {
                bytes_offsets[i] = psdBufferReadInt16BigEndian(buffer, &ok);
            }
        }
        psdBufferDataDestroy(&channel->raw_data);
        channel->raw_data = psd_calloc(channel_data_length, sizeof(*channel->raw_data));
        channel->raw_data_length = channel_data_length;
        for (i = 0; i < num_channels; i++) {
            byte_offsets_index = height * i;
            for (j = 0; j < height; j++) {
                data_byte_count = bytes_offsets[byte_offsets_index + j];
                if (psdUtilHasPlusWrapAround(buffer->offset, data_byte_count) || channel_data_offset >= channel_data_length) {
                    rc = PSD_STATUS_ERROR_SIZE_OVERFLOW;
                    goto error;
                }
                data_end = buffer->offset + data_byte_count;
                while (buffer->offset < data_end) {
                    data_byte_length = psdBufferReadByte(buffer, &ok);
                    if (data_byte_length < 128) {
                        data_byte_length += 1;
                        if (psdUtilHasPlusWrapAround(channel_data_offset, data_byte_length) || channel_data_offset + data_byte_length > channel_data_length) {
                            rc = PSD_STATUS_ERROR_SIZE_OVERFLOW;
                            goto error;
                        }
                        memcpy((channel->raw_data + channel_data_offset), psdBufferGetDataPtr(buffer), data_byte_length);
                        psdBufferSkip(buffer, data_byte_length, &ok);
                    }
                    else if (data_byte_length > 128) {
                        data_byte_length ^= 0xff;
                        data_byte_length += 2;
                        if (psdUtilHasPlusWrapAround(channel_data_offset, data_byte_length) || channel_data_offset + data_byte_length > channel_data_length) {
                            rc = PSD_STATUS_ERROR_SIZE_OVERFLOW;
                            goto error;
                        }
                        pixel_color = psdBufferReadByte(buffer, &ok);
                        memset((channel->raw_data + channel_data_offset), pixel_color, data_byte_length);
                    }
                    else {
                        /* skip 128 */
                        continue;
                    }
                    channel_data_offset += data_byte_length;
                }
            }
        }
    }
error:
    psd_free(bytes_offsets);
    return rc;
}

psd_status_t
psdLayerChannelParseZip(psd_layer_channel_t *channel, psd_buffer_t *buffer, psd_rsize_t channel_data_length)
{
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(channel)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    psd_mark_unused(buffer);
    psd_mark_unused(channel_data_length);
    return PSD_STATUS_ERROR_UNSUPPORTED_IMAGE_COMPRESSION;
}

static psd_status_t
psdLayerChannelCompositeGrayScale(psd_layer_channel_t **channels,
                                  psd_rsize_t num_channels,
                                  psd_uint8_t *pixel_data,
                                  psd_rsize_t channel_data_length,
                                  psd_rsize_t pixel_step)
{
    const psd_layer_channel_t *channel;
    psd_uint32_t *pixel_ptr = (psd_uint32_t *) pixel_data;
    psd_rsize_t i, j;
    psd_uint32_t pixel, g, a;
    if (num_channels == 0 || psd_is_null(channels)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    if (channel_data_length == 0 || pixel_step == 0 || psd_is_null(pixel_data)) {
        return PSD_STATUS_ERROR_NULL_IMAGE;
    }
    for (i = 0; i < channel_data_length; i += pixel_step) {
        g = 0;
        a = 0xff;
        for (j = 0; j < num_channels; j++) {
            channel = channels[j];
            pixel = channel->raw_data[i];
            switch (channel->id) {
            case PSD_LAYER_CHANNEL_ID_GRAY:
                g = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_ALPHA:
                a = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_MAGENTA:
            case PSD_LAYER_CHANNEL_ID_YELLOW:
            case PSD_LAYER_CHANNEL_ID_KEY:
            default:
                /* should not reach here */
                break;
            }
        }
        pixel_ptr[i] = PSD_MAKE_TAG(a, g, g, g);
    }
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdLayerChannelCompositeRgb(psd_layer_channel_t **channels,
                            psd_rsize_t num_channels,
                            psd_uint8_t *pixel_data,
                            psd_rsize_t channel_data_length,
                            psd_rsize_t pixel_step)
{
    const psd_layer_channel_t *channel;
    psd_uint32_t *pixel_ptr = (psd_uint32_t *) pixel_data;
    psd_uint32_t pixel, r, g, b, a;
    psd_rsize_t i, j;
    if (num_channels == 0 || psd_is_null(channels)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    if (channel_data_length == 0 || pixel_step == 0 || psd_is_null(pixel_data)) {
        return PSD_STATUS_ERROR_NULL_IMAGE;
    }
    for (i = 0; i < channel_data_length; i += pixel_step) {
        r = g = b = 0;
        a = 0xff;
        for (j = 0; j < num_channels; j++) {
            channel = channels[j];
            pixel = channel->raw_data[i];
            switch (channel->id) {
            case PSD_LAYER_CHANNEL_ID_RED:
                r = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_GREEN:
                g = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_BLUE:
                b = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_ALPHA:
                a = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_KEY:
            default:
                /* should not reach here */
                break;
            }
        }
        pixel_ptr[i] = PSD_MAKE_TAG(a, b, g, r);
    }
    return PSD_STATUS_SUCCESS;
}

static psd_status_t
psdLayerChannelCompositeCmyk(psd_layer_channel_t **channels,
                             psd_rsize_t num_channels,
                             psd_uint8_t *pixel_data,
                             psd_rsize_t channel_data_length,
                             psd_rsize_t pixel_step)
{
    const psd_layer_channel_t *channel;
    psd_uint32_t *pixel_ptr = (psd_uint32_t *) pixel_data;
    psd_uint32_t pixel, c, m, y, k, r, g, b, a;
    psd_rsize_t i, j;
    if (num_channels == 0 || psd_is_null(channels)) {
        return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
    }
    if (channel_data_length == 0 || pixel_step == 0 || psd_is_null(pixel_data)) {
        return PSD_STATUS_ERROR_NULL_IMAGE;
    }
    for (i = 0; i < channel_data_length; i += pixel_step) {
        c = m = y = k = 0;
        a = 0xff;
        for (j = 0; j < num_channels; j++) {
            channel = channels[j];
            pixel = channel->raw_data[i];
            switch (channel->id) {
            case PSD_LAYER_CHANNEL_ID_CYAN:
                c = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_MAGENTA:
                m = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_YELLOW:
                y = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_KEY:
                k = pixel;
                break;
            case PSD_LAYER_CHANNEL_ID_ALPHA:
                a = pixel;
                break;
            default:
                /* should not reach here */
                break;
            }
        }
        psdColorConvertCmyk2Rgb(PSD_UINT8_MAX - c, PSD_UINT8_MAX - m,
                                PSD_UINT8_MAX - y, PSD_UINT8_MAX - k,
                                &r, &g, &b);
        pixel_ptr[i] = PSD_MAKE_TAG(a, b, g, r);
    }
    return PSD_STATUS_SUCCESS;
}

psd_layer_info_key_t
psdLayerInfoGetKey(const psd_layer_info_t *info)
{
    return psd_is_not_null(info) ? info->key : PSD_LAYER_INFO_INVALID;
}

psd_rsize_t
psdLayerInfoGetLength(const psd_layer_info_t *info)
{
    return psd_is_not_null(info) ? info->length : 0;
}

const psd_uint8_t *
psdLayerInfoGetData(const psd_layer_info_t *info)
{
    return psd_is_not_null(info) ? info->data : PSD_NULL;
}

const psd_descriptor_t *
psdLayerInfoGetDescriptor(const psd_layer_info_t *info)
{
    psd_descriptor_t *descriptor = 0;
    if (psd_is_not_null(info)) {
        switch (info->key) {
        case PSD_LAYER_INFO_GRADIENT_FILL:
            descriptor = info->u.gradient_fill;
            break;
        case PSD_LAYER_INFO_OBJECT_EFFECTS:
            descriptor = info->u.object_effect;
            break;
        case PSD_LAYER_INFO_PLACED_LAYER:
        case PSD_LAYER_INFO_PLACED_LAYER2:
            descriptor = info->u.placed_layer;
            break;
        case PSD_LAYER_INFO_SOLID_COLOR:
            descriptor = info->u.solid_color;
            break;
        case PSD_LAYER_INFO_TYPE_TOOL:
            descriptor = info->u.type_tool;
            break;
        case PSD_LAYER_INFO_VECTOR_ORIGINATION:
            descriptor = info->u.vector_origination;
            break;
        case PSD_LAYER_INFO_VECTOR_STROKE:
            descriptor = info->u.vector_stroke;
            break;
        case PSD_LAYER_INFO_VECTOR_STROKE_CONTENT:
            descriptor = info->u.vector_stroke_content.value;
            break;
        case PSD_LAYER_INFO_ANIMATION_EFFECTS:
        case PSD_LAYER_INFO_BLACK_AND_WHITE:
        case PSD_LAYER_INFO_BRIGHTNESS_AND_CONTRAST:
        case PSD_LAYER_INFO_CHANNEL_MIXER:
        case PSD_LAYER_INFO_COLOR_LOOKUP:
        case PSD_LAYER_INFO_COTENT_GENERATOR_EXTRA_DATA:
        case PSD_LAYER_INFO_EXPOSURE:
        case PSD_LAYER_INFO_FILTER_EFFECTS:
        case PSD_LAYER_INFO_FILTER_EFFECTS2:
        case PSD_LAYER_INFO_FILTER_MASK:
        case PSD_LAYER_INFO_FOREIGN_EFFECT_ID:
        case PSD_LAYER_INFO_KNOCKOUT:
        case PSD_LAYER_INFO_LAYER_MASK_AS_GLOBAL_MASK:
        case PSD_LAYER_INFO_LAYER_VERSION:
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
        case PSD_LAYER_INFO_USING_ALIGNED_RENDERING:
        case PSD_LAYER_INFO_VECTOR_MASK_AS_GLOBAL_MASK:
        case PSD_LAYER_INFO_VIBRANCE:
        case PSD_LAYER_INFO_ANNOTATIONS:
        case PSD_LAYER_INFO_BLEND_CLIPPING_ELEMENTS:
        case PSD_LAYER_INFO_BLEND_INTERIOR_ELEMENTS:
        case PSD_LAYER_INFO_CHANNEL_BLEND_RESTRICTION:
        case PSD_LAYER_INFO_EFFECT_LAYER:
        case PSD_LAYER_INFO_FILL_OPACITY:
        case PSD_LAYER_INFO_GRADIENT:
        case PSD_LAYER_INFO_NESTED_LAYER_DIVIDER:
        case PSD_LAYER_INFO_LAYER_ID:
        case PSD_LAYER_INFO_LAYER_NAME_SOURCE:
        case PSD_LAYER_INFO_LAYER_SECTION_DIVIDER:
        case PSD_LAYER_INFO_LEGACY_TYPE_TOOL:
        case PSD_LAYER_INFO_LOCKED:
        case PSD_LAYER_INFO_METADATA_SETTING:
        case PSD_LAYER_INFO_PATTERN:
        case PSD_LAYER_INFO_PATTERN2:
        case PSD_LAYER_INFO_PATTERN3:
        case PSD_LAYER_INFO_REFERENCE_POINT:
        case PSD_LAYER_INFO_SHEET_COLOR:
        case PSD_LAYER_INFO_UNICODE_NAME:
        case PSD_LAYER_INFO_VECTOR_MASK:
        case PSD_LAYER_INFO_VECTOR_MASK2:
        case PSD_LAYER_INFO_INVALID:
        default:
            descriptor = 0;
            break;
        }
    }
    return descriptor;
}

psd_rect_t
psdLayerGetRect(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->rect : psd_rect_null;
}

const psd_blend_mode_t *
psdLayerGetBlendMode(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->blend_mode : PSD_NULL;
}

const psd_blending_range_t *
psdLayerGetGrayBlendingRange(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? &layer->gray_blending_ranges : PSD_NULL;
}

const psd_blending_range_t *
psdLayerGetChannelBlendingRange(const psd_layer_t *layer, psd_rsize_t *length)
{
    if (psd_is_not_null(layer)) {
        *length = layer->num_channel_blending_ranges;
        return layer->channel_blending_ranges;
    }
    else {
        *length = 0;
        return PSD_NULL;
    }
}

const psd_mask_t *
psdLayerGetMask(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->mask : PSD_NULL;
}

psd_layer_channel_t *const *
psdLayerGetLayerChannels(const psd_layer_t *layer, psd_rsize_t *length)
{
    psd_layer_channel_t **layer_channels = 0;
    *length = 0;
    if (psd_is_not_null(layer) && psd_is_not_null(layer->channels) && layer->num_channels > 0) {
        layer_channels = psd_calloc(layer->num_channels, sizeof(*layer_channels));
        if (psd_is_not_null(layer_channels)) {
            memcpy(layer_channels, layer->channels, sizeof(*layer_channels) * layer->num_channels);
            *length = layer->num_channels;
        }
    }
    return layer_channels;
}

psd_layer_info_t *const *
psdLayerGetLayerInfo(const psd_layer_t *layer, psd_rsize_t *length)
{
    list_node_t *node;
    psd_layer_info_t **layer_info = 0;
    int i = 0;
    *length = 0;
    if (psd_is_not_null(layer) && psd_is_not_null(layer->info) && layer->info->len > 0) {
        layer_info = psd_calloc(layer->info->len, sizeof(*layer_info));
        if (psd_is_not_null(layer_info)) {
            node = layer->info->head;
            while (psd_is_not_null(node)) {
                layer_info[i++] = (psd_layer_info_t *) node->val;
                node = node->next;
            }
            *length = i;
        }
    }
    return layer_info;
}

const char *
psdLayerGetName(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_UNICODE_NAME);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.unicode_name;
    }
    return psdLayerGetPascalName(layer);
}

const char *
psdLayerGetPascalName(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->name : PSD_NULL;
}

int
psdLayerGetChannelCount(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->num_channels : 0;
}

psd_bool_t
psdLayerIsFolderBegin(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_NESTED_LAYER_DIVIDER);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.layer_group.is_folder;
    }
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LAYER_SECTION_DIVIDER);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.layer_section_divider.is_folder;
    }
    return psd_is_not_null(layer) && psd_string_equals(layer->name, "<Layer group>");
}

psd_bool_t
psdLayerIsFolderEnd(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_NESTED_LAYER_DIVIDER);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.layer_group.is_hidden;
    }
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LAYER_SECTION_DIVIDER);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.layer_section_divider.is_hidden;
    }
    return psd_is_not_null(layer) && psd_string_equals(layer->name, "</Layer group>");
}

psd_bool_t
psdLayerIsPositionLocked(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LOCKED);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.locked.position;
    }
    return psd_false;
}

psd_bool_t
psdLayerIsCompositeLocked(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LOCKED);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.locked.composite;
    }
    return psd_false;
}

psd_bool_t
psdLayerIsTransparencyLocked(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LOCKED);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.locked.transparency;
    }
    return psd_false;
}

psd_bool_t
psdLayerIsAllLocked(const psd_layer_t *layer)
{
    const psd_layer_info_t *layer_info;
    layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_LOCKED);
    if (psd_is_not_null(layer_info)) {
        return layer_info->u.locked.composite &&
                layer_info->u.locked.position &&
                layer_info->u.locked.transparency;
    }
    return psd_false;
}

int
psdLayerGetOpacity(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? psdBlendModeGetOpacity(layer->blend_mode) : 0;
}

psd_bool_t
psdLayerIsVisible(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? psdBlendModeIsVisible(layer->blend_mode) : psd_false;
}

psd_bool_t
psdLayerIsClipped(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? psdBlendModeIsClipped(layer->blend_mode) : psd_false;
}

const psd_uint8_t *
psdLayerGetPixelData(const psd_layer_t *layer)
{
    return psd_is_not_null(layer) ? layer->pixel_data : PSD_NULL;
}

psd_status_t
psdLayerExtract(psd_layer_t *layer)
{
    const psd_header_t *header = layer->header;
    psd_buffer_t *buffer;
    psd_layer_channel_t *channel;
    psd_uint8_t *pixel_data;
    psd_image_compression_t compression;
    psd_size_t size;
    psd_rsize_t channel_data_length, pixel_step;
    psd_rect_t rect = psdLayerGetRect(layer);
    psd_bool_t ok;
    psd_status_t rc;
    int num_channels = layer->num_channels, i;
    if (psdRectIsEmpty(&rect)) {
        return PSD_STATUS_ERROR_NULL_LAYER;
    }
    size.columns = psdRectGetWidth(&rect);
    size.rows = psdRectGetHeight(&rect);
    channel_data_length = psdSizeGetImageLength(&size, psdHeaderGetDepth(header));
    pixel_step = psdUtilMinUnsigned(psdHeaderGetDepth(header), 8) / 8;
    psdBufferDataDestroy(&layer->pixel_data);
    
	/* í‚É‰æ‘f‚ð32bit‚Æ‚µ‚Äˆµ‚Á‚Ä‚¢‚½‚½‚ß */
	//layer->pixel_data = pixel_data = psd_calloc(channel_data_length * num_channels * sizeof(psd_uint32_t), sizeof(*layer->pixel_data));
	layer->pixel_data = pixel_data = psd_calloc(channel_data_length * sizeof(psd_uint32_t), sizeof(*layer->pixel_data));

	for (i = 0; i < num_channels; i++) {
        channel = layer->channels[i];
        if (psd_is_null(channel) || psd_is_null(channel->data_ptr)) {
            return PSD_STATUS_ERROR_NULL_LAYER_CHANNEL;
        }
        buffer = psdBufferCreate(channel->data_ptr, channel->data_ptr_length);
        compression = psdBufferReadInt16BigEndianUnsigned(buffer, &ok);
        switch (compression) {
        case PSD_IMAGE_COMPRESSION_RAW:
            rc = psdLayerChannelParseRaw(channel, buffer, channel_data_length);
            break;
        case PSD_IMAGE_COMPRESSION_RLE:
            rc = psdLayerChannelParseRle(channel, buffer, channel_data_length, size.rows, 1, psdHeaderIsBigDocument(header));
            break;
        case PSD_IMAGE_COMPRESSION_ZIP:
        case PSD_IMAGE_COMPRESSION_ZIP_PREDICTION:
            rc = psdLayerChannelParseZip(channel, buffer, channel_data_length);
            break;
        case PSD_IMAGE_COMPRESSION_MAX_ENUM:
        default:
            rc = PSD_STATUS_ERROR_UNSUPPORTED_IMAGE_COMPRESSION;
            break;
        }
        psdBufferDestroy(buffer);
        if (psd_has_error(rc)) {
            return rc;
        }
    }
    switch (psdHeaderGetColorMode(header)) {
    case PSD_COLOR_MODE_GRAY_SCALE:
        return psdLayerChannelCompositeGrayScale(layer->channels, layer->num_channels, pixel_data, channel_data_length, pixel_step);
    case PSD_COLOR_MODE_RGB:
        return psdLayerChannelCompositeRgb(layer->channels, layer->num_channels, pixel_data, channel_data_length, pixel_step);
    case PSD_COLOR_MODE_CMYK:
        return psdLayerChannelCompositeCmyk(layer->channels, layer->num_channels, pixel_data, channel_data_length, pixel_step);
    case PSD_COLOR_MODE_BITMAP:
    case PSD_COLOR_MODE_INDEXED:
    case PSD_COLOR_MODE_HSL:
    case PSD_COLOR_MODE_HSB:
    case PSD_COLOR_MODE_MULTICHANNEL:
    case PSD_COLOR_MODE_DUOTONE:
    case PSD_COLOR_MODE_LAB:
    case PSD_COLOR_MODE_GRAY16:
    case PSD_COLOR_MODE_RGB48:
    case PSD_COLOR_MODE_LAB48:
    case PSD_COLOR_MODE_CMYK64:
    case PSD_COLOR_MODE_DEEP_MULTICHANNEL:
    case PSD_COLOR_MODE_DUOTONE16:
    case PSD_COLOR_MODE_MAX_ENUM:
    default:
        return PSD_STATUS_ERROR_UNSUPPORTED_IMAGE_EXTRACTION;
    }
}

psd_layer_t *const *
psdLayerContainerGetAllLayers(const psd_layer_container_t *container, psd_rsize_t *length, psd_layer_order_t order)
{
    list_node_t *node;
    psd_layer_t **layers = 0;
    int i = 0;
    *length = 0;
    if (psd_is_not_null(container) && psd_is_not_null(container->layers) && container->layers->len > 0) {
        layers = psd_calloc(container->layers->len, sizeof(psd_layer_t*));
        if (psd_is_not_null(layers)) {
            i = 0;
            if (order == PSD_LAYER_ORDER_DESCEND) {
                /* layer starts from tail to head */
                node = container->layers->tail;
                while (psd_is_not_null(node)) {
                    layers[i++] = (psd_layer_t *) node->val;
                    node = node->prev;
                }
            }
            else {
                /* layer starts from head to tail */
                node = container->layers->head;
                while (psd_is_not_null(node)) {
                    layers[i++] = (psd_layer_t *) node->val;
                    node = node->next;
                }
            }
            *length = i;
        }
    }
    return layers;
}

psd_float64_t
psdLayerContainerGetOpacity(const psd_layer_container_t *container)
{
    return psd_is_not_null(container) ? container->opacity : 0;
}

static void
psdImageSetChannel(const psd_image_t *image,
                   psd_layer_channel_id_t id,
                   psd_rsize_t channel_data_length,
                   psd_rsize_t channel_data_offset,
                   psd_layer_channel_t *channel)
{
    const psd_layer_channel_t *image_channel = image->channel;
    channel->layer = PSD_NULL;
    channel->id = id;
    channel->header = image_channel->header;
    channel->raw_data = image_channel->raw_data + channel_data_offset;
    channel->raw_data_length = channel_data_length;
    channel->data_ptr = PSD_NULL;
    channel->data_ptr_length = 0;
}

static void
psdImageCreateChannels(const psd_image_t *image,
                       psd_layer_channel_t **channels,
                       psd_rsize_t max_channels,
                       psd_rsize_t *num_channels)
{
    const psd_header_t *header = image->header;
    psd_layer_channel_t *channel;
    psd_rsize_t i, length, last_channel_index, offset = 0;
    if (*num_channels == max_channels) {
        last_channel_index  = *num_channels;
        last_channel_index -= 1;
        length = image->channel->raw_data_length / *num_channels;
        for (i = 0; i < last_channel_index; i++) {
            channels[i] = channel = psdLayerChannelCreate(header, 0);
            psdImageSetChannel(image, PSD_LAYER_CHANNEL_ID_RED + i, length, offset, channel);
            offset += length;
        }
        channels[i] = channel = psdLayerChannelCreate(header, 0);
        psdImageSetChannel(image, PSD_LAYER_CHANNEL_ID_ALPHA, length, offset, channel);
    }
    else {
        *num_channels = psdUtilMaxUnsigned(*num_channels, max_channels);
        length = image->channel->raw_data_length / *num_channels;
        for (i = 0; i < *num_channels; i++) {
            channels[i] = channel = psdLayerChannelCreate(header, 0);
            psdImageSetChannel(image, PSD_LAYER_CHANNEL_ID_RED + i, length, offset, channel);
            offset += length;
        }
    }
}

static void
psdImageDestroyChannels(psd_layer_channel_t **channels, psd_rsize_t num_channels)
{
    psd_rsize_t i;
    for (i = 0; i < num_channels; i++) {
        psd_free(channels[i]);
    }
}

psd_status_t
psdImageExtract(psd_image_t *image)
{
    psd_layer_channel_t *rgb_channels[PSD_LAYER_CHANNEL_ID_BLUE + 2];
    psd_layer_channel_t *cmyk_channels[PSD_LAYER_CHANNEL_ID_KEY + 2];
    psd_layer_channel_t *gray_channels[PSD_LAYER_CHANNEL_ID_GRAY + 2];
    const psd_rsize_t num_rgb_channels = sizeof(rgb_channels) / sizeof(rgb_channels[0]);
    const psd_rsize_t num_cmyk_channels = sizeof(cmyk_channels) / sizeof(cmyk_channels[0]);
    const psd_rsize_t num_gray_channels = sizeof(gray_channels) / sizeof(gray_channels[0]);
    const psd_header_t *header;
    psd_uint8_t *pixel_data;
    psd_rsize_t height, num_channels, pixel_data_length, pixel_step;
    psd_status_t rc;
    if (psd_is_null(image)) {
        return PSD_STATUS_ERROR_NULL_IMAGE;
    }
    header = image->header;
    height = psdHeaderGetSize(header).rows;
    num_channels = psdHeaderGetChannelCount(header);
    pixel_data_length = psdHeaderGetImageLength(header);
    pixel_step = psdUtilMinUnsigned(psdHeaderGetDepth(header), 8) / 8;
    switch (image->compression) {
    case PSD_IMAGE_COMPRESSION_RAW:
        rc = psdLayerChannelParseRaw(image->channel, image->buffer, pixel_data_length * num_channels);
        break;
    case PSD_IMAGE_COMPRESSION_RLE:
        rc = psdLayerChannelParseRle(image->channel, image->buffer, pixel_data_length * num_channels, height, num_channels, psdHeaderIsBigDocument(header));
        break;
    case PSD_IMAGE_COMPRESSION_ZIP:
    case PSD_IMAGE_COMPRESSION_ZIP_PREDICTION:
        rc = psdLayerChannelParseZip(image->channel, image->buffer, pixel_data_length * num_channels);
        break;
    case PSD_IMAGE_COMPRESSION_MAX_ENUM:
    default:
        rc = PSD_STATUS_ERROR_UNSUPPORTED_IMAGE_COMPRESSION;
        break;
    }
    if (psd_has_error(rc)) {
        return rc;
    }
    psdBufferDataDestroy(&image->pixel_data);
    image->pixel_data = pixel_data = psd_calloc(pixel_data_length, sizeof(*image->pixel_data) * sizeof(psd_uint32_t));
    switch (psdHeaderGetColorMode(header)) {
    case PSD_COLOR_MODE_GRAY_SCALE:
        psdImageCreateChannels(image, gray_channels, num_gray_channels, &num_channels);
        rc = psdLayerChannelCompositeGrayScale(gray_channels, num_channels, pixel_data, pixel_data_length, pixel_step);
        psdImageDestroyChannels(gray_channels, num_channels);
        break;
    case PSD_COLOR_MODE_RGB:
        psdImageCreateChannels(image, rgb_channels, num_rgb_channels, &num_channels);
        rc = psdLayerChannelCompositeRgb(rgb_channels, num_channels, pixel_data, pixel_data_length, pixel_step);
        psdImageDestroyChannels(rgb_channels, num_channels);
        break;
    case PSD_COLOR_MODE_CMYK:
        psdImageCreateChannels(image, cmyk_channels, num_cmyk_channels, &num_channels);
        rc = psdLayerChannelCompositeCmyk(cmyk_channels, num_channels, pixel_data, pixel_data_length, pixel_step);
        psdImageDestroyChannels(cmyk_channels, num_channels);
        break;
    case PSD_COLOR_MODE_BITMAP:
    case PSD_COLOR_MODE_INDEXED:
    case PSD_COLOR_MODE_HSL:
    case PSD_COLOR_MODE_HSB:
    case PSD_COLOR_MODE_MULTICHANNEL:
    case PSD_COLOR_MODE_DUOTONE:
    case PSD_COLOR_MODE_LAB:
    case PSD_COLOR_MODE_GRAY16:
    case PSD_COLOR_MODE_RGB48:
    case PSD_COLOR_MODE_LAB48:
    case PSD_COLOR_MODE_CMYK64:
    case PSD_COLOR_MODE_DEEP_MULTICHANNEL:
    case PSD_COLOR_MODE_DUOTONE16:
    case PSD_COLOR_MODE_MAX_ENUM:
    default:
        rc = PSD_STATUS_ERROR_UNSUPPORTED_IMAGE_EXTRACTION;
    }
    return rc;
}

psd_image_compression_t
psdImageGetCompressionType(const psd_image_t *image)
{
    return psd_is_not_null(image) ? image->compression : PSD_IMAGE_COMPRESSION_RAW;
}

psd_bool_t
psdImageHasMask(const psd_image_t *image)
{
    return psd_is_not_null(image) ? image->has_mask : psd_false;
}

const psd_uint8_t *
psdImageGetPixelData(const psd_image_t *image)
{
    return psd_is_not_null(image) ? image->pixel_data : PSD_NULL;
}

psd_rsize_t
psdImageGetWidth(const psd_image_t *image)
{
    return psd_is_not_null(image) ? psdUtilMinUnsigned(psdHeaderGetSize(image->header).columns, 1) : 1;
}

psd_rsize_t
psdImageGetHeight(const psd_image_t *image)
{
    return psd_is_not_null(image) ? psdUtilMinUnsigned(psdHeaderGetSize(image->header).rows, 1) : 1;
}

psd_document_t *
psdDocumentCreate(void)
{
    psd_header_t *header;
    psd_document_t *document = psd_calloc(1, sizeof(*document));
    if (psd_is_not_null(document)) {
        document->header = header = psdHeaderCreate();
        document->resources = psdResourceContainerCreate(header);
        document->layers = psdLayerContainerCreate(header);
        document->image = psdImageCreate(header);
    }
    return document;
}

psd_status_t
psdDocumentParse(psd_document_t *document, psd_buffer_t *buffer)
{
    psd_status_t result;
    psd_bool_t ok;
    if (psd_is_null(buffer)) {
        return PSD_STATUS_ERROR_NULL_BUFFER;
    }
    if (psd_is_null(document)) {
        return PSD_STATUS_ERROR_NULL_DOCUMENT;
    }
    psdBufferSeek(buffer, 0, &ok);
    result = psdHeaderParse(document->header, buffer);
    if (psd_has_error(result)) {
        return result;
    }
    result = psdResourceContainerParse(document->resources, buffer);
    if (psd_has_error(result)) {
        return result;
    }
    result = psdLayerContainerParse(document->layers, buffer);
    if (psd_has_error(result)) {
        return result;
    }
    result = psdImageParse(document->image, buffer);
    if (psd_has_error(result)) {
        return result;
    }
    return PSD_STATUS_SUCCESS;
}

const psd_header_t *
psdDocumentGetHeader(const psd_document_t *document)
{
    return psd_is_not_null(document) ? document->header : PSD_NULL;
}

const psd_resource_container_t *
psdDocumentGetResourceContainer(const psd_document_t *document)
{
    return psd_is_not_null(document) ? document->resources : PSD_NULL;
}

const psd_image_t *
psdDocumentGetImage(const psd_document_t *document)
{
    return psd_is_not_null(document) ? document->image : PSD_NULL;
}

const psd_layer_container_t *
psdDocumentGetLayerContainer(const psd_document_t *document)
{
    return psd_is_not_null(document) ?  document->layers : PSD_NULL;
}

psd_rsize_t
psdDocumentGetWidth(const psd_document_t *document)
{
    return psd_is_not_null(document) ? psdImageGetWidth(document->image) : 1;
}

psd_rsize_t
psdDocumentGetHeight(const psd_document_t *document)
{
    return psd_is_not_null(document) ? psdImageGetHeight(document->image) : 1;
}

psd_resource_t *const *
psdDocumentGetAllResources(const psd_document_t *document, psd_rsize_t *length)
{
    if (psd_is_not_null(document)) {
        return psdResourceContainerGetAllResources(document->resources, length);
    }
    else if (psd_is_not_null(length)) {
        *length = 0;
    }
    return 0;
}

psd_layer_t *const *
psdDocumentGetAllLayers(const psd_document_t *document, psd_rsize_t *length)
{
    if (psd_is_not_null(document)) {
        return psdLayerContainerGetAllLayers(document->layers, length, PSD_LAYER_ORDER_DESCEND);
    }
    else if (psd_is_not_null(length)) {
        *length = 0;
    }
    return 0;
}

psd_status_t
psdDocumentExtractImage(psd_document_t *document)
{
    if (psd_is_null(document)) {
        return PSD_STATUS_ERROR_NULL_DOCUMENT;
    }
    return psdImageExtract(document->image);
}

void
psdDocumentDestroy(psd_document_t *document)
{
    if (psd_is_not_null(document)) {
        if (psd_is_not_null(document->header)) {
            psdHeaderDestroy(document->header);
            document->header = PSD_NULL;
        }
        if (psd_is_not_null(document->resources)) {
            psdResourceContainerDestroy(document->resources);
            document->resources = PSD_NULL;
        }
        if (psd_is_not_null(document->layers)) {
            psdLayerContainerDestroy(document->layers);
            document->layers = PSD_NULL;
        }
        if (psd_is_not_null(document->image)) {
            psdImageDestroy(document->image);
            document->image = PSD_NULL;
        }
        psd_free(document);
    }
}

psd_bool_t
psdLayerIsText(const psd_layer_t *layer)
{
	const psd_layer_info_t *layer_info;
	layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_TYPE_TOOL);
	if (psd_is_not_null(layer_info)) {
		return psd_true;
	}
	return psd_false;
}

const char *
psdLayerTextGetText(const psd_layer_t *layer)
{
	const psd_layer_info_t *layer_info;
	psd_rsize_t length;
	psd_descriptor_value_t** values = NULL;
	char* ret = NULL;

	layer_info = psdLayerInfoFind(layer, PSD_LAYER_INFO_TYPE_TOOL);
	if (psd_is_not_null(layer_info)) {
		values = psdDescriptorGetValues(layer_info->u.type_tool, &length);

		for (int i = 0; i < length; i++)
		{
			psd_descriptor_value_t* value = (values[i]);
			
			if (strcmp(value->id, "Txt ") == 0)
			{
				ret = psdDescriptorValueGetUnicodeText(value);
			}
		}

		psd_free(values);
		return ret;
	}
	return NULL;
}
