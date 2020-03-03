//
//  GlobalVariable.h
//  binding
//
//  Created by 张志敏 on 2/9/14.
//
//

#pragma once

#include <llvm/IR/GlobalVariable.h>
#include <Dragon/IR/DerivedType.hpp>

namespace Dragon{namespace IR{
   
    class GlobalVariable : public VariableBase, public IValue{
    public:
        GlobalVariable(shared_ptr<IType> sp_type, bool is_const, shared_ptr<ISymbolTable> spScope)
        : _sp_type(sp_type){
            _p_llvm_global_variable = new llvm::GlobalVariable( sp_type->llvmType(),
                                                         is_const, llvm::GlobalValue::ExternalLinkage);
        }
        
        GlobalVariable(shared_ptr<IType> sp_type, bool is_const, shared_ptr<IConstant> sp_init, shared_ptr<ISymbolTable> spScope): _sp_type(sp_type){
            _p_llvm_global_variable = new llvm::GlobalVariable(*spScope->llvm_module(), sp_type->llvmType(), is_const, llvm::GlobalValue::ExternalLinkage, sp_init->llvmConstant());
        }
        
        llvm::Value *llvmValue() const{
            return _p_llvm_global_variable;
        }
        
        shared_ptr<IType> valueType() const{
            return _sp_type;
        }
        
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_llvm_global_variable, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }
        
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            if (spValue->type() != valueType()){
                assert(false); //TODO
            }
            
            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_llvm_global_variable, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }
        
        
        shared_ptr<IType> type() const{
            return ReferenceType::get(_sp_type);
        }
        
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::GlobalVariable *_p_llvm_global_variable;
    };
    
}}