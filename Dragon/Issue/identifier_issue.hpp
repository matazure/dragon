#pragma once

#include <Dragon/AST/Statement.hpp>

namespace Dragon{namespace Issue{
    
    class  undefined_identifier : public CompileError{
    public:
        undefined_identifier(const AST::Identifier &id_)
        : id(id_){}
        
        virtual std::string message() const{
            return "undefined "+id;
        }
        
        AST::Identifier id;
    };
    
    
    class redefined_identifier : public CompileError{
    public:
        redefined_identifier(const AST::Identifier &id_)
        : id(id_){}
        
        virtual std::string message() const{
            return "redefined " +id;
        }
        
        AST::Identifier id;
    };
    
}}