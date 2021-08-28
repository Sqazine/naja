#pragma once
#include <string>
#include <string_view>
namespace NajaLang
{
    enum ObjectType
    {
        INT_NUM_OBJECT,
        FLOAT_NUM_OBJECT,
        STR_OBJECT,
        TRUE_OBJECT,
        FALSE_OBJECT,
        NULL_OBJECT,
    };

    struct Object
    {
        Object() {}
        virtual ~Object() {}

        virtual std::string Stringify() = 0;
        virtual ObjectType Type() = 0;
    };

    struct IntNumObject : public Object
    {
        IntNumObject() {}
        IntNumObject(int64_t value) : value(value) {}
        ~IntNumObject() {}

        std::string Stringify()
        {
            return std::to_string(value);
        }
        ObjectType Type()
        {
            return INT_NUM_OBJECT;
        }

        int64_t value;
    };

    struct FloatNumObject : public Object
    {
        FloatNumObject() {}
        FloatNumObject(double value) : value(value) {}
        ~FloatNumObject() {}

        std::string Stringify()
        {
            return std::to_string(value);
        }
        ObjectType Type()
        {
            return FLOAT_NUM_OBJECT;
        }

        double value;
    };

    struct StrObject : public Object
    {
        StrObject() {}
        StrObject(std::string_view value) : value(value) {}
        ~StrObject() {}

        std::string Stringify()
        {
            return value;
        }
        ObjectType Type()
        {
            return STR_OBJECT;
        }

        std::string value;
    };

    struct TrueObject : public Object
    {
        TrueObject() {}
        ~TrueObject() {}

        std::string Stringify()
        {
            return "true";
        }
        ObjectType Type()
        {
            return TRUE_OBJECT;
        }
    };

    struct FalseObject : public Object
    {
        FalseObject() {}
        ~FalseObject() {}

        std::string Stringify()
        {
            return "false";
        }
        ObjectType Type()
        {
            return FALSE_OBJECT;
        }
    };

    struct NullObject : public Object
    {
        NullObject() {}
        ~NullObject() {}

        std::string Stringify()
        {
            return "null";
        }
        ObjectType Type()
        {
            return NULL_OBJECT;
        }
    };
}
