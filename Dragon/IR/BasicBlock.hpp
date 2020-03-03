//
//  BasicBlock.hpp
//  binding
//
//  Created by 张志敏 on 12/11/13.
//
//

#pragma once


#include <llvm/IR/BasicBlock.h>
#include <Dragon/IR/Function.hpp>
#include <Dragon/IR/SymbolTable.hpp>

namespace Dragon{namespace IR{
    
    class BasicBlock: public IBasicBlock{
    public:
        BasicBlock(shared_ptr<IFunction> sp_parent, shared_ptr<IBasicBlock> sp_insert_before)
        : _sp_parent(sp_parent){
            assert(sp_parent);
            assert(sp_insert_before);
            _p_llvm_block = llvm::BasicBlock::Create(sp_parent->type()->context()->llvmContext(), "", sp_parent->llvm_function(), sp_insert_before->llvmBasicBlock());
        }
        
        BasicBlock(shared_ptr<IFunction> sp_parent)
        : _sp_parent(sp_parent){
            assert(sp_parent);
            _p_llvm_block = llvm::BasicBlock::Create(sp_parent->type()->context()->llvmContext(), "", sp_parent->llvm_function(), nullptr);
        }
        
        virtual llvm::BasicBlock *llvmBasicBlock() const{
            return _p_llvm_block;
        }
        
        virtual shared_ptr<IFunction> parent() const{
            return _sp_parent;
        }
        
        virtual shared_ptr<IType> type() const              { assert(false); }
        virtual llvm::Value *llvmValue() const                   { return llvmBasicBlock(); }
        
    private:
        llvm::BasicBlock *    _p_llvm_block;
        shared_ptr<IFunction>   _sp_parent;
    };
    
    
}}