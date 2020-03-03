#include <Dragon/Compiler/Compiler.h>
#include <setjmp.h>
#include <Dragon/Compiler/CompilerImp.hpp>

namespace Dragon{
    
    typedef std::function<void (std::string)> out_hook_type;
    
    jmp_buf jmp;
    std::function<void (std::string)> globalOutHook = [](std::string s){
//        std::cout << s << std::endl;
    };

    std::string generateJsonMessage(std::string type, std::string data){
        return  "{\"type\":\"" +  type + "\"," + "\"data\": "+ data +"}";
    }
    void plot_c(const char *s){
        assert(globalOutHook);
        globalOutHook(generateJsonMessage("plot", std::string(s)));
    }

    void error_c(const char *s){
        assert(globalOutHook);
        globalOutHook(generateJsonMessage("runtime-error", std::string(s)));
        longjmp(jmp, RUNTIME_ERROR);
    }

    void print_c(const char *s){
        assert(globalOutHook);
        globalOutHook(generateJsonMessage("print", std::string(s)));
    }
    
    void output_c(const char *s){
        assert(globalOutHook);
        globalOutHook(generateJsonMessage("output", std::string(s)));
    }
    
    void assert_c(bool v, const char *s){
        if (!v){
            assert(globalOutHook);
            globalOutHook(generateJsonMessage("expected failed", s));
            longjmp(jmp, EXPECT_FAILED);
        }
    };
    
    void expect_c(bool v, const char *s){
        if (!v){
            assert(globalOutHook);
            globalOutHook(generateJsonMessage("expected failed", s));
        }
    };
    
    Compiler::Compiler(bool is_terminal){
        LLVMInitializeNativeTarget();
//        spCompilerImp = make_shared<detail::CompilerImp>(sp_fs_engine, is_terminal);
        spCompilerImp = make_shared<detail::CompilerImp>(is_terminal);

        spCompilerImp->mapCFunction(&malloc, "malloc");
        spCompilerImp->mapCFunction(&memcpy, "memcpy");
        spCompilerImp->mapCFunction(&plot_c, "plot_c");
        spCompilerImp->mapCFunction(&error_c, "error_c");
        spCompilerImp->mapCFunction(&print_c, "print_c");
        spCompilerImp->mapCFunction(&output_c, "output_c");
        spCompilerImp->mapCFunction(&expect_c,  "expect_c");
        
        set_out_hook([](const std::string &s){
            std::cout << s << std::endl;
        });
    }
    
    void Compiler::require_buildin(){
//        spCompilerImp->require_buildin();
//        
//        for (auto sp_issue: *(spCompilerImp->issueHandler.sp_issues)){
//            _outputHook(sp_issue->serialize());
//        }
//        spCompilerImp->issueHandler.sp_issues->clear();
    }
    
    void Compiler::set_out_hook(out_hook_type out_hook){
        _outputHook = out_hook;
        globalOutHook = out_hook;
    }
    
    bool Compiler::execute(const std::string &code){
        return spCompilerImp->execute(code);
    }
    
    void Compiler::operator()(const std::string &code){
        execute(code);
        dumpIssue();
        clearIssue();
        
        if (_outputHook)
            _outputHook("{\"type\":\"end\"}");
    }
    
    void Compiler::dump(){
        spCompilerImp->dump();
    }
    
    void Compiler::dumpIssue(){
        for (auto sp_issue: *(spCompilerImp->issueHandler.sp_issues)){
            _outputHook(sp_issue->serialize());
        }
    }
    
    void Compiler::clearIssue(){
        spCompilerImp->issueHandler.sp_issues->clear();
    }
    
    Compiler::issue_pointer_iterator Compiler::issue_pointer_begin(){
        return spCompilerImp->issueHandler.sp_issues->begin();
    }
    
    Compiler::issue_pointer_iterator Compiler::issue_pointer_end(){
        return spCompilerImp->issueHandler.sp_issues->end();
    }
    
    Compiler::issue_pointer_value_type Compiler::issue_pointer_front(){
        return spCompilerImp->issueHandler.sp_issues->front();
    }
    
    Compiler::issue_pointer_value_type Compiler::issue_pointer_back(){
        return spCompilerImp->issueHandler.sp_issues->back();
    }
    
//    void Compiler::add_search_path(const std::string &path){
//        spCompilerImp->add_search_path(path);
//    }
 
}
