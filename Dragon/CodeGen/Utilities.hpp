#pragma once

#include <Dragon/IR/IR.hpp>

namespace Dragon{namespace CodeGen{

    namespace detail{
        struct AddressType{
            typedef shared_ptr<IType>  result_type;
            
            result_type operator()(shared_ptr<IValue> spValue) const{
                return spValue->type();
            }
            
            result_type operator()(shared_ptr<IConstant> spConstant) const{
                return spConstant->type();
            }
            
            template <typename T>
            result_type operator()(T t) const{
                assert(false);
            }
        };
        
        struct AddressValue{
            typedef shared_ptr<IValue>  result_type;
            
            result_type operator()(shared_ptr<IValue> spValue) const{
                return spValue;
            }
            
            result_type operator()(shared_ptr<IConstant> spConstant) const{
                return dynamic_pointer_cast<IValue>(spConstant);
            }
            
            template <typename T>
            result_type operator()(T t) const{
                assert(false);
            }
        };
    }
    
    shared_ptr<IType> getAddressType(const Address &addr){
        return apply_visitor(detail::AddressType(), addr);
    }
    
    shared_ptr<IValue> getAddressRValue(const Address &addr){
        return apply_visitor(detail::AddressValue(), addr);
    }
    
}}