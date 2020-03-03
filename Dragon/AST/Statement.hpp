#pragma once

#include <Dragon/AST/Declaration.hpp>

namespace Dragon{namespace AST{

    struct IfStatement;
    struct WhileStatement;
    struct ForStatement;
    struct StatementList;
    
    struct BreakStatement: Tag{};
    struct ContinueStatement: Tag{};
    
    struct ReturnStatement: Tag{
        optional<Expression>            optValue;
    };

    typedef variant<Expression,
                    Declaration,
                    ReturnStatement,
                    ContinueStatement,
                    BreakStatement,
                    recursive_wrapper<StatementList>,
                    recursive_wrapper<CodeBlock>,
                    recursive_wrapper<IfStatement>,
                    recursive_wrapper<WhileStatement>,
                    recursive_wrapper<ForStatement>>                  Statement;
    
    struct StatementList: Tag, vector<Statement>{};
    
    struct CodeBlock: Tag, vector<Statement>{};
    
    struct IfStatement: Tag{
        Expression              condition;
        CodeBlock               then;
        optional<CodeBlock>     optElse;
    };
    
    struct WhileStatement: Tag{
        Expression              condition;
        CodeBlock               body;
    };
    
    struct ForStatement: Tag{
        typedef variant<VariableDeclaration, ExpressionList> ForInit;
        optional<ForInit>               optForInit;
        optional<Expression>            optCondition;
        optional<Expression>            optPost;
        CodeBlock                       body;
    };
    
}}

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::ReturnStatement,
                          (optional<Dragon::AST::Expression>, optValue)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::IfStatement,
                          (Dragon::AST::Expression, condition)
                          (Dragon::AST::CodeBlock, then)
                          (optional<Dragon::AST::CodeBlock>, optElse)
                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::WhileStatement,
                          (Dragon::AST::Expression, condition)
                          (Dragon::AST::CodeBlock, body)
                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::ForStatement,
                          (optional<Dragon::AST::ForStatement::ForInit>, optForInit)
                          (optional<Dragon::AST::Expression>, optCondition)
                          (optional<Dragon::AST::Expression>, optPost)
                          (Dragon::AST::CodeBlock,body)
                          )