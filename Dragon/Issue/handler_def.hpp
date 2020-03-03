#pragma once

#include "handler.hpp"
#include <boost/property_tree/json_parser.hpp>
#include <Dragon/Issue/base.hpp>
#include <Dragon/Issue/sytax_issue.hpp>

namespace Dragon{

    template <typename BaseIterator, typename Iterator>
    void IssueHandler<BaseIterator,  Iterator>::operator()(Iterator first, Iterator err_pos, boost::spirit::info what) const{
        Iterator err_pos1;
        if(err_pos == decltype(err_pos)()){
            err_pos1 = detail::get_last_iterator(first);
        }else{
            err_pos1 = err_pos;
        }
        BaseIterator err_pos_base = err_pos1->begin();
        auto pos = err_pos_base.get_position();
        std::stringstream ss;
        ss << "expecting " << what;

        std::cout << ss.str() << std::endl;
     
        auto  sp_parse_error = make_shared<Issue::sytax_issue>("parse error", ss.str());
        Issue::position position = {pos.line, pos.column, err_pos_base.get_currentline()};
        sp_parse_error->push_position(position);
        
        sp_issues->push_back(sp_parse_error);
    }

    template <typename BaseIterator, typename Iterator>
    void IssueHandler<BaseIterator, Iterator>::operator()(shared_ptr<Issue::CompileIssue> sp_issue){
        assert(sp_issue->position_ids.front() >= 0 && sp_issue->position_ids.front() < sp_iters->size());
        auto iter = std::get<0>((*sp_iters)[sp_issue->position_ids.front()]);
        auto err_pos_base = iter->begin();
        auto pos = err_pos_base.get_position();
        Issue::position position = {pos.line, pos.column, err_pos_base.get_currentline()};
        sp_issue->push_position(position);
        
        sp_issues->push_back(sp_issue);
    }

}
