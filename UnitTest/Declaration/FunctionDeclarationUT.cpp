#include <gtest/gtest.h>
#include <Dragon/Compiler/Compiler.h>
#include <Dragon/Parser/parse.h>

class FunctionDeclartionUT : public testing::Test{
protected:
    FunctionDeclartionUT(): compiler(true){
//        compiler.require_buildin();
    }
    
    Dragon::Compiler    compiler;
};


TEST_F(FunctionDeclartionUT, TestOneArgumentFunction){
//    auto code = "function test()->int{ return arg; };";

    auto code = "if (true){} ;";
    EXPECT_TRUE(compiler.execute(code));
    compiler.dumpIssue();
}
