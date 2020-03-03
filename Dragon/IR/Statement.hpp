//
//  statements.hpp
//  binding
//
//  Created by 张志敏 on 12/11/13.
//
//

#pragma once

namespace Dragon{ namespace IR{
    
//    class statement : public istatement{
//    public:
//        statement(bool is_terminated, shared_ptr<IR::IValue> spValue)
//        : _is_terminated(is_terminated), _sp_value(spValue){}
//        
//        statement(bool is_terminated)
//        : _is_terminated(is_terminated), _sp_value(nullptr){}
//        
//        virtual bool is_terminated() const              { return _is_terminated; }
//        virtual shared_ptr<IR::IValue> value() const    { assert(_sp_value); return _sp_value; }
//    private:
//        bool    _is_terminated;
//        shared_ptr<IR::IValue> _sp_value;
//    };
//    
//    class statement_list : public istatement{
//    public:
//        statement_list(){
//            _stat_list.push_back(make_shared<statement>(false));
//        }
//        
//        void add(shared_ptr<istatement> sp_stat){
//            if (_stat_list.back()->is_terminated()){
//                std::cout <<"warning: the remaining codes are ingored.\n";
//            }
//            
//            _stat_list.push_back(sp_stat);
//        }
//        
//        virtual bool is_terminated() const{
//            for (auto stat: _stat_list){
//                if (stat->is_terminated()){
//                    return true;
//                }
//            }
//            
//            return false;
//        }
//        
//        virtual shared_ptr<IR::IValue> value() const{
//            assert(false);
//        }
//        
//    private:
//        std::list<shared_ptr<istatement>>   _stat_list;
//    };
//    
//    class condition_statement : public istatement{
//    public:
//        condition_statement(shared_ptr<istatement> then_stat, shared_ptr<istatement> else_stat)
//        : _then_stat(then_stat), _else_stat(else_stat){}
//        
//        virtual bool is_terminated() const{
//            assert(_then_stat);
//            if (_else_stat){
//                return _then_stat->is_terminated() && _else_stat->is_terminated();
//            }else{
//                return false;
//            }
//        }
//        
//        virtual shared_ptr<IR::IValue> value() const{
//            assert(false);
//        }
//        
//    private:
//        shared_ptr<istatement>  _then_stat;
//        shared_ptr<istatement>  _else_stat;
//    };
//    
    
}}