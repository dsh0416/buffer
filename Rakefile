
require "bundler/gem_tasks"
require "rake/testtask"
require 'rake/extensiontask'

spec = Gem::Specification.load('buffer.gemspec')
Rake::ExtensionTask.new('buffer_ext', spec) do |ext|
  ext.ext_dir = "ext/buffer"
end

Rake::TestTask.new(:test) do |t|
  t.libs << "test"
  t.libs << "lib"
  t.test_files = FileList["test/**/*_test.rb"]
end

task :default => :test
