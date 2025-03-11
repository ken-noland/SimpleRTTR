#pragma once

namespace SimpleRTTR
{

    class TemplateParameter
    {
    public:
        TemplateParameter(TypeReference type);
        TemplateParameter(Variant value);

        inline const bool has_type() const;
        inline const TypeReference& type() const;

        inline const bool has_value() const;
        inline const Variant& value() const;

    private:
        std::optional<TypeReference> _type;
        std::optional<Variant> _value;
    };

} //namespace SimpleRTTR
