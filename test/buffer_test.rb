require "test_helper"

class BufferTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::Buffer::VERSION
  end

  def test_that_it_has_a_buffer_with_proper_size
    buf = Buffer.new 1024
    assert_equal 1024, buf.size
  end

  def test_that_buffer_could_resize
    buf = Buffer.new 1024
    assert_equal 1024, buf.size
    buf.resize 2048
    assert_equal 2048, buf.size
    buf.resize 1024
    assert_equal 1024, buf.size
  end

  def test_that_it_could_clear_itself
    buf = Buffer.new 1024
    buf.clear
    assert_equal [0] * 1024, buf.bytes
    assert_equal [0] * 1024, buf.to_a
  end

  def test_that_it_could_memset
    buf = Buffer.new 1024
    buf.clear
    buf.memset(1, 24)
    assert_equal [1] * 24 + [0] * 1000, buf.bytes
  end

  def test_that_it_could_be_cloned
    buf = Buffer.new 1024
    buf.clear
    
    buf2 = buf.clone
    buf2.memset(1, 24)

    refute_equal buf, buf2
    assert_equal [0] * 1024, buf.bytes
    assert_equal [1] * 24 + [0] * 1000, buf2.bytes
  end

  def test_that_it_could_be_dupped
    buf = Buffer.new 1024
    buf.clear
    
    buf2 = buf.dup
    buf2.memset(1, 24)

    refute_equal buf, buf2
    assert_equal [0] * 1024, buf.bytes
    assert_equal [1] * 24 + [0] * 1000, buf2.bytes
  end

  def test_that_buffer_created_from_string
    buf = Buffer.from 'Hello World'
    assert_equal 12, buf.size
    assert_equal 'Hello World', buf.to_s
  end

  def test_that_buffer_created_from_utf8_string
    buf = Buffer.from 'おはよう世界！'
    assert_equal 22, buf.size
    assert_equal 'おはよう世界！', buf.to_s.force_encoding('UTF-8')
  end

  def test_that_not_nul_terminated_buffer_to_s
    buf = Buffer.from [1, 1, 1, 1]
    assert_equal 4, buf.size
    assert_equal buf.to_s, "\x01\x01\x01\x01"
  end

  def test_that_buffer_created_from_array
    buf = Buffer.from [1, 2, 3, 254]
    assert_equal 4, buf.size
    assert_equal [1, 2, 3, 254], buf.bytes
  end

  def test_that_buffer_is_exposed
    buf = Buffer.new 1024
    assert_equal Buffer::BufferPayload, buf.data.class
  end

  def test_that_buffer_could_be_compared
    buf = Buffer.from [1, 1, 1, 0]
    buf2 = Buffer.from [1, 1, 1, 0, 0]
    refute_equal buf, buf2
    assert_equal 0, buf.memcmp(buf2)
    assert_equal 0, buf <=> buf2
    assert_equal 0, buf.memcmp(buf2, 4)
  end

  def test_that_buffer_comparison_checks_bounds
    buf = Buffer.from [1, 1, 1, 0]
    buf2 = Buffer.from [1, 1, 1, 0, 0]
    assert_raises RangeError do
      buf.memcmp(buf2, 5)
    end
  end
end
