#pragma once

namespace SimpleRTTR
{
    class Meta
    {
    public:
        template<typename MetaKey, typename MetaValue>
        inline Meta(MetaKey key, MetaValue value);
        inline Meta(const Meta& meta);
        inline Meta(Meta&& meta);
        inline Meta& operator=(const Meta& meta);

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
        template<typename MetaKey>
        const Meta& Get(MetaKey key);
    private:

    };
}
