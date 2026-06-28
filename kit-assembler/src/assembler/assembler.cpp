#include "assembler.hpp"
#include "assembler/segment.hpp"
#include "parser/opcode.hpp"
#include "parser/statement.hpp"
#include <stdexcept>
#include <variant>
#include <vector>

using namespace kit;

static u64 get_instruction_size(assembler& assembler, const instruction& instruction, const handle_function& function)
{
    std::vector<u8> temporary;
    function(assembler, temporary, instruction);

    return temporary.size();
}

namespace kit
{
    assembler::assembler()
    {
        // Text section:
        mSegments.push_back({ .attributes = segment_attribute::read | segment_attribute::exec });
        mSegmentPCs.push_back(0);
        
        // Data section:
        mSegments.push_back({ .attributes = segment_attribute::read | segment_attribute::write });
        mSegmentPCs.push_back(0);
    }

    void assembler::assemble(std::vector<statement>& statements)
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

    void assembler::insert_reallocation(reallocation&& reallocation)
    {
        mReallocations.push_back(std::move(reallocation));
    }
}

namespace kit
{
    void assembler::zeroth_pass(std::vector<statement>& statements)
    {
        for (auto& statement : statements)
        {
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;

                if (!mEntryLabel.empty())
                    return;

                // Section:
                if constexpr (std::is_same_v<T, section>)
                {
                    if (arg.name == TextSectionName)
                        mCurrentSectionID = TextSegment;
                    
                    else if (arg.name == DataSectionName)
                        mCurrentSectionID = DataSegment;
                    
                    return;
                }

                // Entry:
                if constexpr (std::is_same_v<T, entry>)
                {
                    if (mCurrentSectionID == TextSegment)
                        mEntryLabel = arg.name;
                }
            }, statement);
        }

        if (mEntryLabel.empty())
            throw std::runtime_error("attempted to initialize a program without specifying the entry point");
    }

    void assembler::first_pass(std::vector<statement>& statements)
    {
        mCurrentSectionID = TextSegment;

        for (auto& statement : statements)
        {
            std::visit([&](auto&& arg)
            {
                using T = std::decay_t<decltype(arg)>;

                // Section:
                if constexpr (std::is_same_v<T, section>)
                {
                    if (arg.name == TextSectionName)
                        mCurrentSectionID = TextSegment;
                    
                    else if (arg.name == DataSectionName)
                        mCurrentSectionID = DataSegment;
                    
                    return;
                }

                // Label:
                if constexpr (std::is_same_v<T, label>)
                    mLabelMap[arg.name] = mSegmentPCs[mCurrentSectionID];

                // Instruction:
                else if constexpr (std::is_same_v<T, instruction>)
                {
                    if (mCurrentSectionID == TextSegment)
                    {
                        instruction instruction = arg;

                        if (gOpcodeMap.contains(instruction.code))
                            mSegmentPCs[mCurrentSectionID] += get_instruction_size(*this, instruction, gOpcodeMap[instruction.code]);
                    }
                    else if (mCurrentSectionID == DataSegment)
                    {
                        instruction instruction = arg;
                        if (instruction.code != opcode::wb || instruction.operands.empty())
                            return;

                        auto& dataCode = mSegments[DataSegment].code;

                        const auto& nameOperand = instruction.operands[0].label;
                        mMemoryOperandMap[nameOperand] = dataCode.size(); // + DataVirtualBaseAddress

                        for (auto& operand : instruction.operands)
                        {
                            if (operand.type != operand::kind::immediate)
                                continue;
                            
                            dataCode.push_back(static_cast<u8>(operand.immediate));
                        }
                    }                    
                }
            }, statement);
        }

        mReallocations.clear();
    }

    void assembler::second_pass(std::vector<statement>& statements)
    {
        mCurrentSectionID = TextSegment;

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
                    resolve_instruction_operands(inst);

                    if (gOpcodeMap.contains(arg.code))
                    {
                        auto& code = mSegments[mCurrentSectionID].code;
                        gOpcodeMap[arg.code](*this, code, arg);
                    }
                }
            }, statement);
        }
    }
    
    void assembler::resolve_instruction_operands(instruction& instruction)
    {
        if (instruction.operands.empty())
            return;

        for (auto& operand : instruction.operands)
        {
            if (operand.type == operand::kind::label)
            {
                operand.type = operand::kind::immediate;
                operand.immediate = mLabelMap[operand.label]; 
            }

            if (operand.type == operand::kind::memory)
                operand.immediate = mMemoryOperandMap[operand.label];
        }
    }
}