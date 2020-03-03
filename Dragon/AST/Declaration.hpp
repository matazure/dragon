#pragma once


#include <Dragon/AST/Tag.hpp>
#include <Dragon/AST/Attribute.hpp>
#include <Dragon/AST/Pattern.hpp>
#include <Dragon/AST/Expression.hpp>
#include <Dragon/AST/Type.hpp>

namespace Dragon{namespace AST{
    
    struct DeclarationSpecifier: Tag, string{
        DeclarationSpecifier(): string(){}
        DeclarationSpecifier(const string &s): string(s){}
        DeclarationSpecifier(const char *s): string(s){}
        template <typename Iter>
        DeclarationSpecifier(Iter it_begin, Iter it_end): string(it_begin, it_end){}
    };
    
  
    
    struct DeclarationSpecifiers: Tag, vector<DeclarationSpecifier>{};
    
    typedef DeclarationSpecifiers   DeclarationHeader;
    
//    struct DeclarationHeader:Tag{
////        vector<Attributes>                                attributes;
//        DeclarationSpecifiers                           specifiers;
//    };
    
    struct PatternInitializer: Tag{
        Pattern                                         pattern;
        optional<Expression>                            optInitializer;
    };
    
    struct ConstantDeclaration: Tag{
        DeclarationHeader                                   specifiers;
        vector<PatternInitializer>                          patternInitializers;
    };
    
    struct VariableDeclaration: Tag{
        DeclarationHeader                                   specifiers;
        vector<PatternInitializer>                          patternInitializers;
    };
    
    struct CodeBlock;
    
    struct FunctionDeclaration: Tag{
        typedef variant<Identifier, Operator>           Name;
        struct Parameter: Tag{
            Identifier                  name;
            TypeAnnotation              typeAnnotation;
            optional<Expression>        defaultArgument;
        };
        struct ParameterClause: Tag, std::vector<Parameter>{};
        typedef Type                                    Result;
        
        struct Signature{
            ParameterClause                             parameterClause;
            Result                                      result;
        };
        
        DeclarationHeader                               header;
        Name                                            name;
        Signature                                       signature;
        recursive_wrapper<CodeBlock>                    body;
    };
    
    typedef variant<ConstantDeclaration,
                    VariableDeclaration,
                    FunctionDeclaration>                Declaration;
    
}}


BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::PatternInitializer,
                          (Dragon::AST::Pattern, pattern)
                          (optional<Dragon::AST::Expression>, optInitializer)
                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::ConstantDeclaration,
                          (Dragon::AST::DeclarationHeader, specifiers)
                          (std::vector<Dragon::AST::PatternInitializer>, patternInitializers)
                          )

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::VariableDeclaration,
                          (Dragon::AST::DeclarationHeader, specifiers)
                          (std::vector<Dragon::AST::PatternInitializer>, patternInitializers)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::FunctionDeclaration::Parameter,
                          (Dragon::AST::Identifier, name)
                          (Dragon::AST::TypeAnnotation , typeAnnotation )
                          (optional<Dragon::AST::Expression>, defaultArgument)
)


BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::FunctionDeclaration::Signature,
                          (Dragon::AST::FunctionDeclaration::ParameterClause, parameterClause)
                          (Dragon::AST::FunctionDeclaration::Result, result)
)

BOOST_FUSION_ADAPT_STRUCT(
                          Dragon::AST::FunctionDeclaration,
                          (Dragon::AST::DeclarationHeader, header)
                          (Dragon::AST::FunctionDeclaration::Name, name)
                          (Dragon::AST::FunctionDeclaration::Signature, signature)
                          (recursive_wrapper<Dragon::AST::CodeBlock>, body)
)


