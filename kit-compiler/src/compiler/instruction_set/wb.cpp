#include "wb.hpp"
#include "compiler/instruction_set/utils.hpp"

namespace kit
{
    void handle_wb(std::vector<u8>& dataCode, const instruction& instruction)
    {
        for (u64 i = 0; i < instruction.operands.size(); ++i)
        {
            if (i == 0)
                continue;

            u8 value = (u8)instruction.operands[i].immediate;
            dataCode.push_back(value);
        }
    }
}