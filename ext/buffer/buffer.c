#include "buffer.h"

void Init_buffer_ext() {
  kBuffer = rb_define_class("Buffer", rb_cObject);
  kBufferPayload = rb_define_class_under(kBuffer, "BufferPayload", rb_cObject);
  rb_define_singleton_method(kBuffer, "from", method_buffer_from, 1);
  rb_define_method(kBuffer, "initialize", method_buffer_init, 1);
  rb_define_method(kBuffer, "initialize_copy", method_buffer_init_copy, 1);
  rb_define_method(kBuffer, "memset", method_buffer_memset, 2);
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
    VALUE data = rb_iv_get(obj, "@data");
    char* buf_ptr = internal_buffer_data_get(data)->buffer;
    memcpy(buf_ptr, ptr, strlen(ptr) + 1);
    return obj;
  }

  if (TYPE(val) == T_ARRAY) {
    long len = RARRAY_LEN(val);
    VALUE obj = rb_funcall(kBuffer, rb_intern("new"), 1, LONG2NUM(len));
    VALUE data = rb_iv_get(obj, "@data");
    char* buf_ptr = internal_buffer_data_get(data)->buffer;
    for (long i = 0; i < len; i++) {
      VALUE num = rb_ary_entry(val, i);
      buf_ptr[i] = NUM2CHR(num);
    }
    return obj;
  }

  return Qnil;
}

VALUE method_buffer_init(VALUE self, VALUE size) {
  VALUE data = internal_buffer_data_malloc(NUM2SIZET(size));
  rb_iv_set(self, "@data", data);
  return self;
}

VALUE method_buffer_init_copy(VALUE self, VALUE orig) {
  VALUE payload = rb_iv_get(orig, "@data");
  struct buffer_data* data = internal_buffer_data_get(payload);

  VALUE new_obj = internal_buffer_data_malloc(data->buffer_size);
  struct buffer_data* new_data = internal_buffer_data_get(new_obj);

  memcpy(new_data->buffer, data->buffer, data->buffer_size);
  rb_iv_set(self, "@data", new_obj);
  return self;
}

VALUE method_buffer_memset(VALUE self, VALUE ch, VALUE n) {
  VALUE payload = rb_iv_get(self, "@data");
  struct buffer_data* data = internal_buffer_data_get(payload);
  memset(data->buffer, NUM2CHR(ch), NUM2SIZET(n));
  return self;
}

VALUE method_buffer_clear(VALUE self) {
  VALUE payload = rb_iv_get(self, "@data");
  struct buffer_data* data = internal_buffer_data_get(payload);
  return method_buffer_memset(self, CHR2FIX(0), SIZET2NUM(data->buffer_size));
}

VALUE method_buffer_resize(VALUE self, VALUE size) {
  VALUE data = rb_iv_get(self, "@data");
  internal_buffer_data_realloc(data, NUM2SIZET(size));
  return self;
}

VALUE method_buffer_to_s(VALUE self) {
  VALUE payload = rb_iv_get(self, "@data");
  struct buffer_data* data = internal_buffer_data_get(payload);

  for (size_t i = 0; i < data->buffer_size; i++) {
    if (data->buffer[i] == '\0') {
      // zero terminated
      return rb_str_buf_new2(data->buffer);
    }
  }

  // Not zero terminated
  char* tmp = xmalloc(sizeof(char) * (data->buffer_size + 1));
  memcpy(tmp, data->buffer, data->buffer_size);
  tmp[data->buffer_size] = '\0';
  VALUE ret = rb_str_buf_new2(tmp);
  xfree(tmp);
  return ret;
}

VALUE method_buffer_bytes(VALUE self) {
  VALUE data = rb_iv_get(self, "@data");
  struct buffer_data* buffer = internal_buffer_data_get(data);
  VALUE result = rb_ary_new2(buffer->buffer_size);
  for (size_t i = 0; i < buffer->buffer_size; i++) {
    rb_ary_store(result, i, CHR2FIX(buffer->buffer[i])); // # TODO: Why there is no CHR2NUM?
  }
  return result;
}

VALUE method_buffer_buffer_size(VALUE self) {
  VALUE data = rb_iv_get(self, "@data");

  return SIZET2NUM(internal_buffer_data_get(data)->buffer_size);
}

// internal methods
VALUE internal_buffer_data_malloc(size_t size) {
  struct buffer_data* data = xmalloc(sizeof(struct buffer_data));
  data->buffer_size = size;
  data->buffer = xmalloc(sizeof(char) * size);
  return TypedData_Wrap_Struct(kBufferPayload, &type_buffer_data, data);
}

VALUE internal_buffer_data_realloc(VALUE wrapped, size_t size) {
  struct buffer_data* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_data, &type_buffer_data, ptr);
  ptr->buffer_size = size;
  ptr->buffer = xrealloc(ptr->buffer, sizeof(char) * size);
  return wrapped;
}

struct buffer_data* internal_buffer_data_get(VALUE wrapped) {
  struct buffer_data* ptr;
  TypedData_Get_Struct(wrapped, struct buffer_data, &type_buffer_data, ptr);
  return ptr;
}

void internal_buffer_data_free(void* ptr) {
  struct buffer_data* t = (struct buffer_data*) ptr;
  xfree(t->buffer);
  xfree(t);
}

size_t internal_buffer_data_size(const void* ptr) {
  struct buffer_data* t = (struct buffer_data*) ptr;
  return sizeof(struct buffer_data) + t->buffer_size;
}
