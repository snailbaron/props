#pragma once

#include <concepts>
#include <functional>
#include <utility>

// TODO: Change requirements to the standard, named ones?
// TODO: Think about other optional handlers
// TODO: Make sure a raw non-const reference is never returned anywhere
// TODO: Test that every decltype(auto) actually does something meaningful

namespace props {

template <class T>
class Property {
public:
    Property() requires requires { T{}; } {}

    Property(const T& value)
        : _value(value)
    { }

    Property(T&& value)
        : _value(std::move(value))
    { }

    Property& operator=(const Property& other)
    {
        _value = other._value;
        _afterUpdate(_value);
        return *this;
    }

    Property& operator=(Property&& other)
    {
        _value = std::move(other);
        _afterUpdate(_value);
        return *this;
    }

    Property& operator=(const T& value)
    {
        _value = value;
        _afterUpdate(_value);
        return *this;
    }

    Property& operator=(T&& value)
    {
        _value = std::move(value);
        _afterUpdate(_value);
        return *this;
    }

    operator const T& () const
    {
        return _value;
    }

    template <class... Args>
        requires requires (T value) { value(std::declval<Args>()...); }
    decltype(auto) operator()(Args&&... args)
    {
        decltype(auto) result = _value(std::forward<Args>(args)...);
        _afterUpdate(_value);
        return result;
    }

    template <class... Args>
        requires requires (const T value) { value(std::declval<Args>()...); }
    decltype(auto) operator()(Args&&... args) const
    {
        return _value(std::forward<Args>(args)...);
    }

    Property& operator++() requires requires (T x) { ++x; }
    {
        ++_value;
        _afterUpdate(_value);
        return *this;
    }

    Property operator++(int) requires requires (T x) { x++; }
    {
        auto result = Property{_value++};
        _afterUpdate(_value);
        return result;
    }

    Property& operator--() requires requires (T x) { --x; }
    {
        --_value;
        _afterUpdate(_value);
        return *this;
    }

    Property operator--(int) requires requires (T x) { x--; }
    {
        auto result = Property{_value--};
        _afterUpdate(_value);
        return result;
    }

    template <class U>
    Property& operator+=(const U& rhs) requires requires (T x, U y) { x += y; }
    {
        _value += rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U>
    Property& operator-=(const U& rhs) requires requires (T x, U y) { x -= y; }
    {
        _value -= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U>
    Property& operator&=(const U& rhs) requires requires (T x, U y) { x &= y; }
    {
        _value &= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U>
    Property& operator|=(const U& rhs) requires requires (T x, U y) { x |= y; }
    {
        _value |= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U>
    Property& operator^=(const U& rhs) requires requires (T x, U y) { x ^= y; }
    {
        _value ^= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U> requires requires (T x, U y) { x <<= y; }
    Property& operator<<=(const U& rhs)
    {
        _value <<= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class U> requires requires (T x, U y) { x >>= y; }
    Property& operator>>=(const U& rhs)
    {
        _value >>= rhs;
        _afterUpdate(_value);
        return *this;
    }

    template <class S> requires requires (S s, T x) { s << x; }
    friend decltype(auto) operator<<(S& stream, const Property& property)
    {
        return stream << property._value;
    }

    template <class S> requires requires (S s, T x) { s >> x; }
    friend decltype(auto) operator>>(S& stream, Property& property)
    {
        decltype(auto) result = (stream >> property._value);
        property._afterUpdate(property._value);
        return result;
    }

    template <class U> requires requires (T x, U y) { x << y; }
    friend decltype(auto) operator<<(Property& property, const U& x)
    {
        decltype(auto) result = (property._value << x);
        property._afterUpdate(property._value);
        return result;
    }

    template <class U> requires requires (T x, U y) { x >> y; }
    friend decltype(auto) operator>>(Property& property, U& x)
    {
        decltype(auto) result = (property._value >> x);
        property._afterUpdate(property._value);
        return result;
    }

private:
    T _value;
    std::function<void(const T& value)> _afterUpdate = [] (const T&) {};
};

} // namespace props
