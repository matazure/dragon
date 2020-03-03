
#pragma once

#include <Dragon/IR/class.hpp>
#include <Dragon/IR/utility.hpp>

namespace Dragon{namespace IR{
    
        
    class access{
    public:
        typedef Address         result_type;
        
        access(shared_ptr<IBasicBlock> sp_current_block): _sp_current_block(sp_current_block){}
        
        result_type operator()(const Address &object, const std::string &member_name){
            boost::variant<std::string> member_name_wrapper(member_name);
            return boost::apply_visitor(*this, object, member_name_wrapper);
        }
        
        result_type operator()(IR::blank b, const std::string &){
            assert(false);///TODO
        }
        
        result_type operator()(shared_ptr<IType> sp_type, const std::string &member_name){
            assert(false); ///TODO
        }
        
        result_type operator()(shared_ptr<imodule> spModule, const std::string &member_name){
            return spModule->get(member_name);
        }
        
        result_type operator()(shared_ptr<IClass> sp_class, const std::string &member_name){
            assert(false); ///TODO
        }
        
        result_type operator()(shared_ptr<VariableBase> sp_variable, const std::string &member_name){
            auto sp_type = sp_variable->valueType();
            auto spClass = dynamic_pointer_cast<Class>(sp_type);
            if (!spClass){
                assert(false); ///TODO
            }
            
            auto flag = spClass->is_member(member_name);
            switch(flag){
                case Class::member_flag::none: {
                    assert(false); ///TODO
                }
                case Class::member_flag::field: {
                    return static_pointer_cast<VariableBase>(make_shared<FieldAccessor>(sp_variable, member_name, _sp_current_block));
                }
                case Class::member_flag::member_function :{
                    return make_shared<MemberFunctionAccessor>(sp_variable, member_name);
                }
                case Class::member_flag::property : {
                    return make_shared<property_accessor>(sp_variable, member_name);
                }
                    
                default:
                    assert(false);
            }
            
            assert(false); ///unreachable
        }
        
        result_type operator()(shared_ptr<Function> sp_fun, const std::string &member_name){
            assert(false); ///TODO
        }
        
        result_type operator()(shared_ptr<IConstant> sp_constant, const std::string &member_name){
            if (sp_constant->type()->isClass()){
                auto spClass = dynamic_pointer_cast<Class>(sp_constant->type());
                if (!spClass){
                    assert(false);
                }
                
                auto sp_var = make_shared<Variable>(spClass, _sp_current_block->parent());
                sp_var->set(sp_constant, _sp_current_block);
                
                return (*this)(static_pointer_cast<VariableBase>(sp_var), member_name);
            }
            
            assert(false);
        }
        
        result_type operator()(shared_ptr<IValue> spValue, const std::string &member_name){
            auto sp_var = make_shared<Variable>(spValue->type(), _sp_current_block->parent());
            sp_var->set(spValue, _sp_current_block);
            sp_var->writeable(false);
            sp_var->readable(true);
            return (*this)(static_pointer_cast<VariableBase>(sp_var), member_name);
        }
        
        result_type operator()(const AddressList &x, const std::string &member_name){
            assert(false);
        }
        
    private:
        shared_ptr<IBasicBlock>            _sp_current_block;
    };

    
}}















