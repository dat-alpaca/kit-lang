#include <numeric>
#include <vector>
#include <elf.h>
#include <fstream>

#include "common.hpp"
#include "compiler/segment.hpp"
#include "platform/platform.hpp"
#include "utils/memory_utils.hpp"

namespace
{
    using namespace kit;

    static inline Elf64_Ehdr create_elf_header(u64 entryPoint, u16 programHeaderCount)
    {
        return
        {
            .e_ident = { 0x7F, 'E', 'L', 'F', 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            .e_type = ET_EXEC, 
            .e_machine = EM_X86_64, 
            .e_version = 1,
            .e_entry = entryPoint,
            
            .e_phoff = sizeof(Elf64_Ehdr),          // Program Header offset
            .e_shoff = 0,                           // Section Header table offset 

            .e_ehsize = sizeof(Elf64_Ehdr),         // ELF Header size
            .e_phentsize = sizeof(Elf64_Phdr),      // Program Header table size
            
            .e_phnum = programHeaderCount,
            
            .e_shentsize = 0,
            .e_shnum = 0,
            .e_shstrndx = 0,
        };
    }

    static inline Elf64_Phdr create_program_header(const segment& segment, u64 offset, u64 virtualAddress, u64 alignment)
    {
        u32 flags = 0;
        if ((segment.attributes & segment_attribute::read) != segment_attribute::none)
            flags |= PF_R;

        if ((segment.attributes & segment_attribute::write) != segment_attribute::none)
            flags |= PF_W;

        if ((segment.attributes & segment_attribute::exec) != segment_attribute::none)
            flags |= PF_X;

        return
        {
            .p_type = PT_LOAD,
            .p_flags = flags,
            .p_offset = offset,

            .p_vaddr = virtualAddress, 
            .p_paddr = virtualAddress,
            
            .p_filesz = segment.code.size(),
            .p_memsz = segment.code.size(),
            .p_align = alignment
        };
    }
}

namespace kit::platform
{
    inline void write_exit_code(std::vector<u8>& code)
    {
        code.insert(code.end(),
        {
            0x48, 0xc7, 0xc0, 0x3c, 0x00, 0x00, 0x00, // mov rax, 60 (sys_exit)
            0x48, 0x31, 0xff,                         // xor rdi, rdi
            0x0f, 0x05                                // syscall
        });
    }
}

namespace kit::platform
{
    void write_executable(std::ofstream& file, std::vector<segment>& segments)
    {
        auto& textSegment = segments[TextSegment];   
        write_exit_code(textSegment.code);

        static_assert(sizeof(Elf64_Ehdr) == 64);
        static_assert(sizeof(Elf64_Phdr) == 56);

        [[maybe_unused]]
        u64 codeSize = std::accumulate(segments.begin(), segments.end(), 0, [](u64 count, const segment& segment) {
            return segment.code.size() + count;
        });
        [[maybe_unused]]
        u64 totalSize = sizeof(Elf64_Ehdr) + sizeof(Elf64_Phdr) + codeSize;

        // ELF Header:
        u64 segmentCount = segments.size();
        u64 headerOffset = sizeof(Elf64_Ehdr) + (segmentCount * sizeof(Elf64_Phdr));

        u64 entryPoint = BaseAddress + headerOffset;
        Elf64_Ehdr ehdr = create_elf_header(entryPoint, segmentCount);

        // Program headers:
        std::vector<Elf64_Phdr> programHeaders;
        programHeaders.reserve(segmentCount);

        u64 currentFileOffset = headerOffset;
        u64 currentVirtualAddress = entryPoint;
        for (const auto& segment : segments)
        {
            programHeaders.push_back(
                create_program_header(segment, currentFileOffset, currentVirtualAddress, PageSize)
            );

            u64 segmentSize = segment.code.size();
            currentFileOffset += segmentSize;
            currentVirtualAddress = align_address(currentVirtualAddress + segmentSize, PageSize);
        }
        
        file.write(reinterpret_cast<const char*>(&ehdr), sizeof(ehdr));
        file.write(reinterpret_cast<const char*>(programHeaders.data()), programHeaders.size() * sizeof(Elf64_Phdr));
        for (const auto& segment : segments)
        {
            if (!segment.code.empty())
                file.write(reinterpret_cast<const char*>(segment.code.data()), segment.code.size());
        }
    }
}