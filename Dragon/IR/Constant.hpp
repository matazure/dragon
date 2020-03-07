//
//  Constants.h
//  binding
//
//  Created by 张志敏 on 1/6/14.
//
//

#pragma once

#include <Dragon/IR/Class.hpp>

namespace Dragon{namespace IR{
        
    
    template <typename _type>
    class Constant : public IConstant{
    public:
        typedef _type                               ir_type;
        typedef typename _type::cxx_type            cxx_type;
        
        Constant(cxx_type v, shared_ptr<IType> sp_type)
        : _sp_type(sp_type)
        , _pLLvmValue(nullptr){
            if (std::is_same<ir_type, IntType>::value){
                _pLLvmValue = llvm::ConstantInt::get(_sp_type->llvmType(), v, true);
            }
            if (std::is_same<ir_type, FloatType>::value){
                _pLLvmValue = llvm::ConstantFP::get(_sp_type->llvmType(), v);
            }
            if (std::is_same<ir_type, BoolType>::value){
//                _pLLvmValue = llvm::ConstantInt::get(_sp_type->llvmType(), v, false);
            }
            if (std::is_same<ir_type, ByteType>::value){
                _pLLvmValue = llvm::ConstantInt::get(_sp_type->llvmType(), v, false);
            }
            
            assert(_pLLvmValue);
        }
        
        virtual shared_ptr<IType> type() const              { return _sp_type; }
        llvm::Constant *  llvmConstant() const                    { return _pLLvmValue; }
        llvm::Value *llvmValue() const                            { return llvmConstant(); }
        //virtual void dump() const                           { return _pLLvmValue->dump(); }
        
    private:
        shared_ptr<IType>               _sp_type;
        llvm::Constant *                _pLLvmValue;
    };
    
    
    template <>
    class Constant<Class> : public IConstant{
    public:
        Constant(shared_ptr<Class> sp_type, std::vector<shared_ptr<IConstant>> inits)
        : _sp_type(sp_type), _pLLvmValue(nullptr){
            
            assert(sp_type->fields().size() == inits.size());
            
            std::vector<llvm::Constant *>  llvm_Constants;
            for (size_t i = 0;i < inits.size(); ++i){
                llvm_Constants.push_back(inits[i]->llvmConstant());
            }
            
            _pLLvmValue = llvm::ConstantStruct::get(_sp_type->llvm_Class(), llvm_Constants);
        }
        
        Constant(shared_ptr<Class> sp_type)
        : _sp_type(sp_type), _pLLvmValue(nullptr){            
            _pLLvmValue = llvm::ConstantAggregateZero::get(_sp_type->llvm_Class());
        }
        
        virtual shared_ptr<IType> type() const                  { return _sp_type; }
        llvm::Constant *  llvmConstant() const                       { return _pLLvmValue; }
        llvm::Value *llvmValue() const                               { return llvmConstant(); }
        //virtual void dump() const                       { return _pLLvmValue->dump(); }
        
    private:
        shared_ptr<Class>         _sp_type;
        llvm::Constant *                      _pLLvmValue;
    };
    
    
    template <>
    class Constant<ArrayType> : public IConstant{
    public:
        Constant(shared_ptr<ArrayType> sp_type, std::vector<shared_ptr<IConstant>> inits)
        :_sp_type(sp_type){
            if (sp_type->element_size() != inits.size()){
                assert(false);
            }
            
            std::vector<llvm::Constant *>  llvm_Constants;
            for (size_t i = 0;i < inits.size(); ++i){
                llvm_Constants.push_back(inits[i]->llvmConstant());
            }
            
            _pLLvmValue = llvm::ConstantArray::get((llvm::ArrayType *)_sp_type->llvmType(), llvm_Constants);
        }
        
        virtual shared_ptr<IType> type() const                  { return _sp_type; }
        llvm::Constant *  llvmConstant() const                       { assert(_pLLvmValue); return _pLLvmValue; }
        llvm::Value *llvmValue() const                               { return llvmConstant(); }
        //virtual void dump() const                       { return _pLLvmValue->dump(); }
        
    private:
        shared_ptr<ArrayType>               _sp_type;
        llvm::Constant *                    _pLLvmValue;
    };
    
    
    class Constant_int : public IConstant{
    public:
        Constant_int(int64_t v, shared_ptr<IType> sp_type)
        : _sp_type(sp_type){
            _pLLvmValue = llvm::ConstantInt::get(_sp_type->llvmType(), v);
        }
        
        virtual shared_ptr<IType> type() const               { return _sp_type; }
        llvm::Constant *  llvmConstant() const                   { return _pLLvmValue; }
        llvm::Value *llvmValue() const                           { return llvmConstant(); }
        //virtual void dump() const                       { return _pLLvmValue->dump(); }
        
    private:
        shared_ptr<IType>       _sp_type;
        llvm::Constant *              _pLLvmValue;
    };
    
    
    inline shared_ptr<IConstant> zeroInitialize(shared_ptr<IType> sp_type){
        if (sp_type->isClass()){
            auto spClass = dynamic_pointer_cast<Class>(sp_type);
            return make_shared<Constant<Class>>(spClass);
        }
        if (sp_type->isIntergral()){
            return make_shared<Constant<IntType>>(0, sp_type);
        }
        if (sp_type->isFloatingPoint()){
            return make_shared<Constant<FloatType>>(0.0, sp_type);
        }
        
        assert(false); ///< unreachable
    }
    
    
}}

















