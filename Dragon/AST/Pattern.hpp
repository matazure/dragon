#pragma once



#include <Dragon/AST/Type.hpp>


namespace Dragon{namespace AST{
    
    struct IdentifierPattern{
        Identifier                              identifier;
        optional<TypeAnnotation>                optTypeAnnotation;
    };
    
    typedef variant<IdentifierPattern>          Pattern;
    
    
}}

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::IdentifierPattern,
                          (Dragon::AST::Identifier, identifier)
                          (boost::optional<Dragon::AST::TypeAnnotation>, optTypeAnnotation)
)