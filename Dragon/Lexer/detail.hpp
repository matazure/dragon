#pragma once

#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/lex_lexertl.hpp>
#include <boost/spirit/include/lex_lexertl_position_token.hpp>
#include <boost/lexical_cast.hpp>
#include <Dragon/AST/Statement.hpp>

namespace boost{namespace spirit{namespace traits{
    
    namespace detail{
        inline char get_escape_char(char c){
            switch(c){
                case 'a':{
                    return '\a';
                }
                case 'b': {
                    return '\b';
                }
                case 'e':{
                    return '\e';
                }
                case 'f': {
                    return '\f';
                }
                case 'n': {
                    return '\n';
                }
                case 'r' :{
                    return '\r';
                }
                case 't': {
                    return '\t';
                }
                case 'v': {
                    return '\v';
                }
                case '\\':{
                    return  '\\';
                }
                case '\'': {
                    return  '\'';
                }
                case '0':{
                    return '\0';
                }
                case '"':{
                    return  '\"';
                }
                default:
                    assert(false); ///todo
            }
        }
    }
    
//    struct invalied132{};
//    
//    template <typename Iterator>
//    struct assign_to_attribute_from_iterators<typename std::conditional<std::is_same<Dragon::int_t, long long>::value, long long, invalied132>::type, Iterator>{
//        static void
//        call(const Iterator &first, const Iterator &last, Dragon::int_t &v){
//            std::string num_str(first, last);
//            v = boost::lexical_cast<Dragon::int_t, std::string>(num_str);
//        }
//    };
    
//    template <typename Iterator>
//    struct assign_to_attribute_from_iterators<Dragon::ast::char_pattern, Iterator>{
//        static void
//        call(const Iterator &first1, const Iterator &last1, Dragon::ast::char_pattern &v){
//            auto tmp = std::string(first1,last1);
//            auto first = tmp.begin();
//            auto last = tmp.end();
//            if((last-first) == 3){
//                v.value = *(first+1);
//                return;
//            }
//            if((last-first) == 4){
//                assert(*(first+1) == '\\');  ///todo
//                v.value = detail::get_escape_char(*(first+2));
//                return;
//            }
//            
//            assert(false);
//        }
//    };
    
    template <typename Iterator>
    struct assign_to_attribute_from_iterators<Dragon::AST::StringLiteral, Iterator>{
        static void
        call(const Iterator &first, const Iterator &last, Dragon::AST::StringLiteral &v){
            auto tmp = std::string(first, last);
            for (auto it = tmp.begin() + 1; it != tmp.end()-1; ++it){
                if (*it != '\\'){
                    v.value.push_back(*it);
                }else{
                    ++it;
                    //classic_escapes
                    v.value.push_back(detail::get_escape_char(*it));
                }
            }
        }
    };
    
//    template <typename Iterator>
//    struct assign_to_attribute_from_iterators<Dragon::ast::bool_pattern, Iterator>{
//        static void
//        call(const Iterator &first, const Iterator &last, Dragon::ast::bool_pattern &v){
//            auto tmp = std::string(first, last);
//            if (tmp == "true"){
//                v.value =true;
//                return;
//            }
//            if (tmp == "false"){
//                v.value = false;
//                return;
//            }
//            
//            assert(false);
//            // v.value = boost::lexical_cast<Dragon::bool_t>(tmp);
//        }
//    };
    
    template <typename Iterator>
    struct assign_to_attribute_from_iterators<Dragon::AST::IntegerLiteral, Iterator>{
        static void
        call(const Iterator &first, const Iterator &last, Dragon::AST::IntegerLiteral &v){
            auto tmp = std::string(first,last);
            v.value = boost::lexical_cast<Dragon::int_t>(tmp);
        }
    };
    
    template <typename Iterator>
    struct assign_to_attribute_from_iterators<Dragon::AST::FloatingPointLiteral, Iterator>{
        static void
        call(const Iterator &first, const Iterator &last, Dragon::AST::FloatingPointLiteral &v){
            auto tmp = std::string(first,last);
            v.value = boost::lexical_cast<Dragon::float_t>(tmp);
        }
    };
     
}}}
