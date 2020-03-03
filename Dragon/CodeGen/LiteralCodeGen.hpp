#pragma once

#include <memory>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/get.hpp>

#include <Dragon/CodeGen/Builder.hpp>
#include <Dragon/AST/Literal.hpp>

namespace Dragon{namespace CodeGen{
    
    using std::shared_ptr;
    using std::make_shared;
    
    using std::dynamic_pointer_cast;
    using boost::apply_visitor;
    using boost::get;
    namespace IR = Dragon::IR;
    
    class LiteralCodeGen{
    public:
        typedef shared_ptr<IR::IConstant>   result_type;
        
        LiteralCodeGen(shared_ptr<IR::IContext> spContext): _spContext(spContext){}
        
        result_type operator()(const AST::Literal &ast) const{
            return apply_visitor(*this, ast);
        }
        
        result_type operator()(AST::IntegerLiteral ast) const {
            return make_shared<IR::Constant<IR::IntType>>(ast.value, _spContext->getType("int"));
        }
        
        result_type operator()(AST::FloatingPointLiteral ast) const {
            return make_shared<IR::Constant<IR::FloatType>>(ast.value, _spContext->getType("double"));
        }
        
        result_type operator()(AST::StringLiteral ast) const {
//            return make_shared<IR::Constant<IR::BoolType>>(ast.value, _spContext->getType("bool"));
        }
        
//        result_type operator()(ast::char_pattern x) const{
//            return make_shared<IR::Constant<IR::ByteType>>(x.value, _spContext->getType("char"));
//        }
        
    private:
        shared_ptr<IR::IContext>            _spContext;
    };
    
    
}}