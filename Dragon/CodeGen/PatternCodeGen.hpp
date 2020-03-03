#pragma once

#include <Dragon/CodeGen/TypeCodeGen.hpp>
#include <Dragon/CodeGen/Builder.hpp>

namespace Dragon{namespace CodeGen{
    
    class PatternCodeGen{
    public:
        typedef IR::Pattern                 result_type;
      
        PatternCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder),
            _spTypeCodeGen(make_shared<TypeCodeGen>(spBuilder)){}
        
        result_type operator()(const AST::Pattern &ast){
            return apply_visitor(*this, ast);
        }
        
        result_type operator()(const AST::IdentifierPattern &ast){
            IdentifierPattern re;
            re.identifier = ast.identifier;
            re.optSpTypeAnnotation = (*_spTypeCodeGen)(ast.optTypeAnnotation);
            return re;
        }
        
    private:
        shared_ptr<Builder>         _spBuilder;
        shared_ptr<TypeCodeGen>     _spTypeCodeGen;
    };
    
}}