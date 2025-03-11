namespace SimpleRTTR
{

    inline TemplateParameter::TemplateParameter(TypeReference type) 
        : _type(type) 
    {
    }

    inline TemplateParameter::TemplateParameter(Variant value) 
        : _value(value), _type(value.type()) 
    {
    }

    inline const bool TemplateParameter::has_type() const 
    { 
        return _type.has_value(); 
    }

    inline const TypeReference& TemplateParameter::type() const 
    { 
        return _type.value(); 
    }

    inline const bool TemplateParameter::has_value() const 
    { 
        return _value.has_value(); 
    }

    inline const Variant& TemplateParameter::value() const 
    { 
        return _value.value(); 
    }

} //namespace SimpleRTTR