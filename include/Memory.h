#pragma once

#include <cstdint>
#include <cstddef>

#include <vector>
#include <stack>
#include <gsl/span>

#include <gsl/assert>

namespace momiji
{
    namespace details
    {
        struct ExecutableMemoryTag;
        struct StackMemoryTag;
    }

    template <typename Tag>
    class MemoryView;

    template <typename Tag>
    class ConstMemoryView;

    template <typename Container>
    class BasicMemory
    {
    public:
        std::uint32_t read32(std::uint64_t offset) const;
        std::uint16_t read16(std::uint64_t offset) const;
        std::uint8_t read8(std::uint64_t offset) const;

        template <typename T>
        void write32(T val, std::uint64_t offset) = delete;
        void write32(std::uint32_t val, std::uint64_t offset);

        template <typename T>
        void write16(T val, std::uint64_t offset) = delete;
        void write16(std::uint16_t val, std::uint64_t offset);

        template <typename T>
        void write8(T val, std::uint64_t offset) = delete;
        void write8(std::uint8_t val, std::uint64_t offset);

        auto begin() const
        {
            return m_data.begin();
        }

        auto begin()
        {
            return m_data.begin();
        }

        auto end() const
        {
            return m_data.end();
        }
        
        auto end()
        {
            return m_data.end();
        }

        auto size() const
        {
            return m_data.size();
        }

        auto empty() const
        {
            return m_data.empty();
        }

        auto data()
        {
            return m_data.data();
        }

    protected:
        Container m_data;
    };

    template <typename Tag>
    class ModifiableMemory final : public BasicMemory<std::vector<std::uint8_t>>
    {
    public:

        template <typename T>
        void push32(T val) = delete;
        void push32(std::uint32_t val);

        template <typename T>
        void push16(T val) = delete;
        void push16(std::uint16_t val);

        template <typename T>
        void push8(T val) = delete;
        void push8(std::uint8_t val);

        void pop32();
        void pop16();
        void pop8();

    private:

        friend class MemoryView<Tag>;
        friend class ConstMemoryView<Tag>;
    };

    template <typename Tag>
    class MemoryView final : public BasicMemory<gsl::span<std::uint8_t>>
    {
    public:
        MemoryView(ModifiableMemory<Tag>& mem)
        {
            m_data = { mem.m_data.data(), mem.m_data.size() };
        }

        MemoryView(gsl::span<std::uint8_t> span)
        {
            m_data = span;
        }
    };

    template <typename Tag>
    class ConstMemoryView final : public BasicMemory<gsl::span<const std::uint8_t>>
    {
    public:
        ConstMemoryView(const ModifiableMemory<Tag>& mem)
        {
            m_data = { mem.m_data.data(), mem.m_data.size() };
        }

        ConstMemoryView(gsl::span<const std::uint8_t> span)
        {
            m_data = span;
        }
    };

    using ExecutableMemory = ModifiableMemory<details::ExecutableMemoryTag>;
    using ExecutableMemoryView = MemoryView<details::ExecutableMemoryTag>;
    using ConstExecutableMemoryView = ConstMemoryView<details::ExecutableMemoryTag>;

    using StackMemory = ModifiableMemory<details::StackMemoryTag>;
    using StackMemoryView = MemoryView<details::StackMemoryTag>;

    // BasicMemory implementation

    template <typename Container>
    std::uint32_t BasicMemory<Container>::read32(std::uint64_t offset) const
    {
        return   (m_data[offset] << 24)
               | (m_data[offset + 1] << 16)
               | (m_data[offset + 2] << 8)
               | (m_data[offset + 3]);
    }

    template <typename Container>
    std::uint16_t BasicMemory<Container>::read16(std::uint64_t offset) const
    {
        return   (m_data[offset] << 8)
               | (m_data[offset + 1]);
    }

    template <typename Container>
    std::uint8_t BasicMemory<Container>::read8(std::uint64_t offset) const
    {
        return m_data[offset];
    }

    template <typename Container>
    void BasicMemory<Container>::write32(std::uint32_t val, std::uint64_t offset)
    {
        const std::uint8_t first =  (val & 0xFF000000) >> 24;
        const std::uint8_t second = (val & 0x00FF0000) >> 16;
        const std::uint8_t third =  (val & 0x0000FF00) >> 8;
        const std::uint8_t fourth = (val & 0x000000FF);

        m_data[offset] = first;
        m_data[offset + 1] = second;
        m_data[offset + 2] = third;
        m_data[offset + 3] = fourth;
    }

    template <typename Container>
    void BasicMemory<Container>::write16(std::uint16_t val, std::uint64_t offset)
    {
        const std::uint8_t first =  (val & 0xFF00) >> 8;
        const std::uint8_t second = (val & 0x00FF);

        m_data[offset] = first;
        m_data[offset + 1] = second;
    }

    template <typename Container>
    void BasicMemory<Container>::write8(std::uint8_t val, std::uint64_t offset)
    {
        m_data[offset] = val;
    }

    // ModifiableMemory

    template <typename Tag>
    void ModifiableMemory<Tag>::push32(std::uint32_t val)
    {
        const std::uint8_t first  = (val & 0xFF00'0000) >> 24;
        const std::uint8_t second = (val & 0x00FF'0000) >> 16;
        const std::uint8_t third  = (val & 0x0000'FF00) >> 8;
        const std::uint8_t fourth = (val & 0x0000'00FF);

        m_data.push_back(first);
        m_data.push_back(second);
        m_data.push_back(third);
        m_data.push_back(fourth);
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::push16(std::uint16_t val)
    {
        const std::uint8_t first =  (val & 0xFF00) >> 8;
        const std::uint8_t second = (val & 0x00FF);

        m_data.push_back(first);
        m_data.push_back(second);
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::push8(std::uint8_t val)
    {
        m_data.push_back(val);
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::pop32()
    {
        m_data.pop_back();
        m_data.pop_back();
        m_data.pop_back();
        m_data.pop_back();
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::pop16()
    {
        m_data.pop_back();
        m_data.pop_back();
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::pop8()
    {
        m_data.pop_back();
    }
}
