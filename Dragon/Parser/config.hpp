#pragma once

#include <boost/spirit/home/classic/iterator/position_iterator.hpp>
#include <Dragon/Lexer/lexer.hpp>
#include <Dragon/Issue/handler.hpp>
#include <Dragon/AST/Statement.hpp>

namespace Dragon{
    namespace lex = boost::spirit::lex;
    namespace spirit = boost::spirit;
    typedef spirit::classic::position_iterator2<std::string::iterator> base_iterator_type;
    
    typedef lex::lexertl::position_token<
    base_iterator_type,
    boost::mpl::vector<
    std::string,
    AST::StringLiteral,
    AST::IntegerLiteral,
    AST::FloatingPointLiteral,
    AST::Identifier,
    AST::DeclarationSpecifier
    
    ////    AST::Bool
    //                    ast::optoken,
    //                    AST::Identifier,
    //                    ast::declarationSpecifier,
    >>                                      token_type;
    
    typedef lex::lexertl::lexer<token_type>                 lexer_type;
    typedef Lexer::CodeTokens<lexer_type>                   code_tokens_type;
    typedef code_tokens_type::iterator_type                 iterator_type;
    
    
    typedef Dragon::IssueHandler<base_iterator_type, iterator_type> issue_handler_type;
}