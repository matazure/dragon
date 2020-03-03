#pragma once

#include <Dragon/Issue/base.hpp>

namespace Dragon{namespace Issue{
    
    
    class no_matched_function: public CompileError{
    public:
        
        virtual std::string message() const { return "no matched Function."; }
    };
    
    
}}