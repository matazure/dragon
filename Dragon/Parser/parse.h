#pragma once

#include <Dragon/Parser/config.hpp>

namespace Dragon{
    
    bool parse(std::string code,  AST::StatementList &ast, issue_handler_type &eh);
    
}
