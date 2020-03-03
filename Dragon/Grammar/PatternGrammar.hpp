#pragma once


#include <Dragon/Grammar/TypeGrammar.hpp>


namespace Dragon{namespace Grammar{
    
    template <typename Iterator, typename Lexer>
    struct PatternGrammar: grammar< Iterator,
    AST::Pattern(),
    in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
        AST::Pattern(),
        in_state_skipper<typename Lexer::lexer_def>>    base_type;
        typedef typename Lexer::base_iterator_type                          base_iterator_type;
        typedef Iterator                                                    iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                 skipper_type;
        
        template <typename Token>
        PatternGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(pattern), type(tok, eh){
            using boost::phoenix::function;
            typedef function<IssueHandler<base_iterator_type,Iterator>>
            issue_handler_function;
            typedef function<annotation<Iterator>>          annotation_function;
            
            identifier = tok.identifier;
            pattern = identifierPattern;
            
            identifierPattern = identifier >> (-type.typeAnnotation);
            
        }
        
        TypeGrammar<Iterator, Lexer>                                            type;
        
        rule<Iterator, AST::Pattern(), skipper_type>                            pattern;
        rule<Iterator, AST::Identifier(), skipper_type>                         identifier;
        rule<Iterator, AST::IdentifierPattern(), skipper_type>                  identifierPattern;
        
    };
    
}}