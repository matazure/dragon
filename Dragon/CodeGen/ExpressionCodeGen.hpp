#pragma once

#include <Dragon/CodeGen/Utilities.hpp>
#include <Dragon/CodeGen/LiteralCodeGen.hpp>

namespace Dragon{namespace CodeGen{
    
    class LiteralExpressionCodeGen{
    public:
        typedef IR::Address result_type;
        
        LiteralExpressionCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder),
            _spLiteralCodeGen(make_shared<LiteralCodeGen>(spBuilder->context()))
            {   }
        
        result_type operator()(const AST::LiteralExpression &ast){
            return apply_visitor(*this)(ast);
        }
        
        result_type operator()(const AST::Literal &ast){
            return (*_spLiteralCodeGen)(ast);
        }
        
        result_type operator()(const AST::ArrayLiteral &ast){
            //todo
        }
        
        result_type operator()(const AST::DictionaryLiteral &ast){
            
        }

        
    private:
        shared_ptr<Builder> _spBuilder;
        shared_ptr<LiteralCodeGen> _spLiteralCodeGen;
//        weak_ptr<ExpCg> _wpExpressionCodeGen;
    };
    
    template <typename ExpCg>
    class PrimaryExpressionCodeGen{
    public:
        typedef IR::Address result_type;
        
        PrimaryExpressionCodeGen(shared_ptr<Builder> spBuilder, shared_ptr<ExpCg> spExpressionCodeGen)
        :   _spBuilder(spBuilder),   
            _spLiteralExpressionCodeGen(make_shared<LiteralExpressionCodeGen>(spBuilder)),
            _wpExpressionCodeGen(spExpressionCodeGen)
        { }
        
        result_type operator()(const AST::PrimaryExpression &ast){
            return apply_visitor(*this, ast);
        }
        
        result_type operator()(const AST::Identifier &ast){
            auto addr = _spBuilder->get(ast);
            if (addr.which() == 0){
                assert(false);
            }else{
                return addr;
            }
//            auto addr = _sp_builder->get_address(x);
//            if (addr.which() == 0){
//                shared_ptr<issue::compile_issue> sp_error = make_shared<issue::undefined_identifier>(x);
//                sp_error->position_ids.push_back(x.position_id);
//                throw sp_error;
//            }else{
//                return addr;
//            }
        }
        
        result_type operator()(const AST::LiteralExpression &ast){
            return (*_spLiteralExpressionCodeGen)(ast);
        }
        
        result_type operator()(const AST::ExpressionList &ast){
            
        }
        
        result_type operator()(const AST::Expression &ast){
            return (*_wpExpressionCodeGen.lock())(ast);
        }
        
        
    private:
        shared_ptr<Builder> _spBuilder;
        shared_ptr<LiteralExpressionCodeGen> _spLiteralExpressionCodeGen;
        weak_ptr<ExpCg> _wpExpressionCodeGen;
    };
    
    class ExpressionCodeGen: public enable_shared_from_this<ExpressionCodeGen>{
    public:
        typedef PrimaryExpressionCodeGen<ExpressionCodeGen> PrimaryExpressionCodeGen;
        
        //temp
        typedef IR::Address result_type;
        
        ExpressionCodeGen(shared_ptr<Builder> spBuilder):   _spBuilder(spBuilder){}
        
        result_type operator()(const AST::Expression &ast){
            auto first = (*this)(ast.first);
            for (auto &operation: ast.operations){
                auto rhs = (*this)(operation.operand);
                first = binaryExpressionCodeGen(first, operation.operator_, rhs);
            }
            
            return first;
        }
        
        result_type operator()(const AST::BinaryOperand &ast){
            return apply_visitor(*this, ast);
        }
        
        result_type operator()(const AST::PrefixExpression &ast){
            auto operand = (*this)(ast.operand);
            for (auto operator_: ast.operators){
                operand = prefixExpressionCodeGen(operator_, operand);
            }
            
            return operand;
        }
        
        result_type operator()(const AST::PostfixExpression &ast){
            //todo
            
            return (*this)(ast.first);
        }
        
        result_type operator()(const AST::PrimaryExpression &ast){
            PrimaryExpressionCodeGen primaryExpressionCodeGen(_spBuilder, shared_from_this());
            return primaryExpressionCodeGen(ast);
        }
        
        optional<result_type> operator()(const optional<AST::Expression> &optAst){
            if (optAst){
                return (*this)(*optAst);
            }else{
                return optional<result_type>();
            }
        }
        
    private:
        
        result_type binaryExpressionCodeGen(const IR::Address &lhs, const AST::BinaryOperator &op, const IR::Address &rhs){
            auto rhsValue = getAddressRValue(rhs);
            auto lhsValue = getAddressRValue(lhs);
            if ((rhsValue->type()->isArithmetical() && lhsValue->type() == rhsValue->type())){
                auto bop = make_shared<ArithmeticalOperator>(op ,lhsValue, rhsValue, _spBuilder->currentBlock());
                return bop;
//                if (is_logical(x.operator_)){
//                    auto sp_logical_op = make_shared<logical_operator>(x.operator_, lhsValue, rhsValue, _sp_builder->current_block());
//                    return sp_logical_op;
//                }
//                if (is_comparison(x.operator_)){
//                    auto sp_comparison_op = make_shared<comparison_operator>(x.operator_, lhsValue, rhsValue, _sp_builder->current_block());
//                    return sp_comparison_op;
//                }
            }else{
                false;
            }
        }
        
        result_type prefixExpressionCodeGen(const AST::PrefixOperator &operator_, const Address &operand){
            //todo
            return operand;
        }
        
        
    private:
        shared_ptr<Builder> _spBuilder;
//        shared_ptr<PrimaryExpressionCodeGen>  _spPrimaryExpressionCodeGen;
    };
    
}}