#pragma once

#include <Dragon/Parser/parse.h>
#include <Dragon/CodeGen/StatementCodeGen.hpp>
#include <Dragon/ExecutionEngine.hpp>
#include <Dragon/Issue/Issue.hpp>

namespace Dragon{
    const static int SUCCESS = 0;
    //const static int PARSE_ERROR = -1;
    //const static int COMPILE_ERROR = -2;
    const static int RUNTIME_ERROR = -3;
    const static int EXPECT_FAILED = - 4;
}

namespace Dragon{namespace detail{

    template <typename T> struct ctype2type;

    template <>
    struct ctype2type<void>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    { return _spBuilder->getType("void"); }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <>
    struct ctype2type<size_t>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    { return _spBuilder->getType("int"); }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <>
    struct ctype2type<char>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    { return _spBuilder->getType("char"); }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <>
    struct ctype2type<bool>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    { return _spBuilder->getType("bool"); }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };


    template <>
    struct ctype2type<unsigned int>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    { return _spBuilder->getType("int"); }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <typename  T>
    struct ctype2type<const T>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    {
            //do none
            ctype2type<T> t(_spBuilder);
            return t.type();
        }

    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };
    template <typename  T>
    struct ctype2type<T *>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}
        shared_ptr<IR::IType> type()    {
            ctype2type<T> t(_spBuilder);
            return IR::PointerType::get(t.type());
        }

    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <>
    struct ctype2type<void *>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}

        shared_ptr<IR::IType> type() {
            return IR::PointerType::get(_spBuilder->getType("char"));
        }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    template <>
    struct ctype2type<const void *>{
        ctype2type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}

        shared_ptr<IR::IType> type() {
            return IR::PointerType::get(_spBuilder->getType("char"));
        }
    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };

    struct get_argument_type{
        get_argument_type(shared_ptr<CodeGen::Builder> spBuilder): _spBuilder(spBuilder){}

        template <typename P>
        void operator()(P x){
            ctype2type<P> t(_spBuilder);
            parameter_types.push_back(t.type());
        }

        std::vector<shared_ptr<IR::IType>> parameter_types;

    private:
        shared_ptr<CodeGen::Builder> _spBuilder;
    };


    class CompilerImp{
    public:
        typedef void                    result_type;

        CompilerImp(bool is_terminal)
        :_isTerminal(is_terminal),
        _spModule(IR::create_buildin_module(IR::Context::create())),
        _spBuilder(new CodeGen::Builder(_spModule)),
        _spEngine(new ExecutionEngine(_spModule)),
        _spStatementCodeGen(new CodeGen::StatementCodeGen(_spBuilder))
        {
            _spBuilder->engine(_spEngine);
            //add ./ search path
//            add_search_path(".");
        }

//        void operator()(const AST::Statement &x){
//            boost::apply_visitor(*this, x);
//        }

        void operator()(AST::StatementList const& x){
            (*_spStatementCodeGen)(x);
        }
//
//
//        void operator()(const AST::Declaration &x){
////            (*_spStatementCodeGen)(x);
//        }
//
//        void operator()(const AST::Expression &x){
//            auto tmp_todo = _spBuilder->defineCommand();
//
////            auto addr = (*_spStatementCodeGen)(x);
////            auto spValue = (*_spStatementCodeGen).expr_compiler.get_value(ValueAddress);
////            ///<void type doesn't print
////            if (spValue->type() != _spBuilder->getType("void")){
////                _spBuilder->printResult(spValue);
////            }
//        }
//
//        void operator()(const AST::IfStatement &x){
//            auto tmp_todo = _spBuilder->defineCommand();
//            (*_spStatementCodeGen)(x);
//        }
//
//        void operator()(const AST::ForStatement &x){
//            auto tmp_todo = _spBuilder->defineCommand();
//            (*_spStatementCodeGen)(x);
//        }
//
//        void operator()(const AST::WhileStatement &x){
//            auto tmp_todo = _spBuilder->defineCommand();
//            (*_spStatementCodeGen)(x);
//        }
////
////        void add_search_path(const std::string &path){
////            _sp_filesystem_engine->add(make_shared<drive::local_drive>(path));
////        }
//
//        //    void Compiler::operator()(ast::assignment const& x){
//        //        assert(false);
//        //        auto tmp_todo = _spBuilder->defineCommand();
//        //
//        //        auto rhs_addr = (*_spStatementCodeGen).expr_compiler(x.rhs);
//        //        auto sp_rhs_value = (*_spStatementCodeGen).expr_compiler.get_value(rhs_addr);
//        //        //确保右值不是void类型
//        //        if (sp_rhs_value->type() == _spBuilder->getType("void")){
//        //            throw CompileError("rhs value couldn't be void");
//        //        }
//        //        //如果是在terminal,需要打印结果
//        //
//        //        _spBuilder->printResult(sp_rhs_value);
//        //
//        //        IR::Address lhs_addr;
//        //        try{
//        //            lhs_addr = (*_spStatementCodeGen).expr_compiler(x.lhs);
//        //        }catch(undefined_identifier &e){
//        //            auto sp_zero_initializer = zeroInitialize(sp_rhs_value->type());
//        //            auto sp_lhs_g_var = make_shared<CodeGen::GlobalVariable>(sp_rhs_value->type(), false, sp_zero_initializer, _spBuilder->currentScope());
//        //
//        //            lhs_addr = static_pointer_cast<IR::VariableBase>(sp_lhs_g_var);
//        //            _spBuilder->address(e.id, lhs_addr);
//        //        }
//        //
//        //        //assign value
//        //        try{
//        //            CodeGen::assign_helper assign(_spBuilder->currentBlock());
//        //            boost::apply_visitor(assign, rhs_addr, lhs_addr);
//        //        }catch(invalid_assignment &e){
//        //            e.position_id = x.position_id;
//        //            throw e;
//        //        }
//        //    }
//
////        void operator()(const ast::require_statement &x){
////            auto path = x.path.value;
////            if (drive::exists(path, *_sp_filesystem_engine)){
////                std::string code = drive::read_file(path, *_sp_filesystem_engine);
////                execute(code);
////            }else{
////                throw file_not_found(path);
////            }
////        }
//
//
//        void require_buildin(){
//            this->set_compiler_state(CodeGen::CompilerState::buildin);
//            this->execute("require (\"buildin_lib/buildin.d\");");
//            this->set_compiler_state(CodeGen::CompilerState::terminal);
//        }

        bool execute(std::string code){
            Dragon::AST::StatementList ast_tree;

            try{
                if (parse(code, ast_tree, issueHandler)){
                    (*this)(ast_tree);
                    return true;
                }else{
                    //todo
                    std::cout << "failed to parse. " << std::endl;
                    return false;
                }
            }catch(shared_ptr<Issue::CompileIssue> sp_issue){
                issueHandler(sp_issue);
                return false;
            }
        }

        void set_compiler_state(CodeGen::CompilerState state){
            _spBuilder->compiler_state_ = state;
        }

        template <typename FunType>
        shared_ptr<IR::IFunction> mapCFunction(FunType fun_ptr, std::string name){
            get_argument_type get_arg_type(_spBuilder);
            boost::mpl::for_each<boost::function_types::parameter_types<FunType>>(std::ref(get_arg_type));
            ctype2type<typename boost::function_types::result_type<FunType>::type> ct(_spBuilder);
            auto sp_ret_type = ct.type();
            auto sp_fun_type = make_shared<IR::FunctionType>(sp_ret_type, get_arg_type.parameter_types);
            auto sp_fun = _spEngine->mapCFunction(sp_fun_type, (void *)fun_ptr, name);
            _spBuilder->put(name, static_pointer_cast<IR::Function>(sp_fun));

            return sp_fun;
        }

        void dump(){
            _spModule->dump();
        }

    private:
        bool                                                            _isTerminal;
        
        shared_ptr<IR::Module>                                          _spModule;
        shared_ptr<CodeGen::Builder>                                    _spBuilder;
        shared_ptr<ExecutionEngine>                                     _spEngine;

        shared_ptr<CodeGen::StatementCodeGen>                           _spStatementCodeGen;

    public:
        std::function<void (std::string)>                               outputHook;
        issue_handler_type                                              issueHandler;
    };

}}
