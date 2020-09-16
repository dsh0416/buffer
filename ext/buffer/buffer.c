#include "buffer.h"

void Init_buffer_ext() {
  Buffer = rb_define_class("Buffer", rb_cObject);
  BufferPayload = rb_define_class_under(Buffer, "BufferPayload", rb_cObject);
  rb_define_singleton_method(Buffer, "from", buffer_from, 1);
  rb_define_method(Buffer, "initialize", buffer_init, 1);
  rb_define_method(Buffer, "clear", buffer_clear, 0);
  rb_define_method(Buffer, "resize", buffer_resize, 1);
  rb_define_method(Buffer, "to_s", buffer_to_s, 0);
  rb_define_method(Buffer, "bytes", buffer_bytes, 0);
  rb_define_method(Buffer, "size", buffer_buffer_size, 0);
}

VALUE buffer_from(VALUE klass, VALUE val) {
  char* ptr = StringValueCStr(val);
  VALUE obj = rb_funcall(Buffer, rb_intern("new"), 1, SIZET2NUM(strlen(ptr) + 1));
  VALUE payload = rb_iv_get(obj, "@payload");
  char* buf_ptr = buffer_payload_get(payload)->payload;
  memcpy(buf_ptr, ptr, strlen(ptr) + 1);
  return obj;
}

VALUE buffer_init(VALUE self, VALUE size) {
  VALUE payload = buffer_payload_malloc(NUM2SIZET(size));
  rb_iv_set(self, "@payload", payload);
  return self;
}

VALUE buffer_clear(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");

  struct buffer_payload* buffer = buffer_payload_get(payload);

  memset(buffer->payload, 0, buffer->buffer_size);
  return self;
}

VALUE buffer_resize(VALUE self, VALUE size) {
  VALUE payload = rb_iv_get(self, "@payload");
  buffer_payload_realloc(payload, NUM2SIZET(size));
  return self;
}

VALUE buffer_to_s(VALUE self) {
  // FIXME: What if a buffer doesn't have a NUL terminator?
  VALUE payload = rb_iv_get(self, "@payload");

  return rb_str_buf_new2(buffer_payload_get(payload)->payload);
}

VALUE buffer_bytes(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");
  struct buffer_payload* buffer = buffer_payload_get(payload);
  VALUE result = rb_ary_new2(buffer->buffer_size);
  for (size_t i = 0; i < buffer->buffer_size; i++) {
    rb_ary_store(result, i, UINT2NUM(buffer->payload[i]));
  }
  return result;
}

VALUE buffer_buffer_size(VALUE self) {
  VALUE payload = rb_iv_get(self, "@payload");

  return SIZET2NUM(buffer_payload_get(payload)->buffer_size);
}

// internal methods
VALUE buffer_payload_malloc(size_t size) {
  struct buffer_payload* payload = xmalloc(sizeof(struct buffer_payload));
  payload->buffer_size = size;
  payload->payload = xmalloc(sizeof(char) * size);
  return TypedData_Wrap_Struct(BufferPayload, &type_buffer_payload, payload);
}

VALUE buffer_payload_realloc(VALUE wrapped, size_t size) {
  struct buffer_payload* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_payload, &type_buffer_payload, ptr);
  ptr->buffer_size = size;
  xrealloc(ptr->payload, size);
  return wrapped;
}

struct buffer_payload* buffer_payload_get(VALUE wrapped) {
  struct buffer_payload* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_payload, &type_buffer_payload, ptr);
  return ptr;
}

void buffer_payload_free(void* ptr) {
  struct buffer_payload* t = (struct buffer_payload*) ptr;
  xfree(t->payload);
  xfree(t);
}

size_t buffer_payload_size(const void* ptr) {
  struct buffer_payload* t = (struct buffer_payload*) ptr;
  return sizeof(struct buffer_payload) + t->buffer_size;
}
