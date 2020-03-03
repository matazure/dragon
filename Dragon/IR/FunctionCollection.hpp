#pragma once

#include <vector>
#include <Dragon/IR/interface.hpp>
#include <Dragon/Issue/Issue.hpp>

namespace Dragon{namespace IR{
    
    class FunctionCollection{
    public:
        FunctionCollection(const std::string &name): _name(name){}
        
        void add(shared_ptr<Function> sp_fun){
            _functions.push_back(sp_fun);
        }
        
        shared_ptr<Function> match(std::vector<shared_ptr<IType>> types){
            if (types.size() == 0){
                return _functions.front();
            }
            
            auto sp_type = types.front();
            
            for (size_t i = 0; i < _functions.size(); ++i){
                auto types = _functions[i]->FunctionType()->ArgumentTypes();
                if (types[0] == sp_type){
                    return _functions[i];
                }
            }
            
            throw make_shared<Issue::no_matched_function>();
        }
        
    private:
        
        
    private:
        std::string       _name;
        std::vector<shared_ptr<Function>>  _functions;
    };
    
    
}}