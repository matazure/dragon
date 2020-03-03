#pragma once

#include <Dragon/AST/Tag.hpp>

namespace Dragon{namespace AST{
    
    struct Identifier : string, Tag{
        Identifier(): string() {}
        
        Identifier(const string &s): string(s){}
        Identifier(const char *s) : string(s){}
        
        template <typename Iter>
        Identifier(Iter it_begin, Iter it_end): std::string(it_begin, it_end){}
    };
    
    
}}