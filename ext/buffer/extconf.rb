require 'mkmf'
extension_name = 'buffer_ext'
dir_config(extension_name)

create_header
create_makefile(extension_name)
