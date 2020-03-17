#pragma once

#include <cstddef>
#include <cstdint>

#include <gsl/span>
#include <stack>
#include <vector>

#include <asl/detect_features>
#include <gsl/assert>

#include <optional>

namespace momiji
{
#ifdef ASL_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wsign-conversion"
#endif

    namespace details
    {
        // Instructions
        // You should not write here
        struct ExecutableMemoryTag;

        // Static/global variables
        struct StaticMemoryTag;

        // Stack
        struct StackMemoryTag;
    } // namespace details

    template <typename Tag>
    struct MemoryMarker
    {
        using tag = Tag;

        std::int64_t begin { -1 };
        std::int64_t end { -1 };
    };

    template <typename Tag>
    class MemoryView;

    template <typename Tag>
    class ConstMemoryView;

    template <typename Container>
    class BasicMemory
    {
    public:
        BasicMemory() = default;

        BasicMemory(const BasicMemory& oth)     = default;
        BasicMemory(BasicMemory&& oth) noexcept = default;

        BasicMemory& operator=(const BasicMemory& oth) = default;
        BasicMemory& operator=(BasicMemory&& oth) noexcept = default;

        ~BasicMemory() = default;

        [[nodiscard]] std::optional<std::uint32_t>
        read32(std::int64_t offset) const noexcept;

        [[nodiscard]] std::optional<std::uint16_t>
        read16(std::int64_t offset) const noexcept;

        [[nodiscard]] std::optional<std::uint8_t>
        read8(std::int64_t offset) const noexcept;

        template <typename T>
        [[nodiscard]] bool write32(T val,
                                   std::int64_t offset) noexcept = delete;
        [[nodiscard]] bool write32(std::uint32_t val,
                                   std::int64_t offset) noexcept;

        template <typename T>
        [[nodiscard]] bool write16(T val,
                                   std::int64_t offset) noexcept = delete;
        [[nodiscard]] bool write16(std::uint16_t val,
                                   std::int64_t offset) noexcept;

        template <typename T>
        [[nodiscard]] bool write8(T val, std::int64_t offset) noexcept = delete;
        [[nodiscard]] bool write8(std::uint8_t val,
                                  std::int64_t offset) noexcept;

        [[nodiscard]] auto begin() const noexcept;
        [[nodiscard]] auto end() const noexcept;

        [[nodiscard]] auto size() const noexcept;

        [[nodiscard]] auto empty() const noexcept;

        [[nodiscard]] auto data() noexcept;

        [[nodiscard]] Container& underlying();

        MemoryMarker<details::ExecutableMemoryTag> executableMarker {};
        MemoryMarker<details::StackMemoryTag> stackMarker {};
        MemoryMarker<details::StaticMemoryTag> staticMarker {};

    protected:
        BasicMemory(std::int64_t size);
        Container m_data;
    };

    template <typename Tag>
    class ModifiableMemory final
        : private BasicMemory<std::vector<std::uint8_t>>
    {
    public:
        ModifiableMemory() = default;
        ModifiableMemory(std::int64_t size);

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

    struct NullMemoryView
    {
    };

    // TODO(andry): Bad name
    template <typename Tag>
    class MemoryView final : private BasicMemory<gsl::span<std::uint8_t>>
    {
    public:
        MemoryView(ModifiableMemory<Tag>& mem)
        {
            m_data = { mem.m_data.data(), asl::ssize(mem.m_data) };

            executableMarker = mem.executableMarker;
            stackMarker      = mem.stackMarker;
            staticMarker     = mem.staticMarker;
        }

        MemoryView(gsl::span<std::uint8_t> span)
        {
            m_data = span;
        }

        MemoryView(NullMemoryView /*unused*/)
        {
            m_data = { gsl::null_span {} };
        }

        template <typename T>
        MemoryView& operator=(const T& oth)
        {
            MemoryView newMem { oth };
            std::swap(*this, newMem);

            return *this;
        }

        template <typename T>
        MemoryView& operator=(T&& oth)
        {
            MemoryView newMem { std::forward<T>(oth) };
            std::swap(*this, newMem);

            return *this;
        }

    private:
        friend class ConstMemoryView<Tag>;
    };

    template <typename Tag>
    class ConstMemoryView final
        : private BasicMemory<gsl::span<const std::uint8_t>>
    {
    public:
        ConstMemoryView(const ModifiableMemory<Tag>& mem)
        {
            m_data = { mem.m_data.data(), asl::ssize(mem.m_data) };

            executableMarker = mem.executableMarker;
            stackMarker      = mem.stackMarker;
            staticMarker     = mem.staticMarker;
        }

        ConstMemoryView(const MemoryView<Tag>& mem)
        {
            m_data = { mem.m_data.data(), asl::ssize(mem.m_data) };

            executableMarker = mem.executableMarker;
            stackMarker      = mem.stackMarker;
            staticMarker     = mem.staticMarker;
        }

    private:
        friend class MemoryView<Tag>;
    };

    using ExecutableMemory =
        ModifiableMemory<momiji::details::ExecutableMemoryTag>;
    using ExecutableMemoryView =
        MemoryView<momiji::details::ExecutableMemoryTag>;
    using ConstExecutableMemoryView =
        ConstMemoryView<momiji::details::ExecutableMemoryTag>;

    using StackMemory     = ModifiableMemory<momiji::details::StackMemoryTag>;
    using StackMemoryView = MemoryView<momiji::details::StackMemoryTag>;

    // BasicMemory implementation

    template <typename Container>
    BasicMemory<Container>::BasicMemory(std::int64_t size)
        : m_data(size)
    {
        Expects(size > 0, "Passing negative size to basic memory")
    }

    template <typename Container>
    [[nodiscard]] auto BasicMemory<Container>::begin() const noexcept
    {
        return m_data.begin();
    }

    template <typename Container>
    [[nodiscard]] auto BasicMemory<Container>::end() const noexcept
    {
        return m_data.end();
    }

    template <typename Container>
    [[nodiscard]] auto BasicMemory<Container>::size() const noexcept
    {
        return m_data.size();
    }

    template <typename Container>
    [[nodiscard]] auto BasicMemory<Container>::empty() const noexcept
    {
        return m_data.empty();
    }

    template <typename Container>
    [[nodiscard]] auto BasicMemory<Container>::data() noexcept
    {
        return m_data.data();
    }

    template <typename Container>
    [[nodiscard]] Container& BasicMemory<Container>::underlying()
    {
        return m_data;
    }

    template <typename Container>
    [[nodiscard]] std::optional<std::uint32_t>
    BasicMemory<Container>::read32(std::int64_t offset) const noexcept
    {
        if ((offset + 3) >= asl::ssize(m_data) || offset < 0)
        {
            return std::nullopt;
        }

        return std::uint32_t((m_data[offset]) | (m_data[offset + 1] << 8) |
                             (m_data[offset + 2] << 16) |
                             (m_data[offset + 3] << 24));
    }

    template <typename Container>
    [[nodiscard]] std::optional<std::uint16_t>
    BasicMemory<Container>::read16(std::int64_t offset) const noexcept
    {
        if ((offset + 1) >= asl::ssize(m_data) || offset < 0)
        {
            return std::nullopt;
        }

        return std::uint16_t((m_data[offset]) | (m_data[offset + 1] << 8));
    }

    template <typename Container>
    [[nodiscard]] std::optional<std::uint8_t>
    BasicMemory<Container>::read8(std::int64_t offset) const noexcept
    {
        if (offset >= asl::ssize(m_data) || offset < 0)
        {
            return std::nullopt;
        }

        return m_data[offset];
    }

    template <typename Container>
    [[nodiscard]] bool
    BasicMemory<Container>::write32(std::uint32_t val,
                                    std::int64_t offset) noexcept
    {
        if ((offset + 3) >= asl::ssize(m_data) || offset < 0)
        {
            return false;
        }

        const std::uint8_t first  = (val & 0x000000FF);
        const std::uint8_t second = (val & 0x0000FF00) >> 8;
        const std::uint8_t third  = (val & 0x00FF0000) >> 16;
        const std::uint8_t fourth = (val & 0xFF000000) >> 24;

        m_data[offset]     = first;
        m_data[offset + 1] = second;
        m_data[offset + 2] = third;
        m_data[offset + 3] = fourth;

        return true;
    }

    template <typename Container>
    [[nodiscard]] bool
    BasicMemory<Container>::write16(std::uint16_t val,
                                    std::int64_t offset) noexcept
    {
        if ((offset + 1) >= asl::ssize(m_data) || offset < 0)
        {
            return false;
        }

        const std::uint8_t first  = (val & 0x00FF);
        const std::uint8_t second = (val & 0xFF00) >> 8;

        m_data[offset]     = first;
        m_data[offset + 1] = second;

        return true;
    }

    template <typename Container>
    [[nodiscard]] bool
    BasicMemory<Container>::write8(std::uint8_t val,
                                   std::int64_t offset) noexcept
    {
        if (offset >= asl::ssize(m_data.size) || offset < 0)
        {
            return false;
        }

        m_data[offset] = val;

        return true;
    }

    // ModifiableMemory

    template <typename Tag>
    ModifiableMemory<Tag>::ModifiableMemory(std::int64_t size)
        : BasicMemory(size)
    {
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::push32(std::uint32_t val)
    {
        // Possibly align for odd addresses
        if (asl::ssize(m_data) & 1)
        {
            m_data.push_back(0);
        }

        const std::uint8_t first  = (val & 0x0000'00FF);
        const std::uint8_t second = (val & 0x0000'FF00) >> 8;
        const std::uint8_t third  = (val & 0x00FF'0000) >> 16;
        const std::uint8_t fourth = (val & 0xFF00'0000) >> 24;

        m_data.push_back(first);
        m_data.push_back(second);
        m_data.push_back(third);
        m_data.push_back(fourth);
    }

    template <typename Tag>
    void ModifiableMemory<Tag>::push16(std::uint16_t val)
    {
        // Possibly align for odd addresses
        if (asl::ssize(m_data) & 1)
        {
            m_data.push_back(0);
        }

        const std::uint8_t first  = (val & 0x00FF);
        const std::uint8_t second = (val & 0xFF00) >> 8;

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

#ifdef ASL_CLANG
#pragma clang diagnostic pop
#endif

} // namespace momiji
