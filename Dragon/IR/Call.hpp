#pragma once

#include <Dragon/IR/class.hpp>
#include <Dragon/IR/utility.hpp>
#include <Dragon/IR/FunctionCollection.hpp>

namespace Dragon{namespace IR{


    class call{
    public:

        typedef Address         result_type;

        call(shared_ptr<IBasicBlock> sp_cur_block, shared_ptr<ISymbolTable> spScope)
        : _spCurrentBlock(sp_cur_block), _sp_domain(spScope){}

        result_type operator()(Address fun, Address args){
            if (args.which() != AddressId<AddressList>::value){
                assert(false);
            }
            auto addrs = boost::get<AddressList>(args);
            std::vector<Address> arg_addrs(addrs.begin(), addrs.end());
            boost::variant<std::vector<Address>>    args_wrapper(arg_addrs);
            return apply_visitor(*this, fun, args_wrapper);
        }


        template <typename T>
        result_type operator()(T t, const std::vector<Address> &arguments){
            assert(false);///TODO
        }

        result_type operator()(GenericAddress gen_addr, const std::vector<Address> &arguments){
            assert(false);
        }
        
        result_type operator()(shared_ptr<FunctionCollection> sp_fc, std::vector<Address> arguments){
            auto arg_types = get_types(arguments);
            auto sp_fun = sp_fc->match(arg_types);
            return (*this)(sp_fun, arguments);
        }

        result_type operator()(shared_ptr<Function> sp_fun, const std::vector<Address> &arguments){
            return make_shared<Function::call>(sp_fun, arguments, _spCurrentBlock);
        }

        result_type operator()(shared_ptr<MemberFunctionAccessor> sp_mem_fun_ac, const std::vector<Address> &arguments){
            return make_shared<MemberFunctionAccessor::call>(sp_mem_fun_ac, arguments, _spCurrentBlock);
        }

    private:
        shared_ptr<IBasicBlock>    _spCurrentBlock;
        shared_ptr<ISymbolTable>         _sp_domain;
    };


}}
