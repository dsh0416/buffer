#include <ruby.h>

VALUE Buffer = Qnil;
VALUE BufferPayload = Qnil;

void Init_buffer_ext();
VALUE buffer_from(VALUE klass, VALUE val);
VALUE buffer_init(VALUE self, VALUE size);
VALUE buffer_clear(VALUE self);
VALUE buffer_resize(VALUE self, VALUE size);
VALUE buffer_to_s(VALUE self);
VALUE buffer_bytes(VALUE self);
VALUE buffer_buffer_size(VALUE self);

// internal methods
VALUE buffer_payload_malloc(size_t size);
VALUE buffer_payload_realloc(VALUE wrapped, size_t size);
struct buffer_payload* buffer_payload_get(VALUE wrapped);
void buffer_payload_free(void* ptr);
size_t buffer_payload_size(const void* ptr);

struct buffer_payload {
  size_t buffer_size;
  char* payload;
};

static const rb_data_type_t type_buffer_payload = {
  .wrap_struct_name = "buffer_payload",
  .function = {
    .dmark = NULL,
    .dfree = buffer_payload_free,
    .dsize = buffer_payload_size,
  },
  .data = NULL,
  .flags = RUBY_TYPED_FREE_IMMEDIATELY,
};
