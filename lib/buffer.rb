require 'buffer_ext'

class Buffer
  attr_reader :data

  alias_method :to_a, :bytes

  def memcmp(other, n=[self.size, other.size].min)
    internal_memcmp(other, n)
  end

  def <=>(other)
    memcmp(other)
  end
end
