#include <ruby.h>

VALUE kBuffer = Qnil;
VALUE kBufferPayload = Qnil;

void Init_buffer_ext();
VALUE method_buffer_from(VALUE klass, VALUE val);
VALUE method_buffer_init(VALUE self, VALUE size);
VALUE method_buffer_init_copy(VALUE self, VALUE orig);
VALUE method_buffer_memset(VALUE self, VALUE ch, VALUE n);
VALUE method_buffer_clone(VALUE self, VALUE size);
VALUE method_buffer_clear(VALUE self);
VALUE method_buffer_resize(VALUE self, VALUE size);
VALUE method_buffer_to_s(VALUE self);
VALUE method_buffer_bytes(VALUE self);
VALUE method_buffer_buffer_size(VALUE self);

// internal methods
VALUE internal_buffer_data_malloc(size_t size);
VALUE internal_buffer_data_realloc(VALUE wrapped, size_t size);
struct buffer_data* internal_buffer_data_get(VALUE wrapped);
void internal_buffer_data_free(void* ptr);
size_t internal_buffer_data_size(const void* ptr);

struct buffer_data {
  size_t buffer_size;
  char* buffer;
};

static const rb_data_type_t type_buffer_data = {
  .wrap_struct_name = "buffer_data",
  .function = {
    .dmark = NULL,
    .dfree = internal_buffer_data_free,
    .dsize = internal_buffer_data_size,
  },
  .data = NULL,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};
