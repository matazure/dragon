#pragma once

#include <Dragon/AST/Identifier.hpp>
#include <Dragon/IR/Interface.hpp>

namespace Dragon{namespace IR{
    

    struct IdentifierPattern{
        AST::Identifier                     identifier;
        optional<shared_ptr<IType>>         optSpTypeAnnotation;
    };
    
    
    typedef variant<IdentifierPattern>          Pattern;
    
    
}}