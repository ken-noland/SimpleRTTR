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
        for (const Meta& meta : _Meta)
        {
            if (key == meta.Key())
            {
                return true;
            }
        }
        return false;
    }

    const Meta& MetaContainer::Get(Variant key) const
    {
        for (const Meta& meta : _Meta)
        {
            if (key == meta.Key())
            {
                return meta;
            }
        }
        return Meta::InvalidMeta();
    }

    MetaContainer::Iterator MetaContainer::Begin()
    {
        return _Meta.begin();
    }

    MetaContainer::ConstIterator MetaContainer::Begin() const
    {
        return _Meta.begin();
    }

    MetaContainer::Iterator MetaContainer::End()
    {
        return _Meta.end();
    }

    MetaContainer::ConstIterator MetaContainer::End() const
    {
        return _Meta.end();
    }

    Meta& MetaContainer::Back()
    {
        return _Meta.back();
    }

    const Meta& MetaContainer::Back() const
    {
        return _Meta.back();
    }

    std::size_t MetaContainer::Size() const
    {
        return _Meta.size();
    }

    void MetaContainer::PushBack(const Meta& meta)
    {
        _Meta.push_back(meta);
    }
}
