//
//  Function.hpp
//  binding
//
//  Created by 张志敏 on 10/30/13.
//
//

#pragma once

#include <set>
#include <llvm/IR/Function.h>
#include <llvm/IR/Intrinsics.h>
#include <Dragon/IR/Variable.hpp>
#include <Dragon/IR/DerivedType.hpp>
#include <Dragon/IR/SymbolTable.hpp>
#include <Dragon/IR/Value.hpp>
#include <Dragon/IR/Utility.hpp>

namespace Dragon{namespace IR{


    class argument : public iargument{
    public:
        argument(shared_ptr<IType> sp_type, llvm::Argument *llvm_argument)
        : _sp_type(sp_type), _p_llvm_argument(llvm_argument){
            if (sp_type->isReference()){
                auto sp_ref_type = static_pointer_cast<ReferenceType>(sp_type);
                auto sp_var = make_shared<variable_wrapper>(sp_ref_type->valueType(), llvm_argument);
                sp_var->writeable(true);
                sp_var->readable(true);
                _address = static_pointer_cast<VariableBase>(sp_var);
            }else{
                _address = make_shared<value_wrapper>(sp_type, llvm_argument);
            }
        }

        virtual shared_ptr<IType> type() const              { return _sp_type; }
        virtual llvm::Value *llvmValue() const                   { return _p_llvm_argument; }
        virtual ValueAddress Address() const               { return _address; }

    private:
        shared_ptr<IType>               _sp_type;
        llvm::Argument *                      _p_llvm_argument;
        ValueAddress                   _address;
    };


    class Function : public IFunction{
    public:
        /// llvm intrinsics  just need  declaration
        static shared_ptr<Function> get_intrinsic(const std::string &intrinsic_name, shared_ptr<ifunction_type> sp_fun_type, shared_ptr<imodule> spModule){
            auto llvm_intrinsic_name = "llvm." + intrinsic_name + abbr_suffix(sp_fun_type->ArgumentTypes());
            return make_shared<Function>(sp_fun_type, spModule, llvm_intrinsic_name);
        }

        class call : public IInstruction{
        public:
            call(shared_ptr<Function> sp_fun, std::vector<Address> arguments, shared_ptr<IBasicBlock> sp_cur_block)
            : _sp_type(sp_fun->FunctionType()->returnType()){
                auto sp_fun_type = sp_fun->FunctionType();
                auto arg_types = sp_fun_type->ArgumentTypes();
                //check arugments size
                if (arg_types.size() != arguments.size()){
                    assert(false);
                }
                //chech argument types
                for (size_t i = 0; i < arguments.size(); ++i){
                    if (arg_types[i] != get_type(arguments[i])){
                        std::cout << "Error: " << get_type(arguments[i])->uid() << std::endl;
                        assert(false); ///TODO
                    }
                }

                std::vector<llvm::Value *> llvm_args(arg_types.size());
                for (size_t i = 0; i < arg_types.size(); ++i){
                    if (arguments[i].which() == AddressId<IValue>::value){
                        llvm_args[i] = boost::get<shared_ptr<IValue>>(arguments[i])->llvmValue();
                        continue;
                    }
                    if (arguments[i].which() == AddressId<VariableBase>::value){
                        auto sp_var = boost::get<shared_ptr<VariableBase>>(arguments[i]);
                        auto spValue = sp_var->get(sp_cur_block);
                        llvm_args[i] = spValue->llvmValue();

                        continue;
                    }
                    if (arguments[i].which() == AddressId<IConstant>::value){
                        llvm_args[i] = boost::get<shared_ptr<IConstant>>(arguments[i])->llvmValue();
                        continue;
                    }
                    assert(false);
                }

                _p_llvm_call_instruction = llvm::CallInst::Create(sp_fun->llvm_function(), llvm_args, "", sp_cur_block->llvmBasicBlock());
            }


            virtual shared_ptr<IType> type() const          { return _sp_type; }
            virtual llvm::Instruction *llvm_instruction() const   { return _p_llvm_call_instruction; }
            virtual llvm::Value *llvmValue() const               { return llvm_instruction(); }

        private:
            shared_ptr<IType>               _sp_type;
            llvm::CallInst                       *_p_llvm_call_instruction;
        };

        Function(shared_ptr<ifunction_type> sp_fun_type, shared_ptr<ISymbolTable> sp_parent, std::string name="")
        : _sp_fun_type(sp_fun_type), _p_llvm_fun(nullptr), _is_defined(false), _domain(sp_parent){

            //get llvm::FunctionType *
            llvm::FunctionType *p_llvm_type = _sp_fun_type->llvmFunctionType();
            _p_llvm_fun = llvm::Function::Create(p_llvm_type, llvm::Function::ExternalLinkage, name, sp_parent->llvm_module());

            auto arg_types = sp_fun_type->ArgumentTypes();
            int i = 0;
            for (auto it = _p_llvm_fun->arg_begin(); it != _p_llvm_fun->arg_end(); ++it, ++i){
                auto sp_argument = make_shared<argument>(arg_types[i], it);
                _args.push_back(sp_argument);
            }
        }

        virtual shared_ptr<IType> type() const                      { return _sp_fun_type; }
        virtual llvm::Value *llvmValue() const                           { return _p_llvm_fun; }
        virtual shared_ptr<ifunction_type> FunctionType() const    { return _sp_fun_type; }
        virtual llvm::Function *llvm_function() const                     { return _p_llvm_fun; }
        virtual llvm::Constant *llvmConstant() const                     { return llvm_function(); }
        virtual bool is_defined() const                             { return _is_defined; }

        //virtual void dump() const {
        //     _p_llvm_fun->dump();
        // }

        std::vector<shared_ptr<argument>> arguments() const{
            return _args;
        }

    private:
        void set_arguemtns(){

        }

    private:
        shared_ptr<ifunction_type>                      _sp_fun_type;
        llvm::Function *                                      _p_llvm_fun;
        std::vector<shared_ptr<argument>>               _args;
        bool                                            _is_defined;

        SymbolTable                                     _domain;
    };

}}
