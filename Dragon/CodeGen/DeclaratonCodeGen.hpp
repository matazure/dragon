#pragma once

#include <Dragon/CodeGen/PatternCodeGen.hpp>
#include <Dragon/CodeGen/ExpressionCodeGen.hpp>
#include <Dragon/CodeGen/Utilities.hpp>

namespace Dragon{namespace CodeGen{
    
    class PatternInitializerCodeGen{
    public:
        typedef void result_type;
        
        PatternInitializerCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder),
            _spTypeCodeGen(make_shared<TypeCodeGen>(spBuilder)),
            _spExpressionCodeGen(make_shared<ExpressionCodeGen>(spBuilder)){}
        
        result_type operator()(AST::IdentifierPattern pattern, optional<AST::Expression> optAstInitializer){
            auto optInitializer = (*_spExpressionCodeGen)(optAstInitializer);
            shared_ptr<IType> spType(nullptr);
            auto optSpType = (*_spTypeCodeGen)(pattern.optTypeAnnotation);
            if (optSpType){
                spType = *optSpType;
            }else if(optInitializer){
                spType = getAddressType(*optInitializer);
            }else{
                assert(false);
            }

            shared_ptr<VariableBase> spVariable(nullptr);
            if (_spBuilder->inFunctionScope()){
                spVariable = _spBuilder->createVariable(spType);
            }else{
                spVariable = _spBuilder->createGlobalVariable(spType);
            }

            if (optInitializer){
                spVariable->set(getAddressRValue(*optInitializer), _spBuilder->currentBlock());
            }
            
            _spBuilder->put(pattern.identifier, spVariable);
        }
        
        //todo
        
    private:
        shared_ptr<Builder>                     _spBuilder;
        shared_ptr<TypeCodeGen>                 _spTypeCodeGen;
        shared_ptr<ExpressionCodeGen>           _spExpressionCodeGen;
    };

    
    class FunctionDeclarationCodeGen{
    public:
        FunctionDeclarationCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder){}
        
        void operator()(const AST::FunctionDeclaration &ast){
//            auto sp_tmp_domain = _spBuilder->createScope();
//            auto domain_scope = Builder::insertScope(_spBuilder, sp_tmp_domain);
//            
//            TypeCodeGen typeCodeGen(_spBuilder);
//            auto result = typeCodeGen(ast.signature.result);
            
//            std::vector<std::string> arg_ids;
//            for (auto &arg: x.parameters){
//                auto arg_type_address = apply_visitor(expr_compiler, arg.type);
//                auto sp_type = detail::address2type(arg_type_address);
//                arg_types.push_back(sp_type);
//                arg_ids.push_back(arg.id);
//            }
            
            //create function prototype
            //std::string fun_name = x.name + template_suffix + suffix(arg_types);
//            auto sp_fun_type = make_shared<ir::function_type>(sp_return_type, arg_types);
//            auto sp_fun = make_shared<function>(sp_fun_type, sp_tmp_domain->parent(), x.name);
//            
//            auto addr = sp_tmp_domain->parent()->get_address(x.name);
//            if (addr.which() == address_id<blank>::value){
//                auto sp_fc = make_shared<function_collection>(x.name);
//                sp_fc->add(sp_fun);
//                sp_tmp_domain->parent()->set_address(x.name, sp_fc);
//            }else{
//                if (addr.which() != address_id<function_collection>::value){
//                    throw issue::redefined_identifier(x.name);
//                }else{
//                    auto sp_fc = boost::get<shared_ptr<function_collection>>(addr);
//                    sp_fc->add(sp_fun);
//                }
//            }
//            
//            //sp_tmp_domain->parent()->set_address(x.name, sp_fun);
//            _spBuilder->push_function(sp_fun);
//            
//            //create function arguments
//            auto arguments = sp_fun->arguments();
//            for (size_t i = 0; i < arguments.size(); ++i){
//                _spBuilder->set_address(arg_ids[i], arguments[i]->address());
//            }
//            
//            //create entry block(must needed block)
//            auto sp_entry_block = make_shared<basic_block>(sp_fun);
//            {
//                auto sp_tmp_domain = _spBuilder->createScope();
//                auto domain_scope = builder::insertScope(_spBuilder, sp_tmp_domain);
//                _spBuilder->current_block(sp_entry_block);
//                
//                _spBuilder->body_terminators.push(body_terminator());
//                _spBuilder->body_terminators.top().push_back(terminate_type::none);
//                
//                //implement function
//                //try{
//                auto sp_stat =  (*this)(x.body);
//                
//                if (!_spBuilder->body_terminators.top().returned()){
//                    if (sp_return_type != _spBuilder->context()->type("void")){
//                        assert(false);
//                    }else{
//                        _spBuilder->create_return_void();
//                    }
//                }
//                //                    }catch(compile_error &e){
//                //                        throw e;
//                //                    }
//                
//                _spBuilder->body_terminators.top().pop_back();
//                _spBuilder->body_terminators.pop();
//            }
//            _spBuilder->pop_function();
//            
//            return sp_fun;
        }
        
    private:
        shared_ptr<Builder> _spBuilder;
    };
    

    class DeclarationCodeGen{
    public:
        typedef void result_type;

        DeclarationCodeGen(shared_ptr<Builder> spBuilder)
        :   _spBuilder(spBuilder),
            _spTypeCodeGen(make_shared<TypeCodeGen>(spBuilder)),
            _spExpressionCodeGen(make_shared<ExpressionCodeGen>(spBuilder)),
            _spPatternInitialzierCodeGen(make_shared<PatternInitializerCodeGen>(spBuilder)){}

        result_type operator()(const AST::Declaration &ast){
            apply_visitor(*this, ast);
        }

        result_type operator()(const AST::PatternInitializer &ast){
            auto wrapOptInitializer = variant<optional<AST::Expression>>(ast.optInitializer);
            apply_visitor(*_spPatternInitialzierCodeGen, ast.pattern, wrapOptInitializer);
        }

        result_type operator()(const AST::VariableDeclaration &ast){
            for (auto patternInitializer: ast.patternInitializers){
                (*this)(patternInitializer);
            }
        }

        result_type operator()(const AST::ConstantDeclaration &ast){

        }

        result_type operator()(const AST::FunctionDeclaration &ast){
            assert(false);
        }

    private:
        shared_ptr<Builder>                     _spBuilder;
        shared_ptr<TypeCodeGen>                 _spTypeCodeGen;
        shared_ptr<ExpressionCodeGen>           _spExpressionCodeGen;
        shared_ptr<PatternInitializerCodeGen>           _spPatternInitialzierCodeGen;
    };


}}
