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

        inline std::size_t Hash() const;

    protected:
        Variant _Key;
        Variant _Value;
    };

    //TODO: meta should be encoded in a map of sorts(maybe? I mean, it is super fast to iterate and keeps things cache friendly as is)
    class MetaContainer : public DefaultContainer<Meta>
    {
    public:
        inline bool Has(Variant key) const;
        inline const Meta& Get(Variant key) const;
    private:
    };
}
