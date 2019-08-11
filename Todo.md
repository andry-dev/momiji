* The parser should be restructured, again
* The parser should accept math expressions and operations on labels

Ideas:

```cpp
namespace objects {

struct Label {
    std::uint32_t hash;
};

struct Number {
    std::int32_t number;
};

struct ASTNode;

struct MathOperator {
    enum class Type {
        Add,
        Sub,
        Div,
        Mul,
        Pow,
    };

    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
};

struct ASTNode {
    std::variant<Label, Number, MathOperator> value;
};
}

namespace ops {
struct DataRegister {
    std::int8_t reg { 0 };
};

struct AddressRegister {
    std::int8_t reg { 0 };
};

struct Address {
    std::int8_t reg { 0 };
};

struct AddressPre {
    std::int8_t reg { 0 };
};

struct AddressPost {
    std::int8_t reg { 0 };
};

struct AddressOffset {
    std::unique_ptr<objects::ASTNode> offset;
    std::int8_t reg;
};

struct AddressIndex {
    std::unique_ptr<objects::ASTNode> offset;
    std::variant<DataRegister, AddressRegister> oth;
    std::int8_t reg;
};

struct Immediate {
    std::unique_ptr<objects::ASTNode> value;
};

struct AbsoluteShort {
    std::unique_ptr<objects::ASTNode> value;
};

struct AbsoluteLong {
    std::unique_ptr<objects::ASTNode> value;
};
}

using OperandType = std::variant<
    ops::DataRegister,
    ops::AddressRegister,
    ops::AddressPre,
    ops::AddressPost,
    ops::AddressOffset,
    ops::AddressIndex,
    ops::Immediate,
    ops::AbsoluteShort,
    ops::AbsoluteLong>;

struct Instruction {
    Instruction()
        : operands(2)
    {
    }

    std::vector<OperandType> operands;
};

template <typename T, typename Variant, typename Fun>
constexpr bool mapVariant(const Variant& op, Fun&& f) noexcept
{
    if (std::holds_alternative<T>(op)) {
        f(std::get<T>(op));

        return true;
    }

    return false;
}
```
