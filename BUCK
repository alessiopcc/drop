include_defs('//BUCKAROO_DEPS')

cxx_library(
  name = 'drop',
  header_namespace = 'drop',
  srcs = glob([
    'drop/src/**/*.cpp',
  ]),
  exported_headers = subdir_glob([
    ('drop/src', '**/*.h'),
    ('drop/src', '**/*.hpp'),
  ]),
  deps = BUCKAROO_DEPS,
  visibility = ['PUBLIC']
)

cxx_binary(
  name = 'app',
  srcs = glob([
    'drop/app/**/*.cpp',
  ]),
  headers = subdir_glob([
    ('drop/app', '**/*.h'),
    ('drop/app', '**/*.hpp'),
  ]),
  deps = [':drop'],
  visibility = []
)

cxx_binary(
  name = 'test',
  srcs = glob([
    'drop/test/**/*.cpp',
  ]),
  headers = subdir_glob([
    ('drop/test', '**/*.h'),
    ('drop/test', '**/*.hpp'),
  ]),
  deps = [':drop'],
  visibility = []
)
