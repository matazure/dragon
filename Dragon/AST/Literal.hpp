#pragma once

#include <Dragon/AST/Tag.hpp>

namespace Dragon{namespace AST{
  
    
    struct IntegerLiteral : Tag{
        int         value;
    };
    
    struct FloatingPointLiteral: Tag{
        double         value;
    };
    
    struct StringLiteral : Tag{
        string         value;
    };
    
    
    typedef variant<
        IntegerLiteral
    ,   FloatingPointLiteral
    ,   StringLiteral
    >                                               Literal;
    
    
}}