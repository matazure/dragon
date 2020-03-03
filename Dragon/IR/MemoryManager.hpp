//
//  raw_array.h
//  binding
//
//  Created by 张志敏 on 11/26/13.
//
//

#pragma once

#include <Dragon/IR/DerivedType.hpp>
#include <Dragon/IR/Constant.hpp>
#include <Dragon/IR/instruction.hpp>
#include <Dragon/IR/cast.hpp>

namespace Dragon{namespace IR{
    
    class malloc : public IInstruction{
    public:
        malloc(shared_ptr<IType> sp_value_type, shared_ptr<IValue> sp_size, shared_ptr<IBasicBlock> sp_insert_at_end, shared_ptr<Function> sp_malloc_fun)
        :  _sp_array_type(PointerType::get(sp_value_type)){
            assert(sp_size->type()->isIntergral());
            
            auto size = (sp_value_type->bitWidth()+7) / 8;
            auto sp_type_size = make_shared<Constant<IntType>>(size, sp_value_type->context()->getType("int"));
            auto sp_byte_size = make_shared<ArithmeticalOperator>(AST::BinaryOperator("*"), sp_size, sp_type_size, sp_insert_at_end);
            std::vector<Address> args;
            args.push_back(sp_byte_size);
            auto p_byte_malloc = make_shared<Function::call>(sp_malloc_fun, args, sp_insert_at_end)->llvm_instruction();
            _p_llvm_value = new llvm::BitCastInst(p_byte_malloc, _sp_array_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        
        virtual shared_ptr<IType> type() const          { return _sp_array_type; }
        virtual llvm::Instruction *llvm_instruction() const   { assert(_p_llvm_value); return _p_llvm_value; }
        virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }
        
    private:
        
        llvm::Instruction *                           _p_llvm_value;
        shared_ptr<IType>                       _sp_array_type;
    };
    
    class memcpy : public IInstruction{
    public:
        memcpy(shared_ptr<IValue> sp_src, shared_ptr<IValue> sp_des, shared_ptr<IValue> sp_size, shared_ptr<IBasicBlock> sp_insert_at_end, shared_ptr<Function> sp_memcpy_fun){
            if(!sp_size->type()->isIntergral()){
                assert(false);
            }
            
            auto Context = sp_src->type()->context();
            _sp_type = Context->getType("void");
            auto sp_byte_type = Context->getType("byte");
            auto sp_byte_pointer_type = PointerType::get(sp_byte_type);
            auto sp_src_cast = make_shared<bit_cast>(sp_src, sp_byte_pointer_type, sp_insert_at_end);
            auto sp_des_cast = make_shared<bit_cast>(sp_des, sp_byte_pointer_type, sp_insert_at_end);
            std::vector<Address> args;
            args.push_back(sp_des_cast);
            args.push_back(sp_src_cast);
            args.push_back(sp_size);
            _sp_memcpy = make_shared<Function::call>(sp_memcpy_fun, args, sp_insert_at_end);
        }
        
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _sp_memcpy->llvm_instruction(); }
        virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }
        
    private:
        shared_ptr<IInstruction>                _sp_memcpy;
        shared_ptr<IType>                       _sp_type;
    };
        
}}