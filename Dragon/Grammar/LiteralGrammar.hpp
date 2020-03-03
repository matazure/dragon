#pragma once

#include <boost/spirit/include/qi.hpp>
#include <Dragon/AST/Literal.hpp>
#include <Dragon/Issue/handler.hpp>

namespace Dragon{namespace Grammar{
    
    using Dragon::IssueHandler;
    using Dragon::annotation;
    
    template <typename Iterator, typename Lexer>
    struct LiteralGrammar : grammar< Iterator,
    AST::Literal(),
    in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
        AST::Literal(),
        in_state_skipper<typename Lexer::lexer_def>>        base_type;
        typedef typename Lexer::base_iterator_type                          base_iterator_type;
        typedef Iterator                                                    iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                 skipper_type;
        
        template <typename Token>
        LiteralGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(literal){
            
            using boost::phoenix::function;
            typedef function<IssueHandler<base_iterator_type,Iterator>>
            issue_handler_function;
            typedef function<annotation<Iterator>>          annotation_function;
            
            literal =
                    integerLiteral
                |   floatingPointLiteral
                |   stringLiteral

                ;
            
            integerLiteral = tok.int_;
            floatingPointLiteral = tok.float_;
            stringLiteral = tok.string_;
            
   
        }
        
        rule<Iterator, AST::Literal(), skipper_type>                    literal;
        rule<Iterator, AST::IntegerLiteral(), skipper_type>             integerLiteral;
        rule<Iterator, AST::FloatingPointLiteral(), skipper_type>       floatingPointLiteral;
        rule<Iterator, AST::StringLiteral(), skipper_type>              stringLiteral;
    };
    
    
    
}}
