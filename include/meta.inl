#pragma once

namespace SimpleRTTR
{
    const Meta& Meta::InvalidMeta()
    {
        static Meta InvalidMeta(nullptr, nullptr);
        return InvalidMeta;
    }


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

    bool Meta::operator==(const Meta& rhs) const
    {
        return _Key == rhs._Key && _Value == rhs._Value;
    }

    bool Meta::operator!=(const Meta& rhs) const
    {
        return !operator==(rhs);
    }


    const Variant& Meta::Key() const {
        return _Key;
    }

    const Variant& Meta::Value() const {
        return _Value;
    }

    bool MetaContainer::Has(Variant key) const
    {
        for (const Meta& meta : _Data)
        {
            if (key == meta.Key())
            {
                return true;
            }
        }
        return false;
    }

    inline const Meta& MetaContainer::Get(Variant key) const
    {
        for (const Meta& meta : _Data)
        {
            if (key == meta.Key())
            {
                return meta;
            }
        }
        return Meta::InvalidMeta();
    }

}
