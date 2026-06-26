sources = files(
    # move to platform-dependant: 
    'src/platform/linux/executable.cpp',
    
    'src/lexer/lexer.cpp',
    'src/parser/parser.cpp',
    'src/parser/parser.cpp',

    'src/compiler/instruction_set/add.cpp',
    'src/compiler/instruction_set/copy.cpp',
    'src/compiler/instruction_set/utils.cpp',

    'src/main.cpp',
)

kit_compiler_includes = include_directories('src')

executable(
    'kit-compiler',
    sources: sources,
    include_directories: [kit_compiler_includes],
)