//
//  Context.hpp
//  binding
//
//  Created by 张志敏 on 1/5/14.
//
//

#pragma once

#include <map>
#include "llvm/IR/LLVMContext.h"
#include <Dragon/IR/PrimitiveType.hpp>

namespace Dragon{namespace IR{

    class Context : public IContext{
    public:
        Context(){}
        
        void setType(const std::string &name, shared_ptr<IType> sp_type){
            _types[name] = sp_type;
        }
        
    public:
        static shared_ptr<Context> create(){
            auto spContext = make_shared<Context>();
   
            spContext->setType("void", make_shared<VoidType>(spContext));
            spContext->setType("bool", make_shared<BoolType>(spContext));
            spContext->setType("byte", make_shared<ByteType>(spContext));
            spContext->setType("char", spContext->getType("byte"));
            spContext->setType("int", make_shared<IntType>(spContext));
            spContext->setType("double", make_shared<FloatType>(spContext));
            spContext->setType("type", make_shared<MetadataType>(spContext));
            return spContext;
        }
        
        const llvm::LLVMContext &llvmContext() const{
            return _llvmContext;
        }

        llvm::LLVMContext &llvmContext() {
            return _llvmContext;
        }
        
        shared_ptr<IType> getType(const std::string &name) const{
            auto it = _types.find(name);
            assert(it->second);
            return it->second;
        }
        
    private:
        std::map<std::string, shared_ptr<IType>>    _types;
        llvm::LLVMContext   _llvmContext;
    };
    
}}
