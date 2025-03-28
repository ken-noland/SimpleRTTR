#pragma once

namespace SimpleRTTR
{
    template<typename Iterator>
    class IteratorProxyBase
    {
    public:
        using iterator = Iterator;
        using const_iterator = Iterator;

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
    * The purpose of this class is just to provide an STL compliant begin and end iterator to allow for range based for loops. Since we use upper camel case for
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

    /****
    * Since most of our containers are vectors, we can simply inherit from this class and that will set up the majority of the functionality we need, the only drawback is
    * that we can't specify the container name, but that's not a big deal. We can also use this as a base class for other containers that are not vectors, but we will have to
    * define the container type ourselves.
    ****/
    template<typename ClassType, typename Container = std::vector<ClassType>>
    class DefaultContainer : public DefaultIterable<DefaultContainer<ClassType, Container>, Container>
    {
    public:
        DefaultContainer() : _Data() { }
        DefaultContainer(const DefaultContainer& container) : _Data(container._Data) { }
        DefaultContainer(DefaultContainer&& container) : _Data(std::move(container._Data)) { }

        DefaultContainer(const Container& values) : _Data(values) { }
        DefaultContainer(Container&& values) : _Data(std::move(values)) { }

        DefaultContainer& operator=(const DefaultContainer& rhs) { _Data = rhs._Data; return *this; }

        using ContainerType = std::vector<ClassType>;
        using Iterator = typename ContainerType::iterator;
        using ConstIterator = typename ContainerType::const_iterator;

        Iterator begin() { return _Data.begin(); }
        Iterator end() { return _Data.end(); }
        ConstIterator begin() const { return _Data.begin(); }
        ConstIterator end() const { return _Data.end(); }

        inline ClassType& operator[](std::size_t index) { return _Data[index]; }
        inline const ClassType& operator[](std::size_t index) const { return _Data[index]; }

        void add(const ClassType& value) { _Data.push_back(value); }
        void add(ClassType&& value) { _Data.push_back(value); }

        void clear() { _Data.clear(); }

        size_t size() const { return _Data.size(); }

        const ClassType& back() const { return _Data.back(); }
        ClassType& back() { return _Data.back(); }

        void reserve(size_t size) { _Data.reserve(size); }

        std::size_t hash() const
        {
            std::size_t seed = 0;
            for (const ClassType& value : _Data)
            {
                seed ^= std::hash<ClassType>()(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }

    protected:
        ContainerType _Data;
    };
}
