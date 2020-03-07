#pragma once

#include <exception>
#include <vector>
#include <boost/property_tree/json_parser.hpp>
#include <Dragon/Config.hpp>

namespace Dragon{namespace Issue{

    struct position{
        int             line;
        int             column;
        std::string     currentline;
    };

    class IssueBase{
    public:
        void push_position(position p){
            positions.push_back(p);
        }

        virtual std::string type() const = 0;
        virtual std::string message() const = 0;
        
        virtual std::string serialize() const{
            boost::property_tree::ptree pt, pt_positions;
            pt.put("type", type());
            pt.put("message", message());
            for (auto it = positions.rbegin(); it != positions.rend(); ++it){
                boost::property_tree::ptree pc;
                pc.put("line", it->line);
                pc.put("column", it->column);
                pc.put("currentline", it->currentline);
                pt_positions.push_back(std::make_pair("", pc));
            }
            pt.put_child("positions", pt_positions);

            std::stringstream ss;
            boost::property_tree::json_parser::write_json(ss, pt, false);
            return std::move(ss.str());
        }

        std::vector<position>           positions;
    };
    
    class CompileIssue : public IssueBase{
    public:
        std::vector<size_t>             position_ids;
    };
    
    class CompileError: public CompileIssue{
    public:
        CompileError(){}
        CompileError(std::string msg): _msg(msg){}
        
        virtual std::string type() const    { return "compile error"; }
        
        virtual std::string message() const {
            return _msg;
        }
        
    protected:
        std::string _msg;
    };
    
    class CompileWarning: public CompileIssue{
    public:
        virtual std::string type() const    { return "compile warning"; }
    };
    
}}


