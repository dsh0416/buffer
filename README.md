# buffer
The gem to operate low level I/O data buffer

[![Build Status](https://travis-ci.org/dsh0416/buffer.svg?branch=master)](https://travis-ci.org/dsh0416/buffer)

## Example

```
gem install 'data-buffer'
```

```ruby
require 'buffer'

buf = Buffer.new(1024) # Create char* buffer with size of 1024
buf.clear # memset 0 to buffer size
buf.size # return buffer size
buf.to_s # convert to string. WARNING: make sure the string has NUL terminator
buf.bytes # convert to char byte array
buf.memset(1, 4) # set first 4 bytes to 1

buf.resize(2048) # realloc the buffer to 2048

buf = Buffer.from('Hello World') # Create char* from existing string
buf = Buffer.from([1, 2, 3, 244]) # Create char* from existing char array

buf.data # Get raw data struct

# struct buffer_data {
#   size_t buffer_size;
#   char* buffer;
# };
#
# buffer_data* data = (buffer_data*) ((struct RData *)obj)->data;

# Both `clone` and `dup` could copy the buffer
buf2 = buf.clone
buf3 = buf.dup
```
