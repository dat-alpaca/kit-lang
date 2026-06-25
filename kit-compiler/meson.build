sources = files(
    'src/lexer/lexer.cpp',
    'src/parser/parser.cpp',
    'src/main.cpp',

    'src/platform/linux/executable.cpp'
)

kit_compiler_includes = include_directories('src')

executable(
    'kit-compiler',
    sources: sources,
    include_directories: [kit_compiler_includes],
)