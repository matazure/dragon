#pragma once

#include <boost/spirit/include/qi_no_skip.hpp>
#include <Dragon/Grammar/ExpressionGrammar.hpp>
#include <Dragon/Grammar/DeclarationGrammar.hpp>

namespace Dragon{namespace Grammar{

    template <typename Iterator, typename Lexer>
    struct StatementGrammar : grammar< Iterator,
    AST::StatementList(),
    in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
        AST::StatementList(),
        in_state_skipper<typename Lexer::lexer_def>>        base_type;
        typedef typename Lexer::base_iterator_type                          base_iterator_type;
        typedef Iterator                                                    iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                 skipper_type;
        
        template <typename Token>
        StatementGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(statementList), expression(tok, eh), declaration(tok,eh){
            using boost::phoenix::function;
            typedef function<IssueHandler<base_iterator_type,Iterator>>
            issue_handler_function;
            typedef function<annotation<Iterator>>          annotation_function;
            
            statementList.name("statment list");
            statementList = +statement;
            
            codeBlock.name("code block");
            codeBlock = tok.l_braces > *statement > tok.r_braces;
            
            statement.name("statement");
            statement =
                (
                    declaration
                |   expression
                |   ifStatement
                |   whileStatement
                |   forStatement
                |   returnStatement
                |   breakStatement
                |   continueStatement
                |   codeBlock
                 ) > skip(tok.space)[tok.semicolon | tok.newline]
                ;
            
            ifStatement.name("if statement");
            ifStatement =
                    tok.if_
                >   omit[tok.l_bracket] > expression >omit[tok.r_bracket]
                >   codeBlock
                >   -(tok.else_ > codeBlock)
                ;
            
            whileStatement.name("while statement");
            whileStatement =
                    tok.while_
                >   omit[tok.l_bracket] > expression > tok.r_bracket
                >   codeBlock
                ;
            
            forStatement.name("for statement");
            forStatement =
                    tok.for_
                >   omit[tok.l_bracket]
                        > (declaration.variableDeclaration|expression.expressionList)
                        > tok.semicolon > expression
                        > tok.semicolon > expression
                >   tok.r_bracket
                >   codeBlock
                ;
            
            breakStatement.name("break statement");
            breakStatement = tok.break_ > tok.semicolon;
            
            continueStatement.name("continue statement");
            continueStatement = tok.continue_ > tok.semicolon;
            
            returnStatement.name("return statement");
            returnStatement = tok.return_ > -expression;
            
          
            statementSperator.name("statement sperator");
            statementSperator =  tok.newline ^ tok.semicolon;
            
            //for declaration grammar
            declaration.functionBody.name("function body");
            declaration.functionBody = codeBlock;
            
            on_error<fail>(statement, issue_handler_function(eh)(_1, _3, _4));
//            debug(statement);
        }
        
        ExpressionGrammar<Iterator, Lexer>              expression;
        DeclarationGrammar<Iterator, Lexer>             declaration;
    
        rule<Iterator, AST::StatementList(), skipper_type>          statementList;
        rule<Iterator, AST::CodeBlock(), skipper_type>              codeBlock;
        rule<Iterator, AST::Statement(), skipper_type>              statement;
   
        rule<Iterator, AST::BreakStatement(), skipper_type>         breakStatement;
        rule<Iterator, AST::ContinueStatement(), skipper_type>      continueStatement;
        rule<Iterator, AST::ReturnStatement(),skipper_type>         returnStatement;
        
        rule<Iterator, AST::IfStatement(), skipper_type>            ifStatement;
        rule<Iterator, AST::WhileStatement(), skipper_type>         whileStatement;
        rule<Iterator, AST::ForStatement(), skipper_type>           forStatement;
        
        rule<Iterator>    statementSperator;
        rule<Iterator>    spaceSkip;
    };
    
}}
