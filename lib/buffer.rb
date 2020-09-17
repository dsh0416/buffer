require 'buffer_ext'

class Buffer
  attr_reader :data

  alias_method :to_a, :bytes
end
