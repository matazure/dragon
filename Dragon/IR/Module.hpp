//
//  Module.hpp
//  binding
//
//  Created by 张志敏 on 12/27/13.
//
//

#pragma once

#include <Dragon/IR/SymbolTable.hpp>
#include <Dragon/IR/Function.hpp>

namespace Dragon{namespace IR{
    
    
    class Module : public ISymbolTable{
    public:
        Module(const std::string &name, shared_ptr<IContext> sp_context): _name(name), _sp_llvm_module(new llvm::Module(name, sp_context->llvmContext())), _spContext(sp_context){}
        
        
        virtual mapped_type get(const key_type &key) const{
            auto it = _symble_table.find(key);
            if (it != _symble_table.end())
                return it->second;
            
            for (auto &spScope: _importted_domains){
                auto tmp_re = spScope->get(key);
                if (tmp_re.which() != 0)
                    return tmp_re;
            }
            
            return blank();
        }
        
        virtual void put(const key_type &key, mapped_type value){
            auto tmp = _symble_table[key];
            if (tmp.which() != AddressId<blank>::value){
                auto sp_issue = make_shared<Issue::redefined_identifier>(key);
                sp_issue->position_ids.push_back(key.position_id);
                throw static_pointer_cast<Issue::CompileIssue>(sp_issue);
            }
            
            _symble_table[key] = value;
        }
        
        virtual void import(shared_ptr<ISymbolTable> spScope){
            _importted_domains.push_back(spScope);
        }
        
        virtual void export_(shared_ptr<ISymbolTable> spScope){
            auto it = std::find(_importted_domains.begin(), _importted_domains.end(), spScope);
            _importted_domains.erase(it);
        }
        
        std::string name() const{
            return _name;
        }
        
        virtual llvm::Module *llvm_module() const{
            assert(_sp_llvm_module);
            return _sp_llvm_module.get();
        }
        
        virtual shared_ptr<IContext> context() const{
            return _spContext;
        }
        
        //the module uid is its name.
        virtual std::string uid() const{
            return name();
        }
        
        virtual shared_ptr<ISymbolTable> parent() const  { assert(false); }
        virtual void parent(shared_ptr<ISymbolTable> sp_parent) { assert(false); }
        
        void dump(){
            _sp_llvm_module->dump();
        }
        
    private:
        std::string                             _name;
        std::map<key_type, mapped_type>         _symble_table;
        std::list<shared_ptr<ISymbolTable>>        _importted_domains;
        shared_ptr<llvm::Module>                      _sp_llvm_module;
        shared_ptr<IContext>                    _spContext;
    };
    
    
    inline shared_ptr<Module> create_buildin_module(shared_ptr<Context> sp_context){
        auto spModule = make_shared<Module>("buildin", sp_context);
        spModule->put("void", sp_context->getType("void"));
        sp_context->getType("void")->parent(spModule);
        spModule->put("bool", sp_context->getType("bool"));
        sp_context->getType("bool")->parent(spModule);
        spModule->put("byte", sp_context->getType("byte"));
        sp_context->getType("byte")->parent(spModule);
        spModule->put("int", sp_context->getType("int"));
        sp_context->getType("int")->parent(spModule);
        spModule->put("double", sp_context->getType("double"));
        sp_context->getType("double")->parent(spModule);
        spModule->put("type", sp_context->getType("type"));
        sp_context->getType("type")->parent(spModule);
        
        //alias
        spModule->put("char", sp_context->getType("byte"));
        
        //buildin  intrinsics
        auto sp_float_type = sp_context->getType("double");
        std::vector<shared_ptr<IType>> arg_types_float;
        arg_types_float.push_back(sp_float_type);

        auto sp_rf64_f64 = make_shared<FunctionType>(sp_float_type, arg_types_float);
        auto sp_sin = Function::get_intrinsic("sin", sp_rf64_f64, spModule);
        spModule->put("sin/double", sp_sin);
        auto sp_cos = Function::get_intrinsic("cos", sp_rf64_f64, spModule);
        spModule->put("cos/double", sp_cos);
        auto sp_sqrt = Function::get_intrinsic("sqrt", sp_rf64_f64, spModule);
        spModule->put("sqrt/double", sp_sqrt);
        auto sp_exp = Function::get_intrinsic("exp", sp_rf64_f64, spModule);
        spModule->put("exp/double", sp_exp);
        
//        auto sp_rf64_f64 = make_shared<FunctionType>(sp_float_type, arg_types_float);
//        auto sp_sin = Function::get_intrinsic("sin", sp_rf64_f64, spModule);
//        spModule->address("sin/double", sp_sin);
        
        return spModule;
    }
    
}}