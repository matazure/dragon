//
//  user.hpp
//  binding
//
//  Created by 张志敏 on 10/29/13.
//
//

#pragma once

#include <string>

namespace Dragon{namespace IR{
    
    
    ///\brief   The type and value .. will drieved from this class.
    class user{
    public:
        user() {}
        user(const std::string &name) : _name(name) {}
        user(const user &rhs) : _name(rhs._name){}
        user(user &&rhs): _name(move(rhs._name)){}
        
        user& operator=(const user &rhs) = delete;
        
        void name(const std::string &name)              { _name = name; }
        std::string name() const                        { return _name; }
        
    protected:
        std::string _name;
    };
    
    
}}
