//
//  Value.hpp
//  binding
//
//  Created by 张志敏 on 1/16/14.
//
//

#pragma once

namespace Dragon{ namespace IR{
    
    class value_wrapper : public IValue{
    public:
        value_wrapper(shared_ptr<IType> sp_type, llvm::Value *p_llvm_value) : _sp_type(sp_type), _p_llvm_value(p_llvm_value){}
        
        virtual shared_ptr<IType> type() const      { return _sp_type; }
        virtual llvm::Value *llvmValue() const           { return _p_llvm_value; }
        
    private:
        shared_ptr<IType>       _sp_type;
        llvm::Value *                 _p_llvm_value;
    };
    
}}