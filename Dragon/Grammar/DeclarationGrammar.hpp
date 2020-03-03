#pragma once


#include <Dragon/Grammar/TypeGrammar.hpp>
#include <Dragon/Grammar/PatternGrammar.hpp>


namespace Dragon{namespace Grammar{

    template <typename Iterator, typename Lexer>
    struct DeclarationGrammar: grammar< Iterator,
    AST::Declaration(),
    in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
                            AST::Declaration(),
                            in_state_skipper<typename Lexer::lexer_def>>    base_type;
        typedef typename Lexer::base_iterator_type                          base_iterator_type;
        typedef Iterator                                                    iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                 skipper_type;

        template <typename Token>
        DeclarationGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(declaration), pattern(tok, eh), expression(tok,eh), type(tok,eh){
            using boost::phoenix::function;
            typedef function<IssueHandler<base_iterator_type,Iterator>>
            issue_handler_function;
            typedef function<annotation<Iterator>>          annotation_function;

            identifier = tok.identifier;

            declaration =
                    variableDeclaration
                |   functionDeclaration
                |   constantDeclaration
                ;

            declarationSpecifiers = *tok.declarationSpecifier;

            declarationHeader = declarationSpecifiers;

            variableDeclaration.name("variable declaration");
            variableDeclaration =
                    declarationHeader
                >>  tok.var
                >   (+patternInitializer)
                ;

            constantDeclaration.name("constant declaration");
            constantDeclaration =
                    declarationHeader
                >>  tok.let
                >   (+patternInitializer)
                ;


            patternInitializer = pattern > omit[tok.assign] > (-expression);


            functionDeclaration =
                    declarationHeader
                >>  tok.function
                >   functionName
                >   functionSignature
                >   functionBody
                ;

            functionName = identifier | operator_;


            functionSignature =
                    functionParameterClause
                >   functionResult
                ;

            functionParameterClause =
                    omit[tok.l_bracket]
                >   -(functionParameter % tok.comma)
                >   tok.r_bracket
                ;

            functionParameter =
                    identifier
                >   type.typeAnnotation
                >   (-expression)
                ;

            functionResult = tok.arrow > type;

//            constantDeclaration =

//            on_error<fail>(statement_list, issue_handler_function(eh)(_1, _3, _4));
//
//            on_success(identifier, annotation_function(eh.sp_iters)(_val, _1, _2));
//            on_success(statement_, annotation_function(eh.sp_iters)(_val, _1, _2));

        }

        PatternGrammar<Iterator, Lexer>                                    pattern;
        ExpressionGrammar<Iterator, Lexer>                                 expression;
        TypeGrammar<Iterator, Lexer>                                       type;

        rule<Iterator, AST::Identifier(), skipper_type>                    identifier;
        rule<Iterator, AST::Declaration(), skipper_type>                   declaration;
        rule<Iterator, AST::DeclarationSpecifiers(), skipper_type>         declarationSpecifiers;
        rule<Iterator, AST::DeclarationHeader(),skipper_type>              declarationHeader;

        rule<Iterator, AST::ConstantDeclaration(),skipper_type>            constantDeclaration;
        rule<Iterator, AST::VariableDeclaration(), skipper_type>           variableDeclaration;
        rule<Iterator, AST::PatternInitializer(), skipper_type>            patternInitializer;

        rule<Iterator, AST::FunctionDeclaration(), skipper_type>                    functionDeclaration;
        rule<Iterator, AST::FunctionDeclaration::Name(), skipper_type>              functionName;
        rule<Iterator, AST::FunctionDeclaration::Signature(), skipper_type>         functionSignature;
        rule<Iterator, AST::CodeBlock(), skipper_type>                              functionBody;
        rule<Iterator, AST::FunctionDeclaration::Parameter(), skipper_type>         functionParameter;
        rule<Iterator, AST::FunctionDeclaration::ParameterClause(), skipper_type>   functionParameterClause;
        rule<Iterator, AST::FunctionDeclaration::Result(), skipper_type>            functionResult;
        rule<Iterator, AST::Operator(), skipper_type>                               operator_;

    };

}}
