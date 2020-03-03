#pragma once

#include <Dragon/Grammar/TypeGrammar.hpp>
#include <Dragon/Grammar/LiteralGrammar.hpp>

namespace Dragon{namespace Grammar{
    
    namespace qi = boost::spirit::qi;
    namespace ascii = boost::spirit::ascii;
    
    using namespace boost::spirit::qi;
    using boost::spirit::qi::_1;
    using boost::spirit::qi::_2;
    using boost::spirit::qi::_3;
    using boost::spirit::qi::_4;
    using boost::spirit::qi::_val;
    
    
    template <typename Iterator,typename Lexer>
    struct ExpressionGrammar: grammar< Iterator,
    AST::Expression(),
    qi::in_state_skipper<typename Lexer::lexer_def>>
    {
        typedef grammar<Iterator,
        AST::Expression(),
        in_state_skipper<typename Lexer::lexer_def>>                            base_type;
        typedef typename Lexer::base_iterator_type                              base_iterator_type;
        typedef Iterator                                                        iterator_type;
        typedef in_state_skipper<typename Lexer::lexer_def>                     skipper_type;
        
        template <typename Token>
        ExpressionGrammar(const Token &tok, IssueHandler<base_iterator_type, Iterator>& eh)
        : base_type(expression,"leftExpressionession"), type(tok, eh), literal(tok,eh)
        {
            namespace phoenix = boost::phoenix;
            typedef phoenix::function<IssueHandler<base_iterator_type, Iterator> &> issue_handler_function;
            typedef phoenix::function<annotation<Iterator> > annotation_function;
            
            identifier = tok.identifier;

            expression.name("leftExpressionession");
            expression =  assignExpression.alias();
    
            assignOperator = tok.assign;
            assignExpression.name("assign leftExpressionession");
            assignExpression =
                    logicalExpression
                >>  *(assignOperator > logicalExpression)
                ;

            logicalOperator = tok.and_ | tok.or_;
            logicalExpression.name("logical leftExpressionession");
            logicalExpression =
                equalityExpression
                >> *(logicalOperator > equalityExpression)
                ;
            
            equalityOperator = tok.equal | tok.not_equal;
            equalityExpression.name("equality leftExpressionession");
            equalityExpression =
                relationalExpression
                >> *(equalityOperator > relationalExpression)
                ;
            
            relationalOperator = tok.less | tok.less_or_equal | tok.greater | tok.greater_or_equal;
            relationalExpression.name("relational leftExpressionession");
            relationalExpression =
                additiveExpression
                >> *(relationalOperator > additiveExpression)
                ;

            additiveOperator = tok.plus|tok.minus;
            additiveExpression.name("additive leftExpressionession");
            additiveExpression =
                    multiplicativeExpression
                >> *(additiveOperator > multiplicativeExpression)
                ;
            
            multiplicativeOperator = tok.times | tok.divide | tok.remain;
            multiplicativeExpression.name("multiplicative leftExpressionession");
            multiplicativeExpression =
                prefixExpression
                >> *(multiplicativeOperator > prefixExpression)
                ;
            
            prefixOperator = tok.increment | tok.decrement | tok.plus | tok.minus | tok.not_ | tok.ref;
            prefixExpression.name("unary leftExpressionession");
            prefixExpression = *prefixOperator >> postfixExpression;
            
            
            postfixExpression.name("postfix expresion");
            postfixExpression =
                primaryExpression
                >>  *((tok.l_bracket > expressionList > omit[tok.r_bracket])
                      | (tok.l_s_bracket > expressionList > omit[tok.r_s_bracket])
                      | (tok.period > identifier))
                ;
            
            primaryExpression.name("primary leftExpressionession");
            primaryExpression =
                    identifier
                |   literalExpression
                |   parenthesizedExpression
                ;
            
            parenthesizedExpression = omit[tok.l_bracket] > expression >tok.r_bracket;
            
            literalExpression =
                    literal
                |   arrayLiteral
                |   dictionaryLiteral
                ;
            
            arrayLiteral =
                    omit[tok.l_s_bracket]
                >> (-expression%tok.comma)
                > tok.r_s_bracket;
            
            dictionaryLiteral =
                    omit[tok.l_s_bracket]
                >>  (-dictionaryLiteralItem%tok.comma)
                > tok.r_s_bracket;
            dictionaryLiteralItem = expression >> tok.colon >> expression;
            
            expressionList = -(expression % tok.comma);
            
            on_error<fail>(expression, issue_handler_function(eh)(_1, _3, _4));
            
            on_success(assignOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(logicalOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(equalityOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(relationalOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(additiveOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(multiplicativeOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(superscriptOperator, annotation_function(eh.sp_iters)(_val, _1, _2));
            
            on_success(expression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(assignExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(logicalExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(equalityExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(relationalExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(additiveExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(multiplicativeExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(prefixExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(postfixExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(primaryExpression, annotation_function(eh.sp_iters)(_val, _1, _2));
            on_success(expressionList, annotation_function(eh.sp_iters)(_val, _1, _2));
        }
        
        Grammar::TypeGrammar<Iterator, Lexer>                       type;
        Grammar::LiteralGrammar<Iterator, Lexer>                    literal;
        rule<Iterator, AST::Identifier(), skipper_type>             identifier;
        
        rule<Iterator, AST::Expression(), skipper_type>             expression;
        rule<Iterator, AST::ExpressionList(), skipper_type>         expressionList;
        
        rule<Iterator, AST::Operator(), skipper_type>               assignOperator;
        rule<Iterator, AST::Operator(), skipper_type>               logicalOperator;
        rule<Iterator, AST::Operator(), skipper_type>               equalityOperator;
        rule<Iterator, AST::Operator(), skipper_type>               relationalOperator;
        rule<Iterator, AST::Operator(), skipper_type>               additiveOperator;
        rule<Iterator, AST::Operator(), skipper_type>               multiplicativeOperator;
        rule<Iterator, AST::Operator(), skipper_type>               prefixOperator;
        rule<Iterator, AST::Operator(), skipper_type>               superscriptOperator;
        
        rule<Iterator, AST::Expression(), skipper_type>             assignExpression;
        rule<Iterator, AST::Expression(), skipper_type>             logicalExpression;
        rule<Iterator, AST::Expression(), skipper_type>             equalityExpression;
        rule<Iterator, AST::Expression(), skipper_type>             relationalExpression;
        rule<Iterator, AST::Expression(), skipper_type>             additiveExpression;
        rule<Iterator, AST::Expression(), skipper_type>             multiplicativeExpression;
        
        rule<Iterator, AST::PrefixExpression(), skipper_type>       prefixExpression;
        rule<Iterator, AST::PostfixExpression(), skipper_type>      postfixExpression;
        
        rule<Iterator, AST::PrimaryExpression(), skipper_type>      primaryExpression;
        rule<Iterator, AST::Expression(), skipper_type>             parenthesizedExpression;
        
        rule<Iterator, AST::LiteralExpression(), skipper_type>      literalExpression;
        rule<Iterator, AST::ArrayLiteral(), skipper_type>           arrayLiteral;
        rule<Iterator, AST::DictionaryLiteral(), skipper_type>      dictionaryLiteral;
        rule<Iterator, AST::DictionaryLiteralItem(), skipper_type>  dictionaryLiteralItem;
    };
    
}}