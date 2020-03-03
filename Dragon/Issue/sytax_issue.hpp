#pragma once

#include <vector>
#include <boost/property_tree/json_parser.hpp>
#include <Dragon/Issue/base.hpp>

namespace Dragon{namespace Issue{

    class sytax_issue: public IssueBase{
    public:
        sytax_issue(const std::string &type, const std::string &message): _type(type), _message(message){
        }

        virtual std::string type() const{
            return _type;
        }

        virtual std::string message() const{
            return _message;
        }

    private:
        std::string                     _type;
        std::string                     _message;
    };

}}
