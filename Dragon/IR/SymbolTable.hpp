//
//  SymbolTable.hpp
//  binding
//
//  Created by 张志敏 on 1/1/14.
//
//

#pragma once

#include <Dragon/IR/Interface.hpp>
#include <Dragon/Issue/Issue.hpp>

namespace Dragon{namespace IR{
    
    class SymbolTable : public ISymbolTable{
    public:
        SymbolTable(shared_ptr<ISymbolTable> spScope, std::string name=""): _sp_parent(spScope){
            _uid = spScope->uid()+"."+name;
        }
        
        virtual mapped_type get(const key_type &key) const{
            auto it = _symble_table.find(key);
            if (it != _symble_table.end())
                return it->second;
            
            if (_sp_parent){
                auto par_re = _sp_parent->get(key);
                if (par_re.which() != 0)
                    return par_re;
            }
            
            for (auto &spScope: _importted_domains){
                auto tmp_re = spScope->get(key);
                if (tmp_re.which() != 0)
                    return tmp_re;
            }
            
            return blank();
        }
        
        virtual void put(const key_type &key, mapped_type value){
            auto &tmp = _symble_table[key];
            if (tmp.which() != 0){
                auto sp_issue = make_shared<Issue::redefined_identifier>(key);
                sp_issue->position_ids.push_back(key.position_id);
                throw static_pointer_cast<Issue::CompileIssue>(sp_issue);
            }
            
            tmp = value;
        }
        
        virtual void import(shared_ptr<ISymbolTable> spScope){
            _importted_domains.push_back(spScope);
        }
        
        virtual void export_(shared_ptr<ISymbolTable> spScope){
            auto it = std::find(_importted_domains.begin(), _importted_domains.end(), spScope);
            _importted_domains.erase(it);
        }
        
        virtual shared_ptr<ISymbolTable> parent() const  { assert(_sp_parent); return _sp_parent; }
        
        virtual void parent(shared_ptr<ISymbolTable> sp_parent) { _sp_parent = sp_parent; }
        
        virtual llvm::Module *llvm_module() const {
            assert(_sp_parent);
            return _sp_parent->llvm_module();
        }
        
        virtual shared_ptr<IContext> context() const{
            assert(_sp_parent);
            return _sp_parent->context();
        }
        
        virtual std::string uid() const{
            return _uid;
        }
        
    private:
        shared_ptr<ISymbolTable>                      _sp_parent;
        std::map<key_type, mapped_type>         _symble_table;
        std::list<shared_ptr<ISymbolTable>>           _importted_domains;
        std::string                             _uid;
    };
            
    
}}














