//
//  Builder.hpp
//  binding
//
//  Created by 张志敏 on 1/5/14.
//
//

#pragma once

#include <stack>
#include <vector>
#include <llvm/IR/IRBuilder.h>
#include <Dragon/IR/IR.hpp>


namespace Dragon{
    extern jmp_buf jmp;
}

namespace Dragon{namespace CodeGen{

    using namespace IR;
    using boost::optional;
    using boost::apply_visitor;
    using boost::blank;
    
    namespace Detail{
        class ScopeTodo{
        public:
            ScopeTodo(std::function<void ()> fun): _todo(fun){}
            
            ~ScopeTodo(){
                _todo();
            }
            
        private:
            std::function<void ()>  _todo;
        };
    }
    
    enum struct CompilerState {
        buildin,
        terminal,
        script
    };
    
    enum struct TerminateType {
        none = 0,
        return_,
        break_,
        continue_,
        unreachable
    };
    
    struct BodyTerminator: public std::vector<TerminateType>{
        
        bool terminated(){
            for(auto &flag: *this){
                if (flag != TerminateType::none){
                    return true;
                }
            }
            
            return false;
        }
        
        bool returned(){
            for(auto &flag: *this){
                if (flag == TerminateType::return_)
                    return true;
            }
            
            return false;
        }
        
        operator TerminateType(){
            return back();
        }
    };
    
    struct ConditionTerminator{
        TerminateType    if_flag;
        TerminateType    else_flag;
        
        ConditionTerminator(): if_flag(TerminateType::none), else_flag(TerminateType::none){}
        
        bool terminated(){
            if (if_flag != TerminateType::none && else_flag != TerminateType::none){
                return true;
            }
            
            return false;
        }
        
        bool returned(){
            if (if_flag == TerminateType::return_ && else_flag == TerminateType::return_){
                return true;
            }
            
            return false;
        }
        
        operator TerminateType(){
            if (if_flag == TerminateType::none || else_flag==TerminateType::none)
                return TerminateType::none;
            
            if (if_flag == TerminateType::return_ && else_flag == TerminateType::return_)
                return TerminateType::return_;
            
            assert(false);
            //return TerminateType::branch;
        }
    };
    
    
    class Builder{
    public:
        
        class ScopeScope{
        public:
            ScopeScope(shared_ptr<Builder> spBuilder, shared_ptr<ISymbolTable> spCurrentScope)
            : _spBuilder(spBuilder), _spCurrentScope(spCurrentScope){
                spBuilder->currentScope(spCurrentScope);
            }
            
            ~ScopeScope(){
                _spBuilder->currentScope(_spCurrentScope->parent());
            }
            
        private:
            shared_ptr<Builder> _spBuilder;
            shared_ptr<ISymbolTable> _spCurrentScope;
        };
        
        static ScopeScope insertScope(shared_ptr<Builder> spBuilder, shared_ptr<ISymbolTable> spScope){
            return ScopeScope(spBuilder, spScope);
        }
               
        Builder(shared_ptr<ISymbolTable> spScope)
        : _spContext(spScope->context()), _spCurrentScope(spScope), _spCurrentBlock(nullptr), enablePrintResult(true){}
        
        shared_ptr<IContext> context() const{
            return _spContext;
        }
        
        shared_ptr<ISymbolTable> currentScope() const{
            return _spCurrentScope;
        }
        
        shared_ptr<ExecutionEngine> engine() const{
            return _spEngine;
        }
        
        void engine(shared_ptr<ExecutionEngine> spEngine){
            _spEngine = spEngine;
        }
        
        void currentScope(shared_ptr<ISymbolTable> spScope){
            _spCurrentScope = spScope;
        }
        
        shared_ptr<IBasicBlock> currentBlock() const              { return _spCurrentBlock; }
        void currentBlock(shared_ptr<IBasicBlock> spBlock){
            _spCurrentBlock = spBlock;
        }
        
        shared_ptr<IFunction> currentFunction() const              { return _functions.top(); }
        
        void pushFunction(shared_ptr<IFunction> sp_fun){
            _functions.push(sp_fun);
        }
        
        void popFunction(){
            _functions.pop();
        }
        
        
        shared_ptr<Detail::ScopeTodo> defineCommand(){
            std::vector<shared_ptr<IR::IType>> arg_types;
            auto sp_void_type = context()->getType("void");
            auto sp_fun_type = make_shared<IR::FunctionType>(sp_void_type, arg_types);
            auto sp_command = make_shared<IR::Function>(sp_fun_type, currentScope(), "@command");
//            _functions.push(sp_command);
            auto sp_entry_block = make_shared<IR::BasicBlock>(sp_command);
            currentBlock(sp_entry_block);
            auto sp_todo = make_shared<Detail::ScopeTodo>([sp_command, this](){
                createReturnVoid();
//                _functions.pop();
                
                auto re = setjmp(jmp);
                switch (re){
                    case 0:
                        engine()->run_function(sp_command);

//                        sp_command->llvm_function()->eraseFromParent();

                        break;
                    case -3:
                        assert(false);
                    case -4:
                        assert(false);
                    default:
                        assert(false);
                }
                
              
            });

            return sp_todo;
        }
        
        Address get(const std::string &id) const{
            return _spCurrentScope->get(id);
        }
        
        void put(const AST::Identifier &id, Address v){
            _spCurrentScope->put(id, v);
        }
        
        shared_ptr<Variable> createVariable(shared_ptr<IType> sp_type){
            auto sp_var = make_shared<Variable>(sp_type, currentFunction());
            return sp_var;
        }
        
        shared_ptr<GlobalVariable> createGlobalVariable(shared_ptr<IType> spType){
            auto spZeroInitializer = zeroInitialize(spType);
            return make_shared<IR::GlobalVariable>(spType, false, spZeroInitializer, currentScope());
        }
        
        shared_ptr<malloc> createMalloc(shared_ptr<IType> sp_value_type, shared_ptr<IValue> sp_size){
            auto addr_tmp = _spCurrentScope->get("malloc");
            auto sp_malloc_fun = boost::get<shared_ptr<Function>>(addr_tmp);
            auto sp_dynamic_array = make_shared<malloc>(sp_value_type, sp_size, currentBlock(), sp_malloc_fun);
            return sp_dynamic_array;
        }
        
        shared_ptr<memcpy> createMemcpy(shared_ptr<IValue> sp_src, shared_ptr<IValue> sp_des, shared_ptr<IValue> sp_size){
            auto addr_tmp = _spCurrentScope->get("memcpy");
            auto sp_memcpy_fun = boost::get<shared_ptr<Function>>(addr_tmp);
            return make_shared<memcpy>(sp_src, sp_des, sp_size, currentBlock(), sp_memcpy_fun);
        }
        
        shared_ptr<store> createStore(shared_ptr<IValue> spValue, shared_ptr<IValue> sp_ptr){
            return make_shared<store>(spValue, sp_ptr, _spCurrentBlock);
        }
        
        shared_ptr<load> createLoad(shared_ptr<IValue> spValue){
            return make_shared<load>(spValue, _spCurrentBlock);
        }
        
        shared_ptr<return_instruction>  createReturn(shared_ptr<IValue> spValue){
            auto sp_ret = make_shared<return_instruction>(spValue, _spContext, _spCurrentBlock);
            return sp_ret;
        }
        
        shared_ptr<return_instruction> createReturnVoid(){
            return make_shared<return_instruction>(_spContext, _spCurrentBlock);
        }
        
        shared_ptr<Function> createFunction(shared_ptr<ifunction_type> sp_fun_type, const std::string &name){
            auto sp_fun = make_shared<Function>(sp_fun_type, _spCurrentScope, name);
            put(name, sp_fun);
            return sp_fun;
        }
        
        shared_ptr<Function::call> createCall(shared_ptr<Function> sp_fun, std::vector<Address> arguments){
            assert(sp_fun);
            return make_shared<Function::call>(sp_fun, arguments, _spCurrentBlock);
        }
        
        shared_ptr<Function::call> createCall(const std::string &fun_name, std::vector<Address> arguments){
            std::vector<shared_ptr<IType>> arg_types;
            for (auto arg: arguments){
                arg_types.push_back(get_type(arg));
            }
            
            auto fc_addr = get(fun_name);
            if (fc_addr.which() != AddressId<FunctionCollection>::value){
                assert(false);
            }
            auto sp_fc = boost::get<shared_ptr<FunctionCollection>>(fc_addr);
            auto sp_fun = sp_fc->match(get_types(arguments));
            return createCall(sp_fun, arguments);
        }
        
        shared_ptr<SymbolTable> createScope(){
            return make_shared<SymbolTable>(_spCurrentScope);
        }
        
        shared_ptr<Class> createStruct(const std::string &name){
            auto sp_struct = make_shared<Class>(name, _spCurrentScope);
            put(name, static_pointer_cast<IType>(sp_struct));
            return sp_struct;
        }
    
        
        shared_ptr<Class> createClass(const std::string &name){
            auto sp_class = make_shared<Class>(name, _spCurrentScope);
            sp_class->parent(_spCurrentScope);   
            return sp_class;
        }
        
        shared_ptr<IR::BasicBlock> createBasicBlock(){
            return make_shared<IR::BasicBlock>(currentFunction());
        }
        
        shared_ptr<branch_instruction> createConditionBranch(shared_ptr<IBasicBlock> sp_then_block, shared_ptr<IBasicBlock> sp_else_block, shared_ptr<IValue> sp_condition){
            return make_shared<branch_instruction>(sp_then_block, sp_else_block, sp_condition, _spCurrentBlock);
        }
        
        shared_ptr<branch_instruction> createBranch(shared_ptr<IBasicBlock> sp_block){
            return make_shared<branch_instruction>(sp_block,  _spCurrentBlock);
        }
         
        shared_ptr<branch_instruction> createBreak(){
            if (after_blocks.empty()){
                assert(false);
            }
            return createBranch(after_blocks.top());
        }
        
        shared_ptr<branch_instruction> createContinue(){
            if (loop_end_blocks.empty()){
                assert(false);
            }
            return createBranch(loop_end_blocks.top());
        }
        
        shared_ptr<bit_cast> createBitCast(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type){
            return make_shared<bit_cast>(spValue, sp_type, _spCurrentBlock);
        }
        
        shared_ptr<pointer2int> createPointer2Int(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type){
            return make_shared<pointer2int>(spValue, sp_type, _spCurrentBlock);
        }
        
        shared_ptr<int2pointer> createInt2Pointer(shared_ptr<IValue> spValue, shared_ptr<IType> sp_type){
            return make_shared<int2pointer>(spValue, sp_type, _spCurrentBlock);
        }
        
        shared_ptr<IType> type(const std::string name){
            return _spContext->getType(name);
        }
    
        shared_ptr<FieldAccessor> accessField(shared_ptr<VariableBase> sp_object, std::string name){
            assert(sp_object);
            return make_shared<FieldAccessor>(sp_object, name, currentBlock());
        }
        
        shared_ptr<Variable> createTemporaryVariable(shared_ptr<IValue> spValue){
            auto sp_var = createVariable(spValue->type());
            sp_var->set(spValue, _spCurrentBlock);
            sp_var->writeable(false);
            sp_var->readable(true);
            return sp_var;
        }
        
        shared_ptr<Function::call> construct(shared_ptr<Class> sp_class, std::list<Address> arguments){
            std::vector<Address> arg_addrs(arguments.begin(), arguments.end());
            std::vector<shared_ptr<IType>> arg_types;
            for (auto arg: arg_addrs){
                arg_types.push_back(get_type(arg));
            }
            
            auto constructor_name = "::"+sp_class->uid()+suffix(arg_types);
            auto sp_constructor = sp_class->get_constructor(constructor_name);
            if(!sp_constructor){
                assert(false);
            }
            
            return createCall(sp_constructor, arg_addrs);
        }
        
        shared_ptr<IValue> copyConstruct(shared_ptr<IValue> spValue){
            if (spValue->type()->isClass()){
                auto sp_class = dynamic_pointer_cast<Class>(spValue->type());
                assert(sp_class);
                auto sp_copy_constructor = sp_class->get_copy_constructor();
                if (!sp_copy_constructor){
                    return spValue;
                }
                std::vector<Address> args(1);
                args[0] = spValue;
                return createCall(sp_copy_constructor, args);
            }else{
                return spValue;
            }
        }
        
        shared_ptr<Function::call> serialize(shared_ptr<IValue> spValue){
            std::vector<Address> args;
            args.push_back(spValue);
            return createCall("serialize", args);
        }
        
        void print(shared_ptr<IValue> spValue){
            auto sp_serilize = serialize(spValue);
            std::vector<Address> args;
            args.push_back(sp_serilize);
            createCall("print_", args);
        }
        
        void printResult(shared_ptr<IValue> spValue){
            if (enablePrintResult){
                print(spValue);
            }
        }
        
        shared_ptr<IR::IType> getType(const std::string &id){
            auto addr = get(id);
            switch (addr.which()){
                case IR::AddressId<IType>::value:{
                    return boost::get<shared_ptr<IType>>(addr);
                }
                case IR::AddressId<IClass>::value : {
                    //todo
                    //                    return boost::get<shared_ptr<IClass>>(addr);
                }
                default:
                    assert(false);
            }
        }
        
        bool inFunctionScope(){
            return !_functions.empty();
        }
        
    private:
        shared_ptr<IContext>                            _spContext;
        shared_ptr<ISymbolTable>                             _spCurrentScope;
        shared_ptr<IBasicBlock>                         _spCurrentBlock;
        std::stack<shared_ptr<IFunction>>               _functions;
        shared_ptr<ExecutionEngine>                     _spEngine;
        
    public:
        std::stack<shared_ptr<IBasicBlock>>            loop_end_blocks;
        std::stack<shared_ptr<IBasicBlock>>            after_blocks;
        
        std::stack<ConditionTerminator>                ConditionTerminators;
        std::stack<BodyTerminator>                     BodyTerminators;
        
        
        CompilerState                                  compiler_state_;
        
        bool                                            enablePrintResult;
        
        std::stack<size_t>                              in_block_flags;
    };
    
    
}}