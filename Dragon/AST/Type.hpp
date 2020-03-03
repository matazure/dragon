#pragma once

#include <dragon/ast/tag.hpp>
#include <dragon/ast/identifier.hpp>
#include <dragon/ast/attribute.hpp>

namespace Dragon{namespace AST{
    
    
    typedef Identifier IdentifierType;
    
    struct ArrayType;
    struct FunctionType;
    struct TupleType;
    
    typedef variant<IdentifierType,
                    recursive_wrapper<ArrayType>,
                    recursive_wrapper<FunctionType>,
                    recursive_wrapper<TupleType>>     Type;
    
    struct ArrayType: Tag{
        Type                type;
    };
    
    struct FunctionType: Tag{
        Type                argumentType;
        Type                returnType;
    };
    
    typedef Type            TypeAnnotation;
    
    typedef Type TupleTypeElement;
    
    struct TupleType: vector<TupleTypeElement>, Tag{

    };
    
}}

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::ArrayType,
                          (Dragon::AST::Type, type)
                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::FunctionType,
                          (Dragon::AST::Type, argumentType)
                          (Dragon::AST::Type, returnType)
                          )


