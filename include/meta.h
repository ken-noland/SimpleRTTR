#pragma once

namespace SimpleRTTR
{
    class Meta
    {
    public:
        static inline const Meta& InvalidMeta();

        template<typename MetaKey, typename MetaValue>
        inline Meta(MetaKey key, MetaValue value);
        inline Meta(const Meta& meta);
        inline Meta(Meta&& meta);
        inline Meta& operator=(const Meta& meta);

        inline bool operator==(const Meta& rhs) const;
        inline bool operator!=(const Meta& rhs) const;

        inline const Variant& Key() const;
        inline const Variant& Value() const;

    protected:
        Variant _Key;
        Variant _Value;
    };

    //follow std library conventions?
    class MetaContainer
    {
    public:
        using type = Meta;
        using MetaList = std::vector<type>;

        using iterator = MetaList::iterator;
        using const_iterator = MetaList::const_iterator;

        inline const Meta& Get(Variant key) const;

        inline iterator begin();
        inline const_iterator begin() const;

        inline iterator end();
        inline const_iterator end() const;

        inline Meta& back();
        inline const Meta& back() const;

        inline std::size_t size() const;

        inline void push_back(const Meta& meta);

    private:
        //TODO: meta should be encoded in a map of sorts, with the key being the value hash and the type hash combined
        MetaList _Meta;
    };
}
