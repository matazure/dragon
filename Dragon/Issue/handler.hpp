#pragma once

#include <boost/spirit/include/support_info.hpp>
#include <Dragon/AST/Statement.hpp>
#include <Dragon/Issue/base.hpp>

namespace Dragon{

    namespace detail{
        template <typename Iterator>
        inline  Iterator get_last_iterator(Iterator iter){
            assert(iter != Iterator());
            auto iter_tmp = iter;
            while (iter != Iterator()){
                iter_tmp = iter;
                ++iter;
            }

            return iter_tmp;
        }
    }

    template <typename BaseIterator, typename Iterator>
    struct IssueHandler{
        template <typename, typename, typename>
        struct result { typedef void type; };

        IssueHandler()
        :   sp_iters(new std::vector<std::tuple<Iterator, Iterator>>),
        sp_issues(new std::vector<shared_ptr<Issue::IssueBase>>){}

        void operator()(Iterator first, Iterator err_pos, boost::spirit::info what) const;
        void operator()(shared_ptr<Issue::CompileIssue> sp_issue);

        shared_ptr<std::vector<std::tuple<Iterator, Iterator>>>                     sp_iters;
        mutable shared_ptr<std::vector<shared_ptr<Issue::IssueBase>>>              sp_issues;
    };


    template <typename Iterator>
    struct annotation{
        template <typename, typename, typename>
        struct result { typedef void type; };

        shared_ptr<std::vector<std::tuple<Iterator, Iterator>>> sp_iters;
        annotation(shared_ptr<std::vector<std::tuple<Iterator, Iterator>>> sp_iters)
        : sp_iters(sp_iters) {}

        struct set_id{
            typedef void result_type;

            int id;
            set_id(int id) : id(id) {}

            void operator()(int_t) const{}
            void operator()(float_t) const{}
            void operator()(bool_t) const{}

            template <typename T>
            void operator()(T& x) const{
                x.position_id = id;
            }
        };
        
        template <typename T>
        void operator()(T &t, Iterator first, Iterator last) const{
            
        }


//        void operator()(ast::operand& ast, Iterator first, Iterator last) const{
//            int id = sp_iters->size();
//            sp_iters->push_back(std::make_tuple(first, last));
//            boost::apply_visitor(set_id(id), ast);
//        }
//
//        template <typename T>
//        void operator()(T &t, Iterator first, Iterator last) const{
//            t.position_id = sp_iters->size();
//            sp_iters->push_back(std::make_tuple(first, last));
//        }
//
//        void operator()(AST::Statement &stat, Iterator first, Iterator last) const{
//            int id = sp_iters->size();
//            boost::apply_visitor(set_id(id), stat);
//            sp_iters->push_back(std::make_tuple(first, last));
//        }
//        
//        void operator()(AST::Identifier &id, Iterator first, Iterator last) const{
//            id.position_id = sp_iters->size();
//            sp_iters->push_back(std::make_tuple(first, last));
//        }
//
//        void operator()(ast::optoken &op, Iterator first, Iterator last) const{
//            op.position_id = iters.size();
//            iters.push_back(std::make_tuple(first, last));
//        }

    };

}
