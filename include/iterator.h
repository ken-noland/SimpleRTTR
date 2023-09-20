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

    template<typename Container>
    class DefaultIterable
    {
    public:
        class IteratorProxy : public IteratorProxyBase<Container>
        {
        public:
            IteratorProxy(iterator iter) : TypeIteratorProxyBase(iter) {}
            reference operator*() const { return *(*_Iter); }
            pointer operator->() { return *_Iter; }
        };

        class ConstIteratorProxy : public IteratorProxyBase<Container>
        {
        public:
            ConstIteratorProxy(const_iterator iter) : TypeIteratorProxyBase(iter) {}
            const reference operator*() const { return *(*_Iter); }
            const pointer operator->() const { return *_Iter; }
        };


        using iterator = IteratorProxy;
        using const_iterator = ConstIteratorProxy;

        iterator begin() { return static_cast<TypeMgr*>(this)->Begin(); }
        const_iterator begin() const { return static_cast<TypeMgr*>(this)->Begin(); }
        iterator end() { return static_cast<TypeMgr*>(this)->End(); }
        const_iterator end() const { return static_cast<TypeMgr*>(this)->End(); }
    };


}
