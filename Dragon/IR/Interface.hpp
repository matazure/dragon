#pragma once

#include <string>
#include <vector>
#include <map>

#include <boost/variant.hpp>
#include <boost/signals2.hpp>
#include <llvm/IR/Type.h>
#include <llvm/IR/Value.h>
#include <llvm/IR/Constant.h>
#include "llvm/IR/Instruction.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <Dragon/AST/Statement.hpp>
#include <Dragon/IR/declarationSpecifier.hpp>
#include <Dragon/IR/Address.hpp>


namespace Dragon{namespace IR{

    using boost::blank;
    using boost::optional;
    using boost::variant;
    using boost::apply_visitor;

    class IValue;
    class IFunction;
    class IContext;
    class IType;
    class VariableBase;
    class IConstant;
    class IClass;
    class ISymbolTable;
    class IInstruction;
    class AddressList;
    class Function;
    class MemberFunctionAccessor;

    class FieldAccessor;
    class GenericAddress;

    class FunctionCollection;

    class ISymbolTable{
    public:
        typedef AST::Identifier     key_type;
        typedef Address             mapped_type;

        virtual llvm::Module *llvm_module() const = 0;
        virtual shared_ptr<IContext> context() const = 0;

        virtual mapped_type get(const key_type &) const = 0;
        virtual void put(const key_type &, mapped_type) = 0;
        virtual shared_ptr<ISymbolTable> parent() const = 0;
        virtual void parent(shared_ptr<ISymbolTable>) = 0;
        virtual void import(shared_ptr<ISymbolTable>) = 0;
        virtual void export_(shared_ptr<ISymbolTable>) = 0;
        virtual std::string uid() const = 0;
    };

    typedef ISymbolTable                 imodule;

    class IType{
    public:
        virtual std::string uid() const = 0;
        virtual llvm::Type *llvmType() const = 0;
        virtual shared_ptr<IContext> context() const = 0;
        virtual bool isIntergral() const = 0;
        virtual bool isFloatingPoint() const = 0;
        virtual bool isFunctional() const = 0;
        virtual bool isClass() const = 0;
        virtual bool isArithmetical() const = 0;
        virtual bool isReference() const = 0;
        virtual size_t bitWidth() const = 0;
        virtual shared_ptr<ISymbolTable> parent() const = 0;
        virtual void parent(shared_ptr<ISymbolTable> sp_parent) = 0;

    public:
        std::map<std::string, shared_ptr<IFunction>> constructors;
    };

    class ifunction_type : public IType{
    public:
        virtual shared_ptr<IType> returnType() const = 0;
        virtual std::vector<shared_ptr<IType>> ArgumentTypes() const = 0;
    //    virtual std::vector<pass_way>   argument_pass_ways() const = 0;

        virtual llvm::FunctionType *llvmFunctionType() const = 0;
    };


    class IValue{
    public:
        virtual shared_ptr<IType> type() const = 0;
        virtual llvm::Value *llvmValue() const = 0;
        virtual void dump() const{
            assert(false);
        }
    };

    class IInstruction: public IValue{
    public:
        virtual llvm::Instruction *llvm_instruction() const = 0;
    };


    class IConstant : public IValue{
    public:
        virtual llvm::Constant *llvmConstant() const = 0;
    };

    typedef boost::variant<shared_ptr<IValue>, shared_ptr<VariableBase>> ValueAddress;

    class iargument{
    public:
        virtual shared_ptr<IType> type() const = 0;
        virtual llvm::Value *llvmValue() const = 0;
        virtual ValueAddress Address() const = 0;
    };

    class IFunction: public IConstant{
    public:
        virtual shared_ptr<ifunction_type> FunctionType() const = 0;
        virtual llvm::Function *llvm_function() const = 0;
        virtual bool is_defined() const = 0;
    };

    class IBasicBlock : public IValue{
    public:
        virtual llvm::BasicBlock *llvmBasicBlock() const = 0;
        virtual shared_ptr<IFunction> parent() const = 0;
    };

    class VariableBase{
    public:
        virtual shared_ptr<IType> valueType() const = 0;
        virtual llvm::Value *llvmValue() const = 0;
        virtual shared_ptr<IValue> get(shared_ptr<IBasicBlock> sp_cur_block) = 0;
        virtual shared_ptr<IValue> set(shared_ptr<IValue> spValue, shared_ptr<IBasicBlock> sp_cur_block) = 0;
//        virtual bool readable() const  = 0;
//        virtual bool writeable() const = 0;

        virtual bool readable() const               { return _readable; }
        virtual bool writeable() const              { return _writeable; }
        virtual void readable(bool v)               { _readable = v; }
        virtual void writeable(bool v)              { _writeable = v; }

    private:
        bool _readable;
        bool _writeable;
    };


    class IContext{
    public:
        virtual llvm::LLVMContext &llvmContext() const = 0;
        virtual shared_ptr<IType> getType(const std::string &name) const = 0;
    };


    class IClass : public ISymbolTable{};

    class Class;

    typedef boost::variant<boost::blank, shared_ptr<Function>, shared_ptr<ISymbolTable>, boost::recursive_wrapper<GenericAddress>, shared_ptr<Class>> statement;

    template <typename T>
    struct StatementId;

    template<>
    struct StatementId<Class>{ const static size_t value = 4; };

    class GenericAddress{
    public:
        shared_ptr<boost::signals2::signal<statement (std::vector<Address>)>>   sp_specialize;
    };

}}
