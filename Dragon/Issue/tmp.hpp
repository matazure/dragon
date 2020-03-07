#pragma once

#include <Dragon/Issue/tmp.hpp>

#include <Dragon/IR/Interface.hpp> //todo

namespace Dragon{

    class configure_error: public std::logic_error{
    public:
        explicit configure_error(const std::string &info): std::logic_error(info){}
    };

    class CompileError : public std::runtime_error{
    public:
        explicit CompileError(const std::string &info): std::runtime_error(info){}

        std::vector<size_t>         position_ids;
    };

    class assignment_types_not_matched : public CompileError{
    public:
        assignment_types_not_matched(shared_ptr<IR::IType> sp_lhs, shared_ptr<IR::IType> sp_rhs):
        CompileError("assignment type not matched"), sp_lhs_type(sp_lhs), sp_rhs_type(sp_rhs){}

        shared_ptr<IR::IType>       sp_lhs_type;
        shared_ptr<IR::IType>       sp_rhs_type;
    };

//    class invalid_assignment: public CompileError{
//    public:
//
//        invalid_assignment(shared_ptr<IR::IType> sp_lhs, shared_ptr<IR::IType> sp_rhs):
//        CompileError("invalid assignment"), sp_lhs_type(sp_lhs), sp_rhs_type(sp_rhs){}
//
//        shared_ptr<IR::IType>       sp_lhs_type;
//        shared_ptr<IR::IType>       sp_rhs_type;
//    };

    class index_error: public CompileError{
    public:
        index_error(const std::string &msg): CompileError(msg){}
    };

    class invalid_pointer_indexer: public index_error{
    public:
        invalid_pointer_indexer(): index_error("the indexer value type must be int type"){ }
    };

    class invalid_array_indexer : public index_error{
    public:
        invalid_array_indexer() : index_error("the indexer value type must be int type"){}
    };

    class no_matched_indexer: public index_error{
    public:
        no_matched_indexer(): index_error("no matched indexer"){}
    };

    class matrix_format_error : public CompileError{
    public:
        matrix_format_error(const std::string &e): CompileError(e){}
    };

    class matrix_column_size_not_matched: public matrix_format_error{
    public:
        matrix_column_size_not_matched(): matrix_format_error("the matrix column size is not matched"){}
    };

    class file_not_found: public CompileError{
    public:
        file_not_found(const std::string &file): CompileError(file+" is not found"){}
    };

    class no_matched_operator: public CompileError{
    public:
        no_matched_operator(): CompileError("no matched operator. "){}

    };
}
