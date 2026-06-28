#pragma once
#include <vector>
#include "compiler/reallocation.hpp"

namespace kit
{
    class linker
    {
    public:
        linker(std::vector<reallocation>&& reallocations, u64 dataBaseVirtualAddress)
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

                for (int i = 0; i < 8; ++i)
                    code[reallocation.codeByteOffset + i] = static_cast<u8>((absoluteAddress >> (i * 8)) & 0xFF);
            }
        }

    private:
        std::vector<reallocation> mReallocations;
        u64 mDataBaseVirtualAddress = 0;
    };
}