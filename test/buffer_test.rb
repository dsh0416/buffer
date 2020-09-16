require "test_helper"

class BufferTest < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::Buffer::VERSION
  end

  def test_that_it_has_a_buffer_with_proper_size
    buf = Buffer.new(1024)
    assert_equal 1024, buf.size
  end

  def test_that_it_could_clear_itself
    buf = Buffer.new(1024)
    buf.clear
    assert_equal [0] * 1024, buf.bytes
  end

  def test_that_buffer_created_from_string
    buf = Buffer.from('Hello World')
    assert_equal 12, buf.size
    assert_equal 'Hello World', buf.to_s
  end

  def test_that_buffer_created_from_utf8_string
    buf = Buffer.from('おはよう世界！')
    assert_equal 22, buf.size
    assert_equal 'おはよう世界！', buf.to_s.force_encoding('UTF-8')
  end

  def test_that_buffer_is_exposed
    buf = Buffer.new(1024)
    assert_equal Buffer::BufferPayload, buf.payload.class
  end
end
