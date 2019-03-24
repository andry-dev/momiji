#include "Parser.h"

#include <string>
#include <utility>
#include <memory>
#include <string_view>
#include <iostream>
#include <optional>
#include <cassert>

#include "Utils.h"
#include "Instructions.h"

namespace momiji
{
    static auto make_parser_error(int column, int line,
            parser_error::error_type error)
    {
        return nonstd::make_unexpected<parser_error>({ line, column, error });
    }

    struct parser
    {
        parser(std::string str)
            : m_str(std::move(str))
        {

        }
    public:
        nonstd::expected<momiji::instruction, parser_error> parse();

    private:

        template <std::size_t Size>
        std::optional<std::string> readWord(const std::array<char, Size>& pattern);

        auto currentPos(int offset = 0);
        void incrLine();
        bool incrPos(int increment = 1);

        bool hasPositions(int offset = 0);

        bool matchEndl();
        bool skipWhitespace();
        bool skipComments();
        bool tryImmReg(int operandNum, momiji::instruction& instr);

        bool parseCharacter(char c);
        bool parseOperand(momiji::operand_type operand, int operandNum, momiji::instruction& instr);
        bool parseKeyword(std::string_view keywordStr);
        bool parseIdentifier();
        bool parseDataType(momiji::instruction& instr);

        std::string m_str;
        int m_line = 1;
        int m_column = 1;
        int m_pos = 0;
    };

    void parser::incrLine()
    {
        m_column = 1;
        ++m_line;
    }

    bool parser::incrPos(int increment)
    {
        if (matchEndl())
        {
            incrLine();
        }

        if (m_pos < m_str.size())
        {
            m_pos += increment;
            m_column += increment;
            return true;
        }

        return false;
    }

    auto parser::currentPos(int offset)
    {
        return m_str[m_pos + offset];
    }

    bool parser::hasPositions(int offset)
    {
        return (m_pos + offset) < m_str.size();
    }

    bool parser::matchEndl()
    {
        return hasPositions() &&
               currentPos() != 0 &&
               (currentPos() == '\n' ||
                   (hasPositions(1) &&
                    currentPos() == '\r' &&
                    currentPos(1) == '\n'));
    }

    bool parser::skipComments()
    {

        auto comment_checker = [&] () -> bool {
            const bool is_valid = currentPos() != 0;
            const bool is_comment = is_valid && currentPos() == ';';

            return is_comment;
        };

        bool retval = false;

        while (hasPositions() && comment_checker())
        {
            retval = true;
            while (!matchEndl())
            {
                incrPos();
            }
        }

        return retval;
    }

    bool parser::skipWhitespace()
    {
        bool retval = false;

        while (hasPositions())
        {
            const bool is_valid = currentPos() != 0;
            const bool is_space = is_valid &&
                                  (currentPos() == ' ' ||
                                   currentPos() == '\t');


            if (is_space || matchEndl() || skipComments())
            {
                incrPos();
                retval = true;
            }
            else
            {
                break;
            }
        }

        return retval;
    }

    template <std::size_t Size>
    std::optional<std::string> parser::readWord(const std::array<char, Size>& pattern)
    {
        const int saved_pos = m_pos;

        while (hasPositions()) 
        {
            bool found_delimiter = false;
            for (char c : pattern)
            {
                // Found an delimited
                if (currentPos() == c)
                {
                    found_delimiter = true;
                    break;
                }
            }

            if (!found_delimiter)
            {
                incrPos();
            }
            else
            {
                break;
            }
        }

        std::string str = m_str.substr(saved_pos, m_pos);

        if (str.empty())
        {
            m_pos = saved_pos;
            return std::nullopt;
        }

        return str;

    }

    bool parser::parseCharacter(char c)
    {
        return hasPositions() && currentPos() == c;
    }

    bool parser::parseDataType(instruction& instr)
    {
        if (parseCharacter('.') && hasPositions(1))
        {
            const auto saved_pos = m_pos;
            incrPos();

            switch (currentPos())
            {
            case 'l':
                instr.dataType = data_type::Long;
                incrPos();
                return true;
            case 'w':
                instr.dataType = data_type::Word;
                incrPos();
                return true;
            case 'b':
                instr.dataType = data_type::Byte;
                incrPos();
                return true;

            default:
                m_pos = saved_pos;
                return false;
            }
        }

        return false;
    }

    bool parser::parseOperand(momiji::operand_type type, int operandNum, momiji::instruction& instr)
    {
        skipWhitespace();

        switch (type)
        {
        case operand_type::Immediate:
            if (hasPositions() && currentPos() == '#')
            {
                incrPos();

                int idx = 0;

                while (hasPositions() && (currentPos(idx) != ','))
                {
                    ++idx;
                }

                auto str = m_str.substr(m_pos, idx);

                if (str.empty())
                {
                    return false;
                }

                incrPos(idx + 1);

                std::int32_t num = std::stoi(str);
                instr.operands[operandNum].value = num;
                instr.operands[operandNum].operandType = operand_type::Immediate;

                return true;
            }

            return false;

        case operand_type::Register:
            skipWhitespace();

            if (hasPositions())
            {

                auto str = readWord(std::array<char, 4>{' ', '\t', '\n', '\r'});

                if (!str || (*str).size() < 2)
                {
                    return false;
                }

                std::int32_t regNum = std::stoi((*str).substr(1));

                if (regNum < 0 || regNum > 7)
                {
                    return false;
                }

                instr.operands[operandNum].operandType = operand_type::Register;
                instr.operands[operandNum].value = regNum;

                switch ((*str)[0])
                {
                case 'a':
                    instr.operands[operandNum].registerType = register_type::Address;
                    return true;

                case 'd':
                    instr.operands[operandNum].registerType = register_type::Data;
                    return true;
                }
            }
            return false;
        }

        return false;
    }

    bool parser::tryImmReg(int operandNum, momiji::instruction& instr)
    {
        if (!parseOperand(operand_type::Immediate, operandNum, instr))
        {
            return parseOperand(operand_type::Register, operandNum, instr);
        }

        return true;
    }

    nonstd::expected<momiji::instruction, parser_error> parser::parse()
    {
        momiji::instruction instr;

        skipWhitespace();

        auto str = readWord(std::array<char, 3>{' ', '.', '\t'});

        if (!str)
        {
            return make_parser_error(m_column, m_line,
                                     parser_error::error_type::NoInstructionFound);
        }

        auto hashed_str = utils::hash(*str);

        switch (hashed_str)
        {
        case utils::hash("move"):
            instr.numOperands = 2;
            instr.instructionType = instruction_type::Move;

            if (!parseDataType(instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::UnexpectedCharacter);
            }

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            switch (instr.dataType)
            {
            case data_type::Long:
                instr.executefn = op_impl::move32;
                break;
            case data_type::Word:
                instr.executefn = op_impl::move16;
                break;
            case data_type::Byte:
                instr.executefn = op_impl::move8;
                break;
            case data_type::Address:
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongInstruction);
            }

            break;

        case utils::hash("moveq"):
            instr.numOperands = 2;

            instr.instructionType = instruction_type::MoveImmediate;

            if (!parseOperand(operand_type::Immediate, 0, instr))
            {
                make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            instr.executefn = op_impl::moveq;

            break;

        case utils::hash("add"):
            instr.numOperands = 2;

            instr.instructionType = instruction_type::Add;

            if (!parseDataType(instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::UnexpectedCharacter);
            }

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            if (instr.operands[1].registerType == register_type::Address)
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            switch (instr.dataType)
            {
            case data_type::Byte:
                instr.executefn = op_impl::add8;
                break;
            case data_type::Word:
                instr.executefn = op_impl::add16;
                break;
            case data_type::Long:
                instr.executefn = op_impl::add32;
                break;
            case data_type::Address:
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongInstruction);

            }

            break;

        case utils::hash("sub"):
            instr.numOperands = 2;

            instr.instructionType = instruction_type::Sub;

            if (!parseDataType(instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::UnexpectedCharacter);
            }

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                  parser_error::error_type::WrongOperandType);
            }

            switch (instr.dataType)
            {
            case data_type::Byte:
                instr.executefn = op_impl::sub8;
                break;
            case data_type::Word:
                instr.executefn = op_impl::sub16;
                break;
            case data_type::Long:
                instr.executefn = op_impl::sub32;
                break;
            case data_type::Address:
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongInstruction);
            }
            break;

        case utils::hash("muls"):
            instr.numOperands = 2;
            instr.instructionType = instruction_type::SignedMul;

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (instr.operands[0].registerType == register_type::Address ||
                instr.operands[1].registerType == register_type::Address)
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            instr.executefn = op_impl::muls;
            break;

        case utils::hash("mulu"):
            instr.numOperands = 2;
            instr.instructionType = instruction_type::UnsignedMul;

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (instr.operands[0].registerType == register_type::Address ||
                instr.operands[1].registerType == register_type::Address)
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            instr.executefn = op_impl::mulu;
            break;

        case utils::hash("divs"):
            instr.numOperands = 2;
            instr.instructionType = instruction_type::SignedDiv;

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (instr.operands[0].registerType == register_type::Address ||
                instr.operands[1].registerType == register_type::Address)
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            instr.executefn = op_impl::divs;
            break;

        case utils::hash("divu"):
            instr.numOperands = 2;
            instr.instructionType = instruction_type::UnsignedDiv;

            if (!tryImmReg(0, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (!parseOperand(operand_type::Register, 1, instr))
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            if (instr.operands[0].registerType == register_type::Address ||
                instr.operands[1].registerType == register_type::Address)
            {
                return make_parser_error(m_column, m_line,
                                         parser_error::error_type::WrongOperandType);
            }

            instr.executefn = op_impl::divu;
            break;

        case utils::hash("exg"):
            break;

        case utils::hash("swap"):
            break;

        case utils::hash("tst"):
            break;

        case utils::hash("cmp"):
            break;

        case utils::hash("bne"):
            break;

        default:
            return make_parser_error(m_column, m_line,
                                 parser_error::error_type::NoInstructionFound);
        }


        return instr;
    }

    nonstd::expected<instruction, parser_error> readInstruction(const std::string& str)
    {
        parser pars{str};

        return pars.parse();

    }

    std::vector<instruction> readInstructions(const std::string& str)
    {
        int idx = 0;
        std::vector<instruction> instructions;

        return instructions;
    }
}
