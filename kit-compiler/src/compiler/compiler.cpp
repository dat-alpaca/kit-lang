#include "compiler.hpp"
#include "compiler/instruction_set/jmp.hpp"
#include "compiler/segment.hpp"
#include "parser/opcode.hpp"
#include "parser/statement.hpp"
#include <stdexcept>
#include <variant>
#include <vector>

using namespace kit;

static u64 get_instruction_size(const instruction& instruction, const handle_function& function)
{
    std::vector<u8> temporary;
    function(temporary, instruction);

    return temporary.size();
}

namespace kit
{
    compiler::compiler()
    {
        // Text section:
        mSegments.push_back({ .attributes = segment_attribute::read | segment_attribute::exec });
        
        // Data section:
        mSegments.push_back({ .attributes = segment_attribute::read | segment_attribute::write });
    }

    void compiler::compile(std::vector<statement>& statements)
    {
        zeroth_pass(statements);

        {
            instruction inst { .code = opcode::jmp };
            inst.operands.push_back({ .type = operand::kind::label, .label = mEntryLabel });
            
            statements.insert(statements.begin(), inst);
        }

        first_pass(statements);
        second_pass(statements);
    }

    void compiler::zeroth_pass(std::vector<statement>& statements)
    {
        for (auto& statement : statements)
        {
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;

                // Section:
                if constexpr (std::is_same_v<T, section>)
                    return;

                // Entry:
                if constexpr (std::is_same_v<T, entry>)
                    mEntryLabel = arg.name;
            }, statement);
        }

        if (mEntryLabel.empty())
            throw std::runtime_error("attempted to initialize a program without specifying the entry point");
    }

    void compiler::first_pass(std::vector<statement>& statements)
    {
        for (auto& statement : statements)
        {
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;

                // Section:
                if constexpr (std::is_same_v<T, section>)
                    return;

                // Label:
                if constexpr (std::is_same_v<T, label>)
                    mLabelMap[arg.name] = mFirstPassCounter;

                // Instruction:
                else if constexpr (std::is_same_v<T, instruction>)
                {
                    instruction instruction = arg;
                    if (gOpcodeMap.contains(instruction.code))
                        mFirstPassCounter += get_instruction_size(instruction, gOpcodeMap[instruction.code]);
                }
            }, statement);
        }
    }

    void compiler::second_pass(std::vector<statement>& statements)
    {
        for (auto& statement : statements)
        {
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;

                if constexpr (std::is_same_v<T, label>)
                    return;

                else if constexpr (std::is_same_v<T, section>)
                {
                    if (arg.name == TextSectionName)
                        mCurrentSectionID = TextSegment;
                    
                    else if (arg.name == DataSectionName)
                        mCurrentSectionID = DataSegment;
                }                 

                else if constexpr (std::is_same_v<T, instruction>)
                {
                    instruction& inst = arg;
                    
                    // resolve instructions
                    if (!arg.operands.empty())
                    {
                        operand& operand0 = inst.operands[0];
                        if (arg.code == opcode::jmp)
                        {
                            if (operand0.type == operand::kind::label)
                            {
                                operand0.type = operand::kind::immediate;
                                operand0.immediate = mLabelMap[operand0.label]; 
                            }
                        }
                    }

                    if (gOpcodeMap.contains(arg.code))
                    {
                        auto& code = mSegments[mCurrentSectionID].code;
                        gOpcodeMap[arg.code](code, arg);
                    }
                }
            }, statement);
        }
    }
}