sources = files(
    'src/lexer/lexer.cpp',
    'src/parser/parser.cpp',
    'src/parser/parser.cpp',
    'src/assembler/assembler.cpp',

    'src/assembler/instruction_set/utils.cpp',
    'src/assembler/instruction_set/add.cpp',
    'src/assembler/instruction_set/mul.cpp',
    'src/assembler/instruction_set/sub.cpp',
    'src/assembler/instruction_set/copy.cpp',
    'src/assembler/instruction_set/out.cpp',
    'src/assembler/instruction_set/in.cpp',
    'src/assembler/instruction_set/wb.cpp',
    'src/assembler/instruction_set/cmp.cpp',

    'src/main.cpp',
)

if host_machine.system() == 'windows'

elif host_machine.system() == 'linux'
    sources += 'src/platform/linux/executable.cpp'
endif


kit_assembler_includes = include_directories('src')

executable(
    'kit-assembler',
    sources: sources,
    include_directories: [kit_assembler_includes],
)