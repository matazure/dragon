#pragma once

#include <Dragon/Issue/base.hpp>

namespace Dragon{namespace Issue{
    
    class invalid_assigment: public CompileError{
    public:
        
        invalid_assigment(){}
        
        virtual std::string message() const{
            return "todo";
        }
    };
    
    
    


}}
