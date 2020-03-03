#pragma once

#include <Dragon/CodeGen/Builder.hpp>

namespace Dragon{namespace CodeGen{

    class TypeCodeGen{
    public:
        typedef shared_ptr<IType>       result_type;
        
        TypeCodeGen(shared_ptr<Builder> spBuilder): _spBuilder(spBuilder){}
        
        result_type operator()(const AST::Type &ast){
            return apply_visitor(*this, ast);
        }
        
        optional<result_type> operator()(const optional<AST::TypeAnnotation> &ast){
            if (ast){
                return (*this)(*ast);
            }else{
                return nullptr;
            }
        }
        
        result_type operator()(const AST::IdentifierType &ast){
            return _spBuilder->getType(ast);
        }
        
        result_type operator()(const AST::ArrayType &ast){
            auto type = (*this)(ast.type);
            return PointerType::get(type);
        }
        
        result_type operator()(const AST::FunctionType &ast){
//            auto type = (*this)(ast.return
            auto argumentType = (*this)(ast.argumentType);
            auto returnType = (*this)(ast.returnType);
            assert(false);
//            return make_shared<FunctionType>(argumentType, returnType);
        }
        
    private:
        shared_ptr<Builder> _spBuilder;
    };

}}
