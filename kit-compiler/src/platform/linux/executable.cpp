#include <vector>
#include <elf.h>
#include <fstream>

#include "common.hpp"


namespace kit::platform
{
    void write_executable(std::ofstream& file, std::vector<u8> code)
    {
        static_assert(sizeof(Elf64_Ehdr) == 64);
        static_assert(sizeof(Elf64_Phdr) == 56);

        code.insert(code.end(),
        {
            0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 60 (sys_exit)
            0x48, 0x31, 0xff,                         // xor rdi, rdi
            0x0f, 0x05                                // syscall
        });

        Elf64_Ehdr ehdr = 
        {
            .e_ident = { 0x7F, 'E', 'L', 'F', 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            .e_type = ET_EXEC, .e_machine = EM_X86_64, .e_version = 1,
            .e_entry = 0x400000 + sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr),
            .e_phoff = sizeof(Elf64_Ehdr),
            .e_ehsize = sizeof(Elf64_Ehdr),
            .e_phentsize = sizeof(Elf64_Phdr), .e_phnum = 1
        };

        Elf64_Phdr phdr =
        {
            .p_type = PT_LOAD, .p_flags = PF_R | PF_X,
            .p_vaddr = 0x400000, .p_paddr = 0x400000,
            .p_filesz = sizeof(ehdr) + sizeof(phdr) + sizeof(code),
            .p_memsz = sizeof(ehdr) + sizeof(phdr) + sizeof(code),
            .p_align = 0x1000
        };

        file.write(reinterpret_cast<const char*>(&ehdr), sizeof(ehdr));
        file.write(reinterpret_cast<const char*>(&phdr), sizeof(phdr));
        file.write(reinterpret_cast<const char*>(code.data()), code.size());
    }
}