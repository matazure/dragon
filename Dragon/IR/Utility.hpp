//
//  utility.hpp
//  binding
//
//  Created by 张志敏 on 1/16/14.
//
//


#pragma once

#include <setjmp.h>
#include <boost/lexical_cast.hpp>
#include <Dragon/IR/DerivedType.hpp>
#include <Dragon/IR/Function.hpp>
#include <Dragon/AST/Statement.hpp>
#include <Dragon/Issue/Issue.hpp>

namespace Dragon{namespace IR{
    
    inline shared_ptr<IType> get_type(const Address x){
        switch (x.which()){
            case AddressId<IType>::value:
                return boost::get<shared_ptr<IType>>(x);
            case AddressId<IValue>::value:
                return boost::get<shared_ptr<IValue>>(x)->type();
            case AddressId<VariableBase>::value:{
                auto sp_var = boost::get<shared_ptr<VariableBase>>(x);
                return sp_var->valueType();
            }
            case AddressId<IConstant>::value:
                return boost::get<shared_ptr<IConstant>>(x)->type();
            default:
                assert(false);
        }
    }

    inline std::vector<shared_ptr<IType>> get_types(std::vector<Address> arguments){
        std::vector<shared_ptr<IType>> arg_types;
        for (auto arg: arguments){
            arg_types.push_back(get_type(arg));
        }

        return arg_types;
    }

    inline std::string suffix(std::vector<shared_ptr<IType>> ArgumentTypes){
        std::string re;
        for (auto sp_type: ArgumentTypes){
            re.append("/");
            re.append(sp_type->uid());
        }

        return re;
    }

    inline std::string suffix(std::vector<Address> args){
        std::string re;
        for (auto arg: args){
            re.append("/");
            re.append(get_type(arg)->uid());
        }

        return re;
    }


    inline std::string suffix(std::list<Address> args){
        std::string re;
        for (auto arg: args){
            re.append("/");
            re.append(get_type(arg)->uid());
        }

        return re;
    }


    inline std::string uid(Address x){
        switch (x.which()){
            case AddressId<IType>::value:
                return boost::get<shared_ptr<IType>>(x)->uid();
            default:
                assert(false); //unimplement
        }
    }

    inline std::string template_suffix(std::vector<Address> args){
        std::string re;
        for (auto arg: args){
            re.append("@");
            re.append(uid(arg));
        }

        return re;
    }

    inline std::string template_parameter_suffix(std::list<AST::Identifier> gen_params){
        std::string re;
        for (auto id: gen_params){
            re.append("#");
            re.append(id);
        }

        return re;
    }

    inline std::string abbr_suffix(const std::vector<shared_ptr<IType>> ArgumentTypes){
        std::string re;
        for (auto sp_type: ArgumentTypes){
            re.append(".");
            if (sp_type->isIntergral()){
                re.append("i");
                re.append(boost::lexical_cast<std::string>(sp_type->bitWidth()));
                continue;
            }
            if (sp_type->isFloatingPoint()){
                re.append("f");
                re.append(boost::lexical_cast<std::string>(sp_type->bitWidth()));
                continue;
            }
            assert(false);
        }

        return re;
    }

    namespace detail{
        struct state2addr{
            typedef Address result_type;

            template <typename T>
            result_type operator()(T x){
                assert(false);
            }


            result_type operator()(shared_ptr<IR::Function> x){
                assert(x);
                return x;
            }

            result_type operator()(shared_ptr<IR::Class> x){
                assert(x);
                return static_pointer_cast<IType>(x);
            }
        };
    }

    inline IR::Address statement2address(IR::statement x){

        detail::state2addr convertor_;
        return boost::apply_visitor(convertor_, x);
    }


    class type2suffix{
    public:
        type2suffix(llvm::LLVMContext &Context){
            _lut[llvm::Type::getDoubleTy(Context)] = ".f64";
            _lut[llvm::Type::getInt64Ty(Context)] = ".i64";
        }

        std::string operator()(llvm::Type *key) {
            if (_lut[key] == "")
                throw(std::runtime_error("The type has no suffix"));
            return _lut[key];
        }

    private:
        std::map<llvm::Type *, std::string>  _lut;
    };


    class get_value{
    public:
        typedef shared_ptr<IR::IValue> result_type;

        get_value(shared_ptr<IR::IBasicBlock> sp_block): _sp_block(sp_block){}

        result_type operator()(const IR::blank &x) const{
            assert(false);
        };


        result_type operator()(shared_ptr<IR::IType> x) const{
            assert(false); ///TODO
        }

        result_type operator()(shared_ptr<IR::imodule> x) const {
            assert(false); ///TODO
        }

        result_type operator()(shared_ptr<IR::IClass> x) const {
            assert(false); ///TODO
        }

        result_type operator()(shared_ptr<IR::VariableBase> x) const {
            return x->get(_sp_block);
        }

        result_type operator()(shared_ptr<IR::Function> x) const {
            assert(false);
            //  return x;
        }

        result_type operator()(shared_ptr<IR::IConstant> x) const {
            return x;
        }

        result_type operator()(shared_ptr<IR::IValue> x) const {
            return x;
        }

        result_type operator()(const IR::AddressList &x) const{
            assert(false);
        }


        template <typename T>
        result_type operator()(T x) const{
            assert(false); ///TODO
        }

    private:
        shared_ptr<IR::IBasicBlock> _sp_block;
    };



    class assign_helper{
    public:
        typedef void result_type;

        assign_helper(shared_ptr<IR::IBasicBlock> sp_block): _sp_block(sp_block){}

        result_type operator()(Address value, Address var){
            boost::apply_visitor(*this, value, var);
        }

        ///\throw   invalid_assignment and the position id is undefined.
        result_type operator()(shared_ptr<IR::IValue> spValue, shared_ptr<IR::VariableBase> sp_var) throw(shared_ptr<Issue::invalid_assigment>){
            if (spValue->type() != sp_var->valueType()){
                throw make_shared<Issue::invalid_assigment>();
                ///todo
            }

            sp_var->set(spValue, _sp_block);
        }

        result_type operator()(shared_ptr<IR::IConstant> sp_constant, shared_ptr<IR::VariableBase> sp_var){
            return (*this)(static_pointer_cast<IR::IValue>(sp_constant), sp_var);
        }

        result_type operator()(shared_ptr<IR::VariableBase> sp_rhs, shared_ptr<IR::VariableBase> sp_lhs){
            auto spValue = sp_rhs->get(_sp_block);
            return (*this)(spValue, sp_lhs);
        }

      //  result_type operator()(shared_ptr<IR::IConstant> sp_consta)

        template <typename T1, typename T2>
        result_type operator()(T1 t1, T2 t2){
            assert(false); ///TODO;
        }


    private:
        shared_ptr<IR::IBasicBlock> _sp_block;
    };





}}
