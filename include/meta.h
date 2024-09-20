#pragma once

namespace SimpleRTTR
{
    class Meta
    {
    public:
        static inline const Meta& invalid_meta();

        template<typename MetaKey, typename MetaValue>
        inline Meta(MetaKey key, MetaValue value);
        inline Meta(const Meta& meta);
        inline Meta(Meta&& meta);
        inline Meta& operator=(const Meta& meta);

        inline bool operator==(const Meta& rhs) const;
        inline bool operator!=(const Meta& rhs) const;

        inline const Variant& key() const;
        inline const Variant& value() const;

        inline std::size_t hash() const;

    protected:
        Variant _Key;
        Variant _Value;
    };

    //TODO: meta should be encoded in a map of sorts(maybe? I mean, it is super fast to iterate and keeps things cache friendly as is)
    class MetaContainer : public DefaultContainer<Meta>
    {
    public:
        inline bool has(Variant key) const;
        inline const Meta& get(Variant key) const;
    private:
    };
}
