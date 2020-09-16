
require_relative 'lib/buffer/version'

Gem::Specification.new do |spec|
  spec.name          = "data-buffer"
  spec.version       = Buffer::VERSION
  spec.authors       = ["Delton Ding"]
  spec.email         = ["dsh0416@gmail.com"]

  spec.summary       = "The gem to operate low level I/O data buffer."
  spec.description   = "The gem to operate low level I/O data buffer."
  spec.homepage      = "https://github.com/dsh0416/buffer"
  spec.license       = 'BSD-3-Clause'
  spec.required_ruby_version = '>= 2.7.1'

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = "https://github.com/dsh0416/buffer"

  # Specify which files should be added to the gem when it is released.
  # The `git ls-files -z` loads the files in the RubyGem that have been added into git.
  spec.files         = Dir.chdir(File.expand_path('..', __FILE__)) do
    `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  end
  spec.require_paths = ["lib"]
  spec.extensions = ['ext/buffer/extconf.rb']

  spec.add_development_dependency 'rake-compiler', '~> 1.0'
end