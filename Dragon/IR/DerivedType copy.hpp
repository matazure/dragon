//
//  derived_types.hpp
//  binding
//
//  Created by 张志敏 on 11/3/13.
//
//

#pragma once

#include <vector>

#include <llvm/IR/DerivedTypes.h>
#include <boost/lexical_cast.hpp>
#include <Dragon/IR/PrimitiveType.hpp>

namespace Dragon{namespace IR{
    
    class PointerType : public TypeBase{
    public:
        PointerType(shared_ptr<IType> sp_type)
        : TypeBase(sp_type->uid()+" pointer", sp_type->context()), _sp_value_type(sp_type){
            _p_llvm_type = PointerType::get(sp_type->llvmType(), 0);
        }
        
        static shared_ptr<PointerType> get(shared_ptr<IType> sp_value_type){
            auto spScope = sp_value_type->parent();
            auto name = sp_value_type->uid()+" pointer";
            auto addr_tmp = spScope->get(name);
            if (addr_tmp.which() == AddressId<blank>::value){
                auto sp_pointer_type = make_shared<PointerType>(sp_value_type);
                spScope->get(sp_pointer_type->uid(), sp_pointer_type);
                sp_pointer_type->parent(spScope);
                return sp_pointer_type;
            }
            if (addr_tmp.which() == AddressId<IType>::value){
                return dynamic_pointer_cast<PointerType>(boost::get<shared_ptr<IType>>(addr_tmp));
            }
            
            assert(false); //unreachable
        }
        
        shared_ptr<IType>   valueType() const  { return _sp_value_type; }
        
        virtual size_t bitWidth() const{
            return 64;
        }
        
    private:
        shared_ptr<IType>       _sp_value_type;
    };
    
    
    class ReferenceType : public TypeBase{
    public:
        ReferenceType(shared_ptr<IType> sp_type)
        : TypeBase(sp_type->uid()+" reference", sp_type->context()), _sp_value_type(sp_type){
            _p_llvm_type = PointerType::get(sp_type->llvmType(), 0);
        }
        
        static shared_ptr<ReferenceType> get(shared_ptr<IType> sp_value_type){
            auto spScope = sp_value_type->parent();
            auto name = sp_value_type->uid()+" reference";
            auto addr_tmp = spScope->get(name);
            if (addr_tmp.which() == AddressId<blank>::value){
                auto sp_reference_type = make_shared<ReferenceType>(sp_value_type);
                spScope->get(sp_reference_type->uid(), sp_reference_type);
                sp_reference_type->parent(spScope);
                return sp_reference_type;
            }
            if (addr_tmp.which() == AddressId<IType>::value){
                return dynamic_pointer_cast<ReferenceType>(boost::get<shared_ptr<IType>>(addr_tmp));
            }
            
            assert(false); //unreachable
        }
        
        shared_ptr<IType>   valueType() const  { return _sp_value_type; }
        
        virtual bool isReference() const   { return true; }
        
        virtual size_t bitWidth() const{
            return 64;
        }
        
    private:
        shared_ptr<IType>       _sp_value_type;
    };
    
    
    class ArrayType : public TypeBase{
    private:
        static std::string get_name(shared_ptr<IType> sp_value_type, size_t size){
            std::string name = sp_value_type->uid();
            name.append("[");
            name.append(boost::lexical_cast<std::string>(size));
            name.append("]");
            return name;
        }
    public:
        static shared_ptr<ArrayType> get(shared_ptr<IType> sp_value_type, size_t size){
            auto spScope = sp_value_type->parent();
            auto name = get_name(sp_value_type, size);
            auto addr_tmp = spScope->get(name);
            if (addr_tmp.which() == AddressId<blank>::value){
                auto sp_array_type = make_shared<ArrayType>(sp_value_type, size);
                spScope->get(sp_array_type->uid(), sp_array_type);
                sp_array_type->parent(spScope);
                return sp_array_type;
            }
            if (addr_tmp.which() == AddressId<IType>::value){
                return dynamic_pointer_cast<ArrayType>(boost::get<shared_ptr<IType>>(addr_tmp));
            }
            
            assert(false); //unreachable
        }
        
        ArrayType(shared_ptr<IType> sp_value_type, size_t size)
        :TypeBase(get_name(sp_value_type, size), sp_value_type->context()), _size(size), _sp_value_type(sp_value_type){
            _p_llvm_type = ArrayType::get(sp_value_type->llvmType(), size);
        }
        
        shared_ptr<IType>   valueType() const  { return _sp_value_type; }
        
        size_t element_size() const{
            return _size;
        }
        
        virtual size_t bitWidth() const{
            return _sp_value_type->bitWidth() * _size;
        }
        
    private:
        size_t              _size;
        shared_ptr<IType>   _sp_value_type;
    };
    
    class FunctionType :public ifunction_type{
    public:
        FunctionType(shared_ptr<IType> sp_ret_type, std::vector<shared_ptr<IType>> arg_types)
        : _ret_type(sp_ret_type), _arg_types(arg_types){
//            assert(arg_types.size() == arg_pass_ways.size());
            std::vector<llvm::Type *> llvm_arg_types;
            for (size_t i = 0; i < _arg_types.size(); ++i){
                assert(_arg_types[i] != _arg_types[i]->context()->getType("void")); ///TODO
                
                auto llvmType = _arg_types[i]->llvmType();
                llvm_arg_types.push_back(llvmType);
            }
            
            _p_llvm_type = FunctionType::get(_ret_type->llvmType(), llvm_arg_types, false);
            
            _name = _ret_type->uid()+"(";
            for (auto &arg: _arg_types){
                _name.append(arg->uid());
                _name.append(",");
            }
            _name.pop_back();
            _name.append(")");
        }
        
        virtual bool isReference() const   { return false; }
        
        virtual size_t bitWidth() const{
            assert(false); ///TODO
        }
        
        virtual shared_ptr<IType> returnType() const                    { return _ret_type; }
        virtual std::vector<shared_ptr<IType>> ArgumentTypes() const    { return _arg_types; }
        
        virtual bool isIntergral() const                                { return false; }
        virtual bool isFloatingPoint() const                            { return false; }
        virtual bool isFunctional() const                               { return true; }
        virtual bool isClass() const                                    { return false; }
        virtual bool isArithmetical() const                             { return false; }
        
        virtual llvm::Type *llvmType() const                                  { return _p_llvm_type; }
        virtual FunctionType *llvmFunctionType() const                  { return _p_llvm_type; }
        virtual std::string uid() const                                 { return _name; }
        virtual shared_ptr<IContext> context() const                    { return _ret_type->context(); }
        
        virtual shared_ptr<ISymbolTable> parent() const                      { return _wp_parent.lock(); }
        
        virtual void parent(shared_ptr<ISymbolTable> sp_parent)              { _wp_parent = sp_parent;  }
        
    private:
        shared_ptr<IType>                   _ret_type;
        std::vector<shared_ptr<IType>>      _arg_types;
        FunctionType *                      _p_llvm_type;
        std::string                         _name;
        weak_ptr<ISymbolTable>                   _wp_parent;
    };

//    class vectorized_array_type : public TypeBase{
//    public:
//        vectorized_array_type(shared_ptr<IType> sp_value_type, std::size_t num)
//        : TypeBase("vectorized "+sp_value_type->uid()+" array", sp_value_type->context()), _sp_value_type(sp_value_type){
//            _p_llvm_type = VectorType::get(sp_value_type->llvmType(), num);
//        }
//        
//        shared_ptr<IType> valueType() const{
//            return _sp_value_type;
//        }
//        
//    private:
//        shared_ptr<IType> _sp_value_type;
//    };
    
}}
