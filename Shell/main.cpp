#include <iostream>
#include <Dragon/Compiler/Compiler.h>


int main(int argc, char *argv[]){    
    std::cout << "start11 from " << argv[0] << std::endl;

    Dragon::Compiler compiler(true);
    compiler.set_out_hook([](std::string s){
        std::cout << s << std::endl;
    });


    while(true){
        std::string command = "";
        std::cin.clear();
        std::getline(std::cin, command);
        
        if (command == "\\dump"){
            compiler.dump();
            continue;
        }
        
//        if (command == "\\function"){
//            compiler.execute("function test(arg: int)->");
//            continue;
//        }
        
        compiler.execute(command);
        compiler.dumpIssue();
        compiler.clearIssue();
    }

    return 0;
}

//
//#include <boost/variant/recursive_variant.hpp>
//
//struct test1;
//
//typedef boost::variant<int, double> test1;
//typedef boost::variant<test1, int, float> test2;
//struct test3: public boost::variant<test1, test2, std::string>{
//    
//};
//
//struct Fun{
//    typedef  void  result_type;
//    
//    result_type operator()(test1){
//        std::cout << "test1" << std::endl;
//    }
//    
//    result_type operator()(int){
//        std::cout << "int" << std::endl;
//    }
//

//    result_type operator()(float){
//        std::cout << "float" << std::endl;
//    }
//};
//
//int main(){
//    auto  a = test2(test1(1));
//    auto  b = test2(1);
//    auto  c = test2(3.1415f);
////    auto d = test3(c);
//    auto fun = Fun();
//    boost::apply_visitor(fun, a);
//    boost::apply_visitor(fun, b);
//    boost::apply_visitor(fun, c);
////    auto  b =
//}


