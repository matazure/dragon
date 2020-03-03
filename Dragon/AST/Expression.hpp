#pragma once

#include <Dragon/AST/Literal.hpp>
#include <Dragon/AST/Identifier.hpp>

namespace Dragon{namespace AST{
        
//    struct AssignExpression;
//    typedef AssignExpression Expression;
    
    struct Expression;
    struct ExpressionList;
    
    struct ArrayLiteral;
    struct DictionaryLiteral;
    typedef variant<
                Literal
            ,   recursive_wrapper<ArrayLiteral>
            ,   recursive_wrapper<DictionaryLiteral>
            >                                               LiteralExpression;
    
    typedef variant<Identifier,
                    LiteralExpression,
                    recursive_wrapper<Expression>
            ,       recursive_wrapper<ExpressionList>>      PrimaryExpression;
    
    struct Operator: Tag, string{
        Operator(): string(){}
        Operator(const string &s): string(s){}
        Operator(const char *s): string(s){}
        
        template<typename Iter>
        Operator(Iter begin, Iter end): string(begin, end){}
    };
    
    struct PostfixUnaryOperator: Operator{
        PostfixUnaryOperator(): Operator(){}
        PostfixUnaryOperator(const string &s): Operator(s){}
        PostfixUnaryOperator(const char *s): Operator(s){}
        
        template<typename Iter>
        PostfixUnaryOperator(Iter begin, Iter end): Operator(begin, end){}
    };
    
    struct PostfixOperator: Operator{
        PostfixOperator(): Operator(){}
        PostfixOperator(const string &s): Operator(s){}
        PostfixOperator(const char *s): Operator(s){}
        
        template<typename Iter>
        PostfixOperator(Iter begin, Iter end): Operator(begin, end){}
    };
    
    struct PostfixOperation: Tag{
        PostfixOperator     operator_;
        PrimaryExpression   operand;
    };
    
    struct PostfixExpression: Tag{
        PrimaryExpression                   first;
        vector<PostfixOperation>            operations;
    };
    
    struct PrefixOperator: Operator{
        PrefixOperator(): Operator(){}
        PrefixOperator(const string &s): Operator(s){}
        PrefixOperator(const char *s): Operator(s){}
        
        template<typename Iter>
        PrefixOperator(Iter begin, Iter end): string(begin, end){}
    };
    
    struct PrefixExpression: Tag{
        vector<PrefixOperator>          operators;
        PostfixExpression               operand;
    };
    
    struct BinaryOperator: Operator{
        BinaryOperator() : Operator(){}
        BinaryOperator(const string &s): Operator(s){}
        BinaryOperator(const char *s): Operator(s){}
        
        template<typename Iter>
        BinaryOperator(Iter begin, Iter end): Operator(begin, end){}
    };
    
    typedef variant<PrefixExpression, recursive_wrapper<Expression>> BinaryOperand;
    
    struct BinaryOperation{
        BinaryOperator          operator_;
        BinaryOperand           operand;
    };
    
    struct Expression{
        BinaryOperand                 first;
        vector<BinaryOperation>   operations;
    };
    
//    struct MultiplicativeExpression: Tag{
//        typedef PrefixExpression                                            PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
//    
//    struct AdditiveExpression: Tag{
//        typedef MultiplicativeExpression                                    PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
//    
//    struct RelationExpression: Tag{
//        typedef AdditiveExpression                                          PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
//    
//    struct EqualityExpression: Tag{
//        typedef RelationExpression                                          PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
//    
//    struct LogicalExpression: Tag{
//        typedef EqualityExpression                                          PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
//    
//    struct AssignExpression: Tag{
//        typedef LogicalExpression                                           PreExpression;
//        struct Operation{
//            BinaryOperator   operator_;
//            PreExpression    BinaryOperand;
//        };
//        typedef vector<Operation>                                             OperationList;
//        
//        PreExpression                                                       first;
//        OperationList                                                       operations;
//    };
    
    struct ExpressionList : Tag, vector<Expression>{};
    
    struct ArrayLiteral: Tag, vector<Expression>{   };
    
    struct DictionaryLiteralItem: Tag{
        Expression  key;
        Expression  value;
    };
    
    struct DictionaryLiteral: Tag, vector<DictionaryLiteralItem>{};
    
    
}}

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::PostfixOperation,
                          (Dragon::AST::PostfixOperator, operator_)
                          (Dragon::AST::PrimaryExpression, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
                           Dragon::AST::PostfixExpression,
                           (Dragon::AST::PrimaryExpression, first)
                          (std::vector<Dragon::AST::PostfixOperation>, operations)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::PrefixExpression,
                          (std::vector<Dragon::AST::PrefixOperator>, operators)
                          (Dragon::AST::PostfixExpression, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::BinaryOperation,
                          (Dragon::AST::BinaryOperator, operator_)
                          (Dragon::AST::BinaryOperand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::Expression,
                          (Dragon::AST::BinaryOperand, first)
                          (std::vector<Dragon::AST::BinaryOperation>, operations)
)

//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::MultiplicativeExpression,
//                          (Dragon::AST::MultiplicativeExpression::PreExpression, first)
//                          (Dragon::AST::MultiplicativeExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::MultiplicativeExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::MultiplicativeExpression::PreExpression, BinaryOperand)
//                          )
//
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::AdditiveExpression,
//                          (Dragon::AST::AdditiveExpression::PreExpression, first)
//                          (Dragon::AST::AdditiveExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::AdditiveExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::AdditiveExpression::PreExpression, BinaryOperand)
//                          )
//
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::RelationExpression,
//                          (Dragon::AST::RelationExpression::PreExpression, first)
//                          (Dragon::AST::RelationExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::RelationExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::RelationExpression::PreExpression, BinaryOperand)
//                          )
//
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::EqualityExpression,
//                          (Dragon::AST::EqualityExpression::PreExpression, first)
//                          (Dragon::AST::EqualityExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::EqualityExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::EqualityExpression::PreExpression, BinaryOperand)
//                          )
//
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::LogicalExpression,
//                          (Dragon::AST::LogicalExpression::PreExpression, first)
//                          (Dragon::AST::LogicalExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::LogicalExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::LogicalExpression::PreExpression, BinaryOperand)
//                          )
//
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::AssignExpression,
//                          (Dragon::AST::AssignExpression::PreExpression, first)
//                          (Dragon::AST::AssignExpression::OperationList, operations)
//                          )
//BOOST_FUSION_ADAPT_STRUCT(
//                          Dragon::AST::AssignExpression::Operation,
//                          (Dragon::AST::BinaryOperator, operator_)
//                          (Dragon::AST::AssignExpression::PreExpression, BinaryOperand)
//                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::DictionaryLiteralItem,
                          (Dragon::AST::Expression, key)
                          (Dragon::AST::Expression, value)
)



