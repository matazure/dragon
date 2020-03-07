#pragma once

#include <functional>
#include <vector>
#include <Dragon/Config.hpp>

namespace Dragon{
    
    namespace detail{
        class CompilerImp;
    }
    
    namespace Issue{
        class IssueBase;
    }

    class Compiler{
    public:
        typedef void                    result_type;
        
        typedef std::vector<shared_ptr<Issue::IssueBase>>::iterator         issue_pointer_iterator;
        typedef std::vector<shared_ptr<Issue::IssueBase>>::const_iterator   const_pointer_issue_iterator;
        typedef std::vector<shared_ptr<Issue::IssueBase>>::value_type       issue_pointer_value_type;

        Compiler(bool );
        void require_buildin();
        bool execute(const std::string &code);
        void dumpIssue();
        void clearIssue();
        
        void dump();
        
        void operator()(const std::string &code);

        void set_out_hook(std::function<void (std::string)> outhook);
        void add_search_path(const std::string &p);
        
        issue_pointer_iterator issue_pointer_begin();
        issue_pointer_iterator issue_pointer_end();
        issue_pointer_value_type issue_pointer_front();
        issue_pointer_value_type issue_pointer_back();

    private:
        std::shared_ptr<detail::CompilerImp>                spCompilerImp;
        std::function<void (std::string)>                   _outputHook;
    };

}
