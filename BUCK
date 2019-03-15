include_defs('//BUCKAROO_DEPS')

# Configuration

mode = read_config('build', 'mode', 'release')

if mode not in ['release', 'debug']:
    print "Build mode not recognized, using the default release mode."
    mode = 'release'

# Drop sources & headers

drop = {
    'sources': glob([
      'drop/src/**/*.cpp',
    ]),
    'headers': subdir_glob([
      ('drop/src', '**/*.h'),
      ('drop/src', '**/*.hpp'),
    ]),
    'flags': {
        'common': {},
        'debug': {},
        'release': {}
    }
}

# Drop flags

drop['flags']['common'] = {
    'compiler': ['-std=c++2a', '-stdlib=libc++', '-fcoroutines-ts'],
    'linker': ['-stdlib=libc++', '-lc++abi', '-pthread']
}

drop['flags']['debug'] = {
    'compiler': ['-O0', '-g'],
    'linker': []
}

drop['flags']['release'] = {
    'compiler': ['-O3'],
    'linker': []
}

# Test sources & headers

test = {
    'sources': glob([
      'drop/test/**/*.cpp',
    ]),
    'headers': subdir_glob([
      ('drop/test', '**/*.h'),
      ('drop/test', '**/*.hpp'),
    ])
}

# Build targets

cxx_library(
  name = 'drop',
  header_namespace = 'drop',
  srcs = drop['sources'],
  exported_headers = drop['headers'],
  compiler_flags = drop['flags']['common']['compiler'] + drop['flags'][mode]['compiler'],
  linker_flags = drop['flags']['common']['linker'] + drop['flags'][mode]['linker'],
  deps = BUCKAROO_DEPS,
  visibility = ['PUBLIC']
)

cxx_binary(
  name = 'test',
  srcs = test['sources'],
  headers =  test['headers'],
  compiler_flags = drop['flags']['common']['compiler'] + drop['flags'][mode]['compiler'],
  linker_flags = drop['flags']['common']['linker'] + drop['flags'][mode]['linker'],
  deps = [":drop"],
  visibility = []
)
