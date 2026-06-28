sources = files(
    'src/lexer/lexer.cpp',
    'src/parser/parser.cpp',
    'src/parser/parser.cpp',
    'src/compiler/compiler.cpp',

    'src/compiler/instruction_set/utils.cpp',
    'src/compiler/instruction_set/add.cpp',
    'src/compiler/instruction_set/mul.cpp',
    'src/compiler/instruction_set/sub.cpp',
    'src/compiler/instruction_set/copy.cpp',
    'src/compiler/instruction_set/out.cpp',
    'src/compiler/instruction_set/in.cpp',

    'src/main.cpp',
)

if host_machine.system() == 'windows'

elif host_machine.system() == 'linux'
    sources += 'src/platform/linux/executable.cpp'
endif


kit_compiler_includes = include_directories('src')

executable(
    'kit-compiler',
    sources: sources,
    include_directories: [kit_compiler_includes],
)