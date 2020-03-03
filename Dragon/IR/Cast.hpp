//
//  cast.hpp
//  binding
//
//  Created by 张志敏 on 2/13/14.
//
//

#pragma once

#include <Dragon/IR/interface.hpp>

namespace Dragon{namespace IR{
    
    class truncate_cast : public IInstruction{
    public:
        truncate_cast(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::TruncInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };
    
    class zero_extend_cast : public IInstruction{
    public:
        zero_extend_cast(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::ZExtInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };
    
    class float2int : public IInstruction{
    public:
        float2int(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::FPToSIInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };

    class int2float : public IInstruction{
    public:
        int2float(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::SIToFPInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };

    
    class bit_cast : public IInstruction{
    public:
        bit_cast(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::BitCastInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };
    
    class pointer2int : public IInstruction{
    public:
        pointer2int(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::PtrToIntInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };
    
    class int2pointer : public IInstruction{
    public:
        int2pointer(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type, shared_ptr<IBasicBlock> sp_insert_at_end): _sp_type(sp_type){
            _p_llvm_value = new llvm::IntToPtrInst(spValue->llvmValue(), sp_type->llvmType(), "", sp_insert_at_end->llvmBasicBlock());
        }
        
        virtual shared_ptr<IType> type() const          { return _sp_type; }
        virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_value; }
        virtual llvm::Value * llvmValue() const              { return llvm_instruction(); }
        virtual void dump() const                       { return _p_llvm_value->dump(); }
        
    private:
        shared_ptr<IType>   _sp_type;
        llvm::Instruction *       _p_llvm_value;
    };

}}