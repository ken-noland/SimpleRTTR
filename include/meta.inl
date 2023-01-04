#pragma once

namespace SimpleRTTR
{
    template<typename MetaKey, typename MetaValue>
    Meta::Meta(MetaKey key, MetaValue value)
        :
        _Key(key),
        _Value(value)
    {
        Types().GetOrCreateType<MetaKey>();
        Types().GetOrCreateType<MetaValue>();
    }

    Meta::Meta(const Meta& meta)
        :
        _Key(meta._Key),
        _Value(meta._Value)
    {
    }

    Meta::Meta(Meta&& meta)
        :
        _Key(std::move(meta._Key)),
        _Value(std::move(meta._Value))
    {
    }

    Meta& Meta::operator=(const Meta& meta)
    {
        _Key = meta._Key;
        _Value = meta._Value;
        return *this;
    }

    const Variant& Meta::Key() const {
        return _Key;
    }

    const Variant& Meta::Value() const {
        return _Value;
    }

}
