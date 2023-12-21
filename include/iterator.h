#pragma once

namespace SimpleRTTR
{
    template<typename Iterator>
    class IteratorProxyBase
    {
    public:
        using iterator = typename Iterator;
        using const_iterator = typename Iterator;

        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Type;
        using pointer = value_type*;
        using reference = value_type&;

        IteratorProxyBase(iterator iter) : _Iter(iter) {}

        IteratorProxyBase& operator++() { _Iter++; return *this; }
        IteratorProxyBase operator++(int) { IteratorProxyBase tmp = *this; ++(*this); return tmp; }

        friend bool operator== (const IteratorProxyBase<Iterator>& a, const IteratorProxyBase<Iterator>& b) { return a._Iter == b._Iter; };
        friend bool operator!= (const IteratorProxyBase<Iterator>& a, const IteratorProxyBase<Iterator>& b) { return a._Iter != b._Iter; };

    protected:
        iterator _Iter;
    };

    /****
    * The purpose of this class is just to provide an STL compliant being and end iterator to allow for range based for loops. Since we use upper camel case for
    *   member functions, and I want to be consistent, then this just provides a way to use the STL algorithms with our iterators. It also creates a very early 
    *   form of "concepts" since the class that inherits from this must have a Begin() and End() function, as well as defined the Iterator and ConstIterator types.
    ****/

    template<typename Iterable, typename StdContainer>
    class DefaultIterable
    {
    public:
        using ContainerType = StdContainer;
        using Iterator = typename StdContainer::iterator;
        using ConstIterator = typename StdContainer::const_iterator;

        // std type aliases
        using iterator = Iterator;
        using const_interator = ConstIterator;
        iterator begin() { return static_cast<Iterable*>(this)->Begin(); }
        const_interator begin() const { return static_cast<const Iterable*>(this)->Begin(); }
        iterator end() { return static_cast<Iterable*>(this)->End(); }
        const_interator end() const { return static_cast<const Iterable*>(this)->End(); }
    };

    template<typename Iterable>
    class ConstIterable
    {
    public:
        using const_iterator = typename Iterable::ConstIterator;

        const_iterator begin() const { return static_cast<Iterable*>(this)->Begin(); }
        const_iterator end() const { return static_cast<Iterable*>(this)->End(); }
    };
}
