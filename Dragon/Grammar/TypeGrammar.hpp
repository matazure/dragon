#pragma once


#include <boost/spirit/include/qi.hpp>
#include <Dragon/AST/Type.hpp>
#include <Dragon/Issue/handler.hpp>


namespace Dragon{namespace Grammar{
    
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    
    using namespace boost::spirit::qi;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::_2;
    using boost::spirit::qi::_3;
    using boost::spirit::qi::_4;
    using boost::spirit::qi::_val;
    
    using Dragon::IssueHandler;
    using Dragon::annotation;
    
    
    template <typename Iterator,typename Lexer>
    struct TypeGrammar: grammar< Iterator,
    AST::Type(),
    qi::in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
        AST::Type(),
        in_state_skipper<typename Lexer::lexer_def>>                            base_type;
        typedef typename Lexer::base_iterator_type                              base_iterator_type;
        typedef Iterator                                                        iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                     skipper_type;
        
        template <typename Token>
        TypeGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(type,"type")
        {
            namespace phoenix = boost::phoenix;
            typedef phoenix::function<IssueHandler<base_iterator_type, Iterator> &> issue_handler_function;
            typedef phoenix::function<annotation<Iterator> > annotation_function;
            
            typeAnnotation.name("type annotation");
            typeAnnotation = tok.colon > type;
            
            type.name("type");
            type =
                    type_identifier
                |   arrayType
                |   functionType
                |   tupleType;
                ;
            
            type_identifier = tok.identifier;
            arrayType = omit[tok.l_s_bracket] > type > tok.r_s_bracket;
            functionType = omit[tok.l_bracket] > type > tok.r_bracket > tok.arrow > type;
            
            tupleType = omit[tok.l_bracket] > (tupleTypeElement % tok.comma) > tok.r_bracket;
            tupleTypeElement = type;
        }
        
        rule<Iterator, AST::Type(), skipper_type>                               type;
        rule<Iterator, AST::TypeAnnotation(), skipper_type>                     typeAnnotation;
        rule<Iterator, AST::IdentifierType(), skipper_type>                     type_identifier;
        
        rule<Iterator, AST::ArrayType(), skipper_type>                          arrayType;
        rule<Iterator, AST::FunctionType(), skipper_type>                       functionType;
        
        rule<Iterator, AST::TupleType(), skipper_type>                          tupleType;
        rule<Iterator, AST::TupleTypeElement(), skipper_type>                   tupleTypeElement;
    };
    
    
}}
