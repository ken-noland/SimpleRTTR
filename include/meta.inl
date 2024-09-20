#pragma once

namespace SimpleRTTR
{
    const Meta& Meta::invalid_meta()
    {
        static Meta invalid_meta(nullptr, nullptr);
        return invalid_meta;
    }


    template<typename MetaKey, typename MetaValue>
    Meta::Meta(MetaKey key, MetaValue value)
        :
        _Key(key),
        _Value(value)
    {
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

    bool Meta::operator==(const Meta& rhs) const
    {
        return _Key == rhs._Key && _Value == rhs._Value;
    }

    bool Meta::operator!=(const Meta& rhs) const
    {
        return !operator==(rhs);
    }


    const Variant& Meta::key() const {
        return _Key;
    }

    const Variant& Meta::value() const 
    {
        return _Value;
    }

    std::size_t Meta::hash() const
    {
        std::size_t seed = 0;
        hash_combine(seed, _Key, _Value);
        return seed;
    }


    bool MetaContainer::has(Variant key) const
    {
        for (const Meta& meta : _Data)
        {
            if (key == meta.key())
            {
                return true;
            }
        }
        return false;
    }

    inline const Meta& MetaContainer::get(Variant key) const
    {
        for (const Meta& meta : _Data)
        {
            if (key == meta.key())
            {
                return meta;
            }
        }
        return Meta::invalid_meta();
    }

}
