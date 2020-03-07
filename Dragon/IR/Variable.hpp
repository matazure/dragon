//
//  Value.hpp
//  binding
//
//  Created by 张志敏 on 10/28/13.
//
//

#pragma once

#include <limits>

#include <Dragon/IR/Interface.hpp>
#include <Dragon/IR/Value.hpp>
#include <Dragon/IR/DerivedType.hpp>

namespace Dragon{namespace IR{

    class Variable : public VariableBase, public IValue{
    public:
        Variable(shared_ptr<IType> sp_type, shared_ptr<IFunction> sp_fun): _sp_type(sp_type){
            readable(true);
            writeable(true);

            auto parent = sp_fun->llvm_function();
            llvm::IRBuilder<> TmpB(&parent->getEntryBlock(), parent->getEntryBlock().begin());
            _p_alloca = TmpB.CreateAlloca(_sp_type->llvmType(), 0, "");
        }

        virtual shared_ptr<IType> valueType() const{
            return _sp_type;
        }

        virtual shared_ptr<IType> type() const{
            return ReferenceType::get(valueType());
        }

        virtual llvm::Value *llvmValue() const { return _p_alloca; }

        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_alloca, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }

        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            if (spValue->type() != valueType()){
                assert(false); //TODO
            }

            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_alloca, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }

    private:
        shared_ptr<IType>           _sp_type;
        llvm::AllocaInst *                _p_alloca;
    };


    class variable_wrapper : public VariableBase,public IValue{
    public:
        variable_wrapper(shared_ptr<IType> sp_type, llvm::Value *p_address) : _sp_type(sp_type), _p_address(p_address){
            readable(true);
            writeable(true);
        }

        virtual shared_ptr<IType> valueType() const  { return _sp_type; }
        virtual llvm::Value *llvmValue() const       { return _p_address; }

        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_insert_at_end){
            auto p_load_inst = new llvm::LoadInst(_p_address, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_load_inst);
        }

        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_insert_at_end){
            assert(spValue->type() == valueType());

            auto p_store_inst = new llvm::StoreInst(spValue->llvmValue(), _p_address, "", sp_insert_at_end->llvmBasicBlock());
            return make_shared<value_wrapper>(_sp_type, p_store_inst);
        }

        virtual shared_ptr<IType> type() const{
            return ReferenceType::get(_sp_type);
        }

    private:
        shared_ptr<IType>           _sp_type;
        llvm::Value *                     _p_address;
    };


}}
