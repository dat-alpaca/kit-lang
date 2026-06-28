#pragma once
#include <vector>
#include <span>
#include "assembler/reallocation.hpp"

namespace kit
{
    class linker
    {
    public:
        linker(std::span<const reallocation> reallocations, u64 dataBaseVirtualAddress)
            : mReallocations(std::move(reallocations))
            , mDataBaseVirtualAddress(dataBaseVirtualAddress)
        {

        }

    public:
        void link(std::vector<u8>& code, const std::vector<u8>& data)
        {
            for (const auto& reallocation : mReallocations)
            {
                u64 absoluteAddress = mDataBaseVirtualAddress + reallocation.dataByteOffset;

                for (int i = 0; i < reallocation.size; ++i)
                    code[reallocation.codeByteOffset + i] = static_cast<u8>((absoluteAddress >> (i * 8)) & 0xFF);
            }
        }

    private:
        std::span<const reallocation> mReallocations;
        u64 mDataBaseVirtualAddress = 0;
    };
}