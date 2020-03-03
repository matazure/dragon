//
//  dragon_parser.cpp
//  binding
//
//  Created by 张志敏 on 11/13/13.
//
//
#include <Dragon/Parser/parse.h>
#include <Dragon/Grammar/StatementGrammar.hpp>

namespace Dragon{
    bool parse(std::string code,  AST::StatementList &ast, issue_handler_type &eh){
        
        code_tokens_type tokens;
        
        boost::spirit::classic::position_iterator2<std::string::iterator> first(code.begin(), code.end());
        base_iterator_type  last;
        
        iterator_type iter = tokens.begin(first, last);
        iterator_type end = tokens.end();
        
        Grammar::StatementGrammar<iterator_type, code_tokens_type> program(tokens, eh);
        auto t = boost::spirit::qi::in_state("WS")[tokens.self];
        
        return boost::spirit::qi::phrase_parse(iter, end, program, t, boost::spirit::qi::skip_flag::dont_postskip, ast);
    }
}


