#pragma once

#include <Dragon/CodeGen/DeclaratonCodeGen.hpp>

namespace Dragon{namespace CodeGen{
 
    class StatementCodeGen{
    public:
        typedef void result_type;
        
        StatementCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder),
            _spExpressionCodeGen(make_shared<ExpressionCodeGen>(spBuilder)),
            _spDeclarationCodeGen(make_shared<DeclarationCodeGen>(spBuilder)){}
        
        result_type operator()(const AST::Statement &ast){
            apply_visitor(*this, ast);
        }
        
        result_type operator()(const AST::StatementList &ast){
            for (auto stat: ast){
                (*this)(stat);
            }
        }
        
        result_type operator()(const AST::CodeBlock &ast){
//            for (size_t i = 0; i < ast.size(); ++i){
//                if (_spBuilder->unterminated()){
//                    (*this)(ast[i]);
//                }else{
//                    //ErrorIssue
//                }
//            }
        }
        
        result_type operator()(const AST::Expression &ast){
            (*_spExpressionCodeGen)(ast);
        }
        
        result_type operator()(const AST::Declaration &ast){
            if (_spBuilder->inFunctionScope()){
                    (*_spDeclarationCodeGen)(ast);
            }else{
                auto todo = _spBuilder->defineCommand();
                (*_spDeclarationCodeGen)(ast);
            }

        }
        
        result_type operator()(const AST::ReturnStatement &ast){
//            auto spReturnType = _spBuilder->currentFunction()->FunctionType()->returnType();
//            
//            if (ast.optValue){
//                auto spReturnValue = _spExpressionCodeGen->rvalue(*ast.optValue);
//                if (spReturnType != spReturnValue->type()){
//                    //ErrorIssue
//                    assert(false);
//                }
//            }else{
//                if (spReturnType != _spBuilder->context()->getType("void")){
//                    //ErrorIssue
//                    assert(false);
//                }
//                _spBuilder->createReturnVoid();
//            }
        }
        
        result_type operator()(const AST::ContinueStatement &ast){
            _spBuilder->createContinue();
        }
        
        result_type operator()(const AST::BreakStatement &ast){
            _spBuilder->createBreak();
        }
        
        result_type operator()(const AST::IfStatement &ast){
//            auto spCondition = _spExpressionCodeGen->rvalue(ast.condition);
//            if (spCondition->type() != _spBuilder->getType("bool")){
//                //ErrorIssue
//                assert(false);
//            }
//            
//            auto spThenBlock = _spBuilder->createBasicBlock();
//            auto spElseBlock = _spBuilder->createBasicBlock();
//            shared_ptr<IR::BasicBlock> spMergeBlock(nullptr);
//            _spBuilder->createConditionBranch(spThenBlock, spElseBlock, spCondition);
//            
//            _spBuilder->currentBlock(spThenBlock);
//            (*this)(ast.then);
//            if (!_spBuilder->isTerminated()){
//                spMergeBlock = make_shared<IR::BasicBlock(_spBuilder->currentFunction());
//                _spBuilder->createBranch(spMergeBlock);
//            }
//    
//            if (x.optElse){
//                _spBuilder->currentBlock(spElseBlock);
//                (*this)(*x.optElse);
//            }else{
//                if (!_spBuilder->isTerminated()){
//                    if (!spMergeBlock)
//                        spMergeBlock = make_shared<IR::BasicBlock>(_spBuilder->currentFuncton());
//                    _spBuilder->createBranch(spMergeBlock);
//                }
//            }
//
//            if (spMergeBlock){
//                _spBuilder->currentBlock(spMergeBlock);
//            }
        }
        
        result_type operator()(const AST::WhileStatement &ast){
            
        }
        
        result_type operator()(const AST::ForStatement &ast){
            
        }
        
    private:
        shared_ptr<Builder>                     _spBuilder;
        shared_ptr<ExpressionCodeGen>           _spExpressionCodeGen;
        shared_ptr<DeclarationCodeGen>          _spDeclarationCodeGen;
    };
    
}}