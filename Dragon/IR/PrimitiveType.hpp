//
//  PrimitiveType.hpp
//  binding
//
//  Created by 张志敏 on 10/29/13.
//
//

#pragma once

#include <Dragon/config.hpp>
#include <Dragon/IR/interface.hpp>


namespace Dragon{namespace IR{
    
    class VoidType;
    class BoolType;
    class ByteType;
    class IntType;
    class int32_type;
    class FloatType;

    
    template <typename Type>
    struct is_primitive{
        static const bool value = false;
    };
    
    class TypeBase :public IType{
    public:
        TypeBase(const std::string &name, shared_ptr<IContext> sp_context)
        :  _wp_context(sp_context), _p_llvm_type(nullptr), _name(name){ }
        
        virtual llvm::Type *llvmType() const         { assert(_p_llvm_type); return _p_llvm_type; }
        
        virtual std::string uid() const        { return _name; }
        virtual shared_ptr<IContext> context() const    { return _wp_context.lock(); }
        
        virtual bool isIntergral() const                { return false; }
        virtual bool isFloatingPoint() const          { return false; }
        virtual bool isFunctional() const              { return false; }
        virtual bool isClass() const                   { return false; }
        virtual bool isArithmetical() const            { return false; }
        virtual bool isReference() const               { return false; }
        
        virtual shared_ptr<ISymbolTable> parent() const{
            return _wp_parent.lock();
        }
        
        virtual void parent(shared_ptr<ISymbolTable> sp_parent){
            _wp_parent = sp_parent;
        }
        
    protected:
        weak_ptr<IContext>      _wp_context;
        llvm::Type *                  _p_llvm_type;
        std::string             _name;
        
        weak_ptr<ISymbolTable>       _wp_parent;
    };
    
    
    class MetadataType: public TypeBase{
    public:
        MetadataType(shared_ptr<IContext> sp_context): TypeBase("type", sp_context){
            _p_llvm_type = llvm::Type::getMetadataTy(sp_context->llvmContext());
        }
        
        virtual size_t bitWidth() const{
            return 0;
        }
    };

    
    class VoidType: public TypeBase{
    public:
        typedef void                    cxx_type;
        
        VoidType(shared_ptr<IContext> sp_context)
        : TypeBase("void", sp_context){
            _p_llvm_type = llvm::Type::getVoidTy(sp_context->llvmContext());
        }
        
        virtual size_t bitWidth() const{
            return 0;
        }
        
    };
    
    
    class BoolType: public TypeBase{
    public:
        typedef bool                cxx_type;
        
        BoolType(shared_ptr<IContext> sp_context):
        TypeBase("bool", sp_context){
            _p_llvm_type = llvm::Type::getInt1Ty(sp_context->llvmContext());
        }
        
        virtual bool isIntergral() const{
            return true;
        }
        
        virtual size_t bitWidth() const{
            return 1;
        }
        
//        virtual bool isArithmetical() const            { return true; }
        
    };
    
    
    class ByteType: public TypeBase{
    public:
        typedef byte  cxx_type;
        
        ByteType(shared_ptr<IContext> sp_context)
        : TypeBase("byte", sp_context){
            _p_llvm_type = llvm::Type::getInt8Ty(sp_context->llvmContext());
        }
        
        virtual size_t bitWidth() const{
            return 8;
        }
        
        virtual bool isIntergral() const{
            return true;
        }
        
        virtual bool isArithmetical() const            { return true; }

    };
    
    
    class IntType: public TypeBase{
    public:
        typedef     int_t             cxx_type;
        
 
        IntType(shared_ptr<IContext> sp_context)
        : TypeBase("int", sp_context){
            _p_llvm_type = llvm::Type::getInt64Ty(sp_context->llvmContext());
        }
        
        
        virtual bool isIntergral() const{
            return true;
        }
        
        virtual size_t bitWidth() const{
            return 64;
        }
        
        virtual bool isArithmetical() const            { return true; }

    };
    
    class int32_type: public TypeBase{
    public:
        typedef     int_t             cxx_type;
        
        
        int32_type(shared_ptr<IContext> sp_context)
        : TypeBase("int32", sp_context){
            _p_llvm_type = llvm::Type::getInt32Ty(sp_context->llvmContext());
        }
        
        
        virtual bool isIntergral() const{
            return true;
        }
        
        virtual size_t bitWidth() const{
            return 32;
        }
        
        virtual bool isArithmetical() const            { return true; }

    };
    
    
    class FloatType : public TypeBase{
    public:
        typedef float_t              cxx_type;
        
        FloatType(shared_ptr<IContext> sp_context):
        TypeBase("double", sp_context){
            _p_llvm_type = llvm::Type::getDoubleTy(sp_context->llvmContext());
        }
        
        virtual bool isFloatingPoint() const{
            return true;
        }
        
        virtual size_t bitWidth() const{
            return 64;
        }
        
        virtual bool isArithmetical() const            { return true; }

    };
        
//    
//    inline std::string suffix(std::vector<shared_ptr<IValue>> args){
//        std::string re;
//        for (size_t i = 0; i < args.size(); ++i){
//            re.append(".");
//            re.append(args[i]->type()->name());
//        }
//        
//        return re;
//    }
    
}}