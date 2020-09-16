#include "buffer.h"

void Init_buffer_ext() {
  kBuffer = rb_define_class("Buffer", rb_cObject);
  kBufferPayload = rb_define_class_under(kBuffer, "BufferPayload", rb_cObject);
  rb_define_singleton_method(kBuffer, "from", method_buffer_from, 1);
  rb_define_method(kBuffer, "initialize", method_buffer_init, 1);
  rb_define_method(kBuffer, "clear", method_buffer_clear, 0);
  rb_define_method(kBuffer, "resize", method_buffer_resize, 1);
  rb_define_method(kBuffer, "to_s", method_buffer_to_s, 0);
  rb_define_method(kBuffer, "bytes", method_buffer_bytes, 0);
  rb_define_method(kBuffer, "size", method_buffer_buffer_size, 0);
}

VALUE method_buffer_from(VALUE klass, VALUE val) {
  if (TYPE(val) == T_STRING) {
    char* ptr = StringValueCStr(val);
    VALUE obj = rb_funcall(kBuffer, rb_intern("new"), 1, SIZET2NUM(strlen(ptr) + 1));
    VALUE payload = rb_iv_get(obj, "@payload");
    char* buf_ptr = internal_buffer_payload_get(payload)->payload;
    memcpy(buf_ptr, ptr, strlen(ptr) + 1);
    return obj;
  }

  if (TYPE(val) == T_ARRAY) {
    long len = RARRAY_LEN(val);
    VALUE obj = rb_funcall(kBuffer, rb_intern("new"), 1, LONG2NUM(len));
    VALUE payload = rb_iv_get(obj, "@payload");
    char* buf_ptr = internal_buffer_payload_get(payload)->payload;
    for (long i = 0; i < len; i++) {
      VALUE num = rb_ary_entry(val, i);
      buf_ptr[i] = NUM2CHR(num);
    }
    return obj;
  }

  return Qnil;
}

VALUE method_buffer_init(VALUE self, VALUE size) {
  VALUE payload = internal_buffer_payload_malloc(NUM2SIZET(size));
  rb_iv_set(self, "@payload", payload);
  return self;
}

VALUE method_buffer_clear(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");

  struct buffer_payload* buffer = internal_buffer_payload_get(payload);

  memset(buffer->payload, 0, buffer->buffer_size);
  return self;
}

VALUE method_buffer_resize(VALUE self, VALUE size) {
  VALUE payload = rb_iv_get(self, "@payload");
  internal_buffer_payload_realloc(payload, NUM2SIZET(size));
  return self;
}

VALUE method_buffer_to_s(VALUE self) {
  // FIXME: What if a buffer doesn't have a NUL terminator?
  VALUE payload = rb_iv_get(self, "@payload");

  return rb_str_buf_new2(internal_buffer_payload_get(payload)->payload);
}

VALUE method_buffer_bytes(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");
  struct buffer_payload* buffer = internal_buffer_payload_get(payload);
  VALUE result = rb_ary_new2(buffer->buffer_size);
  for (size_t i = 0; i < buffer->buffer_size; i++) {
    rb_ary_store(result, i, CHR2FIX(buffer->payload[i])); // # TODO: Why there is no CHR2NUM?
  }
  return result;
}

VALUE method_buffer_buffer_size(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");

  return SIZET2NUM(internal_buffer_payload_get(payload)->buffer_size);
}

// internal methods
VALUE internal_buffer_payload_malloc(size_t size) {
  struct buffer_payload* payload = xmalloc(sizeof(struct buffer_payload));
  payload->buffer_size = size;
  payload->payload = xmalloc(sizeof(char) * size);
  return TypedData_Wrap_Struct(kBufferPayload, &type_buffer_payload, payload);
}

VALUE internal_buffer_payload_realloc(VALUE wrapped, size_t size) {
  struct buffer_payload* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_payload, &type_buffer_payload, ptr);
  ptr->buffer_size = size;
  ptr->payload = xrealloc(ptr->payload, sizeof(char) * size);
  return wrapped;
}

struct buffer_payload* internal_buffer_payload_get(VALUE wrapped) {
  struct buffer_payload* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_payload, &type_buffer_payload, ptr);
  return ptr;
}

void internal_buffer_payload_free(void* ptr) {
  struct buffer_payload* t = (struct buffer_payload*) ptr;
  xfree(t->payload);
  xfree(t);
}

size_t internal_buffer_payload_size(const void* ptr) {
  struct buffer_payload* t = (struct buffer_payload*) ptr;
  return sizeof(struct buffer_payload) + t->buffer_size;
}
