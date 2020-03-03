#pragma once

#include <boost/variant.hpp>
#include <boost/signals2.hpp>

namespace Dragon{namespace IR{
    
    using boost::blank;
    using boost::optional;
    using boost::variant;
    using boost::apply_visitor;
    
    class IValue;
    class IFunction;
    class IContext;
    class IType;
    class VariableBase;
    class IConstant;
    class IClass;
    class ISymbolTable;
    class IInstruction;
    class AddressList;
    class Function;
    class MemberFunctionAccessor;
    
    class FieldAccessor;
    class GenericAddress;
    
    class FunctionCollection;

    
    typedef boost::variant<
        boost::blank,
        shared_ptr<IType>,
        shared_ptr<ISymbolTable>,
        shared_ptr<IClass>,
        shared_ptr<VariableBase>,
        shared_ptr<Function>,
        shared_ptr<MemberFunctionAccessor>,
        shared_ptr<IConstant>,
        shared_ptr<IValue>,
        boost::recursive_wrapper<AddressList>,
        boost::recursive_wrapper<GenericAddress>,
        shared_ptr<FunctionCollection>
    >                                   Address;
    
    class AddressList : public std::list<Address>{};
    
    template <typename T>
    struct AddressId;
    
    template <>
    struct AddressId<blank>{
        static const size_t value = 0;
    };
    
    template <>
    struct AddressId<IType>{
        static const size_t value = 1;
    };
    
    template <>
    struct AddressId<ISymbolTable>{
        static const size_t value = 2;
    };
    
    template <>
    struct AddressId<IClass>{
        static const size_t value = 3;
    };
    
    template <>
    struct AddressId<VariableBase>{
        static const size_t value = 4;
    };
    
    template <>
    struct AddressId<Function>{
        static const size_t value = 5;
    };
    
    template <>
    struct AddressId<MemberFunctionAccessor>{
        static const size_t value = 6;
    };
    
    template <>
    struct AddressId<IConstant>{
        static const size_t value = 7;
    };
    
    template <>
    struct AddressId<IValue>{
        static const size_t value = 8;
    };
    
    template <>
    struct AddressId<AddressList>{
        static const size_t value = 9;
    };
    
    template<>
    struct AddressId<GenericAddress>{
        static const size_t value = 10;
    };
    
    template <>
    struct AddressId<FunctionCollection>{
        static const size_t value = 11;
    };
    
}}